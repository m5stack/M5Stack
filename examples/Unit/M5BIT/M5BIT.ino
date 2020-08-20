/*
    Description: Use UART communication to control the LED matrix on microbit.
*/
#include <M5Stack.h>
#include <M5StackUpdater.h>

#define WIDTH 320
#define HEIGHT 240
#define BLOCK_SIZE  40
#define UNIT_WIDTH  5
#define UNIT_HEIGHT 5
#define UNIT_SIZE 25
#define GETX(i) ((i) % (5))
#define GETY(i) ((i) / (5))
int world[UNIT_SIZE];
int i;
  
void setup() {
  M5.begin();
  M5.Power.begin();
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0){
    Serial.println("Will load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(35, 220);  
  M5.Lcd.println("  <       *       >");  
    for (i = 0; i < UNIT_SIZE; i++) {
    world[i] = 0;
  }
  i = UNIT_SIZE / 2;
}

void loop() {
      M5.update();
      int x = GETX(i) + 1;
      int y = GETY(i);
      if (world[i] > 0) M5.Lcd.fillRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, LIGHTGREY);
      else M5.Lcd.fillRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, BLUE);
      if (M5.BtnC.wasPressed()) {
         if (world[i] > 0) M5.Lcd.fillRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, WHITE);
         else M5.Lcd.fillRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
         ++i;
         if (i >= UNIT_SIZE) i=0;
      }
      if (M5.BtnA.wasPressed()) {
         if (world[i] > 0) M5.Lcd.fillRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, WHITE);
         else M5.Lcd.fillRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
         --i;
         if (i < 0 ) i=UNIT_SIZE -1;
      }
      if (M5.BtnB.wasPressed()) {
        if (world[i] > 0) world[i]=0;
        else world[i]=1;
        Serial2.print(world[i]);
        Serial2.print(GETX(i));
        Serial2.println(GETY(i));
      }
}
