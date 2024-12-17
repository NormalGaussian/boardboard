#include "Display.hpp"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#include <memory>

#include "Event.hpp"

// Device specific pinouts
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 23
#define TFT_BL 4

using BB_Event::event;
using BB_Event::Event;

namespace BB_Display
{

    class DisplayActual : public Display
    {
    private:
        Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
        char printBuffer[256] = "";

    public:
        DisplayActual() {
            // Ensure the buffer is clear
            memset(printBuffer, 0, sizeof(printBuffer));

            // Turn on the backlight (pin 4)
            pinMode(TFT_BL, OUTPUT);
            digitalWrite(TFT_BL, HIGH);

            // Initalise with the correct resolution (x,y)
            tft.init(135, 240);
            event(Event::DISPLAY__INITIALISED);
        }
        ~DisplayActual() {
            tft.~Adafruit_ST7789();
            event(Event::DISPLAY__DESTROYED);
        }
        void reset() override {
            // tft.fillScreen(ST77XX_RED); // A flash of red to show the reset
            tft.fillScreen(ST77XX_BLACK);
            tft.setCursor(0, 0);
            // tft.setFont(NULL); // built in font
            tft.setRotation(0);
            tft.setTextColor(ST77XX_WHITE);
            tft.setTextSize(1);
            tft.setTextWrap(true);
            event(Event::DISPLAY__RESET);
        };

        size_t printf(const char *format, ...) override {
            va_list args;
            va_start(args, format);
            vsnprintf(printBuffer, sizeof(printBuffer), format, args);
            va_end(args);
            size_t size = tft.print(printBuffer);
            memset(printBuffer, 0, sizeof(printBuffer));
            return size;
        };

        size_t printlnf(const char *format, ...) override {
            va_list args;
            va_start(args, format);
            vsnprintf(printBuffer, sizeof(printBuffer), format, args);
            va_end(args);
            size_t size = tft.println(printBuffer);
            return size;
        };

        int16_t getCursorX() override {
            return tft.getCursorX();
        }
        int16_t getCursorY() override {
            return tft.getCursorY();
        }
        void setCursor(int16_t x, int16_t y) override {
            tft.setCursor(x, y);
        }
        void resetCursorTo(int16_t x, int16_t y) {
            auto w = tft.getCursorX() - x;
            auto h = tft.getCursorY() - y;
            tft.writeFillRect(x, y, w, h, ST77XX_BLACK);
            tft.setCursor(x, y);
        }

        void resetLineTo(int16_t y) {
            auto w = tft.width();
            auto h = tft.getCursorY() - y;
            tft.writeFillRect(0, y, w, h, ST77XX_BLACK);
            tft.setCursor(0, y);
        }
    };


    class DisplayMock : public Display
    {
    public:
        DisplayMock() {}
        ~DisplayMock() {}
        void reset() override {}
        size_t printf(const char *format, ...) override
        {
            return 0;
        }
        size_t printlnf(const char *format, ...) override
        {
            return 0;
        }
        int16_t getCursorX() override
        {
            return 0;
        }
        int16_t getCursorY() override
        {
            return 0;
        }
        void setCursor(int16_t x, int16_t y) override {}
        void resetCursorTo(int16_t x, int16_t y) override {}
        void resetLineTo(int16_t y) override {}
    };
    
    std::shared_ptr<Display> display = nullptr;
    std::shared_ptr<Display> getDisplay()
    {
        event(Event::DISPLAY__GET_DISPLAY);
        if (display == nullptr)
        {
            display = std::make_shared<DisplayActual>();
        }
        return display;
    }
}