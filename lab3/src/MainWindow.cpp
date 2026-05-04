#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QApplication>
#include <algorithm>
#include <fstream>

// ===== GraphWidget Implementation =====
GraphWidget::GraphWidget(QWidget* parent) 
    : QWidget(parent), isDarkMode(false) {
    setMinimumHeight(200);
    setStyleSheet("background-color: #ffffff; border: 1px solid #e5e5ea; border-radius: 8px;");
}

void GraphWidget::setDarkMode(bool dark) {
    isDarkMode = dark;
    if (dark) {
        setStyleSheet("background-color: #252525; border: 1px solid #333333; color: #f5f5f7; border-radius: 8px;");
    } else {
        setStyleSheet("background-color: #ffffff; border: 1px solid #e5e5ea; color: #1d1d1f; border-radius: 8px;");
    }
    update();
}

void GraphWidget::setData(const std::vector<double>& x, const std::vector<double>& y,
                          const std::vector<double>& y2) {
    xData = x;
    yData = y;
    yData2 = y2;
    update();
}

void GraphWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(rect(), isDarkMode ? QColor("#2b2b2b") : Qt::white);
    
    painter.setPen(isDarkMode ? Qt::white : Qt::black);
    
    if (xData.empty() || yData.empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "No data to display");
        return;
    }
    
    // Draw border
    painter.drawRect(0, 0, width()-1, height()-1);
    
    // Draw title
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(5, 15, title);
    
    // Calculate data bounds
    double xMin = *std::min_element(xData.begin(), xData.end());
    double xMax = *std::max_element(xData.begin(), xData.end());
    double yMin = *std::min_element(yData.begin(), yData.end());
    double yMax = *std::max_element(yData.begin(), yData.end());
    
    if (!yData2.empty()) {
        yMin = std::min(yMin, *std::min_element(yData2.begin(), yData2.end()));
        yMax = std::max(yMax, *std::max_element(yData2.begin(), yData2.end()));
    }
    
    // Add padding
    double yPad = (yMax - yMin) * 0.1;
    if (yPad == 0) yPad = 0.5;
    yMin -= yPad;
    yMax += yPad;
    
    int margin = 40;
    int plotW = width() - 2*margin;
    int plotH = height() - 2*margin - 20;
    
    // Draw axes
    painter.setFont(QFont("Arial", 8));
    painter.drawLine(margin, margin, margin, height()-margin);
    painter.drawLine(margin, height()-margin, width()-margin, height()-margin);
    
    // Draw grid and labels
    QColor gridColor = isDarkMode ? QColor("#555") : Qt::lightGray;
    painter.setPen(QPen(gridColor, 1, Qt::DashLine));
    for (int i = 0; i <= 5; ++i) {
        int y = margin + (plotH * i) / 5;
        painter.drawLine(margin, y, width()-margin, y);
        double val = yMax - (yMax - yMin) * i / 5;
        painter.setPen(isDarkMode ? Qt::white : Qt::black);
        painter.drawText(5, y+3, QString::number(val, 'f', 2));
        painter.setPen(QPen(gridColor, 1, Qt::DashLine));
    }
    
    // Draw data lines
    painter.setPen(QPen(isDarkMode ? QColor("#4da6ff") : Qt::blue, 2));
    for (size_t i = 1; i < xData.size() && i < yData.size(); ++i) {
        double x1 = margin + plotW * (xData[i-1] - xMin) / (xMax - xMin);
        double y1 = height()-margin - plotH * (yData[i-1] - yMin) / (yMax - yMin);
        double x2 = margin + plotW * (xData[i] - xMin) / (xMax - xMin);
        double y2 = height()-margin - plotH * (yData[i] - yMin) / (yMax - yMin);
        painter.drawLine(x1, y1, x2, y2);
    }
    
    // Draw second line if present
    if (!yData2.empty()) {
        painter.setPen(QPen(isDarkMode ? QColor("#ff4d4d") : Qt::red, 2));
        for (size_t i = 1; i < xData.size() && i < yData2.size(); ++i) {
            double x1 = margin + plotW * (xData[i-1] - xMin) / (xMax - xMin);
            double y1 = height()-margin - plotH * (yData2[i-1] - yMin) / (yMax - yMin);
            double x2 = margin + plotW * (xData[i] - xMin) / (xMax - xMin);
            double y2 = height()-margin - plotH * (yData2[i] - yMin) / (yMax - yMin);
            painter.drawLine(x1, y1, x2, y2);
        }
    }
}

