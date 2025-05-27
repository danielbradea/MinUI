#ifndef STATUS_BATTERY_H
#define STATUS_BATTERY_H

#include "StatusBarElement.h"
#include "../display/DisplayInterface.h"
#include <Arduino.h>

// StatusBattery represents a visual battery indicator using pixel art.
// Inherits from StatusBarElement to integrate into a status bar system.
class StatusBattery : public StatusBarElement {
private:
  // Static 2D arrays that define the pixel representation of various battery states
  static const uint8_t BATTERY_FULL[9][14];     // Icon for 100% battery
  static const uint8_t BATTERY_65[9][14];       // Icon for ~65% battery
  static const uint8_t BATTERY_30[9][14];       // Icon for ~30% battery
  static const uint8_t BATTERY_EMPTY[9][14];    // Icon for 0% battery
  static const uint8_t BATTERY_CHARGING[9][14]; // Icon for charging state

  // Battery status and display configuration
  int level = 100;            // Battery level percentage (0 to 100)
  bool isCharging = false;    // Indicates if the battery is charging
  bool percent = true;        // Whether to show the numeric battery percentage

public:
  // Override from StatusBarElement: draws the battery icon on the display
  void draw(DisplayInterface& display, int xx = 0, int yy = 0) override;

  // Override from StatusBarElement: returns the width of the battery display
  int getWidth() override;
  int getHeight() override;

  // Setters for battery state
  void setLevel(int level);               // Set the battery level (0-100)
  void setIsCharging(bool charging);      // Set the charging state
  void setShowPercent(bool showPercent);  // Enable/disable percentage display
  bool getShowPercent() { return percent; } // Get whether percentage is shown
};

#endif
