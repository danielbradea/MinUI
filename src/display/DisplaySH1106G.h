#include <Adafruit_SH110X.h>
#include "DisplayInterface.h"
#include <cstdio>
#include <cstdarg>

// Concrete implementation of DisplayInterface using the Adafruit_SH1106G OLED display
class DisplaySH1106G : public DisplayInterface {
private:
  Adafruit_SH1106G oled;  // Instance of the Adafruit SH1106G OLED display driver

public:
  DisplaySH1106G(uint8_t _width, uint8_t _height, int8_t _reset)
    : oled(_width, _height, &Wire, _reset) {}

  bool begin(uint8_t _i2caddr = 0x3C) {
    
    if (!oled.begin(_i2caddr, true)) {
      return false;
    }
    oled.clearDisplay();
    oled.display();
    return true;
  }

  void drawFastHLine(int x, int y, int w, int color) override {
    oled.drawFastHLine(x, y, w, color);
  }

  void drawFastVLine(int x, int y, int h, int color) override {
    oled.drawFastVLine(x, y, h, color);
  }

  void drawLine(int x0, int y0, int x1, int y1, int color) override {
    oled.drawLine(x0, y0, x1, y1, color);
  }

  void drawRect(int x, int y, int w, int h, int color) override {
    oled.drawRect(x, y, w, h, color);
  }

  void fillRect(int x, int y, int w, int h, int color) override {
    oled.fillRect(x, y, w, h, color);
  }

  void drawCircle(int x0, int y0, int r, int color) override {
    oled.drawCircle(x0, y0, r, color);
  }

  void fillCircle(int x0, int y0, int r, int color) override {
    oled.fillCircle(x0, y0, r, color);
  }

  void drawRoundRect(int x, int y, int w, int h, int r, int color) override {
    oled.drawRoundRect(x, y, w, h, r, color);
  }

  void fillRoundRect(int x, int y, int w, int h, int r, int color) override {
    oled.fillRoundRect(x, y, w, h, r, color);
  }

  int width() const override {
    return oled.width();
  }

  int height() const override {
    return oled.height();
  }

  void setTextWrap(bool wrap) override {
    oled.setTextWrap(wrap);
  }

  void setTextColor(int color) override {
    oled.setTextColor(color);
  }

  void setTextColor(int color, int background) override {
    oled.setTextColor(color, background);
  }

  void setCursor(int x, int y) override {
    oled.setCursor(x, y);
  }

  void setTextSize(int size) override {
    oled.setTextSize(size);
  }

  void drawPixel(int x, int y, int color) override {
    oled.drawPixel(x, y, color);
  }

  void print(const char* text) override {
    oled.print(text);
  }

  void println(const char* text) override {
    oled.println(text);
  }

  void printf(const char* format, ...) override {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    oled.print(buffer);
  }

  void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) override {
    oled.drawTriangle(x0, y0, x1, y1, x2, y2, color);
  }

  void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) override {
    oled.fillTriangle(x0, y0, x1, y1, x2, y2, color);
  }

  void clearDisplay() override {
    oled.clearDisplay();
  }

  void setRotation(int rotation) override {
    oled.setRotation(rotation);
  }

  void invertDisplay(bool invert) override {
    oled.invertDisplay(invert);
  }

  void display() override {
    oled.display();
  }

  Adafruit_SH1106G& getDisplay() {
    return oled;
  }
};