// ===== MainWindow Implementation =====
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), simCtrl(nullptr) {
    simCtrl = new SimulationController();
    setupUI();
    setWindowTitle("PID Controller Simulator");
    resize(1400, 1000);
    comboTheme->setCurrentIndex(1);
    applyTheme(true);
    onRunSimulation();
}

MainWindow::~MainWindow() {
    delete simCtrl;
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // ===== LEFT PANEL: Parameters =====
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    // Theme options
    QGroupBox* themeGroup = new QGroupBox("Theme", this);
    QVBoxLayout* themeLayout = new QVBoxLayout(themeGroup);
    comboTheme = new QComboBox();
    comboTheme->addItem("Light Mode");
    comboTheme->addItem("Dark Mode");
    comboTheme->setCurrentIndex(1);
    connect(comboTheme, SIGNAL(currentIndexChanged(int)), this, SLOT(onThemeChanged(int)));
    themeLayout->addWidget(comboTheme);
    leftLayout->addWidget(themeGroup);
    
    // Signal generator
    QGroupBox* sigGroup = new QGroupBox("Signal Generator", this);
    QVBoxLayout* sigLayout = new QVBoxLayout(sigGroup);
    
    comboSignalType = new QComboBox();
    comboSignalType->addItems({"Constant", "Sine", "Square", "Triangle", "White Noise", "Saturation"});
    sigLayout->addWidget(new QLabel("Type:"));
    sigLayout->addWidget(comboSignalType);
    
    spinSigAmp = new QDoubleSpinBox();
    spinSigAmp->setRange(-1000, 1000);
    spinSigAmp->setValue(1.0);
    sigLayout->addWidget(new QLabel("Amplitude / Level / Limit:"));
    sigLayout->addWidget(spinSigAmp);
    
    spinSigPeriod = new QSpinBox();
    spinSigPeriod->setRange(0, 1000);
    spinSigPeriod->setValue(10);
    sigLayout->addWidget(new QLabel("Period:"));
    sigLayout->addWidget(spinSigPeriod);
    
    spinSigDuty = new QDoubleSpinBox();
    spinSigDuty->setRange(0.0, 1.0);
    spinSigDuty->setValue(0.5);
    sigLayout->addWidget(new QLabel("Duty Cycle:"));
    sigLayout->addWidget(spinSigDuty);
    
    btnAddSignal = new QPushButton("Add Decorator", this);
    btnAddSignal->setObjectName("btnAddSignal");
    connect(btnAddSignal, &QPushButton::clicked, this, &MainWindow::onAddSignal);
    sigLayout->addWidget(btnAddSignal);
    
    btnClearSignals = new QPushButton("Clear Signal", this);
    btnClearSignals->setObjectName("btnClearSignals");
    connect(btnClearSignals, &QPushButton::clicked, [this](){
        currentSignal = std::make_shared<BaseSignal>();
        listSignals->clear();
    });
    sigLayout->addWidget(btnClearSignals);
    
    listSignals = new QListWidget();
    listSignals->setMaximumHeight(100);
    sigLayout->addWidget(new QLabel("Current Stack:"));
    sigLayout->addWidget(listSignals);
    
    leftLayout->addWidget(sigGroup);
    
    // Controller parameters
    QGroupBox* ctrlGroup = new QGroupBox("Controller (PID)", this);
    QVBoxLayout* ctrlLayout = new QVBoxLayout(ctrlGroup);
    
    ctrlLayout->addWidget(new QLabel("Gain (k):"));
    spinK = new QDoubleSpinBox();
    spinK->setRange(0, 10);
    spinK->setValue(0.5);
    spinK->setSingleStep(0.1);
    ctrlLayout->addWidget(spinK);
    
    ctrlLayout->addWidget(new QLabel("Integral Constant (TI):"));
    spinTI = new QDoubleSpinBox();
    spinTI->setRange(0, 100);
    spinTI->setValue(10.0);
    spinTI->setSingleStep(0.5);
    ctrlLayout->addWidget(spinTI);
    
    ctrlLayout->addWidget(new QLabel("Derivative Constant (TD):"));
    spinTD = new QDoubleSpinBox();
    spinTD->setRange(0, 10);
    spinTD->setValue(0.2);
    spinTD->setSingleStep(0.1);
    ctrlLayout->addWidget(spinTD);
    
    leftLayout->addWidget(ctrlGroup);
    
    // Model parameters
    QGroupBox* modelGroup = new QGroupBox("Model (ARX)", this);
    QVBoxLayout* modelLayout = new QVBoxLayout(modelGroup);
    
    modelLayout->addWidget(new QLabel("Parameter a1:"));
    spinA1 = new QDoubleSpinBox();
    spinA1->setRange(-2, 0);
    spinA1->setValue(-0.4);
    spinA1->setSingleStep(0.1);
    modelLayout->addWidget(spinA1);
    
    modelLayout->addWidget(new QLabel("Parameter b0:"));
    spinB0 = new QDoubleSpinBox();
    spinB0->setRange(0, 2);
    spinB0->setValue(0.6);
    spinB0->setSingleStep(0.1);
    modelLayout->addWidget(spinB0);
    
    modelLayout->addWidget(new QLabel("Transport Delay:"));
    spinDelay = new QSpinBox();
    spinDelay->setRange(1, 10);
    spinDelay->setValue(1);
    modelLayout->addWidget(spinDelay);
    
    modelLayout->addWidget(new QLabel("Noise Std Dev:"));
    spinNoise = new QDoubleSpinBox();
    spinNoise->setRange(0, 1);
    spinNoise->setValue(0.0);
    spinNoise->setSingleStep(0.01);
    modelLayout->addWidget(spinNoise);
    
    leftLayout->addWidget(modelGroup);
    
    // Buttons
    btnRun = new QPushButton("Run Simulation", this);
    btnRun->setObjectName("btnRun");
    btnReset = new QPushButton("Reset", this);
    btnReset->setObjectName("btnReset");
    btnSave = new QPushButton("Save Results", this);
    btnSave->setObjectName("btnSave");

    // Default signal
    currentSignal = std::make_shared<SquareSignal>(std::make_shared<BaseSignal>(), 1.0, 50, 0.5);
    listSignals->addItem("Square(amp: 1, T: 50, dc: 0.5)");

    
    connect(btnRun, &QPushButton::clicked, this, &MainWindow::onRunSimulation);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::onResetParams);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveResults);
    
    leftLayout->addWidget(btnRun);
    leftLayout->addWidget(btnReset);
    leftLayout->addWidget(btnSave);
    leftLayout->addStretch();
    
    QWidget* leftWidget = new QWidget();
    leftWidget->setLayout(leftLayout);
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(leftWidget);
    scrollArea->setMinimumWidth(280);
    scrollArea->setMaximumWidth(320);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    // ===== RIGHT PANEL: Graphs =====
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    // Error graph
    rightLayout->addWidget(new QLabel("Error Signal"), 0);
    graphError = new GraphWidget();
    graphError->setTitle("Error Signal");
    rightLayout->addWidget(graphError, 1);
    
    // Control graph
    rightLayout->addWidget(new QLabel("Control Signal"), 0);
    graphControl = new GraphWidget();
    graphControl->setTitle("Control Signal");
    rightLayout->addWidget(graphControl, 1);
    
    // Output graph
    rightLayout->addWidget(new QLabel("Output (blue) vs Setpoint (red)"), 0);
    graphOutput = new GraphWidget();
    graphOutput->setTitle("Output vs Setpoint");
    rightLayout->addWidget(graphOutput, 1);
    
    QWidget* rightWidget = new QWidget();
    rightWidget->setLayout(rightLayout);
    
    // Main layout
    mainLayout->addWidget(scrollArea, 0);
    mainLayout->addWidget(rightWidget, 1);
}

