## 📈 Перспективы развития проекта

Проект имеет большой потенциал для дальнейшего улучшения и масштабирования. Ниже представлены основные направления развития, разделённые по приоритетам и сложности реализации.

---

### 1. Ближайшие улучшения (1-3 месяца)
#### 1.1. Второй датчик влажности почвы
* **Проблема:** при выходе из строя одного датчика или при его некорректной работе система может неправильно определять влажность почвы, что приведёт к гибели растений.
* **Решение:** добавить второй датчик влажности почвы на аналоговый пин A2.
* **Реализация:**
```cpp
#define SOIL_MOISTURE_PIN_1 A0
#define SOIL_MOISTURE_PIN_2 A2

int soilRaw1 = analogRead(SOIL_MOISTURE_PIN_1);
int soilRaw2 = analogRead(SOIL_MOISTURE_PIN_2);
int soilRaw = (soilRaw1 + soilRaw2) / 2;  // Усреднение показаний
```
* **Преимущества:**
* Отказоустойчивость (при выходе одного датчика система продолжает работать)
* Более точные показания (усреднение двух измерений)
* Возможность обнаружить неисправность датчика (при большом расхождении показаний)

#### 1.2. Датчик CO₂ (MH-Z19B)
**Почему это важно:** углекислый газ (CO₂) необходим растениям для фотосинтеза. В закрытой теплице концентрация CO₂ может падать ниже оптимального уровня (400 ppm), что замедляет рост растений.

**Характеристики датчика MH-Z19B:**
| Параметр | Значение |
|----------|----------|
| Измеряемый диапазон | 0-5000 ppm |
| Точность | ±50 ppm + 3% |
| Интерфейс | UART / PWM |
| Напряжение питания | 5В |

**Логика работы:**
* При CO₂ < 400 ppm — открыть форточку или включить вентилятор
* При CO₂ > 1200 ppm — также открыть форточку (слишком высокая концентрация вредна)
* В дневное время поддерживать CO₂ на уровне 800-1000 ppm для максимальной скорости фотосинтеза

**Реализация:**
```cpp
#include <SoftwareSerial.h>
SoftwareSerial co2Serial(10, 11); // RX, TX

void readCO2() {
  co2Serial.write(0xFF);
  co2Serial.write(0x01);
  co2Serial.write(0x86);
  co2Serial.write(0x00);
  co2Serial.write(0x00);
  co2Serial.write(0x00);
  co2Serial.write(0x00);
  co2Serial.write(0x00);
  co2Serial.write(0x79);
  
  if (co2Serial.available() >= 9) {
    byte response[9];
    for (int i = 0; i < 9; i++) {
      response[i] = co2Serial.read();
    }
    int co2Value = (response[2] << 8) | response[3];
    Serial.print("CO2: ");
    Serial.print(co2Value);
    Serial.println(" ppm");
  }
}
```

#### 1.3. Вентилятор 12V
**Назначение:**
* Циркуляция воздуха внутри теплицы (предотвращение застоя воздуха и грибковых заболеваний)
* Охлаждение фитоленты (увеличение срока службы)
* Опыление томатов и других самоопыляемых культур
* Выравнивание температуры по объёму теплицы

**Подключение:**
* Вентилятор 12В подключается через дополнительное реле на пин 9
* Включается по таймеру (например, 5 минут каждый час)
* Или включается при превышении температуры 30°C

**Реализация:**
```cpp
#define FAN_PIN 9

void controlFan() {
  static unsigned long lastFanRun = 0;
  unsigned long currentTime = millis();
  
  // Включаем вентилятор на 5 минут каждый час
  if (currentTime - lastFanRun > 3600000) {  // 1 час
    digitalWrite(FAN_PIN, HIGH);  // Включаем
    delay(300000);                 // 5 минут
    digitalWrite(FAN_PIN, LOW);   // Выключаем
    lastFanRun = currentTime;
  }
  
  // Аварийное включение при сильной жаре
  if (tempFiltered > 35.0) {
    digitalWrite(FAN_PIN, HIGH);
  } else if (tempFiltered < 30.0 && millis() - lastFanRun > 60000) {
    digitalWrite(FAN_PIN, LOW);
  }
}
```

#### 1.4. Логирование на microSD
**Назначение:** запись всех показаний датчиков на карту памяти для последующего анализа.

**Что записывать:**
* Время (timestamp)
* Температура воздуха
* Влажность воздуха
* Освещённость
* Влажность почвы
* Уровень воды в баке
* Состояние насоса (вкл/выкл)
* Состояние фитоленты (вкл/выкл)
* Состояние форточки (открыта/закрыта)

**Формат данных (CSV):**
```cpp
2026-05-25 10:00:00,25.3,58,45,35,FULL,OFF,OFF,CLOSED
2026-05-25 10:05:00,25.5,57,46,36,FULL,OFF,OFF,CLOSED
2026-05-25 10:10:00,25.8,56,44,34,FULL,OFF,OFF,CLOSED
```

**Реализация:**
```cpp
#include <SD.h>
File dataFile;

void logData() {
  if (!dataFile) {
    dataFile = SD.open("datalog.csv", FILE_WRITE);
  }
  
  dataFile.print(millis() / 1000);  // время в секундах
  dataFile.print(",");
  dataFile.print(tempFiltered);
  dataFile.print(",");
  dataFile.print(humidityFiltered);
  dataFile.print(",");
  dataFile.print(lightLevelFiltered);
  dataFile.print(",");
  dataFile.print(soilMoistureFiltered);
  dataFile.print(",");
  dataFile.println(waterPresent ? "FULL" : "EMPTY");
  dataFile.flush();  // гарантированная запись на карту
}
```

---

### 2. Среднесрочные улучшения (3-6 месяцев)
#### 2.1. Удалённое управление через Blynk / Telegram
**Назначение:** возможность управлять теплицей со смартфона из любой точки мира.

**Варианты реализации:**
**Вариант А: Замена Arduino на ESP8266 (NodeMCU)**
* Встроенный Wi-Fi
* Совместимость с Arduino IDE
* Цена: 400-600 ₽
* Возможность подключения к Blynk, ThingSpeak, Home Assistant

**Вариант Б: Добавление Wi-Fi модуля ESP-01 к Arduino**
* Подключается через Serial (пины 0 и 1)
* Цена: 200-300 ₽
* Меньше гибкости, но дешевле

**Функционал в приложении Blynk:**
* Графики температуры, влажности, освещённости
* Индикация уровня воды
* Кнопки ручного управления поливом и светом
* Настройка порогов с телефона
* Уведомления: «В баке закончилась вода», «Температура выше 30°C»

**Пример кода для Blynk (ESP8266):**
```cpp
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "your_blynk_token";
char ssid[] = "your_wifi_ssid";
char pass[] = "your_wifi_password";

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  
  // Отправка данных на сервер Blynk
  Blynk.virtualWrite(V0, tempFiltered);      // температура
  Blynk.virtualWrite(V1, humidityFiltered);  // влажность
  Blynk.virtualWrite(V2, soilMoistureFiltered); // влажность почвы
  
  delay(1000);
}

// Управление насосом через кнопку в приложении
BLYNK_WRITE(V3) {
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(PUMP_PIN, HIGH);
  } else {
    digitalWrite(PUMP_PIN, LOW);
  }
}
```

