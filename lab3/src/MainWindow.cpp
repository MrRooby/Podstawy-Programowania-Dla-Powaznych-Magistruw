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
#include <algorithm>
#include <fstream>

// ===== GraphWidget Implementation =====
GraphWidget::GraphWidget(QWidget* parent) 
    : QWidget(parent) {
    setMinimumHeight(200);
    setStyleSheet("background-color: white; border: 1px solid #ccc;");
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
    painter.fillRect(rect(), Qt::white);
    
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
    painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
    for (int i = 0; i <= 5; ++i) {
        int y = margin + (plotH * i) / 5;
        painter.drawLine(margin, y, width()-margin, y);
        double val = yMax - (yMax - yMin) * i / 5;
        painter.setPen(Qt::black);
        painter.drawText(5, y+3, QString::number(val, 'f', 2));
        painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
    }
    
    // Draw data lines
    painter.setPen(QPen(Qt::blue, 2));
    for (size_t i = 1; i < xData.size() && i < yData.size(); ++i) {
        double x1 = margin + plotW * (xData[i-1] - xMin) / (xMax - xMin);
        double y1 = height()-margin - plotH * (yData[i-1] - yMin) / (yMax - yMin);
        double x2 = margin + plotW * (xData[i] - xMin) / (xMax - xMin);
        double y2 = height()-margin - plotH * (yData[i] - yMin) / (yMax - yMin);
        painter.drawLine(x1, y1, x2, y2);
    }
    
    // Draw second line if present
    if (!yData2.empty()) {
        painter.setPen(QPen(Qt::red, 2));
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
    btnReset = new QPushButton("Reset", this);
    btnSave = new QPushButton("Save Results", this);
    
    connect(btnRun, &QPushButton::clicked, this, &MainWindow::onRunSimulation);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::onResetParams);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveResults);
    
    leftLayout->addWidget(btnRun);
    leftLayout->addWidget(btnReset);
    leftLayout->addWidget(btnSave);
    leftLayout->addStretch();
    
    QWidget* leftWidget = new QWidget();
    leftWidget->setLayout(leftLayout);
    leftWidget->setMaximumWidth(280);
    
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
    mainLayout->addWidget(leftWidget, 0);
    mainLayout->addWidget(rightWidget, 1);
}

void MainWindow::onRunSimulation() {
    try {
        // Set parameters
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
        
        // Generate setpoint: 0 at t=0, then 1.0
        std::vector<double> setpoints(35, 1.0);
        setpoints[0] = 0.0;
        
        // Run simulation
        SimulationData data = simCtrl->runSimulation(setpoints);
        
        // Update graphs
        graphError->setData(data.time, data.error);
        graphControl->setData(data.time, data.control);
        graphOutput->setData(data.time, data.output, data.setpoint);
        
        QMessageBox::information(this, "Success", "Simulation completed!");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Simulation failed: %1").arg(e.what()));
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
    
    graphError->setData(std::vector<double>(), std::vector<double>());
    graphControl->setData(std::vector<double>(), std::vector<double>());
    graphOutput->setData(std::vector<double>(), std::vector<double>());
}

void MainWindow::onSaveResults() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Results", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) return;
    
    QMessageBox::information(this, "Info", "Save feature: " + fileName);
}

