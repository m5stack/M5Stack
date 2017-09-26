#include <M5Stack.h>

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
            delay(5);
        }
    }
    m5.lcd.println("");
}

bool gpio_test_flg = 0;
void GPIO_test() {
    // uint8_t gpio_table[] = {23,19,18,3,16,21,2,12,15,26,1,17,22,5,13,0,34};
    uint8_t gpio_table[] = {12,2,21,16,3,18,19,23,15,0,13,5,22,17,1,26,25};
    
    // while(1) 
    {
        for (int i = 0; i<=sizeof(gpio_table) / sizeof(gpio_table[0]); i++) {
            pinMode(gpio_table[i], OUTPUT);
        }
        for(int i=0; i<=sizeof(gpio_table)/sizeof(gpio_table[0]); i++) {
            digitalWrite(gpio_table[i], 1);
            delay(50);
            digitalWrite(gpio_table[i], 0);
            delay(50);
            digitalWrite(gpio_table[i], 1);
            delay(50);
            digitalWrite(gpio_table[i], 0);
            delay(50);
        }
    }
}

void adc_test() {
    int count = 10;
    pinMode(35, INPUT);
    pinMode(36, INPUT);
    pinMode(34, INPUT);
    m5.Lcd.fillScreen(BLACK);
    while(count--) {
        m5.Lcd.setCursor(0, 10);
        m5.Lcd.setTextColor(WHITE, BLACK);
        m5.Lcd.setTextSize(2);
        m5.Lcd.printf("ADC1:%d\r\nADC2:%d\r\nADC3:%d\r\n", analogRead(35), analogRead(36), analogRead(34));
        delay(500);
    }
}

unsigned long testLines(uint16_t color)
{
    unsigned long start, t;
    int x1, y1, x2, y2,
        w = M5.Lcd.width(),
        h = M5.Lcd.height();

    M5.Lcd.fillScreen(TFT_BLACK);

    x1 = y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);
    t = micros() - start; // fillScreen doesn't count against timing

    M5.Lcd.fillScreen(TFT_BLACK);

    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    M5.Lcd.fillScreen(TFT_BLACK);

    x1 = 0;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    M5.Lcd.fillScreen(TFT_BLACK);

    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
        M5.Lcd.drawLine(x1, y1, x2, y2, color);

    return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2)
{
    unsigned long start;
    int x, y, w = M5.Lcd.width(), h = M5.Lcd.height();

    M5.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (y = 0; y < h; y += 5)
        M5.Lcd.drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5)
        M5.Lcd.drawFastVLine(x, 0, h, color2);

    return micros() - start;
}

