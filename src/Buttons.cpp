#include "Buttons.h"
#include "Alarm.h"
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

void saveTimeToRTC() {
  RtcDateTime current = Rtc.GetDateTime();
  RtcDateTime updated(
    current.Year(), current.Month(), current.Day(),
    hours, minutes, 0
  );
  Rtc.SetDateTime(updated);
}

bool holdingAlarmStop = false;
bool alarmModeJustChanged = false;
unsigned long alarmHoldTimer = 0;

// Handles the 3-second hold to enter/exit settings
void checkModeToggle(unsigned long currentMillis) {
 if (!inViewAlarmsMode && alarmStopState == LOW && upState == HIGH && downState == HIGH) {
    if (!holdingAlarmStop) {
      holdingAlarmStop = true;
      alarmHoldTimer = currentMillis;
      alarmModeJustChanged = false;
    } else if (!alarmModeJustChanged && (currentMillis - alarmHoldTimer >= 3000)) {
         if (!inAlarmSettingsMode) {
        addNewAlarm(); // finds a free slot, sets enabled=true, sets editingAlarmIndex, sets inAlarmSettingsMode=true
      } else {
        inAlarmSettingsMode = false; // exiting
      }
      alarmModeJustChanged = true;
      lastActivityTimer = currentMillis; // Reset the idle timer!
      blinkOn = true;
      previousBlinkMillis = currentMillis;
      updateDisplayBuffer();
    }
  } else {
    holdingAlarmStop = false;
  }


  if (!inViewAlarmsMode && upState == LOW && downState == LOW && alarmStopState == HIGH) {
    if (!holdingBoth) {
      holdingBoth = true;
      bothHoldTimer = currentMillis;
      modeJustChanged = false;
    } else if (!modeJustChanged && (currentMillis - bothHoldTimer >= 3000)) {
      // 3 seconds reached! Toggle mode.

      if (inSettingsMode) {
          saveTimeToRTC();
          inSettingsMode = false;
        } else {
          inSettingsMode = true;
      }

      modeJustChanged = true;
      lastActivityTimer = currentMillis; // Reset the idle timer!
      blinkOn = true;
      previousBlinkMillis = currentMillis;
      updateDisplayBuffer();
    }
  } else {
    holdingBoth = false;
  }

  if (upState == LOW && downState == LOW && alarmStopState == LOW) {
    if (!holdingTriple) {
      holdingTriple = true;
      tripleHoldTimer = currentMillis;
      tripleModeJustChanged = false;
    } else if (!tripleModeJustChanged && (currentMillis - tripleHoldTimer >= 5000)) {
      inViewAlarmsMode = !inViewAlarmsMode;
      if (inViewAlarmsMode) {
        selectedAlarmIndex = 0; // start at the top of the list
      }
      tripleModeJustChanged = true;
      lastActivityTimer = currentMillis;
      updateDisplayBuffer();
    }
  } else {
    holdingTriple = false;
  }
}

// Kicks you out of settings if you do nothing for 10 seconds
void checkSettingsTimeout(unsigned long currentMillis) {
  if (inSettingsMode && !holdingBoth) {
    if (currentMillis - lastActivityTimer >= 10000) {
      saveTimeToRTC();
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