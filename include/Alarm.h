#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

void checkAlarm(unsigned long currentMillis);
void handleBuzzer(unsigned long currentMillis);
void stopAlarm();

#endif