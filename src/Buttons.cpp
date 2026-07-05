#include "Buttons.h"
#include "Globals.h"
#include "Display.h"

void setupButtons() {
  pinMode(btnUp, INPUT);
  pinMode(btnDown, INPUT);
}

void readButtons(unsigned long currentMillis) {
  upState = digitalRead(btnUp) == HIGH ? LOW : HIGH;
  downState = digitalRead(btnDown) == HIGH ? LOW : HIGH;
}

void saveButtonStates() {
  lastBtnUpState = upState;
  lastBtnDownState = downState;
}

// Handles the 3-second hold to enter/exit settings
void checkModeToggle(unsigned long currentMillis) {
  if (upState == LOW && downState == LOW) {
    if (!holdingBoth) {
      holdingBoth = true;
      bothHoldTimer = currentMillis;
      modeJustChanged = false;
    } else if (!modeJustChanged && (currentMillis - bothHoldTimer >= 3000)) {
      // 3 seconds reached! Toggle mode.
      inSettingsMode = !inSettingsMode;
      modeJustChanged = true;
      lastActivityTimer = currentMillis; // Reset the idle timer!
      blinkOn = true;
      previousBlinkMillis = currentMillis;
      updateDisplayBuffer();
    }
  } else {
    holdingBoth = false;
  }
}

// Kicks you out of settings if you do nothing for 10 seconds
void checkSettingsTimeout(unsigned long currentMillis) {
  if (inSettingsMode && !holdingBoth) {
    if (currentMillis - lastActivityTimer >= 10000) {
      inSettingsMode = false; // Auto-exit
      updateDisplayBuffer();
    }
  }
}

// Handles button presses to change the time
void handleAdjustments(unsigned long currentMillis) {
  if (!inSettingsMode || holdingBoth) return; // Only adjust in settings mode

  bool timeChanged = false;

  // Minute UP
  if (upState == LOW && lastBtnUpState == HIGH) {
    minutes++;
    seconds = 0;
    if (minutes >= 60) { minutes = 0; hours++; if (hours >= 24) hours = 0; }
    timeChanged = true;
  }

  // Minute DOWN
  if (downState == LOW && lastBtnDownState == HIGH) {
    minutes--;
    seconds = 0;
    if (minutes < 0) { minutes = 59; hours--; if (hours < 0) hours = 23; }
    timeChanged = true;
  }

  if (timeChanged) {
    lastActivityTimer = currentMillis; // Reset the 10-second timeout!
    updateDisplayBuffer();
    delay(50); // debounce
  }
}

// Handles the colon-blink animation while in settings mode
void handleSettingsBlink(unsigned long currentMillis) {
  if (!inSettingsMode) return;

  if (currentMillis - previousBlinkMillis >= blinkInterval) {
    previousBlinkMillis = currentMillis;
    blinkOn = !blinkOn;
    updateDisplayBuffer();
  }
}
