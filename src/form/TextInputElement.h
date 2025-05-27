// TextInputElement.h
#ifndef TEXT_INPUT_ELEMENT_H
#define TEXT_INPUT_ELEMENT_H

#include "../button/ButtonManager.h"
#include "FormElement.h"
#include <string> // Schimbă în <WString.h> dacă folosești Arduino String

class TextInputElement : public FormElement
{
private:
    String text;
    String value;
    int cursorPos = 0;
    bool isEditing = false;
    bool isSelected = false;

public:
    explicit TextInputElement(const String &label, const String &defaultValue = "")
        : text(label), value(defaultValue.isEmpty() ? label : defaultValue) {}

    void draw(DisplayInterface &display, int x, int y) override
    {

        display.setTextColor(1);
        display.setCursor(x, y); // Elimină 0+ deoarece e redundant
        if (isSelected)
        {
            display.print((">" + text + ":").c_str());
        }
        else
        {
            display.print((text + ":").c_str());
        }
        display.setCursor(x + 2, y + 12);
        display.print(value.c_str());

        int boxHeight = 12;
        if (isEditing)
        {
            display.drawRoundRect(x, y + 10, display.width(), boxHeight, 1, 2);
            // Cursor doar dacă este în mod editare
            if (millis() % 1000 < 500)
            {
                int charWidth = 6; // Lățimea aproximativă a unui caracter
                int cursorX = x + 2 + (cursorPos * charWidth);
                display.drawLine(cursorX, y + 12, cursorX, y + 12 + boxHeight - 2, 1);
            }
        }
        else
        {
            display.drawRoundRect(x, y + 10, display.width(), boxHeight, 1, 1);
        }
    }

    void setSelected(bool select)
    {
        isSelected = select;
    }

    bool handleInput(ButtonEvent buttonEvent) override
    {
        if (!isEditing)
            return false;

        switch (buttonEvent.buttonPin)
        {
        case 4:
            if (buttonEvent.action == ButtonAction::SHORT_CLICK)
            {
                cursorPos = constrain(cursorPos - 1, 0, value.length());
            }

            return true;
        case 5:
            if (buttonEvent.action == ButtonAction::SHORT_CLICK)
            {
                cursorPos = constrain(cursorPos + 1, 0, value.length());
            }
            if (buttonEvent.action == ButtonAction::DOUBLE_CLICK)
            {
                if (cursorPos >= value.length())
                {
                    value += ' '; // Extinde dacă cursorul e la sfârșit
                }
                cycleCharAtCursor();
                return true;
            }
        default:
            return true;
        }
    }

    int getHeight() override
    {
        return 25;
    }

    bool canEdit() override { return true; }
    bool getEditing() override { return isEditing; }

    void setEditing(bool editing) override
    {
        isEditing = editing;
        cursorPos = constrain(cursorPos, 0, value.length());
    }

    const String &getValue() const { return value; } // Adăugat pentru acces la valoare

private:
    void cycleCharAtCursor()
    {
        if (cursorPos >= value.length())
            return;

        char c = value[cursorPos];
        if (c >= 'a' && c < 'z')
            c++;
        else if (c == 'z')
            c = 'A';
        else if (c >= 'A' && c < 'Z')
            c++;
        else if (c == 'Z')
            c = '0';
        else if (c >= '0' && c < '9')
            c++;
        else
            c = 'a';

        value.setCharAt(cursorPos, c);
    }
};
#endif