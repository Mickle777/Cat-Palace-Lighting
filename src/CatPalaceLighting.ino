#include <SimpleTimer.h>
#include <Logging.h>
#include "BH1750FVI.h"
#include "config.h"

BH1750FVI light_sensor;
int light_level;

SimpleTimer timer;
int light_timeout = -1;
int threshold_timeout = -1;


void setup(){
    Log.Init(LOG_LEVEL_VERBOSE, 57600);
    Log.Info("Starting cat palace lighting");

    start_lights();
    start_light_sensor();
}


void loop(){
    timer.run();
}


void start_lights(){
    switch_lights(false);
}


void switch_lights(byte lamp_state){
    /**
    * Switch the lights on or off
    * :lamp_state: true for lights on; false for off
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


void start_light_sensor(){
    /**
    * Get the light sensor ready for periodic reads
    */
    light_sensor.begin();
    light_sensor.SetMode(Continuous_H_resolution_Mode);

    timer.setInterval(CHECK_SENSOR_INTERVAL, check_light_sensor);
    check_light_sensor();
}


void check_light_sensor(){
    /**
    * Read the light sensor
    * Values are read into the light_level global variable
    */
    light_level = light_sensor.GetLightIntensity();

    if (light_level >= MIN_LIGHT_THRESHOLD){
        // Above threshold, turn lights off and stop any timers
        switch_lights(false);
        timer.restartTimer(threshold_timeout);
        timer.disable(light_timeout);
    }
    else{
        // Below the threshold - Start threshold countdown/leave it alone
        if (threshold_timeout < 0){
            threshold_timeout = timer.setTimeout(THRESHOLD_TIMEOUT, begin_light_time);
        }
        else{
            timer.restartTimer(threshold_timeout);
        }
    }
}


void begin_light_time(){
    /**
    * Start the lighting time
    */
    switch_lights(true);

    if (light_timeout > 0){
        timer.restartTimer(light_timeout);
    }else{
        light_timeout = timer.setTimeout(ON_TIME, end_light_time);
    }
}


void end_light_time(){
    switch_lights(false);

    light_timeout = -1;
}
