
#include "UART_I2c_Bridge.h"

UartParser bridge;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial.println("Setup ready.");
}

void loop() {
  bridge.readFormattedString();
}

