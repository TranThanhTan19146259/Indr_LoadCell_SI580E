// #include <Arduino.h>
// #include <SerialCommands.h>
// #include "loadcel_si580e.h"

// // ================= UART & MODBUS CONFIG =================
// #define RXD2 33
// #define TXD2 27
// #define USE_FAKE_LOADCELL  0 
// HardwareSerial Serial_Loadcell(2);
// Modbus master(0, Serial_Loadcell, 0);

// // ================= MODBUS BUFFERS =================
// uint16_t au16data_main[40];   
// modbus_t telegram;
// // ================= STATE MACHINE =================
// uint8_t u8state = 0;
// unsigned long u32wait = 0;

// // ================= SERIAL COMMAND =================
// char serial_command_buffer_[100];
// SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

// // ========================================================
// //                 INIT LOADCELL MODBUS
// // ========================================================
// void init_loadcell()
// {
//     Serial_Loadcell.begin(9600, SERIAL_8N1, RXD2, TXD2);

//     master.start();
//     master.setTimeOut(2000);

//     // ---- Telegram 0: Read Main Registers ----
//     telegram.u8id = 1;
//     telegram.u8fct = 3;
//     telegram.u16RegAdd = 0;
//     telegram.u16CoilsNo = 40;
//     telegram.au16reg = au16data_main;

//     u32wait = millis() + 500;
//     u8state = 0;
// }

// // ========================================================
// //               READ & PARSE LOADCELL DATA
// // ========================================================
// void parse_loadcell_main()
// {
//     uint32_t raw_capacity =
//         ((uint32_t)au16data_main[0] << 16) | au16data_main[1];

//     myRam.loadcell_data.capacity_kg = raw_capacity / 10.0;

//     // myRam.loadcell_data.Analog_value_high = au16data_main[4];
//     // myRam.loadcell_data.Analog_value_low  = au16data_main[5];

//     // myRam.loadcell_data.Span_value_high   = au16data_main[6];
//     // myRam.loadcell_data.Span_value_low    = au16data_main[7];

//     // myRam.loadcell_data.Division          = au16data_main[9];
//     // myRam.loadcell_data.Decimal_point     = au16data_main[10];

//     // myRam.loadcell_data.Current_weight_high = au16data_main[11];
//     // myRam.loadcell_data.Current_weight_low  = au16data_main[12];

//     // myRam.loadcell_data.Weighing_mode = au16data_main[23];
//     // myRam.loadcell_data.Weighing_step = au16data_main[24];
// }

// // void parse_loadcell_time()
// // {
// //     uint32_t Date =
// //         ((uint32_t)au16data_time[0] << 16) | au16data_time[1];

// //     uint32_t Time =
// //         ((uint32_t)au16data_time[2] << 16) | au16data_time[3];

// //     myRam.loadcell_data.Date = Date;
// //     myRam.loadcell_data.Time = Time;
// // }

// // ========================================================
// //                  GET CURRENT WEIGHT
// // ========================================================
// // float get_loadcell_weight()
// // {
// //     uint32_t raw_weight =
// //         ((uint32_t)myRam.loadcell_data.Current_weight_high << 16) |
// //          myRam.loadcell_data.Current_weight_low;

// //     return raw_weight / 10.0;
// // }

// // ========================================================
// //            HANDLE MODBUS STATE MACHINE (NON BLOCKING)
// // ========================================================
// void handle_loadcell()
// {

// #if USE_FAKE_LOADCELL

//     fake_loadcell_simulator();
//     delay(50); 

// #else

//     switch (u8state)
//     {
//         case 0:
//             if (millis() > u32wait) u8state++;
//             break;

//         case 1:
//             master.query(telegram);
//             u8state++;
//             break;

//         case 2:
//             master.poll();

//             if (master.getState() == COM_IDLE)
//             {
//                 parse_loadcell_main();  // ← Changed from parse_loadcell_capacity()
//                 u8state = 0;
//                 u32wait = millis() + 300;
//             }
//             break;
//     }

// #endif
// }

// void fake_loadcell_simulator()
// {
//     static float fake_weight = 0;
//     static int direction = 1;

//     // tăng / giảm trọng lượng
//     fake_weight += direction * 0.2;

//     // đổi chiều nếu đạt ngưỡng
//     if (fake_weight > 50) direction = -1;
//     if (fake_weight < 0)  direction = 1;

//     // thêm nhiễu nhỏ
//     float noise = random(-5, 5) / 100.0;

//     myRam.loadcell_data.capacity_kg = fake_weight + noise;
// }

// // ========================================================
// //                  PRINT LOADCELL DATA
// // ========================================================
// void print_loadcell_data()
// {
//     Serial.println("\n======= LOADCELL DATA =======");

//     Serial.print("Capacity (kg): ");
//     Serial.println(myRam.loadcell_data.capacity_kg);

//     // Serial.print("Current Weight (kg): ");
//     // Serial.println(get_loadcell_weight());

//     // Serial.print("Division: ");
//     // Serial.println(myRam.loadcell_data.Division);

//     // Serial.print("Decimal Point: ");
//     // Serial.println(myRam.loadcell_data.Decimal_point);

//     // Serial.print("Weighing Mode: ");
//     // Serial.println(myRam.loadcell_data.Weighing_mode);

//     // Serial.print("Weighing Step: ");
//     // Serial.println(myRam.loadcell_data.Weighing_step);

//     // Serial.print("Date: ");
//     // Serial.println(myRam.loadcell_data.Date);

