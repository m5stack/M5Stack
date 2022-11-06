/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: TF Card. TF卡
* Date: 2022/3/25
*******************************************************************************
    In this example, we will detect the existence of a file and perform read and
    write operations on it
    在这个示例中,我们将会检测某文件是否存在,并进行读写文件操作
*/

#include <M5Stack.h>

void setup() {
    M5.begin();
    if (!SD.begin()) {  // Initialize the SD card. 初始化SD卡
        M5.Lcd.println(
            "Card failed, or not present");  // Print a message if the SD card
                                             // initialization fails or if the
                                             // SD card does not exist
                                             // 如果SD卡初始化失败或者SD卡不存在，则打印消息
        while (1)
            ;
    }
    M5.Lcd.println("TF card initialized.");
    if (SD.exists("/hello.txt")) {  // Check if the "/hello.txt" file
                                    // exists.查看是否存在"/hello.txt"文件
        M5.Lcd.println("hello.txt exists.");
    } else {
        M5.Lcd.println("hello.txt doesn't exist.");
    }
    M5.Lcd.println("Creating hello.txt");
    File myFile = SD.open("/hello.txt",
                          FILE_WRITE);  // Create a new file "/hello.txt".
                                        // 创建一个新文件"/hello.txt"
    if (myFile) {  // If the file is open, then write to it.
                   // 如果文件打开,则进行写入操作
        M5.Lcd.println("Writing to test.txt...");
        myFile.println("SD test.");
        myFile.close();  // Close the file. 关闭文件
        M5.Lcd.println("done.");
    } else {
        M5.Lcd.println("error opening test.txt");
    }
    delay(500);
    myFile = SD.open("/hello.txt",
                     FILE_READ);  // Open the file "/hello.txt" in read mode.
                                  // 以读取模式打开文件"/hello.txt"
    if (myFile) {
        M5.Lcd.println("/hello.txt Content:");
        // Read the data from the file and print it until the reading is
        // complete. 从文件里读取数据并打印到串口,直到读取完成.
        while (myFile.available()) {
            M5.Lcd.write(myFile.read());
        }
        myFile.close();
    } else {
        M5.Lcd.println("error opening /hello.txt");  // If the file is not open.
                                                     // 如果文件没有打开
    }
}

void loop() {
}