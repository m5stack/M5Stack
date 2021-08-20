/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：SPIFFS write & read.  闪存文件读写
* date：2021/8/4
******************************************************************************
*/

#include <M5Stack.h>
#include <SPIFFS.h>
String file_name = "/M5Stack/notes.txt"; //Sets the location and name of the file to be operated on.  设置被操作的文件位置和名称
bool SPIFFS_FORMAT = false; //Whether to initialize the SPIFFS.  是否初始化SPIFFS
//You don't need to format the flash file system every time you use it.
//无需每次使用闪存都进行格式化

void setup() {
  M5.begin(); //Init M5Core.  初始化 M5Core
  M5.Power.begin(); //Init power  初始化电源模块
  M5.Lcd.setTextSize(2);  //Set the font size to 2.  设置字号大小为2
  if(SPIFFS_FORMAT){
    M5.Lcd.println("SPIFFS format start..."); //output format String in LCD.  在屏幕上输出格式化字符串
    SPIFFS.format();    // Formatting SPIFFS.  格式化SPIFFS
    M5.Lcd.println("SPIFFS format finish");
  }
  if(SPIFFS.begin()){ // Start SPIFFS, return 1 on success.  启动闪存文件系统,若成功返回1
    M5.Lcd.println("SPIFFS Begin.");
    //Write operation
    File dataFile = SPIFFS.open(file_name, "w");  // Create a File object dafa File to write information to file_name in the SPIFFS.  建立File对象dafaFile用于向SPIFFS中的file_name写入信息
    dataFile.println("Hello IOT World."); // Writes string information and newlines to the dataFile.  向dataFile写入字符串信息并换行
    dataFile.close(); // Close the file when writing is complete.  完成写入后关闭文件
    M5.Lcd.println("Finished Writing data to SPIFFS");
    M5.Lcd.println("Press BtnA to read form SPIFFS");
  } else {
    M5.Lcd.println("SPIFFS Failed to Begin.\nYou need to Run SPIFFS_Add.ino first");
  }
}

void loop() {
  M5.update();  //Check whether the key is pressed.  检测按键是否按下
  if(M5.BtnA.isPressed()){ //If the button is pressed.  如果按键按下
    if (SPIFFS.exists(file_name)){  //Check whether the file_name file exists in the flash memory.  确认闪存中是否有file_name文件
      M5.Lcd.println("FOUND.");
      M5.Lcd.println(file_name);
    } else {
      M5.Lcd.println("NOT FOUND.");
      M5.Lcd.println(file_name);
    }
    File dataFile = SPIFFS.open(file_name, "r");   // Create aFile object dafaFile to read information to file_name in the SPIFFS.  建立File对象dafaFile用于向SPIFFS中的file_name读取信息
    for(int i=0; i<dataFile.size(); i++){ //Reads file contents and outputs file information through the M5.Lcd port monitor.  读取文件内容并且通过串口监视器输出文件信息
      M5.Lcd.print((char)dataFile.read());
    }
    dataFile.close(); //Close the file after reading the file.  完成文件读取后关闭文件
    delay(300);
  }
}