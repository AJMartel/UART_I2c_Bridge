#pragma once

#include "StandardCplusplus.h"
#include <stdint.h>
#include <vector>

class Uart2I2CBridge {
public:
  void readFormattedString(bool isPrint = true);
  void Uart2I2CBridge::print();
  void print(const String &originalString = "");

private:
  void readSendBytes(const String &in, long int sendByteCount, uint8_t &newPos,
                     uint8_t &commaOffset);

  String destination_;
  String sendByteCount_;
  String expectedByteCount_;
  std::vector<String> sendBytes_;
};
