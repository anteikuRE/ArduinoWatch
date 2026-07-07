#include "Buzzer.h"
#include "Globals.h"


// ---------- Alarm settings ----------
int alarmHour = 7;
int alarmMinute = 30;
bool alarmEnabled = true;

bool alarmActive = false;       // true while the alarm is currently buzzing
bool alarmTriggeredToday = false; // prevents re-triggering every second during that minute

// Buzzer pattern timing (non-blocking beep-beep-beep instead of constant tone)
unsigned long previousBuzzMillis = 0;
const unsigned long buzzOnTime = 500;  // ms buzzer stays ON
const unsigned long buzzOffTime = 500; // ms buzzer stays OFF
bool buzzerState = false;


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
  if (!alarmEnabled) return;

  // Trigger the alarm once, at the exact minute
  if (hours == alarmHour && minutes == alarmMinute) {
    if (!alarmTriggeredToday) {
      alarmActive = true;
      alarmTriggeredToday = true;
    }
  } else {
    // Reset the "already triggered" flag once we've moved past that minute
    alarmTriggeredToday = false;
  }

  if (alarmActive) {
    handleBuzzer(currentMillis);
  }
}
