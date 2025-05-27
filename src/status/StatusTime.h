#ifndef STATUS_TIME_H
#define STATUS_TIME_H

#include "StatusBarElement.h"
#include "../display/DisplayInterface.h"
#include <Arduino.h>

class StatusTime : public StatusBarElement {
private:
  int hours;
  int minutes;
  unsigned long lastUpdateMillis = 0;

public:
  StatusTime()  = default;

  void draw(DisplayInterface& display, int xx = 0, int yy = 0) override {
  int drawX = x + xx;
  int drawY = y + yy;

  // Calculate if colon should be visible this frame (blinking every second)
  bool showColon = (millis() / 500) % 2 == 0;

  display.setTextColor(color);
  display.setTextSize(1);
  display.setCursor(drawX, drawY);
  if (showColon) {
    display.printf("%02d:%02d", hours, minutes);
  } else {
    display.printf("%02d %02d", hours, minutes); // Space instead of colon
  }
}

  int getWidth() override {
    return 40; 
  }

  int getHeight() override {
    return 9; 
  }

  void setTime(int h, int m) {
    hours = h % 24;
    minutes = m % 60;
  }

  /// Actualizează ora internă la fiecare minut
  void update() {
    unsigned long now = millis();
    if (now - lastUpdateMillis >= 60000 || lastUpdateMillis == 0) {
      lastUpdateMillis = now;
      minutes++;
      if (minutes >= 60) {
        minutes = 0;
        hours = (hours + 1) % 24;
      }
    }
  }
};

#endif
