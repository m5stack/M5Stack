/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/extio
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/extio
*
* describe: extio.
* date：2021/8/30
*******************************************************************************
  Please connect to Port A,Control the 8 extended IOs on the EXT.IO Unit to cycle high and low level changes.
  请连接端口 A,在EXT.IO Unit上控制8个扩展的IO来循环高电平和低电平的变化。
*/
#include <M5Stack.h>
#include "PCA9554.h"

PCA9554 ioCon1(0x27);  // Create an object at this address.  在这个地址上创建一个对象

uint8_t res;
void setup()
{
  M5.begin();
  M5.Power.begin();
  Wire.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(0, 90);
  M5.Lcd.print("UNIT_IO EXAMPLE\n");

  ioCon1.twiWrite(21, 22);  //Sets the I2C pin of the connection.  设置连接的I2C引脚
  delay(10);
  res = 1;
  ioCon1.twiRead(res);
  Serial.printf("res:%d\r\n", res);

  ioCon1.portMode(ALLOUTPUT); //Set the port as all output.  设置所有引脚为输出模式
}

void loop()
{
  for(int i=0;i<8;i++)  ioCon1.digitalWrite(i, LOW);
  delay(1000);
  for(int i=0;i<8;i++)  ioCon1.digitalWrite(i, HIGH);
  delay(1000);

  // write 0-7 HIGH.  设置0~7号引脚为高电平
  Serial.println(ioCon1.digitalWritePort(0xff));
  delay(200);

  // write 0-7 LOW.  设置0~7号引脚为低电平
  Serial.println(ioCon1.digitalWritePort(0x00));
  delay(200);

  // write Port, the same read
  for (byte i = 0; i < 8; i++) {
    ioCon1.digitalWritePort((1 << i));
    delay(200);
  }
}
