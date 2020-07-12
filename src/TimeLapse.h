#pragma once

extern unsigned long TIMELAPSINTERVAL;

void TimeLapseSetInterval(unsigned long delta);
bool TimeLapseStart();
bool TimeLapseStop();
bool TimeLapseProcess();
