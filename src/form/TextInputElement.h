#ifndef TEXT_INPUT_ELEMENT_H
#define TEXT_INPUT_ELEMENT_H

#include "../button/ButtonManager.h"
#include "FormElement.h"
#include <WString.h>
#include <vector>
#include <algorithm>

//
// A form input element that allows the user to edit text using buttons.
// It supports character sets: lowercase, uppercase, digits, and symbols.
// Cursor movement and editing is done via button events.
//
class TextInputElement : public FormElement
{
private:
    String label;               // The label displayed above the input box
    String value;               // The current value entered by the user
    int cursorPos = 0;          // Current position of the cursor
    int visibleStart = 0;       // Index of the first visible character
    bool isEditing = false;     // Whether the element is currently being edited
    bool isSelected = false;    // Whether the element is currently selected

    // Enumeration of supported character sets
    enum CharSet
    {
        LOWER,
        UPPER,
        DIGIT,
        SYMBOL
    };
    CharSet charSet = LOWER;    // Current active character set

    // UI layout constants
    const int BOX_HEIGHT = 14;
    const int TEXT_PADDING = 3;
    const int CHAR_WIDTH = 6;
    const int CURSOR_HEIGHT_OFFSET = 2;
    const int LABEL_HEIGHT = 10;

    // List of supported symbols
    const std::vector<char> SYMBOLS = {
        '.', ',', '!', '?', ':', ';', '-', '\'', '(', ')', '[', ']', '{', '}',
        '/', '\\', '^', '~', '*', '&', '%', '$', '#', '@'};

public:
    // Constructor with label and optional default value
    explicit TextInputElement(const String &label, const String &defaultValue = "")
        : label(label), value(defaultValue) {}

    // Draws the text input element on the screen
    void draw(DisplayInterface &display, int x, int y, int elementWidth) override
    {
        if (isSelected)
        {
            display.drawFastVLine(x, y, getHeight(), 1);
        }

        int boxY = y + LABEL_HEIGHT;
        int textX = x + 4 + TEXT_PADDING;
        int textY = boxY + TEXT_PADDING;
        int boxWidth = elementWidth - 4;

        int maxVisibleChars = (boxWidth - 2 * TEXT_PADDING) / CHAR_WIDTH;

        // Ensure the cursor is within the visible portion
        if (cursorPos < visibleStart)
            visibleStart = cursorPos;
        else if (cursorPos >= visibleStart + maxVisibleChars)
            visibleStart = cursorPos - maxVisibleChars + 1;

        // Draw label
        display.setTextColor(1);
        display.setCursor(x + 4, y + 1);
        display.print((label + ":").c_str());

        // Draw input box
        display.drawRoundRect(x + 4, boxY, boxWidth, BOX_HEIGHT, 3, 1);

        // Draw text value (visible part only)
        display.setCursor(textX, textY);
        String visibleValue = value.substring(visibleStart, visibleStart + maxVisibleChars);
        display.print(visibleValue.c_str());

        // Draw cursor if in editing mode and blinking
        if (isEditing && (millis() % 1000 < 500))
        {
            int cursorX = textX + (cursorPos - visibleStart) * CHAR_WIDTH;
            int cursorY1 = textY + 8;
            int cursorY2 = boxY + BOX_HEIGHT - CURSOR_HEIGHT_OFFSET;
            display.drawFastHLine(cursorX, cursorY1, 5, 1);
        }
    }

    // Handles user input based on button events
    bool handleInput(ButtonEvent buttonEvent) override
    {
        if (!isEditing)
            return false;

        // Handle single-click events
        if (buttonEvent.action == ButtonAction::SHORT_CLICK)
        {
            if (buttonEvent.buttonName == "LEFT")
            {
                cursorPos = constrain(cursorPos - 1, 0, value.length());
                updateCharSetByCursor();
                return true;
            }
            else if (buttonEvent.buttonName == "RIGHT")
            {
                cursorPos = constrain(cursorPos + 1, 0, value.length());
                updateCharSetByCursor();
                return true;
            }
            else if (buttonEvent.buttonName == "CENTER")
            {
                setEditing(false);
                return true;
            }
            else if (buttonEvent.buttonName == "UP")
            {
                cycleCharAtCursor();
                return true;
            }
            else if (buttonEvent.buttonName == "DOWN")
            {
                cycleCharAtCursorReverse();
                return true;
            }
        }
        // Handle double-click events for editing operations
        else if (buttonEvent.action == ButtonAction::DOUBLE_CLICK)
        {
            if (buttonEvent.buttonName == "UP")
            {
                charSet = static_cast<CharSet>((charSet + 1) % 4);
                setCharToStartOfCharSet();
                return true;
            }
            else if (buttonEvent.buttonName == "DOWN")
            {
                charSet = static_cast<CharSet>((charSet + 3) % 4);
                setCharToStartOfCharSet();
                return true;
            }
            else if (buttonEvent.buttonName == "LEFT")
            {
                if (cursorPos < value.length())
                {
                    value.remove(cursorPos, 1);
                }
                return true;
            }
            else if (buttonEvent.buttonName == "RIGHT")
            {
                value = value.substring(0, cursorPos) + ' ' + value.substring(cursorPos);
                cursorPos++;
                return true;
            }
            else if (buttonEvent.buttonName == "CENTER")
            {
                setEditing(false);
                return true;
            }
        }

        return false;
    }

