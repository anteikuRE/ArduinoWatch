#include <Arduino.h>
#include "Globals.h"
#include "Oled.h"
#include "Buttons.h"
#include "RTClock.h"
#include "utils/Buzzer.h"
#include "utils/Handlers.h"
#include "Alarm.h"

void setup() {
  Serial.begin(9600);

  setupDisplay();
  setupButtons();
  setupBuzzer();
  setupClock();

  updateDisplayBuffer();
}

void loop() {
  unsigned long currentMillis = millis();

  refreshTimeState();
  readButtons(currentMillis);
  checkModeToggle(currentMillis);
  checkSettingsTimeout(currentMillis);
  handleAdjustments(currentMillis);
  handleAlarmAddition(currentMillis);
  handleAlarmListNavigation(currentMillis);
  handleAlarmDeletion(currentMillis);
  handleSettingsBlink(currentMillis);
  checkAlarm(currentMillis);

  updateDisplayBuffer();
  saveButtonStates();
}
