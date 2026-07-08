#include "Buttons.h"
#include "Globals.h"
#include "Oled.h"

void setupButtons() {
  pinMode(btnUp, INPUT);
  pinMode(btnDown, INPUT);
  pinMode(btnAlarmStop, INPUT);
}

void readButtons(unsigned long currentMillis) {
  upState = digitalRead(btnUp) == HIGH ? LOW : HIGH;
  downState = digitalRead(btnDown) == HIGH ? LOW : HIGH;
  alarmStopState = digitalRead(btnAlarmStop) == HIGH ? LOW : HIGH;
}

void saveButtonStates() {
  lastBtnUpState = upState;
  lastBtnDownState = downState;
  lastBtnAlarmStopState = alarmStopState;
}

bool holdingAlarmStop = false;
bool alarmModeJustChanged = false;
unsigned long alarmHoldTimer = 0;

// Handles the 3-second hold to enter/exit settings
void checkModeToggle(unsigned long currentMillis) {
 if (alarmStopState == LOW) {
    if (!holdingAlarmStop) {
      holdingAlarmStop = true;
      alarmHoldTimer = currentMillis;
      alarmModeJustChanged = false;
    } else if (!alarmModeJustChanged && (currentMillis - alarmHoldTimer >= 3000)) {
      inAlarmSettingsMode = !inAlarmSettingsMode;
      alarmModeJustChanged = true;
      lastActivityTimer = currentMillis; // Reset the idle timer!
      blinkOn = true;
      previousBlinkMillis = currentMillis;
      updateDisplayBuffer();
    }
  } else {
    holdingAlarmStop = false;
  }


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
  if(inAlarmSettingsMode && !holdingAlarmStop) {
    if (currentMillis - lastActivityTimer >= 10000) {
      inAlarmSettingsMode = false; // Auto-exit
      updateDisplayBuffer();
    }
  }
}

void adjustTimeField(bool &timeChanged, int &hours, int &minutes, int &seconds) {

  // UP button
  if (upState == LOW && lastBtnUpState == HIGH) {
    if (currentField == EDIT_HOURS) {
      hours++;
      if (hours >= 24) hours = 0;
    } else {
      minutes++;
      seconds = 0;
      if (minutes >= 60) {
        minutes = 0;
        hours++;
        if (hours >= 24) hours = 0;
      }
    }
    timeChanged = true;
  }

  // DOWN button
  if (downState == LOW && lastBtnDownState == HIGH) {
    if (currentField == EDIT_HOURS) {
      hours--;
      if (hours < 0) hours = 23;
    } else {
      minutes--;
      seconds = 0;
      if (minutes < 0) {
        minutes = 59;
        hours--;
        if (hours < 0) hours = 23;
      }
    }
    timeChanged = true;
  }
}

// Handles button presses to change the time
void handleAlarmAddition(unsigned long currentMillis) {
  if (!inAlarmSettingsMode) return; // Only adjust in settings mode
  long modeDelayTime = 5000;
  bool timeChanged = false;

 // If 5 seconds pass with no field switch, toggle between hours/minutes
  if (currentMillis - lastFieldSwitchMillis >= modeDelayTime) {
    currentField = (currentField == EDIT_HOURS) ? EDIT_MINUTES : EDIT_HOURS;
    lastFieldSwitchMillis = currentMillis;
  }

  int tempHours = hours;
  int tempMinutes = minutes;
  int tempSeconds = seconds;
  adjustTimeField(timeChanged, tempHours, tempMinutes, tempSeconds);

  if (timeChanged) {
    lastActivityTimer = currentMillis; // Reset the 10-second timeout!
    updateDisplayBuffer();
    delay(50); // debounce
  }
}

// Handles button presses to change the time
void handleAdjustments(unsigned long currentMillis) {
  if (!inSettingsMode || holdingBoth) return; // Only adjust in settings mode
  long modeDelayTime = 5000;
  bool timeChanged = false;

 // If 5 seconds pass with no field switch, toggle between hours/minutes
  if (currentMillis - lastFieldSwitchMillis >= modeDelayTime) {
    currentField = (currentField == EDIT_HOURS) ? EDIT_MINUTES : EDIT_HOURS;
    lastFieldSwitchMillis = currentMillis;
  }

  adjustTimeField(timeChanged, hours, minutes, seconds);

  if (timeChanged) {
    lastActivityTimer = currentMillis; // Reset the 10-second timeout!
    updateDisplayBuffer();
    delay(50); // debounce
  }
}

// Handles the colon-blink animation while in settings mode
void handleSettingsBlink(unsigned long currentMillis) {
  if (!inSettingsMode && !inAlarmSettingsMode) return;

  if (currentMillis - previousBlinkMillis >= blinkInterval) {
    previousBlinkMillis = currentMillis;
    blinkOn = !blinkOn;
    updateDisplayBuffer();
  }
}
