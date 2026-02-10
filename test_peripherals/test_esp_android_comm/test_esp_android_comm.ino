#include <Arduino.h>
#include <SerialCommands.h>
#define DUMMY_DATA_MAP "MAP"
#define DUMMY_DATA_TEMP_PV "TEMP_PV"
#define DUMMY_DATA_TEMP_SP "TEMP_SP"
#define DUMMY_DATA_TEMPSENSOR "TEMPSENSOR"
#define DUMMY_DATA_SOLID_RELAY "RELAY"
#define DUMMY_DATA_PRESSURE_PUMP "PUMP"
#define DUMMY_DATA_HEAT "HEAT"
#define DUMMY_DATA_BRIGHTNESS "BRIGHTNESS"
#define DUMMY_DATA_MENU "MENU"

char serial_command_buffer_[100];
SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

//This is the default handler, and gets called when no other command matches. 
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
	sender->GetSerial()->print("Unrecognized command [");
	sender->GetSerial()->print(cmd);
	sender->GetSerial()->println("]");
}

// void cmd_add_new_device(SerialCommands* sender)
// {
//     char* pair_key_str = sender->Next();
// 	if (pair_key_str == NULL)
// 	{
// 		sender->GetSerial()->println("ERROR ADD NEW DEVICE");
// 		return;
// 	}
//     myRam.CIT_Mqtt.cit_mqtt_server_infor.pairKey = pair_key_str;
//     myRam.CIT_Mqtt.add_new_device = 1;
//     sender->GetSerial()->println(myRam.CIT_Mqtt.cit_mqtt_server_infor.pairKey);
// }

