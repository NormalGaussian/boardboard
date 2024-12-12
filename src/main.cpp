// #include <Arduino.h>

/**************************************************************************
  TTGO T-Display ST7789 OLED based on Adafruit example
  https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/examples/graphicstest/graphicstest.ino
 **************************************************************************/
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <WiFi.h>

// pinouts from https://github.com/Xinyuan-LilyGO/TTGO-T-Display
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 23
#define TFT_BL 4

#define WIFI_SSID "Custard"
#define WIFI_PASSWORD "TripleChocolateMuffins"

// constructor for data object named tft 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void scanNetworks();

void setup(void) {
  // Prepare the display
  pinMode(TFT_BL, OUTPUT);      // TTGO T-Display enable Backlight pin 4
  digitalWrite(TFT_BL, HIGH);   // T-Display turn on Backlight
  tft.init(135, 240);           // Initialize ST7789 240x135
}

void loop() {
  // Get WiFi networks
  scanNetworks();
  delay(10000);

  // Connect to a WiFi network and print our IP address
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.println("Connecting to WiFi");
  tft.printf("SSID: %s", WIFI_SSID);
  tft.println();
  tft.printf("Password: %s", WIFI_PASSWORD);
  tft.println();
  tft.println(); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint32_t iterations = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (iterations >= 300) {
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0, 0);
      iterations = 0;
    }
    tft.print(".");
    if (iterations % 15 == 0) {
      tft.println();
    }
    iterations++;
  }
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.println("Connected to WiFi");
  tft.println();
  tft.printf("IP Address: %s", WiFi.localIP().toString().c_str());
  tft.println();

  delay(10000);
}

void scanNetworks() {
  // Get ready to scan WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  if(n == 0) {
    Serial.println("no networks found");
  } else {
    
    // Clear the screen
    tft.fillScreen(ST77XX_BLACK);
    
    // Setup for printing
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(15, 0);
    tft.setTextWrap(true);
    
    tft.setTextSize(3);
    tft.printf("%d ", n);
    tft.println("WiFi Networks");

    tft.setTextSize(1);
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      tft.printf("%2d | %-32.32s (%4ld)", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
      tft.println();
    }
  }
}