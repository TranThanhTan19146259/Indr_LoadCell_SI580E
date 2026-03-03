#pragma once

// #include "network_handle.h"
#include "Ram.h"
#include "soc/rtc_wdt.h"
#include "at_cmd_handle.h"
#include <Arduino.h>
#include <SerialCommands.h>
#include "rfid_handle.h"
// #include "loadcel_si580e.h"

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
#define RNOMINAL  100.0
// #define CONFIG_BTN 13
#define CONFIG_BTN 26
#define ADC_PIN 32

#define SIMULATE_DATA

void init_hw();

void handle_hw();