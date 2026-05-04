
#include "ControllerPID.h"
#include <fstream>

ControllerPID::ControllerPID(double p,double i,double d){
  err_prev=0.0; int_mem=0.0;
  set_k(p); set_ti(i); set_td(d);
}

void ControllerPID::set_val(const double v,double &f){
  f=v<0?0.0:v;
}

void ControllerPID::set_k(const double v){set_val(v,wzmac);}
void ControllerPID::set_ti(const double v){set_val(v,const_int);}
void ControllerPID::set_td(const double v){set_val(v,const_der);}

double ControllerPID::p_part(const double e){return wzmac*e;}
double ControllerPID::i_part(const double e){
  if(const_int<=0)return 0;
  int_mem+=e; return int_mem/const_int;
}
double ControllerPID::d_part(const double e){
  double r=(const_der>0)?const_der*(e-err_prev):0;
  err_prev=e; return r;
}

double ControllerPID::simulate(const double e){
  return p_part(e)+i_part(e)+d_part(e);
}

void ControllerPID::save(const std::string& fn){
  using namespace nlohmann;
  try{
    std::string ext=".json";
    if(fn.length()<ext.length()||fn.compare(fn.length()-ext.length(),ext.length(),ext)!=0)
      throw std::invalid_argument("err: .json only");
    std::ofstream f(fn);
    if(!f.is_open())throw std::runtime_error("err: cant open "+fn);
    json j;
    j["k"]=wzmac; j["ti"]=const_int; j["td"]=const_der;
    j["im"]=int_mem; j["ep"]=err_prev;
    f<<j.dump(2)<<std::endl; f.close();
  }catch(const std::exception&e){throw e;}
}

void ControllerPID::load(const std::string& fn){
  using namespace nlohmann;
  try{
    std::string ext=".json";
    if(fn.length()<ext.length()||fn.compare(fn.length()-ext.length(),ext.length(),ext)!=0)
      throw std::invalid_argument("err: .json only");
    std::ifstream f(fn);
    if(!f.is_open())throw std::runtime_error("err: cant open "+fn);
    json j; f>>j; f.close();
    wzmac=j["k"]; const_int=j["ti"]; const_der=j["td"];
    int_mem=j["im"]; err_prev=j["ep"];
  }catch(const std::exception&e){throw e;}
}
