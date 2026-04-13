#pragma once
#include <vector>
#include <stdexcept>
#include <deque>
#include <random>
#include <fstream>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "ObjectSISO.h"

class Test_ModelARX;

class ModelArx : public ObjectSISO {
public:
    ModelArx(const std::vector<double>& numA, const std::vector<double>& numB, int dly, double stdDev);
    void setA(const std::vector<double>& val);
    void setB(const std::vector<double>& val);
    void setStandardDeviation(double val);
    void setTransportDelayOrder(int val);
    double simulate(double u) override;
    void serialize(const std::string& fpath);
    void deserialize(const std::string& fpath);
    friend class Test_ModelARX;

private:
    double genInterference();
    std::vector<double> m_A;
    std::vector<double> m_B;
    std::deque<double> m_in;
    std::deque<double> m_out;
    unsigned int m_dlyOrder;
    double m_stdDev;
};
