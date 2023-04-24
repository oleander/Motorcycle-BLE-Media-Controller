#include <BleKeyboard.h>
#include <Arduino.h>
#include <OneButton.h>
#include <NimBLEDevice.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

BleKeyboard bleKeyboard("uBMW", "701", 100);

#define BUTTON_PIN 33
#define WIFI_SSID "uBMW"
#define WIFI_PWD "11111111"
#define OTA_ACTIVATION_TIME 10000
#define OTA_INACTIVITY_TIMEOUT 600000 // 10 minutes in milliseconds

OneButton btn = OneButton(
  BUTTON_PIN,
  true,
  true
);

unsigned long otaActivationStartTime = 0;
unsigned long lastOtaActivity;
bool otaActive = false;
bool shouldRestart = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Device started");

  btn.setClickTicks(400);
  btn.setDebounceTicks(100);

  bleKeyboard.begin();

  btn.attachClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }

    if (otaActive) {
      shouldRestart = true;
    }

    Serial.println("Button single click");
  });

  btn.attachDoubleClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    }

    if (otaActive) {
      shouldRestart = true;
    }

    Serial.println("Button double click");
  });

  btn.attachMultiClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }

    if (otaActive) {
      shouldRestart = true;
    }

    Serial.println("Button multi click");
  });

  btn.attachLongPressStart([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }

    otaActivationStartTime = millis();

    Serial.println("Button long press start");
  });

  btn.attachLongPressStop([]() {
    if (!bleKeyboard.isConnected() && (millis() - otaActivationStartTime) >= OTA_ACTIVATION_TIME) {
      enableOTA();
    }

    Serial.println("Button long press stop");
  });
}

void enableOTA() {
  if (!otaActive) {
    Serial.println("Enabling OTA");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PWD);
    ArduinoOTA.begin();
    otaActive = true;
    lastOtaActivity = millis();
  }
}

void loop() {
  btn.tick();

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
