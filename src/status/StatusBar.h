#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "../display/DisplayInterface.h"  // Graphics library for display
#include <vector>              // For dynamic arrays
#include <memory>              // For smart pointers
#include <stack>               // For menu history stack
#include "StatusBarElement.h"  // Status bar element interface
#include <Arduino.h>

class StatusBar {
private:
  DisplayInterface& display;
  std::vector<std::shared_ptr<StatusBarElement>> leftElements;   // Left-aligned status bar elements
  std::vector<std::shared_ptr<StatusBarElement>> rightElements;  // Right-aligned status bar elements
  int elementSpacing = 2;
  int statusBarBgColor = 1;
  int statusBarHeight = 13;
  int statusBarWidth = 128;
  int offsetX = 0;  // Offset pe orizontală (10 pixeli de la marginea stângă)
  int offsetY = 0;  // Offset pe verticală (0 dacă nu ai nevoie)
  bool downPosition = false;


  void drawLeftElements() const;   // Draw left status bar elements
  void drawRightElements() const;  // Draw right status bar elements

public:

  StatusBar(DisplayInterface& disp)
    : display(disp) {}

  // Set spacing between status bar elements
  void setElementSpacing(int spacing) {
    elementSpacing = spacing;
  }

  // Add element to left side of status bar
  void addLeftElement(std::shared_ptr<StatusBarElement> element) {
    leftElements.push_back(element);
  }

  // Add element to right side of status bar
  void addRightElement(std::shared_ptr<StatusBarElement> element) {
    rightElements.push_back(element);
  }

  // Set status bar background color
  void setStatusBarBgColor(int color) {
    statusBarBgColor = color;
  }

  // Get current status bar color
  int getStatusBarBgColor() {
    return statusBarBgColor;
  }

  // Set status bar height
  void setStatusBarHeight(int height) {
    statusBarHeight = height;
  }

  void setStatusBarWidth(int width) {
    statusBarWidth = width;
  }

  void setOffsetX(int xx) {
    offsetX = xx;
  }

  int getOffsetX() {
    return offsetX;
  }

  void setOffsetY(int yy) {
    offsetY = yy;
  }

  int getOffsetY() {
    return offsetY;
  }

  void setDownPosition(bool setDown){
    downPosition = setDown;
  }

  bool getDownPosition(){
    return downPosition;
  }

  void draw();
};
#endif