#pragma once

#include <SPI.h>
#include <MFRC522.h>
#include "Ram.h"


#define SS_PIN 5
#define RST_PIN 27


void init_rfid();

void handle_rfid();