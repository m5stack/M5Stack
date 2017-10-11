#ifndef i2c_MPU9250_h
#define i2c_MPU9250_h

#include "i2c.h"
#include "i2c_Sensor.h"


/** ######################################################################

Driver for the MPU9250-Sensor

 9D-IMU 3.7mA, 8µA sleep

 GYRO XYZ: [3.9 .. 8000 Hz], ±250, ±500, ±1000, and ±2000°/sec and integrated 16-bit ADCs
 8µA, 3.2mA active

 ACCE XYZ: ±2g, ±4g, ±8g and ±16g and integrated 16-bit ADCs
 8µA, 450µA

 MAGN XYZ: ±4800μT, 14 bit (0.6μT/LSB) or 16 bit (15μT/LSB)
 ?µA, 280μA @8Hz

 DMP: Digital Motion Processing

///
########################################################################  */

class MPU9250 : public i2cSensor
{

private:

    /** ######### Register-Map ################################################################# */
    static const uint8_t    I2C_ADDRESS 	                =(0x68);

//MPU9250 Register map
    static const uint8_t    REG_SELF_TEST_X_GYRO            =(0x00);
    static const uint8_t    REG_SELF_TEST_Y_GYRO            =(0x01);
    static const uint8_t    REG_SELF_TEST_Z_GYRO            =(0x02);
    static const uint8_t    REG_SELF_TEST_X_ACCEL           =(0x0D);
    static const uint8_t    REG_SELF_TEST_Y_ACCEL           =(0x0E);
    static const uint8_t    REG_SELF_TEST_Z_ACCEL           =(0x0F);
    static const uint8_t    REG_XG_OFFSET_H                 =(0x13);
    static const uint8_t    REG_XG_OFFSET_L                 =(0x14);
    static const uint8_t    REG_YG_OFFSET_H                 =(0x15);
    static const uint8_t    REG_YG_OFFSET_L                 =(0x16);
    static const uint8_t    REG_ZG_OFFSET_H                 =(0x17);
    static const uint8_t    REG_ZG_OFFSET_L                 =(0x18);
    static const uint8_t    REG_SMPLRT_DIV                  =(0x19);
    inline const uint8_t        HZ_TO_DIV(const uint16_t hz)
    {
        return ((1000/hz) - 1);
    };

    static const uint8_t    REG_CONFIG                      =(0x1A);
    static const uint8_t        MSK_FIFO_MODE               =(0x40);
    static const uint8_t        MSK_EXT_SYNC_SET            =(0x38);
    static const uint8_t        MSK_DLPF_CFG                =(0x07);

    static const uint8_t    REG_GYRO_CONFIG                 =(0x1B);
    static const uint8_t        MSK_XGYRO_CTEN              =(0x80);
    static const uint8_t        MSK_YGYRO_CTEN              =(0x40);
    static const uint8_t        MSK_ZGYRO_CTEN              =(0x20);
    static const uint8_t        MSK_GYRO_FS_SEL             =(0x18);
    static const uint8_t            VAL_GYRO_FS_0250        =(0x00);
    static const uint8_t            VAL_GYRO_FS_0500        =(0x01);
    static const uint8_t            VAL_GYRO_FS_1000        =(0x02);
    static const uint8_t            VAL_GYRO_FS_2000        =(0x03);
    static const uint8_t        MSK_FCHOICE_B               =(0x03);

    static const uint8_t    REG_ACCEL_CONFIG                =(0x1C);
    static const uint8_t        MSK_AX_ST_EN                =(0x80);
    static const uint8_t        MSK_AY_ST_EN                =(0x40);
    static const uint8_t        MSK_AZ_ST_EN                =(0x20);
    static const uint8_t        MSK_ACCEL_FS_SEL            =(0x18);
    static const uint8_t            VAL_ACCEL_FS_02         =(0x00);
    static const uint8_t            VAL_ACCEL_FS_04         =(0x01);
    static const uint8_t            VAL_ACCEL_FS_08         =(0x02);
    static const uint8_t            VAL_ACCEL_FS_16         =(0x03);

