#pragma once
#include <vector>
#include <memory>
#include "ControllerPID.h"
#include "ModelArx.h"
#include "SignalGenerator.h"

struct SimulationData {
    std::vector<double> time;
    std::vector<double> setpoint;
    std::vector<double> output;
    std::vector<double> error;
    std::vector<double> control;
};

class SimulationController {
private:
    ControllerPID* controller = nullptr;
    ModelArx* model = nullptr;
    std::shared_ptr<SignalGenerator> signal;
    double prev_output = 0.0;
    
public:
    SimulationController();
    ~SimulationController();
    
    void setControllerParams(double k, double ti, double td);
    void setModelParams(double a1, double b0, int delay, double noise_std);
    void setSignalGenerator(std::shared_ptr<SignalGenerator> sig);
    
    SimulationData runSimulation(
        int steps,
        double dt = 1.0
    );
    
    void reset();
};
