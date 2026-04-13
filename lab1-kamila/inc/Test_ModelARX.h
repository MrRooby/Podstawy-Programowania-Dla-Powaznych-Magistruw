#ifndef TESTMODELARX_H
#define TESTMODELARX_H
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "ModelArx.h"

class Test_ModelARX {
public:
    static void performTests();
private:
    static void logMismatch(const std::vector<double>& exp, const std::vector<double>& act);
    static bool verifySeq(const std::vector<double>& exp, const std::vector<double>& act);
    static void test_ModelArx_noExcitation();
    static void test_ModelArx_unitStep_v1();
    static void test_ModelArx_unitStep_v2();
    static void test_ModelArx_unitStep_v3();
};
#endif