void MainWindow::onAddSignal() {
    int typeIndex = comboSignalType->currentIndex();
    double amp = spinSigAmp->value();
    int period = spinSigPeriod->value();
    double duty = spinSigDuty->value();
    
    QString label = "";
    
    switch(typeIndex) {
        case 0: // Constant
            currentSignal = std::make_shared<ConstantSignal>(currentSignal, amp);
            label = QString("Constant(lvl: %1)").arg(amp);
            break;
        case 1: // Sine
            currentSignal = std::make_shared<SineSignal>(currentSignal, amp, period);
            label = QString("Sine(amp: %1, T: %2)").arg(amp).arg(period);
            break;
        case 2: // Square
            currentSignal = std::make_shared<SquareSignal>(currentSignal, amp, period, duty);
            label = QString("Square(amp: %1, T: %2, dc: %3)").arg(amp).arg(period).arg(duty);
            break;
        case 3: // Triangle
            currentSignal = std::make_shared<TriangleSignal>(currentSignal, amp, period, duty);
            label = QString("Triangle(amp: %1, T: %2, dc: %3)").arg(amp).arg(period).arg(duty);
            break;
        case 4: // White Noise
            currentSignal = std::make_shared<WhiteNoiseSignal>(currentSignal, amp);
            label = QString("WhiteNoise(limit: %1)").arg(amp);
            break;
        case 5: // Saturation
            currentSignal = std::make_shared<SaturationDecorator>(currentSignal, amp);
            label = QString("Saturation(limit: %1)").arg(amp);
            break;
    }
    
    listSignals->addItem(label);
}

