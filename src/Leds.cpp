#include "Leds.h"
#define INTERNAL_LED_PWM 1

const int PWM_FREQUENCY = 5000;
#if INTERNAL_LED_PWM
    const int PWM_CHANNEL_BUILTIN_LED = 14;
#endif
const int PWM_CHANNEL_FLASH = 15;
const int PWM_RESOLUTION = 8;


void initializeLEDs()
{    
    #if INTERNAL_LED_PWM 
        ledcSetup(PWM_CHANNEL_BUILTIN_LED, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(BUILTIN_LED_PIN, PWM_CHANNEL_BUILTIN_LED);        
    #else
        pinMode(BUILTIN_LED_PIN, OUTPUT);
    #endif
    setStatusLedState(StatusLedState::OFF);
    
    #ifdef USE_SDMMC_1BIT    
        ledcSetup(PWM_CHANNEL_FLASH, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(LED_FLASH, PWM_CHANNEL_FLASH);        
    #endif
}

uint8_t _brightnessToDutyCycle(uint8_t brigthnessPercent, bool reverse=false) {
    constexpr uint8_t PERCENT_MIN = 0U;
    constexpr uint8_t PERCENT_MAX = 100U;
    brigthnessPercent = max(brigthnessPercent, PERCENT_MIN);
    brigthnessPercent = min(brigthnessPercent, PERCENT_MAX);

    constexpr auto MAX_RES = (0x1 << PWM_RESOLUTION) -1;

    if(reverse) {
        return map(brigthnessPercent, PERCENT_MIN, PERCENT_MAX, MAX_RES, 0);    
    }
    return map(brigthnessPercent, PERCENT_MIN, PERCENT_MAX, 0, MAX_RES);    
}

void setStatusLedState(StatusLedState st)
{
    uint8_t internalLedBrigtnessPercent = 0;
    switch(st) {        
        case StatusLedState::ON:
            // Serial.printf("Back led ON\n");
            internalLedBrigtnessPercent = 100;
            break;
        case StatusLedState::DIMMED:
            // Serial.printf("Back led DIMMED\n");
            internalLedBrigtnessPercent = 25;
            break;
        case StatusLedState::OFF:
        default:
            // Serial.printf("Back led OFF\n");
            internalLedBrigtnessPercent = 0;
            break;
    }    
    #if INTERNAL_LED_PWM
        ledcWrite(PWM_CHANNEL_BUILTIN_LED, _brightnessToDutyCycle(internalLedBrigtnessPercent, true));
    #else
        digitalWrite(BUILTIN_LED_PIN, internalLedBrigtnessPercent > 0? LOW : HIGH);
    #endif
}

void setFlashLedBrightness(uint8_t brigthnessPercent)
{
    #ifdef USE_SDMMC_1BIT
        Serial.printf("DutyCycle: %d\n", _brightnessToDutyCycle(brigthnessPercent));
        ledcWrite(PWM_CHANNEL_FLASH, _brightnessToDutyCycle(brigthnessPercent));
    #endif
}

uint8_t getFlashLedBrigthess() {
    //could just do a global var, but...
    constexpr auto MAX_RES = (0x1 << PWM_RESOLUTION) -1;
    return map(ledcRead(PWM_CHANNEL_FLASH), 0, MAX_RES, 0, 100);   
}