    static const uint8_t    REG_ACCEL_CONFIG2               =(0x1D);
    static const uint8_t        MSK_ACCEL_FCHOICE_B         =(0xC0);
    static const uint8_t        MSK_A_DLPF_CFG              =(0x03);

    static const uint8_t    REG_LP_ACCEL_ODR                =(0x1E);
    static const uint8_t        MSK_LPOSC_CLKSEL            =(0x0F);

    static const uint8_t    REG_WOM_THR                     =(0x1F);

    static const uint8_t    REG_FIFO_EN                     =(0x23);
    static const uint8_t        MSK_TEMP_FIFO_EN            =(0x80);
    static const uint8_t        MSK_GYRO_XOUT               =(0x40);
    static const uint8_t        MSK_GYRO_YOUT               =(0x20);
    static const uint8_t        MSK_GYRO_ZOUT               =(0x10);
    static const uint8_t        MSK_ACCEL                   =(0x08);
    static const uint8_t        MSK_SLV2                    =(0x04);
    static const uint8_t        MSK_SLV1                    =(0x02);
    static const uint8_t        MSK_SLV0                    =(0x01);

    static const uint8_t    REG_I2C_MST_CTRL                =(0x24);
    static const uint8_t        MSK_MULT_MST_EN             =(0x80);
    static const uint8_t        MSK_WAIT_FOR_ES             =(0x40);
    static const uint8_t        MSK_SLV_3_FIFO_EN           =(0x20);
    static const uint8_t        MSK_I2C_MST_P_NSR           =(0x10);
    static const uint8_t        MSK_I2C_MST_CLK             =(0x0F);

    static const uint8_t    REG_I2C_SLV0_ADDR               =(0x25);
    static const uint8_t        MSK_I2C_SLV0_RNW            =(0x80);
    static const uint8_t        MSK_I2C_ID_0                =(0x7F);

    static const uint8_t    REG_I2C_SLV0_REG                =(0x26);
    static const uint8_t    REG_I2C_SLV0_CTRL               =(0x27);
    static const uint8_t        MSK_I2C_SLV0_EN             =(0x80);
    static const uint8_t        MSK_I2C_SLV0_BYTE_SW        =(0x40);
    static const uint8_t        MSK_I2C_SLV0_REG_DIS        =(0x20);
    static const uint8_t        MSK_I2C_SLV0_GRP            =(0x10);
    static const uint8_t        MSK_I2C_SLV0_LENG           =(0x0F);

// [without SLV0 to SLV4]

    static const uint8_t    REG_I2C_MST_STATUS              =(0x36);
    static const uint8_t        MSK_PASS_THROUGH            =(0x80);
    static const uint8_t        MSK_I2C_SLV4_DONE           =(0x40);
    static const uint8_t        MSK_I2C_LOST_ARB            =(0x20);
    static const uint8_t        MSK_I2C_SLV4_NACK           =(0x10);
    static const uint8_t        MSK_I2C_SLV3_NACK           =(0x08);
    static const uint8_t        MSK_I2C_SLV2_NACK           =(0x04);
    static const uint8_t        MSK_I2C_SLV1_NACK           =(0x02);
    static const uint8_t        MSK_I2C_SLV0_NACK           =(0x01);

    static const uint8_t    REG_INT_PIN_CFG                 =(0x37);
    static const uint8_t        MSK_ACTL                    =(0x80);
    static const uint8_t        MSK_OPEN                    =(0x40);
    static const uint8_t        MSK_LATCH_INT_EN            =(0x20);
    static const uint8_t        MSK_INT_ANYRD_2CLEAR        =(0x10);
    static const uint8_t        MSK_ACTL_FSYNC              =(0x08);
    static const uint8_t        MSK_FSYNC_INT_MODE_EN       =(0x04);
    static const uint8_t        MSK_BYPASS_EN               =(0x02);

