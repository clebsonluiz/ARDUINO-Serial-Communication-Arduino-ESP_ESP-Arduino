#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "WebSecrets.h"


const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASS;
const char *api_key_name = API_KEY_NAME;
const char *api_key_value = API_KEY_VALUE;

String content;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (!Serial) {
    ;
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to: ");
  Serial.print(ssid);
  Serial.print(" IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("OK!, Im Ready!");
  content.reserve(256);
}

void loop() {

  StaticJsonDocument<256> doc;

  String response;

  response.reserve(256);

  bool stringReady = false;

  while (!stringReady) {
    if (Serial.available()) {

      stringReady = true;

      DeserializationError error = deserializeJson(doc, Serial);
      if (error) {
        Serial.print("DeserializeJson function failed: ");
        Serial.println(error.c_str());
      }

      else {

        const int key = doc["key"].as<const int>();
        doc.remove("key");

        if (key == NULL || key <= 0) return;

        serializeJson(doc, response);
        content = response;
        delay(100);
        Serial.flush();

        WiFiClient client;
        HTTPClient http;

        http.begin(client, String("http://") + HOST_ADDRESS + key); //Only HTTP, Not HTTPS

        //Request Headers
        http.addHeader("Content-Type", "application/json");
        http.addHeader(api_key_name, api_key_value);

        Serial.println("[HTTP RESQUEST] ...");
        // Envia um PUT request
        int httpCode = http.PUT(content);

        Serial.println("[RESPONSE CODE]: " + httpCode);

        // Se o codigo for negativo ou 0 significa que a requisição deu errado
        if (httpCode > 0) {

          // se 201 (Codigo HTTP referente ao PUT request)
          if (httpCode == HTTP_CODE_CREATED) {
            const String payload = http.getString();
            Serial.println("Payload:\n");
            Serial.println(payload);
            Serial.println();
          }
        } else {
          Serial.print("[HTTP REQUEST FAILED], [ERROR: ");
          Serial.print(http.errorToString(httpCode).c_str());
          Serial.print("]");
          Serial.println();
        }

        http.end(); // Sempre precisa encerrar a conexão.
      }
    }
  }
}
