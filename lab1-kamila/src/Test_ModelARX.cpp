#include "Test_ModelARX.h"

void Test_ModelARX::performTests() {
    test_ModelArx_noExcitation();
    test_ModelArx_unitStep_v1();
    test_ModelArx_unitStep_v2();
    test_ModelArx_unitStep_v3();
}

void Test_ModelARX::logMismatch(const std::vector<double>& exp, const std::vector<double>& act) {
    std::cerr << std::fixed << std::setprecision(3);
    std::cerr << "  Expected:\t";
    for (double v : exp) {
        std::cerr << v << ", ";
    }
    
    std::cerr << "\n  Actual:\t";
    for (double v : act) {
        std::cerr << v << ", ";
    }
    std::cerr << "\n\n";
}

bool Test_ModelARX::verifySeq(const std::vector<double>& exp, const std::vector<double>& act) {
    if (exp.size() != act.size()) {
        return false;
    }
    
    for (size_t k = 0; k < act.size(); ++k) {
        if (std::fabs(act[k] - exp[k]) >= 1e-3) {
            return false;
        }
    }
    
    return true;
}

void Test_ModelARX::test_ModelArx_noExcitation() {
    std::cerr << "ModelArx (-0.4 | 0.6 | 1 | 0 ) -> zero excitation test: ";
    
    try {
        ModelArx m({-0.4}, {0.6}, 1, 0.0);
        std::vector<double> in(30, 0.0);
        std::vector<double> exp(30, 0.0);
        std::vector<double> act(30);
        
        for (int k = 0; k < 30; ++k) {
            act[k] = m.simulate(in[k]);
        }
        
        if (verifySeq(exp, act)) {
            std::cerr << "OK!\n";
        } else {
            std::cerr << "FAIL!\n";
            logMismatch(exp, act);
        }
        
        m.serialize("ModelArx_test1_output.json");
        
    } catch (...) {
        std::cerr << "INTERRUPTED!\n";
    }
}

void Test_ModelARX::test_ModelArx_unitStep_v1() {
    std::cerr << "ModelArx (-0.4 | 0.6 | 1 | 0 ) -> step response test, version 1: ";
    
    try {
        ModelArx m({-0.4}, {0.6}, 1, 0.0);
        std::vector<double> in(30);
        std::vector<double> act(30);
        
        for (int k = 0; k < 30; ++k) {
            if (k != 0) {
                in[k] = 1.0;
            } else {
                in[k] = 0.0;
            }
        }
        
        std::vector<double> exp = { 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        
        for (int k = 0; k < 30; ++k) {
            act[k] = m.simulate(in[k]);
        }
        
        if (verifySeq(exp, act)) {
            std::cerr << "OK!\n";
        } else {
            std::cerr << "FAIL!\n";
            logMismatch(exp, act);
        }
        
    } catch (...) {
        std::cerr << "INTERRUPTED!\n";
    }
}

void Test_ModelARX::test_ModelArx_unitStep_v2() {
    std::cerr << "ModelArx (-0.4 | 0.6 | 2 | 0 ) -> step response test, version 2: ";
    
    try {
        ModelArx m({-0.4}, {0.6}, 2, 0.0);
        std::vector<double> in(30);
        std::vector<double> act(30);
        
        for (int k = 0; k < 30; ++k) {
            if (k != 0) {
                in[k] = 1.0;
            } else {
                in[k] = 0.0;
            }
        }
        
        std::vector<double> exp = { 0, 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        
        for (int k = 0; k < 30; ++k) {
            act[k] = m.simulate(in[k]);
        }
        
        if (verifySeq(exp, act)) {
            std::cerr << "OK!\n";
        } else {
            std::cerr << "FAIL!\n";
            logMismatch(exp, act);
        }
        
    } catch (...) {
        std::cerr << "INTERRUPTED!\n";
    }
}

void Test_ModelARX::test_ModelArx_unitStep_v3() {
    std::cerr << "ModelArx (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> step response test, version 3: ";
    
    try {
        ModelArx m({-0.4, 0.2}, {0.6, 0.3}, 2, 0.0);
        std::vector<double> in(30);
        std::vector<double> act(30);
        
        for (int k = 0; k < 30; ++k) {
            if (k != 0) {
                in[k] = 1.0;
            } else {
                in[k] = 0.0;
            }
        }
        
        std::vector<double> exp = { 0, 0, 0, 0.6, 1.14, 1.236, 1.1664, 1.11936, 1.11446, 1.12191, 1.12587, 1.12597, 1.12521, 1.12489, 1.12491, 1.12499, 1.12501, 1.12501, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125 };
        
        for (int k = 0; k < 30; ++k) {
            act[k] = m.simulate(in[k]);
        }
        
        if (verifySeq(exp, act)) {
            std::cerr << "OK!\n";
        } else {
            std::cerr << "FAIL!\n";
            logMismatch(exp, act);
        }
        
    } catch (...) {
        std::cerr << "INTERRUPTED!\n";
    }
}
