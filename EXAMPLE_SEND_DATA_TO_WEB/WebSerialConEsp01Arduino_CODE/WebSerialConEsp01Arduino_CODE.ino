#include <ArduinoJson.h>

#include <DHT.h>
#include "SoftwareSerial.h"

#define pinKY015 5

#define ID_DHT11 1

/*
   Defindo o sensor dht que é usado pelo modulo (DHT11).
 **/
DHT dht11(pinKY015, DHT11);

SoftwareSerial ESP_Serial(2, 3); // RX, TX, respectivamente, da placa Arduino

void setup() {
  Serial.begin(9600);
  ESP_Serial.begin(9600);
  dht11.begin();

  delay(2000);
}

void loop() {

  /**
     A leitura da temperatura ou umidade pode levar aprximadamente 300ms
     O atraso do sensor pode chegar a 2 segundos
   **/
  float tempAr = dht11.readTemperature(); // lê a temperatura em Graus Celsius
  float umidAr = dht11.readHumidity(); // lê a umidade em %

  if (isnan(umidAr) || isnan(tempAr)) {
    Serial.println("Falha na leitura do Sensor DHT11!");
    delay(2000); // Delay de 2 segundos
  }
  else {

    StaticJsonDocument<256> dhtJson;
    dhtJson["key"] = ID_DHT11;
    dhtJson["sensor"] = "Modulo (DHT11)";
    dhtJson["descricao"] = "Monitorando Temperatura e umidade da zona de testes...";
    JsonArray statusList = dhtJson.createNestedArray("status");

    char str_tempAr[25]; // Suficiente para comportar a string e os 5 bits de informação "-00.00"
    char str_umidAr[25]; // Suficiente para comportar a string e os 5 bits de informação "-00.00"

    sprintf(str_tempAr, "Temperatura: %d.%02d *C", (int)tempAr, (int)(abs(tempAr) * 100) % 100); // Works with negative floats
    sprintf(str_umidAr, "Umidade: %d.%02d %%", (int)umidAr, (int)(abs(umidAr) * 100) % 100); // Works with negative floats

    statusList.add(str_tempAr);
    statusList.add(str_umidAr);

    serializeJson(dhtJson, ESP_Serial);

    delay(5000);
  }
  
//  while (ESP_Serial.available()){
//    char c = ESP_Serial.read();
//    Serial.print(c);
//  }

}
