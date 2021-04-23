#pragma once

#include "Arduino.h"

#define READ_TIMEOUT 1000

class ODrive {
public:
    ODrive(Stream& serial_in);

    enum AxisState_t {
        AXIS_STATE_UNDEFINED = 0,           //<! will fall through to idle
        AXIS_STATE_IDLE = 1,                //<! disable PWM and do nothing
        AXIS_STATE_STARTUP_SEQUENCE = 2, //<! the actual sequence is defined by the config.startup_... flags
        AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,   //<! run all calibration procedures, then idle
        AXIS_STATE_MOTOR_CALIBRATION = 4,   //<! run motor calibration
        AXIS_STATE_SENSORLESS_CONTROL = 5,  //<! run sensorless control
        AXIS_STATE_ENCODER_INDEX_SEARCH = 6, //<! run encoder index search
        AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7, //<! run encoder offset calibration
        AXIS_STATE_CLOSED_LOOP_CONTROL = 8  //<! run closed loop control
    };

    enum ControlMode_t {
        CONTROL_MODE_VOLTAGE_CONTROL  = 0,
        CONTROL_MODE_TORQUE_CONTROL   = 1,
        CONTROL_MODE_VELOCITY_CONTROL = 2,
        CONTROL_MODE_POSITION_CONTROL = 3,
    };

    // Commands
    void setPosition(float position);
    void setPosition(float position, float velocity_feedforward);
    void setPosition(float position, float velocity_feedforward, float current_feedforward);
    void setVelocity(float velocity);
    void setVelocity(float velocity, float current_feedforward);
    void setCurrent(float current);
    void setGain(float pos, float vel, float vel_integrator);
    void setControlMode(int32_t mode);
    void setControlInputPos(float pos);
    void trapezoidalMove(float position);
    // Getters
    float getVelocity();
    float getVbusVoltage();
    int32_t getEncoderShadowCount();
    float getEncoderPosEstimate();
    float getMotorTemp();
    float getPhaseCurrent();
    float getBusCurrent();
    bool checkError(int32_t* axis=NULL, int32_t* motor_thermistor=NULL, int32_t* encoder=NULL, int32_t* controller=NULL);
    // General params


    // State helper
    bool runState(int32_t requested_state, uint32_t timeout);

    // device will reboot
    void eraseConfig();
    void reboot();
    void saveConfig();
    void setDefaultConfig();

    float readFloat();
    int32_t readInt();
    void writeToDeive(const char* data);
    void writeConfig(const char* config, float_t value);
    void writeConfig(const char* config, int32_t value);
    int32_t readConfigInt(const char*config);
    float readConfigFloat(const char*config);

private:
    String readString();
    void readFlush();
    Stream& serial;
};

