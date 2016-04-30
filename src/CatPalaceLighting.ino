#include <SimpleTimer.h>
#include <Logging.h>
#include "BH1750FVI.h"
#include "config.h"
#include <LiquidCrystal.h>
#include <RTCLib.h>

///////////////////////////////////////////////////////////////////////////////
//  Object initialisationd
BH1750FVI light_sensor;
int light_level;

SimpleTimer timer;
int light_timeout = -1;
int threshold_timeout = -1;
int remaining_minutes = 0;

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

    start_lights();
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

void start_lights(){
    /**
    * Set the initial conditions of the lights (off)
    */
    switch_lights(false);
}


void switch_lights(byte lamp_state){
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
    digitalWrite(RED_PIN, level);
    digitalWrite(GREEN_PIN, level);
    digitalWrite(BLUE_PIN, level);
}


void begin_light_time(){
    /**
    * Start the lighting time
    */
    switch_lights(true);

    if (light_timeout > 0){
        timer.restartTimer(light_timeout);
    }else{
        light_timeout = timer.setTimeout(ONE_MINUTE, end_light_time);
    }

    remaining_minutes = LIGHT_ON_TIMER_MINUTES;
}


void check_light_timer(){
    remaining_minutes--;
    if (remaining_minutes <= 0){
        light_timeout = timer.setTimeout(ONE_MINUTE, check_light_timer);
    }
}


void end_light_time(){
    switch_lights(false);
    light_timeout = -1;
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
    /**
    * Turn lights off if the ambient light level is above the minimum threshold.
    * If the ambient light level is below the threshold, start the threshold timeout.
    * The lamp will turn on of the threshold timeout finishes.
    * Ambient light levels above the threshold will also cancel the timeout.
    */

    light_level = light_sensor.GetLightIntensity();
    Log.Debug("Light level: %d lux", light_level);

    // Ambient light above threshold - Turn off light and cancel threshold timeout
    if (light_level >= MIN_LIGHT_THRESHOLD){
        switch_lights(false);
        timer.restartTimer(threshold_timeout);
        timer.disable(light_timeout);
        remaining_minutes = 0;
    }

    // Ambient light below threshold - Start the threshold timeout
    else{

        // No existing threshold timeout and light is off - create new timeout
        if (!timer.isEnabled(threshold_timeout) && remaining_minutes > 0){
            threshold_timeout = timer.setTimeout(THRESHOLD_TIMEOUT, begin_light_time);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Power monitor

void check_power(){
    //TODO Schedule regular power check
}

float get_voltage(){
    //TODO Calculate the voltage from the power monitor
}

float get_current(){
    //TODO Calculate the current from the power monitor
}
