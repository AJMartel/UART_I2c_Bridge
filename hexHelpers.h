#pragma once

#include "Arduino.h"

unsigned int hexToDec(const String &hexString);
String decToHex(byte decValue, byte desiredStringLength);
