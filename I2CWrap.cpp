#include "I2CWrap.h"
#include <Wire.h>

#if ARDUINO >= 100
#define WireTransfer Wire.write
#else
#define WireTransfer Wire.send
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

void I2CWrap::run(const int &destination, const std::vector<String> &sendBytes,
                  const int &expectedByteCount) {

  Serial.print("Destination : ");
  Serial.println(destination);

  Wire.beginTransmission(destination);

  auto remoteRegister = hexToDec(sendBytes[0]);
  // WireTransfer(remoteRegister);
  Serial.print("RemoteRegister : ");
  Serial.println(remoteRegister);

  if (sendBytes.rbegin() != sendBytes.rend() - 1) {
    Serial.println();
    Serial.println("Argument bytes : ");
  }

  for (const auto &it = sendBytes.rbegin(); it != sendBytes.rend() - 1; ++it) {
    // WireTransfer(hexToDec(*it));
    Serial.println(hexToDec(*it));
  }

  Wire.endTransmission();
}
