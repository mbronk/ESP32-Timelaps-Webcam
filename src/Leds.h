#pragma once
#include <Arduino.h>
#include "settings.h"

enum class StatusLedState {
    OFF,
    DIMMED,
    ON
};


void initializeLEDs();
void setStatusLedState(StatusLedState st);

void setFlashLedBrightness(uint8_t brigthness);
uint8_t getFlashLedBrigthess();

class OnAir {
public:
    OnAir() 
    {
        setStatusLedState(StatusLedState::ON);
    }
    ~OnAir() {
        setStatusLedState(StatusLedState::DIMMED);
    }
};
