/*
  ======================================================================
  ТЕСТ ДАТЧИКА УРОВНЯ ВОДЫ HCW-M203
  ======================================================================
  
  НАЗНАЧЕНИЕ:
  Проверить работу датчика воды и определить правильную логику.
  
  ПОДКЛЮЧЕНИЕ:
  - VCC датчика → 5V Arduino (или через реле)
  - GND датчика → GND Arduino
  - SIG датчика → Pin 6 Arduino
  
  ПРИНЦИП РАБОТЫ:
  Датчик HCW-M203 выдает:
  - HIGH (1) когда датчик В ВОДЕ
  - LOW (0) когда датчик В ВОЗДУХЕ
  
  ВАЖНО:
  Если датчик работает наоборот, в основном коде включите инверсию:
  const bool WATER_SENSOR_INVERTED = true;
  ======================================================================
*/

#define WATER_SENSOR_PIN 6   // Сигнальный пин

// Если у вас подключено реле для импульсного питания — раскомментируйте:
// #define WATER_SENSOR_RELAY_PIN 8

void setup() {
  Serial.begin(9600);
  Serial.println(F("========================================"));
  Serial.println(F("ТЕСТ ДАТЧИКА УРОВНЯ ВОДЫ HCW-M203"));
  Serial.println(F("========================================"));
  
  pinMode(WATER_SENSOR_PIN, INPUT);
  Serial.println(F("Датчик подключен к пину 6"));
  
  // Если используете реле — настройте его
  // pinMode(WATER_SENSOR_RELAY_PIN, OUTPUT);
  // digitalWrite(WATER_SENSOR_RELAY_PIN, HIGH);  // Реле выключено
  
  Serial.println(F("\nИнструкция:"));
  Serial.println(F("1. Опустите датчик в воду → должен показывать HIGH (1)"));
  Serial.println(F("2. Выньте датчик из воды → должен показывать LOW (0)"));
  Serial.println(F("3. Если наоборот — включите инверсию в основном коде"));
  Serial.println(F("\nНачинаю мониторинг...\n"));
}

void loop() {
  // Если используете импульсное питание — раскомментируйте:
  // digitalWrite(WATER_SENSOR_RELAY_PIN, LOW);   // Включаем питание
  // delay(50);                                   // Ждём стабилизации
  
  // Читаем состояние датчика
  int sensorValue = digitalRead(WATER_SENSOR_PIN);
  
  // digitalRead() возвращает:
  // HIGH (1) — на пине 5В (датчик в воде)
  // LOW  (0) — на пине 0В (датчик в воздухе)
  
  // Выключаем питание (если используется реле)
  // digitalWrite(WATER_SENSOR_RELAY_PIN, HIGH);  // Выключаем питание
  
  // Выводим результат
  Serial.print(F("Датчик: "));
  
  if (sensorValue == HIGH) {
    Serial.println(F("HIGH (1) → ВОДА ЕСТЬ"));
  } else {
    Serial.println(F("LOW (0) → ВОДЫ НЕТ"));
  }
  
  // Визуальная индикация
  Serial.print(F("Статус: "));
  if (sensorValue == HIGH) {
    Serial.println(F("██████████ FULL"));
  } else {
    Serial.println(F("__________ EMPTY"));
  }
  
  Serial.println(F("----------------------------------------"));
  
  // Если датчик показывает противоположное значение — выдаём предупреждение
  static bool warningShown = false;
  if (!warningShown) {
    Serial.println(F("\n[СОВЕТ] Если датчик показывает воду в воздухе:"));
    Serial.println(F("  В основном коде установите:"));
    Serial.println(F("  const bool WATER_SENSOR_INVERTED = true;"));
    warningShown = true;
    Serial.println();
  }
  
  delay(1000);  // Читаем каждую секунду
}
