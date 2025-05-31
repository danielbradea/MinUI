#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H
#include <Arduino.h>
#include "DisplayInterface.h"
#include "../button/ButtonManager.h"
#include <algorithm>

class TextDisplay
{
private:
    DisplayInterface &display;

    static constexpr uint8_t DEFAULT_CHAR_WIDTH = 6;
    static constexpr uint8_t DEFAULT_CHAR_HEIGHT = 8;
    static constexpr uint8_t SCROLL_BAR_WIDTH = 4;

    uint16_t charsPerLine;
    uint16_t visibleLines;
    uint8_t charWidth;
    uint8_t charHeight;
    uint16_t lineSpacing;

    struct TextLine
    {
        String text;
    };

    TextLine *lines;
    uint16_t maxLines;
    uint16_t currentLines;
    uint16_t selectedIndex;
    uint16_t firstVisibleIndex;
    uint16_t horizontalScroll;

public:
    TextDisplay(DisplayInterface &disp, uint16_t maxLines = 300)
        : display(disp),
          maxLines(maxLines),
          currentLines(0),
          selectedIndex(0),
          firstVisibleIndex(0),
          horizontalScroll(0)
    {
        lines = new TextLine[maxLines];
        calculateDisplayParams();
    }

    ~TextDisplay()
    {
        delete[] lines;
    }

    void calculateDisplayParams()
    {
        charWidth = DEFAULT_CHAR_WIDTH;
        charHeight = DEFAULT_CHAR_HEIGHT;
        charsPerLine = (display.width() - SCROLL_BAR_WIDTH) / charWidth;
        lineSpacing = static_cast<uint16_t>(charHeight * 1.2);
        visibleLines = display.height() / lineSpacing;
    }

    void addLine(const String &text)
    {
        if (currentLines < maxLines)
        {
            lines[currentLines++].text = text;
        }
        else
        {
            // Shift toate liniile cu una în sus
            for (uint16_t i = 1; i < maxLines; ++i)
            {
                lines[i - 1] = lines[i];
            }
            lines[maxLines - 1].text = text;
        }

        // Mută selecția la ultima linie
        selectedIndex = currentLines > 0 ? currentLines - 1 : 0;

        // Scroll automat dacă e plin ecranul
        if (currentLines > visibleLines)
        {
            firstVisibleIndex = currentLines - visibleLines;
        }
    }

    void clear()
    {
        for (uint16_t i = 0; i < currentLines; ++i)
        {
            lines[i].text = "";
        }
        currentLines = 0;
        selectedIndex = 0;
        firstVisibleIndex = 0;
        horizontalScroll = 0;
    }

    void scrollUp()
    {
        if (selectedIndex > 0)
        {
            selectedIndex--;
            if (selectedIndex < firstVisibleIndex)
            {
                firstVisibleIndex = selectedIndex;
            }
        }
    }

    void scrollDown()
    {
        if (selectedIndex < currentLines - 1)
        {
            selectedIndex++;
            if (selectedIndex >= firstVisibleIndex + visibleLines)
            {
                firstVisibleIndex = selectedIndex - visibleLines + 1;
            }
        }
    }

    void scrollLeft(uint16_t step = 1)
    {
        uint16_t maxScroll = getMaxHorizontalScroll();
        horizontalScroll = std::min<uint16_t>(horizontalScroll + step, maxScroll);
    }

    void scrollRight(uint16_t step = 1)
    {
        if (horizontalScroll >= step)
        {
            horizontalScroll -= step;
        }
        else
        {
            horizontalScroll = 0;
        }
    }

    void draw()
    {
        display.setTextWrap(false);
        display.setTextSize(1);
        display.setTextColor(1);

        uint16_t maxVisible = std::min(visibleLines, static_cast<uint16_t>(currentLines - firstVisibleIndex));

        for (uint16_t i = 0; i < maxVisible; i++)
        {
            uint16_t lineIdx = firstVisibleIndex + i;
            String displayText = getDisplayText(lines[lineIdx].text);

            display.setCursor(0, i * lineSpacing);
            display.print((lineIdx == selectedIndex) ? ">" : " ");
            display.print(displayText.c_str());
        }

        drawScrollIndicator();
    }

    void handleInput(ButtonEvent buttonEvent)
    {
        if (buttonEvent.buttonName == "UP" && buttonEvent.action == ButtonAction::SHORT_CLICK)
            scrollUp();
        else if (buttonEvent.buttonName == "DOWN" && buttonEvent.action == ButtonAction::SHORT_CLICK)
            scrollDown();
        else if (buttonEvent.buttonName == "RIGHT" && buttonEvent.action == ButtonAction::SHORT_CLICK)
            scrollRight();
        else if (buttonEvent.buttonName == "LEFT" && buttonEvent.action == ButtonAction::SHORT_CLICK)
            scrollLeft();
        else if (buttonEvent.buttonName == "RIGHT" && buttonEvent.action == ButtonAction::DOUBLE_CLICK)
            scrollRight(5);
        else if (buttonEvent.buttonName == "LEFT" && buttonEvent.action == ButtonAction::DOUBLE_CLICK)
            scrollLeft(5);
    }

private:
    String getDisplayText(const String &text)
    {
        String paddedText = text;
        while (paddedText.length() < horizontalScroll + charsPerLine)
        {
            paddedText += " ";
        }
        return paddedText.substring(horizontalScroll, horizontalScroll + charsPerLine);
    }

    uint16_t getMaxHorizontalScroll()
    {
        uint16_t maxScroll = 0;
        uint16_t endLine = std::min(static_cast<uint16_t>(firstVisibleIndex + visibleLines), currentLines);

        for (uint16_t i = firstVisibleIndex; i < endLine; ++i)
        {
            const String &text = lines[i].text;
            uint16_t totalLength = std::max<uint16_t>(charsPerLine, static_cast<uint16_t>(text.length()));
            uint16_t possible = totalLength > charsPerLine ? totalLength - charsPerLine : 0;
            if (possible > maxScroll)
            {
                maxScroll = possible;
            }
        }

        return maxScroll;
    }

    void drawScrollIndicator() const
    {
        const int barX = display.width() - 2;
        const int barHeight = display.height();
        const int totalItems = currentLines;

        for (int y = 0; y < barHeight; ++y)
        {
            if (y % 2 == 0)
            {
                display.drawPixel(barX, y, 1);
            }
        }

        if (totalItems > visibleLines)
        {
            float percent = selectedIndex / static_cast<float>(totalItems - 1);
            int centerY = static_cast<int>(percent * (barHeight - 1));

            for (int dy = -1; dy <= 1; ++dy)
            {
                int y = centerY + dy;
                if (y >= 0 && y < barHeight)
                {
                    display.drawPixel(barX - 1, y, 1);
                    display.drawPixel(barX, y, 1);
                    display.drawPixel(barX + 1, y, 1);
                }
            }
        }
    }

    
};
#endif