#ifndef ClockUtils_h
#define ClockUtils_h

#include <Arduino.h>
#include <DS3231.h>    // https://github.com/NorthernWidget/DS3231

class ClockUtils
{
 public:
    ClockUtils(bool century);

    void printClock();
    void readClockFromSerial();
    byte getDayOfTheWeek();
    bool isWeekEnd();
    byte getHour(); // Always as if we were in 24h mode.
    byte getMinute();

    // XXX
    void setClock();

 private:
    DS3231 _clock;
    bool   _century = false;
    enum DAY_OF_THE_WEEK {
                          MONDAY = 1,
                          TUESDAY,
                          WEDNESDAY,
                          THURSDAY,
                          FRIDAY,
                          SATURDAY,
                          SUNDAY,
    };

};

#endif
