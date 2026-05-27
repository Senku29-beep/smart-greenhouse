# 🌱 Умная теплица нового поколения
## Автоматизированная система управления микроклиматом для выращивания растений

---

## 📌 О проекте

**Умная теплица** — это полностью автоматизированная система, которая самостоятельно поддерживает оптимальные условия для роста растений. Проект разработан в рамках конкурса в Кванториуме и представляет собой действующий прототип, способный:

- Поливать растения при сухой почве
- Включать подсветку при недостатке естественного света
- Открывать и закрывать форточку для вентиляции
- Отображать все параметры на LCD дисплее
- Самостоятельно восстанавливаться после сбоев

Система построена на платформе **Arduino Uno** с использованием датчиков и исполнительных устройств. Главная особенность — **отказоустойчивость**: при зависании программы аппаратный Watchdog перезагружает контроллер, а дисплей автоматически восстанавливается при сбоях I2C.

---

## 🎯 Актуальность

Сельское хозяйство сталкивается с глобальными вызовами:

- **Рост населения планеты** — к 2050 году потребуется на 70% больше продовольствия
- **Нехватка рабочих рук** — урбанизация, старение кадров, молодёжь не идёт в сельское хозяйство
- **Изменение климата** — засухи, наводнения, непредсказуable заморозки
- **Экологические проблемы** — чрезмерное использование удобрений и пестицидов загрязняет почвы и воды

Традиционное земледелие становится всё более рискованным и ресурсозатратным. **Умные теплицы** с контролируемой средой — это перспективное решение, позволяющее выращивать продукты круглый год без участия человека. Однако существующие системы работают по жёстким алгоритмам и не способны адаптироваться к изменениям. Это создаёт запрос на **новое поколение самообучающихся теплиц**.

---

### Сравнение с аналогами

| Характеристика | Мой проект | Xiaomi Flower Care | Priva (профессиональная) | OpenGrow (open-source) |
|----------------|------------|--------------------|--------------------------|------------------------|
| Цена | 4000-6000 ₽ | 2000 ₽ | от 500000 ₽ | 5000-10000 ₽ |
| Автополив | ✅ Есть | ❌ Нет | ✅ Есть | ✅ Есть |
| Автоосвещение | ✅ Есть | ❌ Нет | ✅ Есть | ⚠️ Частично |
| Автовентиляция | ✅ Есть | ❌ Нет | ✅ Есть | ❌ Нет |
| Датчик воды | ✅ Есть | ❌ Нет | ✅ Есть | ❌ Нет |
| Дисплей | ✅ Есть | ❌ Нет | ✅ Есть | ⚠️ Опционально |
| Watchdog (автоперезагрузка) | ✅ Есть | ❌ Нет | ✅ Есть | ❌ Нет |
| Удалённое управление | ⚠️ В планах | ✅ Есть (приложение) | ✅ Есть | ⚠️ Частично |
| Библиотека растений | ⚠️ В планах | ❌ Нет | ✅ Есть | ❌ Нет |
| Цена готового решения | Низкая | Низкая | Очень высокая | Средняя |

**Вывод:** Проект занимает золотую середину между дешёвыми китайскими гаджетами (нет автоматизации) и дорогими профессиональными системами (высокая цена). При этом по ключевым функциям (полив, свет, вентиляция) проект не уступает профессиональным решениям.

---

### 8. 🔮 УМНЫЙ АЛГОРИТМ САМООБУЧЕНИЯ (AI-агроном) — В ПЛАНАХ

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

### 9. 📋 СИСТЕМА ОПОВЕЩЕНИЙ НА СЛУЧАЙ ЧРЕЗВЫЧАЙНЫХ СИТУАЦИЙ — ПРОРАБОТАНО

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

### 10. 📋 ЭНЕРГОНЕЗАВИСИМАЯ ПАМЯТЬ (EEPROM) — ПРОРАБОТАНО

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
---

### 💻 Программное обеспечение
#### Используемые библиотеки
```cpp
#include <Wire.h>               // I2C шина для связи с дисплеем
#include <LiquidCrystal_I2C.h>  // LCD дисплей 16x2
#include <DHT.h>                // Датчик температуры и влажности DHT11
#include <Servo.h>              // Управление сервоприводом
#include <avr/wdt.h>            // Watchdog для автоперезагрузки
```

#### Основные настройки (раздел 7)
```cpp
// Временные параметры
const unsigned long PUMP_RUN_TIME = 2000;        // 2 секунды работы насоса
const unsigned long WATERING_COOLDOWN = 600000;  // 10 минут между поливами
const unsigned long MAX_PUMP_RUN_TIME = 30000;   // 30 секунд аварийный таймер

// Пороги полива
const int SOIL_DRY = 40;   // Включаем полив при влажности < 40%
const int SOIL_WET = 70;   // Выключаем полив при влажности > 70%

// Пороги форточки
const float OPEN_TEMP = 28.0;   // Открыть форточку при температуре > 28°C
const float CLOSE_TEMP = 26.0;  // Закрыть форточку при температуре < 26°C

// Пороги фитоленты (датчик инвертирован!)
const int LIGHT_ON_THRESHOLD = 30;   // Включить фитоленту при освещённости < 30%
const int LIGHT_OFF_THRESHOLD = 50;  // Выключить фитоленту при освещённости > 50%

// Тип реле
const bool RELAY_INVERTED = false;  // HIGH = ВКЛ, LOW = ВЫКЛ

// Калибровка датчика влажности почвы
const int SOIL_DRY_CALIB = 554;  // Значение в сухой земле (0% влажности)
const int SOIL_WET_CALIB = 233;  // Значение в мокрой земле (100% влажности)

// Калибровка датчика освещённости
const int DARK_VALUE = 0;       // Значение в полной темноте
const int BRIGHT_VALUE = 675;   // Значение при ярком свете

// Watchdog
wdt_enable(WDTO_4S);  // Включаем watchdog с таймаутом 4 секунды
```

---

### 🔮 ВЕБ-ИНТЕРФЕЙС С 3D-ВИЗУАЛИЗАЦИЕЙ ТЕПЛИЦЫ — В ПЛАНАХ

**Идея:** Управление теплицей через красивый веб-интерфейс с 3D-моделью теплицы в реальном времени.

#### Что показывает интерфейс:

| Элемент | Отображение |
|---------|-------------|
| Температура | Термометр + цветовая индикация (зелёный/жёлтый/красный) |
| Влажность воздуха | Капля воды + процент |
| Влажность почвы | Шкала с цветом от красного (сухо) до синего (мокро) |
| Уровень воды в баке | Анимированная шкала |
| Состояние насоса | Анимация качающейся воды |
| Состояние фитоленты | Светящаяся лента |
| Состояние форточки | Открывающаяся створка |
| Прогноз погоды на 3 дня | Иконки + температура |

#### Технологии:
- **Three.js** — 3D-графика в браузере
- **WebSockets** — обновление данных в реальном времени
- **Chart.js** — графики истории
- **Bootstrap** — адаптивный дизайн для телефонов

#### Пример кода (сервер на ESP8266):
```cpp
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

void setup() {
  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });
  webSocket.begin();
}

void loop() {
  webSocket.loop();
  // Отправка данных всем клиентам
  String data = "{\"temp\":" + String(tempFiltered) + 
                ",\"hum\":" + String(humidityFiltered) + 
                ",\"soil\":" + String(soilMoistureFiltered) + "}";
  webSocket.broadcastTXT(data);
  delay(1000);
}
```

#### Пример HTML/JS (фрагмент):
```cpp
<!DOCTYPE html>
<html>
<head>
  <title>Умная теплица - 3D интерфейс</title>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
  <div id="temp" class="gauge">--°C</div>
  <div id="humidity" class="gauge">--%</div>
  <div id="soil" class="gauge">--%</div>
  <canvas id="historyChart"></canvas>
  <button onclick="turnOnPump()">Включить полив</button>
  <button onclick="turnOffPump()">Выключить полив</button>
  <button onclick="turnOnLight()">Включить свет</button>
  <button onclick="turnOffLight()">Выключить свет</button>
  <button onclick="openWindow()">Открыть форточку</button>
  <button onclick="closeWindow()">Закрыть форточку</button>

  <script>
    var ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onmessage = function(event) {
      var data = JSON.parse(event.data);
      document.getElementById('temp').innerHTML = data.temp + '°C';
      document.getElementById('humidity').innerHTML = data.hum + '%';
      document.getElementById('soil').innerHTML = data.soil + '%';
    };
    
    function turnOnPump() { ws.send('PUMP_ON'); }
    function turnOffPump() { ws.send('PUMP_OFF'); }
    function turnOnLight() { ws.send('LIGHT_ON'); }
    function turnOffLight() { ws.send('LIGHT_OFF'); }
    function openWindow() { ws.send('WINDOW_OPEN'); }
    function closeWindow() { ws.send('WINDOW_CLOSE'); }
  </script>
</body>
</html>
```

#### Интерфейс на телефоне (адаптивный дизайн):
```cpp
┌─────────────────────────────────┐
│  🌡️ 24.5°C    💧 58%    🌱 35%  │
├─────────────────────────────────┤
│  ┌─────────────────────────┐    │
│  │                         │    │
│  │      3D МОДЕЛЬ          │    │
│  │        ТЕПЛИЦЫ          │    │
│  │                         │    │
│  └─────────────────────────┘    │
├─────────────────────────────────┤
│  [💧 ПОЛИВ]  [💡 СВЕТ]  [🪟 ОКНО] │
├─────────────────────────────────┤
│  📊 ГРАФИК ТЕМПЕРАТУРЫ ЗА 24 Ч   │
│  ████████░░░░░░░░░░░░░░░░░░░░   │
├─────────────────────────────────┤
│  🌤️ ПРОГНОЗ: +22°C, без осадков │
└─────────────────────────────────┘
```

---

### 💡 ИНТЕГРАЦИЯ С ГОЛОСОВЫМИ ПОМОЩНИКАМИ — ИДЕЯ

**Идея:** Управление теплицей голосом через Алису, Салют, Марусю или Google Assistant. Пользователь может просто сказать команду, и теплица выполнит действие — включит полив, откроет форточку или расскажет о состоянии растений.

#### Проблема, которую решает:

