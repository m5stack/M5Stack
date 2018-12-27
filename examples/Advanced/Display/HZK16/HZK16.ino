/* 
  在 src.h 文件中 GbkStr 显示为乱码为正常现象
  HZK16为GB2312中文编码格式字库, 需要显示中文建议使用notepad++或其它软件（Arduino IDE 为UTF-8）通过编码GB2312打开str.h
  即可修改为个人想要输出中文字符
*/

#include <M5Stack.h>
#include "display_ch.h"
#include "str.h"

DisplayCh displaych;
void setup() {
  
  M5.begin();
  
  displaych.loadHzk16();

  displaych.setTextColor(WHITE, BLACK);
  
  // Set text with red highlight color
  displaych.setHighlightColor(RED);
  
  // Set text size to 1
  displaych.setTextSize(1);
  displaych.setCursor(0,0,1);
  displaych.writeHzk(AscStr);
  // Print some ASCII text on the screen
  delay(1000);
  // Highlight the text
  displaych.highlight(true);
  displaych.setCursor(0,32);
  displaych.writeHzk(AscStr);
  delay(1000); 
  // Clear screen
  //M5.Lcd.clear();
  // Turn highlight off
  displaych.highlight(false);
  displaych.setCursor(0,70);
  // print some GBK text on the screen
  displaych.writeHzk(GbkStr);
  
  delay(1000);
  displaych.highlight(true);
  displaych.setCursor(0,90);
  displaych.writeHzk(GbkStr);
  
  delay(1000);
  displaych.fillScreen(BLACK);
 
  // Set text size to 2
  displaych.setTextSize(2);
  displaych.setCursor(0,0);
  displaych.highlight(false);
  displaych.writeHzk(AscStr);
  delay(1000);
  displaych.highlight(true);
  displaych.setCursor(0,70);
  displaych.writeHzk(AscStr);  
  delay(1000);
  
  //displaych.clear();
  displaych.highlight(false);
  displaych.setCursor(0,145);
  displaych.writeHzk(GbkStr);
  delay(1000);
  displaych.highlight(true);
  displaych.setCursor(0,195);
  displaych.writeHzk(GbkStr);

  
}

void loop() {
  // put your main code here, to run repeatedly:

}
