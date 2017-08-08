#include "UartParser.h"


const String UartParser::kVerbRead = "R";
const String UartParser::kVerbWrite = "W";

void UartParser::print(const String &originalString) {
  Serial.println("Got : " + originalString);

  Serial.print(verb_ + "   ");
  Serial.print(destination_ + "   ");
  for (const auto &sendByte : sendBytes_) {
    Serial.print(sendByte + "   ");
  }

  Serial. println(requested_bytes_);
}

uint8_t UartParser::readSendBytes(const String &in) {

  
  auto i = 0;
  auto commaOffset = 0;
  auto initialOffset = kVerbLength_+1+kHexLength_+1;
  auto finalPos = 0;
  
  for (; ; ++i) {

    if (i > 0){
      commaOffset = 1;
    }
      
    auto start = initialOffset + i * kHexLength_ + commaOffset;
    auto end = initialOffset + (i+1) * kHexLength_ + commaOffset;
    String sendByte = in.substring(start, end);
    
    sendBytes_.push_back(sendByte);
    finalPos = end;
    if(',' != in [end]) break;
  }

  return finalPos;
}

void UartParser::readFormattedString(bool isPrint = true) {
  // Format: 0x<HEX Address of Destination>:<Number of send Bytes>:0x<Send
  // Byte0>[,0x<Send Byte1>[,...]]
  // Example1 : W:0x40:0x12
  // Example2 : R:0x40:0x12,0xEF:1

  while (Serial.available() == 0)
    ;

  String str = Serial.readString();

  verb_ = str.substring(0, kVerbLength_);
  destination_ = str.substring(kVerbLength_+1, kHexLength_+kVerbLength_+1);
  
  int posInInput = readSendBytes(str);
  if(posInInput < str.length()){
    requested_bytes_ = str.substring(posInInput+1, str.length());
  }
  

  if (isPrint) {
    print(str);
  }
}