Мобильное приложение и веб-интерфейс требуют, чтобы пользователь достал телефон, открыл приложение и нажал кнопку. Голосовое управление позволяет взаимодействовать с теплицей "без рук" — это быстрее, удобнее и доступнее для людей с ограниченными возможностями.

#### Поддерживаемые платформы:

| Платформа | Устройства | Команды | Сложность интеграции |
|-----------|------------|---------|---------------------|
| **Яндекс.Алиса** | Яндекс.Станция, колонки с Алисой | Да | Средняя |
| **Салют (Сбер)** | SberBoom, SberPortal | Да | Высокая |
| **Маруся (VK)** | Капсула, умные колонки VK | Да | Средняя |
| **Google Assistant** | Google Home, телефоны Android | Да | Низкая (через IFTTT) |
| **Apple Siri** | HomePod, iPhone, iPad | Да | Средняя (через HomeBridge) |

#### Примеры голосовых команд:

| Команда | Действие | Платформа |
|---------|----------|-----------|
| "Алиса, скажи состояние теплицы" | "Температура 25 градусов, влажность 60%, почва влажная" | Все |
| "Алиса, включи полив на 30 секунд" | Насос включается на 30 секунд | Яндекс, Салют |
| "Алиса, выключи фитоленту" | Фитолента гаснет | Все |
| "Алиса, какой сегодня прогноз для теплицы?" | "Ожидается дождь, влажность повысится" | Яндекс |
| "Алиса, что с томатами?" | "Томаты политы 2 часа назад, влажность почвы 65%" | Все |
| "Алиса, открой форточку" | Сервопривод открывает форточку | Все |
| "Алиса, закрой форточку" | Форточка закрывается | Все |
| "Google, water my plants for 10 seconds" | Насос включается на 10 секунд | Google Assistant |
| "Алиса, через сколько поливать?" | "Почва ещё влажная, полив не требуется" | Все |
| "Алиса, что не так с теплицей?" | "Закончилась вода в баке. Низкая освещённость" | Все |

#### Архитектура интеграции:
``` cpp
┌─────────────────────────────────────────────────────────────────┐
│                       ПОЛЬЗОВАТЕЛЬ                              │
│                            │                                    │
│                  "Алиса, включи полив"                          │
│                            ↓                                    │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │ ГОЛОСОВОЙ ПОМОЩНИК (Алиса)                              │   │
│   │ - Распознаёт речь                                       │   │
│   │ - Понимает команду                                      │   │
│   │ - Отправляет запрос в Навык (Skill)                     │   │
│   └─────────────────────────────────────────────────────────┘   │
│                           ↓                                     │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │ НАВЫК (Яндекс.Диалоги)                                  │   │
│   │ - Обрабатывает команду                                  │   │
│   │ - Формирует запрос к теплице                            │   │
│   └─────────────────────────────────────────────────────────┘   │
│                           ↓                                     │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │ ОБЛАЧНЫЙ СЕРВЕР (MQTT)                                  │   │
│   │ - Принимает команды от навыка                           │   │
│   │ - Отправляет команды на ESP8266                         │   │
│   └─────────────────────────────────────────────────────────┘   │
│                           ↓                                     │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │ ESP8266 (Wi-Fi)                                         │   │
│   │ - Получает команды из облака                            │   │
│   │ - Управляет Arduino через Serial                        │   │
│   └─────────────────────────────────────────────────────────┘   │
│                           ↓                                     │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │ ARDUINO UNO                                             │   │
│   │ - Выполняет команды (включает насос, свет и т.д.)       │   │
│   └─────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

#### Упрощённая архитектура (через IFTTT для Google Assistant):
``` cpp
┌─────────────┐   ┌─────────────┐   ┌─────────────┐   ┌─────────────┐
│ Google      │ → │ IFTTT       │ → │ Blynk       │ → │ ESP8266     │
│ Assistant   │   │ (Webhook)   │   │ (Webhook)   │   │             │
└─────────────┘   └─────────────┘   └─────────────┘   └─────────────┘
        │
        ↓
┌─────────────┐
│ Arduino     │
│ Uno         │
└─────────────┘
```


#### Код для ESP8266 (получение команд из MQTT):

```cpp
// =====================================================================
// ESP8266 КОД — ГОЛОСОВОЕ УПРАВЛЕНИЕ ЧЕРЕЗ MQTT
// =====================================================================

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Wi-Fi настройки
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT настройки
const char* mqtt_server = "mqtt.cloud.yandex.net";
const char* mqtt_topic = "/greenhouse/commands";
const char* mqtt_status_topic = "/greenhouse/status";

WiFiClient espClient;
PubSubClient client(espClient);

// Подключение к Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

// Обработка входящих команд от голосового помощника
void callback(char* topic, byte* payload, unsigned int length) {
  String command;
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  
  Serial.print("Command received: ");
  Serial.println(command);
  
  // Парсим JSON
  StaticJsonDocument<200> doc;
  deserializeJson(doc, command);
  
  String action = doc["action"];
  
  // Отправка команды на Arduino через Serial
  if (action == "PUMP_ON") {
    Serial.println("CMD:PUMP_ON");
    client.publish(mqtt_status_topic, "{\"status\":\"pump turned on\"}");
  }
  else if (action == "PUMP_OFF") {
    Serial.println("CMD:PUMP_OFF");
    client.publish(mqtt_status_topic, "{\"status\":\"pump turned off\"}");
  }
  else if (action == "PUMP_ON_FOR") {
    int duration = doc["duration"];
    Serial.print("CMD:PUMP_ON_FOR:");
    Serial.println(duration);
  }
  else if (action == "LIGHT_ON") {
    Serial.println("CMD:LIGHT_ON");
  }
  else if (action == "LIGHT_OFF") {
    Serial.println("CMD:LIGHT_OFF");
  }
  else if (action == "WINDOW_OPEN") {
    Serial.println("CMD:WINDOW_OPEN");
  }
  else if (action == "WINDOW_CLOSE") {
    Serial.println("CMD:WINDOW_CLOSE");
  }
  else if (action == "GET_STATUS") {
    Serial.println("CMD:GET_STATUS");
  }
}

// Подключение к MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Получение статуса от Arduino и отправка в облако
  if (Serial.available()) {
    String status = Serial.readStringUntil('\n');
    client.publish(mqtt_status_topic, status.c_str());
  }
}
```

#### Код для Arduino (приём команд и отправка статуса):
``` cpp
// =====================================================================
// ARDUINO КОД — ПРИЁМ КОМАНД ОТ ESP8266
// =====================================================================

String inputString = "";
bool commandReceived = false;

void setup() {
  Serial.begin(115200);
  // ... остальная инициализация ...
}

void processCommand(String cmd) {
  if (cmd.startsWith("PUMP_ON_FOR:")) {
    int duration = cmd.substring(12).toInt();
    digitalWrite(PUMP_PIN, HIGH);
    delay(duration * 1000);
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("{\"event\":\"pump_finished\"}");
  }
  else if (cmd == "PUMP_ON") {
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("{\"event\":\"pump_on\"}");
  }
  else if (cmd == "PUMP_OFF") {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("{\"event\":\"pump_off\"}");
  }
  else if (cmd == "LIGHT_ON") {
    digitalWrite(LED_GROW_PIN, HIGH);
    Serial.println("{\"event\":\"light_on\"}");
  }
  else if (cmd == "LIGHT_OFF") {
    digitalWrite(LED_GROW_PIN, LOW);
    Serial.println("{\"event\":\"light_off\"}");
  }
  else if (cmd == "WINDOW_OPEN") {
    ventServo.write(SERVO_OPEN);
    Serial.println("{\"event\":\"window_opened\"}");
  }
  else if (cmd == "WINDOW_CLOSE") {
    ventServo.write(SERVO_CLOSED);
    Serial.println("{\"event\":\"window_closed\"}");
  }
  else if (cmd == "GET_STATUS") {
    // Отправляем статус в формате JSON
    Serial.print("{\"temp\":");
    Serial.print(tempFiltered);
    Serial.print(",\"hum\":");
    Serial.print(humidityFiltered);
    Serial.print(",\"soil\":");
    Serial.print(soilMoistureFiltered);
    Serial.print(",\"light\":");
    Serial.print(lightLevelFiltered);
    Serial.print(",\"water\":");
    Serial.print(waterPresent ? "true" : "false");
    Serial.println("}");
  }
}