////////////////////////////////////////////////////////Reset toàn bộ câu hình máy/////////////////////////////////////////
// void cmd_factory_rerset(SerialCommands* sender)
// {
// 	myRam.Coffee_Machine.factory_reset = 1;
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// void cmd_fake_data(SerialCommands* sender)
// {
// 	const char* dummy_data_type = sender->Next();
// 	const char* param1 = sender->Next();
// 	const char* param2 = sender->Next();
// 	const char* param3 = sender->Next();
// 	const char* param4 = sender->Next();
// 	const char* param5 = sender->Next();
// 	const char* param6 = sender->Next();
// 	const char* param7 = sender->Next();
// 	const char* param8 = sender->Next();
// 	if (memcmp(dummy_data_type, DUMMY_DATA_MAP, strlen(DUMMY_DATA_MAP)) == 0)
// 	{
// 		// myRam.Coffee_Machine.GPS.lat.degree = atof(param1);
// 		// myRam.Coffee_Machine.GPS.lat.dir = atoi(param2);
// 		// myRam.Coffee_Machine.GPS.lng.degree = atof(param3);
// 		// myRam.Coffee_Machine.GPS.lng.dir = atoi(param4);
// 		// char debug_msg[50];
// 		// char lat_dir, lng_dir;
// 		// if (myRam.Coffee_Machine.GPS.lat.dir == 1) lat_dir = 'S';
// 		// else lat_dir = 'N';
// 		// if (myRam.Coffee_Machine.GPS.lng.dir == 1) lng_dir = 'W';
// 		// else lng_dir = 'E';
// 		// sprintf(debug_msg, "lat: %fdeg%c lng: %fdeg%c", myRam.Coffee_Machine.GPS.lat.degree
// 		// 											  , lat_dir
// 		// 											  , myRam.Coffee_Machine.GPS.lng.degree
// 		// 											  , lng_dir
// 		// );
// 		// Serial.println(debug_msg);
// 	}
// 	if (memcmp(dummy_data_type, DUMMY_DATA_TEMP_PV, strlen(DUMMY_DATA_TEMP_PV)) == 0)
// 	{
// 		myRam.Coffee_Machine.temp[0].temp_pv = atof(param1);
// 		char debug_msg[50];
// 		sprintf(debug_msg, "Temp_PV: %f", myRam.Coffee_Machine.temp[0].temp_pv);
// 		Serial.println(debug_msg);
// 	}	
// 	if (memcmp(dummy_data_type, DUMMY_DATA_TEMP_SP, strlen(DUMMY_DATA_TEMP_SP)) == 0)
// 	{
// 		myRam.Coffee_Machine.temp[0].temp_sp = atof(param1);
// 		char debug_msg[50];
// 		sprintf(debug_msg, "Temp_SP: %f", myRam.Coffee_Machine.temp[0].temp_sp);
// 		Serial.println(debug_msg);
// 	}
// 	if (memcmp(dummy_data_type, DUMMY_DATA_TEMPSENSOR, strlen(DUMMY_DATA_TEMPSENSOR)) == 0)
// 	{
// 		myRam.Coffee_Machine.tempsensor.status = atoi(param1);
// 		char debug_msg[50];
// 		sprintf(debug_msg, "Temp Sensor Status: %d", myRam.Coffee_Machine.tempsensor.status);
// 		Serial.println(debug_msg);
// 	}
// 	if (memcmp(dummy_data_type, DUMMY_DATA_SOLID_RELAY, strlen(DUMMY_DATA_SOLID_RELAY)) == 0)
// 	{
// 		myRam.Coffee_Machine.solid_relay.status = atoi(param1);
// 		char debug_msg[50];
// 		sprintf(debug_msg, "Solid relay Status: %d", myRam.Coffee_Machine.solid_relay.status);
// 		Serial.println(debug_msg);
// 	}
// 	if (memcmp(dummy_data_type, DUMMY_DATA_PRESSURE_PUMP, strlen(DUMMY_DATA_PRESSURE_PUMP)) == 0)
// 	{
// 		myRam.Coffee_Machine.pressure_pump.status = atoi(param1);
// 		myRam.Coffee_Machine.pressure_pump.value = atoi(param2);
// 		char debug_msg[50];
// 		sprintf(debug_msg, "Pressure pump Status: %d Pressure pump Value: %d", myRam.Coffee_Machine.pressure_pump.status
// 																			 , myRam.Coffee_Machine.pressure_pump.value
// 		);
// 		Serial.println(debug_msg);
// 	}
// 	if (memcmp(dummy_data_type, DUMMY_DATA_HEAT, strlen(DUMMY_DATA_HEAT)) == 0)
// 	{
// 		myRam.Coffee_Machine.heat.status = atoi(param1);
// 		char debug_msg[50];
// 		sprintf(debug_msg, "Heat Status: %d", myRam.Coffee_Machine.heat.status);
// 		Serial.println(debug_msg);
// 	}
// 	if (memcmp(dummy_data_type, DUMMY_DATA_BRIGHTNESS, strlen(DUMMY_DATA_BRIGHTNESS)) == 0)
// 	{
// 		myRam.Coffee_Machine.brightness.value = atoi(param1);
// 		char debug_msg[50];
// 		sprintf(debug_msg, "Brightness: %d", myRam.Coffee_Machine.brightness.value);
// 		Serial.println(debug_msg);
// 	}
// 	if (memcmp(dummy_data_type, DUMMY_DATA_MENU, strlen(DUMMY_DATA_MENU)) == 0)
// 	{
// 		Serial.println("SJDASDADASDADADASD");
// 		uint8_t course_debug;
// 		course_debug = atoi(param1);
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[0] = course_debug;
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[1] = atoi(param2);
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[2] = atoi(param3);
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[3] = atoi(param4);
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[4] = atoi(param5);
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[5] = atoi(param6);
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[6] = atoi(param7);
// 		myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[7] = atoi(param8);
// 		// char s[] = "28tech c programming   blog   python  ";
// 		// char *param_ingred[10];
// 		// char *myPtr = strtok((char*)param2, ",");
// 		// uint8_t count = 0;
// 		// while (myPtr != NULL) {
// 		// 	// printf("%s\n", myPtr);
// 		// 	// param_ingred[count] = myPtr;
// 		// 	myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[count+1] = atoi(myPtr);
// 		// 	// atoi(myPtr);
// 		// 	myPtr = strtok(NULL, ",");
// 		// 	// memcpy(param_ingred[count], myPtr, strlen(myPtr));
// 		// 	count++;
// 		// }
// 		// Serial.print(param_ingred[0]);

// 		for (uint8_t i = 0; i < 8; i++)
// 		{
// 			Serial.print(myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[i]);
// 			// Serial.print(param_ingred[i]);
// 			Serial.print("\t");
// 		}
// 		Serial.println("");
// 		myRam.Coffee_Machine.coffee_course.isNewCourse = 1;
		
		
// 		// myRam.Coffee_Machine.coffee_course.raw_coffee_course_array[6] = atoi(param7);
// 	}

// }


// void cmd_read_device_info(SerialCommands* sender)
// {
	
