#pragma once

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <stdint.h>
#include <vector>

class I2CWrap {
public:
  void run(const int &destination, const std::vector<String> &sendBytes,
           const int &expectedByteCount);

private:
  std::vector<uint8_t> response_;
};
