#include <BleKeyboard.h>
#include <Arduino.h>
#include <OneButton.h>
#include <NimBLEDevice.h>

BleKeyboard bleKeyboard("uBMW", "701", 100);

#define BUTTON_PIN 33

OneButton btn = OneButton(
  BUTTON_PIN,
  true,
  true
);

void setup() {
  Serial.begin(9600); // Initialize serial communication
  Serial.println("Device started");

  btn.setClickTicks(400);
  btn.setDebounceTicks(100);

  bleKeyboard.begin();

  btn.attachClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }
    Serial.println("Button single click");
  });

  btn.attachDoubleClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    }
    Serial.println("Button double click");
  });

  btn.attachMultiClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }
    Serial.println("Button multi click");
  });

  btn.attachLongPressStart([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }
    Serial.println("Button long press start");
  });

  btn.attachLongPressStop([]() {
    Serial.println("Button long press stop");
  });
}

void loop() {
  btn.tick();
}
