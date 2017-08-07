#include "UartParser.h"

static constexpr uint8_t kHexLength = 4; // format example : 0xAB

void UartParser::print(const String &originalString) {
  Serial.println("Got : " + originalString);
  Serial.print(destination_ + "   " + expectedByteCount_ + "   ");
  for (const auto &sendByte : sendBytes_) {
    Serial.print(sendByte + "   ");
  }

  Serial.println(expectedByteCount_);
}

void UartParser::readSendBytes(const String &in, long int sendByteCount,
                               uint8_t &newPos, uint8_t &commaOffset) {

  auto posSendByteBegin = kHexLength + 3;
  auto i = 0;
  commaOffset = 0;
  for (; i < sendByteCount; ++i) {

    if (i > 0)
      commaOffset = 1;
    String sendByte = in.substring(
        posSendByteBegin + i * kHexLength + commaOffset,
        posSendByteBegin + kHexLength + i * kHexLength + commaOffset);
    sendBytes_.push_back(sendByte);
  }
  newPos = posSendByteBegin + i * kHexLength + commaOffset + 1;
}

void UartParser::readFormattedString(bool isPrint = true) {
  // Format: 0x<HEX Address of Destination>:<Number of send Bytes>:0x<Send
  // Byte0>[,0x<Send Byte1>[,...]]:<Number of expected bytes>
  // Example1 : 0x40:1:0x12:1
  // Example2 : 0x40:2:0x12,0xEF:1

  while (Serial.available() == 0)
    ;

  String str = Serial.readString();

  destination_ = str.substring(0, kHexLength);
  sendByteCount_ = str.substring(kHexLength + 1, kHexLength + 2);

  uint8_t posExpectedByteCount = 0, commaOffset = 0;

  readSendBytes(str, sendByteCount_.toInt(), posExpectedByteCount, commaOffset);

  expectedByteCount_ =
      str.substring(posExpectedByteCount, posExpectedByteCount + 1);

  if (isPrint) {
    print(str);
  }
}
