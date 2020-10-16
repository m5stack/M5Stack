#include "pid.h"

PID::PID(float point, float kp, float ki, float kd) {
  _kp = kp;
  _ki = ki;
  _kd = kd;
  _point = point;
  output_max = 10000;
  output_min = -10000;
  integral_max = 10000;
  integral_min = -10000;
  error_integral = 0;
  _dir = 1;
}

float PID::Update(float input) {
  float output;
  float dinput;
  error = _point - input;
  error_integral += _ki * error + error_integral_offset;
  if(error_integral > integral_max) { error_integral = integral_max; }
  if(error_integral < integral_min) { error_integral = integral_min; }
  dinput = input - input_last;
  output = _kp * error + error_integral - _kd * dinput;
  input_last = input;
  if(output < output_min) { output = output_min; }
  if(output > output_max) { output = output_max; }
  return output;
} 

void PID::SetOutputLimits(float max_out, float min_out) {
  output_max = max_out;
  output_min = min_out;
} 

void PID::SetIntegralLimits(float max_out, float min_out) {
  integral_max = max_out;
  integral_min = min_out;
}

void PID::SetDirection(int8_t dir) {
  _dir = dir;
  _kp = fabs(_kp) * _dir;
  _ki = fabs(_ki) * _dir;
  _kd = fabs(_kd) * _dir;
}

void PID::SetIntegral(float integral) {
  error_integral = integral;
}

void PID::SetIntegralOffset(float offset) {
  error_integral_offset = offset;
}

void PID::UpdateParam(float kp, float ki, float kd) {
  _kp = fabs(kp) * _dir;
  _ki = fabs(ki) * _dir;
  _kd = fabs(kd) * _dir;
}

void PID::SetPoint(float point) {
  _point = point;
}

