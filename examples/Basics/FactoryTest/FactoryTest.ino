#include <M5Stack.h>

//TF card test
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
}

// the setup routine runs once when M5Stack starts up
void setup(){
  // initialize the M5Stack object
  m5.begin();
  randomSeed(100);

  // Beep
  for(int i=0; i<200; i++) {
    digitalWrite(BEEP_PIN, !digitalRead(BEEP_PIN));
    delay(1);
  }

  //LED ON
  m5.ledOn();
  delay(200);
  m5.ledOff();

  // TF card test
  listDir(SD, "/", 0);
  writeFile(SD, "/hello.txt", "Hello ");
  readFile(SD, "/hello.txt");

  // lcd display
  m5.lcd.fillScreen(WHITE);
  delay(100);
  m5.lcd.fillScreen(RED);
  delay(100);
  m5.lcd.fillScreen(GREEN);
  delay(100);
  m5.lcd.fillScreen(BLUE);
  delay(100);
  m5.lcd.fillScreen(BLACK);
  delay(100);

  // text print
  m5.lcd.setFont();
  m5.lcd.fillScreen(BLACK);
  m5.lcd.setCursor(10, 10);
  m5.lcd.setTextColor(WHITE);
  m5.lcd.setTextSize(1);
  m5.lcd.printf("Display Test!");

  // draw graphic
  delay(100);
  m5.lcd.drawRect(100, 100, 50, 50, BLUE);
  delay(100);
  m5.lcd.fillRect(100, 100, 50, 50, BLUE);
  delay(100);
  m5.lcd.drawCircle(100, 100, 50, RED);
  delay(100);
  m5.lcd.fillCircle(100, 100, 50, RED);
  delay(100);
  m5.lcd.drawTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  delay(100);
  m5.lcd.fillTriangle(30, 30, 180, 100, 80, 150, YELLOW);

  m5.lcd.buttonSet(BTN_A, "A");
  m5.lcd.buttonSet(BTN_B, "B");
  m5.lcd.buttonSet(BTN_C, "C");
  
/*
  m5.lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
  m5.lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
  m5.lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  m5.lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
*/
}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  m5.lcd.fillTriangle(random(219), random(175), random(219), random(175), random(219), random(175), random(0xfffe));
  m5.loop();
}
