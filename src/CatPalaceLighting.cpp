////////////////////////////////////////////////////////////////////
// Cat Palace Lighting

///////////////////////////////////////////////////////////////////////////////
// Library imports

#include <Arduino.h>
#include <RTClib.h>
#include <ArduinoLog.h>
#include <Wire.h>
#include <LampControl.h>

// Function prototypes so the compiler shuts up...
void check_time();
void check_light();

// Logging/Comms
const long SERIAL_BAUD = 57600;
const int LOGGER_LEVEL = LOG_LEVEL_VERBOSE;

// RTC
RTC_DS3231 rtc;
const int ON_HOUR = 5;
const int ON_MINUTE = 30;
const int OFF_HOUR = 23;
const int OFF_MINUTE = 0;
bool lights_enabled;
long time_of_last_check;
const long TIME_BETWEEN_CHECKS = 10000;

// LDR
const int LDR_PIN = A0;
const int LDR_LOWER_THRESHOLD = 30;
const int LDR_UPPER_THRESHOLD = 80;

// Lamp
const int LAMP_PIN = 5;
LampControl light(LAMP_PIN);


///////////////////////////////////////////////////////////////////////////////
// Main Functions

void setup(){
    Serial.begin(SERIAL_BAUD);
    Log.begin(LOGGER_LEVEL, &Serial);
    Log.notice(F("Working...\n"));

    rtc.begin();
    DateTime now = rtc.now();
    time_of_last_check = millis();
    Log.notice(F("Time is %d:%d\n"), now.hour(), now.minute());

    pinMode(LDR_PIN, INPUT);

    lights_enabled = true;
}


void loop(){
    if (abs(millis() - time_of_last_check) > TIME_BETWEEN_CHECKS) {
      check_time();
      check_light();
      time_of_last_check = millis();
    }

    light.tick();
}


///////////////////////////////////////////////////////////////////////////////
// Functions

void check_time(){
  /**
  * Enables/disables the lamp according to time
  */

    DateTime now = rtc.now();
    Log.verbose(F("Time check:\t%d:%d:%d\n"), now.hour(), now.minute(), now.second());

    if (!lights_enabled){
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
        Log.verbose(F("LDR level: %d\n"), ldr_level);

        // Check levels and switch lamp accordingly
        if (light.is_active){
            if (ldr_level > LDR_UPPER_THRESHOLD){
                light.deactivate_lamp();
            }
        }else{
            if (ldr_level < LDR_LOWER_THRESHOLD){
                light.timeout_enabled = false;
                light.activate_lamp();
            }
        }
    }
}
