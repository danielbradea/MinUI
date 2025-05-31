#include "MenuListView.h"

// Draws the menu and scroll indicator
void MenuListView::draw()
{
  drawMenu();
  drawScrollIndicator();
}

// Draws the visible portion of the menu
void MenuListView::drawMenu()
{
  const int visibleElements = menuListViewHeight / lineHeight;
  const unsigned long pauseDuration = 1500; // ms pause at text edges
  const int scrollBarWidth = charWidth;
  const int prefixWidth = selectedPrefix.length() * charWidth;
  const int paddingRight = 1;

  const int maxCharsThatFit = (menuListViewWidth - paddingRight) / charWidth;
  const int availableWidth = maxCharsThatFit * charWidth;

  if (visibleElements <= 0)
    return;

  int x = offsetX;
  int y = offsetY;

  display.setTextWrap(false);
  display.setTextColor(1);

  for (int i = 0; i < visibleElements; i++)
  {
    int idx = i + scrollOffset;
    if (idx >= currentMenu.size())
      break;

    const std::string &label = currentMenu[idx]->getLabel();
    int textX = x;
    int textY = y + i * lineHeight;

    bool isSelected = (idx == selectedIndex);

    if (isSelected)
    {
      // Draw selection prefix (e.g. "> ")
      display.setCursor(textX, textY);
      display.print(selectedPrefix.c_str());
      textX += prefixWidth;

      int labelPixelWidth = label.length() * charWidth;
      int textAvailableWidth = menuListViewWidth - prefixWidth - scrollBarWidth - paddingRight;

      // Reset scroll if a new item is selected
      if (idx != lastSelectedIndex)
      {
        labelScrollOffset = 0;
        scrollingRight = false;
        isPausing = false;
        lastScrollUpdate = millis();
        lastSelectedIndex = idx;
      }

      if (labelPixelWidth > textAvailableWidth)
      {
        unsigned long now = millis();

        // Handle scrolling pause at the start/end
        if (isPausing)
        {
          if (now - lastScrollUpdate >= pauseDuration)
          {
            isPausing = false;
            lastScrollUpdate = now;
          }
        }
        else if (now - lastScrollUpdate >= scrollSpeed)
        {
          // Scroll label left or right
          if (scrollingRight)
          {
            labelScrollOffset = std::max(0, labelScrollOffset - scrollStep);
            if (labelScrollOffset == 0)
            {
              isPausing = true;
              scrollingRight = false;
            }
          }
          else
          {
            labelScrollOffset = std::min(labelPixelWidth - textAvailableWidth, labelScrollOffset + scrollStep);
            if (labelScrollOffset >= labelPixelWidth - textAvailableWidth)
            {
              isPausing = true;
              scrollingRight = true;
            }
          }
          lastScrollUpdate = now;
        }

        // Calculate and render visible portion of the label
        int startChar = labelScrollOffset / charWidth;
        int remainingWidth = textAvailableWidth;
        std::string visibleText;

        for (int j = startChar; j < label.length() && remainingWidth > 0; j++)
        {
          visibleText += label[j];
          remainingWidth -= charWidth;
        }

        display.setCursor(textX, textY);
        display.print(visibleText.c_str());
      }
      else
      {
        // Label fits fully
        display.setCursor(textX, textY);
        display.print(label.c_str());
      }
    }
    else
    {
      // Non-selected item
      display.setCursor(textX, textY);
      if (label.length() > maxCharsThatFit)
      {
        std::string clipped = label.substr(0, maxCharsThatFit - 2) + "..";
        display.print(clipped.c_str());
      }
      else
      {
        display.print(label.c_str());
      }
    }
  }
}

// Renders the scroll indicator on the right side of the display
void MenuListView::drawScrollIndicator() const
{
  int barX = menuListViewWidth - 2;
  int totalItems = currentMenu.size();
  int barHeight = menuListViewHeight - offsetY + charWidth;

  // Draw dotted vertical scrollbar
  for (int y = 0; y < barHeight; y++)
  {
    int pixelY = y + offsetY;
    if (y % 2 == 0)
    {
      display.drawPixel(barX, pixelY, 1);
    }
  }

  // Draw scroll position marker
  float percent = selectedIndex / (float)(totalItems - 1);
  int centerY = offsetY + static_cast<int>(percent * (barHeight - 1));

  for (int dy = -1; dy <= 1; dy++)
  {
    display.drawPixel(barX - 1, centerY + dy, 1);
    display.drawPixel(barX, centerY + dy, 1);
    display.drawPixel(barX + 1, centerY + dy, 1);
  }
}

// Sets the current menu and clears submenu history
void MenuListView::setMenu(const std::vector<std::shared_ptr<MenuItem>> &menu)
{
  currentMenu = menu;
  selectedIndex = scrollOffset = 0;
  while (!menuHistory.empty())
    menuHistory.pop();
}

// Moves selection up by one item
void MenuListView::moveSelectionUp()
{
  if (selectedIndex > 0)
  {
    selectedIndex--;
    if (selectedIndex < scrollOffset)
      scrollOffset--;
  }
}

// Moves selection down by one item
void MenuListView::moveSelectionDown()
{
  int visibleElements = menuListViewHeight / lineHeight;
  if (selectedIndex < currentMenu.size() - 1)
  {
    selectedIndex++;
    if (selectedIndex >= scrollOffset + visibleElements)
      scrollOffset++;
  }
}

// Enters submenu
void MenuListView::enterSubmenu()
{
  if (selectedIndex >= 0 && selectedIndex < currentMenu.size())
  {
    auto selected = currentMenu[selectedIndex];
    if (selected && selected->hasSubmenu())
    {
      menuHistory.push(currentMenu);
      currentMenu = selected->getSubmenu();
      selectedIndex = scrollOffset = 0;
    }
  }
}

void MenuListView::activateSelectedItem()
{
  if (selectedIndex >= 0 && selectedIndex < currentMenu.size())
  {
    auto selected = currentMenu[selectedIndex];
    if (selected && !selected->hasSubmenu())
    {
      selected->activate(); // Execute associated action
    }
  }
}

// Navigates back to the previous menu
void MenuListView::navigateBack()
{
  if (!menuHistory.empty())
  {
    currentMenu = menuHistory.top();
    menuHistory.pop();
    selectedIndex = scrollOffset = 0;
  }
}

// Returns true if there is a previous menu to return to
bool MenuListView::canGoBack() const
{
  return !menuHistory.empty();
}

void MenuListView::handleInput(ButtonEvent buttonEvent)
{

  if (buttonEvent.buttonName == "UP" && buttonEvent.action == ButtonAction::SHORT_CLICK)
  {
    moveSelectionUp();
  }
  else if (buttonEvent.buttonName == "DOWN" && buttonEvent.action == ButtonAction::SHORT_CLICK)
  {
    moveSelectionDown();
  }
  else if (buttonEvent.buttonName == "RIGHT" && buttonEvent.action == ButtonAction::SHORT_CLICK)
  {
    enterSubmenu();
  }
  else if (buttonEvent.buttonName == "LEFT" && buttonEvent.action == ButtonAction::SHORT_CLICK)
  {
    navigateBack();
  }
  else if (buttonEvent.buttonName == "CENTER" && buttonEvent.action == ButtonAction::SHORT_CLICK)
  {
    activateSelectedItem();
  }
}