#### 2.2. Автоматическое внесение удобрений
**Назначение:** дозированная подача жидких удобрений при каждом поливе.

**Компоненты:**
* Перистальтический насос 12В (200-400 ₽)
* Ёмкость с жидким удобрением
* Трубки для подачи удобрения в капельную линию

**Реализация:**
```cpp
#define FERTILIZER_PIN 10

void controlFertilizer() {
  static unsigned long lastFertilizerTime = 0;
  const unsigned long FERTILIZER_INTERVAL = 604800000;  // 7 дней (неделя)
  
  // Если полив включён и пришло время удобрять
  if (pumpState && (millis() - lastFertilizerTime > FERTILIZER_INTERVAL)) {
    digitalWrite(FERTILIZER_PIN, HIGH);  // Включаем насос удобрений
    delay(30000);  // 30 секунд подачи удобрений
    digitalWrite(FERTILIZER_PIN, LOW);
    lastFertilizerTime = millis();
    Serial.println("Fertilizer applied");
  }
}
```

#### 2.3. Автоматическая калибровка датчиков
**Проблема:** со временем датчики могут дрейфовать (менять показания даже при одинаковых условиях).
**Решение:** встроенная калибровка без участия человека.

**Для датчика влажности почвы:**
* Запоминать максимальное значение (сухая земля) и минимальное (мокрая земля)
* Обновлять калибровочные значения, если новые экстремумы выходят за старые пределы
```cpp
void autoCalibrateSoil() {
  int currentValue = readSoilMoistureStable();
  
  if (currentValue > SOIL_DRY_CALIB) {
    SOIL_DRY_CALIB = currentValue;  // Новая сухая граница
    EEPROM.put(0, SOIL_DRY_CALIB);  // Сохраняем в EEPROM
  }
  if (currentValue < SOIL_WET_CALIB) {
    SOIL_WET_CALIB = currentValue;  // Новая мокрая граница
    EEPROM.put(4, SOIL_WET_CALIB);  // Сохраняем в EEPROM
  }
}
```

#### 2.4. WEB-интерфейс с графиками
**Назначение:** визуализация показаний датчиков в реальном времени через веб-страницу.

**Технологии:**
* ESP8266 как веб-сервер
* HTML/CSS для интерфейса
* JavaScript + Chart.js для графиков
* AJAX для обновления данных без перезагрузки страницы

**Пример страницы:**
```cpp
<!DOCTYPE html>
<html>
<head>
  <title>Умная теплица</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
  <h1>Умная теплица - мониторинг</h1>
  <canvas id="tempChart" width="400" height="200"></canvas>
  <p>Температура: <span id="temp">--</span> °C</p>
  <p>Влажность: <span id="hum">--</span> %</p>
  <p>Влажность почвы: <span id="soil">--</span> %</p>
  
  <button onclick="turnOnPump()">Включить полив</button>
  <button onclick="turnOffPump()">Выключить полив</button>
</body>
</html>
```
---

### 3. Долгосрочные улучшения (6-12 месяцев)
#### 3.1. БИБЛИОТЕКА РАСТЕНИЙ (Plant Library) — Главная инновация
#### Концепция

Одним из ключевых недостатков существующих умных теплиц является их **жёсткая настройка** под один тип растений или универсальные параметры. Пользователь не может просто выбрать растение и получить автоматически настроенные пороги температуры, влажности, освещённости и полива.

**Идея:** создать встроенную базу данных (библиотеку) растений, где для каждого вида будут прописаны оптимальные условия роста. Пользователь выбирает растение через интерфейс (дисплей + кнопки, Web-интерфейс или приложение), и система автоматически применяет соответствующие настройки.

#### Пример таблицы параметров для библиотеки растений

| Растение | Влажность почвы (мин/макс) | Температура (мин/макс) | Освещённость (PAR) | Влажность воздуха | Полив (л/день) |
|----------|---------------------------|----------------------|--------------------|--------------------|----------------|
| Томаты | 40-70% | 18-28°C | 400-600 мкмоль/м²/с | 60-70% | 3-5 л |
| Огурцы | 50-80% | 20-30°C | 300-500 мкмоль/м²/с | 70-80% | 4-6 л |
| Перец | 40-65% | 18-28°C | 300-500 мкмоль/м²/с | 60-70% | 2-4 л |
| Баклажаны | 45-70% | 20-30°C | 400-600 мкмоль/м²/с | 60-70% | 3-5 л |
| Салат | 50-75% | 15-22°C | 200-300 мкмоль/м²/с | 60-75% | 1-2 л |
| Редис | 40-60% | 10-20°C | 200-300 мкмоль/м²/с | 50-60% | 1-2 л |
| Клубника | 50-70% | 15-25°C | 300-500 мкмоль/м²/с | 60-70% | 2-3 л |
| Базилик | 40-60% | 18-25°C | 300-500 мкмоль/м²/с | 60-70% | 1-2 л |
| Петрушка | 40-60% | 15-22°C | 200-400 мкмоль/м²/с | 60-70% | 1-2 л |
| Укроп | 40-60% | 15-22°C | 200-400 мкмоль/м²/с | 60-70% | 1-2 л |

#### Реализация в коде (пример на Arduino)

