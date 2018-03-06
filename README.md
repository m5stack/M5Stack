# M5Stack Library

## Usage
### Installing the USB Driver
- [Download the SiLabs CP2104 Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) but 
do NOT use the Windows 10 Universal (v10.1.1) driver! If you happen to download the Universal version, you may need to manually revert to 6.7.1 
on a regular basis as Windows will repeatedly use the newer, incorrect driver after each Windows update.

(Note:If you cannot upload, please try the legacy v4 driver below. Note you will need to uninstall the v5 driver.And try to use the low band: 115200)
- [Download the Silabs CP210X v10 Driver Universal binary for win 10 in case of rebooting problems while flashing procedure] (https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip )
No need of reinstaling any drivers if were installed before, they should be updated "on the fly"

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

#### Using Git (most environments)
```sh
cd ~/Documents/Arduino/libraries/
git clone https://github.com/m5stack/M5Stack.git
```

#### Using Git (Windows)
```sh
c:
cd %USERPROFILE%\documents\libraries
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
GROVE SDA | GPIO21
GROVE SCL | GPIO22


### LoRa module
Peripheral Devices | ESP32 | RA-02 | Note
---|---|---|---
MOSI     | GPIO23 | MOSI | shared with TFCARD
MISO     | GPIO19 | MISO | shared with TFCARD
SCK      | GPIO18 | SCK | shared with TFCARD
RFM95_CS | GPIO5 | NSS | pull up before M5.Begin()
RFM95_RST | GPIO36 | RST | incorrectly labeled GPIO26 in some places
RFM95_INT | GPIO26 | DIO0 | incorrectly labeled GPIO36 in some places


### M-BUS
![image](http://olcunuug8.bkt.clouddn.com/M-BUS.jpg?imageView/2/w/500/q/100)
