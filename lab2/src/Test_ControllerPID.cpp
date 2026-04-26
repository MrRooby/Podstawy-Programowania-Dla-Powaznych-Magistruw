#include "Test_ControllerPID.h"

void Test_ControllerPID::performTests(){
  t_p_noexc(); t_p_step(); t_pi1_step(); t_pi2_step(); t_pid_step(); t_save_load();
}

void Test_ControllerPID::t_p_noexc(){
  std::cerr<<"p(0.5) no-signal: ";
  try{
    ControllerPID c(0.5); std::vector<double> in(30),ex(30),out(30);
    for(int i=0;i<30;i++)out[i]=c.simulate(in[i]);
    if(chk_seq(ex,out))std::cerr<<"OK!\n";
    else{std::cerr<<"FAIL!\n"; print_diff(ex,out);}
  }catch(...){std::cerr<<"EXC!\n";}
}

void Test_ControllerPID::t_p_step(){
  std::cerr<<"p(0.5) step: ";
  try{
    ControllerPID c(0.5); std::vector<double> in(30),ex(30),out(30);
    for(int i=0;i<30;i++)in[i]=!!i;
    ex={0.0,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    for(int i=0;i<30;i++)out[i]=c.simulate(in[i]);
    if(chk_seq(ex,out))std::cerr<<"OK!\n";
    else{std::cerr<<"FAIL!\n"; print_diff(ex,out);}
  }catch(...){std::cerr<<"EXC!\n";}
}

void Test_ControllerPID::t_pi1_step(){
  std::cerr<<"pi(0.5,1.0) step-1: ";
  try{
    ControllerPID c(0.5,1.0); std::vector<double> in(30),ex(30),out(30);
    for(int i=0;i<30;i++)in[i]=!!i;
    ex={0,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,13.5,14.5,15.5,16.5,17.5,18.5,19.5,20.5,21.5,22.5,23.5,24.5,25.5,26.5,27.5,28.5,29.5};
    for(int i=0;i<30;i++)out[i]=c.simulate(in[i]);
    if(chk_seq(ex,out))std::cerr<<"OK!\n";
    else{std::cerr<<"FAIL!\n"; print_diff(ex,out);}
  }catch(...){std::cerr<<"EXC!\n";}
}

void Test_ControllerPID::t_pi2_step(){
  std::cerr<<"pi(0.5,10.0) step-2: ";
  try{
    ControllerPID c(0.5,10.0); std::vector<double> in(30),ex(30),out(30);
    for(int i=0;i<30;i++)in[i]=!!i;
    ex={0,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3,3.1,3.2,3.3,3.4};
    for(int i=0;i<30;i++)out[i]=c.simulate(in[i]);
    if(chk_seq(ex,out))std::cerr<<"OK!\n";
    else{std::cerr<<"FAIL!\n"; print_diff(ex,out);}
  }catch(...){std::cerr<<"EXC!\n";}
}

void Test_ControllerPID::t_pid_step(){
  std::cerr<<"pid(0.5,10.0,0.2) step: ";
  try{
    ControllerPID c(0.5,10.0,0.2); std::vector<double> in(30),ex(30),out(30);
    for(int i=0;i<30;i++)in[i]=!!i;
    ex={0,0.8,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3,3.1,3.2,3.3,3.4};
    for(int i=0;i<30;i++)out[i]=c.simulate(in[i]);
    if(chk_seq(ex,out))std::cerr<<"OK!\n";
    else{std::cerr<<"FAIL!\n"; print_diff(ex,out);}
  }catch(...){std::cerr<<"EXC!\n";}
}

void Test_ControllerPID::t_save_load(){
  std::cerr<<"save/load: ";
  try{
    ControllerPID c1(0.5,1.0,0.2);
    c1.simulate(1.0); c1.save("ctrl.json");
    ControllerPID c2(0.0);
    c2.load("ctrl.json");
    std::vector<double> in(10),o1(10),o2(10);
    for(int i=0;i<10;i++)in[i]=!!i;
    ControllerPID c3(0.5,1.0,0.2);
    c3.simulate(1.0);
    for(int i=0;i<10;i++){o1[i]=c3.simulate(in[i]); o2[i]=c2.simulate(in[i]);}
    if(chk_seq(o1,o2))std::cerr<<"OK!\n";
    else{std::cerr<<"FAIL!\n"; print_diff(o1,o2);}
  }catch(...){std::cerr<<"EXC!\n";}
}
