#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// OLED display pins: VCC, GND, SCL, SDA

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// OLED display TWI address
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// WiFi Parameters
const char* ssid = "Good Life";
const char* password = "LifeIsGood";

void setup() {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      // display a line of text
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.print("Kur tas internetas?");
    // update display with all of the above graphics
    display.display();
    delay(1000);
    Serial.println("Connecting...");
  }
  display.clearDisplay();
  display.display();
  display.setCursor(0,10);
  display.println("Radau!");
  display.display();
  
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.println("Tikrinu  stoteliu duomenis.");
    display.display();
    delay(2000);
    
    HTTPClient http;  //Object of class HTTPClient
    
    // Define device IDs: 
    // 1166 = Vilnius
    // 1164 = Didziulio ezeras
    // 308 = Vievis
    // 1208 = Backonys
    // 1181 = Rumsiskes
    // 2709 = Meskinio tiltas
    // 310 = IX Fortas
    // 1262 = Vilijampole
    // 413 = Garliava
    int deviceIDs[] = {1166, 1164, 308, 1208, 1181, 2709, 310, 1262, 413}; // Vilnius - Garliava devices
    int deviceCount = 9;

// Start gathering weather station data
    for (int i = 0; i < deviceCount; i++)
    {
        int id = deviceIDs[i];
        String url = "http://eismoinfo.lt/weather-conditions-retrospective?id=";
        url = url + id;
        url = url + "&number=1";
        Serial.print("URL: ");
        Serial.println(url);
        http.begin(url);
        int httpCode = http.GET();

//Check the returning code                                                                  
        if (httpCode > 0) 
        {
// Parsing
          const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(19) + 430;
          DynamicJsonBuffer jsonBuffer(bufferSize);
          JsonArray& root = jsonBuffer.parseArray(http.getString());

          JsonObject& root_0 = root[0];
          int id = root_0["id"]; // "2709"
          String surinkimo_data = root_0["surinkimo_data"]; // "2018-11-13 17:52:33"
          String krituliu_tipas = root_0["krituliu_tipas"]; // "N"
          String krituliu_kiekis = root_0["krituliu_kiekis"]; //
          float dangos_temperatura = root_0["dangos_temperatura"]; // "7.1"
          String matomumas = root_0["matomumas"]; // 2000
          String kelio_danga = root_0["kelio_danga"]; // "Drėgna"
          float sukibimo_koeficientas = root_0["sukibimo_koeficientas"]; // "0.81" // "2.7"

  // Map station names
          const char* stotele;
          if (id == 1166)
              stotele = "VILNIUS";
          if (id == 1164)
              stotele = "DIDZIULIS";
          if (id == 308)
              stotele = "VIEVIS";
          if (id == 1208)
              stotele = "BACKONYS";
          if (id == 1181)
              stotele = "RUMSISKES";
          if (id == 2709)
              stotele = "MESKINIS";
          if (id == 310)
              stotele = "IX FORTAS";
          if (id == 1262)
              stotele = "SLABOTKE";
          if (id == 413)
              stotele = "GARLIAVA";

  // Define traction information
          String sukibimas;
          if (isnan(sukibimo_koeficientas))
            sukibimas = "-";
          else
          {
            float sukibimo_koeficientas_toint = sukibimo_koeficientas * 100;
            int sukibimo_koeficientas_int = round(sukibimo_koeficientas_toint);
            if (sukibimo_koeficientas_int > 80)
              sukibimas = " Geras";
            else if (sukibimo_koeficientas_int < 30)
              sukibimas = "! SLIDU";
            else if (sukibimo_koeficientas_int >= 30 && sukibimo_koeficientas_int <= 80)
              sukibimas = String(sukibimo_koeficientas_int);
          }

  // Define road surface state
          if (kelio_danga == "null")
            kelio_danga = "-";
          if (kelio_danga == "Dru0117gna")
            kelio_danga = "Dregna";
          if (kelio_danga == "u0160lapia")
            kelio_danga = "Slapia";

  // Define fall (rain/snow) information
          if (krituliu_tipas == "Nu0117ra" || krituliu_tipas == "N" || krituliu_tipas == "null")
            krituliu_tipas = "-";
          else if (krituliu_tipas == "Ru016bkas")
            krituliu_tipas = "Rukas";

  // Define visibility information
          if (matomumas == "N" || matomumas == "null")
            matomumas = "-";
          else if (matomumas == "2000")
            matomumas = "Geras";
          else
            matomumas = "Matomumas: " + matomumas;

  // Define road surface temperature
          String dangos_temp_str;
          if (isnan(dangos_temperatura))
            dangos_temp_str = "-";
          else
            dangos_temp_str = String(dangos_temperatura);

  // Display all data on the screen:
  //  Station name
          display.clearDisplay();
          display.display();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(0,0);
          display.println(stotele);
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.println("");

  //  Traction information
          display.print("Sukibimas: ");
          sukibimas.toUpperCase();
          display.println(sukibimas);
          display.println("");

  // Road surface state
          display.print("Danga: ");
          kelio_danga.toUpperCase();
          display.println(kelio_danga);

  // If visibility < 2000 then display visibility info
          if (matomumas != "Geras")
          {
            matomumas.toUpperCase();
            display.println(matomumas);
            display.print("Krituliai: ");
            krituliu_tipas.toUpperCase();
            display.println(krituliu_tipas);
          }

  // Fall (rain/snow) details, if any
          if (matomumas == "Geras" && krituliu_tipas != "-")
          {
            display.print("Krituliai: ");
            krituliu_tipas.toUpperCase();
            display.println(krituliu_tipas);
            display.println("");
          }
          else
          {
  // Road surface temperature
            display.println("");
            display.print("Dangos temp: ");
            display.println(dangos_temp_str);
            display.println("");
          }
          display.display();

  // Output to serial monitor
          Serial.print("Irenginio ID: ");
          Serial.println(id);
          Serial.print("Irenginys: ");
          Serial.println(stotele);
          Serial.print("Surinkimo data: ");
          Serial.println(surinkimo_data);
          Serial.print("Dangos temp: "); 
          Serial.println(dangos_temperatura);
          Serial.print("Krituliu tipas: "); 
          Serial.println(krituliu_tipas);
          Serial.print("Krituliu kiekis: "); 
          Serial.println(krituliu_kiekis);
          Serial.print("Kelio danga: "); 
          Serial.println(kelio_danga);
          Serial.print("Sukibimas: "); 
          Serial.println(sukibimo_koeficientas);
        }
        http.end();   //Close connection
        delay(5000);
    } // End reading weather station data

  }
  // Delay between checking again
  delay(10000);
}
