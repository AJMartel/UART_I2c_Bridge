#include "I2CWrap.h"
#include <Wire.h>

#if ARDUINO >= 100
#define WireTransfer Wire.write
#else
#define WireTransfer Wire.send
#endif
void I2CWrap::run(const int &destination, const std::vector<String> &sendBytes,
                  const int &expectedByteCount) {
  Wire.beginTransmission(destination);

  auto remoteRegister = sendBytes[0].toInt();
  WireTransfer(remoteRegister);

  for (const auto &it = sendBytes.rbegin(); it != sendBytes.rend() - 1; ++it) {
    WireTransfer((*it).toInt());
  }

  Wire.endTransmission();
}
