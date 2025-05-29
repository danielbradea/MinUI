#ifndef BUTTON_ELEMENT_H
#define BUTTON_ELEMENT_H

#include "FormElement.h"
#include <WString.h>
#include <functional>

class ButtonElement : public FormElement
{
private:
    String label;
    std::function<void(String)> callback;
    bool isSelected = false;

    // UI constants
    const int ELEMENT_HEIGHT = 12;
    const int CHAR_WIDTH = 6;
    const int CORNER_RADIUS = 4;

public:
    ButtonElement(const String &label, std::function<void(String)> callback)
        : label(label), callback(callback) {}

    void draw(DisplayInterface &display, int x, int y, int /*elementWidth*/) override
    {
        int textWidth = label.length() * CHAR_WIDTH;
        int rectWidth = textWidth + 6 * CHAR_WIDTH; // 3 chars padding left + right
        int rectHeight = getHeight();

        // Draw button rectangle with rounded corners
        if (isSelected)
        {
            display.fillRoundRect(x, y, rectWidth, rectHeight, CORNER_RADIUS, 1); // filled if selected
            display.setTextColor(0); // invert text
        }
        else
        {
            display.drawRoundRect(x, y, rectWidth, rectHeight, CORNER_RADIUS, 1);
            display.setTextColor(1);
        }

        // Center the label inside the button
        int textX = x + 3 * CHAR_WIDTH;
        int textY = y + (rectHeight - 8) / 2; // 8 = approximate text height

        display.setCursor(textX, textY);
        display.print(label.c_str());
    }

    bool handleInput(ButtonEvent buttonEvent) override
    {
        if (buttonEvent.action == ButtonAction::SHORT_CLICK &&
            buttonEvent.buttonName == "CENTER")
        {
            if (callback)
                callback(label);
            return true;
        }

        return false;
    }

    int getHeight() override { return ELEMENT_HEIGHT; }

    bool canEdit() override { return false; }

    bool getEditing() override { return false; }

    void setEditing(bool /*editing*/) override {} // not used

    void setSelected(bool select) { isSelected = select; }

    const String &getLabel() const { return label; }
};

#endif
