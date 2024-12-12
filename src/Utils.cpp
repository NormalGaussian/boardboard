#include "Utils.hpp"
#include "Event.hpp"
#include <stdlib.h>

using BB_Event::Event;
using BB_Event::event;

namespace BB_Utils {

class RAII_Malloc {
    size_t _size;
    void *ptr;
public:
    RAII_Malloc(size_t size) {
        _size = size;
        ptr = malloc(_size);
        if(ptr == NULL) {
            event(Event::MALLOC__FAILED);
        }
    }
    ~RAII_Malloc() {
        free(ptr);
    }
    bool failed_to_allocate() {
        return ptr == NULL;
    }
    size_t size() {
        return _size;
    }
    operator void *() {
        return ptr;
    }
    operator char *() {
        return (char *)ptr;
    }

};
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

}