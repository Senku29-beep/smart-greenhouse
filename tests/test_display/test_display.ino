/*
  ======================================================================
  ТЕСТ LCD ДИСПЛЕЯ 16x2 I2C
  ======================================================================
  
  НАЗНАЧЕНИЕ:
  Проверить работу LCD дисплея и автоматически определить
  правильный I2C адрес (0x27 или 0x3F).
  
  ПОДКЛЮЧЕНИЕ:
  - VCC → 5V Arduino
  - GND → GND Arduino
  - SDA → A4 (Arduino Uno)
  - SCL → A5 (Arduino Uno)
  
  ПРИНЦИП РАБОТЫ:
  1. Сканируем шину I2C в поисках дисплея
  2. Пытаемся инициализировать дисплей по найденному адресу
  3. Выводим тестовые сообщения
  4. Запускаем бегущую строку
  ======================================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C* lcd = NULL;  // Указатель на объект дисплея

void setup() {
  Serial.begin(9600);
  Serial.println(F("========================================"));
  Serial.println(F("ТЕСТ LCD ДИСПЛЕЯ 16x2 I2C"));
  Serial.println(F("========================================"));
  
  // Запускаем шину I2C
  Wire.begin();
  Serial.println(F("Шина I2C инициализирована"));
  
  // ШАГ 1: ПОИСК УСТРОЙСТВ НА ШИНЕ I2C
  Serial.println(F("\n--- ПОИСК УСТРОЙСТВ I2C ---"));
  scanI2C();
  
  // ШАГ 2: ПОДКЛЮЧЕНИЕ К ДИСПЛЕЮ
  Serial.println(F("\n--- ПОДКЛЮЧЕНИЕ ДИСПЛЕЯ ---"));
  if (!connectToLCD()) {
    Serial.println(F("[ОШИБКА] Дисплей не найден!"));
    Serial.println(F("  Проверьте подключение:"));
    Serial.println(F("  - SDA → A4, SCL → A5"));
    Serial.println(F("  - Питание 5V и GND"));
    Serial.println(F("  - Контрастность (подстроечный резистор)"));
    while (1);  // Останавливаем программу
  }
  
  // ШАГ 3: ТЕСТОВЫЕ СООБЩЕНИЯ
  Serial.println(F("\n--- ВЫВОД НА ДИСПЛЕЙ ---"));
  testMessages();
  
  // ШАГ 4: БЕГУЩАЯ СТРОКА
  Serial.println(F("\n--- БЕГУЩАЯ СТРОКА ---"));
  scrollingText();
  
  Serial.println(F("\n========================================"));
  Serial.println(F("ТЕСТ ЗАВЕРШЁН"));
  Serial.println(F("Если вы видите текст на дисплее — всё работает!"));
  Serial.println(F("========================================"));
}

void loop() {
  // Бесконечная бегущая строка
  static unsigned long lastScroll = 0;
  static int position = 0;
  
  if (millis() - lastScroll > 300) {
    lastScroll = millis();
    position = (position + 1) % 32;
    
    String message = "Hello from Smart Greenhouse!  ";
    message = message.substring(position) + message.substring(0, position);
    
    lcd->setCursor(0, 1);
    lcd->print(message.substring(0, 16));
  }
}

// ======================================================================
// ФУНКЦИЯ 1: СКАНИРОВАНИЕ ШИНЫ I2C
// ======================================================================
void scanI2C() {
  byte error, address;
  int deviceCount = 0;
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print(F("  Найдено устройство: 0x"));
      Serial.print(address, HEX);
      
      // Определяем тип устройства
      if (address == 0x27) {
        Serial.println(F(" → LCD 1602 (0x27)"));
        deviceCount++;
      } 
      else if (address == 0x3F) {
        Serial.println(F(" → LCD 1602 (0x3F)"));
        deviceCount++;
      }
      else {
        Serial.println(F(" → Другое устройство"));
        deviceCount++;
      }
    }
  }
  
  if (deviceCount == 0) {
    Serial.println(F("  [ОШИБКА] Устройства не найдены!"));
  } else {
    Serial.print(F("  Всего устройств: "));
    Serial.println(deviceCount);
  }
}

// ======================================================================
// ФУНКЦИЯ 2: ПОДКЛЮЧЕНИЕ К ДИСПЛЕЮ
// ======================================================================
bool connectToLCD() {
  // Список возможных адресов дисплея
  byte addresses[] = {0x27, 0x3F};
  
  for (int i = 0; i < 2; i++) {
    byte address = addresses[i];
    
    Serial.print(F("Пробуем адрес 0x"));
    Serial.print(address, HEX);
    Serial.print(F("... "));
    
    // Проверяем, отвечает ли устройство
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.println(F("УСПЕХ!"));
      
      // Создаём объект дисплея
      lcd = new LiquidCrystal_I2C(address, 16, 2);
      lcd->init();
      lcd->backlight();
      lcd->clear();
      
      Serial.print(F("  Дисплей инициализирован на адресе 0x"));
      Serial.println(address, HEX);
      return true;
      
    } else {
      Serial.println(F("нет ответа"));
    }
  }
  
  return false;
}

// ======================================================================
// ФУНКЦИЯ 3: ТЕСТОВЫЕ СООБЩЕНИЯ
// ======================================================================
void testMessages() {
  // Очищаем экран
  lcd->clear();
  delay(500);
  
  // Сообщение 1
  lcd->setCursor(0, 0);
  lcd->print("LCD Display Test");
  lcd->setCursor(0, 1);
  lcd->print("Hello, World!");
  Serial.println(F("  Выведено: 'Hello, World!'"));
  delay(2000);
  
  // Сообщение 2
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Smart Greenhouse");
  lcd->setCursor(0, 1);
  lcd->print("v1.0");
  Serial.println(F("  Выведено: 'Smart Greenhouse'"));
  delay(2000);
  
  // Сообщение 3 — адрес дисплея
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("I2C Address:");
  lcd->setCursor(0, 1);
  lcd->print("0x");
  lcd->print(lcdAddress, HEX);
  Serial.print(F("  Адрес дисплея: 0x"));
  Serial.println(lcdAddress, HEX);
  delay(3000);
  
  Serial.println(F("[OK] Тестовые сообщения выведены"));
}

// ======================================================================
// ФУНКЦИЯ 4: БЕГУЩАЯ СТРОКА
// ======================================================================
void scrollingText() {
  String longText = "Welcome to Smart Greenhouse! Automatic plant care system. ";
  
  for (int start = 0; start < longText.length(); start++) {
    lcd->setCursor(0, 1);
    
    // Показываем 16 символов начиная с позиции start
    String displayText = longText.substring(start, start + 16);
    lcd->print(displayText);
    
    // Очищаем остаток строки пробелами
    for (int i = displayText.length(); i < 16; i++) {
      lcd->print(" ");
    }
    
    delay(300);
  }
  
  Serial.println(F("[OK] Бегущая строка завершена"));
}
