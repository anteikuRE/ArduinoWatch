#include "Globals.h"

int hours = 0;
int minutes = 0;
int seconds = 0;
unsigned long previousClockMillis = 0;

const int btnUp = A0;
const int btnDown = A1;
int upState = HIGH;
int downState = HIGH;
int lastBtnUpState = HIGH;
int lastBtnDownState = HIGH;

bool inSettingsMode = false;
bool holdingBoth = false;
bool modeJustChanged = false;
unsigned long bothHoldTimer = 0;
unsigned long lastActivityTimer = 0;

bool blinkOn = true;
unsigned long previousBlinkMillis = 0;
const unsigned long blinkInterval = 400; // ms
