#include "Globals.h"

ThreeWire myWire(3, 2, 4); // DAT, CLK, RST pins
RtcDS1302<ThreeWire> Rtc(myWire);

int hours = 0;
int minutes = 0;
int seconds = 0;
unsigned long previousClockMillis = 0;

const int btnUp = A0;
const int btnDown = A1;
const int btnAlarmStop = A2;
int upState = HIGH;
int downState = HIGH;
int alarmStopState = HIGH;
int lastBtnUpState = HIGH;
int lastBtnDownState = HIGH;
int lastBtnAlarmStopState = HIGH;
const int buzzerPin = 13;

bool inSettingsMode = false;
bool inAlarmSettingsMode = false;
bool holdingBoth = false;
bool modeJustChanged = false;
unsigned long bothHoldTimer = 0;
unsigned long lastActivityTimer = 0;
unsigned long lastFieldSwitchMillis = 0;

bool blinkOn = true;
unsigned long previousBlinkMillis = 0;
const unsigned long blinkInterval = 400; // ms

// Handles button presses to change the time
EditField currentField = EDIT_HOURS;


Alarm alarms[MAX_ALARMS] = {
  {7, 0, 0, true, false},
  {7, 0, 0, false, false},
  {7, 20, 0, true, false},
  {7, 0, 0, false, false},
  {7, 0, 0, false, false}
};

int editingAlarmIndex = 0;

// ---------- View Alarms mode ----------
bool inViewAlarmsMode = false;
bool holdingTriple = false;
unsigned long tripleHoldTimer = 0;
bool tripleModeJustChanged = false;
int selectedAlarmIndex = 0;