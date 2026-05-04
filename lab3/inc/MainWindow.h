#pragma once
#include <QMainWindow>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include "SimulationController.h"

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    GraphWidget(QWidget* parent = nullptr);
    void setData(const std::vector<double>& x, const std::vector<double>& y, 
                 const std::vector<double>& y2 = std::vector<double>());
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    std::vector<double> xData, yData, yData2;
    QString title;
    
public:
    void setTitle(const QString& t) { title = t; }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void onRunSimulation();
    void onResetParams();
    void onSaveResults();
    
private:
    void setupUI();
    
    // Parameter inputs
    QDoubleSpinBox* spinK;
    QDoubleSpinBox* spinTI;
    QDoubleSpinBox* spinTD;
    QDoubleSpinBox* spinA1;
    QDoubleSpinBox* spinB0;
    QSpinBox* spinDelay;
    QDoubleSpinBox* spinNoise;
    
    // Control buttons
    QPushButton* btnRun;
    QPushButton* btnReset;
    QPushButton* btnSave;
    
    // Graph widgets
    GraphWidget* graphError;
    GraphWidget* graphControl;
    GraphWidget* graphOutput;
    
    // Data table
    QTableWidget* dataTable;
    
    SimulationController* simCtrl;
};

