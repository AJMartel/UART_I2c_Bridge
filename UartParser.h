#pragma once
#include "Arduino.h"
#include "StandardCplusplus.h"
#include <stdint.h>
#include <vector>

class UartParser {
public:
  UartParser(bool verbose = true) { verbose_ = verbose; }
  void readFormattedString(bool isPrint = true);
  void print(const String &originalString = "");

  const String &getVerb() { return verb_; }
  const String &getDestination() { return destination_; }
  inline std::vector<String> &getSendBytes() { return sendBytes_; }
  const String &getExpectedByteCount() { return requested_bytes_; }

  static constexpr uint8_t kHexLength_ = 4;  // format example : 0xAB
  static constexpr uint8_t kVerbLength_ = 1; // example : W = Write, R = Read
  static const String kVerbRead;
  static const String kVerbWrite;

private:
  void clear() {
    destination_ = "";
    sendBytes_.clear();
  }

  uint8_t readSendBytes(const String &in);

  String verb_;
  bool verbose_;
  String destination_;
  String requested_bytes_;
  std::vector<String> sendBytes_;
};
