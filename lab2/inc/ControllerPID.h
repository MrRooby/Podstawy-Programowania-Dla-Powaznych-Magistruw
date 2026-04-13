#include "ObjectSISO.h"

class ControllerPID : public ObjectSISO {
private:
  double k = 0.0;  // gain P 
  double TI = 0.0; // integral constant I
  double TD = 0.0; // derivative constant D

  double mem_integral = 0.0;
  double prev_error = 0.0;
  void nonZeroSetter(const double value, double &value_to_set);
  
public:
  ControllerPID(double P = 0.0, double I = 0.0, double D = 0.0);

  void setK(const double value);
  void setTI(const double value);
  void setTD(const double value);

  double simulate(const double error) override;
};
