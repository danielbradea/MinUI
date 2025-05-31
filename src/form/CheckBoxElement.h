#ifndef CHECK_BOX_ELEMENT_H
#define CHECK_BOX_ELEMENT_H

#include "../button/ButtonManager.h"
#include "FormElement.h"
#include <WString.h>

//
// A form input element that represents a checkbox with a label
// Toggling is done via button events (center button or up/down)
// When selected, the label scrolls left-right if it doesn't fit
//
class CheckBoxElement : public FormElement
{
private:
    String label;            // The label displayed next to the checkbox
    bool isChecked;          // Whether the checkbox is checked
    bool isEditing = false;  // Whether the element is currently being edited
    bool isSelected = false; // Whether the element is currently selected

    // UI layout constants
    const int BOX_SIZE = 10;
    const int BOX_MARGIN = 3;
    const int TEXT_PADDING = 3;
    const int ELEMENT_HEIGHT = 14;
    const int CURSOR_HEIGHT = 2;
    const int CURSOR_OFFSET = 1; // Space between box and cursor line
    const int CHAR_WIDTH = 6;    // Approximate width of one character

    unsigned long lastScrollTime = 0;
    int scrollOffset = 0;
    int scrollDirection = 1; // 1 for right, -1 for left

public:
    // Constructor with label and optional default state
    explicit CheckBoxElement(const String &label, bool defaultState = false)
        : label(label), isChecked(defaultState) {}

    // Draws the checkbox element on the screen
    void draw(DisplayInterface &display, int x, int y, int elementWidth) override
    {
        if (isSelected)
        {
            display.drawFastVLine(x, y, getHeight(), 1);
        }

        int boxX = x + 4;
        int boxY = y + (ELEMENT_HEIGHT - BOX_SIZE) / 2;
        display.drawRect(boxX, boxY, BOX_SIZE, BOX_SIZE, 1);

        if (isChecked)
        {
            display.drawLine(boxX + 1, boxY + BOX_SIZE / 2,
                             boxX + BOX_SIZE / 2, boxY + BOX_SIZE - 2, 1);
            display.drawLine(boxX + BOX_SIZE / 2, boxY + BOX_SIZE - 2,
                             boxX + BOX_SIZE - 1, boxY + 1, 1);
        }

        int textX = boxX + BOX_SIZE + BOX_MARGIN;
        int textY = y + 3;
        int availableWidth = elementWidth - (textX - x);

        display.setTextColor(1);
        display.setCursor(textX, textY);

        int maxVisibleChars = availableWidth / CHAR_WIDTH;

        String visibleText = label;

        if (isSelected && label.length() > maxVisibleChars)
        {
            unsigned long now = millis();
            if (now - lastScrollTime > 200)
            {
                scrollOffset += scrollDirection;
                if (scrollOffset < 0)
                {
                    scrollOffset = 0;
                    scrollDirection = 1;
                }
                else if (scrollOffset > label.length() - maxVisibleChars)
                {
                    scrollOffset = label.length() - maxVisibleChars;
                    scrollDirection = -1;
                }
                lastScrollTime = now;
            }

            visibleText = label.substring(scrollOffset, scrollOffset + maxVisibleChars);
        }
        else
        {
            scrollOffset = 0;
            scrollDirection = 1;
            if (label.length() > maxVisibleChars)
            {
                visibleText = label.substring(0, maxVisibleChars);
            }
        }

        display.print(visibleText.c_str());

        if (isEditing && (millis() % 1000 < 500))
        {
            int cursorY = boxY + BOX_SIZE + CURSOR_OFFSET;
            display.drawFastHLine(boxX, cursorY, BOX_SIZE, 1);
        }
    }

    // Handles user input based on button events
    bool handleInput(ButtonEvent buttonEvent) override
    {
        if (!isEditing)
            return false;

        if (buttonEvent.action == ButtonAction::SHORT_CLICK)
        {
            if (buttonEvent.buttonName == "UP")
            {
                isChecked = true;
                return true;
            }
            else if (buttonEvent.buttonName == "DOWN")
            {
                isChecked = false;
                return true;
            }
            else if (buttonEvent.buttonName == "CENTER")
            {
                isChecked = !isChecked;
                return true;
            }
        }
        else if (buttonEvent.action == ButtonAction::DOUBLE_CLICK &&
                 buttonEvent.buttonName == "CENTER")
        {
            setEditing(false);
            return true;
        }

        return false;
    }

    // Returns the height of the element (including space for cursor)
    int getHeight() override { return ELEMENT_HEIGHT; }

    // Indicates if this element supports editing
    bool canEdit() override { return true; }

    // Returns whether it is in editing mode
    bool getEditing() override { return isEditing; }

    // Sets editing mode
    void setEditing(bool editing) override
    {
        isEditing = editing;
    }

    // Sets whether this element is selected in the UI
    void setSelected(bool select) { isSelected = select; }

    // Returns the current checkbox state
    bool getValue() const { return isChecked; }

    // Returns the label of the checkbox
    const String &getLabel() const { return label; }
};

#endif
