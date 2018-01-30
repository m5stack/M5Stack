# M5Stack Library

## Usage
### Installing the USB Driver
- [Download the SiLabs CP2104 Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)
(Note:If you cannot upload, please try the legacy v4 driver below. Note you will need to uninstall the v5 driver.And try to use the low band: 115200)

### Installing the ESP32 Arduino Core
- Using Arduino IDE
  + [Instructions for Windows](docs/arduino-ide/windows.md)
  + [Instructions for Mac](docs/arduino-ide/mac.md)
  + [Instructions for Debian/Ubuntu Linux](docs/arduino-ide/debian_ubuntu.md)
  + [Instructions for Fedora](docs/arduino-ide/fedora.md)
  + [Instructions for openSUSE](docs/arduino-ide/opensuse.md)
- [Using PlatformIO](docs/platformio.md)
- [Building with make](docs/make.md)
- [Using as ESP-IDF component](docs/esp-idf_component.md)

### Download Library

#### Using the Arduino IDE Library Manager

1. Choose ```Sketch``` -> ```Include Library``` -> ```Manage Libraries...```
2. Type ```m5stack``` into the search box.
3. Click the row to select the library.
4. Click the ```Install``` button to install the library.
5. Click "File-> Examples". Here are some test programs in "M5Stack->"

#### Using Git
```sh
cd ~/Documents/Arduino/libraries/
git clone https://github.com/m5stack/M5Stack.git
```

## API
See [API](https://github.com/m5stack/M5Stack/blob/master/src/M5Stack.h#L19).

## Examples
See [examples](examples) folder.


## Hardware
[Schematic](https://github.com/m5stack/M5-hardware/blob/master/M5_Core_SCH(20171206).pdf).
### Pinout
Peripheral Devices | ESP32 
---|---
ILI9341 RST | GPIO33 
ILI9341 DC | GPIO27 
ILI9341 CS | GPIO14
ILI9341 MOSI | GPIO23
ILI9341 CLK | GPIO18
ILI9341 LIGHT | GPIO32
TFCARD MOSI | GPIO23
TFCARD MISO | GPIO19
TFCARD CLK | GPIO18
TFCARD CS | GPIO4
BUTTON A | GPIO39
BUTTON B | GPIO38
BUTTON C | GPIO37
SPEAKER | GPIO25
MPU9250 SDA | GPIO21
MPU9250 SCL | GPIO22
GOVER SDA | GPIO21
GOVER SCL | GPIO22

### M-BUS
![image](http://olcunuug8.bkt.clouddn.com/M-BUS.jpg?imageView/2/w/500/q/100)
