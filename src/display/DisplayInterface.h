#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

// Abstract interface for display functionality
// This allows different types of displays to be used interchangeably
class DisplayInterface {
public:
  virtual ~DisplayInterface() = default;

  // --- Pixel-level control ---
  virtual void drawPixel(int x, int y, int color) = 0;

  // --- Line & shape drawing ---
  virtual void drawFastHLine(int x, int y, int w, int color) = 0;
  virtual void drawFastVLine(int x, int y, int h, int color) = 0;
  virtual void drawLine(int x0, int y0, int x1, int y1, int color) = 0;
  virtual void drawRect(int x, int y, int w, int h, int color) = 0;
  virtual void fillRect(int x, int y, int w, int h, int color) = 0;
  virtual void drawCircle(int x0, int y0, int r, int color) = 0;
  virtual void fillCircle(int x0, int y0, int r, int color) = 0;
  virtual void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) = 0;
  virtual void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) = 0;
  virtual void drawRoundRect(int x, int y, int w, int h, int r, int color) = 0;
  virtual void fillRoundRect(int x, int y, int w, int h, int r, int color) = 0;

  // --- Text handling ---
  virtual void setCursor(int x, int y) = 0;
  virtual void setTextColor(int color) = 0;
  virtual void setTextColor(int color, int background) = 0;
  virtual void setTextSize(int size) = 0;
  virtual void setTextWrap(bool wrap) = 0;
  virtual void print(const char* text) = 0;
  virtual void println(const char* text) = 0;
  virtual void printf(const char* format, ...) = 0;

  // --- Display buffer control ---
  virtual void display() = 0;         // Pushes buffer to display
  virtual void clearDisplay() = 0;    // Clears buffer
  virtual void setRotation(int rotation) = 0;
  virtual void invertDisplay(bool invert) = 0;

  // --- Display dimensions ---
  virtual int width() const = 0;
  virtual int height() const = 0;
};

#endif // DISPLAY_INTERFACE_H
