#include <Arduino.h>

///////////////////////////////////////////////////////////////////////////////
// Config

// Timers
const long CHECK_SENSOR_INTERVAL = 10000;    // Frequency of sensor checking in ms
const long THRESHOLD_TIMEOUT = 10*60*1000LL;
const int ONE_MINUTE = 60000;   // One minute in milliseconds

// Switching options
const int MIN_LIGHT_THRESHOLD = 20;     //
const int LIGHT_ON_TIMER_MINUTES = 360;     // On-time for the lamp in minutes ]

// LCD Options
const int LCD_NUM_ROWS = 2;     // Number of rows on the LCD display
const int LCD_NUM_COLUMNS = 16;     // Number of columns on the LCD display

// Comms
const long SERIAL_BAUD = 57600;


///////////////////////////////////////////////////////////////////////////////
// Pin Assignments

const int RED_PIN = 3;  // Pin for Red channel of RGB LED strip (PWM)
const int GREEN_PIN = 5;    // Pin for Green channel of RGB LED strip (PWM)
const int BLUE_PIN = 6;     // Pin for Blue channel of RGB LED strip (PWM)

const int LCD_RS_PIN = 4;
const int LCD_ENABLE_PIN = 7;
const int LCD_DATA_0_PIN = 8;
const int LCD_DATA_1_PIN = 9;
const int LCD_DATA_2_PIN =10;
const int LCD_DATA_3_PIN = 11;

void start_lights();
void start_light_sensor();
void check_light_sensor();
