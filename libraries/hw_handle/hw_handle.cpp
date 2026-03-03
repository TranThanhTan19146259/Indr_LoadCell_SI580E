#include "hw_handle.h"

void init_hw()
{
  Serial.begin( 115200 ); // baud-rate at 19200

  init_at_cmd();
  init_rfid();
    
}

void handle_hw()
{
  handle_at_cmd();
  handle_rfid();
}