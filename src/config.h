#include <Arduino.h>

///////////////////////////////////////////////////////////////////////////////
// Config

// Timers
const long CHECK_SENSOR_INTERVAL = 10000;   // Frequency of sensor checking in ms
const long THRESHOLD_TIMEOUT_MINUTES = 10;  // Duration of threshold timeout in minutes
const int ONE_MINUTE = 60000;   // One minute in milliseconds

// Switching options
const int MIN_LIGHT_THRESHOLD = 20; // Maximum ambient light threshold to turn on the lamps (in lux)
const int LIGHT_ON_TIMER_MINUTES = 360; // On-time for the lamp in minutes ]

// LCD Options
const int LCD_NUM_ROWS = 2; // Number of rows on the LCD display
const int LCD_NUM_COLUMNS = 16; // Number of columns on the LCD display

// Comms
const long SERIAL_BAUD = 57600; // Baud rate for serial comms

// Power monitoring
const float VOLTAGE_RATIO = 0.5;    // Ratio of measured voltage to actual voltage (voltage divider)
const int SUPPLY_VOLTAGE = 5000;    // Supply voltage of Arduino in mV
const int MEASURE_VOLTAGE = SUPPLY_VOLTAGE/VOLTAGE_RATIO;
const int CURRENT_RATIO = 185;  // Ratio between measured mV and  current in A
const int NUM_VOLTAGE_SAMPLES = 3;  // Number of sample reads for voltage monitoring
const int NUM_CURRENT_SAMPLES = 10; // Number of sample reads for current monitoring

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

const int VOLTAGE_READ_PIN = A0;
const int CURRENT_READ_PIN = A1;


///////////////////////////////////////////////////////////////////////////////
// Function Headers

void reset_lamp();
void switch_lamp(byte lamp_state);
void switch_lamp(int r, int g, int b);
void start_threshold_timer(int timeout_minutes);
void check_threshold_timer();
void begin_lamp_on_timer();
void check_lamp_on_timer();
void start_light_sensor();
void check_light_sensor();
void check_power();
int get_voltage(int samples);
int get_current(int samples);
