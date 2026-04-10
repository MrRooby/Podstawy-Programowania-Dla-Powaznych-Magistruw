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
#include "Test_ModelARX.h"

class ModelArx : public ObjectSISO{
private:
  std::vector<double> A; // denominator
  std::vector<double> B; // numerator
  std::deque<double>  input_signal;
  std::deque<double> output_signal;
  std::deque<double> transport_delay;

  unsigned int signal_memory_size;
  unsigned int transport_delay_order;

  double standard_deviation;

  double simulate_interf_sig();

public:
  ModelArx(
    const std::vector<double> &A,
    const std::vector<double> &B,
    const int transport_delay,
    const double standard_deviation
  );

  void setA(const std::vector<double> &value);
      

  void setB(const std::vector<double> &value);

  void setStandardDeviation(const double value);

  void setTransportDelayOrder(const int value);

  double simulate(const double u) override;

  void serialize(const std::string& filename);

  void deserialize(const std::string& filename);

  friend class Test_ModelARX;
};
