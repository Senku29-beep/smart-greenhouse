/*
  ======================================================================
  ТЕСТ ВСЕХ ДАТЧИКОВ (КОМПЛЕКСНЫЙ)
  ======================================================================
  
  НАЗНАЧЕНИЕ:
  Одновременно проверить работу всех датчиков системы:
  - DHT11 (температура, влажность воздуха)
  - Датчик влажности почвы
  - Фоторезистор (освещённость)
  - Датчик уровня воды (HCW-M203)
  
  ПОДКЛЮЧЕНИЕ:
  - DHT11          → Pin 2
  - Датчик почвы   → A0
  - Фоторезистор   → A1
  - Датчик воды    → Pin 6
  
  ПРИНЦИП РАБОТЫ:
  Выводит показания всех датчиков в Serial Monitor в удобном
  табличном формате и наглядно показывает, работают ли они.
  ======================================================================
*/

#include <DHT.h>

// ======================================================================
// НАСТРОЙКИ ПИНОВ
// ======================================================================
#define DHT_PIN          2   // DHT11
#define DHT_TYPE         DHT11
#define SOIL_PIN         A0  // Датчик влажности почвы
#define LIGHT_PIN        A1  // Фоторезистор
#define WATER_PIN        6   // Датчик уровня воды

// Создаём объект DHT
DHT dht(DHT_PIN, DHT_TYPE);

// Переменные для хранения показаний
float temperature = 0;
float humidity = 0;
int soilRaw = 0;
int lightRaw = 0;
bool waterPresent = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("========================================"));
  Serial.println(F("КОМПЛЕКСНЫЙ ТЕСТ ВСЕХ ДАТЧИКОВ"));
  Serial.println(F("========================================"));
  
  // Инициализация DHT11
  dht.begin();
  Serial.println(F("DHT11 инициализирован"));
  
  // Настройка пина датчика воды
  pinMode(WATER_PIN, INPUT);
  Serial.println(F("Датчик воды инициализирован"));
  
  Serial.println(F("\nЧтение датчиков...\n"));
  delay(2000);  // DHT11 требует времени на первый запуск
  
  Serial.println(F("┌─────────────┬──────────┬──────────┬────────────┐"));
  Serial.println(F("│   ДАТЧИК    │ ЗНАЧЕНИЕ │ ЕД.ИЗМ.  │   СТАТУС   │"));
  Serial.println(F("├─────────────┼──────────┼──────────┼────────────┤"));
}

void loop() {
  // ====================================================================
  // 1. ЧТЕНИЕ DHT11 (температура и влажность)
  // ====================================================================
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  if (!isnan(temp)) temperature = temp;
  if (!isnan(hum)) humidity = hum;
  
  // ====================================================================
  // 2. ЧТЕНИЕ ДАТЧИКА ВЛАЖНОСТИ ПОЧВЫ
  // ====================================================================
  soilRaw = analogRead(SOIL_PIN);
  
  // Преобразуем в проценты (как в основном коде)
  int soilPercent = map(soilRaw, 233, 554, 100, 0);
  soilPercent = constrain(soilPercent, 0, 100);
  
  // ====================================================================
  // 3. ЧТЕНИЕ ФОТОРЕЗИСТОРА
  // ====================================================================
  lightRaw = analogRead(LIGHT_PIN);
  
  // Преобразуем в проценты (инвертированно!)
  int lightPercent = map(lightRaw, 0, 675, 100, 0);
  lightPercent = constrain(lightPercent, 0, 100);
  
  // ====================================================================
  // 4. ЧТЕНИЕ ДАТЧИКА ВОДЫ
  // ====================================================================
  waterPresent = (digitalRead(WATER_PIN) == HIGH);
  
  // ====================================================================
  // 5. ВЫВОД РЕЗУЛЬТАТОВ
  // ====================================================================
  
  // Температура
  Serial.print(F("│ Температура │    "));
  Serial.print(temperature, 1);
  Serial.print(F("   │    °C     │ "));
  if (!isnan(temperature) && temperature > 0 && temperature < 50) {
    Serial.println(F("      OK     │"));
  } else {
    Serial.println(F("   ОШИБКА!   │"));
  }
  
  // Влажность воздуха
  Serial.print(F("│ Влажность   │    "));
  Serial.print(humidity, 0);
  Serial.print(F("   │     %     │ "));
  if (!isnan(humidity) && humidity >= 20 && humidity <= 90) {
    Serial.println(F("      OK     │"));
  } else {
    Serial.println(F("   ОШИБКА!   │"));
  }
  
  // Влажность почвы
  Serial.print(F("│ Почва       │    "));
  Serial.print(soilPercent);
  Serial.print(F("   │     %     │ "));
  if (soilPercent >= 0 && soilPercent <= 100) {
    Serial.println(F("      OK     │"));
  } else {
    Serial.println(F("   ОШИБКА!   │"));
  }
  
  // Освещённость
  Serial.print(F("│ Освещённость│    "));
  Serial.print(lightPercent);
  Serial.print(F("   │     %     │ "));
  if (lightPercent >= 0 && lightPercent <= 100) {
    Serial.println(F("      OK     │"));
  } else {
    Serial.println(F("   ОШИБКА!   │"));
  }
  
  // Уровень воды
  Serial.print(F("│ Вода в баке │    "));
  Serial.print(waterPresent ? " ЕСТЬ " : " НЕТ  ");
  Serial.print(F(" │    -      │ "));
  Serial.println(waterPresent ? "      OK     │" : "   ВНИМАНИЕ! │");
  
  Serial.println(F("└─────────────┴──────────┴──────────┴────────────┘"));
  Serial.println();
  
  // Визуальная шкала влажности почвы
  Serial.print(F("Почва: ["));
  for (int i = 0; i < soilPercent / 10; i++) Serial.print("#");
  for (int i = soilPercent / 10; i < 10; i++) Serial.print(".");
  Serial.print(F("] "));
  Serial.print(soilPercent);
  Serial.println(F("%"));
  
  // Визуальная шкала освещённости
  Serial.print(F("Свет:  ["));
  for (int i = 0; i < lightPercent / 10; i++) Serial.print("#");
  for (int i = lightPercent / 10; i < 10; i++) Serial.print(".");
  Serial.print(F("] "));
  Serial.print(lightPercent);
  Serial.println(F("%"));
  
  Serial.println(F("----------------------------------------"));
  Serial.println();
  
  // Задержка между измерениями
  delay(2000);
}
