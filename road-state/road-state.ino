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
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);

  // display a line of text
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,30);
  display.print("HoHoHo!");

  // update display with all of the above graphics
  display.display();
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {

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

    for (int i = 0; i < deviceCount; i++)
    {
        int id = deviceIDs[i];
        String url = "http://eismoinfo.lt/weather-conditions-retrospective?id=";
        url = url + id;
        url = url + "&number=1";
        // Start reading device data
        Serial.print("URL: ");
        Serial.println(url);
        http.begin(url);
        int httpCode = http.GET();

        //Check the returning code                                                                  
        if (httpCode > 0) {
        // Parsing
        const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(19) + 430;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonArray& root = jsonBuffer.parseArray(http.getString());

        JsonObject& root_0 = root[0];
        const char* surinkimo_data = root_0["surinkimo_data"]; // "1542124353"
        const char* id = root_0["id"]; // "2709"
        const char* surinkimo_data = root_0["surinkimo_data"]; // "2018-11-13 17:52:33"
        const char* krituliu_tipas = root_0["krituliu_tipas"]; // "N"
        const char* krituliu_kiekis = root_0["krituliu_kiekis"]; //
        const char* dangos_temperatura = root_0["dangos_temperatura"]; // "7.1"
        const char* kelio_danga = root_0["kelio_danga"]; // "Drėgna"
        const char* sukibimo_koeficientas = root_0["sukibimo_koeficientas"]; // "0.81" // "2.7"

        display.clearDisplay();
        display.display();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        // switch here to define device name and display on the screen
        switch (id) 
        {
          case 1166:
            display.println("VILNIUS");
          case 1164:
            display.println("DIDZIULIS");
          case 308:
            display.println("VIEVIS");
          case 1208:
            display.println("BACKONYS");
          case 1181:
            display.println("RUMSISKES");
          case 2709:
            display.println("MESKINIS");
          case 310:
            display.println("IX FORTAS");
          case 1262:
            display.println("VILIJAMPOLE");
          case 413:
            display.println("GARLIAVA");
        }
        // end of switch
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Danga: ");
        display.println(kelio_danga);
        display.print("Sukibimas: ");
        display.println(sukibimo_koeficientas);
        display.println(surinkimo_data);
        display.display();
        
        
        // Output to serial monitor
        Serial.print("Irenginio ID: ");
        Serial.println(id);
        //Serial.print("Irenginys: ");
        //Serial.println(pavadinimas);
        Serial.print("Surinkimo data: ");
        Serial.println(surinkimo_data_unix);
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
        delay(2000);
    }     // End reading device data

  }
  // Delay
  delay(60000);
}