```cpp
// =====================================================================
// БИБЛИОТЕКА РАСТЕНИЙ
// =====================================================================

// Структура, описывающая параметры для одного растения
struct Plant {
  const char* name;           // Название растения
  int soilDry;                // Влажность для включения полива (%)
  int soilWet;                // Влажность для выключения полива (%)
  float tempMin;              // Минимальная температура для форточки (°C)
  float tempMax;              // Максимальная температура для форточки (°C)
  int lightOn;                // Освещённость для включения лампы (%)
  int lightOff;               // Освещённость для выключения лампы (%)
  int airHumidityMin;         // Минимальная влажность воздуха (%)
  int airHumidityMax;         // Максимальная влажность воздуха (%)
  int wateringTime;           // Время работы насоса (мс)
  int wateringInterval;       // Интервал между поливами (мин)
};

// Массив с параметрами для разных растений
const Plant plantDatabase[] = {
  // Название,        soilDry, soilWet, tempMin, tempMax, lightOn, lightOff, airMin, airMax, waterTime, waterInterval
  {"Томаты",          40,      70,      18.0,    28.0,    30,     50,       60,     70,     3000,     720},
  {"Огурцы",          50,      80,      20.0,    30.0,    30,     50,       70,     80,     4000,     480},
  {"Перец",           40,      65,      18.0,    28.0,    30,     50,       60,     70,     2500,     600},
  {"Баклажаны",       45,      70,      20.0,    30.0,    30,     50,       60,     70,     3000,     720},
  {"Салат",           50,      75,      15.0,    22.0,    20,     40,       60,     75,     1500,     360},
  {"Редис",           40,      60,      10.0,    20.0,    20,     40,       50,     60,     1500,     480},
  {"Клубника",        50,      70,      15.0,    25.0,    25,     45,       60,     70,     2000,     600},
  {"Базилик",         40,      60,      18.0,    25.0,    30,     50,       60,     70,     1500,     360},
  {"Петрушка",        40,      60,      15.0,    22.0,    25,     45,       60,     70,     1500,     480},
  {"Укроп",           40,      60,      15.0,    22.0,    25,     45,       60,     70,     1500,     480},
  {"Руккола",         40,     65,      15.0,    22.0,    25,     45,       55,     70,     1500,     480}
};

// Количество растений в базе
const int plantCount = sizeof(plantDatabase) / sizeof(plantDatabase[0]);

// Текущее выбранное растение (индекс в массиве)
int currentPlantIndex = 0;

// Функция применения параметров выбранного растения
void applyPlantSettings(int plantIndex) {
  if (plantIndex < 0 || plantIndex >= plantCount) return;
  
  const Plant& p = plantDatabase[plantIndex];
  
  // Применяем параметры к глобальным настройкам системы
  SOIL_DRY = p.soilDry;
  SOIL_WET = p.soilWet;
  OPEN_TEMP = p.tempMax;
  CLOSE_TEMP = p.tempMin;
  LIGHT_ON_THRESHOLD = p.lightOn;
  LIGHT_OFF_THRESHOLD = p.lightOff;
  PUMP_RUN_TIME = p.wateringTime;
  WATERING_COOLDOWN = (unsigned long)p.wateringInterval * 60000;  // минуты → мс
  
  // Выводим информацию на дисплей
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Selected: ");
  lcd->print(p.name);
  lcd->setCursor(0, 1);
  lcd->print("Settings applied");
  delay(2000);
  
  Serial.print("Plant selected: ");
  Serial.println(p.name);
}

// Функция выбора растения (по кнопкам или с энкодера)
void selectPlant() {
  // Статическая переменная для запоминания последнего времени нажатия кнопки
  static unsigned long lastButtonPress = 0;
  
  // Если нажата кнопка "вверх" (пин 7)
  if (digitalRead(7) == LOW && millis() - lastButtonPress > 300) {
    lastButtonPress = millis();
    currentPlantIndex++;
    if (currentPlantIndex >= plantCount) currentPlantIndex = 0;
    
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Plant:");
    lcd->setCursor(0, 1);
    lcd->print(plantDatabase[currentPlantIndex].name);
    delay(500);
  }
  
  // Если нажата кнопка "вниз" (пин 9)
  if (digitalRead(9) == LOW && millis() - lastButtonPress > 300) {
    lastButtonPress = millis();
    currentPlantIndex--;
    if (currentPlantIndex < 0) currentPlantIndex = plantCount - 1;
    
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Plant:");
    lcd->setCursor(0, 1);
    lcd->print(plantDatabase[currentPlantIndex].name);
    delay(500);
  }
  
  // Если нажата кнопка "выбор" (пин 10)
  if (digitalRead(10) == LOW && millis() - lastButtonPress > 300) {
    lastButtonPress = millis();
    applyPlantSettings(currentPlantIndex);
  }
}
```

#### Интерфейс выбора растения
Для выбора растения можно использовать:
* Две кнопки — «вверх» и «вниз» для навигации по списку
* Одну кнопку — для подтверждения выбора
* Энкодер — для быстрого перебора вариантов
* Сенсорный экран — для современных версий
Отображение на дисплее:
```cpp
┌────────────────────────────────┐
│ SELECT PLANT                   │
│ > Томаты                       │
│   Огурцы                       │
│   Перец                        │
│   Баклажаны                    │
└────────────────────────────────┘
```

#### Расширенная библиотека с учётом стадий роста
Для профессиональных систем можно добавить разные стадии роста для каждого растения:
```cpp
struct GrowthStage {
  const char* stageName;   // Название стадии (рассада, вегетация, цветение, плодоношение)
  int soilDry;             // Параметры для этой стадии
  int soilWet;
  float tempMin;
  float tempMax;
  int lightOn;
  int lightOff;
  int wateringTime;
  int wateringInterval;
};

struct Plant {
  const char* name;
  GrowthStage stages[4];   // до 4 стадий роста
  int stageCount;          // количество стадий для этого растения
};
```

#### Преимущества библиотеки растений
| Преимущество | Описание |
|----------|----------|
| Простота использования | Пользователь выбирает растение из списка — всё остальное настраивается автоматически |
| Оптимальные условия | Каждое растение получает именно те параметры, которые ему нужны |
| Сменяемость культур | Можно выращивать разные растения в разное время года без перенастройки |
| Обучение | Новички узнают, какие условия нужны разным растениям |
| Масштабируемость | Базу можно расширять, добавляя новые растения через обновление прошивки |

#### Интеграция с облачными сервисами
В перспективе библиотеку растений можно синхронизировать с облачными базами данных:
```cpp
Arduino/ESP8266 → Интернет → Облачная база растений
                            ↓
                      Обновление списка растений
                      Добавление новых видов
                      Рекомендации по уходу
```
Пользователь сможет:
* Скачивать новые профили растений через Wi-Fi
* Делиться своими настройками с сообществом
* Получать уведомления о смене стадий роста

#### Пример сценария использования
1. Пользователь заходит в меню «Выбор растения»
2. Пролистывает список: Томаты → Огурцы → Перец → Салат → Клубника
3. Выбирает «Томаты»
4. Система автоматически настраивает:
* Полив: включать при 40%, выключать при 70%
* Температура: открывать форточку при 28°C, закрывать при 18°C
* Освещение: включать фитоленту при 30%, выключать при 50%
* Время полива: 3 секунды работы насоса
* Интервал полива: 12 часов (720 минут)
5. Система выводит подтверждение: «Выбраны томаты. Параметры применены.»


#### 3.2. Машинное обучение для прогнозирования
**Назначение:** анализ данных с датчиков для предсказания оптимальных условий роста.

**Что можно прогнозировать:**
* Оптимальное время полива (не по порогам, а по трендам)
* Наступление цветения и плодоношения
* Риск заболеваний (по косвенным признакам)
* Оптимальное время сбора урожая

**Платформы для реализации:**
* TensorFlow Lite Micro (на ESP32)
* Edge Impulse (облачное обучение + загрузка модели на устройство)
* Бинарная логика на Arduino (упрощённая версия)

**Пример упрощённого прогнозирования (тренды):**
```cpp
float tempTrend = 0;  // положительный = нагревается, отрицательный = охлаждается

void calculateTrend() {
  static float prevTemp = 0;
  static unsigned long lastTempTime = 0;
  
  if (millis() - lastTempTime > 3600000) {  // раз в час
    lastTempTime = millis();
    tempTrend = tempFiltered - prevTemp;
    prevTemp = tempFiltered;
    
    if (tempTrend > 2.0) {
      Serial.println("Trend: Rapid warming detected");
      // Можно приоткрыть форточку заранее
      controlWindow(tempFiltered + 3);
    }
  }
}
```

