#include "SimulationController.h"

SimulationController::SimulationController() {
    controller = new ControllerPID(0.5);
    model = new ModelArx({1.0, 0.4}, {0.6}, 1, 0.0);
}

SimulationController::~SimulationController() {
    delete controller;
    delete model;
}

void SimulationController::setControllerParams(double k, double ti, double td) {
    if (controller) {
        controller->set_k(k);
        controller->set_ti(ti);
        controller->set_td(td);
    }
}

void SimulationController::setModelParams(double a1, double b0, int delay, double noise_std) {
    if (model) {
        delete model;
        model = new ModelArx({1.0, a1}, {b0}, delay, noise_std);
    }
}

SimulationData SimulationController::runSimulation(
    const std::vector<double>& setpoints,
    double dt) {
    
    SimulationData data;
    prev_output = 0.0;
    
    for (size_t i = 0; i < setpoints.size(); ++i) {
        double sp = setpoints[i];
        double error = sp - prev_output;
        double control = controller->simulate(error);
        double output = model->simulate(control);
        
        data.time.push_back(i * dt);
        data.setpoint.push_back(sp);
        data.output.push_back(output);
        data.error.push_back(error);
        data.control.push_back(control);
        
        prev_output = output;
    }
    
    return data;
}

void SimulationController::reset() {
    prev_output = 0.0;
}
