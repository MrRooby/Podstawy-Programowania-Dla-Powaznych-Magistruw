#pragma once
#include <vector>
#include "ControllerPID.h"
#include "ModelArx.h"

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
    double prev_output = 0.0;
    
public:
    SimulationController();
    ~SimulationController();
    
    void setControllerParams(double k, double ti, double td);
    void setModelParams(double a1, double b0, int delay, double noise_std);
    
    SimulationData runSimulation(
        const std::vector<double>& setpoints,
        double dt = 1.0
    );
    
    void reset();
};
