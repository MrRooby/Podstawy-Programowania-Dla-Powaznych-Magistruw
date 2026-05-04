#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "ModelArx.h"

class Test_ModelARX {
public:
  ~Test_ModelARX() = default;
  static void performTests();

private:
  static void reportErrorInSequence(std::vector<double>& expected, std::vector<double>& actual);
  static bool compareSequences(std::vector<double>& expected, std::vector<double>& actual);
  static void test_ModelArx_noExcitation();
  static void test_ModelArx_unitStep_v1();
  static void test_ModelArx_unitStep_v2();
  static void test_ModelArx_unitStep_v3();
};
