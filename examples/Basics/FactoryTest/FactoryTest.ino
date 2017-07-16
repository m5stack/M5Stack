#include <M5Stack.h>

void GPIO_test() {
    uint8_t gpio_table[] = {23,19,18,3,16,21,2,12,15,35,36,26,1,17,22,5,13,0,34};
    for(int i=0; i<sizeof(gpio_table)/sizeof(gpio_table[0]); i++) {
        pinMode(gpio_table[i], OUTPUT);
    }

    while(1) {
        for(int i=0; i<sizeof(gpio_table)/sizeof(gpio_table[0]); i++) {
            digitalWrite(gpio_table[i], 1);
            delay(1);
            digitalWrite(gpio_table[i], 0);
        }
    }
}

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
void setup() {
  // initialize the M5Stack object
//   GPIO_test();
  m5.begin();
  randomSeed(100);

  // Beep
  for(int i=0; i<200; i++) {
    digitalWrite(BEEP_PIN, !digitalRead(BEEP_PIN));
    delay(1);
  }

  // TF card test
  listDir(SD, "/", 0);
  writeFile(SD, "/hello.txt", "Hello ");
  readFile(SD, "/hello.txt");

  // Lcd display
  m5.Lcd.fillScreen(WHITE);
  delay(100);
  m5.Lcd.fillScreen(RED);
  delay(100);
  m5.Lcd.fillScreen(GREEN);
  delay(100);
  m5.Lcd.fillScreen(BLUE);
  delay(100);
  m5.Lcd.fillScreen(BLACK);
  delay(100);

  // text print
  m5.Lcd.setFont();
  m5.Lcd.fillScreen(BLACK);
  m5.Lcd.setCursor(10, 10);
  m5.Lcd.setTextColor(WHITE);
  m5.Lcd.setTextSize(1);
  m5.Lcd.printf("Display Test!");

  // draw graphic
  delay(100);
  m5.Lcd.drawRect(100, 100, 50, 50, BLUE);
  delay(100);
  m5.Lcd.fillRect(100, 100, 50, 50, BLUE);
  delay(100);
  m5.Lcd.drawCircle(100, 100, 50, RED);
  delay(100);
  m5.Lcd.fillCircle(100, 100, 50, RED);
  delay(100);
  m5.Lcd.drawTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  delay(100);
  m5.Lcd.fillTriangle(30, 30, 180, 100, 80, 150, YELLOW);

//   m5.Lcd.buttonSet(BTN_A, "A");
//   m5.Lcd.buttonSet(BTN_B, "B");
//   m5.Lcd.buttonSet(BTN_C, "C");
  
/*
  m5.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
  m5.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
  m5.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  m5.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
*/
}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  m5.Lcd.fillTriangle(random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(0xfffe));
  m5.update();
}
