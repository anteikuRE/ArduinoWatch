#include <Arduino.h>
#include "Globals.h"
#include "Display.h"
#include "Buttons.h"
#include "Clock.h"
#include "Buzzer.h"

void setup() {
  setupDisplay();
  setupButtons();
  setupBuzzer();

  // Grab the compile time to set initial clock
  sscanf(__TIME__, "%d:%d:%d", &hours, &minutes, &seconds);

  updateDisplayBuffer();
}

void loop() {
  unsigned long currentMillis = millis();

  readButtons(currentMillis);
  checkModeToggle(currentMillis);
  checkSettingsTimeout(currentMillis);
  handleAdjustments(currentMillis);
  tickClock(currentMillis);
  handleSettingsBlink(currentMillis);
  // handleBuzzer();

  // Must run every loop pass, as fast as possible, or the digits flicker/dim.
  refreshDisplayMux();

  saveButtonStates();
}
