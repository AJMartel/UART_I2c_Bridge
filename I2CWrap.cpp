#include "I2CWrap.h"
#include <Wire.h>

void I2CWrap::run(const int &destination, const std::vector<String> &sendBytes,
                  const int &expectedByteCount) {
  Wire.begin();
  Wire.requestFrom(destination,
                   expectedByteCount); // request 6 bytes from slave device #8
  while (Wire.available()) {
    response_.push_back(static_cast<uint8_t>(Wire.read()));
  }
}
