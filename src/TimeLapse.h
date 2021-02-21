#pragma once

extern unsigned long TIMELAPSINTERVAL;

void TimeLapseSetInterval(unsigned long delta);
void TimeLapseSetDuration(unsigned long delta);
unsigned long TimeLapseGetInterval();
unsigned long TimeLapseGetDuration();
unsigned long TimeLapseGetRemainigDurationSeconds();

bool TimeLapseStart();
bool TimeLapseStop();
bool TimeLapseProcess();
