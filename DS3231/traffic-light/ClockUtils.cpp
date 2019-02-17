#include "ClockUtils.h"
#include "Utils.h"

ClockUtils::ClockUtils(bool century)
{
  _century = century;
}

void ClockUtils::printClock()
{
  // Year
  Serial.print('2');
  if (_century) {			// Won't need this for 89 years.
    Serial.print('1');
  } else {
    Serial.print('0');
  }
  byte year = _clock.getYear();
  if (year <= 9) {
    Serial.print('0');
  }
  Serial.print(year);

  // Month
  Serial.print('-');
  Serial.print( _clock.getMonth(_century) );

  // Day
  Serial.print('-');
  Serial.print( _clock.getDate() );

  // Day of the Week
  Serial.print(" (");
  Serial.print( _clock.getDoW() );
  Serial.print(") ");

  // Hour
  Serial.print( getHour() );

  // Minute
  Serial.print(':');
  Serial.print( _clock.getMinute() );

  // Second
  Serial.print(':');
  Serial.print( _clock.getSecond() );
}

// Call this if you notice something coming in on the serial port. The stuff
// coming in should be in the order YYMMDDwHHMMSS, with an 'x' at the end.
void ClockUtils::readClockFromSerial()
{
  char str[13];

  Utils::readStringFromSerial('x', str);
  Serial.print("Raw String: ");
  Serial.println(str);

  byte year, month, day, dow, hour, minute, second;
  year   = Utils::stringToNumber(&str[0],  2); // YY
  month  = Utils::stringToNumber(&str[2],  2); // MM
  day    = Utils::stringToNumber(&str[4],  2); // DD
  dow    = Utils::stringToNumber(&str[6],  1); // w
  hour   = Utils::stringToNumber(&str[7],  2); // HH
  minute = Utils::stringToNumber(&str[9],  2); // MM
  second = Utils::stringToNumber(&str[11], 2); // SS

  _clock.setClockMode(false); // Set to 24h mode.
  _clock.setYear(year);
  _clock.setMonth(month);
  _clock.setDate(day);
  _clock.setDoW(dow);
  _clock.setHour(hour);
  _clock.setMinute(minute);
  _clock.setSecond(second);

  Serial.print("New Clock: ");
  printClock();
  Serial.print("\n");
}

byte ClockUtils::getDayOfTheWeek()
{
  return _clock.getDoW();
}

bool ClockUtils::isWeekEnd()
{
  switch ( getDayOfTheWeek() ) {
  case MONDAY:
  case TUESDAY:
  case WEDNESDAY:
  case THURSDAY:
  case FRIDAY:
    return false;
  case SATURDAY:
  case SUNDAY:
  default:
    return true;
  }
}

byte ClockUtils::getHour()
{
	bool h12  = false; // true if 12h mode, false if 24h mode.
  bool pm   = false;
  byte hour = _clock.getHour(h12, pm);
  if (h12 && pm) { // Emulate 24h mode if needed.
    hour += 12;
  }
  return hour;
}

byte ClockUtils::getMinute()
{
  return _clock.getMinute();
}

// XXX
void ClockUtils::setClock()
{
  _clock.setClockMode(false); // Set to 24h.
  _clock.setYear(19);
  _clock.setMonth(2);
  _clock.setDate(14);
  _clock.setDoW(4);
  _clock.setHour(14);
  _clock.setMinute(43);
  _clock.setSecond(20);
}
