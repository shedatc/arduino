#ifndef Utils_h
#define Utils_h

#include <Arduino.h>

class Utils
{
 public:
    // Read a string from Serial. The string is expected to end with endChar.
    static void readStringFromSerial(char endChar, char* str);

    // Convert a string representing a number made of digitCount digits into an
    // int. E.g., stringToNumber("213xx0", 2) => 21.
    static int  stringToNumber(char* str, byte digitCount);
};

#endif