#### 3.3. Робот для сбора урожая (концепция)
**Назначение:** автоматический сбор созревших плодов.

**Компоненты:**
* Роботизированная рука (3-5 сервоприводов)
* Камера (ESP32-CAM) для распознавания плодов
* Тележка с моторами для перемещения между грядками
* Датчики для навигации (ультразвуковые, инфракрасные)

**Принцип работы:**
* Камера сканирует растения
* Нейросеть распознаёт созревшие плоды
* Роботизированная рука захватывает плод
* Аккуратно отрывает или отрезает плодоножку
* Кладиёт в контейнер

#### 3.4. Интеграция с системами умного дома
**Назначение:** управление теплицей через голосовых помощников и единую экосистему умного дома.

**Платформы:**
* Home Assistant (Open Source)
* Яндекс.Алиса / Маруся
* Google Home
* Apple HomeKit (через HomeBridge)

**Пример интеграции с Home Assistant (через MQTT):**
```cpp
#include <PubSubClient.h>

void publishState() {
  client.publish("greenhouse/temperature", String(tempFiltered).c_str());
  client.publish("greenhouse/humidity", String(humidityFiltered).c_str());
  client.publish("greenhouse/soil", String(soilMoistureFiltered).c_str());
  client.publish("greenhouse/pump", pumpState ? "ON" : "OFF");
  client.publish("greenhouse/lamp", lampState ? "ON" : "OFF");
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "greenhouse/pump/set") == 0) {
    if (payload[0] == '1') {
      digitalWrite(PUMP_PIN, HIGH);
    } else {
      digitalWrite(PUMP_PIN, LOW);
    }
  }
}
```

---

### 📋 ЭНЕРГОНЕЗАВИСИМАЯ ПАМЯТЬ (EEPROM) — ПРОРАБОТАНО

**Идея:** Даже после отключения электричества теплица запоминает все настройки, калибровки и историю поливов. При включении система восстанавливается в том же состоянии, в котором была до отключения.

#### Проблема, которую решает:

При отключении питания Arduino теряет все переменные — калибровка датчиков слетает, пороги сбрасываются, счётчик поливов обнуляется. Пользователю приходится заново настраивать теплицу. EEPROM (Electrically Erasable Programmable Read-Only Memory) — это энергонезависимая память, встроенная в микроконтроллер, которая сохраняет данные даже после выключения питания.

#### Технические характеристики EEPROM на Arduino Uno:

| Параметр | Значение |
|----------|----------|
| Объём памяти | 1024 байта (1 КБ) |
| Циклов перезаписи | ~100 000 |
| Время хранения данных | ~100 лет |
| Скорость чтения/записи | ~50 мкс / ~3 мс |

#### Что сохранять в EEPROM:

| Адрес | Данные | Тип | Размер (байт) | Когда сохранять |
|-------|--------|-----|---------------|-----------------|
| 0 | SOIL_DRY_CALIB (сухая земля) | int | 2 | После калибровки |
| 2 | SOIL_WET_CALIB (мокрая земля) | int | 2 | После калибровки |
| 4 | DARK_VALUE (темнота) | int | 2 | После калибровки |
| 6 | BRIGHT_VALUE (свет) | int | 2 | После калибровки |
| 8 | SOIL_DRY (порог полива) | int | 2 | После изменения |
| 10 | SOIL_WET (порог остановки) | int | 2 | После изменения |
| 12 | OPEN_TEMP | float | 4 | После изменения |
| 16 | CLOSE_TEMP | float | 4 | После изменения |
| 20 | LIGHT_ON_THRESHOLD | int | 2 | После изменения |
| 22 | LIGHT_OFF_THRESHOLD | int | 2 | После изменения |
| 24 | PUMP_RUN_TIME | unsigned long | 4 | После изменения |
| 28 | WATERING_COOLDOWN | unsigned long | 4 | После изменения |
| 32 | currentPlantIndex | int | 2 | При выборе растения |
| 34 | lastWatering (время последнего полива) | unsigned long | 4 | После каждого полива |
| 38 | plantAge (возраст растения, дней) | int | 2 | Раз в день |
| 40 | Режим работы (авто/ручной) | bool | 1 | При переключении режима |
| 41-50 | Резерв | - | 10 | - |
| 60-200 | История поливов (20 записей) | struct | 140 | После каждого полива |
| 200-1023 | Свободно | - | 823 | - |

#### Структура для истории поливов:
```cpp
// Структура одной записи о поливе (7 байт)
struct WateringRecord {
  unsigned long timestamp;  // время полива (4 байта)
  int soilBefore;           // влажность до полива (2 байта)
  int soilAfter;            // влажность после полива (2 байта)
  float tempAtTime;         // температура во время полива (4 байта) — потребуется 4 байта, итого 12 байт
};

// Оптимизированная структура (для экономии места)
struct WateringRecordCompact {
  unsigned long timestamp;  // 4 байта
  byte soilBefore;          // 1 байт (0-100%)
  byte soilAfter;           // 1 байт (0-100%)
  byte tempAtTime;          // 1 байт (0-50°C с точностью до 0.5°C)
};  // Итого 7 байт на запись

// Максимальное количество записей: ~140 байт / 7 байт = 20 записей
const int MAX_WATERING_RECORDS = 20;
WateringRecordCompact wateringHistory[MAX_WATERING_RECORDS];
int historyCount = 0;
```

