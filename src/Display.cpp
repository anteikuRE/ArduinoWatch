#include "Display.h"
#include "Globals.h"

// ==========================================
// Display wiring (confirmed working - common CATHODE)
// ==========================================
int pinA = 11, pinB = 7, pinC = 4, pinD = 2;
int pinE = 1,  pinF = 10, pinG = 5;
int pinDP = 3;
int D1 = 12, D2 = 8, D3 = 9, D4 = 6;

int segPins[8]   = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
int digitPins[4] = {D1, D2, D3, D4};

// Segment patterns for 0-9 (order: A,B,C,D,E,F,G,DP) - true = segment ON
const bool digitPatterns[10][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}  // 9
};

// What's shown on each of the 4 digits right now (-1 = blank)
int displayBuffer[4] = {-1, -1, -1, -1};
bool colonOn = false; // colon dot, shown via DP of digit index 1 - adjust if wired differently

// Multiplexing state
static int currentDigit = 0;
static unsigned long previousMuxMicros = 0;
static const unsigned long muxInterval = 3000; // microseconds per digit (~83Hz full refresh)

void setupDisplay() {
  for (int i = 0; i < 8; i++) pinMode(segPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digitPins[i], OUTPUT);
}

static void allDigitsOff() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPins[i], HIGH); // common cathode: HIGH = off
  }
}

// Lights ONE digit position with ONE character for a brief moment.
// Called every loop() pass - this IS what makes the display look "on".
void refreshDisplayMux() {
  unsigned long now = micros();
  if (now - previousMuxMicros < muxInterval) return;
  previousMuxMicros = now;

  allDigitsOff();

  int value = displayBuffer[currentDigit];

  if (value >= 0 && value <= 9) {
    for (int s = 0; s < 7; s++) {
      bool on = digitPatterns[value][s];
      digitalWrite(segPins[s], on ? HIGH : LOW); // common cathode: HIGH = segment on
    }
    bool dpOn = (currentDigit == 1) && colonOn; // colon dot on digit 2's DP - adjust if needed
    digitalWrite(segPins[7], dpOn ? HIGH : LOW);

    digitalWrite(digitPins[currentDigit], LOW); // common cathode: LOW = digit on
  }
  // if value is -1 (blank), all digits stay off for this slot

  currentDigit = (currentDigit + 1) % 4;
}

// Loads the digits to show into displayBuffer based on current time/mode
void updateDisplayBuffer() {
  if (inSettingsMode && !blinkOn) {
    // Blank display briefly - this is our "settings mode" indicator
    displayBuffer[0] = displayBuffer[1] = displayBuffer[2] = displayBuffer[3] = -1;
    return;
  }

  displayBuffer[0] = hours / 10;
  displayBuffer[1] = hours % 10;
  displayBuffer[2] = minutes / 10;
  displayBuffer[3] = minutes % 10;

  colonOn = inSettingsMode ? blinkOn : (seconds % 2 == 0);
}
