#include "odrive.h"

#include <stdio.h>

const char* default_config =
    "w config.brake_resistance 0\n"
    "w config.dc_bus_undervoltage_trip_level 8\n"
    "w config.dc_bus_overvoltage_trip_level 34\n"
    "w config.dc_max_negative_current -5\n"
    "w config.max_regen_current 0\n"
    "w axis0.motor.config.pole_pairs 4\n"
    "w axis0.motor.config.calibration_current 5\n"
    "w axis0.motor.config.resistance_calib_max_voltage 4\n"
    "w axis0.motor.config.motor_type 0\n"
    "w axis0.motor.config.current_lim 5\n"
    "w axis0.motor.config.requested_current_range 20\n"
    "w axis0.encoder.config.mode 0\n"
    "w axis0.encoder.config.use_index 1\n"
    "w axis0.encoder.config.cpr 4000\n"
    "w axis0.controller.config.control_mode 3\n"
    "w axis0.controller.config.vel_limit 65536000\n"
    "w axis0.controller.config.pos_gain 20\n"
    "w axis0.motor_thermistor.config.poly_coefficient_0 551.8902587890625\n"
    "w axis0.motor_thermistor.config.poly_coefficient_1 -753.2103271484375\n"
    "w axis0.motor_thermistor.config.poly_coefficient_2 436.8048095703125\n"
    "w axis0.motor_thermistor.config.poly_coefficient_3 -42.3204460144043\n"
    "w axis0.motor_thermistor.config.temp_limit_lower 75\n"
    "w axis0.motor_thermistor.config.temp_limit_upper 90\n"
    "w axis0.motor_thermistor.config.enabled 1\n";

ODrive::ODrive(Stream& serial_in) : serial(serial_in) {
}

void ODrive::setPosition(float position) {
    setPosition(position, 0.0f, 0.0f);
}

void ODrive::setPosition(float position, float velocity_feedforward) {
    setPosition(position, velocity_feedforward, 0.0f);
}

void ODrive::setPosition(float position, float velocity_feedforward,
                         float current_feedforward) {
    serial.printf("p 0 %f %f %f\n", position, velocity_feedforward,
                  current_feedforward);
}

void ODrive::setVelocity(float velocity) {
    setVelocity(velocity, 0.0f);
}

void ODrive::setVelocity(float velocity, float current_feedforward) {
    serial.printf("v 0 %f %f\n", velocity, current_feedforward);
}

void ODrive::setCurrent(float current) {
    serial.printf("c 0 %f\n", current);
}

void ODrive::setGain(float pos, float vel, float vel_integrator) {
    writeConfig("axis0.controller.config.pos_gain", pos);
    writeConfig("axis0.controller.config.vel_gain", vel);
    writeConfig("axis0.controller.config.vel_integrator_gain", vel_integrator);
}

void ODrive::setControlMode(int32_t mode) {
    writeConfig("axis0.controller.config.control_mode", mode);
}

void ODrive::setControlInputPos(float pos) {
    writeConfig("axis0.controller.input_pos", pos);
}

void ODrive::trapezoidalMove(float position) {
    serial.printf("t 0 %f\n", position);
}

bool ODrive::runState(int32_t requested_state, uint32_t timeout) {
    uint32_t time_start = millis();
    int32_t state       = -1;
    writeConfig("axis0.requested_state", requested_state);
    state = readConfigInt("axis0.requested_state");
    while (state != requested_state && millis() - time_start < timeout) {
        delay(100);
        state = readConfigInt("axis0.requested_state");
    }
    return state == requested_state;
}

float ODrive::getVelocity() {
    return readConfigFloat("axis0.encoder.vel_estimate");
}

float ODrive::getVbusVoltage() {
    return readConfigFloat("vbus_voltage");
}

float ODrive::getPhaseCurrent() {
    return readConfigFloat("axis0.motor.current_control.Iq_measured");
}

float ODrive::getBusCurrent() {
    return readConfigFloat("axis0.motor.current_control.Ibus");
}

int32_t ODrive::getEncoderShadowCount() {
    return readConfigInt("axis0.encoder.shadow_count");
}

float ODrive::getEncoderPosEstimate() {
    return readConfigInt("axis0.encoder.pos_estimate");
}

float ODrive::getMotorTemp() {
    return readConfigFloat("axis0.motor_thermistor.temperature");
}

void ODrive::eraseConfig() {
    writeToDeive("se\n");
}

void ODrive::saveConfig() {
    writeToDeive("ss\n");
}

void ODrive::reboot() {
    writeToDeive("sr\n");
}

void ODrive::setDefaultConfig() {
    writeToDeive("\n");
    writeToDeive(default_config);
    saveConfig();
}

bool ODrive::checkError(int32_t* axis, int32_t* motor_thermistor,
                        int32_t* encoder, int32_t* controller) {
    int32_t errors[4] = {0, 0, 0, 0};
    errors[0]         = readConfigInt("axis0.error");
    errors[1]         = readConfigInt("axis0.motor_thermistor.error");
    errors[2]         = readConfigInt("axis0.encoder.error");
    errors[3]         = readConfigInt("axis0.controller.error");

    if (axis) {
        *axis = errors[0];
    }
    if (motor_thermistor) {
        *motor_thermistor = errors[1];
    }
    if (encoder) {
        *encoder = errors[2];
    }
    if (controller) {
        *controller = errors[3];
    }

    if (errors[0] || errors[1] || errors[2] || errors[3]) {
        return true;
    }

    return false;
}

void ODrive::readFlush() {
    while (serial.available()) {
        serial.read();
    }
}

String ODrive::readString() {
    String str                  = "";
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial.available()) {
            if (millis() - timeout_start >= READ_TIMEOUT) {
                return str;
            }
        }
        char c = serial.read();
        if (c == '\n') {
            break;
        }
        str += c;
    }
    return str;
}

float ODrive::readFloat() {
    return readString().toFloat();
}

int32_t ODrive::readInt() {
    return readString().toInt();
}

void ODrive::writeToDeive(const char* data) {
    if (data == NULL) {
        return;
    }
    serial.write(data);
}

void ODrive::writeConfig(const char* config, float value) {
    char* out_string = NULL;
    int result       = asprintf(&out_string, "w %s %f\n", config, value);
    if (result != -1) {
        writeToDeive(out_string);
        free(out_string);
    }
}

void ODrive::writeConfig(const char* config, int32_t value) {
    char* out_string = NULL;
    int result       = asprintf(&out_string, "w %s %d\n", config, value);
    if (result != -1) {
        writeToDeive(out_string);
        free(out_string);
    }
}

int32_t ODrive::readConfigInt(const char* config) {
    char* out_string = NULL;
    int result       = asprintf(&out_string, "r %s\n", config);
    if (result != -1) {
        readFlush();
        writeToDeive(out_string);
        free(out_string);
    }

    return readInt();
}

float ODrive::readConfigFloat(const char* config) {
    char* out_string = NULL;
    int result       = asprintf(&out_string, "r %s\n", config);
    if (result != -1) {
        readFlush();
        writeToDeive(out_string);
        free(out_string);
    }

    return readFloat();
}
