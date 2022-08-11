#include "M5Bala.h"

#include "M5Stack.h"

#define MPU9250_ID 0x71
#define MPU6050_ID 0x68

M5Bala::M5Bala() {
    wire = &Wire;
}

M5Bala::M5Bala(TwoWire &w) {
    wire = &w;
}

void M5Bala::begin() {
    // IMU
    imu_CalcInit();

    // Motor
    setMotor(0, 0);

    // PID param
    K1 = 60;
    K2 = 24;
    K5 = 0;
    K3 = 8.5;
    K4 = 5.2;
    // K5 = 8;

    loop_interval  = 0;
    left_offset    = 0;
    right_offset   = 0;
    forward_offset = 0;

    for (int i = 0; i < 128; i++) {
        imu_update();
    }
    pitch = imu_getAngleX();
}

uint8_t M5Bala::i2c_readByte(uint8_t address, uint8_t subAddress) {
    uint8_t data;
    M5.I2C.readByte(address, subAddress, &data);
    return data;  // Return data read from slave register
}

void M5Bala::setMotor(int16_t pwm0, int16_t pwm1) {
    // Value range
    int16_t m0 = constrain(pwm0, -255, 255);
    int16_t m1 = constrain(pwm1, -255, 255);

    // Dead zone
    if (((m0 > 0) && (m0 < DEAD_ZONE)) || ((m0 < 0) && (m0 > -DEAD_ZONE)))
        m0 = 0;
    if (((m1 > 0) && (m1 < DEAD_ZONE)) || ((m1 < 0) && (m1 > -DEAD_ZONE)))
        m1 = 0;

    // Same value
    static int16_t pre_m0, pre_m1;
    if ((m0 == pre_m0) && (m1 == pre_m1)) return;
    pre_m0 = m0;
    pre_m1 = m1;

    uint8_t i2c_out_buff[4];
    i2c_out_buff[0] = m0 & 0xff;
    i2c_out_buff[1] = (m0 >> 8) & 0xff;
    i2c_out_buff[2] = (m1 >> 0) & 0xff;
    i2c_out_buff[3] = (m1 >> 8) & 0xff;
    M5.I2C.writeBytes(M5GO_WHEEL_ADDR, MOTOR_CTRL_ADDR, i2c_out_buff, 4);
}

void M5Bala::readEncder() {
    static float _speed_input0 = 0, _speed_input1 = 0;
    int16_t rx_buf[2];
    M5.I2C.readBytes(M5GO_WHEEL_ADDR, ENCODER_ADDR, 4, (uint8_t *)rx_buf);

    // filter
    _speed_input0 *= 0.9;
    _speed_input0 += 0.1 * rx_buf[0];
    _speed_input1 *= 0.9;
    _speed_input1 += 0.1 * rx_buf[1];

    speed_input0 = constrain((int16_t)(-_speed_input0), -255, 255);
    speed_input1 = constrain((int16_t)(_speed_input1), -255, 255);
}

void M5Bala::PIDCompute() {
    static float last_angle;
    static int16_t last_wheel;
    float angle, angle_velocity;
    int16_t wheel, wheel_velocity;
    int16_t torque, speed_diff, speed_diff_adjust;

    angle          = pitch;
    angle_velocity = angle - last_angle;
    last_angle     = angle;

    wheel = (speed_input0 + speed_input1) /
            2; /* wheel = read_encoder()-profiler() */
    wheel_velocity = wheel - last_wheel;
    last_wheel     = wheel;

    torque = (angle_velocity * K1) + (angle * K2) + (wheel_velocity * K3) +
             (wheel * K4);

    speed_diff        = (speed_input0 - speed_input1);
    speed_diff_adjust = (K5 * speed_diff);

    pwm_out0 = torque - speed_diff_adjust;
    pwm_out1 = torque;
    pwm_out0 = constrain(pwm_out0, -255, 255);
    pwm_out1 = constrain(pwm_out1, -255, 255);
}

void M5Bala::run() {
    if (micros() >= loop_interval) {
        loop_interval = micros() + 10000;

        // Attitude sample
        imu_update();
        pitch = imu_getAngleX() + angle_offset;

        // Car down
        if (abs(pitch) > 45) {
            setMotor(0, 0);
            return;
        }

        // Encoder sample
        readEncder();

        // PID Compute
        PIDCompute();

        // Motor out
        setMotor(pwm_out0 + forward_offset + left_offset,
                 pwm_out1 + forward_offset + right_offset);
    }
}

void M5Bala::stop() {
    left_offset  = 0;
    right_offset = 0;
}

void M5Bala::move(int16_t speed, uint16_t duration) {
    forward_offset = -speed;
    if (duration != 0) {
        delay(duration);
        stop();
    }
}

void M5Bala::turn(int16_t speed, uint16_t duration) {
    if (speed > 0) {
        left_offset  = speed;
        right_offset = 0;

    } else if (speed < 0) {
        left_offset  = 0;
        right_offset = -speed;
    }

    if (duration != 0) {
        delay(duration);
        stop();
    }
}

void M5Bala::rotate(int16_t speed, uint16_t duration) {
    left_offset  = speed;
    right_offset = -speed;

    if (duration != 0) {
        delay(duration);
        stop();
    }
}
