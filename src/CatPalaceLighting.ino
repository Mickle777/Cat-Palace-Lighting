////////////////////////////////////////////////////////////////////
// Cat Palace Lighting

///////////////////////////////////////////////////////////////////////////////
// Library imports

#include <SimpleTimer.h>
#include <Logging.h>
#include "BH1750FVI.h"
#include "config.h"
#include <LiquidCrystal.h>


///////////////////////////////////////////////////////////////////////////////
//  Object initialisation

BH1750FVI light_sensor;

SimpleTimer timer;
int lamp_duration_timer_id = -1;
int threshold_timer_id = -1;

bool new_night = false; // Whether or not the lamp has run already on the same night (prevents lamp coming on more than once)
int threshold_timeout = THRESHOLD_TIMEOUT_MINUTES;
int remaining_lamp_minutes = 0;

LiquidCrystal lcd(LCD_RS_PIN,
    LCD_ENABLE_PIN,
    LCD_DATA_0_PIN,
    LCD_DATA_1_PIN,
    LCD_DATA_2_PIN,
    LCD_DATA_3_PIN);


///////////////////////////////////////////////////////////////////////////////
// Main Functions

void setup(){
    /**
    * System initialisation
    */

    Log.Init(LOG_LEVEL_VERBOSE, SERIAL_BAUD);
    Log.Info("Starting cat palace lighting");

    reset_lamp();
    start_light_sensor();
    lcd.begin(LCD_NUM_COLUMNS, LCD_NUM_ROWS);
}


void loop(){
    /**
    * Main loop
    */

    timer.run();
}


///////////////////////////////////////////////////////////////////////////////
// Light control

void switch_lamp(byte lamp_state){
    /**
    * Switch the lights on or off
    * @param lamp_state true for lights on; false for off
    */

    byte level;
    if (lamp_state){
        level = 255;
    }
    else{
        level = 0;
    }

    switch_lamp(level, level, level);
}

void switch_lamp(int r, int g, int b){
    /** Switch the LED strip to a specific colour.
    * @param r value of red channel
    * @param g value of green channel
    * @param b value of blue channel
    */

    constrain(r, 0, 255);
    digitalWrite(RED_PIN, r);

    constrain(g, 0, 255);
    digitalWrite(GREEN_PIN, g);

    constrain(b, 0, 255);
    digitalWrite(BLUE_PIN, b);
}


void start_threshold_timer(int timeout_minutes=THRESHOLD_TIMEOUT_MINUTES){
    /** Start threshold counting for turning on lamp
    * The threshold timer is a time buffer (default: 10 minutes) to verify that ambient light level.
    * Ambient light levels above the threshold (default: 20lux) will restart the threshold timer.
    * If the timer reaches its end (after it's been dark for 10 min), the lamp will turn on.
    *
    * | Low light | >> Threshold timer >> | >> Lamp ON timer >>>>>>>>>>>|
    *                  ^                                  |
    *                  |< ------High light ----------
    *
    *@param timeout_minutes duration of threshold timer in minutes (default: 10 minutes)
    */

    threshold_timeout = timeout_minutes;
    timer.setTimeout(ONE_MINUTE, check_threshold_timer);
}


void check_threshold_timer(){
    /**
    * Recursive callback function for threshold timer
    * This function is called every minute and decrements the threshold timeout.
    * Once the threshold timeout reaches 0, the lamp will turn on
    */

    threshold_timeout--;

    if (threshold_timeout <= 0 ){
        Log.Debug("Threshold timeout finished - Lights on!");
        begin_lamp_on_timer();

    }else{
        Log.Debug("Threshold timeout: %d  minutes remaining until the lights come on", threshold_timeout);
        timer.setTimeout(ONE_MINUTE, check_threshold_timer);
    }
}


void begin_lamp_on_timer(){
    /**
    * Start the lighting time
    */
    switch_lamp(true);
    lamp_duration_timer_id = timer.setTimeout(ONE_MINUTE, reset_lamp);
    remaining_lamp_minutes = LIGHT_ON_TIMER_MINUTES;
}


