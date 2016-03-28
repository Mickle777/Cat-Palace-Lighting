#include <Arduino.h>

// Globals
const long CHECK_SENSOR_INTERVAL = 1000;
const long THRESHOLD_TIMEOUT = 10*60*1000;
const int LIGHT_ON_TIMER_MINUTES = 360;
const int ONE_MINUTE = 60000;
const int RED_PIN = 5;
const int GREEN_PIN = 3;
const int BLUE_PIN = 6;

const int LCD_RS_PIN = 4;
const int LCD_ENABLE_PIN = 7;
const int LCD_DATA_0_PIN = 8;
const int LCD_DATA_1_PIN = 9;
const int LCD_DATA_2_PIN =10;
const int LCD_DATA_3_PIN = 11;

const int MIN_LIGHT_THRESHOLD = 20;


void start_lights();
void start_light_sensor();
void check_light_sensor();
