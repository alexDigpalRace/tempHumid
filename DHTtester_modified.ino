
#include "DHT.h"
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define DHTPIN 2       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(/*rotation*/ U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE);  // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

unsigned long startTime;
bool isHeatIndex;

void setup() {
  startTime = millis();
  isHeatIndex = false;

  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  u8g2.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // Serial.print("Humidity: ");
  // Serial.print(String(h) + "% ");
  // Serial.print("Temperature: ");
  // Serial.print(String(t) + "'C ");
  // Serial.print("Heat index: " + String(hic) + "'C\n");

  if (isHeatIndex) {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 22, ("Heat Ind: " + String(hic) + " C").c_str());
    u8g2.sendBuffer();
  } else {
    u8g2.clearBuffer();                                           // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr);                           // choose a suitable font
    u8g2.drawStr(0, 15, ("Temp:  " + String(t) + " C").c_str());  // write something to the internal memory
    u8g2.drawStr(0, 30, ("Humid: " + String(h) + " %").c_str());  // write something to the internal memory
    u8g2.sendBuffer();                                            // transfer internal memory to the display
  }

  // wait 3 seconds before changing screens 
  if (millis() - startTime > 5000) {
    startTime = millis();
    isHeatIndex = !isHeatIndex;
  }
}