//     // Serial.print("Time: ");
//     // Serial.println(myRam.loadcell_data.Time);

//     Serial.println("==============================");
// }

// // ========================================================
// //               SERIAL COMMAND HANDLER
// // ========================================================
// void cmd_unrecognized(SerialCommands* sender, const char* cmd)
// {
//     sender->GetSerial()->print("Unrecognized command: ");
//     sender->GetSerial()->println(cmd);
// }

// void cmd_read_loadcell(SerialCommands* sender)
// {
//     print_loadcell_data();
// }

// void cmd_help(SerialCommands* sender)
// {
//     Serial.println("\n------ COMMAND LIST ------");
//     Serial.println("HELP");
//     Serial.println("READ_LOADCELL");
//     Serial.println("---------------------------");
// }

// // Register commands
// SerialCommand cmd_read_loadcell_("READ_LOADCELL", cmd_read_loadcell);
// SerialCommand cmd_help_("HELP", cmd_help);

// // ========================================================
// //                         SETUP
// // ========================================================
// void setup()
// {
//     Serial.begin(115200);

//     init_loadcell();

//     serial_commands_.SetDefaultHandler(cmd_unrecognized);
//     serial_commands_.AddCommand(&cmd_help_);
//     serial_commands_.AddCommand(&cmd_read_loadcell_);
// }

// // ========================================================
// //                          LOOP
// // ========================================================
// void loop()
// {
//     handle_loadcell();        // Read Modbus loadcell
//     serial_commands_.ReadSerial(); // Read Serial CLI
// }


#include <Arduino.h>
#include "loadcel_si580e.h"


HardwareSerial Serial_Loadcell(2);
Modbus master(0, Serial_Loadcell, 0);
uint16_t au16data_main[40];   
modbus_t telegram;
uint8_t u8state = 0;
unsigned long u32wait = 0;

void init_loadcell()
{
    Serial_Loadcell.begin(9600, SERIAL_8N1, RXD2, TXD2);

    master.start();
    master.setTimeOut(2000);

    telegram.u8id = 1;
    telegram.u8fct = 3;
    telegram.u16RegAdd = 0;
    telegram.u16CoilsNo = 40;
    telegram.au16reg = au16data_main;

    u32wait = millis() + 500;
    u8state = 0;
}

void parse_loadcell_main()
{
    // Đọc Capacity (tải trọng tối đa)
    uint32_t raw_capacity =
        ((uint32_t)au16data_main[0] << 16) | au16data_main[1];
    myRam.loadcell_data.capacity_kg = raw_capacity / 10.0;

    // Đọc Current Weight - CHỈ DÙNG 1 REGISTER VÀ CHIA 100
    myRam.loadcell_data.current_weight_kg = au16data_main[11] / 100.0;  // ← CHIA 100, KHÔNG PHẢI 10!

     myRam.loadcell_data.Error = au16data_main[21];
    // Set device_id
    myRam.loadcell_data.device_id = 1;
}

void handle_loadcell()
{
#if USE_FAKE_LOADCELL
    fake_loadcell_simulator();
    delay(50); 
#else
    switch (u8state)
    {
        case 0:
            if (millis() > u32wait) u8state++;
            break;

        case 1:
            master.query(telegram);
            u8state++;
            break;

        case 2:
            master.poll();

            if (master.getState() == COM_IDLE)
            {
                parse_loadcell_main();
                u8state = 0;
                u32wait = millis() + 50;
            }
            break;
    }
#endif
}

void fake_loadcell_simulator()
{
    static float fake_weight = 0;
    static int direction = 1;

    fake_weight += direction * 0.2;

    if (fake_weight > 50) direction = -1;
    if (fake_weight < 0)  direction = 1;

    float noise = random(-5, 5) / 100.0;

    myRam.loadcell_data.capacity_kg = fake_weight + noise;
}

// ========================================================
//                  PRINT LOADCELL DATA
// ========================================================
void print_loadcell_data()
{
    char buffer[64];
    char rfid_code_buf[20];
    sprintf(rfid_code_buf, "%02X:%02X:%02X:%02X", myRam.rfid.nuidPICC[0],
                                          myRam.rfid.nuidPICC[1],
                                          myRam.rfid.nuidPICC[2],
                                          myRam.rfid.nuidPICC[3]
    );
    // rfid_code = "AABBCCDD";
    sprintf(buffer, "RAW_DATA %d %.2f %s\r\n", 
            myRam.loadcell_data.device_id,
            myRam.loadcell_data.current_weight_kg,
            rfid_code_buf);
    Serial.print(buffer);
}
// ========================================================
//               SERIAL COMMAND HANDLERS
// ========================================================
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
    sender->GetSerial()->print("Unrecognized command: ");
    sender->GetSerial()->println(cmd);
}

void cmd_read_loadcell(SerialCommands* sender)  // ← Should appear ONLY ONCE
{
    print_loadcell_data();
}

void cmd_help(SerialCommands* sender)
{
    Serial.println("\n========== COMMAND LIST ==========");
    Serial.println("CONNECT <id>   - Connect to device (e.g., CONNECT 1)");
    Serial.println("DISCONNECT     - Disconnect from device");
    Serial.println("START          - Auto-send data every 1s");
    Serial.println("STOP           - Stop auto-send");
    Serial.println("READ           - Read once");
    Serial.println("STATUS         - Show connection status");
    Serial.println("HELP           - Show this help");
    Serial.println("==================================");
}