# Motorcycle-BLE-Media-Controller

A Bluetooth Low Energy (BLE) single-button media controller for motorcycles, compatible with iPhone and other devices, enabling easy control of music playback and volume.

## Requirements

This project is developed using Arduino Studio and tested on an ESP32 microcontroller. The following libraries are required:

1. [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard) (with a minor modification, see below)
2. [OneButton](https://github.com/mathertel/OneButton)
3. [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) (required by ESP32-BLE-Keyboard)

## Installing Libraries

### OneButton

1. Open Arduino Studio.
2. Go to `Tools > Manage Libraries...` or press `Ctrl + Shift + I`.
3. Search for `OneButton` in the search bar.
4. Click on the `OneButton` library by Matthias Hertel and click on `Install`.

### NimBLE-Arduino

1. Open Arduino Studio.
2. Go to `Tools > Manage Libraries...` or press `Ctrl + Shift + I`.
3. Search for `NimBLE-Arduino` in the search bar.
4. Click on the `NimBLE-Arduino` library by h2zero and click on `Install`.

### ESP32-BLE-Keyboard

1. Download the library from the [GitHub repository](https://github.com/T-vK/ESP32-BLE-Keyboard).
2. Unzip the downloaded file.
3. Rename the unzipped folder to `ESP32_BLE_Keyboard`.
4. Move the folder to the Arduino `libraries` folder (usually located in your `Documents` folder under `Arduino/libraries`).
5. Apply the modification mentioned below.

## Modification to ESP32-BLE-Keyboard Library

In order to prevent iOS from showing a border around the phone (because it thinks it's a keypad instead of a keyboard), a modification to the ESP32-BLE-Keyboard library is required. Inside the `BLEKeyboard.cpp` file, find the following line:

```cpp
USAGE(1),           0x06,          // USAGE (Keyboard)
```

Change it to:

```cpp
USAGE(1),           0x07,          // USAGE (Keyboard)
```

## Pin code

In `BLEKeyboard.cpp`, below `BLEDevice::init(deviceName);` add the following line:

```cpp
NimBLEDevice::setSecurityPasskey(111111);
NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY);
```

`111111` is the pin code, you can change it to whatever you want, but it has to be 6 digits.

## Enable NimBLE

In `BLEKeyboard.h`, uncomment the following line:

```cpp
// #define USE_NIMBLE
```

## Implemented Events

The single button on the motorcycle handlebar can trigger the following events:

* Single click: Play/Pause music
* Double click: Next track
* Triple click: Previous track
* Long press: Volume up

## Setup and Usage

1. Install the required libraries mentioned above.
2. Apply the modification to the ESP32-BLE-Keyboard library (if not already applied).
3. Upload the main sketch to the ESP32 microcontroller.
4. Pair the ESP32 with your iPhone or other devices.
5. Use the single button to control media playback and volume.

## License

This project is open-source and available under the MIT License.
