/*
DS3231_set.pde
Eric Ayars
4/11

Test of set-time routines for a DS3231 RTC

*/

#include <DS3231.h> /* https://github.com/NorthernWidget/DS3231 */
#include <Wire.h>

DS3231 Clock;

bool Century=false;

void read_string_from_serial(char end_char, char* str) {
	for (byte i = 0; Serial.available(); i++) {
		char c = Serial.read();
		str[i] = c;
		if (c == 'x')
			break;
	}
}

int string_to_number(char* str, byte digit_count) {
	int number = 0;
	for (int e = digit_count - 1; e >= 0; e--, str++) {
		/* Serial.print("e="); */
		/* Serial.print(e, DEC); */

		byte digit = *str - '0';
		/* Serial.print(" digit="); */
		/* Serial.print(digit, DEC); */

		int p = pow(10, e);
		/* Serial.print(" p="); */
		/* Serial.print(p, DEC); */

		number += digit * p;
		/* Serial.print(" number="); */
		/* Serial.println(number, DEC); */
	}
	return number;
}

void print_clock(byte Year, byte Month, byte Day,
                 byte DoW,
                 byte Hour, byte Minute, byte Second,
                 bool h12, bool pm) {
	Serial.print('2');
	if (Century) {			// Won't need this for 89 years.
		Serial.print('1');
	} else {
		Serial.print('0');
	}
	if (Year <= 9) {
		Serial.print('0');
	}
  Serial.print(Year);
	Serial.print('-');
	Serial.print(Month);
	Serial.print('-');
	Serial.print(Day);
	Serial.print(" (");
	Serial.print(DoW);
	Serial.print(") ");
	Serial.print(Hour);
	Serial.print(':');
	Serial.print(Minute);
	Serial.print(':');
	Serial.print(Second);
	if (h12) {
		if (pm) {
			Serial.print(" PM");
		} else {
			Serial.print(" AM");
		}

	}
}

void print_current_clock() {
	bool h12  = false;
	bool pm   = false;
	byte hour = Clock.getHour(h12, pm);
  print_clock(Clock.getYear(), Clock.getMonth(Century), Clock.getDate(),
              Clock.getDoW(),
              hour, Clock.getMinute(), Clock.getSecond(),
              h12, pm);
}

// Call this if you notice something coming in on 
// the serial port. The stuff coming in should be in 
// the order YYMMDDwHHMMSS, with an 'x' at the end.
void read_clock_from_serial() {
	char str[13];

	read_string_from_serial('x', str);
	Serial.print("Raw String: ");
	Serial.println(str);

	byte year, month, day, dow, hour, minute, second;
	year   = string_to_number(&str[0],  2); // YY
	month  = string_to_number(&str[2],  2); // MM
	day    = string_to_number(&str[4],  2); // DD
	dow    = string_to_number(&str[6],  1); // w
	hour   = string_to_number(&str[7],  2); // HH
	minute = string_to_number(&str[9],  2); // MM
	second = string_to_number(&str[11], 2); // SS

	Clock.setClockMode(false); // Set to 24h.
	Clock.setYear(year);
	Clock.setMonth(month);
	Clock.setDate(day);
	Clock.setDoW(dow);
	Clock.setHour(hour);
	Clock.setMinute(minute);
	Clock.setSecond(second);

	Serial.print("New Clock: ");
	print_current_clock();
	Serial.print("\n");
}

void setup() {
	Wire.begin();       // Start the I2C interface
	Serial.begin(115200); // Start the serial port
}

void set_clock() {
	Clock.setClockMode(false); // Set to 24h.
	Clock.setYear(18);
	Clock.setMonth(2);
	Clock.setDate(2);
	Clock.setDoW(6);
	Clock.setHour(22);
	Clock.setMinute(20);
	Clock.setSecond(20);
}

void loop() {
	/* set_clock(); */

	// If something is coming in on the serial line, it's
	// a time correction so set the clock accordingly.
	if (Serial.available()) {
		Serial.println("Reading time correction...");
		read_clock_from_serial();
	} else {
		Serial.print("Current Clock: ");
		print_current_clock();
		Serial.print("\n");
	}
	delay(1000);
}
