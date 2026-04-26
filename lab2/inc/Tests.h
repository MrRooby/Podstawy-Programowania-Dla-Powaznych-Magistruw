#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

class Tests{
public:
  ~Tests()=default;
  static void print_diff(std::vector<double>&exp,std::vector<double>&got){
    std::cerr<<std::fixed<<std::setprecision(3);
    std::cerr<<"  exp: "; for(auto x:exp)std::cerr<<x<<" ";
    std::cerr<<"\n  got: "; for(auto x:got)std::cerr<<x<<" ";
    std::cerr<<"\n\n";
  }
  static bool chk_seq(std::vector<double>&exp,std::vector<double>&got){
    if(got.size()!=exp.size())return 0;
    for(size_t i=0;i<got.size();i++)if(fabs(got[i]-exp[i])>1e-3)return 0;
    return 1;
  }
};
