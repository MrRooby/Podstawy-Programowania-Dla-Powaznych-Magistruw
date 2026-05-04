#pragma once
#include "ObjectSISO.h"
#include <string>
#include <nlohmann/json.hpp>

class ControllerPID : public ObjectSISO {
private:
  double wzmac=0.0;
  double const_int=0.0;
  double const_der=0.0;
  double int_mem=0.0;
  double err_prev=0.0;
  
  void set_val(const double v,double &f);
  double p_part(const double e);
  double i_part(const double e);
  double d_part(const double e);
  
public:
  ControllerPID(double p=0.0,double i=0.0,double d=0.0);

  void set_k(const double v);
  void set_ti(const double v);
  void set_td(const double v);

  double simulate(const double e) override;
  
  void save(const std::string& fn);
  void load(const std::string& fn);
};
