#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

namespace BB_Utils
{

    /**
     * RAII wrapper for malloc
     */
    class Allocation
    {
        size_t _size;
        void *ptr;

    public:
        Allocation(size_t size);
        ~Allocation();
        bool failed_to_allocate();
        size_t size();
        operator void *()
        {
            return ptr;
        }
        operator char *()
        {
            return (char *)ptr;
        }
    };

    /**
     * Calculate the size of a buffer required to store the data
     *  with a given alignment.
     *
     * e.g. packedAndAligned(4, 5) == 8
     *      packedAndAligned(4, 8) == 8
     *      packedAndAligned(4, 9) == 12
     */
    size_t packedAndAligned(size_t alignment, size_t size);

    /**
     * A simple XOR checksum over a buffer
     */
    char xorChecksum(char *buffer, size_t len);
    char xorChecksum(void *buffer, size_t len);
    char xorChecksum(const void *buffer, size_t len);

    /**
     * Check if the buffer is null terminated
     * i.e. it has a '\0' character in it
     * n.b. this does not check that the last character is a '\0'
     */
    bool isNullTerminated(char *buffer, size_t len);

    /**
     * Copy a string from src to dest, ensuring that the string is null terminated
     * and that the length is not exceeded.
     *
     * returns true if the string was copied, false if it was unsuitable
     */
    bool copyString(char *dest, const char *src, size_t len);

    /**
     * Copy a string from src to dest, ensuring that the string is null terminated and not empty
     * and that the length is not exceeded.
     *
     * returns true if the string was copied, false if it was unsuitable
     */
    bool copyNonEmptyString(char *dest, const char *src, size_t len);

    /**
     * Check if a string is empty
     */
    bool isEmptyString(const char *str);

}

#endif