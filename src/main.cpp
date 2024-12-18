#include "Network.hpp"
#include "Display.hpp"
#include "Logger.hpp"
#include "DisplayLogger.hpp"
#include "SerialLogger.hpp"
#include "EventLogger.hpp"
#include "Event.hpp"
#include "Person.hpp"
#include "Messages.hpp"

#include <memory>

#define WIFI_SSID "Custard"
#define WIFI_PASSWORD "TripleChocolateMuffins"

using BB_Logger::LogLevel;
using BB_Logger::defaultLogger;
using BB_Logger::getDisplayLogger;

std::shared_ptr<Logger> logger = nullptr;
std::shared_ptr<BB_Event::EventLogger> eventLogger = nullptr;
std::shared_ptr<BB_Display::Display> display = nullptr;
std::shared_ptr<BB_Network::Network> network = nullptr;
std::shared_ptr<Person> person = nullptr;

void drawTree();

void setup(void)
{
  person = std::make_shared<Person>();

  logger = BB_Logger::SerialLogger::create();
  eventLogger = BB_Event::EventLogger::create(logger);

  BB_Logger::setDefaultLogger(logger);
  
  logger->infof("ID: 0x%llx", ESP.getEfuseMac());
  
  logger->info("Initialising display");
  display = BB_Display::getDisplay();
  display->reset();
  logger->info("Display initialised");
  
  logger->info("Initialising network");
  network = BB_Network::getNetwork();

  if(network->hasConnectionDetails()) {
    logger->info("Connection details found");
  } else {
    logger->info("Connection details not found");
    logger->info("Setting connection info");
    if(!network->updateConnection(WIFI_SSID, WIFI_PASSWORD)) {
      logger->error("Failed to update connection info");
    } else {
      logger->info("Connection info updated");
    }
  }

  if(network->hasHostDetails()) {
    logger->info("Host details found");
  } else {
    logger->info("Host details not found");
    logger->info("Setting host info");
    if(!network->updateHost("BB_Network", "BB_Network_Password")) {
      logger->error("Failed to update host info");
    } else {
      logger->info("Host info updated");
    }
  }

  logger->info("Network initialised");

  logger->info("Setup Complete");
}

void loop() {
  // Print name
  if(person->isKnown()) {
    display->printlnf("Mery Christmas %s", person->name);
  } else {
    display->printlnf("Merry Christmas");
    display->printlnf("MAC: 0x%llx", ESP.getEfuseMac());
  }

  // Draw tree
  drawTree();

  // Write out instructions to host WiFi on phone

  // Tell the user which wifi network to host
  // The network name is their name, and the password
  //  is the first 8 characters of the device MAC address
  char ssid[32];
  memset(ssid, 0, sizeof(ssid));
  char password[64];
  memset(password, 0, sizeof(password));
  strncpy(ssid, person->name, sizeof(ssid));
  sprintf(password, "%llx", ESP.getEfuseMac());
  password[9] = 0;

  network->updateConnection(ssid, password);

  display->printlnf("Create a WiFi network with the following details:");
  display->printlnf("    SSID: %s", ssid);
  display->printlnf("Password: %s", password);

  // Connect to WiFi
  auto y = display->getCursorY();
  uint8_t spinner_phase = 0;
  while(true) {
    auto connected = network->connect(20);
    if(connected) {
      break;
    }
  }

  if(!network->connected()) {
    delay(2000);
  }

  while(network->connected()) {
    display->reset();

    display->printlnf("Merry Christmas %s", person->name);
    //display->printlnf("IP: %s", network->IP().c_str());
    
    // Draw tree
    drawTree();

    // Download the latest messages
    memset(BB_Messages::messages, 0, sizeof(BB_Messages::messages));
    display->printlnf("Downloading messages...");
    BB_Messages::update();
    display->printlnf(BB_Messages::messages);

    delay(300000);
  }
}

void drawTree() {
  display->setTextColour(0x07E0);
  display->printlnf("                     ");
  display->printlnf("          *          ");
  display->printlnf("         ***         ");
  display->printlnf("        *****        ");
  display->printlnf("       *******       ");
  display->printlnf("      *********      ");
  display->printlnf("        *****        ");
  display->printlnf("       *******       ");
  display->printlnf("      *********      ");
  display->printlnf("     ***********     ");
  display->printlnf("    *************    ");
  display->printlnf("   ***************   ");
  display->printlnf("     ***********     ");
  display->printlnf("    *************    ");
  display->printlnf("   ***************   ");
  display->printlnf("  *****************  ");
  display->printlnf(" ******************* ");
  display->setTextColour(0x9B80);
  display->printlnf("         ***         ");
  display->printlnf("         ***         ");
  display->printlnf("                     ");
  display->setTextColour(0xFFFF);
}