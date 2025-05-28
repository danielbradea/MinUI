#include "ButtonManager.h" 

// Constructor: Initializes the button map and sets a default last event with no action
ButtonManager::ButtonManager(const std::map<String, uint8_t>& buttons) 
  : buttons(buttons), lastEvent({"", 0, NO_ACTION}) {}

// Sets up the button pins as INPUT_PULLUP and initializes internal state tracking
void ButtonManager::begin() {
  for (const auto& pair : buttons) {
    pinMode(pair.second, INPUT_PULLUP); // Use internal pull-up resistor
    states[pair.second] = ButtonState(); // Initialize state for each button
  }
}

// Main update loop that should be called frequently to detect button events
void ButtonManager::update() {
  unsigned long now = millis(); // Get current time in milliseconds
  lastEvent = {"", 0, NO_ACTION}; // Reset the last event

  // Loop through all registered buttons
  for (const auto& pair : buttons) {
    const String& name = pair.first;
    uint8_t pin = pair.second;
    ButtonState& s = states[pin];
    bool pressed = (digitalRead(pin) == LOW); // Active LOW logic

    if (pressed) {
      // If the button is pressed and we weren’t waiting for release, register the press
      if (!s.waitingForRelease) {
        s.lastPressTime = now;
        s.waitingForRelease = true;
        s.longPressReported = false;
      }

      // If button held long enough and long press not yet reported, trigger long press
      if (!s.longPressReported && (now - s.lastPressTime > 800)) {
        s.longPressReported = true;
        s.clickCount = 0;
        s.clickPending = false;
        lastEvent = {name, pin, LONG_PRESS};
      }

    } else {
      // If button was released
      if (s.waitingForRelease) {
        s.waitingForRelease = false;

        // Short click candidate (quick release, not a long press)
        if ((now - s.lastPressTime) < 300 && !s.longPressReported) {
          s.clickCount++;

          if (s.clickCount == 1) {
            // First click - start waiting for possible double click
            s.lastClickTime = now;
            s.clickPending = true;
          } else if (s.clickCount == 2 && (now - s.lastClickTime) < 400) {
            // Double click detected
            s.clickPending = false;
            s.clickCount = 0;
            lastEvent = {name, pin, DOUBLE_CLICK};
          }
        }
      }

      // If time passed since first click and no second click came, trigger short click
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

// Returns the last button event and resets the event to NO_ACTION
ButtonEvent ButtonManager::getAction() {
  ButtonEvent event = lastEvent;
  lastEvent = {"", 0, NO_ACTION}; // Clear event after reading
  return event;
}