void loop() {
  // Чтение команд от ESP8266
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      commandReceived = true;
      break;
    }
    inputString += inChar;
  }
  
  if (commandReceived) {
    processCommand(inputString);
    inputString = "";
    commandReceived = false;
  }
  
  // ... остальной код loop() ...
}
```

#### Настройка Яндекс.Диалогов (Навык для Алисы):
``` cpp
{
  "intents": {
    "get_status": {
      "tokens": ["состояние", "статус", "расскажи", "что с"]
    },
    "turn_on_pump": {
      "tokens": ["включи", "полив", "полей", "помой"]
    },
    "turn_off_pump": {
      "tokens": ["выключи", "останови", "перестань", "хватит"]
    },
    "turn_on_light": {
      "tokens": ["включи", "свет", "лампу", "подсветку"]
    },
    "turn_off_light": {
      "tokens": ["выключи", "свет", "лампу", "погаси"]
    },
    "open_window": {
      "tokens": ["открой", "форточку", "окно", "проветри"]
    },
    "close_window": {
      "tokens": ["закрой", "форточку", "окно", "затвори"]
    },
    "get_forecast": {
      "tokens": ["прогноз", "погода", "сегодня", "завтра"]
    }
  }
}
```
---

### 🔮 МОБИЛЬНОЕ ПРИЛОЖЕНИЕ С АР-ВИЗУАЛИЗАЦИЕЙ — В ПЛАНАХ

**Идея:** Приложение на телефоне, которое показывает данные с датчиков в реальном времени прямо поверх изображения теплицы (Augmented Reality). Пользователь наводит камеру на теплицу — и над ней появляются показания температуры, влажности, уровня воды и т.д.

#### Проблема, которую решает:
Существующие приложения для умных теплиц показывают данные в виде графиков и цифр на экране. Пользователь должен мысленно сопоставлять эти цифры с реальной теплицей. AR-визуализация накладывает данные прямо на изображение теплицы — это интуитивно понятно, наглядно и впечатляюще.

#### Функции мобильного приложения:

| Функция | Описание |
|---------|----------|
| **AR-режим** | Навести камеру на теплицу — поверх покажутся показания датчиков |
| **Управление** | Кнопки включения полива, света, форточки |
| **Уведомления** | Push-уведомления о проблемах (закончилась вода, высокая температура) |
| **Графики** | История температуры, влажности, поливов за день/неделю/месяц |
| **Дневник садовода** | Фото растений, заметки, напоминания о подкормке |
| **QR-код на теплице** | Быстрое подключение к нужной теплице (если их несколько) |
| **Голосовое управление** | Команды через приложение |
| **Шеринг данных** | Отправка отчёта о состоянии теплицы друзьям или агроному |


#### Технологии для реализации:

| Компонент | Технология | Почему |
|-----------|-----------|--------|
| **Кроссплатформенность** | Flutter / React Native | Один код для iOS и Android |
| **AR-визуализация** | ARCore (Android) / ARKit (iOS) | Стандарты дополненной реальности |
| **Облачное хранение** | Firebase | Бесплатно до 1 ГБ, реальное время |
| **Пуш-уведомления** | Firebase Cloud Messaging | Бесплатно, надёжно |
| **Авторизация** | Firebase Auth | Google, Apple, email |
| **Графики** | FL Chart (Flutter) / Victory (React) | Красивые и интерактивные |
| **Аналитика** | Firebase Analytics | Бесплатно, понимание использования |

#### Архитектура приложения:
``` cpp
┌─────────────────────────────────────────────────────────────────────┐
│                      МОБИЛЬНОЕ ПРИЛОЖЕНИЕ                           │ 
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│   ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐   │
│   │ AR-МОДУЛЬ   │ │ УПРАВЛЕНИЕ  │ │ ГРАФИКИ     │ │ ДНЕВНИК     │   │
│   │ - ARCore    │ │ - Кнопки    │ │ - Chart.js  │ │ - Фото      │   │
│   │ - ARKit     │ │ - Слайдеры  │ │ - История   │ │ - Заметки   │   │
│   └─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘   │
│          │               │               │               │          │
│          └───────────────┼───────────────┼───────────────┘          │
│                          ↓               ↓                          │
│                   ┌─────────────────────────────┐                   │
│                   │ Firebase / Supabase         │                   │
│                   │ - Realtime Database         │                   │
│                   │ - Authentication            │                   │
│                   │ - Cloud Messaging           │                   │
│                   └─────────────────────────────┘                   │
│                                    │                                │
└────────────────────────────────────┼───────────────────────────────┘
                                     │
                                     ↓
                          ┌─────────────────────┐
                          │ ESP8266 / Arduino   │
                          │ - Wi-Fi связь       │
                          │ - MQTT / HTTP       │
                          └─────────────────────┘
```


#### Пример кода для Flutter (AR-экран):

```dart
// =====================================================================
// FLUTTER AR-ЭКРАН (Android)
// =====================================================================

import 'package:flutter/material.dart';
import 'package:arcore_flutter_plugin/arcore_flutter_plugin.dart';
import 'package:firebase_database/firebase_database.dart';

class ARScreen extends StatefulWidget {
  @override
  _ARScreenState createState() => _ARScreenState();
}

class _ARScreenState extends State<ARScreen> {
  late ArCoreController arCoreController;
  DatabaseReference dbRef = FirebaseDatabase.instance.ref('greenhouse');
  double temp = 24.5;
  double humidity = 58.0;
  int soil = 35;
  int light = 45;
  bool waterPresent = true;

