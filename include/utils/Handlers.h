#ifndef HANDLERS_H
#define HANDLERS_H

#include <Arduino.h>

void handleAdjustments(unsigned long currentMillis);
void handleAlarmAddition(unsigned long currentMillis);
void handleAlarmListNavigation(unsigned long currentMillis);
void handleAlarmDeletion(unsigned long currentMillis);
void handleSettingsBlink(unsigned long currentMillis);

#endif