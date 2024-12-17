#include "Network.hpp"
#include "Display.hpp"
#include "Logger.hpp"
#include "DisplayLogger.hpp"
#include "SerialLogger.hpp"

#include <memory>

#define WIFI_SSID "Custard"
#define WIFI_PASSWORD "TripleChocolateMuffins"

using BB_Display::getDisplay;
using BB_Network::getNetwork;
using BB_Network::Mode;
using BB_Logger::LogLevel;
using BB_Logger::defaultLogger;
using BB_Logger::getDisplayLogger;
using BB_Logger::setDefaultLogger;
using BB_Logger::SerialLogger;

void setup(void)
{
  auto serialLogger = SerialLogger::create();
  auto logger = setDefaultLogger(serialLogger);
  
  logger->info("Initialising display");
  auto display = getDisplay();
  display->reset();
  logger->info("Display initialised");
  
  logger->info("Initialising network");
  auto network = getNetwork();

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

void loop()
{
  auto display = getDisplay();
  auto network = getNetwork();
  auto logger = defaultLogger;

  display->reset();
  logger->info("Outer Looping");
  display->printlnf("IP: %s", network->IP().c_str());
  display->printlnf("Mode: %d", network->getMode());

  switch (network->getMode())
  {
  case BB_Network::Mode::CLIENT:
    logger->info("Mode: CLIENT");
    network->disconnect();
    network->host();
    break;

  case BB_Network::Mode::OFF:
    logger->info("Mode: OFF");
    network->host();
    break;

  case BB_Network::Mode::HOST:
    logger->info("Mode: HOST");
    network->stopHosting();
    network->connect();
    break;

  default:
    logger->info("Mode: DEFAULT");
    network->host();
    break;
  }

  delay(3000);

  for (int i = 0; i < 20; i++)
  {
    if(i%5 == 0) display->reset();
    logger->info("Inner Looping");
    display->printlnf("IP: %s", network->IP().c_str());
    display->printlnf("Mode: %d", network->getMode());
    display->printlnf("Iter: %d", i);
    delay(1000);
  }
}