void MainWindow::onThemeChanged(int index) {
    applyTheme(index == 1);
}

void MainWindow::applyTheme(bool dark) {
    if (dark) {
        qApp->setStyleSheet(
            "QWidget { background-color: #1e1e1e; color: #f5f5f7; font-family: -apple-system, 'Segoe UI', sans-serif; font-size: 13px; }"
            "QPushButton { background-color: #333333; border: 1px solid #444444; border-radius: 6px; padding: 6px 14px; font-weight: bold; color: #f5f5f7; }"
            "QPushButton:hover { background-color: #404040; }"
            "QPushButton:pressed { background-color: #4d4d4d; }"
            
            "QPushButton#btnRun { background-color: #1b4527; border: 1px solid #235c34; color: #4cd964; }"
            "QPushButton#btnRun:hover { background-color: #235c34; }"
            "QPushButton#btnRun:pressed { background-color: #2a7341; }"
            
            "QPushButton#btnReset { background-color: #5c1e1a; border: 1px solid #7a2822; color: #ff3b30; }"
            "QPushButton#btnReset:hover { background-color: #7a2822; }"
            "QPushButton#btnReset:pressed { background-color: #943029; }"
            
            "QPushButton#btnAddSignal, QPushButton#btnSave { background-color: #1a3d66; border: 1px solid #225188; color: #64d2ff; }"
            "QPushButton#btnAddSignal:hover, QPushButton#btnSave:hover { background-color: #225188; }"
            "QPushButton#btnAddSignal:pressed, QPushButton#btnSave:pressed { background-color: #2a65a6; }"
            
            "QPushButton#btnClearSignals { background-color: #5c3d1a; border: 1px solid #7a5122; color: #ff9f0a; }"
            "QPushButton#btnClearSignals:hover { background-color: #7a5122; }"
            "QPushButton#btnClearSignals:pressed { background-color: #946229; }"
            
            "QGroupBox { background-color: #252525; border: 1px solid #333333; border-radius: 8px; margin-top: 16px; padding-top: 12px; }"
            "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; top: 0px; color: #98989d; font-weight: bold; }"
            "QComboBox, QSpinBox, QDoubleSpinBox { background-color: #2c2c2e; border: 1px solid #3a3a3c; border-radius: 6px; padding: 5px; color: #f5f5f7; }"
            "QComboBox::drop-down { border: none; width: 22px; }"
            "QListWidget { background-color: #252525; border: 1px solid #333333; border-radius: 8px; padding: 4px; }"
            "QScrollBar:vertical { border: none; background: transparent; width: 12px; margin: 0px; }"
            "QScrollBar::handle:vertical { background: #48484a; min-height: 20px; border-radius: 6px; }"
        );
    } else {
        qApp->setStyleSheet(
            "QWidget { background-color: #f5f5f7; color: #1d1d1f; font-family: -apple-system, 'Segoe UI', sans-serif; font-size: 13px; }"
            "QPushButton { background-color: #ffffff; border: 1px solid #d1d1d6; border-radius: 6px; padding: 6px 14px; font-weight: bold; color: #1d1d1f; }"
            "QPushButton:hover { background-color: #f0f0f0; }"
            "QPushButton:pressed { background-color: #e5e5ea; }"
            
            "QPushButton#btnRun { background-color: #e4f8ea; border: 1px solid #d4f3e0; color: #248a3d; }"
            "QPushButton#btnRun:hover { background-color: #d4f3e0; }"
            "QPushButton#btnRun:pressed { background-color: #c4eed0; }"
            
            "QPushButton#btnReset { background-color: #ffe5e5; border: 1px solid #ffd4d4; color: #c9342c; }"
            "QPushButton#btnReset:hover { background-color: #ffd4d4; }"
            "QPushButton#btnReset:pressed { background-color: #ffc4c4; }"
            
            "QPushButton#btnAddSignal, QPushButton#btnSave { background-color: #e5f1ff; border: 1px solid #d4e8ff; color: #0056b3; }"
            "QPushButton#btnAddSignal:hover, QPushButton#btnSave:hover { background-color: #d4e8ff; }"
            "QPushButton#btnAddSignal:pressed, QPushButton#btnSave:pressed { background-color: #c2ddff; }"
            
            "QPushButton#btnClearSignals { background-color: #fff0e5; border: 1px solid #ffe4d4; color: #b36b00; }"
            "QPushButton#btnClearSignals:hover { background-color: #ffe4d4; }"
            "QPushButton#btnClearSignals:pressed { background-color: #ffd7c2; }"
            
            "QGroupBox { background-color: #ffffff; border: 1px solid #e5e5ea; border-radius: 8px; margin-top: 16px; padding-top: 12px; }"
            "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; top: 0px; color: #86868b; font-weight: bold; }"
            "QComboBox, QSpinBox, QDoubleSpinBox { background-color: #ffffff; border: 1px solid #d1d1d6; border-radius: 6px; padding: 5px; color: #1d1d1f; }"
            "QComboBox::drop-down { border: none; width: 22px; }"
            "QListWidget { background-color: #ffffff; border: 1px solid #e5e5ea; border-radius: 8px; padding: 4px; }"
            "QScrollBar:vertical { border: none; background: transparent; width: 12px; margin: 0px; }"
            "QScrollBar::handle:vertical { background: #d1d1d6; min-height: 20px; border-radius: 6px; }"
        );
    }
    
    graphError->setDarkMode(dark);
    graphControl->setDarkMode(dark);
    graphOutput->setDarkMode(dark);
}

