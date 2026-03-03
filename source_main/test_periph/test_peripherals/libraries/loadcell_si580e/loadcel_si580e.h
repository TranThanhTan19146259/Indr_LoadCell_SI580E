#pragma once
#include "Ram.h"
#include <Arduino.h>
#include <SerialCommands.h>
#include "ModbusRtu.h"

#ifndef LOADCEL_SI580E_H
#define LOADCEL_SI580E_H
// ================= UART & MODBUS CONFIG =================
#define RXD2 35
#define TXD2 27
#define USE_FAKE_LOADCELL  0 

// ================= EXTERNAL DECLARATIONS =================
extern HardwareSerial Serial_Loadcell;
extern Modbus master;
extern uint16_t au16data_main[40];
extern modbus_t telegram;
extern uint8_t u8state;
extern unsigned long u32wait;
extern uint8_t device_id;
// ================= FUNCTION DECLARATIONS =================
void init_loadcell();
void parse_loadcell_main();
void handle_loadcell();
void fake_loadcell_simulator();
void print_loadcell_data();

// Serial command handlers
void cmd_unrecognized(SerialCommands* sender, const char* cmd);
void cmd_read_loadcell(SerialCommands* sender);
void cmd_help(SerialCommands* sender);

#endif