    static const uint8_t    REG_INT_ENABLE                  =(0x38);
    static const uint8_t        MSK_WOM_EN                  =(0x40);
    static const uint8_t        MSK_FIFO_OFLOW_EN           =(0x10);
    static const uint8_t        MSK_FSYNC_INT_EN            =(0x08);
    static const uint8_t        MSK_RAW_RDY_EN              =(0x01);

    static const uint8_t    REG_INT_STATUS                  =(0x3A);
    static const uint8_t        MSK_WOM_INT                 =(0x40);
    static const uint8_t        MSK_FIFO_OFLOW_INT          =(0x10);
    static const uint8_t        MSK_FSYNC_INT               =(0x08);
    static const uint8_t        MSK_RAW_DATA_RDY_INT        =(0x01);

    static const uint8_t    REG_ACCEL_XOUT_H                =(0x3B);
    static const uint8_t    REG_ACCEL_XOUT_L                =(0x3C);
    static const uint8_t    REG_ACCEL_YOUT_H                =(0x3D);
    static const uint8_t    REG_ACCEL_YOUT_L                =(0x3E);
    static const uint8_t    REG_ACCEL_ZOUT_H                =(0x3F);
    static const uint8_t    REG_ACCEL_ZOUT_L                =(0x40);
    static const uint8_t    REG_TEMP_OUT_H                  =(0x41);
    static const uint8_t    REG_TEMP_OUT_L                  =(0x42);
    static const uint8_t    REG_GYRO_XOUT_H                 =(0x43);
    static const uint8_t    REG_GYRO_XOUT_L                 =(0x44);
    static const uint8_t    REG_GYRO_YOUT_H                 =(0x45);
    static const uint8_t    REG_GYRO_YOUT_L                 =(0x46);
    static const uint8_t    REG_GYRO_ZOUT_H                 =(0x47);
    static const uint8_t    REG_GYRO_ZOUT_L                 =(0x48);

    static const uint8_t    REG_EXT_SENS_DATA_00            =(0x49);
    static const uint8_t    REG_EXT_SENS_DATA_01            =(0x4A);
    static const uint8_t    REG_EXT_SENS_DATA_02            =(0x4B);
    static const uint8_t    REG_EXT_SENS_DATA_03            =(0x4C);
    static const uint8_t    REG_EXT_SENS_DATA_04            =(0x4D);
    static const uint8_t    REG_EXT_SENS_DATA_05            =(0x4E);
    static const uint8_t    REG_EXT_SENS_DATA_06            =(0x4F);
    static const uint8_t    REG_EXT_SENS_DATA_07            =(0x50);
    static const uint8_t    REG_EXT_SENS_DATA_08            =(0x51);
    static const uint8_t    REG_EXT_SENS_DATA_09            =(0x52);
    static const uint8_t    REG_EXT_SENS_DATA_10            =(0x53);
    static const uint8_t    REG_EXT_SENS_DATA_11            =(0x54);
    static const uint8_t    REG_EXT_SENS_DATA_12            =(0x55);
    static const uint8_t    REG_EXT_SENS_DATA_13            =(0x56);
    static const uint8_t    REG_EXT_SENS_DATA_14            =(0x57);
    static const uint8_t    REG_EXT_SENS_DATA_15            =(0x58);
    static const uint8_t    REG_EXT_SENS_DATA_16            =(0x59);
    static const uint8_t    REG_EXT_SENS_DATA_17            =(0x5A);
    static const uint8_t    REG_EXT_SENS_DATA_18            =(0x5B);
    static const uint8_t    REG_EXT_SENS_DATA_19            =(0x5C);
    static const uint8_t    REG_EXT_SENS_DATA_20            =(0x5D);
    static const uint8_t    REG_EXT_SENS_DATA_21            =(0x5E);
    static const uint8_t    REG_EXT_SENS_DATA_22            =(0x5F);
    static const uint8_t    REG_EXT_SENS_DATA_23            =(0x60);