    // Returns the height of the element
    int getHeight() override { return LABEL_HEIGHT + BOX_HEIGHT + 5; }

    // Indicates if this element supports editing
    bool canEdit() override { return true; }

    // Returns whether it is in editing mode
    bool getEditing() override { return isEditing; }

    // Sets editing mode
    void setEditing(bool editing) override
    {
        isEditing = editing;
        if (editing)
        {
            if (cursorPos >= value.length())
            {
                value += 'a'; // Default character when starting editing
            }
            charSet = LOWER;
        }
    }

    // Sets whether this element is selected in the UI
    void setSelected(bool select) { isSelected = select; }

    // Returns the current text value
    const String &getValue() const { return value; }

    // Returns the label of the input
    const String &getLabel() const { return label; }

private:
    // Ensures that the cursor does not go beyond the current value
    void ensureCursorInValue()
    {
        while (cursorPos >= value.length())
        {
            value += ' ';
        }
    }

    // Sets character at cursor to the starting character of the active charset
    void setCharToStartOfCharSet()
    {
        ensureCursorInValue();
        char c;
        switch (charSet)
        {
        case LOWER: c = 'a'; break;
        case UPPER: c = 'A'; break;
        case DIGIT: c = '0'; break;
        case SYMBOL: c = SYMBOLS.front(); break;
        }
        value.setCharAt(cursorPos, c);
    }

    // Cycles the character at cursor forward within the current charset
    void cycleCharAtCursor()
    {
        ensureCursorInValue();
        char c = value.charAt(cursorPos);

        switch (charSet)
        {
        case LOWER:
            c = (c < 'z') ? c + 1 : 'a';
            break;
        case UPPER:
            c = (c < 'Z') ? c + 1 : 'A';
            break;
        case DIGIT:
            c = (c < '9') ? c + 1 : '0';
            break;
        case SYMBOL:
        {
            auto it = std::find(SYMBOLS.begin(), SYMBOLS.end(), c);
            c = (it != SYMBOLS.end() && (it + 1) != SYMBOLS.end()) ? *(it + 1) : SYMBOLS.front();
            break;
        }
        }

        value.setCharAt(cursorPos, c);
    }

    // Cycles the character at cursor backward within the current charset
    void cycleCharAtCursorReverse()
    {
        ensureCursorInValue();
        char c = value.charAt(cursorPos);

        switch (charSet)
        {
        case LOWER:
            c = (c > 'a') ? c - 1 : 'z';
            break;
        case UPPER:
            c = (c > 'A') ? c - 1 : 'Z';
            break;
        case DIGIT:
            c = (c > '0') ? c - 1 : '9';
            break;
        case SYMBOL:
        {
            auto it = std::find(SYMBOLS.begin(), SYMBOLS.end(), c);
            c = (it != SYMBOLS.end() && it != SYMBOLS.begin()) ? *(it - 1) : SYMBOLS.back();
            break;
        }
        }

        value.setCharAt(cursorPos, c);
    }

    // Updates the current charset based on the character at the cursor
    void updateCharSetByCursor()
    {
        if (cursorPos < value.length())
        {
            char c = value.charAt(cursorPos);
            if (c >= 'a' && c <= 'z' || c == ' ')
                charSet = LOWER;
            else if (c >= 'A' && c <= 'Z')
                charSet = UPPER;
            else if (c >= '0' && c <= '9')
                charSet = DIGIT;
            else if (std::find(SYMBOLS.begin(), SYMBOLS.end(), c) != SYMBOLS.end())
                charSet = SYMBOL;
        }
    }
};

#endif
