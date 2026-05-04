import sys

with open("src/MainWindow.cpp", "r") as f:
    text = f.read()

old_main = """MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), simCtrl(nullptr) {
    simCtrl = new SimulationController();
    currentSignal = std::make_shared<BaseSignal>();
    setupUI();
    setWindowTitle("PID Controller Simulator");
    resize(1400, 1000);
}"""
new_main = """MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), simCtrl(nullptr) {
    simCtrl = new SimulationController();
    setupUI();
    setWindowTitle("PID Controller Simulator");
    resize(1400, 1000);
    comboTheme->setCurrentIndex(1);
    applyTheme(true);
    onRunSimulation();
}"""

old_ui = """    comboTheme = new QComboBox();
    comboTheme->addItem("Light Mode");
    comboTheme->addItem("Dark Mode");"""
new_ui = """    comboTheme = new QComboBox();
    comboTheme->addItem("Light Mode");
    comboTheme->addItem("Dark Mode");
    comboTheme->setCurrentIndex(1);"""
    
old_sig = """    btnSave->setObjectName("btnSave");"""
new_sig = """    btnSave->setObjectName("btnSave");

    // Default signal
    currentSignal = std::make_shared<SquareSignal>(std::make_shared<BaseSignal>(), 1.0, 50, 0.5);
    listSignals->addItem("Square(amp: 1, T: 50, dc: 0.5)");
"""


text = text.replace(old_main, new_main).replace(old_ui, new_ui).replace(old_sig, new_sig)

with open("src/MainWindow.cpp", "w") as f:
    f.write(text)