void check_lamp_on_timer(){
    /**
    * Recursive callback for lighting timer.
    * The function is called every minute and decrements the remaining on-minutes of the lamp.
    * When the on-minutes falls to zero, the lights have been on for their full duration and will turn off
    */

    remaining_lamp_minutes--;
    if (remaining_lamp_minutes < 0){
        timer.setTimeout(ONE_MINUTE, check_lamp_on_timer);
    }
    else{
        reset_lamp();
        new_night = false;
    }
}


void reset_lamp(){
    /** Set the lamp and timers to their initial states (off).
    * Resetting the lamp will also reset the new_night status.
    * This means the lamp will be allowed to turn on un
    */

    switch_lamp(false);
    new_night = true;

    threshold_timeout = THRESHOLD_TIMEOUT_MINUTES;
    remaining_lamp_minutes = 0;
}


///////////////////////////////////////////////////////////////////////////////
// Light Sensor

void start_light_sensor(){
    /**
    * Ready the BH1750FVI illuminance sensor for periodic reads
    */
    light_sensor.begin();
    light_sensor.SetMode(Continuous_H_resolution_Mode);

    // Start regular callbacks to check light sensor
    timer.setInterval(CHECK_SENSOR_INTERVAL, check_light_sensor);
    check_light_sensor();
}


void check_light_sensor(){
    /** Check the ambient light sensor for lamp switching.
    * Turn lights off if the ambient light level is above the minimum threshold.
    * If the ambient light level is below the threshold, start the threshold timeout.
    * The lamp will turn on of the threshold timeout finishes.
    * Ambient light levels above the threshold will also cancel the timeout.
    */

    int light_level = light_sensor.GetLightIntensity();
    Log.Debug("Light level: %d lux", light_level);

    // Ambient light above threshold - Turn off light and cancel threshold timeout
    if (light_level >= MIN_LIGHT_THRESHOLD){
        reset_lamp();
    }

    // Ambient light below threshold - Start the threshold timeout
    else{
        // Only start the threshold timer if the the lamp is off
        if (remaining_lamp_minutes <= 0){
            start_threshold_timer();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Power monitor

void check_power(){
    /** Measure the external voltage and current consumption.
    * Power is calculated and printed to screen.
    */
    int voltage = get_voltage(3);
    int current = get_current(10);

    int power = voltage * current; // Get power in milliWatts

    Log.Info("Voltage: %d mV,\t Current: %d mA,\t Power: %d mW", voltage, current, power);

    lcd.setCursor(0,0);
    lcd.print("Power ");
    lcd.print(power);
    lcd.print("mW");
}


int get_voltage(int samples=1){
    /** Read the external power voltage.
    * External power is read through a shunt voltage divider circuit.
    * The voltage divider ratio can be adjusted using the VOLTAGE_RATIO constant in the config file.
    *
    *@param samples Number of voltage reads to perform
    *@ret Average recorded voltage from sampling (in mV)
    */

    int voltage= analogRead(VOLTAGE_READ_PIN);

    // Average additional samples to smooth out reading accuracy
    for (int i = 1; i < samples; i++) {
        int new_voltage = analogRead(VOLTAGE_READ_PIN);
        voltage = (voltage + new_voltage)/2;
        delay(1);
    }

    voltage = map(voltage, 0, 1024, 0, MEASURE_VOLTAGE);    //Convert counts to mV
    return voltage;
}


int get_current(int samples=1){
    /** Read the external current consumption.
    * DC current consumption is measured from the ACS712 hall-effect sensor
    * Current can be read from the sensor as an analog voltage with a ratio of 185mV/A.
    * The current ratio can be adjusted with the CURRENT_RATIO constant in the config file.
    *
    *@param samples Number of current reads to perform
    *@ret Average recorded current from sampling (in mA)
    */

    int current = analogRead(VOLTAGE_READ_PIN);

    // Average additional samples to smooth out reading accuracy
    for (int i = 1; i < samples; i++) {
        int new_current = analogRead(VOLTAGE_READ_PIN);
        current = (current + new_current)/2;
        delay(1);
    }

    current = map(current, 0, 1024, 0, SUPPLY_VOLTAGE);    //Convert counts to mV
    current = long(current * 1000) / CURRENT_RATIO; // Convert mV to mA


    return current;
}
