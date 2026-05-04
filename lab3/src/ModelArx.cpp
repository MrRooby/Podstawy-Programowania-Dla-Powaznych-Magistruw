#include "ModelArx.h"
#include <random>

ModelArx::ModelArx(const std::vector<double> &A, const std::vector<double> &B,
                   const int transport_delay_order,
                   const double  standard_deviation) {
  setA(A);
  setB(B);
  signal_memory_size = A.size() + B.size();
  setTransportDelayOrder(transport_delay_order);
  setStandardDeviation(standard_deviation);
}

void ModelArx::setA(const std::vector<double> &value) {
  const size_t A_size = value.size();

  A.push_back(0);
  for (size_t i = 0; i < A_size; i++) {
    A.push_back(value[i]);
  }
}

void ModelArx::setB(const std::vector<double> &value) {
  const size_t B_size = value.size();

  for (size_t i = 0; i < B_size; i++) {
    B.push_back(value[i]);
  }
}

void ModelArx::setStandardDeviation(const double value) {
  if (value < 0)
    standard_deviation = 0.0;
  else
    standard_deviation = value;
}

void ModelArx::setTransportDelayOrder(const int value) {
  if (value < 0)
    transport_delay_order = 1;
  else
    transport_delay_order = value;
}

double ModelArx::simulate_interf_sig() {
  if (standard_deviation == 0.0)
    return 0.0;
  
  static std::mt19937 generator(std::random_device{}());
  std::normal_distribution<double> distribution(0.0, standard_deviation);
  return distribution(generator);
}

double ModelArx::simulate(const double u) {
  double y = 0.0;
  double b_sum = 0.0;
  double a_sum = 0.0;
  double e = simulate_interf_sig();
  input_signal.push_back(u);

  for (size_t i = 0; i < B.size(); i++) {
    int index = (int)input_signal.size() - 1 - transport_delay_order - (int)i;
    if(index >= 0) 
      b_sum += B[i] * input_signal.at(index);
  }

  for (size_t i = 1; i < A.size(); i++) {
    int index = (int)output_signal.size() - (int)i;
    if(index >= 0)
      a_sum += A[i] * output_signal.at(index);
  }
  
  y = b_sum - a_sum + e;
  output_signal.push_back(y);
  return y;
}


void ModelArx::serialize(const std::string& filename){
  using namespace nlohmann;
  try {
    std::string ext = ".json";
    if (filename.length() < ext.length() || 
        filename.compare(filename.length() - ext.length(), ext.length(), ext) != 0) {
      throw std::invalid_argument("Error: File extension must be .json");
    }

    std::ofstream file(filename);

    if (!file.is_open()) {
      throw std::runtime_error("Error: File does not exist or cannot be opened: " + filename);
    }

    json j;
    j["A"] = A;
    j["B"] = B;
    j["standard_deviation"] = standard_deviation;
    j["transport_delay_order"] = transport_delay_order;
    j["input_signal"] = input_signal;
    j["output_signal"] = output_signal;
 
    file << j.dump(4);
    file.close();

  } catch (const nlohmann::json::exception& e) {
    std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void ModelArx::deserialize(const std::string& filename){
  using namespace nlohmann;
  try {
    std::string ext = ".json";
    if (filename.length() < ext.length() || 
        filename.compare(filename.length() - ext.length(), ext.length(), ext) != 0) {
      throw std::invalid_argument("Error: File extension must be .json");
    }

    std::ifstream file(filename);

    if (!file.is_open()) {
      throw std::runtime_error("Error: File does not exist or cannot be opened: " + filename);
    }

    json j;
    setA(j.at("A").get<std::vector<double>>());
    setB(j.at("B").get<std::vector<double>>());
    setStandardDeviation(j.at("standard_deviation").get<double>());
    setTransportDelayOrder(j.at("transport_delay_order").get<int>());

    file.close();

  } catch (const nlohmann::json::exception& e) {
    std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
