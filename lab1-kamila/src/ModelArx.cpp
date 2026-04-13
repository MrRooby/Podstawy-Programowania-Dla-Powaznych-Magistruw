#include "ModelArx.h"

ModelArx::ModelArx(const std::vector<double>& numA, const std::vector<double>& numB, int dly, double stdDev) {
    setA(numA);
    setB(numB);
    setTransportDelayOrder(dly);
    setStandardDeviation(stdDev);
}

void ModelArx::setA(const std::vector<double>& val) {
    m_A.clear();
    m_A.push_back(0.0);
    
    for (double v : val) {
        m_A.push_back(v);
    }
}

void ModelArx::setB(const std::vector<double>& val) {
    m_B.clear();
    
    for (double v : val) {
        m_B.push_back(v);
    }
}

void ModelArx::setStandardDeviation(double val) {
    if (val < 0.0) {
        m_stdDev = 0.0;
    } else {
        m_stdDev = val;
    }
}

void ModelArx::setTransportDelayOrder(int val) {
    if (val < 0) {
        m_dlyOrder = 1;
    } else {
        m_dlyOrder = (unsigned int)val;
    }
}

double ModelArx::genInterference() {
    if (m_stdDev <= 0.0) {
        return 0.0;
    }
    
    static std::mt19937 gen(std::random_device{}());
    std::normal_distribution<double> dist(0.0, m_stdDev);
    
    return dist(gen);
}

double ModelArx::simulate(double u) {
    m_in.push_back(u);
    
    double sumB = 0.0;
    for (size_t i = 0; i < m_B.size(); i++) {
        int idx = (int)m_in.size() - 1 - m_dlyOrder - (int)i;
        if (idx >= 0) {
            sumB += m_B[i] * m_in[idx];
        }
    }
    
    double sumA = 0.0;
    for (size_t i = 1; i < m_A.size(); i++) {
        int idx = (int)m_out.size() - (int)i;
        if (idx >= 0) {
            sumA += m_A[i] * m_out[idx];
        }
    }
    
    double y = sumB - sumA + genInterference();
    m_out.push_back(y);
    
    return y;
}

void ModelArx::serialize(const std::string& fpath) {
    using namespace nlohmann;
    
    try {
        if (fpath.size() < 5 || fpath.substr(fpath.size() - 5) != ".json") {
            throw std::invalid_argument("Extension must be .json");
        }
        
        std::ofstream ofs(fpath);
        if (!ofs.is_open()) {
            throw std::runtime_error("Cannot open file: " + fpath);
        }
        
        json data;
        data["A"] = m_A;
        data["B"] = m_B;
        data["standard_deviation"] = m_stdDev;
        data["transport_delay_order"] = m_dlyOrder;
        data["input_signal"] = m_in;
        data["output_signal"] = m_out;
        
        ofs << data.dump(4);
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void ModelArx::deserialize(const std::string& fpath) {
    using namespace nlohmann;
    
    try {
        if (fpath.size() < 5 || fpath.substr(fpath.size() - 5) != ".json") {
            throw std::invalid_argument("Extension must be .json");
        }
        
        std::ifstream ifs(fpath);
        if (!ifs.is_open()) {
            throw std::runtime_error("Cannot open file: " + fpath);
        }
        
        json data;
        ifs >> data;
        
        std::vector<double> a_vals = data.at("A").get<std::vector<double>>();
        if (!a_vals.empty()) {
            a_vals.erase(a_vals.begin());
        }
        
        setA(a_vals);
        setB(data.at("B").get<std::vector<double>>());
        setStandardDeviation(data.at("standard_deviation").get<double>());
        setTransportDelayOrder(data.at("transport_delay_order").get<int>());
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
