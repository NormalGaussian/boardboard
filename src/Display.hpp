#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// Device specific pinouts
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 23
#define TFT_BL 4

class Display
{
private:
    Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

public:
    Display(/* args */);
    ~Display();

    /**
     * Reset the display to a blank screen with all graphics
     *  settings reset to default.
     * 
     * This is useful when you do not know the state of the
     *  display and want to ensure consistency.
     */
    void reset();

    size_t printf(const char *format, ...);
    size_t printlnf(const char *format, ...);
};

/**
 * The device only has a single display, so it should be a singleton.
 */
Display display = Display();

#endif