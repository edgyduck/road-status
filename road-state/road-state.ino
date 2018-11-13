#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

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

    for (int device = 0; device < deviceCount; device++)
    {
        int id = deviceIDs[device];
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
        //JsonObject& root = jsonBuffer.parseObject(http.getString());
        JsonArray& root = jsonBuffer.parseArray(http.getString());

        JsonObject& root_0 = root[0];
        const char* surinkimo_data_unix = root_0["surinkimo_data_unix"]; // "1542124353"
        const char* id = root_0["id"]; // "2709"
        const char* surinkimo_data = root_0["surinkimo_data"]; // "2018-11-13 17:52:33"
        //const char* t_0_oro_temperatura = root_0["oro_temperatura"]; // "7"
        //const char* root_0_vejo_greitis_vidut = root_0["vejo_greitis_vidut"]; // "4.47"
        const char* krituliu_tipas = root_0["krituliu_tipas"]; // "N"
        const char* krituliu_kiekis = root_0["krituliu_kiekis"]; //
        const char* dangos_temperatura = root_0["dangos_temperatura"]; // "7.1"
        //const char* root_0_matomumas = root_0["matomumas"]; // "N"
        //const char* root_0_rasos_taskas = root_0["rasos_taskas"]; // "5.36"
        const char* kelio_danga = root_0["kelio_danga"]; // "Drėgna"
        //const char* root_0_vejo_greitis_maks = root_0["vejo_greitis_maks"]; // "4.47"
        //const char* root_0_vejo_kryptis = root_0["vejo_kryptis"]; // "Pietų"
        const char* sukibimo_koeficientas = root_0["sukibimo_koeficientas"]; // "0.81" // "2.7"
        
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
    }     // End reading device data

  }
  // Delay
  delay(60000);
}
