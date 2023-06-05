#include <Arduino.h>
#include <ArduinoOTA.h>
#include <BleKeyboard.h>
#include <NimBLEDevice.h>
#include <OneButton.h>
#include <WiFi.h>

BleKeyboard bleKeyboard("uBMW", "701", 100);

#define BUTTON_PIN_1           0
#define BUTTON_PIN_2           27
#define WIFI_SSID              "uBMW"
#define WIFI_PWD               "11111111"
#define OTA_ACTIVATION_TIME    10000
#define OTA_INACTIVITY_TIMEOUT 600000 // 10 minutes in milliseconds

OneButton btn1             = OneButton(BUTTON_PIN_1, true, true);
OneButton btn2             = OneButton(BUTTON_PIN_2, true, true);
bool rallyMode             = false;
int holdCount              = 0;
static int volumeFrequency = 500;
unsigned long button1Held  = 0;
unsigned long button2Held  = 0;

void clickHandler1() {
  if (!bleKeyboard.isConnected()) return;

  if (rallyMode) {
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    Serial.println("KEY_MEDIA_VOLUME_DOWN");
  } else {
    bleKeyboard.print('2');
    Serial.println("Toggle ANC (2)");
  }

  Serial.println("[1] Button single click");
}

void doubleClickHandler1() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    Serial.println("KEY_MEDIA_PREVIOUS_TRACK");
  }

  Serial.println("[1] Button double click");
}

void tripleClickHandler1() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_EJECT);
    Serial.println("KEY_MEDIA_EJECT");
  }

  Serial.println("[1] Button triple click");
}

void longPressStartHandler1() {
  holdCount++;
  button1Held = millis();

  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    Serial.println("KEY_MEDIA_VOLUME_DOWN");
  }

  Serial.println("[1] Button long press start");
}

void clickHandler2() {
  if (!bleKeyboard.isConnected()) return;

  if (rallyMode) {
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    Serial.println("KEY_MEDIA_VOLUME_UP");
  } else {
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    Serial.println("KEY_MEDIA_PLAY_PAUSE");
  }

  Serial.println("[2] Button single click");
}

void doubleClickHandler2() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    Serial.println("KEY_MEDIA_NEXT_TRACK");
  }

  Serial.println("[2] Button double click");
}

void tripleClickHandler2() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.print('1');
    Serial.println("Connect AirPods (1)");
  }

  Serial.println("[2] Button triple click");
}

void longPressStartHandler2() {
  holdCount++;
  button2Held = millis();
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    Serial.println("KEY_MEDIA_VOLUME_UP");
  }
  Serial.println("[2] Button long press start");
}

void longPressStopHandler2() {
  holdCount--;
  button2Held = 0;
  Serial.println("[2] Button long press stop");
}

void longPressStopHandler1() {
  holdCount--;
  button1Held = 0;
  Serial.println("[1] Button long press stop");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Device started");

  btn1.setClickMs(500);
  btn2.setClickMs(500);

  bleKeyboard.begin();

  if (digitalRead(BUTTON_PIN_1) == LOW && digitalRead(BUTTON_PIN_2) == LOW) {
    Serial.println("Enable rally mode");
    rallyMode = true;
  } else {
    btn1.attachDoubleClick(doubleClickHandler1);
    btn1.attachMultiClick(tripleClickHandler1);

    btn2.attachDoubleClick(doubleClickHandler2);
    btn2.attachMultiClick(tripleClickHandler2);
  }

  btn1.attachClick(clickHandler1);
  btn1.attachLongPressStart(longPressStartHandler1);
  btn1.attachLongPressStop(longPressStopHandler1);

  btn2.attachClick(clickHandler2);
  btn2.attachLongPressStart(longPressStartHandler2);
  btn2.attachLongPressStop(longPressStopHandler2);

  if (rallyMode) {
    bleKeyboard.print('3');
  }
}

void loop() {
  btn1.tick();
  btn2.tick();

  if (holdCount == 2) {
    ESP.restart();
  }

  if (button1Held && ((millis() - button1Held) % volumeFrequency) == 0) {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
      Serial.println("KEY_MEDIA_VOLUME_DOWN");
    }
  }

  if (button2Held && ((millis() - button2Held) % volumeFrequency) == 0) {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
      Serial.println("KEY_MEDIA_VOLUME_UP");
    }
  }
}
