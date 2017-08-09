#pragma once

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <stdint.h>
#include <vector>

#include "hexHelpers.h"

class I2CWrap {
public:
  I2CWrap(bool verbose = true) { verbose_ = verbose; }
  void run(const String &verb, const String &destination,
           const std::vector<String> &sendBytes,
           const String &expectedByteCount);
  void printResponse(bool extraNewLine = true);
  void setVerbosity(bool verbosity) { verbose_ = verbosity; }

private:
  void printResponse(uint8_t response, const String &stage = "",
                     bool printDelimiter = false, bool extraNewLine = false);

  void printInfo(const String &leadingText, const std::vector<String> &toPrint,
                 bool printDelimiter = false);

  template <typename T>
  void printInfo(const String &leadingText, T toPrint,
                 bool printDelimiter = false) {
    if (!verbose_)
      return;

    Serial.print(leadingText);
    Serial.println(toPrint);

    if (printDelimiter) {
      Serial.println("----------------------");
    }
  }

  void read(uint8_t destination, uint8_t remoteRegister,
            uint8_t expectedReplyCount);

  void write(uint8_t destination, uint8_t remoteRegister,
             const std::vector<String> &sendBytes);

  bool verbose_;
  std::vector<uint8_t> response_;
};
