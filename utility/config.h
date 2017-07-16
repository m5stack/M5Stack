#ifndef CONFIG_H_
#define	CONFIG_H_

//screen
#define TFT_LED_PIN		32
#define TFT_DC_PIN    27
#define TFT_CS_PIN    14
#define TFT_MOSI_PIN  23
#define TFT_CLK_PIN   18
#define TFT_RST_PIN   33
#define TFT_MISO_PIN  0
//----------------------------

//auto back-light levels
#define BACKLIGHT_MIN 0
#define BACKLIGHT_MAX 255

//battery voltage monitor
#define BAT_PIN A6
#define NUM_LVL 4
#define BAT_LVL_CRITIC  3500
#define BAT_LVL_LOW		3550
#define BAT_LVL_MED		3700
#define BAT_LVL_FULL	3900

//SD card
#define TFCARD_CS_PIN       4

//screens back light
#define BACKLIGHT_PIN       5

//number of buttons
#define NUM_BTN         	3
//buttons ID
#define BTN_A               0
#define BTN_B               1
#define BTN_C               2
#define BUTTON_A            0
#define BUTTON_B            1
#define BUTTON_C            2
#define BUTTON_A_PIN        39
#define BUTTON_B_PIN        38
#define BUTTON_C_PIN        37

// BEEP PIN
#define BEEP_PIN            5
#define SPEAKER_PIN         25

// LED 
#define LED_PIN             13
#define LED_BUILTIN         13

#define USE_SERIAL 			Serial

#endif /* SETTINGS_C */
