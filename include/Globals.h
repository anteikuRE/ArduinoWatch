#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

// ---------- Time tracking ----------
extern int hours;
extern int minutes;
extern int seconds;
extern unsigned long previousClockMillis;

// ---------- Button pins & states ----------
extern const int btnUp;
extern const int btnDown;
extern int upState;
extern int downState;
extern int lastBtnUpState;
extern int lastBtnDownState;

// ---------- System modes & timers ----------
extern bool inSettingsMode;
extern bool holdingBoth;
extern bool modeJustChanged;
extern unsigned long bothHoldTimer;
extern unsigned long lastActivityTimer;
extern unsigned long lastFieldSwitchMillis;

// ---------- Blink control for settings mode ----------
extern bool blinkOn;
extern unsigned long previousBlinkMillis;
extern const unsigned long blinkInterval;


// ---------- Handles button presses to change the time ----------
enum EditField { EDIT_HOURS, EDIT_MINUTES };
extern EditField currentField;
#endif
