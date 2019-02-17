#include <DS3231.h> /* https://github.com/NorthernWidget/DS3231 */
#include <Wire.h>

#include "ClockUtils.h"
#include "Utils.h"

ClockUtils clockUtils(false /* century */);

// Digital pins controling the LEDs:
#define GREEN_LED_PIN  22
#define ORANGE_LED_PIN 23
#define RED_LED_PIN    24
#define FIRST_LED_PIN  GREEN_LED_PIN
#define LAST_LED_PIN   RED_LED_PIN

// LEDs ////////////////////////////////////////////////////////////////////////

void printLedColor(byte ledPin)
{
  Serial.print("LED: ");
  switch (ledPin) {
  case GREEN_LED_PIN:  Serial.println("Green");  break;
  case RED_LED_PIN:    Serial.println("Red");    break;
  case ORANGE_LED_PIN: Serial.println("Orange"); break;
  default:             Serial.println("UNKNOWN");
  }
}

void switchLight(byte targetLedPin)
{
  for (byte currentLedPin = FIRST_LED_PIN; currentLedPin <= LAST_LED_PIN; currentLedPin++) {
    if (currentLedPin == targetLedPin) {
      digitalWrite(currentLedPin, HIGH);
      printLedColor(currentLedPin);
    }
    else
      digitalWrite(currentLedPin, LOW);
  }
}

// Transitions:
//     7h15  red    -> orange
//     7h30  orange -> green
//     20h30 green  -> red
byte selectLightDuringWorkingDay()
{
  byte hour   = clockUtils.getHour();
  byte minute = clockUtils.getMinute();

  Serial.print("Time Slot: ");
  if (hour == 7) {                                // 7h00..7h59
    if (minute <= 14) {                           // 7h00..7h14
      Serial.println("7h00..7h14");
      return RED_LED_PIN;
    }
    else if (minute <= 29) {                      // 7h15..7h29
      Serial.println("7h15..7h29");
      return ORANGE_LED_PIN;
    }
    else {                                        // 7h30..7h59
      Serial.println("7h30..7h59");
      return GREEN_LED_PIN;
    }
    // NOTREACHED
  }
  else if (hour >= 8 && hour <= 19) {             // 8h00..19h59
    Serial.println("8h00..19h59");
    return GREEN_LED_PIN;
  }
  else if (hour == 20) {                          // 20h00..20h59
    if (minute <= 14) {                           // 20h00..20h14
      Serial.println("20h00..20h14");
      return GREEN_LED_PIN;
    }
    else if (minute <= 29) {                      // 20h00..20h29
      Serial.println("20h00..20h29");
      return ORANGE_LED_PIN;
    }
    else {                                        // 20h30..20h59
      Serial.println("20h30..20h59");
      return RED_LED_PIN;
    }
    // NOTREACHED
  }
  else {                                          // Default
    Serial.println("default");
    return RED_LED_PIN;
  }
  // NOTREACHED
}

// Transitions:
//     8h15  red    -> orange
//     8h30  orange -> green
//     13h30 green  -> red
//     14h30 red    -> orange
//     14h40 orange -> green
//     20h30 green  -> red
byte selectLightDuringWeekEnd()
{
  byte hour   = clockUtils.getHour();
  byte minute = clockUtils.getMinute();

  Serial.print("Time Slot: ");
  if (hour == 8) {                                // 8h00..8h59
    if (minute <= 14) {                           // 8h00..8h14
      Serial.println("8h00..7h14");
      return RED_LED_PIN;
    }
    else if (minute <= 29) {                      // 8h15..8h29
      Serial.println("8h15..8h29");
      return ORANGE_LED_PIN;
    }
    else {                                        // 8h30..8h59
      Serial.println("7h30..7h59");
      return GREEN_LED_PIN;
    }
    // NOTREACHED
  }
  else if (hour >= 9 && hour <= 12) {             // 9h00..12h59
    Serial.println("9h00..12h59");
    return GREEN_LED_PIN;
  }
  else if (hour == 13) {                          // 13h00..13h59
    if (minute <= 29) {                           // 13h00..13h29
      Serial.println("13h00..13h29");
      return GREEN_LED_PIN;
    }
    else {                                        // 13h30..13h59
      Serial.println("13h30..13h59");
      return RED_LED_PIN;
    }
    // NOTREACHED
  }
  else if (hour == 14) {                          // 14h00..14h59
    if (minute <= 29) {                           // 14h00..14h29
      Serial.println("14h00..14h29");
      return RED_LED_PIN;
    }
    else if (minute <= 39) {                      // 14h30..14h39
      Serial.println("14h30..14h39");
      return ORANGE_LED_PIN;
    }
    else {                                        // 14h40..14h59
      Serial.println("14h40..14h59");
      return GREEN_LED_PIN;
    }
    // NOTREACHED
  }
  else if (hour >= 15 && hour <= 19) {             // 15h00..19h59
    Serial.println("15h00..19h59");
    return GREEN_LED_PIN;
  }
  else if (hour == 20) {                          // 20h00..20h59
    if (minute <= 14) {                           // 20h00..20h14
      Serial.println("20h00..20h14");
      return GREEN_LED_PIN;
    }
    else if (minute <= 29) {                      // 20h00..20h29
      Serial.println("20h00..20h29");
      return ORANGE_LED_PIN;
    }
    else {                                        // 20h30..20h59
      Serial.println("20h30..20h59");
      return RED_LED_PIN;
    }
    // NOTREACHED
  }
  else {                                          // Default
    Serial.println("default");
    return RED_LED_PIN;
  }
  // NOTREACHED
}

void triggerLights()
{
  byte ledPin;

  Serial.print("Day Slot: ");
  if (clockUtils.isWeekEnd()) {
    Serial.println("Week-End");
    ledPin = selectLightDuringWeekEnd();
  }
  else {
    Serial.println("Working Day");
    ledPin = selectLightDuringWorkingDay();
  }
  switchLight(ledPin);
}

// SETUP & LOOP ////////////////////////////////////////////////////////////////

void setup()
{
  Wire.begin();         // Start the I2C interface.
  Serial.begin(115200); // Start the serial port.

  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // XXX Set the clock.
  if (false) {
    clockUtils.setClock();
    Serial.print("Clock set.\n");
  }
}

void loop()
{
  // XXX Test the LEDs.
  if (false) {
    switchLight(GREEN_LED_PIN);
    delay(1000);
    switchLight(ORANGE_LED_PIN);
    delay(1000);
    switchLight(RED_LED_PIN);
    delay(1000);
    return;
    // NOTREACHED
  }

  Serial.print("\n");
  if (Serial.available()) {
    Serial.println("Reading time correction...");
    clockUtils.readClockFromSerial();
  }
  else {
    Serial.print("Clock: ");
    clockUtils.printClock();
    Serial.print("\n");
  }
  triggerLights();
  delay(1000);
}
