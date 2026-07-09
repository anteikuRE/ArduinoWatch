#include "Buzzer.h"
#include "Globals.h"


// ---------- Alarm settings ----------
bool alarmActive = false;       // true while the alarm is currently buzzing
bool alarmTriggeredToday = false; // prevents re-triggering every second during that minute

// Buzzer pattern timing (non-blocking beep-beep-beep instead of constant tone)
unsigned long previousBuzzMillis = 0;
const unsigned long buzzOnTime = 500;  // ms buzzer stays ON
const unsigned long buzzOffTime = 500; // ms buzzer stays OFF
bool buzzerState = false;

void addNewAlarm() {
  for (int i = 0; i < MAX_ALARMS; i++) {
    if (!alarms[i].enabled) {
      editingAlarmIndex = i;
      alarms[i].hour = 7;
      alarms[i].minute = 0;
      alarms[i].enabled = true;
      alarms[i].triggeredToday = false;

      inAlarmSettingsMode = true;
      currentField = EDIT_HOURS;
      lastActivityTimer = millis();
      return;
    }
  }
}

void stopAlarm() {
  alarmActive = false;
  buzzerState = false;
  digitalWrite(buzzerPin, LOW);
}

void handleBuzzer(unsigned long currentMillis) {
  unsigned long interval = buzzerState ? buzzOnTime : buzzOffTime;

  if(alarmStopState == LOW) {
    stopAlarm();
    return;
  }

  if (currentMillis - previousBuzzMillis >= interval) {
    previousBuzzMillis = currentMillis;
    buzzerState = !buzzerState;
    digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
  }
}

void checkAlarm(unsigned long currentMillis) {
  for (int i = 0; i < MAX_ALARMS; i++) {
    if (!alarms[i].enabled) continue;

    if (hours == alarms[i].hour && minutes == alarms[i].minute) {
      if (!alarms[i].triggeredToday) {
        alarmActive = true;
        alarms[i].triggeredToday = true;
      }
    } else {
      alarms[i].triggeredToday = false;
    }
  }

  if (alarmActive) {
    handleBuzzer(currentMillis);
  }
}
