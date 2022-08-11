#include <hidboot.h>

int mou_px, mou_py, mou_button;

class MouseRptParser : public MouseReportParser {
   protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
};
void SendToBT(MOUSEINFO *mi) {
    byte Button = 0;

    if (mi->bmLeftButton)
        Button |= BIT0;
    else
        Button & !BIT0;

    if (mi->bmRightButton)
        Button |= BIT1;
    else
        Button & !BIT1;

    if (mi->bmMiddleButton)
        Button |= BIT2;
    else
        Button & !BIT2;

    mou_px     = mi->dX;
    mou_py     = mi->dY;
    mou_button = Button;
    // /*
    Serial.println("L Mouse Move");
    Serial.print("dx=");
    Serial.print(mi->dX, DEC);
    Serial.print(" dy=");
    Serial.println(mi->dY, DEC);
    Serial.println(Button, DEC);
    // */

    /*
    Serial.write(0x08);  //BYTE1
    Serial.write(0x00);  //BYTE2
    Serial.write(0xA1);  //BYTE3
    Serial.write(0x02);  //BYTE4
    Serial.write(Button); //BYTE5
    Serial.write(mi->dX); //BYTE6
    Serial.write(mi->dY);  //BYTE7
    Serial.write(0);  //BYTE8
    */
}
void MouseRptParser::OnMouseMove(MOUSEINFO *mi) {
    SendToBT(mi);
};
void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi) {
    // Serial.println("L Butt Up");
    SendToBT(mi);
};
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi) {
    // Serial.println("L Butt Dn");
    SendToBT(mi);
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi) {
    // Serial.println("R Butt Up");
    SendToBT(mi);
};
void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi) {
    // Serial.println("R Butt Dn");
    SendToBT(mi);
};
void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi) {
    // Serial.println("M Butt Up");
    SendToBT(mi);
};
void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi) {
    // Serial.println("M Butt Dn");
    SendToBT(mi);
};
