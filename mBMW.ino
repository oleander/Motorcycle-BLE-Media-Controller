#include <BleKeyboard.h>
#include <Arduino.h>
#include <OneButton.h>

BleKeyboard bleKeyboard("uBMW");

#define BUTTON_PIN 33

OneButton btn = OneButton(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);

void setup() {
  bleKeyboard.begin();

  // Single click
  btn.attachClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }
  });

  // Double click
  btn.attachDoubleClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    }
  });

  // Tripple click
  btn.attachMultiClick([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }
  });

  // Hold
  btn.attachLongPressStart([]() {
    if (bleKeyboard.isConnected()) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }
  });
}

void loop() {
  btn.tick();
}
