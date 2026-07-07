#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Globals.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(1, 28);
  display.println("Welcome To Watch!");
  display.display();
  delay(1000);  // Show greeting for 1 second
}

void updateDisplayBuffer() {
  char timeStr[6]; // "HH:MM" + null terminator
  char sep = (inSettingsMode || seconds % 2 == 0) ? ':' : ' ';
  sprintf(timeStr, "%02d%c%02d", hours, sep, minutes);

  display.clearDisplay();
  display.setTextSize(4);
  display.setCursor(6, 28);
  display.println(timeStr);

  if (inSettingsMode && !blinkOn) {
    if (currentField == EDIT_HOURS) {
      display.fillRect(6, 28, 44, 32, SSD1306_BLACK);
    } else { // EDIT_MINUTES
      display.fillRect(75, 28, 50, 32, SSD1306_BLACK);
    }
  }

  display.display();
}