    static const uint8_t    REG_I2C_SLV0_DO                 =(0x63);
    static const uint8_t    REG_I2C_SLV1_DO                 =(0x64);
    static const uint8_t    REG_I2C_SLV2_DO                 =(0x65);
    static const uint8_t    REG_I2C_SLV3_DO                 =(0x66);

    static const uint8_t    REG_I2C_MST_DELAY_CTRL          =(0x67);
    static const uint8_t        MSK_DELAY_ES_SHADOW         =(0x80);
    static const uint8_t        MSK_I2C_SLV4_DLY_EN         =(0x10);
    static const uint8_t        MSK_I2C_SLV3_DLY_EN         =(0x08);
    static const uint8_t        MSK_I2C_SLV2_DLY_EN         =(0x04);
    static const uint8_t        MSK_I2C_SLV1_DLY_EN         =(0x02);
    static const uint8_t        MSK_I2C_SLV0_DLY_EN         =(0x01);

    static const uint8_t    REG_SIGNAL_PATH_RESET           =(0x68);
    static const uint8_t        MSK_GYRO_RST                =(0x04);
    static const uint8_t        MSK_ACCEL_RST               =(0x02);
    static const uint8_t    MSK_TEMP_RST                    =(0x01);

    static const uint8_t    REG_MOT_DETECT_CTRL             =(0x69);
    static const uint8_t        MSK_ACCEL_INTEL_EN          =(0x80);
    static const uint8_t        MSK_ACCEL_INTEL_MODE        =(0x40);

    static const uint8_t    REG_USER_CTRL                   =(0x6A);
    static const uint8_t        MSK_FIFO_EN                 =(0x40);
    static const uint8_t        MSK_I2C_MST_EN              =(0x20);
    static const uint8_t        MSK_I2C_IF_DIS              =(0x10);
    static const uint8_t        MSK_FIFO_RST                =(0x04);
    static const uint8_t        MSK_I2C_MST_RST             =(0x02);
    static const uint8_t        MSK_SIG_COND_RST            =(0x01);

    static const uint8_t    REG_PWR_MGMT_1                  =(0x6B);
    static const uint8_t        MSK_H_RESET                 =(0x80);
    static const uint8_t        MSK_SLEEP                   =(0x40);
    static const uint8_t        MSK_CYCLE                   =(0x20);
    static const uint8_t        MSK_GYRO_STANDBY_CYCLE      =(0x10);
    static const uint8_t        MSK_PD_PTAT                 =(0x08);
    static const uint8_t        MSK_CLKSEL                  =(0x07);

    static const uint8_t    REG_PWR_MGMT_2                  =(0x6C);
    static const uint8_t        MSK_DISABLE_XA              =(0x20);
    static const uint8_t        MSK_DISABLE_YA              =(0x10);
    static const uint8_t        MSK_DISABLE_ZA              =(0x08);
    static const uint8_t        MSK_DISABLE_XG              =(0x04);
    static const uint8_t        MSK_DISABLE_YG              =(0x02);
    static const uint8_t        MSK_DISABLE_ZG              =(0x01);
    static const uint8_t        MSK_DISABLE_XYZA            =(0x38);
    static const uint8_t        MSK_DISABLE_XYZG            =(0x07);

    static const uint8_t    REG_FIFO_COUNTH                 =(0x72);
    static const uint8_t    REG_FIFO_COUNTL                 =(0x73);
    static const uint8_t    REG_FIFO_R_W                    =(0x74);
    static const uint8_t    REG_WHO_AM_I                    =(0x75);
    static const uint8_t        VAL_WHOAMI_6500             =(0x70);
    static const uint8_t        VAL_WHOAMI_9250             =(0x71);