#### Полный код для работы с EEPROM:
```cpp
#include <EEPROM.h>

// =====================================================================
// ИНИЦИАЛИЗАЦИЯ EEPROM (ПРИ ПЕРВОМ ЗАПУСКЕ)
// =====================================================================

void initEEPROM() {
  // Проверяем, есть ли признак "первого запуска"
  int magicNumber;
  EEPROM.get(50, magicNumber);
  
  if (magicNumber != 0xAA55) {  // Если признака нет — первый запуск
    Serial.println("First boot: Initializing EEPROM with default settings");
    
    // Сохраняем значения по умолчанию
    EEPROM.put(0, SOIL_DRY_CALIB);     // 554
    EEPROM.put(2, SOIL_WET_CALIB);     // 233
    EEPROM.put(4, DARK_VALUE);         // 0
    EEPROM.put(6, BRIGHT_VALUE);       // 675
    EEPROM.put(8, SOIL_DRY);           // 40
    EEPROM.put(10, SOIL_WET);          // 70
    EEPROM.put(12, OPEN_TEMP);         // 28.0
    EEPROM.put(16, CLOSE_TEMP);        // 26.0
    EEPROM.put(20, LIGHT_ON_THRESHOLD); // 30
    EEPROM.put(22, LIGHT_OFF_THRESHOLD);// 50
    EEPROM.put(24, PUMP_RUN_TIME);     // 2000
    EEPROM.put(28, WATERING_COOLDOWN); // 600000
    EEPROM.put(32, 0);                 // currentPlantIndex = 0
    EEPROM.put(34, 0UL);               // lastWatering = 0
    EEPROM.put(38, 0);                 // plantAge = 0
    EEPROM.put(40, true);              // autoMode = true
    
    // Устанавливаем magic number (признак инициализации)
    int magic = 0xAA55;
    EEPROM.put(50, magic);
    
    Serial.println("EEPROM initialized with default settings");
  } else {
    Serial.println("EEPROM already initialized, loading settings...");
  }
}

// =====================================================================
// ЗАГРУЗКА ВСЕХ НАСТРОЕК ИЗ EEPROM
// =====================================================================

void loadSettingsFromEEPROM() {
  // Загружаем калибровки
  EEPROM.get(0, SOIL_DRY_CALIB);
  EEPROM.get(2, SOIL_WET_CALIB);
  EEPROM.get(4, DARK_VALUE);
  EEPROM.get(6, BRIGHT_VALUE);
  
  // Загружаем пороги
  EEPROM.get(8, SOIL_DRY);
  EEPROM.get(10, SOIL_WET);
  EEPROM.get(12, OPEN_TEMP);
  EEPROM.get(16, CLOSE_TEMP);
  EEPROM.get(20, LIGHT_ON_THRESHOLD);
  EEPROM.get(22, LIGHT_OFF_THRESHOLD);
  
  // Загружаем временные параметры
  EEPROM.get(24, PUMP_RUN_TIME);
  EEPROM.get(28, WATERING_COOLDOWN);
  
  // Загружаем состояние системы
  EEPROM.get(32, currentPlantIndex);
  EEPROM.get(34, lastWatering);
  EEPROM.get(38, plantAge);
  EEPROM.get(40, autoMode);
  
  Serial.println("Settings loaded from EEPROM:");
  Serial.print("  SOIL_DRY_CALIB: "); Serial.println(SOIL_DRY_CALIB);
  Serial.print("  SOIL_WET_CALIB: "); Serial.println(SOIL_WET_CALIB);
  Serial.print("  SOIL_DRY: "); Serial.println(SOIL_DRY);
  Serial.print("  SOIL_WET: "); Serial.println(SOIL_WET);
  Serial.print("  OPEN_TEMP: "); Serial.println(OPEN_TEMP);
  Serial.print("  currentPlantIndex: "); Serial.println(currentPlantIndex);
  Serial.print("  lastWatering: "); Serial.println(lastWatering);
  Serial.print("  plantAge: "); Serial.println(plantAge);
  Serial.print("  autoMode: "); Serial.println(autoMode ? "AUTO" : "MANUAL");
}

// =====================================================================
// СОХРАНЕНИЕ ВСЕХ НАСТРОЕК В EEPROM
// =====================================================================

void saveSettingsToEEPROM() {
  EEPROM.put(8, SOIL_DRY);
  EEPROM.put(10, SOIL_WET);
  EEPROM.put(12, OPEN_TEMP);
  EEPROM.put(16, CLOSE_TEMP);
  EEPROM.put(20, LIGHT_ON_THRESHOLD);
  EEPROM.put(22, LIGHT_OFF_THRESHOLD);
  EEPROM.put(24, PUMP_RUN_TIME);
  EEPROM.put(28, WATERING_COOLDOWN);
  EEPROM.put(32, currentPlantIndex);
  EEPROM.put(34, lastWatering);
  EEPROM.put(38, plantAge);
  EEPROM.put(40, autoMode);
  
  Serial.println("Settings saved to EEPROM");
}

// =====================================================================
// СОХРАНЕНИЕ КАЛИБРОВОК (только при изменении)
// =====================================================================

void saveCalibrationToEEPROM() {
  static int lastDryCalib = 0, lastWetCalib = 0;
  static int lastDark = 0, lastBright = 0;
  
  // Сохраняем только если изменились
  if (lastDryCalib != SOIL_DRY_CALIB) {
    EEPROM.put(0, SOIL_DRY_CALIB);
    lastDryCalib = SOIL_DRY_CALIB;
    Serial.println("SOIL_DRY_CALIB saved to EEPROM");
  }
  if (lastWetCalib != SOIL_WET_CALIB) {
    EEPROM.put(2, SOIL_WET_CALIB);
    lastWetCalib = SOIL_WET_CALIB;
    Serial.println("SOIL_WET_CALIB saved to EEPROM");
  }
  if (lastDark != DARK_VALUE) {
    EEPROM.put(4, DARK_VALUE);
    lastDark = DARK_VALUE;
    Serial.println("DARK_VALUE saved to EEPROM");
  }
  if (lastBright != BRIGHT_VALUE) {
    EEPROM.put(6, BRIGHT_VALUE);
    lastBright = BRIGHT_VALUE;
    Serial.println("BRIGHT_VALUE saved to EEPROM");
  }
}

// =====================================================================
// СОХРАНЕНИЕ ИСТОРИИ ПОЛИВОВ (ЦИКЛИЧЕСКИЙ БУФЕР)
// =====================================================================

void saveWateringToHistory(int soilBefore, int soilAfter, float temp) {
  // Сдвигаем старые записи
  for (int i = MAX_WATERING_RECORDS - 1; i > 0; i--) {
    EEPROM.get(60 + (i-1) * sizeof(WateringRecordCompact), wateringHistory[i]);
  }
  
  // Сохраняем новую запись
  wateringHistory[0].timestamp = millis();
  wateringHistory[0].soilBefore = constrain(soilBefore, 0, 100);
  wateringHistory[0].soilAfter = constrain(soilAfter, 0, 100);
  wateringHistory[0].tempAtTime = (byte)(temp * 2);  // Сохраняем с точностью 0.5°C
  
  // Записываем в EEPROM
  EEPROM.put(60, wateringHistory[0]);
  
  if (historyCount < MAX_WATERING_RECORDS) historyCount++;
  
  Serial.println("Watering record saved to EEPROM");
}

// =====================================================================
// ЗАГРУЗКА ИСТОРИИ ПОЛИВОВ
// =====================================================================

void loadWateringHistory() {
  historyCount = 0;
  for (int i = 0; i < MAX_WATERING_RECORDS; i++) {
    WateringRecordCompact temp;
    EEPROM.get(60 + i * sizeof(WateringRecordCompact), temp);
    if (temp.timestamp != 0) {  // Если запись не пустая
      wateringHistory[i] = temp;
      historyCount++;
    }
  }
  Serial.print("Loaded "); Serial.print(historyCount); Serial.println(" watering records");
}

// =====================================================================
// ВЫВОД СТАТИСТИКИ НА ДИСПЛЕЙ
// =====================================================================

void showStatistics() {
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Watering stats:");
  lcd->setCursor(0, 1);
  lcd->print("Total: ");
  lcd->print(historyCount);
  
  delay(2000);
  
  if (historyCount > 0) {
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Last watering:");
    lcd->setCursor(0, 1);
    
    float lastTemp = wateringHistory[0].tempAtTime / 2.0;
    lcd->print("Temp: ");
    lcd->print(lastTemp, 1);
    lcd->print("C");
    
    delay(2000);
    
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Soil before:");
    lcd->setCursor(0, 1);
    lcd->print(wateringHistory[0].soilBefore);
    lcd->print("% -> ");
    lcd->print(wateringHistory[0].soilAfter);
    lcd->print("%");
    
    delay(2000);
  }
}
```

