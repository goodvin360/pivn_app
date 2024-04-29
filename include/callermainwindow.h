#ifndef PIVN_APP_VER_1_CALLERMAINWINDOW_H
#define PIVN_APP_VER_1_CALLERMAINWINDOW_H

#include <iostream>
#include <tchar.h>
#include "QMainWindow"
#include "QtWidgets/QLineEdit"
#include "QtWidgets/QTextBrowser"
#include "QtWidgets/QComboBox"
#include "QtWidgets/QPushButton"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include "QFileDialog"
#include "QApplication"
#include "QScrollBar"
#include "windows.h"
#include <sstream>
#include "fstream"
#include "iomanip"
#include "thread"
#include "string"
#include "SerialPort.h"
#include "map"
#include "vector"
#include "FileWriter.h"
#include "vecFill.h"
#include "math.h"
#include <QTextCodec>
#include <QTextStream>
#include "Plotter.h"
#include "FluxCalc.h"
#include "QTimer"

#pragma region Docs

using namespace QtCharts;

class CallerMainWindow : public QMainWindow {
    Q_OBJECT

public:
    CallerMainWindow(QWidget* parent = nullptr);

    ~CallerMainWindow();

    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_2 = nullptr;
    QLineEdit*lineEdit_3 = nullptr;
    QLineEdit*lineEdit_4 = nullptr;
    QLineEdit*lineEdit_5 = nullptr;
    QLineEdit*lineEdit_6 = nullptr;
    QTextBrowser*textBrowser = nullptr;
    QTextBrowser*textBrowser_2 = nullptr;
    QComboBox*comboBox = new QComboBox();
    QPushButton*pushButton_3 = new QPushButton();
    QPushButton*pushButton_4 = new QPushButton();
    QPushButton*pushButton_5 = new QPushButton();
    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_2 = new QCheckBox();
    QString pName;
    QString fileName;
    double coeff_a = 0.0004;
    double coeff_b = 300;
    double distance = 24;
    int measTime = 5;
    double fluxTrig = 0;
    bool onFlag = false;
    int plotState = 0;
    int plotTotalState = 0;
    int coefState = 0;
    int timeState = 0;
    double integrationTime = 100;
    double totalCounts = 0;

    int counter = 0;

    vecFill*vecData = new vecFill();
    vecFill*vecDataFile = new vecFill();
    FluxCalc*fluxCalc = new FluxCalc();

    QTimer*m_timer;
    SerialPort*esp32;
    Plotter*makePlot;

    std::map<double, std::string> resultsNew;
    int tempVar1 = 0;
    int tempVar2 = 0;
    int flushCounter = 0;

    std::pair<std::string, int> dotsFind (std::string str, std::string delim);
    std::vector<std::string> fileRead (std::string str);

public slots:

    void printMsg(QString msg);
    void addStop();
    void connectCOM();
    void addStart();
    void setTime(QString dataEntered);
    void getCOM(QString itemName);
    void setCoefA(QString coef_a);
    void setCoefB(QString coef_b);
    void setDist(QString dist);
    void addStartFile();
    void addLoadFile();
    void plotTrigger(int st);
    void plotTriggerTotal(int stTot);
    void coefTrigger(int trig);
    void setFiniteTime(int stTime);
    void setCountIntTime(QString intTime);
    void startByTimer();
};

#endif //PIVN_APP_VER_1_CALLERMAINWINDOW_H