    static const uint8_t    REG_XA_OFFSET_H                 =(0x77);
    static const uint8_t    REG_XA_OFFSET_L                 =(0x78);
    static const uint8_t    REG_YA_OFFSET_H                 =(0x7A);
    static const uint8_t    REG_YA_OFFSET_L                 =(0x7B);
    static const uint8_t    REG_ZA_OFFSET_H                 =(0x7D);
    static const uint8_t    REG_ZA_OFFSET_L                 =(0x7E);

//reset values
    static const uint8_t    WHOAMI_RESET_VAL                =(0x71);
    static const uint8_t    POWER_MANAGMENT_1_RESET_VAL     =(0x01);
    static const uint8_t    DEFAULT_RESET_VALUE             =(0x00);
    static const uint8_t    WHOAMI_DEFAULT_VAL              =(0x68);

    /**< TODO: Switch to separate lib for the AK8963 */

//Magnetometer register maps
    static const uint8_t    REG_MAG_WIA                     =(0x00);
    static const uint8_t    REG_MAG_INFO                    =(0x01);
    static const uint8_t    REG_MAG_ST1                     =(0x02);
    static const uint8_t    REG_MAG_XOUT_L                  =(0x03);
    static const uint8_t    REG_MAG_XOUT_H                  =(0x04);
    static const uint8_t    REG_MAG_YOUT_L                  =(0x05);
    static const uint8_t    REG_MAG_YOUT_H                  =(0x06);
    static const uint8_t    REG_MAG_ZOUT_L                  =(0x07);
    static const uint8_t    REG_MAG_ZOUT_H                  =(0x08);
    static const uint8_t    REG_MAG_ST2                     =(0x09);
    static const uint8_t    REG_MAG_CNTL                    =(0x0A);
    static const uint8_t    REG_MAG_RSV                     =(0x0B); //reserved mystery meat
    static const uint8_t    REG_MAG_ASTC                    =(0x0C);
    static const uint8_t    REG_MAG_TS1                     =(0x0D);
    static const uint8_t    REG_MAG_TS2                     =(0x0E);
    static const uint8_t    REG_MAG_I2CDIS                  =(0x0F);
    static const uint8_t    REG_MAG_ASAX                    =(0x10);
    static const uint8_t    REG_MAG_ASAY                    =(0x11);
    static const uint8_t    REG_MAG_ASAZ                    =(0x12);
//Magnetometer register masks
    static const uint8_t    MPU9250_WIA                     =(0x48);
    static const uint8_t    MPU9250_                        =(0x00);

    float gSensitivity, aSensitivity;

    /** ######### function definition ################################################################# */


public:
    /**< TODO: do i need a constructor? */
    MPU9250(void): gSensitivity(1), aSensitivity(1)
    {
    };

    void setEnabled(const uint8_t enable = 1)
    {
        uint8_t _value;
        if (enable) _value = 0;
        else        _value = 255;
        i2c.setRegister(I2C_ADDRESS, REG_PWR_MGMT_1, MSK_SLEEP, _value);
        i2c.setRegister(I2C_ADDRESS, REG_PWR_MGMT_2, B00111111, _value); // enable XYZ of Gyr & Acc
    };

    /**< takes 0..3 for 250, 500, 1000, 2000 dps */
    inline void setGSensitivity(const uint8_t gScaleRange = VAL_GYRO_FS_0500)
    {
        if (gScaleRange<4) i2c.setRegister(I2C_ADDRESS, REG_GYRO_CONFIG, MSK_GYRO_FS_SEL, gScaleRange<<3);

        if      (gScaleRange==VAL_GYRO_FS_0250) gSensitivity=131.0;
        else if (gScaleRange==VAL_GYRO_FS_0500) gSensitivity=65.5;
        else if (gScaleRange==VAL_GYRO_FS_1000) gSensitivity=32.8;
        else if (gScaleRange==VAL_GYRO_FS_2000) gSensitivity=16.4;
    };

