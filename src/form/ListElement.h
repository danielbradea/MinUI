#ifndef LIST_ELEMENT_H
#define LIST_ELEMENT_H

#include "FormElement.h"
#include <WString.h>
#include <vector>

class ListElement : public FormElement
{
private:
    String label;
    std::vector<String> options;
    int selectedIndex = 0;
    bool isEditing = false;
    bool isSelected = false;
    unsigned long lastBlinkTime = 0;
    bool blinkState = false;
    const int BLINK_INTERVAL = 500; // ms

    // UI constants
    const int ELEMENT_HEIGHT = 22;
    const int CURSOR_HEIGHT = 2;
    const int TEXT_PADDING = 4;
    const int CHAR_WIDTH = 6; // Approximate character width

    // Scroll logic
    int scrollOffset = 0;
    int scrollDirection = 1; // 1 = right, -1 = left
    unsigned long lastScrollTime = 0;
    const int SCROLL_INTERVAL = 200; // ms

public:
    ListElement(const String &label, const std::vector<String> &options, int defaultIndex = 0)
        : label(label), options(options), selectedIndex(defaultIndex)
    {
        if (selectedIndex < 0 || selectedIndex >= (int)options.size())
            selectedIndex = 0;
    }

    void draw(DisplayInterface &display, int x, int y, int elementWidth) override
    {
        if (isSelected)
        {
            display.drawFastVLine(x, y, getHeight(), 1);
        }

        // Draw label
        display.setTextColor(1);
        display.setCursor(x + 4, y + 2);
        display.print((label+':').c_str());

        // Calculate positions
        int textY = y + ELEMENT_HEIGHT - 9;
        int textX = x + 4;
        int maxTextWidth = elementWidth - 2 * TEXT_PADDING - 2 * CHAR_WIDTH; // space for < >
        int maxVisibleChars = maxTextWidth / CHAR_WIDTH;

        // Get current value
        String value = options.empty() ? "" : options[selectedIndex];

        // Scroll logic (ping-pong)
        if ((isSelected || isEditing) && value.length() > maxVisibleChars)
        {
            unsigned long currentTime = millis();
            if (currentTime - lastScrollTime > SCROLL_INTERVAL)
            {
                scrollOffset += scrollDirection;

                if (scrollOffset > (int)(value.length() - maxVisibleChars))
                {
                    scrollOffset = (int)(value.length() - maxVisibleChars);
                    scrollDirection = -1;
                }
                else if (scrollOffset < 0)
                {
                    scrollOffset = 0;
                    scrollDirection = 1;
                }

                lastScrollTime = currentTime;
            }
        }
        else
        {
            scrollOffset = 0;
            scrollDirection = 1;
        }

        String visibleValue = value;
        if (value.length() > maxVisibleChars)
        {
            visibleValue = value.substring(scrollOffset, scrollOffset + maxVisibleChars);
        }

        // Decide left/right symbols
        String left, right;
        if (isEditing)
        {
            bool atStart = selectedIndex == 0;
            bool atEnd = selectedIndex == (int)options.size() - 1;

            if (atStart && atEnd)
            {
                left = "|";
                right = "|";
            }
            else if (atStart)
            {
                left = "|";
                right = ">";
            }
            else if (atEnd)
            {
                left = "<";
                right = "|";
            }
            else
            {
                left = "<";
                right = ">";
            }
        }
        else
        {
            left = "<";
            right = ">";
        }

        // Draw value
        display.setCursor(textX, textY);
        display.print(left.c_str());
        display.print(visibleValue.c_str());
        display.print(right.c_str());

        // Blinking underline cursor
        if (isEditing && !options.empty())
        {
            unsigned long currentTime = millis();
            if (currentTime - lastBlinkTime > BLINK_INTERVAL)
            {
                blinkState = !blinkState;
                lastBlinkTime = currentTime;
            }

            if (blinkState)
            {
                int underlineX = textX + CHAR_WIDTH;
                int underlineY = textY + 8;
                int underlineLength = visibleValue.length() * CHAR_WIDTH;
                display.drawFastHLine(underlineX, underlineY, underlineLength, 1);
            }
        }
    }

    bool handleInput(ButtonEvent buttonEvent) override
    {
        if (!isEditing)
            return false;

        if (buttonEvent.action == ButtonAction::SHORT_CLICK)
        {
            if (buttonEvent.buttonName == "LEFT")
            {
                if (selectedIndex > 0)
                {
                    selectedIndex--;
                    return true;
                }
            }
            else if (buttonEvent.buttonName == "RIGHT")
            {
                if (selectedIndex < (int)options.size() - 1)
                {
                    selectedIndex++;
                    return true;
                }
            }
        }
        else if (buttonEvent.action == ButtonAction::DOUBLE_CLICK)
        {
            if (buttonEvent.buttonName == "CENTER")
            {
                setEditing(false);
                return true;
            }
        }

        return false;
    }

    int getHeight() override
    {
        return ELEMENT_HEIGHT ;
    }

    bool canEdit() override { return true; }

    bool getEditing() override { return isEditing; }

    void setEditing(bool editing) override
    {
        isEditing = editing;
        blinkState = true;
        lastBlinkTime = millis();
        lastScrollTime = millis();
        scrollOffset = 0;
        scrollDirection = 1;
    }

    void setSelected(bool select) { isSelected = select; }

    int getSelectedIndex() const { return selectedIndex; }

    const String &getSelectedValue() const
    {
        static String empty;
        if (options.empty())
            return empty;
        return options[selectedIndex];
    }

    const String &getLabel() const { return label; }

    const std::vector<String> &getOptions() const { return options; }
};

#endif
