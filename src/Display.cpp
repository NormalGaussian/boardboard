#include "Display.hpp"

/**
 * Construct a new Display object.
 */
Display::Display() {
    // Turn on the backlight (pin 4)
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    // Initalise with the correct resolution (x,y)
    tft.init(135, 240);
}

/**
 * Destroy the Display object.
 */
Display::~Display() {
    tft.~Adafruit_ST7789();
}

void Display::reset() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 0);
    tft.setFont(NULL); // built in font
    tft.setRotation(0);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.setTextWrap(true);
}

/**
 * Print a formatted string to the display.
 *
 * Wrapping, positioning, colour, size, rotation, and font are all
 *  dependent on the displays state.
 */
size_t Display::printf(const char *format, ...) {
    tft.printf(format);
}

/**
 * Print a formatted string to the display followed by a newline.
 * 
 * Wrapping, positioning, colour, size, rotation, and font are all
 *  dependent on the displays state.
 */
size_t Display::printlnf(const char *format, ...) {
    tft.printf(format);
    tft.println();
}
