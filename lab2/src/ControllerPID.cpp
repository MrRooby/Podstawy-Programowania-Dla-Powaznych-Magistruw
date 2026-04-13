
#include "ControllerPID.h"

ControllerPID::ControllerPID(double P, double I, double D){
  prev_error = 0.0;
  mem_integral = 0.0;
  setK(P);
  setTI(I);
  setTD(D);
}


void ControllerPID::nonZeroSetter(const double value, double &value_to_set){
  if(value < 0) value_to_set = 0.0;
  else value_to_set = value;
}

void ControllerPID::setK(const double value){
  nonZeroSetter(value, k);
}
void ControllerPID::setTI(const double value){
  nonZeroSetter(value, TI);
}
void ControllerPID::setTD(const double value){
  nonZeroSetter(value, TD);
}

double ControllerPID::simulate(const double error){
  return k * error;
}
