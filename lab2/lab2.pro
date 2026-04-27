QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = lab2_ui
TEMPLATE = app

SOURCES += \
    src/main_gui.cpp \
    src/ControllerPID.cpp \
    src/ModelArx.cpp \
    src/MainWindow.cpp \
    src/SimulationController.cpp

HEADERS += \
    inc/ControllerPID.h \
    inc/ModelArx.h \
    inc/ObjectSISO.h \
    inc/FeedbackLoop.h \
    inc/Tests.h \
    inc/Test_ControllerPID.h \
    inc/Test_ModelARX.h \
    inc/MainWindow.h \
    inc/SimulationController.h

INCLUDEPATH += inc

unix {
    LIBS += -lm
}

# JSON library (nlohmann)
INCLUDEPATH += /usr/include/nlohmann

TARGET = Controller_UI
