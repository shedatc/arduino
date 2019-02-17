#include "Utils.h"

void Utils::readStringFromSerial(char endChar, char* str) {
  for (byte i = 0; Serial.available(); i++) {
    char c = Serial.read();
    str[i] = c;
    if (c == endChar)
      break;
  }
}

int Utils::stringToNumber(char* str, byte digitCount) {
  int number = 0;
  for (int e = digitCount - 1; e >= 0; e--, str++) {
    byte digit = *str - '0';
    int  p     = pow(10, e);

    number += digit * p;
  }
  return number;
}