    /**< takes 0..3 for 2, 4, 8 and 16g */
    inline void setASensitivity(const uint8_t aScaleRange = VAL_ACCEL_FS_04)
    {
        if (aScaleRange<4) i2c.setRegister(I2C_ADDRESS, REG_ACCEL_CONFIG, MSK_ACCEL_FS_SEL, aScaleRange<<3);

        if      (aScaleRange==VAL_ACCEL_FS_02) aSensitivity=16384;
        else if (aScaleRange==VAL_ACCEL_FS_04) aSensitivity= 8192;
        else if (aScaleRange==VAL_ACCEL_FS_08) aSensitivity= 4096;
        else if (aScaleRange==VAL_ACCEL_FS_16) aSensitivity= 2048;
    };

    /**< round numbers are: 1 kHz, 500 Hz, 200, 125, 100, 50, 25, 20, 10 ... */
    inline void setDatarate(const uint16_t hzFreq=100)
    {
        /**< with clockratedivider for samplerate 1kHz (Configure FChoice) */
        i2c.writeByte(I2C_ADDRESS, REG_SMPLRT_DIV, (uint8_t) HZ_TO_DIV(hzFreq));
    };

    inline void setBandwidth(const uint16_t hzFreq=100)
    {
        uint8_t dlpf;
        if      (hzFreq>184)    dlpf   = 0; // 460 Hz Acc, FS= 1 kHz // 250 Hz Gyro, 4000 Hz Temp, FS=8kHz
        else if (hzFreq>92)     dlpf   = 1; // 184 Hz Gyro/Acc, 4000 Hz Temp, FS=1kHz
        else if (hzFreq>41)     dlpf   = 2; // 92 Hz Gyro/Acc, 4000 Hz Temp, FS=1kHz
        else if (hzFreq>20)     dlpf   = 3; // 41 Hz Gyro/Acc, 42 Hz Temp, FS=1kHz
        else if (hzFreq>10)     dlpf   = 4; // 20 Hz Gyro/Acc, 20 Hz Temp, FS=1kHz
        else if (hzFreq>5)      dlpf   = 5; // 10 Hz Gyro/Acc, 10 Hz Temp, FS=1kHz
        else                    dlpf   = 6; //  5 Hz Gyro/Acc,  5 Hz Temp, FS=1kHz

        /**< Rework, page 15 - FS=1kHz --> Bandbreite einstellen */
        i2c.setRegister(I2C_ADDRESS, REG_CONFIG,      MSK_DLPF_CFG,   dlpf); // 0x1A DLPF p13
        i2c.setRegister(I2C_ADDRESS, REG_GYRO_CONFIG, MSK_FCHOICE_B,  0); // 0x1B gFChoice

        i2c.setRegister(I2C_ADDRESS, REG_ACCEL_CONFIG2, MSK_A_DLPF_CFG,      dlpf);
        i2c.setRegister(I2C_ADDRESS, REG_ACCEL_CONFIG2, MSK_ACCEL_FCHOICE_B, 0); // aFChoice
    };

    inline void reset(void)
    {
        uint8_t rVector;
        rVector = MSK_GYRO_RST | MSK_ACCEL_RST | MSK_TEMP_RST;
        i2c.setRegister(I2C_ADDRESS, REG_SIGNAL_PATH_RESET, rVector, rVector);           // Registers of Sensors
        i2c.setRegister(I2C_ADDRESS, REG_USER_CTRL, MSK_SIG_COND_RST, MSK_SIG_COND_RST); // Signal paths of Sensors
        i2c.setRegister(I2C_ADDRESS, REG_PWR_MGMT_1, MSK_H_RESET, MSK_H_RESET);          // internal Registers
    };