#### Автоматическое восстановление после сбоя питания:
```cpp
void setup() {
  // ... инициализация пинов ...
  
  // Инициализируем EEPROM (при первом запуске)
  initEEPROM();
  
  // Загружаем все настройки
  loadSettingsFromEEPROM();
  
  // Загружаем историю поливов
  loadWateringHistory();
  
  // Восстанавливаем состояние системы
  if (autoMode) {
    Serial.println("System restored to AUTO mode");
  } else {
    Serial.println("System restored to MANUAL mode");
  }
  
  // ... остальной код setup ...
}
```

### 🔮 УМНЫЙ АЛГОРИТМ САМООБУЧЕНИЯ (AI-агроном) — В ПЛАНАХ

**Идея:** Система не просто выполняет жёсткие пороги, а **анализирует историю роста растений и оптимизирует параметры**.

#### Проблема, которую решает:

Существующие умные теплицы работают по статическим алгоритмам: задал порог полива 40% — и всегда поливай при 40%. Но разные сорта растений, разный возраст, разные условия требуют разного подхода. AI-агроном адаптируется под конкретное растение и конкретные условия.

#### Как это работает:

| Что анализирует | Как использует | Результат |
|----------------|----------------|-----------|
| Скорость роста растений | Увеличивает/уменьшает пороги полива | Растения растут быстрее |
| Реакция на полив | Корректирует время работы насоса | Экономия воды |
| Суточные колебания температуры | Прогнозирует открытие форточки | Стабильный микроклимат |
| История урожайности | Выбирает лучшие параметры для следующего сезона | Повышение урожайности |

#### Три уровня самообучения:

**Уровень 1: Адаптивные пороги (базовый)**
```cpp
// Адаптивный порог полива (растёт вместе с растением)
int adaptiveSoilDry = SOIL_DRY_BASE + (plantAge / 10);

if (plantAge < 30) {
  adaptiveSoilDry = SOIL_DRY_BASE - 10;  // молодым растениям нужно больше воды
} else if (plantAge > 60) {
  adaptiveSoilDry = SOIL_DRY_BASE + 5;   // взрослым — меньше
}

if (soilMoistureFiltered < adaptiveSoilDry && !pumpState) {
  // Включаем полив с адаптивным порогом
}
```

**Уровень 2: Обучение на истории (средний):**
```cpp
// Анализируем эффективность полива
struct WateringRecord {
  unsigned long timestamp;
  int soilBefore;
  int soilAfter;
  float tempAtTime;
  int effectiveness;  // на сколько процентов поднялась влажность
};

WateringRecord history[100];
int recordCount = 0;

void analyzeAndOptimize() {
  int avgEffectiveness = 0;
  for (int i = 0; i < recordCount; i++) {
    avgEffectiveness += history[i].effectiveness;
  }
  avgEffectiveness /= recordCount;
  
  // Если полив слишком короткий (эффективность низкая) — увеличиваем время
  if (avgEffectiveness < 20 && PUMP_RUN_TIME < 5000) {
    PUMP_RUN_TIME += 500;
    Serial.println("AI: Increased watering time due to low efficiency");
  }
  // Если полив слишком длинный (эффективность высокая) — уменьшаем
  else if (avgEffectiveness > 60 && PUMP_RUN_TIME > 1000) {
    PUMP_RUN_TIME -= 500;
    Serial.println("AI: Decreased watering time due to high efficiency");
  }
}
```

**Уровень 3: Прогнозирование на основе трендов (продвинутый):**
```cpp
// Прогнозируем, когда почва высохнет
float soilDryRate = 0;  // скорость высыхания (% в час)

void calculateDryRate() {
  static float lastSoil = 0;
  static unsigned long lastTime = 0;
  unsigned long now = millis();
  
  if (lastTime != 0 && now - lastTime > 3600000) {  // раз в час
    soilDryRate = (lastSoil - soilMoistureFiltered) / ((now - lastTime) / 3600000.0);
    lastSoil = soilMoistureFiltered;
    lastTime = now;
    
    // Прогнозируем, через сколько часов потребуется полив
    if (soilDryRate > 0) {
      int hoursUntilDry = (soilMoistureFiltered - SOIL_DRY) / soilDryRate;
      Serial.print("AI: Soil will be dry in ");
      Serial.print(hoursUntilDry);
      Serial.println(" hours");
      
      // Если скоро будет темно, можно полить сейчас
      if (hoursUntilDry < 2 && lightLevelFiltered > 50) {
        // Запланировать полив
      }
    }
  }
  lastSoil = soilMoistureFiltered;
  lastTime = now;
}
```

**Сравнение с обычной автоматикой:**
| Параметр | Обычная автоматика | AI-агроном |
|----------------|----------------|-----------|
| Порог полива | Фиксированный (40%) | Адаптивный (30-50%) |
| Время полива | Фиксированное (2 сек) | Адаптивное (1-5 сек) |
| Учёт возраста растения | ❌ Нет | ✅ Да |
| Учёт сезонных колебаний | ❌ Нет | ✅ Да |
| Самообучение | ❌ Нет | ✅ Да |
| Прогнозирование | ❌ Нет | ✅ Да |

**Архитектура AI-агронома:**
```cpp
┌─────────────────────────────────────────────────────────────┐
│                      AI-АГРОНОМ                             │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐   │
│  │  СБОР ДАННЫХ │ →  │   АНАЛИЗ     │ →  │  ОПТИМИЗАЦИЯ │   │
│  │  - датчики   │    │  - тренды    │    │  - пороги    │   │
│  │  - история   │    │  - паттерны  │    │  - тайминги  │   │
│  └──────────────┘    └──────────────┘    └──────────────┘   │
│         ↑                    ↓                    ↓         │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐   │
│  │  ОБУЧЕНИЕ    │ ←  │   ПАМЯТЬ     │ ←  │  ПРИМЕНЕНИЕ  │   │
│  │  - нейросеть │    │  - EEPROM    │    │  - исполнит. │   │
│  │  - веса      │    │  - SD карта  │    │  - устройства│   │
│  └──────────────┘    └──────────────┘    └──────────────┘   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

**Хранение данных (EEPROM + SD):**
```cpp
// Структура для хранения обученных параметров
struct TrainedParams {
  int optimalSoilDry;
  int optimalSoilWet;
  int optimalPumpTime;
  float optimalTempMin;
  float optimalTempMax;
  int optimalLightOn;
  int optimalLightOff;
  int growthRate;           // скорость роста (мм/день)
  int healthScore;          // оценка здоровья растения (0-100)
};

