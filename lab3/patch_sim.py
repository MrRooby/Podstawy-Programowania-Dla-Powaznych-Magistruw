import sys

with open("src/SimulationController.cpp", "r") as f:
    text = f.read()

old_controller = """void SimulationController::setControllerParams(double k, double ti, double td) {
    if (controller) {
        controller->set_k(k);
        controller->set_ti(ti);
        controller->set_td(td);
    }
}"""
new_controller = """void SimulationController::setControllerParams(double k, double ti, double td) {
    if (controller) delete controller;
    controller = new ControllerPID(k, ti, td);
}"""

old_model = """void SimulationController::setModelParams(double a1, double b0, int delay, double noise_std) {
    if (model) {
        delete model;
        model = new ModelArx({1.0, a1}, {b0}, delay, noise_std);
    }
}"""
new_model = """void SimulationController::setModelParams(double a1, double b0, int delay, double noise_std) {
    if (model) delete model;
    model = new ModelArx({1.0, a1}, {b0}, delay, noise_std);
}"""

text = text.replace(old_controller, new_controller)
text = text.replace(old_model, new_model)

with open("src/SimulationController.cpp", "w") as f:
    f.write(text)

