#include "mqtt_handle.h"
WiFiClient wifiClient;
PubSubClient client(wifiClient);


void callback(char* topic, byte *payload, unsigned int length)
{
    char status[20];
    // Serial.println("-------new message from broker-----");
    // Serial.print("topic: ");
    // Serial.println(topic);
    
    //parse topic 
    char *str = (char *)topic;
    const char s[2] = "/";
    char *token;
    char buf[5][20];
    token = strtok(str, s);
    int i = 0;
    while (token != NULL)
    {
      strcpy(buf[i], token);
      token = strtok(NULL, s);
      i++;
    }
    Serial.println(buf[0]);
    Serial.println(buf[1]);
    Serial.println(buf[2]);
    if (memcmp(buf[0], PT100_BASE_MQTT_TOPIC, strlen(PT100_BASE_MQTT_TOPIC)) == 0)
    {
      ESP_LOGD(TAG,"correct base!");
      if (memcmp(buf[1], PT100_CONFIG_BASE_MQTT_TOPIC, strlen(PT100_CONFIG_BASE_MQTT_TOPIC)) == 0)
      {
        ESP_LOGD(TAG,"correct base config!");
        if (memcmp(buf[2], PT100_CONFIG_TIMESEND_CMD, strlen(PT100_CONFIG_TIMESEND_CMD)) == 0)
        {
          Serial.print("message config: ");
          Serial.write(payload, length);
          Serial.println(); 
          String payload_str;
          for (uint8_t i = 0; i < length; i++)
          {
            payload_str += (char)payload[i];
          }
          Serial.println(payload_str); 
          // parse json string of time send from server 
          StaticJsonDocument<48> doc;
          DeserializationError error = deserializeJson(doc, payload_str);

          if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
          }

          int timeSend = doc["timeSend"]; // 5
          
          ESP_LOGD(TAG,"start configuring time send for esp32 at: %d", timeSend);
          myRam.pt100_data.time_get_data = timeSend;
          // generate response of timesend to publish to server 
          String output;
          String str_timeSend_response;
          str_timeSend_response = "Time send is set to " + String(myRam.pt100_data.time_get_data) + " seconds";
          StaticJsonDocument<80> doc_response_timeSend;
          doc_response_timeSend["response"] = str_timeSend_response;
          serializeJson(doc_response_timeSend, output);
          Serial.println(output);
          send_data_mqtt(PT100_LOGGER_DATA_TOPIC_PUB, output);

          EEPROM.write(0, myRam.pt100_data.time_get_data);
          EEPROM.commit();
        }
        
      }
      if (memcmp(buf[1], PT100_INFO_BASE_MQTT_TOPIC, strlen(PT100_INFO_BASE_MQTT_TOPIC)) == 0)
      {
        ESP_LOGD(TAG,"correct base infor!");
        if (memcmp(buf[2], PT100_STATUS_CMD, strlen(PT100_STATUS_CMD)) == 0)
        {
          Serial.print("message infor: ");
          Serial.write(payload, length);
          Serial.println(); 
          send_data_mqtt(PT100_LOGGER_STATUS_TOPIC_PUB, "pong");
        }
      }
      
    }



}
bool isConnectedBroker;

void connect_to_broker(char *usr, char *pass)
{
  static uint8_t disc_to_sv_counter;
  if (!client.connected())
  {

    Serial.print("Attempting MQTT connection...");
    String clientId = "PT100_Logger";
    clientId += myRam.wifi_config_data.MAC_Address;

    if (client.connect(clientId.c_str(), usr, pass))
    {
      isConnectedBroker = 1;
      disc_to_sv_counter = 0;
      Serial.println("connected");
      client.subscribe(PT100_LOGGER_TOPIC_SUB);

    }
    else
    {
        disc_to_sv_counter++;
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 2 seconds");
    }
    //check how many times device disconnect to server
    if (disc_to_sv_counter > MQTT_SERVER_TIMEOUT_COUNT)
    {
      myRam.working_status.esp_working_modes = ENTER_SLEEP_MODE;
    }
    
  }
}

void reconnectMqtt()
{
  static unsigned long t;
  if (millis() - t > 1000)
  {
    connect_to_broker((char*)myRam.mqtt_config_data.username.c_str(), (char*)myRam.mqtt_config_data.password.c_str());
    // connect_to_broker(MQTT_USERNAME, MQTT_PASSWORD);
    client.loop();
    t = millis();
  }
}



void send_data_mqtt(String topic, String jsonData)
{
  static unsigned long t;
  reconnectMqtt(); // use when device disconnect to server
  const char *data = jsonData.c_str();
  client.publish(topic.c_str(), data); // use to publish data via mqtt
}

