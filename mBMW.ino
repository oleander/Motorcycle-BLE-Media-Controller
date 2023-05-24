#include <Arduino.h>
#include <ArduinoOTA.h>
#include <BleKeyboard.h>
#include <NimBLEDevice.h>
#include <OneButton.h>
#include <WiFi.h>

BleKeyboard bleKeyboard("uBMW", "701", 100);

#define BUTTON_PIN_1           27
#define BUTTON_PIN_2           26
#define WIFI_SSID              "uBMW"
#define WIFI_PWD               "11111111"
#define OTA_ACTIVATION_TIME    10000
#define OTA_INACTIVITY_TIMEOUT 600000 // 10 minutes in milliseconds

OneButton btn1 = OneButton(BUTTON_PIN_1, true, true);
OneButton btn2 = OneButton(BUTTON_PIN_2, true, true);

unsigned long otaActivationStartTime = 0;
unsigned long lastOtaActivity;
bool otaActive     = false;
bool shouldRestart = false;

void clickHandler() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
  }

  if (otaActive) {
    shouldRestart = true;
  }

  Serial.println("Button single click");
}

void doubleClickHandler() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
  }

  if (otaActive) {
    shouldRestart = true;
  }

  Serial.println("Button double click");
}

void tripleClickHandler() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
  }

  if (otaActive) {
    shouldRestart = true;
  }

  Serial.println("Button triple click");
}

void longPressStartHandler() {
  if (bleKeyboard.isConnected()) {
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
  }

  Serial.println("Button long press start");
}

void longPressStopHandler() {
  if (!bleKeyboard.isConnected() && (millis() - otaActivationStartTime) >= OTA_ACTIVATION_TIME) {
    enableOTA();
  }

  Serial.println("Button long press stop");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Device started");

  btn1.setClickTicks(400);
  btn1.setDebounceTicks(100);
  btn2.setClickTicks(400);
  btn2.setDebounceTicks(100);

  bleKeyboard.begin();

  btn1.attachClick(clickHandler);
  btn1.attachDoubleClick(doubleClickHandler);
  btn1.attachMultiClick(tripleClickHandler);
  btn1.attachLongPressStart(longPressStartHandler);
  btn1.attachLongPressStop(longPressStopHandler);

  btn2.attachClick(clickHandler);
  btn2.attachDoubleClick(doubleClickHandler);
  btn2.attachMultiClick(tripleClickHandler);
  btn2.attachLongPressStart(longPressStartHandler);
  btn2.attachLongPressStop(longPressStopHandler);
}

void enableOTA() {
  if (!otaActive) {
    Serial.println("Enabling OTA");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PWD);
    ArduinoOTA.begin();
    otaActive       = true;
    lastOtaActivity = millis();
  }
}

void loop() {
  btn1.tick();
  btn2.tick();

  if (otaActive) {
    ArduinoOTA.handle();

    if (shouldRestart) {
      ESP.restart();
    }

    if (millis() - lastOtaActivity >= OTA_INACTIVITY_TIMEOUT) {
      Serial.println("OTA inactive for 10 minutes, restarting...");
      ESP.restart();
    }
  }
}
