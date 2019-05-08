/* Based on DS3231_test.pde by Eric Ayars.

   Test/demo of read routines for a DS3231 RTC.

   Turn on the serial monitor after loading this to check if things are
   working as they should.
*/

#include <DS3231.h> /* https://github.com/NorthernWidget/DS3231 */
#include <Wire.h>

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;

void setup() {
	Wire.begin();         // Start the I2C interface
	Serial.begin(115200); // Start the serial interface
}

void print_date() {
	// Year
	Serial.print('2');
	if (Century) {			// Won't need this for 89 years.
		Serial.print('1');
	} else {
		Serial.print('0');
	}
	int year = Clock.getYear();
	if (year <= 9) {
		Serial.print('0');
	}
	Serial.print(year, DEC);

	// Month
	Serial.print('-');
	Serial.print(Clock.getMonth(Century), DEC);

	// Day
	Serial.print('-');
	Serial.print(Clock.getDate(), DEC);
}

void print_time() {
	Serial.print(Clock.getHour(h12, PM), DEC);
	Serial.print(':');
	Serial.print(Clock.getMinute(), DEC);
	Serial.print(':');
	Serial.print(Clock.getSecond(), DEC);

	// Add AM/PM indicator
	if (h12) {
		if (PM) {
			Serial.print(" PM ");
		} else {
			Serial.print(" AM ");
		}
	}
}

void loop() {
	print_date();
	Serial.print(' ');
	print_time();
	Serial.print('\n');
	delay(1000);
}
