#include "Arduino.h"
#include "esp_camera.h"
#include <stdio.h>
#include "SD.h"
#include "Leds.h"

unsigned int fileIndex = 0;
unsigned int lapseIndex = 0;
unsigned long TIMELAPSINTERVAL = 1;
unsigned long long LAPSE_MAX = 0;
unsigned long long LAPSE_MAX_PERIOD_MSEC = 0;
bool mjpeg = true;
bool lapseRunning = false;
unsigned long nexttimelaps = 0;

unsigned long TimeLapseGetInterval() { return TIMELAPSINTERVAL; }
unsigned long TimeLapseGetDuration() { //in minutes
    return LAPSE_MAX_PERIOD_MSEC / 60ULL / 1000ULL;
}

unsigned long TimeLapseGetRemainigDurationSeconds() { 
    auto now = millis();
    if(now > LAPSE_MAX) {
        return 0;
    }
    return  (LAPSE_MAX - now) / 1000UL;
};

void TimeLapseSetInterval(unsigned long interval)
{
    TIMELAPSINTERVAL = interval;
}

void TimeLapseSetDuration(unsigned long maxDurationMinutes)
{
    LAPSE_MAX_PERIOD_MSEC = 60ULL * 1000ULL * maxDurationMinutes;
    Serial.printf("Setting Time Lapse duration (0 means inf.) -> %llu[msec], %lu[min]\r\n", LAPSE_MAX_PERIOD_MSEC, maxDurationMinutes);
}

bool TimeLapseStart()
{
    Serial.printf("Starting time lapse (interval: %lu[s]", TIMELAPSINTERVAL);
    if(LAPSE_MAX_PERIOD_MSEC != 0) {
        LAPSE_MAX = LAPSE_MAX_PERIOD_MSEC + millis();
        Serial.printf(", duration: %lu[min])\r\n", TimeLapseGetDuration());
    } else {
        LAPSE_MAX = 0;
        Serial.printf(", duration: infinity)\r\n");
    }
    if(lapseRunning) return true;
    fileIndex = 0;
    char path[32];
    for(; lapseIndex < 10000; lapseIndex++)
    {
        sprintf(path, "/lapse%03u", lapseIndex);
        if (!SDFileExists(path))
        {
            SDCreateDir(path);
            lapseRunning = true;
            return true;
        }
    }
	return false;
}

bool TimeLapseStop()
{
    Serial.printf("Stopping time lapse\r\n");
    lapseRunning = false;
    return true;
}

bool TimeLapseProcess()
{
    if(!lapseRunning) return false;
    if(nexttimelaps >  millis() ) return false;
    if(LAPSE_MAX != 0 &&  LAPSE_MAX < millis()) {TimeLapseStop();}
    nexttimelaps = millis() + (1000 * TIMELAPSINTERVAL);

    camera_fb_t *fb = NULL;
    OnAir _;
    fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Camera capture failed");
        return false;
    }

    char path[32];
    sprintf(path, "/lapse%03u/pic%05u.jpg", lapseIndex, fileIndex);
    Serial.println(path);
    if(!SDWriteFile(path, (const unsigned char *)fb->buf, fb->len))
    {
        lapseRunning = false;
        return false;
    }
    fileIndex++;
    esp_camera_fb_return(fb);

    return true;
}
