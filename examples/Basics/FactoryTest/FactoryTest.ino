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
    m5.Lcd.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        m5.Lcd.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        m5.Lcd.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            m5.Lcd.print("  DIR : ");
            Serial.println(file.name());
            m5.Lcd.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            m5.Lcd.print("  FILE: ");
            Serial.print(file.name());
            m5.Lcd.print(file.name());
            Serial.print("  SIZE: ");
            m5.Lcd.print("  SIZE: ");
            Serial.println(file.size());
            m5.Lcd.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path) {
    Serial.printf("Reading file: %s\n", path);
    m5.Lcd.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        m5.Lcd.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    m5.Lcd.print("Read from file: ");
    while(file.available()){
        int ch = file.read();
        Serial.write(ch);
        m5.Lcd.write(ch);
    }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);
    m5.Lcd.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        m5.Lcd.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
        m5.Lcd.println("File written");
    } else {
        Serial.println("Write failed");
        m5.Lcd.println("Write failed");
    }
}

void buttons_test() {
    if(M5.BtnA.wasPressed()) {
        M5.Lcd.printf("A");
        Serial.printf("A");
    }
    if(M5.BtnB.wasPressed()) {
        M5.Lcd.printf("B");
        Serial.printf("B");
    } 
    if(M5.BtnC.wasPressed()) {
        M5.Lcd.printf("C");
        Serial.printf("C");
    } 
}

void wifi_test() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    m5.lcd.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    m5.lcd.println("scan done");
    if (n == 0) {
        m5.lcd.println("no networks found");
    } else {
        m5.lcd.print(n);
        m5.lcd.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            m5.lcd.print(i + 1);
            m5.lcd.print(": ");
            m5.lcd.print(WiFi.SSID(i));
            m5.lcd.print(" (");
            m5.lcd.print(WiFi.RSSI(i));
            m5.lcd.print(")");
            m5.lcd.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    m5.lcd.println("");
}

// the setup routine runs once when M5Stack starts up
void setup() {
    // initialize the M5Stack object
    m5.begin();
    m5.startupLogo();

    // Lcd display
    m5.lcd.setBrightness(100);
    m5.Lcd.setFont();
    m5.Lcd.fillScreen(BLACK);
    m5.Lcd.setCursor(10, 10);
    m5.Lcd.setTextColor(WHITE);
    m5.Lcd.setTextSize(1);
    m5.Lcd.printf("Display Test!");
    delay(800);

    m5.Lcd.fillScreen(WHITE);
    delay(200);
    m5.Lcd.fillScreen(RED);
    delay(200);
    m5.Lcd.fillScreen(GREEN);
    delay(200);
    m5.Lcd.fillScreen(BLUE);
    delay(200);
    m5.Lcd.fillScreen(BLACK);
    delay(200);

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
    delay(500);

    //rand draw 
    int i = 250;
    while(--i) {
        m5.Lcd.fillTriangle(random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(0xfffe));
    }
    for(int i=255; i>=0; i--) {
        m5.lcd.setBrightness(i);
        delay(2);
    }

    //wifi test
    m5.Lcd.setCursor(0, 10);
    m5.Lcd.fillScreen(BLACK);
    for(int i=0; i<200; i++) {
        m5.lcd.setBrightness(i);
        delay(2);
    }
    m5.Lcd.println("wifi test:");
    wifi_test();
    delay(2000);

    // TF card test
    m5.Lcd.fillScreen(BLACK);
    m5.Lcd.setCursor(0, 10);
    m5.Lcd.printf("TF card test:\r\n");
    listDir(SD, "/", 0);
    writeFile(SD, "/hello.txt", "Hello world");
    readFile(SD, "/hello.txt");

    //Button test
    m5.Lcd.println();
    m5.Lcd.println();
    m5.Lcd.print("buttons Test:");
    m5.Lcd.setTextColor(RED);

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
    buttons_test();
    m5.update();
}
