#include "I2CWrap.h"
#include <Wire.h>

#include "UartParser.h" // needed for verbs

#if ARDUINO >= 100
#define WireTransferWrite Wire.write
#define WireTransferRead Wire.read
#else
#define WireTransferWrite Wire.send
#define WireTransferRead Wire.receive
#endif

void I2CWrap::run(const String &verb, const String &destination,
                  const std::vector<String> &sendBytes,
                  const String &expectedByteCount) {

  response_.clear();

  printInfo("Verb: ", verb);
  printInfo("Destination: ", destination);
  if (UartParser::kVerbRead == verb) {
    printInfo("Expected bytes: ", expectedByteCount);
  }
  printInfo("Argument bytes :", sendBytes);
  printInfo("Remote register :", sendBytes[0], true);

  auto destinationAddress = static_cast<uint8_t>(hexToDec(destination));
  auto remoteRegister = hexToDec(sendBytes[0]);
  auto expectedReplyCount = static_cast<uint8_t>(hexToDec(expectedByteCount));

  if (UartParser::kVerbRead == verb) {
    read(destinationAddress, remoteRegister, expectedReplyCount);
  } else if (UartParser::kVerbWrite == verb) {
    write(destinationAddress, remoteRegister, sendBytes);
  }
}

void I2CWrap::read(uint8_t destinationAddress, uint8_t remoteRegister,
                   uint8_t expectedReplyCount) {
  uint8_t response = 255;

  Wire.beginTransmission(destinationAddress);
  WireTransferWrite(remoteRegister);
  response = Wire.endTransmission();

  printResponse(response, "[Read] Send remote register address: ");
  delay(1);

  Wire.beginTransmission(destinationAddress);
  Wire.requestFrom(destinationAddress, expectedReplyCount);
  while (!Wire.available()) {
  };

  for (uint8_t i = 0; i < expectedReplyCount && i < 8; ++i) {
    response_.push_back(WireTransferRead());
  }

  response = Wire.endTransmission();
  printResponse(response, "[Data] Read: ", true);
}

void I2CWrap::write(uint8_t destinationAddress, uint8_t remoteRegister,
                    const std::vector<String> &sendBytes) {
  uint8_t toSend[8], i = 0;

  for (auto it = sendBytes.begin() + 1; it != sendBytes.end() && i < 8;
       ++it, ++i) {
    toSend[i] = hexToDec(*it);
  }

  Wire.beginTransmission(destinationAddress);
  WireTransferWrite(remoteRegister);

  for (i = 0; i < sendBytes.size() && i < 8; ++i) {
    WireTransferWrite(toSend[i]);
  }
  uint8_t response = Wire.endTransmission();
  printResponse(response, "[Data] Write: ", true, true);
}

void I2CWrap::printResponse(bool extraNewLine) {

  if (!verbose_ || 0 == response_.size())
    return;

  Serial.print("Reply: ");
  for (auto &it : response_) {
    String formattedReply = decToHex(static_cast<byte>(it), 2);
    Serial.print(formattedReply);
    Serial.print("   ");
  }
  Serial.println();
  if (extraNewLine) {
    Serial.println();
  }
}

void I2CWrap::printResponse(uint8_t response, const String &stage,
                            bool printDelimiter, bool extraNewLine) {

  if (!verbose_)
    return;

  Serial.print(stage + " ");
  switch (response) {
  case 0:
    Serial.println("Success.");
    break;
  case 1:
    Serial.println("Data too long to fit in transmit buffer.");
    break;
  case 2:
    Serial.println("Received NACK on transmit of address.");
    break;
  case 3:
    Serial.println("Received NACK on transmit of data.");
    break;
  case 4:
    Serial.println("Other error.");
    break;
  case 255:
    Serial.println("No errorcode set.");
    break;
  }

  if (printDelimiter) {
    Serial.println("----------------------");
  }
  if (extraNewLine) {
    Serial.println();
  }
}

void I2CWrap::printInfo(const String &leadingText,
                        const std::vector<String> &toPrint,
                        bool printDelimiter) {
  if (toPrint.size() > 1) {
    Serial.print("Argument bytes : ");
    for (auto it = toPrint.begin() + 1; it != toPrint.end(); ++it) {
      Serial.print(hexToDec(*it));
      Serial.print(" ");
    }
    Serial.println(".");
  }
  if (printDelimiter) {
    Serial.println("----------------------");
  }
}
