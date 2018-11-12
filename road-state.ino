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
        String url = "http://eismoinfo.lt/weather-conditions-retrospective?id=" + device + "&number=1";
        // Start reading device data
        http.begin(url);
        int httpCode = http.GET();

        //Check the returning code                                                                  
        if (httpCode > 0) {
        // Parsing
        const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(19) + 430;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject& root = jsonBuffer.parseObject(http.getString());

        // Parameters
        int id = root["id"]; // 1166
        int surinkimo_data_unix = root["surinkimo_data_unix"] // "1542033300"
        const char* dangos_temperatura = root["dangos_temperatura"]; // "5.6"
        const char* krituliu_tipas = root["krituliu_tipas"]; // "Nėra"
        const char* krituliu_kiekis = root["krituliu_kiekis"]; // "0"
        const char* kelio_danga = root["kelio_danga"]; // "Drėgna"
        const char* sukibimo_koeficientas = root["sukibimo_koeficientas"]; // "0.81"
        //const char* perspejimai0_pavadinimas = root["perspejimai"][0]["pavadinimas"]; // "Kelio dangos būklė: Drėgna"
        // const char* root_0_vejo_greitis_vidut = root_0["vejo_greitis_vidut"]; // "2.7"
        
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
