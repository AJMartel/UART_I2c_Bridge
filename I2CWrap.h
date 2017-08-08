#pragma once

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <stdint.h>
#include <vector>

class I2CWrap {
public:
  void run(const String& verb, const String& destination, const std::vector<String> &sendBytes,
           const String& expectedByteCount);

private:
  void decodeResponse(uint8_t response, const String& stage="");

  std::vector<uint8_t> response_;
};
