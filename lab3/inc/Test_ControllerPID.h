#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "ControllerPID.h"
#include "Tests.h"

class Test_ControllerPID:public Tests{
public:
  ~Test_ControllerPID()=default;
  static void performTests();
private:
  static void t_p_noexc();
  static void t_p_step();
  static void t_pi1_step();
  static void t_pi2_step();
  static void t_pid_step();
  static void t_save_load();
};
