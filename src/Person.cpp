#include "Person.hpp"
#include <string.h>
#include <Arduino.h>

const uint64_t EOIN_MAC = 0xe492746cdda0;
const uint64_t MATT_MAC = 0x1;
const uint64_t GEOR_MAC = 0x2;
const uint64_t MICH_MAC = 0x3;

  Person::Person(uint64_t id) {
    if(id == 0x0) {
        id = ESP.getEfuseMac();
    }
    memset(name, 0, sizeof(name));
    mac = id;
    switch(mac) {
      case EOIN_MAC:
        strcpy(name, "Eoin");
        break;
      case MATT_MAC:
        strcpy(name, "Matt");
        break;
      case GEOR_MAC:
        strcpy(name, "George");
        break;
      case MICH_MAC:
        strcpy(name, "Michael");
        break;
      default:
        strcpy(name, "Unknown");
        break;
    }
  };
  bool Person::isKnown() {
    return name[0] != 0;
  };

