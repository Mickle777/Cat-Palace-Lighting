#include <Arduino.h>

// Globals
const long CHECK_SENSOR_INTERVAL = 1000;
const long THRESHOLD_TIMEOUT = 10*60*1000;
const long ON_TIME = 6*60*60*1000;
const int RED_PIN = 5;
const int GREEN_PIN = 3;
const int BLUE_PIN = 6;

const int MIN_LIGHT_THRESHOLD = 20;


void start_lights();
void start_light_sensor();
void check_light_sensor();