// 	char debug_msg[150];
// 	// char lat_dir, lng_dir;
// 	// if (myRam.Coffee_Machine.GPS.lat.dir == 1) lat_dir = 'S';
// 	// else lat_dir = 'N';
// 	// if (myRam.Coffee_Machine.GPS.lng.dir == 1) lng_dir = 'W';
// 	// else lng_dir = 'E';
// 	sprintf(debug_msg, "power: %d\ntemp - SP:%f - PV:%f\ndeg%c %fdeg%c\nsolid_relay_status: %d\npump: %d - %d\nheat_status: %d\nbrightness: %d"
// 					 , myRam.Coffee_Machine.power
// 					 , myRam.Coffee_Machine.temp[0].temp_sp, myRam.Coffee_Machine.temp[0].temp_pv
// 					//  , myRam.Coffee_Machine.GPS.lat.degree, lat_dir, myRam.Coffee_Machine.GPS.lng.degree, lng_dir
// 					 , myRam.Coffee_Machine.solid_relay.status
// 					 , myRam.Coffee_Machine.pressure_pump.status, myRam.Coffee_Machine.pressure_pump.value
// 					 , myRam.Coffee_Machine.heat.status
// 					 , myRam.Coffee_Machine.brightness.value 
// 	);	
// 	Serial.println(debug_msg);
// }

// void cmd_read_rom(SerialCommands* sender)
// {
// 	for (uint8_t i = 0; i < 25; i++)
// 	{
// 		Serial.print(EEPROM.read(i));
// 		Serial.print(" ");
// 	}
// 	Serial.println();
// 	for (uint8_t i = 25; i < 50; i++)
// 	{
// 		Serial.print(EEPROM.read(i));
// 		Serial.print(" ");
// 	}
// 	Serial.println();
// }

// void cmd_clear_rom(SerialCommands* sender)
// {
// 	for (uint8_t i = 0; i < 60; i++)
//     {
//         EEPROM.write(i, 0);
//         EEPROM.commit();
//     }
// }

void cmd_help(SerialCommands* sender)
{
	Serial.println("<----------------------------------------------------------------->");
	Serial.println("CHANGE_PAIR_KEY <pair_key>");
	Serial.println("ADD_DEVICE <pair_key>");
	Serial.println("DUMMY	<MAP>			<LAT_DEG> <LAT_DIR> <LNG_DEG> <LNG_DIR>");
	Serial.println("||		<TEMP_PV>		<TEMP_PV>");
	Serial.println("||		<TEMP_SP>		<TEMP_SP>");
	Serial.println("||		<TEMPSENSOR>		<STATUS>");
	Serial.println("||		<RELAY>			<STATUS>");
	Serial.println("||		<PUMP>			<STATUS> <VALUE>");
	Serial.println("||		<HEAT>			<STATUS>");
	Serial.println("||		<BRIGHTNESS>		<VALUE>");
	Serial.println("FACTORY_RESET");
	Serial.println("READ_SV_INFO");
	Serial.println("READ_DEV_INFO");
	Serial.println("<----------------------------------------------------------------->");
}

// void cmd_dummy_modbus(SerialCommands* sender)
// {
// 	char* modbus_addr_str = sender->Next();
// 	char* data_addr_str = sender->Next();
// 	int modbus_addr = atoi(modbus_addr_str);
// 	int data_addr = atoi(data_addr_str);
// 	myRam.Modbus_properties.buf_data_modbus[modbus_addr] = data_addr; 
// 	Serial.println("SEND MODBUS");
// 	myRam.Modbus_properties.read_write_modbus = 1;
  
// }

// SerialCommand cmd_dummy_modbus_("MODBUS", cmd_dummy_modbus);
// SerialCommand cmd_add_new_device_key_("ADD_DEVICE", cmd_add_new_device);
// SerialCommand cmd_factory_rerset_("FACTORY_RESET", cmd_factory_rerset);
// SerialCommand cmd_fake_data_("DUMMY", cmd_fake_data);
// SerialCommand cmd_read_device_info_("READ_DEV_INFO", cmd_read_device_info);
// SerialCommand cmd_read_rom_("READ_ROM", cmd_read_rom);
// SerialCommand cmd_clear_rom_("CLEAR_ROM", cmd_clear_rom);
SerialCommand cmd_help_("HELP", cmd_help);

void setup()
{
	Serial.begin(115200);
  serial_commands_.SetDefaultHandler(cmd_unrecognized);
	serial_commands_.AddCommand(&cmd_help_);
	// serial_commands_.AddCommand(&cmd_add_new_device_key_);
	// serial_commands_.AddCommand(&cmd_factory_rerset_);
	// serial_commands_.AddCommand(&cmd_fake_data_);
	// serial_commands_.AddCommand(&cmd_read_device_info_);
	// serial_commands_.AddCommand(&cmd_read_rom_);
	// serial_commands_.AddCommand(&cmd_clear_rom_);
	// serial_commands_.AddCommand(&cmd_dummy_modbus_);
}

void loop()
{
	serial_commands_.ReadSerial();
}