TrainedParams loadTrainedParams(int plantId) {
  TrainedParams params;
  // Загрузка из EEPROM или SD карты
  EEPROM.get(plantId * sizeof(TrainedParams), params);
  return params;
}
```

**Интеграция с библиотекой растений:**
```cpp
// AI адаптирует параметры под конкретное растение
void applyAISettings(int plantIndex) {
  // 1. Загружаем базовые параметры из библиотеки растений
  const Plant& base = plantDatabase[plantIndex];
  
  // 2. Загружаем обученные параметры для этого растения
  TrainedParams trained = loadTrainedParams(plantIndex);
  
  // 3. Адаптируем параметры с учётом обучения
  SOIL_DRY = (base.soilDry + trained.optimalSoilDry) / 2;
  SOIL_WET = (base.soilWet + trained.optimalSoilWet) / 2;
  PUMP_RUN_TIME = (base.wateringTime + trained.optimalPumpTime) / 2;
  
  // 4. Если растение болеет (низкий healthScore) — применяем щадящий режим
  if (trained.healthScore < 50) {
    SOIL_DRY = max(SOIL_DRY, 50);  // не даём пересыхать
    Serial.println("AI: Plant health low, applying gentle mode");
  }
}
```
---

### 📋 СИСТЕМА ОПОВЕЩЕНИЙ НА СЛУЧАЙ ЧРЕЗВЫЧАЙНЫХ СИТУАЦИЙ — ПРОРАБОТАНО

**Идея:** Теплица сама сообщает о проблемах до того, как они уничтожат урожай.

#### Проблема, которую решает:

Умная теплица должна быть не только автоматической, но и **информировать пользователя** о критических ситуациях. Если закончилась вода или сломался датчик, растения могут погибнуть за несколько часов. Система оповещений заранее предупредит пользователя.

#### Типы оповещений:

| Ситуация | Тип оповещения | Приоритет | Канал связи |
|----------|----------------|-----------|-------------|
| Закончилась вода в баке | "Налейте воду! Насос не включится" | 🔴 КРИТИЧНЫЙ | Зуммер + светодиод + экран + Telegram |
| Слишком жарко (>35°C) | "Аварийная жара! Откройте форточку" | 🔴 КРИТИЧНЫЙ | Зуммер + мигание экрана |
| Слишком холодно (<10°C) | "Аварийный холод! Включите обогрев" | 🔴 КРИТИЧНЫЙ | Зуммер + экран + Telegram |
| Насос не качает (нет протока) | "Проверьте насос! Воздушная пробка или засор" | 🟠 ВЫСОКИЙ | Зуммер + светодиод |
| Зависание программы | "Watchdog перезагрузил систему в X:XX" | 🟡 СРЕДНИЙ | Запись в лог |
| Датчик вышел из строя | "Датчик X не отвечает! Проверьте подключение" | 🟠 ВЫСОКИЙ | Экран + зуммер |
| Фитолента перегрелась | "Фитолента >60°C! Выключаю на 10 минут" | 🟠 ВЫСОКИЙ | Экран + Telegram |
| Низкий уровень воды (<20%) | "Вода заканчивается! Осталось ~5 литров" | 🟡 СРЕДНИЙ | Экран |
| Полив пропущен (почва сухая >6 часов) | "Растения не поливались 6 часов! Проверьте систему" | 🟡 СРЕДНИЙ | Экран + Telegram |
| Сервопривод не отвечает | "Форточка не открывается! Проверьте серво" | 🟡 СРЕДНИЙ | Экран |

#### Аппаратная реализация:

**Компоненты:**
- Зуммер (пин 7) — звуковое оповещение
- Встроенный светодиод (пин 13) — визуальное оповещение
- Режим мигания экрана — для критических ситуаций

**Подключение зуммера:**
```cpp
Зуммер (+) → PIN 7 Arduino
Зуммер (-) → GND Arduino
```

#### Код реализации:

```cpp
// =====================================================================
// СИСТЕМА ОПОВЕЩЕНИЙ
// =====================================================================

#define BUZZER_PIN 7
#define STATUS_LED 13

// Типы оповещений
enum AlertPriority {
  PRIORITY_CRITICAL,   // Красный — срочно!
  PRIORITY_HIGH,       // Оранжевый — важно
  PRIORITY_MEDIUM,     // Жёлтый — внимание
  PRIORITY_INFO        // Зелёный — информация
};

// Структура оповещения
struct Alert {
  const char* message;
  AlertPriority priority;
  unsigned long lastShown;
  int repeatCount;
};

// Функция звукового оповещения
void playAlert(AlertPriority priority) {
  switch(priority) {
    case PRIORITY_CRITICAL:
      // Три длинных сигнала
      for (int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, 1000, 500);
        delay(600);
        noTone(BUZZER_PIN);
        delay(400);
      }
      break;
      
    case PRIORITY_HIGH:
      // Два коротких сигнала
      for (int i = 0; i < 2; i++) {
        tone(BUZZER_PIN, 2000, 200);
        delay(250);
        noTone(BUZZER_PIN);
        delay(250);
      }
      break;
      
    case PRIORITY_MEDIUM:
      // Один короткий сигнал
      tone(BUZZER_PIN, 3000, 100);
      delay(150);
      noTone(BUZZER_PIN);
      break;
      
    default:
      // Нет звука для информации
      break;
  }
}

// Функция светового оповещения (мигание)
void flashLight(int times, int duration, AlertPriority priority) {
  for (int i = 0; i < times; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(duration);
    digitalWrite(STATUS_LED, LOW);
    delay(duration);
  }
  
  // Для критических ситуаций — частое мигание
  if (priority == PRIORITY_CRITICAL) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  }
}

// Вывод сообщения на дисплей
void showAlertOnDisplay(const char* message, AlertPriority priority) {
  if (currentDisplayMode != DISPLAY_NORMAL) return;
  
  lcd->clear();
  lcd->setCursor(0, 0);
  
  // Выбор иконки в зависимости от приоритета
  switch(priority) {
    case PRIORITY_CRITICAL:
      lcd->print("!!! CRITICAL !!!");
      break;
    case PRIORITY_HIGH:
      lcd->print("!! WARNING !!");
      break;
    case PRIORITY_MEDIUM:
      lcd->print("! ATTENTION !");
      break;
    default:
      lcd->print("INFO");
      break;
  }
  
  lcd->setCursor(0, 1);
  // Обрезаем сообщение до 16 символов
  char shortMsg[17];
  strncpy(shortMsg, message, 16);
  shortMsg[16] = '\0';
  lcd->print(shortMsg);
}

