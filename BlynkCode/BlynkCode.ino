/***************************************************************************************************************
 * IoT Weather Station using NodeMCU ESP-12 Develop Kit V1.0
 *  DHT connected to NodeMCU pin D3
 *  Sensor Data on local OLED Display
 *  Sensor data sent to Blynk app
 *  TECC Garage
 ********************************************************************************************************************************/

/* ESP & Blynk */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "9269f83c355d4bfd94b7b1351cb7823b";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "azlynn";
char pass[] = "Alynn121";

/* OLED */
#include <ACROBOTIC_SSD1306.h> // library for OLED: SCL ==> D1; SDA ==> D2
#include <SPI.h>
#include <Wire.h>

/* DHT22*/
#include "DHT.h"
#define DHTPIN D3  
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float temp = 0;

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;


void setup()
{
  pinMode(D7,OUTPUT);
  Serial.begin(115200);
  delay(10);
  Blynk.begin(auth, ssid, pass);
  oledStart();
  dht.begin();
  timer.setInterval(2000L, getDhtData);
  timer.setInterval(5000L, sendUptime);
}

void loop()
{
  displayData();
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
}

/***************************************************
 * Get DHT data
 **************************************************/
void getDhtData(void)
{
  float tempIni = temp;
  float humIni = hum;
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  if (isnan(hum) || isnan(temp))   // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    temp = tempIni;
    hum = humIni;
    return;
  }
}

/***************************************************
 * Send DHT data to Blynk
 **************************************************/
void sendUptime()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(10, temp); //virtual pin V10 
}


/***************************************************
 * Start OLED
 **************************************************/
void oledStart(void)
{
  Wire.begin();  
  oled.init();                      // Initialze SSD1306 OLED display
  clearOledDisplay();
  //oled.setFont(font5x7);            // Set font type (default 8x8)
  oled.clearDisplay();              // Clear screen
  oled.setTextXY(0,0);              
  oled.putString("TECC GARAGE");
  oled.setTextXY(1,0);              
  oled.putString("IoT project");
}

/***************************************************
 * Display data at Serial Monitora & OLED Display
 **************************************************/
void displayData(void)
{ 
  oled.setTextXY(3,0);              // Set cursor position, start of line 2
  oled.putString("Temp: " + String(temp) + " oC");
  oled.setTextXY(4,0);              // Set cursor position, start of line 2
  oled.putString("Hum:  " + String(hum) + " %");
}

/***************************************************
 * Clear OLED Display
 **************************************************/
void clearOledDisplay()
{
  oled.setFont(font8x8);
  oled.setTextXY(0,0); oled.putString("                ");
  oled.setTextXY(1,0); oled.putString("                ");
  oled.setTextXY(2,0); oled.putString("                ");
  oled.setTextXY(3,0); oled.putString("                ");
  oled.setTextXY(4,0); oled.putString("                ");
  oled.setTextXY(5,0); oled.putString("                ");
  oled.setTextXY(6,0); oled.putString("                ");
  oled.setTextXY(7,0); oled.putString("                ");
  oled.setTextXY(0,0); oled.putString("                ");              
}
BLYNK_WRITE(V1)
{
  int value1 = param.asInt();
  if(value1 == HIGH)
  {
    digitalWrite(D7,HIGH);
  }
  else
  {
    digitalWrite(D7,LOW);
    
  }
}

