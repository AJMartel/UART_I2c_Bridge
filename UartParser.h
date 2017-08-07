#pragma once
#include "Arduino.h"
#include "StandardCplusplus.h"
#include <stdint.h>
#include <vector>

class UartParser {
public:
  void readFormattedString(bool isPrint = true);
  void print(const String &originalString = "");

  inline int getDestination() { return destination_.toInt(); }
  inline std::vector<String> &getSendBytes() { return sendBytes_; }
  inline int getExpectedByteCount() { return expectedByteCount_.toInt(); }

private:
  void readSendBytes(const String &in, long int sendByteCount, uint8_t &newPos,
                     uint8_t &commaOffset);

  String destination_;
  String sendByteCount_;
  String expectedByteCount_;
  std::vector<String> sendBytes_;
};
