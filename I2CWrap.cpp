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

// adapted from
// https://github.com/benrugg/Arduino-Hex-Decimal-Conversion/blob/master/hex_dec.ino
unsigned int hexToDec(const String &hexString) {

  unsigned int decValue = 0;
  int nextInt;

  int i = 0;
  if ('x' == hexString[1])
    i = 2; // skip;

  for (; i < hexString.length(); i++) {

    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57)
      nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70)
      nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102)
      nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}

String decToHex(byte decValue, byte desiredStringLength) {

  String hexString = String(decValue, HEX);
  while (hexString.length() < desiredStringLength)
    hexString = "0" + hexString;

  return "0x" + hexString;
}

void I2CWrap::run(const String &verb, const String &destination,
                  const std::vector<String> &sendBytes,
                  const String &expectedByteCount) {

  Serial.print("Verb : ");
  Serial.println(verb);

  Serial.print("Destination : ");
  Serial.println(destination);

  auto destinationAddress = static_cast<uint8_t>(hexToDec(destination));
  uint8_t remoteRegister = hexToDec(sendBytes[0]);
  auto expectedReplyCount = static_cast<uint8_t>(hexToDec(expectedByteCount));

  Serial.print("RemoteRegister : ");
  Serial.println(remoteRegister);

  if (UartParser::kVerbRead == verb) {
    Serial.print("Expecting : ");
    Serial.print(expectedByteCount);
    Serial.println(" bytes.");
  } else if (UartParser::kVerbWrite == verb) {
  }

  if (sendBytes.size() > 1) {
    Serial.print("Argument bytes : ");
    for (auto it = sendBytes.begin() + 1; it != sendBytes.end(); ++it) {
      Serial.print(hexToDec(*it));
      Serial.print(" ");
    }
    Serial.println(".");
  }
  Serial.println("----------------------");

  uint8_t response = 255;

  if (UartParser::kVerbRead == verb) {

    // --- Register pointer set ---

    Wire.beginTransmission(destinationAddress);
    WireTransferWrite(remoteRegister);
    response = Wire.endTransmission();

    decodeResponse(response, "[Read] Send remote register address: ");
    delay(1);

    Wire.beginTransmission(destinationAddress);
    Wire.requestFrom(destinationAddress, expectedReplyCount);
    while (!Wire.available()) {
    };

    uint8_t reply[8];
    for (uint8_t i = 0; i < expectedReplyCount && i < 8; ++i) {
      reply[i] = WireTransferRead();
    }

    response = Wire.endTransmission(); // needs to be immediately after
                                       // write/read to avoid timeout

    Serial.print("Reply: ");
    for (uint8_t i = 0; i < expectedReplyCount && i < 8; ++i) {
      String formattedReply = decToHex(static_cast<byte>(reply[i]), 2);
      Serial.print(formattedReply);
      Serial.print("   ");
    }
    Serial.println();

  } else if (UartParser::kVerbWrite == verb) {

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
    response = Wire.endTransmission(); // needs to be immediately after
                                       // write/read to avoid timeout
  }

  decodeResponse(response, "[End] ");
  Serial.println();
}

void I2CWrap::decodeResponse(uint8_t response, const String &stage) {

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
}
