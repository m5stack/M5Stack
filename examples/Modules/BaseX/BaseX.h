/*
 * @Author: Sorzn
 * @Date: 2019-12-12 14:33:50
 * @LastEditTime: 2019-12-13 15:47:59
 * @Description: M5Stack project
 * @FilePath: /M5Stack/examples/Modules/BaseX/BaseX.h
 */

#ifndef __BASE_X_H__
#define __BASE_X_H__

#include "Arduino.h"

#define BASE_X_ADDR (0x22)

#define BASE_X_SERVO_ANGLE_ADDR (0x00)
#define BASE_X_SERVO_PULSE_ADDR (0x10)
#define BASE_X_PWM_DUTY_ADDR    (0x20)
#define BASE_X_ENCODER_ADDR      (0x30)
#define BASE_X_SPEED_ADDR       (0x40)

/*
   |  0  |       1     |      2     |     3      |    4, 5, 6, 7  |          8         |     9    |    10   |    11   |     12      |          
   | mod |  position-p | position-i | position-d | position-point | position-max-speed |  speed-p | speed-i | speed-d | speed-point | 
*/
#define BASE_X_CONFIG_ADDR      (0x50)

#define NORMAL_MODE     (0x00)
#define POSITION_MODE   (0x01)
#define SPEED_MODE      (0x02)


class BASE_X
{
    public:
        void SetMode(uint8_t pos, uint8_t mode);

        int32_t GetEncoderValue(uint8_t pos);
        void SetEncoderValue(uint8_t pos, int32_t encode);
        
        void SetMotorSpeed(uint8_t pos, int8_t duty);
        int8_t GetMotorSpeed(uint8_t pos);
        
        int8_t GetMotorSpeed20MS(uint8_t pos);
        
        void SetPositionPID(uint8_t pos, uint8_t kp, uint8_t ki, uint8_t kd);
        void SetPositionPoint(uint8_t pos, int32_t position_point);
        void SetPostionPIDMaxSpeed(uint8_t pos, uint8_t max_pwm);

        void SetSpeedPID(uint8_t pos, uint8_t kp, uint8_t ki, uint8_t kd);
        void SetSpeedPoint(uint8_t pos, int8_t speed_point);

        void SetServoAngle(uint8_t pos, uint8_t angle);
        void SetServoPulseWidth(uint8_t pos, uint16_t width);
    
    private:
        uint8_t CheckPos(uint8_t pos);
};

#endif