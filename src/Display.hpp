#ifndef DISPLAY_H
#define DISPLAY_H

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
    };

    /**
     * The device only has a single display, so it should be a singleton.
     */
    std::shared_ptr<Display> getDisplay();
};

#endif