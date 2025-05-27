#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>
#include <map>

enum ButtonAction {
  NO_ACTION,
  SHORT_CLICK,
  LONG_PRESS,
  DOUBLE_CLICK
};

struct ButtonEvent {
  String buttonName;    // Numele butonului
  uint8_t buttonPin;    // Pinul GPIO
  ButtonAction action;  // Tipul acțiunii
};

class ButtonManager {
public:
  ButtonManager(const std::map<String, uint8_t>& buttons);
  
  void begin();
  void update();
  ButtonEvent getAction();

private:
  struct ButtonState {
    unsigned long lastPressTime = 0;
    unsigned long lastClickTime = 0;
    bool waitingForRelease = false;
    bool longPressReported = false;
    bool clickPending = false;
    uint8_t clickCount = 0;
  };

  std::map<String, uint8_t> buttons;  // Nume -> Pin
  std::map<uint8_t, ButtonState> states; // Pin -> Stare
  ButtonEvent lastEvent;
};

#endif