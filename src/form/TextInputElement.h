#ifndef TEXT_INPUT_ELEMENT_H
#define TEXT_INPUT_ELEMENT_H

#include "../button/ButtonManager.h"
#include "FormElement.h"
#include <WString.h>
#include <vector>
#include <algorithm>

class TextInputElement : public FormElement
{
private:
    String label;
    String value;
    int cursorPos = 0;
    int visibleStart = 0;
    bool isEditing = false;
    bool isSelected = false;

    enum CharSet
    {
        LOWER,
        UPPER,
        DIGIT,
        SYMBOL
    };
    CharSet charSet = LOWER;

    const int BOX_HEIGHT = 14;
    const int TEXT_PADDING = 3;
    const int CHAR_WIDTH = 6;
    const int CURSOR_HEIGHT_OFFSET = 2;
    const int LABEL_HEIGHT = 10;

    const std::vector<char> SYMBOLS = {
        '.', '!', '?', '/', '\\', '~', '@', '#', '$', '%', '&', '^', '*',
        '(', ')', '[', ']', '{', '}', ';', ','};

public:
    explicit TextInputElement(const String &label, const String &defaultValue = "")
        : label(label), value(defaultValue) {}

    void draw(DisplayInterface &display, int x, int y) override
    {
        if (isSelected)
        {
            display.drawFastVLine(x, y, getHeight(), 1);
        }

        int boxY = y + LABEL_HEIGHT;
        int textX = x + 2 + TEXT_PADDING;
        int textY = boxY + TEXT_PADDING;
        int boxWidth = display.width() - 4;

        int maxVisibleChars = (boxWidth - 2 * TEXT_PADDING) / CHAR_WIDTH;

        if (cursorPos < visibleStart)
            visibleStart = cursorPos;
        else if (cursorPos >= visibleStart + maxVisibleChars)
            visibleStart = cursorPos - maxVisibleChars + 1;

        display.setTextColor(1);
        display.setCursor(x + 2, y + 1);
        display.print((label + ":").c_str());

        display.drawRoundRect(x + 2, boxY, boxWidth, BOX_HEIGHT, 3, 1);

        display.setCursor(textX, textY);
        String visibleValue = value.substring(visibleStart, visibleStart + maxVisibleChars);
        display.print(visibleValue.c_str());

        if (isEditing && (millis() % 1000 < 500))
        {
            int cursorX = textX + (cursorPos - visibleStart) * CHAR_WIDTH;
            int cursorY1 = textY + 8;
            int cursorY2 = boxY + BOX_HEIGHT - CURSOR_HEIGHT_OFFSET;
            display.drawFastHLine(cursorX, cursorY1, 5, 1);
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

    int getHeight() override { return LABEL_HEIGHT + BOX_HEIGHT + 5; }
    bool canEdit() override { return true; }
    bool getEditing() override { return isEditing; }

    void setEditing(bool editing) override
    {
        isEditing = editing;
        if (editing)
        {
            if (cursorPos >= value.length())
            {
                value += 'a';
            }
            charSet = LOWER;
        }
    }

    void setSelected(bool select) { isSelected = select; }
    const String &getValue() const { return value; }
    const String &getLabel() const { return label; }

private:
    void ensureCursorInValue()
    {
        while (cursorPos >= value.length())
        {
            value += ' ';
        }
    }

    void setCharToStartOfCharSet()
    {
        ensureCursorInValue();
        char c;
        switch (charSet)
        {
        case LOWER:
            c = 'a';
            break;
        case UPPER:
            c = 'A';
            break;
        case DIGIT:
            c = '0';
            break;
        case SYMBOL:
            c = SYMBOLS.front();
            break;
        }
        value.setCharAt(cursorPos, c);
    }

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
            if (it != SYMBOLS.end() && (it + 1) != SYMBOLS.end())
                c = *(it + 1);
            else
                c = SYMBOLS.front();
            break;
        }
        }

        value.setCharAt(cursorPos, c);
    }

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
            if (it != SYMBOLS.end() && it != SYMBOLS.begin())
                c = *(it - 1);
            else
                c = SYMBOLS.back();
            break;
        }
        }

        value.setCharAt(cursorPos, c);
    }

    void updateCharSetByCursor()
    {
        if (cursorPos < value.length())
        {
            char c = value.charAt(cursorPos);
            if (c >= 'a' && c <= 'z')
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