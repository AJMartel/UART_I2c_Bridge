#include "I2CWrap.h"
#include <Wire.h>

#include "UartParser.h" // needed for verbs

#if ARDUINO >= 100
#define WireTransferWrite Wire.write
#define WireTransferRead Wire.read
#else
#define WireTransferWrite Wire.send
#define WireTransferRead Wire.receive
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

void I2CWrap::run(const String& verb, const String& destination, const std::vector<String> &sendBytes,
                  const String& expectedByteCount) {

  Serial.print("Verb : "); 
  Serial.println(verb);

  Serial.print("Destination : ");
  Serial.println(destination);

  auto remoteRegister = hexToDec(sendBytes[0]);
  // WireTransfer(remoteRegister);
  Serial.print("RemoteRegister : ");
  Serial.println(remoteRegister);

  if(UartParser::kVerbRead == verb){
    Serial.print("Expecting : ");
    Serial.print(expectedByteCount);
    Serial.println(" bytes.");
  } else if (UartParser::kVerbWrite == verb) {
    
  }
  

  
  if(UartParser::kVerbRead == verb){
    //WireTransferRead(remoteRegister);
  } else if (UartParser::kVerbWrite == verb) {
    //WireTransferWrite(remoteRegister);
  }

  Wire.beginTransmission(static_cast<uint8_t>(hexToDec(destination)));

  if (sendBytes.size()>1) {
    
    Serial.print("Argument bytes : ");
  
    for (const auto &it = sendBytes.rbegin(); it != sendBytes.rend(); ++it) {
      // WireTransfer(hexToDec(*it));
      Serial.print(hexToDec(*it)); Serial.print(" ");
    }
    Serial.println(".");  
  }
  
  Wire.endTransmission();
}
