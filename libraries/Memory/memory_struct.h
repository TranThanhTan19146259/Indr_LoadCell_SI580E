#pragma once
#include <Arduino.h>

typedef struct PT100_data_t
{
    float temp;
    uint16_t resistor;
    uint8_t time_get_data;
} PT100_data;

typedef struct Wifi_config_data
{
    bool wifi_ap_sta; //0: AP || 1: STA 
    bool is_wifi_connected;
    bool reset_wifi;
    String ssid_ap;
    String password_ap;
    String ssid_sta;
    String password_sta;
    String STA_IP;
    String AP_IP;
    String MAC_Address;
} Wifi_config_data_t;

typedef struct Mqtt_config_data
{
    String host;
    uint16_t port;
    String username;
    String password;
} Mqtt_config_data_t;


typedef struct Loadcell_SI580E
{
    // Address 1-2: Capacity (32-bit)
    float capacity_kg;
    
    // Address 4-5: Analog Value (32-bit split into high/low)
    float Analog_value_high;
    float Analog_value_low;
    
    // Address 6-7: Span Value (32-bit split into high/low)
    float Span_value_high;
    float Span_value_low;
    
    // Address 9: Division (8-bit)
    uint8_t Division;
    
    // Address 10: Decimal Point (8-bit)
    uint8_t Decimal_point;
    
    // // Address 11-12: Current Weight (32-bit)
    float Current_weight_high;
    float Current_weight_low;

    // // Address 13-14: Tare Weight (32-bit)
    float Tare_weight_high;
    float Tare_weight_low;
    
    // Address 15-16: Measured Weight (32-bit)
    float Measured_weight_High;
    float Measured_weight_Low;
    // // Address 17-18: Digital Input (16-bit)
    uint16_t Digital_input;
    
    // Address 19-20: Lamp (16-bit)
    uint16_t Lamp;
    
    // Address 21-22: Error (16-bit)
    uint16_t Error;
    
    // Address 23: Weighing Mode (8-bit)
    uint8_t Weighing_mode;
    
    // Address 24: Weighing Step (8-bit)
    uint8_t Weighing_step;
    
    // Address 33-34: Grand Total Count (32-bit)
    float Grand_Count;
    
    // // Address 35-36: Grand Total Weight (32-bit)
    float Grand_Weight;
    
    // Address 437-438: Date (32-bit)
    float Date;
    
    // Address 439-440: Time (32-bit)
    float Time;
    
    // Address 441: Key Value (8-bit)
    uint8_t Key_value;
    
    // Address 443: Relay Output (8-bit)
    uint8_t Relay_output;
    
    // Address 445-446: Current SP1 (16-bit)
    uint16_t Current_SP1;
    
    // Address 447-448: Current SP2 (16-bit)
    uint16_t Current_SP2;
    
    // Address 449-450: Current SP3 (16-bit)
    uint16_t Current_SP3;
    
    // Address 451-452: Current SP4 (16-bit)
    uint16_t Current_SP4;
    
    // Address 461-462: Free Fall of SP1 (16-bit)
    uint16_t Free_fall_SP1;
    
    // Address 463-464: Free Fall of SP2 (16-bit)
    uint16_t Free_fall_SP2;
    
    // Address 465-466: Free Fall of SP3 (16-bit)
    uint16_t Free_fall_SP3;
    
    // Address 467-468: Free Fall of SP4 (16-bit)
    uint16_t Free_fall_SP4;
    
} Loadcell_SI580E_t;


typedef enum Esp_working_modes{
    ENTER_SLEEP_MODE,
    SLEEP,
    ACTIVE_MODE,
    CONFIG_MODE,
    SEND_LASTWILL_MSG
} Esp_working_modes_e;

typedef struct Esp_working_status
{
    Esp_working_modes_e esp_working_modes;
} Esp_working_status_t;

