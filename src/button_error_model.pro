#-------------------------------------------------
#
# Project created by QtCreator 2016-06-01T14:54:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    CONFIG += C++11
} else {
    QMAKE_CXXFLAGS += -std=c++0x
    CONFIG += qt
}


TARGET = button_error_model
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
