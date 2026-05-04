#pragma once
#include <QMainWindow>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QListWidget>
#include "SimulationController.h"
#include "SignalGenerator.h"

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    GraphWidget(QWidget* parent = nullptr);
    void setData(const std::vector<double>& x, const std::vector<double>& y, 
                 const std::vector<double>& y2 = std::vector<double>());
    void setDarkMode(bool dark);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    std::vector<double> xData, yData, yData2;
    QString title;
    bool isDarkMode;
    
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
    void onAddSignal();
    void onThemeChanged(int index);
    
private:
    void setupUI();
    void applyTheme(bool dark);
    
    // Parameter inputs
    QDoubleSpinBox* spinK;
    QDoubleSpinBox* spinTI;
    QDoubleSpinBox* spinTD;
    QDoubleSpinBox* spinA1;
    QDoubleSpinBox* spinB0;
    QSpinBox* spinDelay;
    QDoubleSpinBox* spinNoise;
    
    // Signal UI
    QComboBox* comboSignalType;
    QDoubleSpinBox* spinSigAmp;
    QSpinBox* spinSigPeriod;
    QDoubleSpinBox* spinSigDuty;
    QListWidget* listSignals;
    QPushButton* btnAddSignal;
    QPushButton* btnClearSignals;
    std::shared_ptr<SignalGenerator> currentSignal;
    
    // Control buttons
    QPushButton* btnRun;
    QPushButton* btnReset;
    QPushButton* btnSave;
    QComboBox* comboTheme;
    
    // Graph widgets
    GraphWidget* graphError;
    GraphWidget* graphControl;
    GraphWidget* graphOutput;
    
    // Data table
    QTableWidget* dataTable;
    
    SimulationController* simCtrl;
};

