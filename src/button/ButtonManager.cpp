#include "ButtonManager.h"

ButtonManager::ButtonManager(const std::map<String, uint8_t>& buttons) 
  : buttons(buttons), lastEvent({"", 0, NO_ACTION}) {}

void ButtonManager::begin() {
  for (const auto& pair : buttons) {
    pinMode(pair.second, INPUT_PULLUP);
    states[pair.second] = ButtonState();
  }
}

void ButtonManager::update() {
  unsigned long now = millis();
  lastEvent = {"", 0, NO_ACTION};

  for (const auto& pair : buttons) {
    const String& name = pair.first;
    uint8_t pin = pair.second;
    ButtonState& s = states[pin];
    bool pressed = (digitalRead(pin) == LOW);

    if (pressed) {
      if (!s.waitingForRelease) {
        s.lastPressTime = now;
        s.waitingForRelease = true;
        s.longPressReported = false;
      }

      if (!s.longPressReported && (now - s.lastPressTime > 800)) {
        s.longPressReported = true;
        s.clickCount = 0;
        s.clickPending = false;
        lastEvent = {name, pin, LONG_PRESS};
      }
    } else {
      if (s.waitingForRelease) {
        s.waitingForRelease = false;

        if ((now - s.lastPressTime) < 300 && !s.longPressReported) {
          s.clickCount++;

          if (s.clickCount == 1) {
            s.lastClickTime = now;
            s.clickPending = true;
          } else if (s.clickCount == 2 && (now - s.lastClickTime) < 400) {
            s.clickPending = false;
            s.clickCount = 0;
            lastEvent = {name, pin, DOUBLE_CLICK};
          }
        }
      }

      if (s.clickPending && (now - s.lastClickTime > 400)) {
        s.clickPending = false;
        if (s.clickCount == 1) {
          lastEvent = {name, pin, SHORT_CLICK};
        }
        s.clickCount = 0;
      }
    }
  }
}

ButtonEvent ButtonManager::getAction() {
  ButtonEvent event = lastEvent;
  lastEvent = {"", 0, NO_ACTION};
  return event;
}