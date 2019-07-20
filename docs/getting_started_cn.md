# M5Stack 库

[English](../README.md) | 中文 | [日本語](getting_started_ja.md)

## 1. 上手指南

*1.For MacOS*

https://docs.m5stack.com/#/zh_CN/quick_start/m5core/m5stack_core_get_started_Arduino_MacOS

*2. For Windows*

https://docs.m5stack.com/#/zh_CN/quick_start/m5core/m5stack_core_get_started_Arduino_Windows


## 2. 例程

https://github.com/m5stack/M5Stack/tree/master/examples

## 3. API 参考

https://github.com/m5stack/M5Stack/blob/master/src/M5Stack.h#L19

https://docs.m5stack.com/#/zh_CN/api

## 4. H/W 参考

#### 管脚映射

*我们有几款不同配置的主控 M5Cores，这是他们之间的[对比](https://github.com/m5stack/M5-Schematic/blob/master/Core/hardware_difference_between_cores.md).*

**LCD & TF Card**

*LCD 分辨率: 320x240*

<table>
 <tr><td>ESP32 Chip</td><td>GPIO23</td><td>GPIO19</td><td>GPIO18</td><td>GPIO14</td><td>GPIO27</td><td>GPIO33</td><td>GPIO32</td><td>GPIO4</td></tr>
 <tr><td>ILI9341</td><td>/</td><td>MISO</td><td>CLK</td><td>CS</td><td>DC</td><td>RST</td><td>BL</td><td> </td></tr>
 <tr><td>TF Card</td><td>MOSI</td><td>MISO</td><td>CLK</td><td> </td><td> </td><td> </td><td> </td><td>CS</td></tr>

</table>

**Button & Speaker**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO39</td><td>GPIO38</td><td>GPIO37</td><td>GPIO25</td></tr>
 <tr><td>Button Pin</td><td>BUTTON A</td><td>BUTTON B</td><td>BUTTON C</td></tr>
 <tr><td>Speaker</td><td> </td><td> </td><td> </td><td>Speaker Pin</td></tr>
</table>

**GROVE A**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO22</td><td>GPIO21</td></tr>
 <tr><td>GROVE A</td><td>SCL</td><td>SDA</td></tr>
</table>


### M-BUS
![image](../docs/M-BUS.jpg)

#### Note:

* 如何安装 CP210x 串口驱动

  https://docs.m5stack.com/#/zh_CN/related_documents/establish_serial_connection

* 如何升级 M5Stack 库

  https://docs.m5stack.com/#/zh_CN/related_documents/upgrade_m5stack_lib
