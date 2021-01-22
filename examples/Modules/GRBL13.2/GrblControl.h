#include <M5Stack.h>

class GRBL
{

    private:
        void SendByte(byte b);
        void SendBytes(uint8_t *data, size_t size);
    public:

        GRBL(int addr=0x70);
        void Init();
        void Init(uint32_t x_step, uint32_t y_step,uint32_t z_step,uint32_t acc);
        int addr;
        String mode;
        void Gcode(char *c);
        void UnLock();
        void ReadClean();
        void WaitIdle();
        void SetMotor(int x=0, int y=0, int z=0, int speed=300);
        void SetMode(String mode);
        String ReadLine();
        String ReadStatus();
        bool ReadIdle();
        bool InLock();
};
