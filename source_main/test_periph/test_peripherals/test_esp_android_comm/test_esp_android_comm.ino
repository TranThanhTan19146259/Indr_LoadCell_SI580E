#include <Arduino.h>
#include <SerialCommands.h>
#include "loadcel_si580e.h"

// ================= AUTO-SEND CONTROL =================
bool enableLoadcellSend = false;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // 1 second

// ================= CONNECTION STATE =================
bool isConnected = false;
uint8_t connectedDeviceId = 0;

// ================= SERIAL COMMAND =================
char serial_command_buffer_[100];
SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

// ================= COMMAND HANDLERS =================
void cmd_connect(SerialCommands* sender) {
    uint8_t device_id = 1; // ID mặc định

    connectedDeviceId = device_id;
    myRam.loadcell_data.device_id = device_id;
    telegram.u8id = device_id;

    isConnected = true;

    Serial.println("CONNECTED");
}


void cmd_disconnect(SerialCommands* sender)
{
    isConnected = false;
    enableLoadcellSend = false;
    connectedDeviceId = 0;
    
    Serial.println("DISCONNECTED");
}

void cmd_start_loadcell(SerialCommands* sender)
{
    if (!isConnected)
    {
        Serial.println("ERROR: Not connected. Use CONNECT <device_id> first");
        return;
    }
    
    enableLoadcellSend = true;
    Serial.println("Auto-send ENABLED");
}

void cmd_stop_loadcell(SerialCommands* sender)
{
    enableLoadcellSend = false;
    Serial.println("Auto-send DISABLED");
}

void cmd_status(SerialCommands* sender)
{
    Serial.println("\n===== SYSTEM STATUS =====");

    if (isConnected) {
        Serial.println("CONNECTED");
    } else {
        Serial.println("DISCONNECTED");
    }

    // Serial.print("Auto-send: ");
    // Serial.println(enableLoadcellSend ? "ENABLED" : "DISABLED");

    // Serial.println("=========================");
}
void cmd_raw_data(SerialCommands* sender)
{
    handle_loadcell();
    print_loadcell_data();
}

// Register commands
SerialCommand cmdConnect("CONNECT", cmd_connect);
SerialCommand cmdDisconnect("DISCONNECT", cmd_disconnect);
SerialCommand cmdStart("START", cmd_start_loadcell);
SerialCommand cmdRaw("RAW_DATA", cmd_raw_data);
SerialCommand cmdStop("STOP", cmd_stop_loadcell);
SerialCommand cmdRead("READ", cmd_read_loadcell);
SerialCommand cmdStatus("STATUS", cmd_status);
SerialCommand cmdHelp("HELP", cmd_help);

// ========================================================
//                         SETUP
// ========================================================
void setup()
{
    Serial.begin(115200);

    init_loadcell();

    serial_commands_.SetDefaultHandler(cmd_unrecognized);
    serial_commands_.AddCommand(&cmdConnect);
    serial_commands_.AddCommand(&cmdDisconnect);
    serial_commands_.AddCommand(&cmdStart);
    serial_commands_.AddCommand(&cmdStop);
    serial_commands_.AddCommand(&cmdRead);
    serial_commands_.AddCommand(&cmdStatus);
    serial_commands_.AddCommand(&cmdHelp);
    serial_commands_.AddCommand(&cmdRaw);

}

// ========================================================
//                          LOOP
// ========================================================
void loop()
{
    serial_commands_.ReadSerial();

    // LUÔN cập nhật dữ liệu loadcell
    handle_loadcell();

    // Chỉ auto-send khi đã CONNECT
    if (isConnected && enableLoadcellSend)
    {
        unsigned long now = millis();
        if (now - lastSendTime >= sendInterval)
        {
            lastSendTime = now;
            print_loadcell_data();
        }
    }
}
