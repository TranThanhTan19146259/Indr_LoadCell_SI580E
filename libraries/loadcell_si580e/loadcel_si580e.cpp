#include "loadcel_si580e.h"

uint16_t au16data_main[40];   // telegram 0
uint16_t au16data_time[40];    // telegram 1

uint8_t u8state;
uint8_t u8query;

#define RXD2 33
#define TXD2 27

HardwareSerial Serial_PT100(2);
Modbus master(0, Serial_PT100, 0);

modbus_t telegram[2];
unsigned long u32wait;


void init_loadcell()
{
  Serial_PT100.begin(9600, SERIAL_8N1, RXD2, TXD2);

  master.start();
  master.setTimeOut(2000);

  // ===== telegram 0: READ MAIN DATA =====
  telegram[0].u8id = 1;
  telegram[0].u8fct = 3;
  telegram[0].u16RegAdd = 0;
  telegram[0].u16CoilsNo = 40;
  telegram[0].au16reg = au16data_main;

  // ===== telegram 1: READ DATE & TIME =====
  telegram[1].u8id = 1;
  telegram[1].u8fct = 3;
  telegram[1].u16RegAdd = 436;
  telegram[1].u16CoilsNo = 4;
  telegram[1].au16reg = au16data_time;

  u32wait = millis() + 500;
  u8state = 0;
  u8query = 0;
}

void handle_loadcell()
{
  switch (u8state)
  {
    case 0: // WAIT
      if (millis() > u32wait) u8state++;
      break;

    case 1: // SEND QUERY
      master.query(telegram[u8query]);
      u8state++;
      break;

    case 2: // POLL
      master.poll();

      if (master.getState() == COM_IDLE)
      {
        // ===== telegram 0: MAIN DATA =====
        if (u8query == 0)
        {
          uint32_t raw_capacity =
            ((uint32_t)au16data_main[0] << 16) | au16data_main[1];
          myRam.loadcell_data.capacity_kg = raw_capacity / 10.0;
          uint32_t Digital_input =
            ((uint32_t)au16data_main[17] << 16) | au16data_main[18];

          uint32_t Lamp =
            ((uint32_t)au16data_main[19] << 16) | au16data_main[20];

          uint32_t Error =
            ((uint32_t)au16data_main[21] << 16) | au16data_main[22];

          uint32_t Grand_Count =
            ((uint32_t)au16data_main[33] << 16) | au16data_main[34];

          uint32_t Grand_Weight =
            ((uint32_t)au16data_main[35] << 16) | au16data_main[36];
          

          myRam.loadcell_data.Analog_value_high = au16data_main[4];
          myRam.loadcell_data.Analog_value_low  = au16data_main[5];

          myRam.loadcell_data.Span_value_high   = au16data_main[6];
          myRam.loadcell_data.Span_value_low    = au16data_main[7];

          myRam.loadcell_data.Current_weight_high = au16data_main[11];
          myRam.loadcell_data.Current_weight_low  = au16data_main[12];

          myRam.loadcell_data.Division      = au16data_main[9];
          myRam.loadcell_data.Decimal_point = au16data_main[10];

          myRam.loadcell_data.Weighing_mode = au16data_main[23];
          myRam.loadcell_data.Weighing_step = au16data_main[24];

          
        }

        // ===== telegram 1: DATE & TIME =====
        if (u8query == 1)
        {
          uint32_t Date =
            ((uint32_t)au16data_time[0] << 16) | au16data_time[1];

          uint32_t Time =
            ((uint32_t)au16data_time[2] << 16) | au16data_time[3];

          myRam.loadcell_data.Date = Date;
          myRam.loadcell_data.Time = Time;

          myRam.loadcell_data.Key_value = au16data_time[441];
          myRam.loadcell_data.Relay_output = au16data_time[443];

          uint32_t Current_SP1 =
            ((uint32_t)au16data_time[444] << 16) | au16data_time[445];

          uint32_t Current_SP2 =
            ((uint32_t)au16data_time[446] << 16) | au16data_time[447];

          uint32_t Current_SP3 =
            ((uint32_t)au16data_time[448] << 16) | au16data_time[449];

          uint32_t Current_SP4 =
            ((uint32_t)au16data_time[450] << 16) | au16data_time[451];

          uint32_t Free_fall_SP1 =
            ((uint32_t)au16data_time[460] << 16) | au16data_time[461];

          uint32_t Free_fall_SP2 =
            ((uint32_t)au16data_time[462] << 16) | au16data_time[463];

          uint32_t Free_fall_SP3 =
            ((uint32_t)au16data_time[464] << 16) | au16data_time[465];

          uint32_t Free_fall_SP4 =
            ((uint32_t)au16data_time[466] << 16) | au16data_time[467];
        }

        // ===== chuyển telegram =====
        u8query++;
        if (u8query > 1) u8query = 0;

        u8state = 0;
        u32wait = millis() + 300;
      }
      break;
  }
}
