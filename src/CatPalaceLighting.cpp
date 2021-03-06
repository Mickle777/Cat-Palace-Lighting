////////////////////////////////////////////////////////////////////
// Cat Palace Lighting

///////////////////////////////////////////////////////////////////////////////
// Library imports

#include <Arduino.h>
#include <RTClib.h>
#include <ArduinoLog.h>
#include "Ticker.h"
#include <Wire.h>
#include <LampControl.h>
#include <LiquidCrystal.h>

// Function prototypes so the compiler shuts up...
void check_time();
void check_light();
//void update_lcd();
float get_current();

// Logging/Comms
const long SERIAL_BAUD = 57600;
const int LOGGER_LEVEL = LOG_LEVEL_VERBOSE;

// RTC
//* Wiring; VCC to LIGHT + (3V3), GND to LIGHT -, SDA to LIGHT ~ (A4), SCL to LIGHT C (A5)
RTC_DS3231 rtc;
const int ON_HOUR = 5;
const int ON_MINUTE = 30;
const int OFF_HOUR = 23;
const int OFF_MINUTE = 30;
bool lights_enabled;
long time_of_last_check;
const long TIME_BETWEEN_CHECKS = 1000; //! ms CHANGE BACK TO 10000 AFTER TESTING
Ticker rtc_check_timer(check_time, TIME_BETWEEN_CHECKS, 0, MILLIS);

// LDR
const int LDR_PIN = A0; //* Fork 1 ~
const int LDR_LOWER_THRESHOLD = 30;
const int LDR_UPPER_THRESHOLD = 80;
Ticker light_check_timer(check_light, TIME_BETWEEN_CHECKS, 0, MILLIS);


/**
// Display
const int LCD_ENABLE_PIN = A2;
const int LCD_RS_PIN = A3;
const int LCD_D4 = 2;
const int LCD_D5 = 3;
const int LCD_D6 = 4;
const int LCD_D7 = A6; // Can go on D5
const int LCD_BACKLIGHT = 11;
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Ticker update_lcd_timer(update_lcd, TIME_BETWEEN_CHECKS, 0, MILLIS);

const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;

*/

// Switches
const int LAMP_PIN = 13; //! Keep this change from 10 to 13 so can see board led on when lamp on. Lamp switch is Q2
LampControl light(LAMP_PIN);
const int PUMP_PIN = 12; //* Q1 gate

// Current meter
const int CURRENT_PIN = A1; //* Fork 2 ~
const float CURRENT_SCALAR = 0.2;

///////////////////////////////////////////////////////////////////////////////
// Main Functions

void setup(){
    Serial.begin(SERIAL_BAUD);
    Log.begin(LOGGER_LEVEL, &Serial);
    Log.notice(F("Working...\n")); //! What does \n do?

    rtc.begin();
    DateTime now = rtc.now();
    time_of_last_check = millis();
    Log.notice(F("Time is %d:%d\n"), now.hour(), now.minute());
    rtc_check_timer.start();

    pinMode(LDR_PIN, INPUT);
    light_check_timer.start();

    //! Can we add some lines that we can have // out then run when need to reset rtc to PC time

   // lcd.begin(LCD_WIDTH, LCD_HEIGHT);
   // update_lcd_timer.start();

    lights_enabled = true;
}


void loop(){
  rtc_check_timer.update();
  light_check_timer.update();
 // update_lcd_timer.update();
  light.tick(); //! what does this do??
}


///////////////////////////////////////////////////////////////////////////////
// Functions

void check_time(){
  /**
  * Enables/disables the lamp according to time
  */

    DateTime now = rtc.now();
    Log.verbose(F("Time check:\t%d:%d:%d\n"), now.hour(), now.minute(), now.second());

    if (lights_enabled){ //! removed ! ??
        if (now.hour() >= ON_HOUR && now.minute() >= ON_MINUTE) {
            lights_enabled = true;
            Log.notice(F("Lights enabled\n"));
        }
    }else{
        if (now.hour() >= OFF_HOUR && now.minute() >= OFF_MINUTE) {
            lights_enabled = false;
            light.deactivate_lamp();
            Log.notice(F("Lights disabled\n"));
        }
    }
}

void check_light(){
    if (lights_enabled){

        int ldr_level = analogRead(LDR_PIN);
        Log.verbose(F("LDR level: %d\n"), ldr_level); //! why %d\n??

        // Check levels and switch lamp accordingly
       //! whats this shit?? if (light.is_active){
            if (ldr_level > LDR_UPPER_THRESHOLD){
                light.deactivate_lamp(); 
                Log.notice(F("Lights off\n"));               
            }
        //!}
        else{
            if (ldr_level < LDR_LOWER_THRESHOLD){
               //! whats this?? light.timeout_enabled = false;
                light.activate_lamp();
                Log.notice(F("Lights on\n"));
            }
        }
    }
}

/**
void update_lcd(){
  // TIME-HHMM C.C A
  // ON-HHMM OFF-HHMM

  lcd.setCursor(0, 0);

  // Current time
  DateTime now = rtc.now();
  lcd.write("TIME-");
  if (now.hour() < 10) {
    lcd.write('0');
  }
  lcd.write(now.hour());
  lcd.write(':');
  if (now.minute() < 10) {
    lcd.write('0');
  }
  lcd.write(now.minute());
  lcd.write(' ');

  // Current
  float current = get_current();
  lcd.write(int(current));
  lcd.write('.');
  lcd.write(int((current - int(current)) * 10));
  lcd.write(" A");

  lcd.setCursor(0, 1);

  // ON time
  lcd.write("ON-");
  if (ON_HOUR < 10) {
    lcd.write('0');
  }
  lcd.write(ON_HOUR);
  if (ON_MINUTE < 10) {
    lcd.write('0');
  }
  lcd.write(ON_MINUTE);

  // OFF time
  lcd.write("OFF-");
  if (OFF_HOUR < 10) {
    lcd.write('0');
  }
  lcd.write(OFF_HOUR);
  if (OFF_MINUTE < 10) {
    lcd.write('0');
  }
  lcd.write(OFF_MINUTE);

} 



float get_current(){
  int read = analogRead(CURRENT_PIN);
  float current = read/float(1024) * CURRENT_SCALAR;
  return current;
  Log.verbose(F("Current: %d\n"), current);
}

*/
