#include <Arduino.h>
#include "camera.h"
#include "WiFi_func.h"
#include "SD.h"
#include "TimeLapse.h"
#include "HTTPApp.h"
#include "version.h"
#include "version_build.h"
#include "Pref.h"
#include "Leds.h"
#include "mDNS.h"
#include "OTA.h"

bool STOP_RESET = false;
unsigned long ESP_RESTART = 0;


void setup() 
{  
  bool allOK=true;
  Serial.begin(115200);
  Serial.println("=======================================");
  Serial.println("ESP32 Timelapse Webcam");
  Serial.println("Sketch: " VERSION_MAJOR "." VERSION_MINOR "." VERSION_PATCH "." BUILD_COMMIT "-" BUILD_BRANCH);
  Serial.println("Builddate: " BUILD_DATE " " BUILD_TIME);

  initializeLEDs();

  if (PrefLoadInt("clearsettings",1,true)) { PrefClear(); }
  PrefSaveInt("clearsettings",1 , true);
  
  WiFiInit();
  initOTA();


  SDInitFileSystem();
  if(!CameraInit()) {
    Serial.println("Failed initializing camera - server will be defunct");
    ESP_RESTART = millis() + 20000; //self restart in 20 s
    allOK = false;
  } else {
    if(CameraLoadSettings()) { 
      Serial.println("Setting Load OK"); 
    } else { 
      Serial.println("Setting Load FAILED"); 
      allOK = false;
    }
  }

  HTTPAppStartCameraServer();
  if(allOK) {
    setStatusLedState(StatusLedState::DIMMED);
  }
}

unsigned long OTA_INTERVAL_SECONDS = 5;
unsigned long nextOtaCheck = 0;

void loop() 
{
	TimeLapseProcess();
  if(millis() > 1000*10 && STOP_RESET == false) 
  { 
    Serial.println("Stop config reset on boot");
    PrefSaveInt("clearsettings",0 , true); 
    STOP_RESET = true;
  }

  if(ESP_RESTART != 0 && millis() > ESP_RESTART)
  {
    ESP.restart();
  }

  if(nextOtaCheck <=  millis() ) {
    nextOtaCheck = millis() + (1000 * OTA_INTERVAL_SECONDS);
    otaHandle();
  }
}
