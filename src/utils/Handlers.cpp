
#include "Buttons.h"
#include "Alarm.h"
#include "Globals.h"
#include "Oled.h"

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

  adjustTimeField(timeChanged, alarms[editingAlarmIndex].hour, alarms[editingAlarmIndex].minute, alarms[editingAlarmIndex].seconds);

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

void handleAlarmListNavigation(unsigned long currentMillis) {
  if (!inViewAlarmsMode || holdingBoth || holdingTriple) return;

  bool selectionChanged = false;

  if (upState == LOW && lastBtnUpState == HIGH) {
    selectedAlarmIndex--;
    if (selectedAlarmIndex < 0) selectedAlarmIndex = MAX_ALARMS - 1; // wrap to bottom
    selectionChanged = true;
  }

  if (downState == LOW && lastBtnDownState == HIGH) {
    selectedAlarmIndex++;
    if (selectedAlarmIndex >= MAX_ALARMS) selectedAlarmIndex = 0; // wrap to top
    selectionChanged = true;
  }

  if (selectionChanged) {
    lastActivityTimer = currentMillis;
    updateDisplayBuffer(); // or drawAlarmList() directly, depending on your dispatch
    delay(50); // debounce
  }
}

bool lastAlarmStopState = HIGH; // tracks previous state for edge detection

void handleAlarmDeletion(unsigned long currentMillis) {
  if (!inViewAlarmsMode || upState == LOW || downState == LOW) {
    lastAlarmStopState = alarmStopState;
    return;
  }

  // Button just pressed
  if (alarmStopState == LOW && lastAlarmStopState == HIGH) {
    digitalWrite(buzzerPin, HIGH); // Quick beep to indicate deletion
    delay(50);
    digitalWrite(buzzerPin, LOW);

    deleteAlarm(selectedAlarmIndex);
    lastActivityTimer = currentMillis;
    updateDisplayBuffer();
    delay(50);
  }

  lastAlarmStopState = alarmStopState;
}

