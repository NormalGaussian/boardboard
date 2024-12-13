#include "Network.hpp"
#include "Display.hpp"

#define WIFI_SSID "Custard"
#define WIFI_PASSWORD "TripleChocolateMuffins"

using BB_Network::network;
using BB_Display::display;

void setup(void) {
  display.reset();
  network.updateConnection(WIFI_SSID, WIFI_PASSWORD);
  network.updateHost("BB_Network", "BB_Network_Password");
}

void loop() {
  display.reset();
  switch(network.getMode()) {
    case BB_Network::Mode::CLIENT:
      network.disconnect();
      network.host();
      break;

    case BB_Network::Mode::OFF:
      network.host();
      break;
    
    //case BB_Network::Mode::HOST:
    //  network.stopHosting();
    //  network.connect();
    //  break;
    
    default:
      network.host();
      break;
  }
  
  delay(3000);
  
  for(int i = 0; i < 20; i++) {
    display.reset();
    display.printlnf("IP: %s", network.IP());
    display.printlnf("Mode: %d", network.getMode());
    display.printlnf("Iter: %d", i);
    delay(1000);
  }
  delay(60000);
}