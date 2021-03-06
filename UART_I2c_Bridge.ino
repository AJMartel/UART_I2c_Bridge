#include "UartParser.h"
#include "I2CWrap.h"

UartParser uartRequest;
I2CWrap i2cWrap;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup ready.");
}

void loop() {
  uartRequest.readFormattedString();
  i2cWrap.run(uartRequest.getVerb(), uartRequest.getDestination(), uartRequest.getSendBytes(), uartRequest.getExpectedByteCount());
  i2cWrap.printResponse();
}