    inline uint8_t initialize(void)
    {
        if (i2c.probe(I2C_ADDRESS) == 0) return 0;

        // 0x19 Sample Rate Divider - standard = 0 (+1)
        reset();
        delay(20);

        //setEnabled(0);

        setBandwidth(20);
        setDatarate(100);
        setGSensitivity(VAL_GYRO_FS_0500);
        setASensitivity(VAL_ACCEL_FS_04);

        // Clocksource
        i2c.setRegister(I2C_ADDRESS, REG_PWR_MGMT_1, MSK_CLKSEL, 1); // should be VAL_CLOCK_PLL_XGYRO, is it?
        // PWR MGMT
        i2c.setRegister(I2C_ADDRESS, REG_PWR_MGMT_1, MSK_CYCLE | MSK_GYRO_STANDBY_CYCLE | MSK_PD_PTAT, 0); // Normal operation

        // INTs
        i2c.setRegister(I2C_ADDRESS, REG_INT_PIN_CFG, MSK_INT_ANYRD_2CLEAR,MSK_INT_ANYRD_2CLEAR); // Clear INT at read
        i2c.setRegister(I2C_ADDRESS, REG_INT_ENABLE , MSK_RAW_RDY_EN,      MSK_RAW_RDY_EN);     // INT: Raw Data Ready to read

        // I2C-Master
        i2c.setRegister(I2C_ADDRESS, REG_USER_CTRL  , MSK_I2C_MST_EN,      0);                  // disable I2C-Master
        i2c.setRegister(I2C_ADDRESS, REG_INT_PIN_CFG  , MSK_BYPASS_EN, MSK_BYPASS_EN);      // enable Bypass mode!

        setEnabled(1);

        uint8_t who =  i2c.readByte(I2C_ADDRESS, REG_WHO_AM_I);
        if      (who == VAL_WHOAMI_6500) return 2;
        else if (who == VAL_WHOAMI_9250) return 3;
        else                             return 1;
    };

    /**< check for new data, return 1 when Measurement is ready */
    inline uint8_t checkMeasurement(void)
    {
        if (i2c.readByte(I2C_ADDRESS,REG_INT_STATUS)&MSK_RAW_DATA_RDY_INT) return 1;
        else                                                               return 0;
    };

    /**<  if you started a measurement and want to actively wait for it to finish */
    inline uint8_t awaitMeasurement(void)
    {
        uint8_t _counter = 0;
        while(checkMeasurement()==0)
        {
            if(++_counter > 250) return 0; //Error out after max of 250ms for a read
            delay(1);
        }
        return 1; // Measurement finished
    };

    /**< TODO: separate RAW and Scaled GET */
    void getMeasurement(float xyz_AccTemGyr[])
    {
        uint8_t _data[14];
        i2c.read(I2C_ADDRESS,REG_ACCEL_XOUT_H, _data, 14);
        // RAW
        xyz_AccTemGyr[0] = int16_t(_data[0]<<8 | _data[1]); // ACC
        xyz_AccTemGyr[1] = int16_t(_data[2]<<8 | _data[3]);
        xyz_AccTemGyr[2] = int16_t(_data[4]<<8 | _data[5]);

        xyz_AccTemGyr[3] = int16_t(_data[6]<<8 | _data[7]); // TEMP

        xyz_AccTemGyr[4] = int16_t(_data[8]<<8 | _data[9]); // GYR
        xyz_AccTemGyr[5] = int16_t(_data[10]<<8| _data[11]);
        xyz_AccTemGyr[6] = int16_t(_data[12]<<8| _data[13]);

        // Scale
        xyz_AccTemGyr[3] = xyz_AccTemGyr[3] / 333.87 + 21; // TEMP_degC = ((TEMP_OUT – RoomTemp_Offset)/Temp_Sensitivity) + 21degC

        xyz_AccTemGyr[0] /= aSensitivity; // ACC
        xyz_AccTemGyr[1] /= aSensitivity;
        xyz_AccTemGyr[2] /= aSensitivity;

        xyz_AccTemGyr[4] /= gSensitivity; // GYR
        xyz_AccTemGyr[5] /= gSensitivity;
        xyz_AccTemGyr[6] /= gSensitivity;

    };

    /** ######### Digital Motion Processor ################################################################# */

};

/** ######### Preinstantiate Object ################################################################# */
/** < it's better when this is done by the user */
//PRESET preset = PRESET();

#endif




