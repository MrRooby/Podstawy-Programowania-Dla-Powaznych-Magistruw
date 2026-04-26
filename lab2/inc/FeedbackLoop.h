#pragma once
#include "ControllerPID.h"
#include "ModelArx.h"

inline double sim_loop(ControllerPID&reg,ModelArx&obj,const double sp){
  static double y_prev=0.0;
  double err=sp-y_prev;
  double u=reg.simulate(err);
  double y=obj.simulate(u);
  y_prev=y;
  return y;
}
