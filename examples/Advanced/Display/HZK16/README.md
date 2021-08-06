HZK16 is the GB2312 Chinese encoding format font, so it is normal that GbkStr is displayed as garbled in the src.h file
To display Chinese normally, notepad++ or other software (UTF-8 for Arduino IDE) should be used to open str.h with encoding GB2312
Modify the content in GbkStr to replace it with the character you want to display

HZK16为GB2312中文编码格式字库, 故在 src.h 文件中 GbkStr 显示为乱码为正常现象
若要正常显示中文应使用notepad++或其它软件（Arduino IDE 为UTF-8）使用编码GB2312打开str.h
修改GbkStr里的内容即可更换为想要显示的字符