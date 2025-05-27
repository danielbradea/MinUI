#include <string>
#include "StatusBattery.h"


// Sprite definitions - 9x14 pixel arrays representing different battery states

// Full battery icon (100% charge)
const uint8_t StatusBattery::BATTERY_FULL[9][14] = {
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, 
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 }, 
  { 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 }, 
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }  
};

// 65% battery icon (similar structure but with fewer bars)
const uint8_t StatusBattery::BATTERY_65[9][14] = {
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1 }, 
  { 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1 }, 
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

// 30% battery icon (only one bar visible)
const uint8_t StatusBattery::BATTERY_30[9][14] = {
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

// Empty battery icon (no bars visible)
const uint8_t StatusBattery::BATTERY_EMPTY[9][14] = {
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

// Charging battery icon (with lightning Plug symbol)
const uint8_t StatusBattery::BATTERY_CHARGING[9][14] = {
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1 }, 
  { 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 }, 
  { 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1 }, 
  { 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 }, 
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};



// Draw method implementation
void StatusBattery::draw(DisplayInterface& display, int xx, int yy) {
  // Pointer to the selected sprite
  const uint8_t(*sprite)[14];
  
  // Calculate drawing position with offsets
  // Adjust X position based on whether icon is left or right aligned
  int drawX = x + xx ; 
  // Y position with vertical offset
  int drawY = y + yy ;
 

  // Select appropriate sprite based on battery state
  if (isCharging) {
    sprite = BATTERY_CHARGING;
  } else if (level > 65) {
    sprite = BATTERY_FULL;
  } else if (level > 30) {
    sprite = BATTERY_65;
  } else if (level > 5) {
    sprite = BATTERY_30;
  } else {
    sprite = BATTERY_EMPTY;
  }

  // Format percentage text with proper spacing
  char textBuf[6]; // Buffer for percentage text
  if (level == 100) {
    snprintf(textBuf, sizeof(textBuf), "100%%"); // Full percentage
  } else if (level < 10) {
    snprintf(textBuf, sizeof(textBuf), "  %d%%", level); // Single digit with extra space
  } else {
    snprintf(textBuf, sizeof(textBuf), " %d%%", level); // Double digit
  }
  
  // Calculate text dimensions
  const int charWidth = 6; // Width of each character
  const int batteryWidth = 14; // Width of battery icon
  const int textWidth = strlen(textBuf) * charWidth; // Total text width

  // Handle left-aligned position
  if (position == StatusBarElementPosition::LEFT) {
    // Draw battery icon
    for (int row = 0; row < 9; row++) {
      for (int col = 0; col < 14; col++) {
        if (sprite[row][col]) {
          display.drawPixel(drawX + col, drawY + row, color);
        }
      }
    }
    
    // Draw percentage text if enabled
    if (percent) {
      display.setTextColor(color);
      display.setCursor(drawX + batteryWidth, drawY + 1);
      display.print(textBuf);
    }
  } 
  else { 
    // Handle right-aligned position
    if (percent) {
      display.setTextColor(color);
      display.setCursor(drawX, drawY + 1);
      display.print(textBuf);
    }
    
    // Draw battery icon after text for right alignment
    int batteryStartX = drawX + (percent ? textWidth : 0);
    for (int row = 0; row < 9; row++) {
      for (int col = 0; col < 14; col++) {
        if (sprite[row][col]) {
          display.drawPixel(batteryStartX + col, drawY + row, color);
        }
      }
    }
  }
}

// Get total width of icon (including text if shown)
int StatusBattery::getWidth() {
  if (percent) {
    return 14 + (4 * 6); // Battery width (14) + max text width (4 chars * 6px)
  }
  return 14; // Just battery width
}

int StatusBattery::getHeight() {
  return 9; 
}

// Set battery level (0-100) with bounds checking
void StatusBattery::setLevel(int lvl) {
  level = constrain(lvl, 0, 100); // Constrain to valid range
}

// Set charging state
void StatusBattery::setIsCharging(bool charging) {
  isCharging = charging;
}

// Toggle percentage display
void StatusBattery::setShowPercent(bool showPercent) {
  percent = showPercent;
}


