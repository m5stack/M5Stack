# M5Stack Library

[English](../README.md) | [中文](getting_started_cn.md) | 日本語

M5Stack Core のライブラリへようこそ

## 1. 始めよう

#### M5Stackの開発環境を構築しよう

*1.MacOS環境*

https://docs.m5stack.com/#/ja/quick_start/m5core/m5stack_core_get_started_Arduino_MacOS

*2.Windows環境*

https://docs.m5stack.com/#/ja/quick_start/m5core/m5stack_core_get_started_Arduino_Windows


## 2. プログラム例

https://github.com/m5stack/M5Stack/tree/master/examples

## 3. 関数リファレンス（API仕様）

https://github.com/m5stack/M5Stack/blob/master/src/M5Stack.h#L19

https://docs.m5stack.com/#/ja/api


## 4. ハードウェア仕様

#### I/Oインタフェイス 

*M5Coreは複数のバリエーションがあります。違いは [こちら](https://github.com/m5stack/M5-Schematic/blob/master/Core/hardware_difference_between_cores_ja.md)*

**LCD & SDカード**

*LCD 解像度: 320x240*

<table>
 <tr><td>ESP32 Chip</td><td>GPIO23</td><td>GPIO19</td><td>GPIO18</td><td>GPIO14</td><td>GPIO27</td><td>GPIO33</td><td>GPIO32</td><td>GPIO4</td></tr>
 <tr><td>ILI9341</td><td>/</td><td>MISO</td><td>CLK</td><td>CS</td><td>DC</td><td>RST</td><td>BL</td><td> </td></tr>
 <tr><td>SD Card</td><td>MOSI</td><td>MISO</td><td>CLK</td><td> </td><td> </td><td> </td><td> </td><td>CS</td></tr>

</table>

**ボタン & スピーカー**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO39</td><td>GPIO38</td><td>GPIO37</td><td>GPIO25</td></tr>
 <tr><td>ボタン</td><td>Aボタン</td><td>Bボタン</td><td>Cボタン</td><td> </td></tr>
 <tr><td>スピーカー</td><td> </td><td> </td><td> </td><td>Speaker Pin</td></tr>
</table>

**GROVE A**

<table>
 <tr><td>ESP32 Chip</td><td>GPIO22</td><td>GPIO21</td></tr>
 <tr><td>GROVE A</td><td>SCL</td><td>SDA</td></tr>
</table>


### M-バス
![image](M-BUS.jpg)

## 5. 素晴らしい応用例

* [M5Stack-SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater) - カスタマイズ可能な M5Stack用メニュー - アプリはSDカードから読み込めます
  card

* [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)  - いくつかのチップをサポートする TFT ライブラリ（ESP8266/ ESP32向け）


* [M5Widgets](https://github.com/Kongduino/M5Widgets) -  M5Stack用ウィジット

* [M5StackSAM](https://github.com/tomsuch/M5StackSAM) - シンプルな M5Stack用アプリケーションメニュー

* [cfGUI](https://github.com/JF002/cfGUI) -  M5Stack (ESP32)用のシンプルなGUIライブラリ

* [GUIslice](https://github.com/ImpulseAdventure/GUIslice) - 組み込みディスプレイ向けの 軽量GUIフレームワーク

* [M5ez](https://github.com/ropg/M5ez) - M5Stackで簡単にプログラムをするインターフェースビルダーライブラリ


* [M5Stack MultiApp Advanced](https://github.com/botofancalin/M5Stack-MultiApp-Advanced) -  すべてのアプリを再起動や再起動なしで実行できる、マルチアプリファームウェア


* [M5Stack ESP32 Oscilloscope](https://github.com/botofancalin/M5Stack-ESP32-Oscilloscope) -  ESP32 M5Stackをベースにした多機能オシロスコープ

* [M5Stack-Avatar](https://github.com/meganetaaan/m5stack-avatar) -アバターを表示できるライブラリ

* [M5Stack_CrackScreen](https://github.com/nomolk/M5Stack_CrackScreen) - M5Stackの画面が割れた時を模擬します。

* [M5_Shuttle_Run](https://github.com/n0bisuke/M5_Shuttle_Run) - M5Stackでシャトルランができます。

* [nixietubeM5](https://github.com/drayde/nixietubeM5) - M5Stackでニキシー管ディスプレイを再現します。

* [M5Stack_BTCTicker](https://github.com/dankelley2/M5Stack_BTCTicker) - M5Stack（ESP32）とCoindesk APIを併用した小さなBitcoinレート表示器

* [M5Stack_ETHPrice](https://github.com/donma/M5StackWifiSettingWithETHPrice) -  WiFiをつかって、Maicoinから ETH 価格を取得する設定例 

* [M5Stack-PacketMonitor](https://github.com/tobozo/M5Stack-PacketMonitor) - M5Stack ESP32 WiFiパケットモニター

* [M5-FFT](https://github.com/ElectroMagus/M5-FFT) - M5Stack 用FFTアナライザー（グラフィックイコライザ）

* [M5Stack_ESP32_radio](https://github.com/anton-b/M5Stack_ESP32_radio) - インターネットmp3ストリームを再生

* [mp3-player-m5stack](https://github.com/dsiberia9s/mp3-player-m5stack) - M5Stack用 MP3プレーヤ

* [ArduinoWiFiPhotoBackup](https://github.com/moononournation/ArduinoWiFiPhotoBackup) - M5STACK WiFi 写真バックアップデバイス

* [M5StackHIDCtrlAltDel](https://github.com/mhama/M5StackHIDCtrlAltDel) - M5StackからPCに Ctrl + Alt + Delを送信

* [M5Stack Markdown Web Server](https://github.com/PartsandCircuits/M5Stack-MarkdownWebServer) -SDからマークダウンとアイコンを読み込んでWebページを表示

* [M5Stack-Tetris](https://github.com/PartsandCircuits/M5Stack-Tetris) - テトリス for M5Stack (macsbugによる移植) - https://macsbug.wordpress.com/

* [M5Stack_FlappyBird_game](https://github.com/pcelli85/M5Stack_FlappyBird_game) - M5Stack FlappyBird ゲーム

* [M5Stack-SpaceShooter](https://github.com/PartsandCircuits/M5Stack-SpaceShooter) - スペースインベーダー for M5Stack

* [M5Stack-Pacman-JoyPSP](https://github.com/tobozo/M5Stack-Pacman-JoyPSP) - パックマン for M5Stack/PSP ジョイパッド、サウンド付き。

* [M5Stack-Thermal-Camera](https://github.com/hkoffer/M5Stack-Thermal-Camera-) - M5Stack サーモカメラ (AMG8833 サーモセンサ版）

* [M5Stack-3DPrintFiles](https://github.com/PartsandCircuits/M5Stack-3DPrintFiles) - M5Stack 3Dプリンタ印刷用モデルデータ 

* [truetype2gfx](https://github.com/ropg/truetype2gfx) -  TrueType フォントを Adafruit GFX形式に変換

* [m5stack-onscreen-keyboard](https://github.com/yellowelise/m5stack-onscreen-keyboard) - フルサイズ qwerty キーボード

#### ノート:

* シリアル通信用USBドライバの導入方法

  https://docs.m5stack.com/#/ja/related_documents/establish_serial_connection

* M5Stackライブラリの更新方法

  https://docs.m5stack.com/#/ja/related_documents/upgrade_m5stack_lib
