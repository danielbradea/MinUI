#include "StatusBar.h"

void StatusBar::draw() {
  if (downPosition) {
    int displayH = display.height();
    
    if (statusBarBgColor == 0) {
      display.drawFastHLine(0 + offsetX, displayH - offsetY - statusBarHeight, statusBarWidth - offsetX, 1);  // Transparent status bar
    } else {
      display.fillRect(0 + offsetX, displayH - offsetY - statusBarHeight, statusBarWidth - offsetX, statusBarHeight, statusBarBgColor);  // Filled top bar
    }
  } else {
    if (statusBarBgColor == 0) {
      display.drawFastHLine(0 + offsetX, statusBarHeight + offsetY, statusBarWidth - offsetX, 1);  // Transparent status bar
    } else {
      display.fillRect(0 + offsetX, 0 + offsetY, statusBarWidth - offsetX, statusBarHeight, statusBarBgColor);  // Filled top bar
    }
  }
  drawLeftElements();
  drawRightElements();
}

void StatusBar::drawLeftElements() const {

  int displayH = display.height();
  int x = 0;
  for (const auto& item : leftElements) {
    if (item) {
      item->setPosition(StatusBarElementPosition::LEFT);
      item->setColor(statusBarBgColor == 1 ? 0 : 1);
      int itemYoffset = item->getOffsetY();
      int itemXoffset = item->getOffsetX();
      if (downPosition) {
        int displayH = display.height();
        int itemHeight = item->getHeight();
        item->draw(display, x + offsetX + itemXoffset ,  displayH - offsetY - itemHeight - itemYoffset);
      } else {
        item->draw(display, x + offsetX + itemXoffset, 0 + offsetY + itemYoffset);
      }
      
      x += item->getWidth() + elementSpacing;
    }
  }
}


void StatusBar::drawRightElements() const {
  int x = statusBarWidth - offsetX;
  for (auto it = rightElements.rbegin(); it != rightElements.rend(); ++it) {
    if (*it) {
      const auto& item = *it;
      int itemWidth = item->getWidth();
      x -= itemWidth;
      item->setPosition(StatusBarElementPosition::RIGHT);
      item->setColor(statusBarBgColor == 1 ? 0 : 1);  // Invert text color for contrast
      int itemYoffset = item->getOffsetY();
      int itemXoffset = item->getOffsetX();
      if (downPosition) {
        int displayH = display.height();
        int itemHeight = item->getHeight();
        item->draw(display,x + offsetX - itemXoffset, displayH - offsetY - itemHeight - itemYoffset);
      } else {
        item->draw(display,x + offsetX - itemXoffset, 0 + offsetY + itemYoffset);
      }
      x -= elementSpacing;
    }
  }
}