unsigned long testRects(uint16_t color)
{
    unsigned long start;
    int n, i, i2,
        cx = M5.Lcd.width() / 2,
        cy = M5.Lcd.height() / 2;

    M5.Lcd.fillScreen(TFT_BLACK);
    n = min(M5.Lcd.width(), M5.Lcd.height());
    start = micros();
    for (i = 2; i < n; i += 6)
    {
        i2 = i / 2;
        M5.Lcd.drawRect(cx - i2, cy - i2, i, i, color);
    }

    return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2)
{
    unsigned long start, t = 0;
    int n, i, i2,
        cx = M5.Lcd.width() / 2 - 1,
        cy = M5.Lcd.height() / 2 - 1;

    M5.Lcd.fillScreen(TFT_BLACK);
    n = min(M5.Lcd.width(), M5.Lcd.height());
    for (i = n - 1; i > 0; i -= 6)
    {
        i2 = i / 2;
        start = micros();
        M5.Lcd.fillRect(cx - i2, cy - i2, i, i, color1);
        t += micros() - start;
        // Outlines are not included in timing results
        M5.Lcd.drawRect(cx - i2, cy - i2, i, i, color2);
    }

    return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color)
{
    unsigned long start;
    int x, y, w = M5.Lcd.width(), h = M5.Lcd.height(), r2 = radius * 2;

    M5.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (x = radius; x < w; x += r2)
    {
        for (y = radius; y < h; y += r2)
        {
            M5.Lcd.fillCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color)
{
    unsigned long start;
    int x, y, r2 = radius * 2,
              w = M5.Lcd.width() + radius,
              h = M5.Lcd.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for (x = 0; x < w; x += r2)
    {
        for (y = 0; y < h; y += r2)
        {
            M5.Lcd.drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testTriangles()
{
    unsigned long start;
    int n, i, cx = M5.Lcd.width() / 2 - 1,
              cy = M5.Lcd.height() / 2 - 1;

    M5.Lcd.fillScreen(TFT_BLACK);
    n = min(cx, cy);
    start = micros();
    for (i = 0; i < n; i += 5)
    {
        M5.Lcd.drawTriangle(
            cx, cy - i,     // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            M5.Lcd.color565(0, 0, i));
    }

    return micros() - start;
}

unsigned long testFilledTriangles()
{
    unsigned long start, t = 0;
    int i, cx = M5.Lcd.width() / 2 - 1,
           cy = M5.Lcd.height() / 2 - 1;

    M5.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (i = min(cx, cy); i > 10; i -= 5)
    {
        start = micros();
        M5.Lcd.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                            M5.Lcd.color565(0, i, i));
        t += micros() - start;
        M5.Lcd.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                            M5.Lcd.color565(i, i, 0));
    }

    return t;
}

unsigned long testRoundRects()
{
    unsigned long start;
    int w, i, i2,
        cx = M5.Lcd.width() / 2 - 1,
        cy = M5.Lcd.height() / 2 - 1;

    M5.Lcd.fillScreen(TFT_BLACK);
    w = min(M5.Lcd.width(), M5.Lcd.height());
    start = micros();
    for (i = 0; i < w; i += 6)
    {
        i2 = i / 2;
        M5.Lcd.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, M5.Lcd.color565(i, 0, 0));
    }

    return micros() - start;
}

unsigned long testFilledRoundRects()
{
    unsigned long start;
    int i, i2,
        cx = M5.Lcd.width() / 2 - 1,
        cy = M5.Lcd.height() / 2 - 1;

    M5.Lcd.fillScreen(TFT_BLACK);
    start = micros();
    for (i = min(M5.Lcd.width(), M5.Lcd.height()); i > 20; i -= 6)
    {
        i2 = i / 2;
        M5.Lcd.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, M5.Lcd.color565(0, i, 0));
    }

    return micros() - start;
}

// the setup routine runs once when M5Stack starts up
void setup() {
    
    //gpio test 
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    if(digitalRead(BUTTON_A_PIN) == 0) {
        gpio_test_flg = 1;
    }

    if (gpio_test_flg) {
        GPIO_test();
    }

    // initialize the M5Stack object
    m5.begin();

    // dac test
    if (gpio_test_flg)
    {
        adc_test();
    }

    m5.startupLogo();

    // Lcd display
    m5.lcd.setBrightness(100);
    m5.Lcd.fillScreen(BLACK);
    m5.Lcd.setCursor(10, 10);
    m5.Lcd.setTextColor(WHITE);
    m5.Lcd.setTextSize(1);
    m5.Lcd.printf("Display Test!");
    delay(300);

    m5.Lcd.fillScreen(WHITE);
    delay(150);
    m5.Lcd.fillScreen(RED);
    delay(150);
    m5.Lcd.fillScreen(GREEN);
    delay(150);
    m5.Lcd.fillScreen(BLUE);
    delay(150);
    m5.Lcd.fillScreen(BLACK);
    delay(150);

    // draw graphic
    yield();
    Serial.print(F("Lines                    "));
    yield();
    Serial.println(testLines(TFT_CYAN));
    //total+=testLines(TFT_CYAN);
    //delay(500);

    yield();
    Serial.print(F("Horiz/Vert Lines         "));
    yield();
    Serial.println(testFastLines(TFT_RED, TFT_BLUE));
    //total+=testFastLines(TFT_RED, TFT_BLUE);
    //delay(500);

    yield();
    Serial.print(F("Rectangles (outline)     "));
    yield();
    Serial.println(testRects(TFT_GREEN));
    //total+=testRects(TFT_GREEN);
    //delay(500);

    yield();
    Serial.print(F("Rectangles (filled)      "));
    yield();
    Serial.println(testFilledRects(TFT_YELLOW, TFT_MAGENTA));
    //total+=testFilledRects(TFT_YELLOW, TFT_MAGENTA);
    //delay(500);

    yield();
    Serial.print(F("Circles (filled)         "));
    yield();
    Serial.println(testFilledCircles(10, TFT_MAGENTA));
    //total+= testFilledCircles(10, TFT_MAGENTA);

    yield();
    Serial.print(F("Circles (outline)        "));
    yield();
    Serial.println(testCircles(10, TFT_WHITE));
    //total+=testCircles(10, TFT_WHITE);
    //delay(500);

    yield();
    Serial.print(F("Triangles (outline)      "));
    yield();
    Serial.println(testTriangles());
    //total+=testTriangles();
    //delay(500);

    yield();
    Serial.print(F("Triangles (filled)       "));
    yield();
    Serial.println(testFilledTriangles());
    //total += testFilledTriangles();
    //delay(500);

    yield();
    Serial.print(F("Rounded rects (outline)  "));
    yield();
    Serial.println(testRoundRects());
    //total+=testRoundRects();
    //delay(500);

    yield();
    Serial.print(F("Rounded rects (filled)   "));
    yield();
    Serial.println(testFilledRoundRects());
    //total+=testFilledRoundRects();
    //delay(500);

    yield();
    Serial.println(F("Done!"));
    yield();

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
    // digitalWrite(TFT_CS, 1);
    listDir(SD, "/", 0);
    writeFile(SD, "/hello.txt", "Hello world");
    readFile(SD, "/hello.txt");

    //Button test
    m5.Lcd.println();
    m5.Lcd.println();
    m5.Lcd.print("buttons Test:");
    m5.Lcd.setTextColor(RED);
}

// the loop routine runs over and over again forever
void loop(){
    buttons_test();
    m5.update();
}
