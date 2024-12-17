#pragma once

#include <stddef.h>
#include <memory>

namespace BB_Display
{

    class Display
    {
    public:
        /**
         * Reset the display to a blank screen with all graphics
         *  settings reset to default.
         *
         * This is useful when you do not know the state of the
         *  display and want to ensure consistency.
         */
        virtual void reset() = 0;

        /**
         * Print text to the display at the current cursor position.
         */
        virtual size_t printf(const char *format, ...) = 0;

        /**
         * Print text to the display at the current cursor position
         *  and move the cursor to the next line.
         */
        virtual size_t printlnf(const char *format, ...) = 0;

        /**
         * Get the current cursor position.
         */
        virtual int16_t getCursorX() = 0;
        virtual int16_t getCursorY() = 0;
        /**
         * Set the cursor position.
         */
        virtual void setCursor(int16_t x, int16_t y) = 0;

        virtual void resetCursorTo(int16_t x, int16_t y) = 0;

        virtual void resetLineTo(int16_t y) = 0;
    };

    /**
     * The device only has a single display, so it should be a singleton.
     */
    std::shared_ptr<Display> getDisplay();
};
