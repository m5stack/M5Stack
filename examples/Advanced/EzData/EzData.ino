/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: EzData.
* Date: 2021/10/17
*******************************************************************************
*/
#include "M5Stack.h"
#include "M5_EzData.h"

// Configure the name and password of the connected wifi and your token.
// 配置所连接wifi的名称、密码以及你的token
const char* ssid     = "Explore-F";
const char* password = "xingchentansuo123";
const char* token    = "";

void setup() {
    M5.begin();  // Initialize M5Stack
    M5.Power.begin();
    if (setupWifi(ssid, password)) {  // Connect to wifi.  连接到wifi
        M5.Lcd.printf("Success connecting to %s\n", ssid);
    } else {
        M5.Lcd.printf("Connecting to %s failed\n", ssid);
    }
}

void loop() {
    // Save the data 20 to the top of the testData topic queue.  保存数据20至
    // testData 队列首位
    if (setData(token, "testData", 20)) {
        M5.Lcd.printf("Success sending data to the topic\n");
    } else {
        M5.Lcd.print("Fail to save data\n");
    }
    delay(5000);

    // Save 3 data in sequence to the first place of testList. 依次保存3个数据至
    // testList首位
    for (int i = 0; i < 3; i++) {
        if (addToList(token, "testList", i)) {
            M5.Lcd.printf("Success sending %d to the list\n", i);
        } else {
            M5.Lcd.print("Fail to save data\n");
        }
        delay(100);
    }
    delay(5000);

    // Get a piece of data from a topic and store the value in result.  从一个
    // topic中获取一个数据,并将值存储在 result
    int result = 0;
    if (getData(token, "testData", result)) {
        M5.Lcd.printf("Success get data %d\n", result);
    } else {
        M5.Lcd.print("Fail to get data\n");
    }
    delay(5000);

    // Get a set of data from a list and store the values in the Array array.
    // 从一个 list中获取一组数据,并将值存储在 Array数组中
    int Array[3] = {};
    if (getData(token, "testList", Array, 0, 3)) {
        M5.Lcd.print("Success get list\n");
        for (int i = 0; i < 3; i++) {
            M5.Lcd.printf("Array[%d]=%d,", i, Array[i]);
        }
        M5.Lcd.println("");
    } else {
        M5.Lcd.println("Fail to get data");
    }
    delay(5000);

    // Remove data
    if (removeData(token, "testData"))
        M5.Lcd.printf("Success remove data\n");
    else
        M5.Lcd.println("Fail to remove data");

    if (removeData(token, "testList"))
        M5.Lcd.printf("Success remove data from the list\n");
    else
        M5.Lcd.println("Fail to remove data");
    delay(5000);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
}