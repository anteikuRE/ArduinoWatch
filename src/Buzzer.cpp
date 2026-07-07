#include "Buzzer.h"
#include "Globals.h"

// A short ascending chime - swap these out for whatever you like
static int melodyNotes[] = {523, 659, 784, 1047}; // C5, E5, G5, C6
static int melodyDurations[] = {120, 120, 120, 200}; // ms per note
static const int melodyLength = 4;

static int currentNote = -1; // -1 = not playing
static unsigned long noteStartTime = 0;
static bool melodyPlaying = false;

void setupBuzzer() {
  pinMode(buzzerPin, OUTPUT);
}

// Simple beep while a button is held
// void handleBuzzer() {
//   if (upState == LOW) {
//     digitalWrite(buzzerPin, HIGH); // button pressed - buzz
//   } else if (downState == LOW) {
//     digitalWrite(buzzerPin, HIGH); // button pressed - buzz
//   } 
//   else {
//     digitalWrite(buzzerPin, LOW);  // no button pressed - silent
//   }
// }

// // Call this anywhere (e.g. when entering/exiting settings mode) to play the chime
// void startMelody() {
//   melodyPlaying = true;
//   currentNote = 0;
//   noteStartTime = millis();
//   tone(buzzerPin, melodyNotes[currentNote]);
// }

// // Must be called every loop() pass to advance the melody without blocking
// void updateMelody() {
//   if (!melodyPlaying) return;

//   if (millis() - noteStartTime >= melodyDurations[currentNote]) {
//     currentNote++;
//     if (currentNote >= melodyLength) {
//       noTone(buzzerPin);
//       melodyPlaying = false;
//       return;
//     }
//     noteStartTime = millis();
//     tone(buzzerPin, melodyNotes[currentNote]);
//   }
// }
