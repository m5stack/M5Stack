#ifndef _PID_H_
#define _PID_H_

#include "Arduino.h"

class PID {


public:

  //commonly used functions **************************************************************************
  PID(float point, float kp, float ki, float kd); 
	
  float Update(float input);                     
  void SetOutputLimits(float max_out, float min_out); 
  void SetIntegralLimits(float max_out, float min_out);
  void SetPoint(float point);
	void SetDirection(int8_t dir);
  void UpdateParam(float kp, float ki, float kd);
  void SetIntegral(float integral);
  void SetIntegralOffset(float offset);

  float error_integral;

private:
  float _kp, _ki, _kd;
  float _point;
  float error_integral_offset;
  float input_last;
  float error;
  float output_max, output_min;
  float integral_max, integral_min;
  int8_t _dir;
};

#endif