// Основная функция проверки и отправки оповещений
void checkAlerts() {
  static unsigned long lastNoWaterAlert = 0;
  static unsigned long lastHighTempAlert = 0;
  static unsigned long lastLowTempAlert = 0;
  static unsigned long lastSensorErrorAlert = 0;
  unsigned long now = millis();
  
  // ===== 1. ПРОВЕРКА УРОВНЯ ВОДЫ (КРИТИЧНЫЙ) =====
  if (!waterPresent && !pumpState) {
    if (now - lastNoWaterAlert > 30000) {  // Не чаще раза в 30 секунд
      lastNoWaterAlert = now;
      
      const char* msg = "NO WATER!";
      showAlertOnDisplay(msg, PRIORITY_CRITICAL);
      playAlert(PRIORITY_CRITICAL);
      flashLight(3, 200, PRIORITY_CRITICAL);
      
      Serial.println("ALERT: CRITICAL - Water tank is EMPTY!");
      
      // Дополнительно: блокируем насос, чтобы не сгорел
      if (pumpState) {
        digitalWrite(PUMP_PIN, LOW);
        pumpState = false;
        Serial.println("EMERGENCY: Pump blocked due to no water");
      }
    }
  }
  
  // ===== 2. ПРОВЕРКА ТЕМПЕРАТУРЫ (КРИТИЧНЫЙ) =====
  if (tempFiltered > 35.0) {
    if (now - lastHighTempAlert > 60000) {  // Не чаще раза в минуту
      lastHighTempAlert = now;
      
      char msg[20];
      snprintf(msg, 20, "TEMP: %.1fC!", tempFiltered);
      showAlertOnDisplay(msg, PRIORITY_CRITICAL);
      playAlert(PRIORITY_CRITICAL);
      flashLight(3, 300, PRIORITY_CRITICAL);
      
      Serial.print("ALERT: CRITICAL - High temperature ");
      Serial.print(tempFiltered);
      Serial.println("C!");
      
      // Аварийное открытие форточки
      if (!windowIsOpen) {
        ventServo.write(SERVO_OPEN);
        windowIsOpen = true;
        Serial.println("EMERGENCY: Window opened due to high temperature");
      }
    }
  }
  
  // ===== 3. ПРОВЕРКА НИЗКОЙ ТЕМПЕРАТУРЫ (ВЫСОКИЙ) =====
  else if (tempFiltered < 10.0) {
    if (now - lastLowTempAlert > 60000) {
      lastLowTempAlert = now;
      
      char msg[20];
      snprintf(msg, 20, "COLD: %.1fC!", tempFiltered);
      showAlertOnDisplay(msg, PRIORITY_HIGH);
      playAlert(PRIORITY_HIGH);
      
      Serial.print("ALERT: High - Low temperature ");
      Serial.print(tempFiltered);
      Serial.println("C!");
      
      // Аварийное закрытие форточки
      if (windowIsOpen) {
        ventServo.write(SERVO_CLOSED);
        windowIsOpen = false;
        Serial.println("EMERGENCY: Window closed due to low temperature");
      }
    }
  }
  
  // ===== 4. ПРОВЕРКА ОШИБКИ ДАТЧИКА DHT11 =====
  if (dhtErrorCount > 5) {
    if (now - lastSensorErrorAlert > 120000) {  // Не чаще раза в 2 минуты
      lastSensorErrorAlert = now;
      
      showAlertOnDisplay("DHT11 ERROR!", PRIORITY_HIGH);
      playAlert(PRIORITY_HIGH);
      flashLight(2, 150, PRIORITY_HIGH);
      
      Serial.println("ALERT: High - DHT11 sensor not responding!");
    }
  }
  
  // ===== 5. ПРОВЕРКА ДАТЧИКА ВЛАЖНОСТИ ПОЧВЫ (вышел из строя) =====
  static int lastSoilRaw = 0;
  if (abs(lastSoilRaw - soilMoistureFiltered) < 2 && 
      lastSoilRaw != 0 && 
      millis() > 600000) {  // Через 10 минут после старта
    if (now - lastSensorErrorAlert > 300000) {  // Не чаще раза в 5 минут
      lastSensorErrorAlert = now;
      showAlertOnDisplay("SOIL SENSOR STUCK!", PRIORITY_HIGH);
      Serial.println("ALERT: High - Soil moisture sensor may be stuck!");
    }
  }
  lastSoilRaw = soilMoistureFiltered;
  
  // ===== 6. ПРОВЕРКА ФИТОЛЕНТЫ (перегрев) =====
  static unsigned long lampOffTime = 0;
  static bool lampOverheated = false;
  
  if (lampState && millis() - lastLampTurnOnTime > 3600000) {  // Лампа работает час
    if (!lampOverheated) {
      lampOverheated = true;
      showAlertOnDisplay("GROW LIGHT HOT!", PRIORITY_MEDIUM);
      Serial.println("ALERT: Medium - Grow light has been on for over 1 hour");
    }
  } else if (!lampState) {
    lampOverheated = false;
  }
  
  // ===== 7. ПРОВЕРКА НАСОСА (долгая работа) =====
  if (pumpState && (millis() - pumpStartTime > 30000)) {
    if (now - lastSensorErrorAlert > 60000) {
      lastSensorErrorAlert = now;
      showAlertOnDisplay("PUMP RUNNING TOO LONG!", PRIORITY_HIGH);
      playAlert(PRIORITY_HIGH);
      Serial.println("ALERT: High - Pump running more than 30 seconds!");
      
      // Аварийное выключение насоса
      digitalWrite(PUMP_PIN, LOW);
      pumpState = false;
      Serial.println("EMERGENCY: Pump forced OFF");
    }
  }
}

// Функция для отправки в Telegram (в перспективе)
void sendTelegramAlert(const char* message) {
  // Для ESP8266 версии
  #ifdef ESP8266
    String url = "/bot" + String(TELEGRAM_BOT_TOKEN) + "/sendMessage";
    url += "?chat_id=" + String(TELEGRAM_CHAT_ID);
    url += "&text=" + String(message);
    // Отправка HTTP запроса
  #endif
  Serial.print("TELEGRAM: ");
  Serial.println(message);
}
```

#### Логирование всех оповещений:
``` cpp
// Запись оповещений в лог (на microSD)
void logAlert(const char* message, AlertPriority priority) {
  #ifdef SD_AVAILABLE
    File logFile = SD.open("alerts.csv", FILE_WRITE);
    if (logFile) {
      logFile.print(millis() / 1000);  // время в секундах
      logFile.print(",");
      logFile.print(priority);
      logFile.print(",");
      logFile.print(message);
      logFile.println();
      logFile.close();
    }
  #endif
}
```

#### Интерфейс на дисплее при оповещении:
``` cpp
При нормальном режиме:
┌────────────────────────────────┐
│ TEMPERATURE: 24.5 C            │
│ WINDOW: CLOSED                 │
└────────────────────────────────┘

При критической ситуации:
┌────────────────────────────────┐
│ !!! CRITICAL !!!               │
│ NO WATER!                      │
└────────────────────────────────┘
(экран мигает, зуммер пищит)

При предупреждении:
┌────────────────────────────────┐
│ !! WARNING !!                  │
│ TEMP: 36.2C!                   │
└────────────────────────────────┘
(экран не мигает, но зуммер пищит)
```

---

