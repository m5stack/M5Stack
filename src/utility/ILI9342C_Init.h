
// // This is the command sequence that initialises the ILI9341 driver
// //
// // This setup information uses simple 8 bit SPI writecommand() and
// writedata() functions
// //
// // See ST7735_Setup.h file for an alternative format

{
    writecommand(0xC8);
    writedata(0xFF);
    writedata(0x93);
    writedata(0x42);

    writecommand(ILI9341_PWCTR1);
    writedata(0x12);
    writedata(0x12);

    writecommand(ILI9341_PWCTR2);
    writedata(0x03);

    writecommand(0xB0);
    writedata(0xE0);

    writecommand(0xF6);
    writedata(0x00);
    writedata(0x01);
    writedata(0x01);

    writecommand(ILI9341_MADCTL);  // Memory Access Control
#ifdef M5STACK
    writedata(TFT_MAD_MY | TFT_MAD_MV |
              TFT_MAD_COLOR_ORDER);  // Rotation 0 (portrait mode)
#else
    writedata(TFT_MAD_MX | TFT_MAD_COLOR_ORDER);  // Rotation 0 (portrait mode)
#endif

    writecommand(ILI9341_PIXFMT);
    writedata(0x55);

    writecommand(ILI9341_DFUNCTR);  // Display Function Control
    writedata(0x08);
    writedata(0x82);
    writedata(0x27);

    writecommand(ILI9341_GMCTRP1);  // Set Gamma
    writedata(0x00);
    writedata(0x0C);
    writedata(0x11);
    writedata(0x04);
    writedata(0x11);
    writedata(0x08);
    writedata(0x37);
    writedata(0x89);
    writedata(0x4C);
    writedata(0x06);
    writedata(0x0C);
    writedata(0x0A);
    writedata(0x2E);
    writedata(0x34);
    writedata(0x0F);

    writecommand(ILI9341_GMCTRN1);  // Set Gamma
    writedata(0x00);
    writedata(0x0B);
    writedata(0x11);
    writedata(0x05);
    writedata(0x13);
    writedata(0x09);
    writedata(0x33);
    writedata(0x67);
    writedata(0x48);
    writedata(0x07);
    writedata(0x0E);
    writedata(0x0B);
    writedata(0x2E);
    writedata(0x33);
    writedata(0x0F);

    writecommand(ILI9341_SLPOUT);  // Exit Sleep

    spi_end();
    delay(120);
    spi_begin();

    writecommand(ILI9341_DISPON);  // Display on
}