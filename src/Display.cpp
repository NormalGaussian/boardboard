#include "Display.hpp"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#include <memory>

// Device specific pinouts
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 23
#define TFT_BL 4

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
        }
        ~DisplayActual() {
            tft.~Adafruit_ST7789();
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
    };
    
    std::shared_ptr<Display> display = nullptr;
    std::shared_ptr<Display> getDisplay()
    {
        if (display == nullptr)
        {
            display = std::make_shared<DisplayActual>();
        }
        return display;
    }
}