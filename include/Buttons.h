#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

void setupButtons();
void readButtons(unsigned long currentMillis);
void saveButtonStates();
void checkModeToggle(unsigned long currentMillis);
void checkSettingsTimeout(unsigned long currentMillis);
void handleAdjustments(unsigned long currentMillis);
void handleSettingsBlink(unsigned long currentMillis);

#endif