void MainWindow::onRunSimulation() {
    try {
        simCtrl->setControllerParams(
            spinK->value(),
            spinTI->value(),
            spinTD->value()
        );
        
        simCtrl->setModelParams(
            spinA1->value(),
            spinB0->value(),
            spinDelay->value(),
            spinNoise->value()
        );
        
        simCtrl->setSignalGenerator(currentSignal);
        
        int steps = 100; // Hardcoded length for simulation preview
        SimulationData data = simCtrl->runSimulation(steps, 1.0);
        
        graphError->setData(data.time, data.error);
        graphControl->setData(data.time, data.control);
        graphOutput->setData(data.time, data.output, data.setpoint);
        
    } catch (std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::onResetParams() {
    spinK->setValue(0.5);
    spinTI->setValue(10.0);
    spinTD->setValue(0.2);
    spinA1->setValue(-0.4);
    spinB0->setValue(0.6);
    spinDelay->setValue(1);
    spinNoise->setValue(0.0);
    simCtrl->reset();
    currentSignal = std::make_shared<BaseSignal>();
    listSignals->clear();
}

void MainWindow::onSaveResults() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Generator Config", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;
    
    std::ofstream file(fileName.toStdString());
    if (file.is_open()) {
        file << currentSignal->serialize();
        file.close();
        QMessageBox::information(this, "Success", "Signal configuration saved successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
    }
}
        
        // Generate setpoint: 0 at t=0, then 1.0
        std::vector<double> setpoints(35, 1.0);
