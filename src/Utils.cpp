#include "Utils.hpp"
#include "Event.hpp"
#include <stdlib.h>
#include <string.h>

using BB_Event::Event;
using BB_Event::event;

namespace BB_Utils {

    RAII_Malloc::RAII_Malloc(size_t size) {
        _size = size;
        ptr = malloc(_size);
        if(ptr == NULL) {
            event(Event::MALLOC__FAILED);
        }
    }
    RAII_Malloc::~RAII_Malloc() {
        free(ptr);
    }
    bool RAII_Malloc::failed_to_allocate() {
        return ptr == NULL;
    }
    size_t RAII_Malloc::size() {
        return _size;
    }

    size_t packedAndAligned(size_t alignment, size_t size)
    {
        if (size % alignment == 0)
        {
            return size;
        }
        return size + (alignment - (size % alignment));
    }

    char xorChecksum(char *buffer, size_t len)
    {
        char checksum = 0;
        for (size_t i = 0; i < len; i++)
        {
            checksum ^= buffer[i];
        }
        return checksum;
    }
    char xorChecksum(void *buffer, size_t len)
    {
        return xorChecksum((char *)buffer, len);
    }
    char xorChecksum(const void *buffer, size_t len)
    {
        return xorChecksum((char *)buffer, len);
    }

    bool isNullTerminated(char *buffer, size_t len)
    {
        for (size_t i = 0; i < len; i++)
        {
            if (buffer[i] == '\0')
            {
                return true;
            }
        }
        return false;
    }

    bool isEmptyString(const char *str)
    {
        return str[0] == '\0';
    }

    bool copyString(char *dest, const char *src, size_t len)
    {
        size_t src_len = strnlen(src, len);
        if(src_len == len) {
            return false;
        }
        strncpy(dest, src, len);
        return true;
    }

    bool copyNonEmptyString(char *dest, const char *src, size_t len)
    {
        size_t src_len = strnlen(src, len);
        if(src_len == 0 || src_len == len) {
            return false;
        }
        strncpy(dest, src, len);
        return true;
    }

}