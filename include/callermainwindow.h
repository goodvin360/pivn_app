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
    QTextBrowser*textBrowser = nullptr;
    QTextBrowser*textBrowser_2 = nullptr;
    QComboBox*comboBox = new QComboBox();
    QPushButton*pushButton_3 = new QPushButton();
    QPushButton*pushButton_4 = new QPushButton();
    QPushButton*pushButton_5 = new QPushButton();
    QCheckBox*checkBox = new QCheckBox();
    QString pName;
    QString fileName;
    double coefficient = 0.0004;
    int measTime = 5;
    double fluxTrig = 0;
    bool onFlag = false;
    int plotState = 0;
    int timeState = 0;

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
    void input(QString dataEntered);
    void getCOM(QString itemName);
    void addCoef(QString coef);
    void addStartFile();
    void addLoadFile();
    void plotTrigger(int st);
    void setFiniteTime(int stTime);
    void startByTimer();
};

#endif //PIVN_APP_VER_1_CALLERMAINWINDOW_H