void initMqtt()
{
  String output;
  readString_from_spiffs(config_mqtt_protocol, output);
  Serial.println(output);
  StaticJsonDocument<192> doc;

  DeserializationError error = deserializeJson(doc, output);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* host = doc["host"]; // "192.168.11.101"
  const char* port = doc["port"]; // "1883"
  const char* username = doc["username"]; // "Indr_Pt100"
  const char* password = doc["password"]; // "123456789"

  uint16_t _port = atoi(port);
  ESP_LOGD(TAG,"host: %s\nport: %d\nusername: %s\npassword: %s\n",host, _port, username, password);
  myRam.mqtt_config_data.host = host; 
  myRam.mqtt_config_data.port = _port; 
  myRam.mqtt_config_data.username = username; 
  myRam.mqtt_config_data.password = password; 
  client.setServer(myRam.mqtt_config_data.host.c_str(), myRam.mqtt_config_data.port);
  client.setCallback(callback);
  client.setBufferSize(MQTT_MAX_BUFFER);
  connect_to_broker((char*)myRam.mqtt_config_data.username.c_str(), (char*)myRam.mqtt_config_data.password.c_str());
}

void handle_mqtt()
{
  reconnectMqtt();

  static uint32_t t_send = 0;
  static uint8_t send_case = 0;   // 0 = step 1, 1 = step 2

  if (millis() - t_send < 1000) return;
  t_send = millis();

  if (isConnectedBroker != 1) return;
  if (myRam.working_status.esp_working_modes != ACTIVE_MODE) return;

  StaticJsonDocument<512> doc;
  String output;

  switch (send_case)
  {
    case 0:
      // ========== STEP 1 ==========
      doc["step"] = 1;

      doc["Max_load"] = myRam.loadcell_data.capacity_kg;

      doc["Analog Value_High"] = myRam.loadcell_data.Analog_value_high;
      doc["Analog Value_Low"]  = myRam.loadcell_data.Analog_value_low;

      doc["Span Value_High"] = myRam.loadcell_data.Span_value_high;
      doc["Span Value_Low"]  = myRam.loadcell_data.Span_value_low;

      doc["Division"] = myRam.loadcell_data.Division;
      doc["Decimal Point"] = myRam.loadcell_data.Decimal_point;

      doc["Current Weight High"] = myRam.loadcell_data.Current_weight_high;
      doc["Current Weight Low"]  = myRam.loadcell_data.Current_weight_low;

      doc["Tare Weight High"] = myRam.loadcell_data.Tare_weight_high;
      doc["Tare Weight Low"]  = myRam.loadcell_data.Tare_weight_low;

      doc["Measured Weight High"] = myRam.loadcell_data.Measured_weight_High;
      doc["Measured Weight Low"]  = myRam.loadcell_data.Measured_weight_Low;

      send_case = 1;   // lần sau gửi step 2
      break;

    case 1:
      // ========== STEP 2 ==========
      doc["step"] = 2;

      doc["Digital Input"] = myRam.loadcell_data.Digital_input;
      doc["Lamp"]          = myRam.loadcell_data.Lamp;
      doc["Error"]         = myRam.loadcell_data.Error;

      doc["Weighing Mode"] = myRam.loadcell_data.Weighing_mode;
      doc["Weighing Step"] = myRam.loadcell_data.Weighing_step;

      doc["Grand total Count"]  = myRam.loadcell_data.Grand_Count;
      doc["Grand total Weight"] = myRam.loadcell_data.Grand_Weight;

      doc["Date"] = myRam.loadcell_data.Date;
      doc["Time"] = myRam.loadcell_data.Time;

      doc["Key_value"] = myRam.loadcell_data.Key_value;
      doc["Relay_output"] = myRam.loadcell_data.Relay_output;

      doc["SP1"] = myRam.loadcell_data.Current_SP1;
      doc["SP2"] = myRam.loadcell_data.Current_SP2;
      doc["SP3"] = myRam.loadcell_data.Current_SP3;
      doc["SP4"] = myRam.loadcell_data.Current_SP4;

      doc["ffo_SP1"] = myRam.loadcell_data.Free_fall_SP1;
      doc["ffo_SP2"] = myRam.loadcell_data.Free_fall_SP2;
      doc["ffo_SP3"] = myRam.loadcell_data.Free_fall_SP3;
      doc["ffo_SP4"] = myRam.loadcell_data.Free_fall_SP4;

      send_case = 0;   // quay lại step 1
      break;
  }

  serializeJson(doc, output);
  send_data_mqtt(PT100_LOGGER_TB_PUBLISH, output);
  Serial.println(output);
}

  
  
  
  // static uint32_t t_send_data_to_sv;
  // if (millis() - t_send_data_to_sv > myRam.pt100_data.time_get_data*1000)
  // {
  // t_send_data_to_sv = millis();
  // }
