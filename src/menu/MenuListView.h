#ifndef MENU_LIST_VIEW_H
#define MENU_LIST_VIEW_H

#include "../display/DisplayInterface.h" // Display interface abstraction (e.g., for OLED)
#include "../button/ButtonManager.h"
#include <Arduino.h>  // Arduino utility functions like millis()
#include "MenuItem.h" // Menu item structure/class
#include <vector>     // Used to hold lists of menu items
#include <memory>     // For using shared_ptr with menu items
#include <stack>      // For tracking menu navigation history

// Class responsible for rendering and navigating a list of menu items
class MenuListView
{
private:
  DisplayInterface &display; // Reference to the display used for rendering

  std::vector<std::shared_ptr<MenuItem>> currentMenu;             // Currently visible menu
  std::stack<std::vector<std::shared_ptr<MenuItem>>> menuHistory; // Stack of previous menus (used for navigation)

  // Display configuration
  int charWidth = 6;           // Width of each character in pixels
  int lineHeight = 10;         // Height of each menu line
  int menuListViewWidth = 128; // Width of the display area for the menu
  int menuListViewHeight = 64; // Height of the display area for the menu

  // Screen positioning
  int offsetX = 0; // X offset for rendering the menu
  int offsetY = 0; // Y offset for rendering the menu

  // Scrolling configuration
  int scrollOffset = 0; // Vertical scroll offset (index of first visible item)
  int scrollSpeed = 80; // Time between each scroll step in ms
  int scrollStep = 1;   // Number of pixels to scroll each step

  std::string selectedPrefix = "> "; // Prefix shown before selected item

  // State variables
  int selectedIndex = 0;              // Index of the currently selected item
  unsigned long lastScrollUpdate = 0; // Last time label scroll was updated
  int labelScrollOffset = 0;          // Offset used for scrolling long labels
  bool scrollingRight = false;        // Direction of label scrolling
  int lastSelectedIndex = -1;         // Tracks the previously selected item index
  bool isPausing = false;             // Used to delay scroll when selection changes

public:
  // Constructor: requires reference to a display
  MenuListView(DisplayInterface &disp)
      : display(disp) {}

  // Sets the current menu and resets history
  void setMenu(const std::vector<std::shared_ptr<MenuItem>> &menu);

  void moveSelectionUp();      // Moves the selection up by one item in the current menu
  void moveSelectionDown();    // Moves the selection down by one item in the current menu
  void enterSubmenu();         // Enters the submenu of the currently selected item, if it exists
  void activateSelectedItem(); // Activates the currently selected item, if it has an associated action
  void navigateBack();         // Navigates back to the previous menu, if there is one
  bool canGoBack() const;      // Returns true if history is not empty

  void handleInput(ButtonEvent buttonEvent);

  // Getters and setters for layout and behavior
  void setOffsetX(int xx)
  {
    offsetX = xx;
  }
  int getOffsetX() const
  {
    return offsetX;
  }

  void setOffsetY(int yy)
  {
    offsetY = yy;
  }
  int getOffsetY() const
  {
    return offsetY;
  }

  void setMenuListViewHeight(int height)
  {
    menuListViewHeight = height;
  }
  int getMenuListViewHeight() const
  {
    return menuListViewHeight;
  }

  void setMenuListViewWidth(int width)
  {
    menuListViewWidth = width;
  }
  int getMenuListViewWidth() const
  {
    return menuListViewWidth;
  }

  void setScrollSpeed(int speed)
  {
    scrollSpeed = speed;
  }
  int getScrollSpeed() const
  {
    return scrollSpeed;
  }

  void setScrollStep(int step)
  {
    scrollStep = step;
  }
  int getScrollStep() const
  {
    return scrollStep;
  }

  void setSelectedPrefix(const std::string &prefix)
  {
    selectedPrefix = prefix;
  }
  std::string getSelectedPrefix() const
  {
    return selectedPrefix;
  }

  // Main draw method that updates the display
  void draw();

private:
  // Helper methods for rendering
  void drawMenu();                  // Draws visible menu items
  void drawScrollIndicator() const; // Draws scroll bar indicator
};

#endif // MENU_LIST_VIEW_H
