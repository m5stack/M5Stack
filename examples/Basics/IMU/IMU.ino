/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: MPU6886 example.  惯性传感器
* Date: 2022/9/8
*******************************************************************************
*/

#include <M5Stack.h>

float accX = 0.0F;  // Define variables for storing inertial sensor data
float accY = 0.0F;  //定义存储惯性传感器相关数据的相关变量
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temp = 0.0F;

/* After M5Core is started or reset
the program in the setUp () function will be run, and this part will only be run
once. 在 M5Core
启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
    M5.begin();        // Init M5Core.  初始化 M5Core
    M5.Power.begin();  // Init Power module.  初始化电源

    M5.IMU.Init();  // Init IMU sensor.  初始化惯性传感器

    M5.Lcd.fillScreen(BLACK);  // Set the screen background color to black.
                               // 设置屏幕背景色为黑色
    M5.Lcd.setTextColor(
        GREEN, BLACK);  // Sets the foreground color and background color of the
                        // displayed text.  设置显示文本的前景颜色和背景颜色
    M5.Lcd.setTextSize(2);  // Set the font size.  设置字体大小
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
    // Stores the triaxial gyroscope data of the inertial sensor to the relevant
    // variable 将惯性传感器的三轴陀螺仪数据存储至相关变量
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.IMU.getAccelData(
        &accX, &accY,
        &accZ);  // Stores the triaxial accelerometer.  存储三轴加速度计数据
    M5.IMU.getAhrsData(
        &pitch, &roll,
        &yaw);  // Stores the inertial sensor attitude.  存储惯性传感器的姿态
    M5.IMU.getTempData(&temp);  // Stores the inertial sensor temperature to
                                // temp.  存储惯性传感器的温度

    /* The M5Core screen is 320x240 pixels, starting at the top left corner of
    the screen (0,0). gyroscope output related M5Stack屏幕像素为
    320x240,以屏幕左上角为原点 (0,0)*/
    // gyroscope output related.  陀螺仪输出相关
    M5.Lcd.setCursor(
        0, 20);  // Move the cursor position to (x,y).  移动光标位置到(x,y)处
    M5.Lcd.printf("gyroX,  gyroY, gyroZ");  // Screen printingformatted string.
                                            // 输出格式化字符串
    M5.Lcd.setCursor(0, 42);
    M5.Lcd.printf("%6.2f %6.2f%6.2f o/s", gyroX, gyroY, gyroZ);

    // Accelerometer output is related
    //加速度计输出相关
    M5.Lcd.setCursor(0, 70);
    M5.Lcd.printf("accX,   accY,  accZ");
    M5.Lcd.setCursor(0, 92);
    M5.Lcd.printf("%5.2f  %5.2f  %5.2f G", accX, accY, accZ);

    // Pose output is related
    //姿态输出相关
    M5.Lcd.setCursor(0, 120);
    M5.Lcd.printf("pitch,  roll,  yaw");
    M5.Lcd.setCursor(0, 142);
    M5.Lcd.printf("%5.2f  %5.2f  %5.2f deg", pitch, roll, yaw);

    // Inertial sensor temperature output related
    //惯性传感器温度输出相关
    M5.Lcd.setCursor(0, 175);
    M5.Lcd.printf("Temperature : %.2f C", temp);

    delay(10);  // Delay 10ms  //延迟10ms
}