  @override
  void initState() {
    super.initState();
    // Подписка на изменения данных в Firebase
    dbRef.child('sensors').onValue.listen((event) {
      final data = event.snapshot.value as Map;
      setState(() {
        temp = data['temperature'] ?? 24.5;
        humidity = data['humidity'] ?? 58.0;
        soil = data['soil_moisture'] ?? 35;
        light = data['light'] ?? 45;
        waterPresent = data['water_present'] ?? true;
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('AR-режим')),
      body: Stack(
        children: [
          // AR-камера
          ArCoreView(
            onArCoreViewCreated: _onArCoreViewCreated,
            enableTapRecognizer: true,
          ),
          // Наложение данных
          Positioned(
            top: 50,
            right: 20,
            child: Container(
              padding: EdgeInsets.all(16),
              decoration: BoxDecoration(
                color: Colors.black.withOpacity(0.7),
                borderRadius: BorderRadius.circular(12),
              ),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text('🌡️ $temp°C', style: TextStyle(color: Colors.white)),
                  Text('💧 $humidity%', style: TextStyle(color: Colors.white)),
                  Text('🌱 $soil%', style: TextStyle(color: soil < 40 ? Colors.red : Colors.green)),
                  Text('💡 $light%', style: TextStyle(color: Colors.white)),
                  Text('💧 ${waterPresent ? "ЕСТЬ" : "НЕТ"}', 
                       style: TextStyle(color: waterPresent ? Colors.green : Colors.red)),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  void _onArCoreViewCreated(ArCoreController controller) {
    arCoreController = controller;
    // Добавление 3D-меток в реальное пространство
    _addDataLabels();
  }

  void _addDataLabels() {
    // 3D-метка температуры
    final tempNode = ArCoreNode(
      shape: ArCoreCube(
        materials: [ArCoreMaterial(color: Colors.red)],
        size: Vector3(0.1, 0.1, 0.1),
      ),
      position: Vector3(0.5, 0.5, -1.0),
    );
    arCoreController.addArCoreNode(tempNode);
  }
}
```

#### Пример кода для Firebase Cloud Functions (уведомления):
``` cpp
// =====================================================================
// FIREBASE CLOUD FUNCTIONS — ОТПРАВКА ПУШ-УВЕДОМЛЕНИЙ
// =====================================================================

const functions = require('firebase-functions');
const admin = require('firebase-admin');
admin.initializeApp();

// Мониторинг уровня воды
exports.waterLevelAlert = functions.database.ref('/greenhouse/sensors/water_present')
  .onUpdate((change, context) => {
    const wasPresent = change.before.val();
    const isPresent = change.after.val();
    
    if (wasPresent === true && isPresent === false) {
      // Вода только что закончилась
      const payload = {
        notification: {
          title: '⚠️ Закончилась вода!',
          body: 'Налейте воду в бак теплицы',
          sound: 'default',
        },
      };
      return admin.messaging().sendToTopic('greenhouse_alerts', payload);
    }
    return null;
  });

// Мониторинг температуры
exports.temperatureAlert = functions.database.ref('/greenhouse/sensors/temperature')
  .onUpdate(async (change, context) => {
    const temp = change.after.val();
    
    if (temp > 35) {
      const payload = {
        notification: {
          title: '🌡️ Критическая температура!',
          body: `${temp}°C в теплице. Откройте форточку!`,
          sound: 'default',
        },
      };
      return admin.messaging().sendToTopic('greenhouse_alerts', payload);
    }
    return null;
  });
```

---

### 📋 ТЕПЛИЦА-КОНСТРУКТОР (3D-ПЕЧАТЬ И МОДУЛЬНОСТЬ) — ПРОРАБОТАНО

**Идея:** Все детали теплицы (корпус контроллера, кронштейны для датчиков, крепления для сервопривода, уголки для каркаса) можно распечатать на 3D-принтере и собрать как конструктор. Это делает проект доступным для повторения и масштабирования.

#### Проблема, которую решает:

Многие хотят повторить умную теплицу, но не имеют доступа к лазерной резке или фрезерному станку. 3D-печать — это доступная технология: принтер стоит от 10 000 ₽, а печать деталей обходится в копейки. Кроме того, 3D-модели можно легко модифицировать под свои нужды.

#### Преимущества модульного подхода:

| Преимущество | Описание |
|--------------|----------|
| **Доступность** | 3D-принтеры есть в школах, кванториумах, коворкингах |
| **Низкая стоимость** | Пластик PLA стоит ~1500 ₽/кг, все детали ~200-300 ₽ |
| **Модифицируемость** | Можно изменить размеры под свою теплицу |
| **Ремонтопригодность** | Сломанную деталь можно перепечатать за час |
| **Масштабируемость** | Легко добавить новые модули |
| **Обмен опытом** | Модели можно выложить на Thingiverse, Printables |

#### Полный список деталей для 3D-печати:

| № | Деталь | Количество | Назначение | Время печати | PLA (грамм) |
|:-:|--------|------------|------------|--------------|-------------|
| 1 | Корпус контроллера (нижняя часть) | 1 | Защита Arduino и реле | 3 часа | 80 г |
| 2 | Крышка контроллера | 1 | Закрывает корпус | 1.5 часа | 40 г |
| 3 | Кронштейн для сервопривода | 1 | Крепление серво к форточке | 1 час | 25 г |
| 4 | Держатель DHT11 | 1 | Крепление датчика температуры | 30 мин | 10 г |
| 5 | Держатель фоторезистора | 1 | Крепление датчика света | 30 мин | 10 г |
| 6 | Держатель датчика влажности почвы | 1 | Фиксация датчика в горшке | 45 мин | 15 г |
| 7 | Уголок для каркаса (4 шт) | 4 | Соединение стенок теплицы | 30 мин/шт | 15 г/шт |
| 8 | Кронштейн для фитоленты | 2 | Крепление светодиодной ленты | 1 час/шт | 20 г/шт |
| 9 | Органайзер для проводов | 2 | Аккуратная укладка проводов | 20 мин/шт | 5 г/шт |
| 10 | Крепление для LCD дисплея | 1 | Установка дисплея на корпус | 1 час | 25 г |
| 11 | Кронштейн для насоса | 1 | Крепление насоса к стенке | 1 час | 20 г |
| 12 | Крышка для бака с водой | 1 | Защита воды от мусора | 2 часа | 50 г |

**Итого:** ~12-15 часов печати, ~300-350 грамм пластика (200-300 ₽)

#### 1. Корпус контроллера (Arduino + реле + дисплей)
```
// Описание модели (в реальном STL-файле)
// Размеры: 160 x 100 x 50 мм
// Крепления: 4 отверстия под винты M3
// Отверстия для проводов: 6 (диаметр 8 мм)
// Окно для дисплея: 80 x 36 мм
// Крепление платы Arduino: 4 стойки под винты M3
// Крепление реле: 3 отверстия под винты M3
```
**Особенности дизайна:**
* Вентиляционные отверстия для охлаждения реле
* Кабельные вводы с резиновыми заглушками
* Крышка на защёлках (без винтов)
* Место для подписи маркером

#### 2. Кронштейн для сервопривода
```
Описание модели:
* Крепление серво SG90 / MG995
* Угол поворота: регулируемый (0-120°)
* Крепление к форточке: 2 отверстия под саморезы
* Крепление серво: 4 отверстия под винты M2.5
```

#### 3. Держатель DHT11 с защитным экраном
```
Описание модели:
* Защита от прямых солнечных лучей
* Вентиляционные отверстия для доступа воздуха
* Крепление на стену или на штатив
```

#### 4. Держатель фоторезистора с затемнением
```
Описание модели:
* Трубка для направления света
* Защита от боковой засветки
* Крепление на двухсторонний скотч
```

#### 5. Уголок для каркаса теплицы
```
Описание модели:
* Размер: 40 x 40 x 40 мм
* Крепление: 4 отверстия под саморезы
* Материал: PETG (более прочный) или PLA
```

#### 6. Кронштейн для фитоленты
```
Описание модели:
* Длина: регулируемая (30-100 см)
* Направляющие для ленты
* Отражатель (белый ABS)
```
---

### 🔮 ЭКОЛОГИЧНОСТЬ: РАБОТА ОТ СОЛНЕЧНОЙ БАТАРЕИ — В ПЛАНАХ

**Идея:** Теплица может работать полностью автономно от солнечной энергии. Это делает проект независимым от электросети, позволяет устанавливать теплицу в отдалённых местах (дача, поле, крыша) и снижает эксплуатационные расходы до нуля.

#### Проблема, которую решает:

Большинство умных теплиц требуют постоянного подключения к розетке 220В. Это ограничивает их использование в местах без электричества (дача в межсезонье, поле, балкон). Кроме того, электроэнергия стоит денег. Солнечная батарея решает обе проблемы.

#### Преимущества солнечного питания:

| Преимущество | Описание |
|--------------|----------|
| **Энергонезависимость** | Теплица работает даже при отключении электричества |
| **Нулевые эксплуатационные расходы** | Солнце бесплатно |
| **Экологичность** | Нет выбросов CO₂ |
| **Мобильность** | Можно установить в любом месте |
| **Автономность** | Не нужна розетка |
| **Образовательная ценность** | Демонстрация возобновляемой энергии |

#### Компоненты солнечной системы:

| Компонент | Характеристики | Цена | Где купить |
|-----------|----------------|------|------------|
| Солнечная панель | 12В, 50-100 Вт (поликристаллическая) | 3000-5000 ₽ | Ozon, AliExpress |
| Контроллер заряда | PWM 10А (с защитой от перезаряда) | 500-1000 ₽ | Ozon, AliExpress |
| Аккумулятор | 12В, 7-20 А·ч (гелевый или AGM) | 1500-3000 ₽ | Автомагазин |
| Инвертор (опционально) | 12В → 220В, 200 Вт (для насоса) | 1000-1500 ₽ | Ozon |
| Разъёмы MC4 | 1 пара | 200 ₽ | AliExpress |
| Кабель солнечный (4 мм²) | 5 метров | 300 ₽ | AliExpress |
| Кронштейны для панели | 2 шт | 500 ₽ | Можно напечатать на 3D-принтере |
| Предохранитель 10А | 1 шт | 50 ₽ | Любой магазин электроники |

**Итого:** 5000-10000 ₽ (окупается за 1-2 года экономии на электричестве)

#### Расчёт энергопотребления теплицы:

| Устройство | Мощность | Время работы в сутки | Потребление (Вт·ч) |
|------------|----------|----------------------|-------------------|
| Arduino Uno | 2 Вт | 24 ч | 48 |
| Датчик DHT11 | 0.1 Вт | 24 ч | 2.4 |
| Датчик влажности почвы | 0.05 Вт | 24 ч | 1.2 |
| Фоторезистор | 0.01 Вт | 24 ч | 0.24 |
| LCD дисплей | 0.5 Вт | 24 ч | 12 |
| **Итого логика** | **2.66 Вт** | | **~64 Вт·ч** |
| Реле (катушка) | 0.36 Вт | 24 ч (но срабатывает редко) | ~1 Вт·ч |
| Насос 12В | 15 Вт | 12 секунд × 6 раз = 72 секунды | 0.3 Вт·ч |
| Фитолента (ночью) | 15 Вт | 12 часов | 180 Вт·ч |
| Сервопривод (1 цикл) | 2 Вт | 1 секунда | 0.0005 Вт·ч |
| **ИТОГО максимум** | | | **~245 Вт·ч/сутки** |

#### Расчёт необходимой солнечной панели:
Потребление: 245 Вт·ч/сутки
Потери в контроллере и АКБ: +20% = 294 Вт·ч/сутки

Выработка панели 50 Вт при 5 часах инсоляции: 50 Вт × 5 ч = 250 Вт·ч
Выработка панели 100 Вт при 5 часах инсоляции: 100 Вт × 5 ч = 500 Вт·ч

Вывод: панели 50 Вт достаточно для основного режима (без фитоленты)
панели 100 Вт достаточно с запасом (включая фитоленту)


#### Расчёт ёмкости аккумулятора:
Потребление за сутки: 245 Вт·ч
Напряжение: 12В
Ёмкость в А·ч = 245 Вт·ч / 12В = 20.4 А·ч

С учётом того, что АКБ нельзя разряжать полностью (максимум 50% для долгой жизни):
20.4 × 2 = 40.8 А·ч

Рекомендуемая ёмкость: 20-25 А·ч (хватит на ночь, а днём панель заряжает)


#### Схема подключения солнечной системы:
```cpp
┌─────────────────────────────────────────────────────────────────────┐
│                      СОЛНЕЧНАЯ СИСТЕМА                              │
│                                                                     │
│     ┌─────────────┐     ┌─────────────────┐     ┌─────────────────┐ │
│     │ Солнечная   │     │ Контроллер      │     │  Аккумулятор    │ │
│     │ панель      │────→│ заряда          │────→│ 12В             │ │
│     │ 12В/100Вт   │     │ PWM 10А         │     │ 20-25 А·ч       │ │
│     └─────────────┘     └─────────────────┘     └────────┬────────┘ │
│                                                          │          │
│                                                          ↓          │
│                                                 ┌─────────────────┐ │
│                                                 │ Arduino +       │ │
│                                                 │ датчики         │ │
│                                                 │ (5V через       │ │
│                                                 │ DC-DC)          │ │
│                                                 └─────────────────┘ │
│                                                                     │
│ Дополнительно:                                                      │
│    ┌─────────────┐     ┌─────────────────┐     ┌─────────────────┐  │
│    │ АКБ 12В     │────→│ DC-DC 5V        │────→│ Arduino         │  │
│    │             │     │ понижающий      │     │ и датчики       │  │
│    └─────────────┘     └─────────────────┘     └─────────────────┘  │
│         │                                                           │
│         ↓                                                           │
│ ┌─────────────────┐                                                 │
│ │ Насос +         │                                                 │
│ │ Фитолента       │                                                 │
│ │ (12V)           │                                                 │
│ └─────────────────┘                                                 │
└─────────────────────────────────────────────────────────────────────┘
```


#### Код для контроля заряда аккумулятора:

```cpp
// =====================================================================
// КОНТРОЛЬ ЗАРЯДА АККУМУЛЯТОРА (ДЛЯ СОЛНЕЧНОЙ СИСТЕМЫ)
// =====================================================================

#define BATTERY_PIN A3      // Делитель напряжения на аккумуляторе
#define LOW_BATTERY_ALERT 11.0  // Аварийный порог (Вольт)
#define CRITICAL_BATTERY 10.5   // Критический порог (отключение)

float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN);
  // Делитель напряжения: R1=10к, R2=10к (коэффициент 2)
  float voltage = (raw / 1023.0) * 5.0 * 2.0;
  return voltage;
}

void checkBattery() {
  static unsigned long lastBatteryCheck = 0;
  static bool lowBatteryAlertSent = false;
  static bool criticalShutdown = false;
  unsigned long now = millis();
  
  if (now - lastBatteryCheck > 60000) {  // Раз в минуту
    lastBatteryCheck = now;
    float batteryVoltage = readBatteryVoltage();
    
    Serial.print("Battery voltage: ");
    Serial.print(batteryVoltage);
    Serial.println("V");
    
    // Вывод на дисплей (экран 4)
    lcd->setCursor(0, 1);
    lcd->print("BAT:");
    lcd->print(batteryVoltage, 1);
    lcd->print("V   ");
    
    // Предупреждение о низком заряде
    if (batteryVoltage < LOW_BATTERY_ALERT && !lowBatteryAlertSent) {
      lowBatteryAlertSent = true;
      
      if (currentDisplayMode == DISPLAY_NORMAL) {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("! LOW BATTERY !");
        lcd->setCursor(0, 1);
        lcd->print("Charge needed");
      }
      Serial.println("WARNING: Low battery!");
      playAlert(PRIORITY_HIGH);
    }
    
    // Критический разряд — отключаем нагрузку
    if (batteryVoltage < CRITICAL_BATTERY && !criticalShutdown) {
      criticalShutdown = true;
      
      // Отключаем фитоленту
      if (lampState) {
        digitalWrite(LED_GROW_PIN, LOW);
        lampState = false;
        Serial.println("EMERGENCY: Grow light turned off (critical battery)");
      }
      
      // Отключаем насос
      if (pumpState) {
        digitalWrite(PUMP_PIN, LOW);
        pumpState = false;
        Serial.println("EMERGENCY: Pump turned off (critical battery)");
      }
      
      // Закрываем форточку
      if (windowIsOpen) {
        ventServo.write(SERVO_CLOSED);
        windowIsOpen = false;
        Serial.println("EMERGENCY: Window closed (critical battery)");
      }
      
      Serial.println("CRITICAL: System entering power saving mode!");
    }
    
    // Восстановление заряда
    if (batteryVoltage > LOW_BATTERY_ALERT + 1.0 && criticalShutdown) {
      criticalShutdown = false;
      lowBatteryAlertSent = false;
      Serial.println("Battery recovered, normal operation resumed");
    }
  }
}
```

#### Режимы энергосбережения:
``` cpp
// =====================================================================
// РЕЖИМЫ ЭНЕРГОСБЕРЕЖЕНИЯ
// =====================================================================

enum PowerMode {
  POWER_FULL,      // Всё включено (день, аккумулятор заряжен)
  POWER_NORMAL,    // Нормальный режим (день)
  POWER_ECONOMY,   // Экономичный (ночь, аккумулятор разряжен)
  POWER_CRITICAL   // Критический (только датчики)
};

PowerMode currentPowerMode = POWER_NORMAL;
unsigned long lastPowerModeCheck = 0;

void updatePowerMode() {
  unsigned long now = millis();
  if (now - lastPowerModeCheck < 60000) return;  // Раз в минуту
  lastPowerModeCheck = now;
  
  float batteryVoltage = readBatteryVoltage();
  
  // Определяем время суток по освещённости
  bool isDaytime = (lightLevelFiltered > 30);  // Грубо, реально >30% = день
  
  if (batteryVoltage > 12.5) {
    currentPowerMode = POWER_FULL;
  } else if (batteryVoltage > 11.5) {
    currentPowerMode = POWER_NORMAL;
  } else if (batteryVoltage > 11.0) {
    currentPowerMode = POWER_ECONOMY;
  } else {
    currentPowerMode = POWER_CRITICAL;
  }
  
  // Применяем настройки энергосбережения
  switch(currentPowerMode) {
    case POWER_FULL:
      // Всё работает в штатном режиме
      break;
      
    case POWER_NORMAL:
      // Уменьшаем яркость дисплея
      lcd->setBacklight(128);
      break;
      
    case POWER_ECONOMY:
      // Отключаем дисплей ночью
      if (!isDaytime) {
        lcd->noBacklight();
      }
      // Увеличиваем интервал опроса датчиков
      // (уже есть в коде)
      break;
      
    case POWER_CRITICAL:
      // Отключаем не критичные функции
      lcd->noBacklight();
      // Прерываем полив, если идёт
      if (pumpState) {
        digitalWrite(PUMP_PIN, LOW);
        pumpState = false;
      }
      // Выключаем фитоленту
      if (lampState) {
        digitalWrite(LED_GROW_PIN, LOW);
        lampState = false;
      }
      break;
  }
  // Выводим на дисплей
  if (currentDisplayMode == DISPLAY_NORMAL && isDaytime) {
    lcd->setCursor(12, 0);
    switch(currentPowerMode) {
      case POWER_FULL: lcd->print("FULL"); break;
      case POWER_NORMAL: lcd->print("NORM"); break;
      case POWER_ECONOMY: lcd->print("ECO "); break;
      case POWER_CRITICAL: lcd->print("CRIT"); break;
    }
  }
}
```

---

### 📁 Структура кода
Весь код организован в 18 разделов для удобства чтения и отладки:
| Раздел | Назначение |
|----------|----------|
| 1 | Подключение внешних библиотек (Wire, LCD, DHT, Servo, avr/wdt) |
| 2 | Умная система защиты дисплея (указатели, адреса I2C) |
| 3 | Система восстановления дисплея (enum, флаги, макросы) |
| 4 | Конфигурация датчиков (макросы ENABLE_DHT, ENABLE_SOIL, ENABLE_LIGHT, ENABLE_WATER) |
| 5 | Подключение устройств к пинам (#define для всех датчиков и исполнительных устройств) |
| 6 | Создание объектов (DHT, Servo) |
| 7 | Настройки системы (пороги, калибровка, watchdog, тип реле) |
| 8 | Переменные для хранения данных (отфильтрованные значения, состояния устройств, таймеры) |
| 9 | Функции для автоматического восстановления дисплея (discoverLCD, initializeLCD, checkLCDHealth, safePrint, attemptDisplayRecovery, recoverySuccess) |
| 10 | Вспомогательные функции (applyFilter — фильтр сглаживания, readSoilMoistureStable, readAnalogSensorStable) |
| 11 | Функция начальной настройки setup() (инициализация всех подсистем, включение watchdog, поиск дисплея) |
| 12 | Чтение всех датчиков readSensors() (DHT11, влажность почвы, освещённость, уровень воды) |
| 13 | Управление вентиляцией controlWindow() (открытие/закрытие форточки по температуре) |
| 14 | Управление всеми системами controlSystems() (полив, освещение, вентиляция) |
| 15 | Обновление информации на дисплее updateDisplay() (5 экранов, смена каждые 3 секунды) |
| 16 | Отладочный вывод в Serial монитор debugOutput() (раз в 5 секунд) |
| 17 | Проверка уровня воды и вывод предупреждений checkWaterLevel() |
| 18 | Главный цикл loop() (управление дисплеем, чтение датчиков, управление системами, пауза) |

---

### 🐛 Устранение неполадок (с которыми пришлось столкнуться)
В процессе разработки и отладки проекта возникли следующие проблемы и были найдены их решения:

#### Проблема 1: Дисплей не работал внутри теплицы
* Симптом: дисплей зависал, показывал странные символы или вообще не включался
* Причина: электромагнитные помехи от силовых цепей (фитолента, насос, сервопривод) на длинных проводах I2C
* Решение: вынос дисплея за пределы теплицы, укорачивание проводов SDA/SCL до 20-30 см, уменьшение скорости I2C до 100 кГц
* Результат: дисплей работает стабильно

#### Проблема 2: Watchdog перезагружал Arduino во время инициализации
* Симптом: система постоянно перезагружалась при старте, не доходя до основного цикла
* Причина: длительные задержки (delay(3000), delay(2000)) превышали таймаут watchdog 2 секунды
* Решение: разбивка всех длинных задержек на циклы из коротких пауз (по 100 мс) с вызовом wdt_reset() внутри цикла
* Результат: watchdog перестал срабатывать ложно, система успешно инициализируется

#### Проблема 3: Насос не выключался
* Симптом: насос включался и работал бесконечно
* Причина: переменная pumpStartTime не обновлялась при включении насоса, условие timeCondition никогда не становилось true
* Решение: добавление строки pumpStartTime = currentTime; в блок включения насоса
* Результат: насос работает ровно 2 секунды и выключается

#### Проблема 4: Фитолента моргала (часто включалась и выключалась)
* Симптом: реле фитоленты постоянно щёлкало, лампа мигала
* Причина: свет от фитоленты попадал на датчик освещённости, создавая обратную связь
* Решение: добавление программной блокировки — после включения лампы датчик игнорируется 15 секунд
* Результат: фитолента перестала моргать, работает стабильно

#### Проблема 5: Осадок гидроксида меди в баке с водой
* Симптом: синий осадок на дне бака, насос забивался
* Причина: электролиз медных проводов датчика воды из-за постоянного напряжения 12В
* Решение: замена медных проводов на графитовые стержни (из старых батареек), импульсное питание датчика (включается только на 50 мс при измерении)
* Результат: осадок перестал появляться, датчик работает корректно

#### Проблема 6: Датчик влажности почвы показывал 0% во влажной земле
* Симптом: дисплей показывал 0% влажности, хотя земля была мокрой
* Причина: неправильный порядок аргументов в функции map()
* Решение: исправление map(soilRaw, SOIL_DRY_CALIB, SOIL_WET_CALIB, 0, 100) → map(soilRaw, SOIL_WET_CALIB, SOIL_DRY_CALIB, 0, 100)
* Результат: влажность почвы отображается корректно (сухая → 0%, мокрая → 100%)

#### Проблема 7: Фитолента включалась при свете, а не при темноте
* Симптом: лампа загоралась, когда на датчик падал свет, и гасла в темнот
* Причина: датчик освещённости работал инвертированно
* Решение: инвертирование map() для света: map(lightRaw, DARK_VALUE, BRIGHT_VALUE, 100, 0) → map(lightRaw, DARK_VALUE, BRIGHT_VALUE, 0, 100)
* Результат: фитолента включается в темноте и выключается при свете

#### Проблема 8: Двойное объявление переменной MIN_PUMP_OFF_TIME
* Симптом: ошибка компиляции "redefinition of 'const long unsigned int MIN_PUMP_OFF_TIME'"
* Причина: константа была объявлена дважды в разделе 7
* Решение: удаление дубликата, оставление только одного объявления
* Результат: код успешно компилируется

---

### 🎯 УМНАЯ БИБЛИОТЕКА РАСТЕНИЙ (PLANT LIBRARY 2.0) — В ПЛАНАХ

**Идея:** Создание встроенной базы данных (библиотеки) растений, где для каждого вида прописаны оптимальные условия роста. Пользователь выбирает растение через интерфейс (дисплей + кнопки, Web-интерфейс или приложение), и система автоматически применяет соответствующие настройки.

#### Проблема, которую решает:

Одним из ключевых недостатков существующих умных теплиц является их **жёсткая настройка** под один тип растений или универсальные параметры. Пользователь не может просто выбрать растение и получить автоматически настроенные пороги температуры, влажности, освещённости и полива.

#### Уровни развития библиотеки:

| Уровень | Название | Описание | Статус |
|:-------:|----------|----------|--------|
| **1** | Статическая библиотека | Ручной выбор растения из списка, фиксированные параметры | ⚠️ В планах |
| **2** | Автоматическая калибровка | Система сама подбирает параметры под конкретные условия | ⏳ Ожидает |
| **3** | Распознавание по фото | Камера определяет растение по листу | 🔮 Перспектива |
| **4** | Облачная синхронизация | Обновление базы через Wi-Fi, обмен настройками с сообществом | 🔮 Перспектива |
| **5** | Машинное обучение | Оптимизация параметров на основе истории роста | 🔮 Перспектива |

#### Статическая библиотека (Уровень 1):

**Структура данных для одного растения:**

```cpp
// =====================================================================
// БИБЛИОТЕКА РАСТЕНИЙ — БАЗОВАЯ СТРУКТУРА
// =====================================================================

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
  int germinationDays;        // Дни до всходов
  int harvestDays;            // Дни до сбора урожая
  const char* description;    // Краткое описание
};

// Массив с параметрами для разных растений
const Plant plantDatabase[] = {
  // ОВОЩНЫЕ КУЛЬТУРЫ
  {"Томаты",          40, 70, 18.0, 28.0, 30, 50, 60, 70, 3000, 720, 7, 80, "Популярная овощная культура"},
  {"Огурцы",          50, 80, 20.0, 30.0, 30, 50, 70, 80, 4000, 480, 5, 50, "Требуют много влаги"},
  {"Перец сладкий",   40, 65, 18.0, 28.0, 30, 50, 60, 70, 2500, 600, 10, 90, "Любит тепло и свет"},
  {"Баклажаны",       45, 70, 20.0, 30.0, 30, 50, 60, 70, 3000, 720, 10, 80, "Чувствительны к холоду"},
  {"Кабачки",         40, 65, 18.0, 28.0, 25, 45, 60, 75, 2000, 480, 5, 50, "Быстрорастущие"},
  {"Тыква",           35, 60, 18.0, 28.0, 25, 45, 55, 70, 2500, 600, 7, 100, "Требует много места"},
  {"Редис",           40, 60, 10.0, 20.0, 20, 40, 50, 60, 1500, 480, 4, 25, "Быстро созревает"},
  {"Морковь",         35, 55, 12.0, 22.0, 20, 40, 55, 70, 2000, 720, 10, 70, "Корнеплод"},
  {"Свёкла",          35, 55, 12.0, 22.0, 20, 40, 55, 70, 2000, 720, 10, 70, "Корнеплод"},
  {"Лук",             30, 50, 10.0, 22.0, 25, 45, 50, 65, 1500, 480, 8, 90, "Неприхотлив"},
  
  // ЗЕЛЕНЬ И САЛАТЫ
  {"Салат",           50, 75, 15.0, 22.0, 20, 40, 60, 75, 1500, 360, 3, 30, "Быстро растёт"},
  {"Укроп",           40, 60, 15.0, 22.0, 25, 45, 60, 70, 1500, 480, 7, 35, "Ароматная зелень"},
  {"Петрушка",        40, 60, 15.0, 22.0, 25, 45, 60, 70, 1500, 480, 10, 40, "Двулетнее растение"},
  {"Базилик",         40, 60, 18.0, 25.0, 30, 50, 60, 70, 1500, 360, 5, 30, "Любит тепло"},
  {"Кинза",           40, 60, 15.0, 25.0, 25, 45, 60, 70, 1500, 480, 7, 35, "Пряная трава"},
  {"Мята",            45, 65, 15.0, 25.0, 25, 45, 60, 75, 2000, 600, 10, 60, "Разрастается"},
  {"Руккола",         40, 65, 15.0, 22.0, 25, 45, 55, 70, 1500, 480, 4, 25, "Острый вкус"},
  {"Шпинат",          45, 70, 10.0, 20.0, 20, 40, 60, 75, 1500, 480, 5, 30, "Холодостойкий"},
  
  // ЯГОДЫ
  {"Клубника",        50, 70, 15.0, 25.0, 25, 45, 60, 70, 2000, 600, 14, 60, "Сладкие ягоды"},
  {"Земляника",       50, 70, 15.0, 25.0, 25, 45, 60, 70, 2000, 600, 14, 60, "Лесная ягода"},
  {"Малина",          45, 65, 15.0, 25.0, 25, 45, 60, 70, 2500, 720, 14, 90, "Кустарник"},
  {"Смородина",       40, 60, 15.0, 25.0, 25, 45, 60, 70, 2500, 720, 14, 90, "Витаминная"},
  {"Голубика",        50, 70, 15.0, 25.0, 25, 45, 60, 70, 2500, 720, 14, 90, "Любит кислую почву"},
  
  // ЦВЕТЫ
  {"Розы",            45, 65, 15.0, 25.0, 30, 50, 60, 70, 3000, 720, 14, 120, "Красивые цветы"},
  {"Тюльпаны",        40, 60, 10.0, 20.0, 25, 45, 55, 65, 1500, 480, 14, 60, "Луковичные"},
  {"Орхидеи",         50, 75, 20.0, 28.0, 25, 45, 65, 80, 2000, 600, 21, 180, "Экзотические"},
  {"Петунии",         40, 60, 18.0, 25.0, 30, 50, 60, 70, 1500, 480, 5, 70, "Однолетние"},
  {"Фиалки",          45, 65, 18.0, 25.0, 25, 45, 60, 75, 1500, 480, 14, 90, "Комнатные"},
  
  // ТРАВЫ
  {"Розмарин",        30, 50, 18.0, 28.0, 35, 55, 50, 65, 1500, 720, 14, 90, "Средиземноморская трава"},
  {"Тимьян",          30, 50, 18.0, 28.0, 35, 55, 50, 65, 1500, 720, 14, 90, "Пряная трава"},
  {"Орегано",         30, 50, 18.0, 28.0, 35, 55, 50, 65, 1500, 720, 14, 90, "Пряная трава"},
  {"Шалфей",          30, 50, 18.0, 28.0, 35, 55, 50, 65, 1500, 720, 14, 90, "Лекарственное"},
  
  // ЭКЗОТИЧЕСКИЕ
  {"Авокадо",         45, 65, 18.0, 28.0, 35, 55, 60, 70, 3000, 1440, 30, 365, "Из косточки"},
  {"Лимон",           40, 60, 18.0, 28.0, 35, 55, 60, 70, 2500, 1440, 30, 365, "Цитрус"},
  {"Манго",           45, 65, 22.0, 30.0, 40, 60, 60, 70, 3000, 1440, 30, 365, "Тропический"},
  {"Инжир",           40, 60, 18.0, 28.0, 35, 55, 55, 65, 2500, 1440, 21, 180, "Сладкие плоды"}
};

const int plantCount = sizeof(plantDatabase) / sizeof(plantDatabase[0]);
int currentPlantIndex = 0;
```

#### Интерфейс выбора растения на дисплее:
``` cpp
// =====================================================================
// ИНТЕРФЕЙС ВЫБОРА РАСТЕНИЯ
// =====================================================================

// Предполагается использование кнопок:
// Пин 7 — ВВЕРХ
// Пин 8 — ВНИЗ  
// Пин 9 — ВЫБОР

#define BTN_UP 7
#define BTN_DOWN 8
#define BTN_SELECT 9

void selectPlantMenu() {
  int displayStart = 0;
  int maxDisplay = 4;  // Показываем 4 растения на странице
  bool inMenu = true;
  
  // Сохраняем текущие настройки дисплея
  DisplayMode previousMode = currentDisplayMode;
  currentDisplayMode = DISPLAY_NORMAL;
  
  lcd->clear();
  
  while (inMenu) {
    // Отображаем список растений
    lcd->setCursor(0, 0);
    lcd->print("SELECT PLANT:");
    
    for (int i = 0; i < maxDisplay; i++) {
      int idx = displayStart + i;
      if (idx >= plantCount) break;
      
      lcd->setCursor(0, i + 1);
      if (idx == currentPlantIndex) {
        lcd->print(">");
      } else {
        lcd->print(" ");
      }
      lcd->print(plantDatabase[idx].name);
      // Заполняем пробелами до 15 символов
      for (int j = strlen(plantDatabase[idx].name); j < 15; j++) {
        lcd->print(" ");
      }
    }
    
    // Обработка кнопок
    if (digitalRead(BTN_UP) == LOW) {
      currentPlantIndex--;
      if (currentPlantIndex < 0) currentPlantIndex = plantCount - 1;
      if (currentPlantIndex < displayStart) displayStart = currentPlantIndex;
      delay(300);
    }
    
    if (digitalRead(BTN_DOWN) == LOW) {
      currentPlantIndex++;
      if (currentPlantIndex >= plantCount) currentPlantIndex = 0;
      if (currentPlantIndex >= displayStart + maxDisplay) displayStart = currentPlantIndex - maxDisplay + 1;
      delay(300);
    }
    
    if (digitalRead(BTN_SELECT) == LOW) {
      // Показываем информацию о растении
      const Plant& p = plantDatabase[currentPlantIndex];
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print(p.name);
      lcd->setCursor(0, 1);
      lcd->print("Apply? YES/NO");
      delay(500);
      
      // Ожидаем подтверждения
      bool confirmed = false;
      for (int i = 0; i < 100; i++) {  // 10 секунд на подтверждение
        if (digitalRead(BTN_SELECT) == LOW) {
          confirmed = true;
          break;
        }
        if (digitalRead(BTN_UP) == LOW || digitalRead(BTN_DOWN) == LOW) {
          confirmed = false;
          break;
        }
        delay(100);
      }
      
      if (confirmed) {
        applyPlantSettings(currentPlantIndex);
      }
      inMenu = false;
    }
    
    delay(100);
  }
  
  // Восстанавливаем режим дисплея
  currentDisplayMode = previousMode;
}
```

#### Расширенная библиотека с учётом стадий роста (Уровень 2):
``` cpp
// =====================================================================
// РАСШИРЕННАЯ БИБЛИОТЕКА — СТАДИИ РОСТА
// =====================================================================

enum GrowthStage {
  STAGE_SEEDLING,    // Рассада (0-14 дней)
  STAGE_VEGETATIVE,  // Вегетация (14-45 дней)
  STAGE_FLOWERING,   // Цветение (45-70 дней)
  STAGE_FRUITING     // Плодоношение (70+ дней)
};

struct PlantStage {
  int soilDry;
  int soilWet;
  float tempMin;
  float tempMax;
  int lightOn;
  int lightOff;
  int wateringTime;
  int wateringInterval;
};

struct AdvancedPlant {
  const char* name;
  PlantStage stages[4];  // для каждой стадии роста
  int daysToGrow[4];     // сколько дней до следующей стадии
  int plantAge;          // текущий возраст растения (дней)
  GrowthStage currentStage;
};

// Пример для томатов по стадиям
const AdvancedPlant tomato = {
  "Томаты",
  {
    {30, 60, 20.0, 25.0, 25, 45, 2000, 480},   // Рассада
    {40, 70, 18.0, 28.0, 30, 50, 3000, 720},   // Вегетация
    {45, 75, 20.0, 30.0, 35, 55, 3500, 600},   // Цветение
    {50, 80, 18.0, 28.0, 30, 50, 4000, 720}    // Плодоношение
  },
  {14, 31, 25, 0},  // дни до смены стадии (0 = последняя)
  0,
  STAGE_SEEDLING
};

void updatePlantStage(AdvancedPlant &plant) {
  plant.plantAge++;
  
  if (plant.plantAge < plant.daysToGrow[STAGE_SEEDLING]) {
    plant.currentStage = STAGE_SEEDLING;
  } else if (plant.plantAge < plant.daysToGrow[STAGE_SEEDLING] + plant.daysToGrow[STAGE_VEGETATIVE]) {
    plant.currentStage = STAGE_VEGETATIVE;
  } else if (plant.plantAge < plant.daysToGrow[STAGE_SEEDLING] + plant.daysToGrow[STAGE_VEGETATIVE] + plant.daysToGrow[STAGE_FLOWERING]) {
    plant.currentStage = STAGE_FLOWERING;
  } else {
    plant.currentStage = STAGE_FRUITING;
  }
  
  // Применяем параметры текущей стадии
  const PlantStage& stage = plant.stages[plant.currentStage];
  SOIL_DRY = stage.soilDry;
  SOIL_WET = stage.soilWet;
  OPEN_TEMP = stage.tempMax;
  CLOSE_TEMP = stage.tempMin;
  LIGHT_ON_THRESHOLD = stage.lightOn;
  LIGHT_OFF_THRESHOLD = stage.lightOff;
  PUMP_RUN_TIME = stage.wateringTime;
  WATERING_COOLDOWN = (unsigned long)stage.wateringInterval * 60000;
}
```

#### Интеграция с облачными сервисами (Уровень 3):
``` cpp
// =====================================================================
// ОБЛАЧНАЯ СИНХРОНИЗАЦИЯ (ДЛЯ ESP8266)
// =====================================================================

void syncPlantDatabase() {
  #ifdef ESP8266
    HTTPClient http;
    http.begin("https://api.mygreenhouse.com/plants");
    int httpCode = http.GET();
    
    if (httpCode == 200) {
      String payload = http.getString();
      // Парсим JSON и обновляем локальную базу
      DynamicJsonDocument doc(4096);
      deserializeJson(doc, payload);
      
      int newPlantCount = doc.size();
      // Обновление массива растений
      // ...
      
      Serial.println("Plant database synced from cloud");
    }
    http.end();
  #endif
}
```

---

### 💡 КОММЕРЦИАЛИЗАЦИЯ: КАК ЗАРАБОТАТЬ НА ПРОЕКТЕ — ИДЕЯ

**Идея:** Проект может стать не просто школьной поделкой, а **реальным бизнесом**. Этот раздел показывает, что вы мыслите как предприниматель и видите потенциал своего проекта на рынке.

#### Проблема, которую решает:

Многие инженерные проекты остаются на уровне прототипов, потому что их авторы не думают о коммерциализации. Анализ рынка и бизнес-моделей показывает, что проект жизнеспособен и может приносить доход.

#### Целевая аудитория:

| Сегмент | Размер рынка | Потребность | Цена готового решения |
|---------|--------------|-------------|----------------------|
| **Владельцы частных домов** | Миллионы в РФ | Автоматизация приусадебного хозяйства | 5000-10000 ₽ |
| **Дачники** | Десятки миллионов | Упрощение ухода за растениями | 3000-5000 ₽ |
| **Школьные теплицы** | Тысячи школ | Образовательные проекты | 10000-15000 ₽ |
| **Урбан-фермеры** | Растущий рынок | Круглогодичное выращивание зелени | 15000-25000 ₽ |
| **Рестораны** | Тысячи заведений | Своя свежая зелень | 20000-30000 ₽ |
| **Питомники растений** | Сотни в РФ | Автоматизация полива | 15000-25000 ₽ |
| **Коворкинги и офисы** | Тысячи | Зелёные зоны с автополивом | 10000-20000 ₽ |

#### Бизнес-модели:

| Модель | Описание | Потенциальный доход | Риски |
|--------|----------|---------------------|-------|
| **DIY-наборы** | Продажа комплектов деталей для самостоятельной сборки | 3000-5000 ₽/набор | Низкие (пользователь сам собирает) |
| **Готовые теплицы** | Полностью собранные и протестированные устройства | 10000-15000 ₽/шт | Средние (нужна сборка) |
| **Подписка на облако** | Хранение истории, уведомления, премиум-функции | 100-300 ₽/месяц | Низкие (регулярный доход) |
| **Франшиза** | Обучение сборке и продаже теплиц | 50000-100000 ₽ | Высокие (сложно масштабировать) |
| **Консультации** | Помощь в настройке под конкретные культуры | 1000-3000 ₽/час | Низкие (экспертность) |
| **Образовательные курсы** | Обучение сборке и программированию | 5000-10000 ₽/курс | Низкие (онлайн-формат) |
| **Партнёрство с магазинами** | Продажа через Ozon, Wildberries, Яндекс.Маркет | Комиссия 10-20% | Средние (зависит от платформы) |

#### Комплектации DIY-наборов:

| Название | Компоненты | Цена | Маржинальность |
|----------|------------|------|----------------|
| **Стартовый** | Arduino + датчики + реле (база) | 3000 ₽ | 30% |
| **Оптимальный** | Стартовый + насос + фитолента | 5000 ₽ | 35% |
| **Профессиональный** | Оптимальный + серво + датчик воды + 3D-корпус | 8000 ₽ | 40% |
| **Под ключ** | Полностью собранная теплица | 15000 ₽ | 50% |

#### Себестоимость компонентов (закупка оптом):

| Компонент | Розница | Опт (50 шт) | Опт (200 шт) |
|-----------|---------|-------------|--------------|
| Arduino Uno (копия) | 500 ₽ | 350 ₽ | 250 ₽ |
| Датчик DHT11 | 150 ₽ | 100 ₽ | 70 ₽ |
| Датчик влажности почвы | 200 ₽ | 130 ₽ | 90 ₽ |
| Фоторезистор + резистор | 30 ₽ | 20 ₽ | 15 ₽ |
| Релейный модуль (3 канала) | 300 ₽ | 200 ₽ | 150 ₽ |
| Насос 12В | 400 ₽ | 280 ₽ | 200 ₽ |
| Фитолента (1 метр) | 500 ₽ | 350 ₽ | 250 ₽ |
| Сервопривод SG90 | 250 ₽ | 170 ₽ | 120 ₽ |
| LCD дисплей 16x2 I2C | 400 ₽ | 280 ₽ | 200 ₽ |
| Блок питания 12В/5А | 800 ₽ | 550 ₽ | 400 ₽ |
| Блок питания 5В/2А | 500 ₽ | 350 ₽ | 250 ₽ |
| Провода, корпус, крепёж | 300 ₽ | 200 ₽ | 150 ₽ |

**Себестоимость комплекта:**
- Стартовый: ~1000 ₽ (продажа 3000 ₽ → прибыль 2000 ₽)
- Оптимальный: ~1800 ₽ (продажа 5000 ₽ → прибыль 3200 ₽)
- Профессиональный: ~3000 ₽ (продажа 8000 ₽ → прибыль 5000 ₽)

#### Маркетинговые каналы:

| Канал | Охват | Стоимость | Эффективность |
|-------|-------|-----------|---------------|
| **YouTube** (обзоры, сборка) | Миллионы зрителей | Бесплатно (своими силами) | Высокая |
| **Telegram-канал** | Тысячи подписчиков | Бесплатно | Средняя |
| **Дзен / Medium** (статьи) | Десятки тысяч | Бесплатно | Средняя |
| **Профильные выставки** (AGROS, Золотая осень) | Тысячи посетителей | 5000-20000 ₽ | Высокая |
| **Гранты и субсидии** | — | Бесплатно | Очень высокая |
| **Партнёрство с интернет-магазинами** | Миллионы покупателей | Комиссия | Высокая |
| **Сарафанное радио** | Ограниченный | Бесплатно | Очень высокая |

#### План продаж (прогноз на первый год):

| Месяц | DIY-наборы | Готовые теплицы | Подписки | Выручка | Прибыль |
|-------|------------|-----------------|----------|---------|---------|
| 1 | 0 | 0 | 0 | 0 ₽ | -5000 ₽ (регистрация ИП) |
| 2 | 5 | 0 | 0 | 15000 ₽ | 5000 ₽ |
| 3 | 10 | 1 | 5 | 35000 ₽ | 15000 ₽ |
| 4 | 15 | 2 | 10 | 60000 ₽ | 28000 ₽ |
| 5 | 20 | 3 | 20 | 90000 ₽ | 45000 ₽ |
| 6 | 30 | 5 | 35 | 140000 ₽ | 75000 ₽ |
| 7 | 40 | 7 | 50 | 200000 ₽ | 110000 ₽ |
| 8 | 50 | 10 | 70 | 270000 ₽ | 155000 ₽ |
| 9 | 60 | 12 | 90 | 340000 ₽ | 200000 ₽ |
| 10 | 70 | 15 | 110 | 420000 ₽ | 250000 ₽ |
| 11 | 80 | 18 | 130 | 500000 ₽ | 300000 ₽ |
| 12 | 100 | 25 | 160 | 650000 ₽ | 400000 ₽ |

**Итого за год:** выручка ~2.7 млн ₽, прибыль ~1.6 млн ₽ (при полной занятости)

#### Гранты и субсидии для финансирования:

| Программа | Организатор | Сумма | Условия |
|-----------|-------------|-------|---------|
| **УМНИК** | Фонд содействия инновациям | 500 000 ₽ | Инновационный проект |
| **Старт-1** | Фонд содействия инновациям | 1 000 000 ₽ | Прототип + бизнес-план |
| **Грант Президента РФ** | Министерство науки и высшего образования | 300 000 ₽ | Молодые исследователи |
| **Агростартап** | Минсельхоз РФ | 3 000 000 ₽ | Сельскохозяйственный проект |
| **Сколково** | Фонд "Сколково" | 500 000 - 5 000 000 ₽ | Резидентство |
| **Российский фонд развития информационных технологий** | РФРИТ | 500 000 - 20 000 000 ₽ | IT-проекты |

#### Программа лояльности и подписки:

**Подписка «Премиум» (300 ₽/месяц) включает:**
- Хранение истории датчиков (до 1 года)
- Push-уведомления на телефон (Telegram / приложение)
- Экспорт данных в Excel / CSV
- Приоритетная техподдержка
- Ранний доступ к новым функциям

**Подписка «Бизнес» (1000 ₽/месяц) включает:**
- Всё из «Премиум»
- Управление до 10 теплицами
- Белый этикет (брендирование)
- API доступ
- Интеграция с 1С

#### Необходимая документация для продаж:

| Документ | Стоимость | Срок | Назначение |
|----------|-----------|------|------------|
| Регистрация ИП (самозанятость) | 800 ₽ (госпошлина) | 3 дня | Легальные продажи |
| Патент на полезную модель | 3000-5000 ₽ | 6-12 месяцев | Защита интеллектуальной собственности |
| Товарный знак | 15000-30000 ₽ | 12-18 месяцев | Брендирование |
| Сертификация ЕАЭС | 20000-50000 ₽ | 1-2 месяца | Экспорт в страны ЕАЭС |
| Добровольная сертификация | 5000-10000 ₽ | 2-4 недели | Доверие покупателей |

#### Дорожная карта коммерциализации:

| Этап | Задача | Срок | Бюджет |
|------|--------|------|--------|
| 1 | Регистрация ИП (самозанятость) | 3 дня | 800 ₽ |
| 2 | Закупка компонентов оптом (первые 50 наборов) | 1 месяц | 50000 ₽ |
| 3 | Сборка и тестирование наборов | 1 месяц | - |
| 4 | Создание YouTube-канала, серии роликов | 1 месяц | 10000 ₽ |
| 5 | Запуск продаж на Ozon / Wildberries | 1 месяц | 5000 ₽ (размещение) |
| 6 | Подача заявки на грант УМНИК | 2 месяца | Бесплатно |
| 7 | Патентование полезной модели | 6-12 месяцев | 5000 ₽ |
| 8 | Запуск производства (аутсорсинг сборки) | 3 месяца | 50000 ₽ |
| 9 | Расширение ассортимента (модели для цветов, томатов и т.д.) | 6 месяцев | - |
| 10 | Экспорт в страны СНГ (сертификация) | 12 месяцев | 50000 ₽ |

---

### 🤝 ПАРТНЁРЫ — ПОТЕНЦИАЛЬНЫЕ

**Идея:** Для развития проекта необходимы партнёры — организации и люди, которые могут помочь с тестированием, производством, продвижением или финансированием. Этот раздел показывает, что вы умеете выстраивать партнёрские отношения.

#### Типы партнёров и их роль:

| Тип партнёра | Роль | Что даёт проекту | Что получает партнёр |
|--------------|------|------------------|---------------------|
| **Образовательные учреждения** | База для тестирования и демонстрации | Реальные отзывы, фото/видео материалы | Современное оборудование для обучения |
| **Производители компонентов** | Снабжение деталями | Оптовые цены, техническая поддержка | Расширение рынка сбыта |
| **IT-компании** | Облачные сервисы, хостинг | Бесплатный или льготный хостинг | Репутация, помощь образованию |
| **Агрофирмы** | Пилотное внедрение | Тестирование в реальных условиях | Оптимизация производства |
| **Инвесторы и грантодатели** | Финансирование | Деньги на развитие | Потенциальная прибыль |
| **СМИ и блогеры** | Продвижение | Бесплатная реклама | Контент для публикаций |
| **3D-печатные студии** | Производство корпусов | Изготовление деталей | Заказы |
| **Кванториумы и IT-кубы** | Экспертиза и менторство | Консультации, связи | Успешный проект для портфолио |

#### Потенциальные партнёры (контакты планируются):

| Организация | Тип | Что предлагает проект | Что нужно от партнёра | План действий |
|-------------|-----|----------------------|----------------------|---------------|
| **МОУ «Туношёнская средняя школа»** | Образовательный | Установка теплицы для демонстрации | Помещение, согласие администрации | Обсудить с директором |
| **Ярославский Кванториум** | Образовательный | Демонстрация на мастер-классах | Помещение, экспертная оценка | Подать заявку на конкурс проектов |
| **Ярославский государственный университет (ЯрГУ)** | Образовательный | Проект для студентов | Консультации, лаборатория | Связаться с кафедрой физики |
| **Сбер / Яндекс (облачные сервисы)** | IT-компания | Интеграция с умным домом | Бесплатный хостинг / грант | Подать заявку на грант для стартапов |
| **Ozon / Wildberries** | Маркетплейс | Продажа DIY-наборов | Размещение товара | Зарегистрироваться как продавец |
| **AliExpress / Ozon (поставщики)** | Производитель | Оптовые закупки | Скидки 30-50% | Связаться с менеджерами |
| **Местный питомник растений** | Агробизнес | Тестирование на реальных культурах | Отзывы, рекомендации | Личный визит, презентация |
| **АНО «Агентство стратегических инициатив»** | Грантодатель | Инновационный проект | Финансирование | Подать заявку на конкурс |
| **Фонд содействия инновациям (ФАСИ)** | Грантодатель | Проект для «УМНИК» | Грант 500 000 ₽ | Подать заявку в сентябре |
| **YouTube-блогеры (по робототехнике)** | Медиа | Обзор проекта | Видео о проекте | Написать предложение |
| **Сообщество Arduino в Telegram/VK** | Комьюнити | Распространение информации | Помощь в отладке | Опубликовать проект |
| **Thingiverse / Printables** | Платформа 3D-моделей | Публикация STL-файлов | Бесплатные модели | Загрузить файлы |

#### Ресурсы для поиска партнёров:

| Ресурс | Что искать | Как использовать |
|--------|------------|------------------|
| **Грант.ру** | Гранты и конкурсы | Поиск финансирования |
| **Фонд содействия инновациям** | «УМНИК», «Старт-1» | Подача заявки |
| **Сколково** | Резидентство | Заявка онлайн |
| **Кружковое движение НТИ** | Конкурсы, гранты | Участие в конкурсах |
| **ЯрГУ, ЯГТУ** | Кафедры физики, информатики | Личные визиты |
| **Кванториум Ярославль** | Экспертная оценка | Конкурс проектов |
| **Российский фонд развития информационных технологий** | Гранты для IT-стартапов | Подача заявки |
| **Тепличные хозяйства Ярославской области** | Пилотное внедрение | Обзвон, личные встречи |

#### Партнёрские соглашения (что можно предложить):

| Партнёр | Что предложить | Что получить |
|---------|----------------|--------------|
| **Школа** | Бесплатная установка теплицы (после доработки) | Реклама, отзывы, фото |
| **Кванториум** | Проведение мастер-класса | Экспертная оценка, помещение |
| **IT-компания** | Упоминание бренда на сайте | Бесплатный хостинг / сервер |
| **Магазин электроники** | Реклама их компонентов | Скидка 20-30% |
| **Блогер** | Эксклюзивный обзор | Бесплатная реклама |
| **Агрофирма** | Бесплатная установка для теста | Отзывы, данные для кейса |
| **3D-печатная студия** | Партнёрское размещение ссылок | Скидка на печать деталей |
  
---

### Заключение

Разработанная система умной теплицы представляет собой полностью функциональный, отказоустойчивый и масштабируемый продукт. Она решает актуальные проблемы современного сельского хозяйства:

- **Экономия ресурсов** (вода, электроэнергия)
- **Снижение трудозатрат** (автоматизация)
- **Повышение урожайности** (оптимальные условия)
- **Доступность** (низкая стоимость)

Проект имеет большой потенциал для дальнейшего развития и может быть адаптирован для использования в личных подсобных хозяйствах, школьных теплицах и малых фермерских хозяйствах.

**Ключевые преимущества проекта перед аналогами:**
1. Низкая стоимость при широком функционале
2. Отказоустойчивость (Watchdog, авто-восстановление дисплея)
3. Возможность кастомизации и масштабирования
4. Библиотека растений (в перспективе)
5. Полная открытость (код, схема, документация)

---

### 👨‍💻 Автор
**Смурыгин Алексей Сергеевич**  
МОУ «Туношёнская средняя школа»  
Ярославский муниципальный район, с. Туношна  
Руководитель: Аврамова Ольга Борисовна  
Год: 2026

---

### 📧 Контакты

По вопросам сотрудничества, использования проекта или предложений по улучшению:
- **GitHub Issues** в данном репозитории
- **Email**: aleksejsmurygin29@gmail.com

---

### 📁 Состав репозитория

- `smart_greenhouse.ino` — основной код программы
- `LICENSE` — лицензия MIT
- `README.md` — описание проекта (этот файл)
- `photos/` — фотографии процесса сборки
  - `schema.png` — схема подключения
  - `greenhouse.jpg` — фото теплицы
  - `display.jpg` — фото работающего дисплея
  - `assembly.jpg` — фото процесса сборки

---

### 🙏 Благодарности

Выражаю искреннюю благодарность:

- **Arduino Community** — за отличную документацию, библиотеки и примеры кода
- **Преподавателям Кванториума** — за поддержку, консультации и помощь в отладке
- **Создателям библиотек** — LiquidCrystal_I2C (John Rickman), DHT (Adafruit), Servo (Arduino)
- **Всем, кто тестировал систему** — за ценные замечания и помощь в поиске ошибок
- **Open Source сообществу** — за бесплатные инструменты и знания
- **Моим родителям** — за поддержку и понимание

Особая благодарность моему научному руководителю **Аврамовой Ольге Борисовне** за терпение, наставничество и ценные советы в процессе работы над проектом.

---

## 📊 Варианты маркеров для обозначения статуса

| Маркер | Значение | Когда использовать |
|:------:|----------|--------------------|
| 🔮 В ПЛАНАХ | Запланировано на будущее | Для идей, которые точно будут реализованы |
| 💡 ИДЕЯ | Концепция, требующая проработки | Для гипотетических улучшений |
| 📋 ПРОРАБОТАНО | Технически проработано, но не реализовано | Для функций, по которым есть схема/код |
| ⏳ ОЖИДАЕТ РЕАЛИЗАЦИИ | Будет сделано после конкурса | Для ближайших планов |

---

### 📜 Лицензия
Проект распространяется под лицензией MIT.
При использовании обязательно указывайте автора.
