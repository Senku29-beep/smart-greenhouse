// Тест датчика температуры и влажности DHT11
#include <DHT.h>

#define DHT_PIN 2
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Testing DHT11...");
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  if (isnan(temp) || isnan(hum)) {
    Serial.println("ERROR: Sensor not responding!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("C, Humidity: ");
    Serial.print(hum);
    Serial.println("%");
  }
  
  delay(2000);
}
