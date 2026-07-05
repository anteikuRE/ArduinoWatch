#include "Clock.h"
#include "Globals.h"
#include "Display.h"

void tickClock(unsigned long currentMillis) {
  if (currentMillis - previousClockMillis >= 1000) {
    previousClockMillis = currentMillis;
    seconds++;

    if (seconds >= 60) {
      seconds = 0;
      minutes++;
      if (minutes >= 60) {
        minutes = 0;
        hours++;
        if (hours >= 24) hours = 0;
      }
    }

    if (!inSettingsMode) {
      updateDisplayBuffer(); // colon blinks with the seconds tick
    }
  }
}
