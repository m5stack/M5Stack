Please modify <m5stack path>/src/utility/In_eSPI_Setup.h, At the end of the file add "#define USE_M5_FONT_CREATOR"

if use PlatformIO, need add "board_build.partitions = no_ota.csv" in platformio.ini

if want get customize unicode, try use https://github.com/m5stack/FontCreator, create font file

  if add "#define USE_M5_FONT_CREATOR" and want to use standard gfx font, need modify font file
    #ifdef USE_M5_FONT_CREATOR
    0, 0,
    #endif
  like:

   ``` cpp
   const GFXfont FreeMono12pt7b PROGMEM = {
         (uint8_t  *)FreeMono12pt7bBitmaps,
         (GFXglyph *)FreeMono12pt7bGlyphs,
         0x20, 0x7E, 24,
         #ifdef USE_M5_FONT_CREATOR
         0, 0,
         #endif
         }
   ```





------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
zh:
如果编译过程中出现错误,请在<m5stack path>/src/utility/In_eSPI_Setup.h 中最后一行添加#define USE_M5_FONT_CREATOR

如果你使用platformIO,需要在platformio.ini中添加board_build.partitions = no_ota.csv

如果你想自定义unicode,请尝试使用https://github.com/m5stack/FontCreator 去创建相关字体文件

如果添加了“#define USE_M5_FONT_CREATOR”，并且想使用标准的gfx字体，需要修改CUF_24px.h文件


```cpp
#ifdef USE_M5_FONT_CREATOR
#define _unicode_24px_H_
const GFXfont FreeMono12pt7b PROGMEM = {
      (uint8_t  *)FreeMono12pt7bBitmaps,
      (GFXglyph *)FreeMono12pt7bGlyphs,
      0x20, 0x7E, 24,
    };

#endif
```



