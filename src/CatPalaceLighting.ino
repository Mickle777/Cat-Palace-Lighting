#include <SimpleTimer.h>
#include <Logging.h>
#include "BH1750FVI.h"
#include "config.h"
#include <LiquidCrystal.h>

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

void setup(){
    Log.Init(LOG_LEVEL_VERBOSE, 57600);
    Log.Info("Starting cat palace lighting");

    start_lights();
    start_light_sensor();
    lcd.begin(16, 2);
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
        remaining_minutes = 0;
    }
    else{
        // Below the threshold - Start threshold countdown/leave it alone
        if (timer.isEnabled(threshold_timeout)){
            timer.restartTimer(threshold_timeout);
        }
        else{
            threshold_timeout = timer.setTimeout(THRESHOLD_TIMEOUT, begin_light_time);
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

void check_power(){}

float get_voltage(){

}

float get_current(){
    
}
