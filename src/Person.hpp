#pragma once

#include <stdint.h>

class Person {
    public:
        uint64_t mac;
        char name[32];
        Person(uint64_t id = 0x0);
        bool isKnown();
};
