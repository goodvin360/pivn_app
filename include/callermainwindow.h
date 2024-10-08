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
#include "Counters.h"
#include "Coefficients.h"
#include "Processing.h"
#include "../ui_counters.h"
#include "../ui_coefficients.h"
#include "../ui_processing.h"

#pragma region Docs

using namespace QtCharts;

class CallerMainWindow : public QMainWindow {
    Q_OBJECT

public:
    CallerMainWindow(QWidget* parent = nullptr);

    ~CallerMainWindow();

    Ui::Form_1 counters;
    Ui::Form_2 coefficients;
    Ui::Form_3 processing;


    Counters*cntSettings;
    Coefficients*coefSettings;
    Processing*procSetting;

    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_5 = nullptr;
    QLineEdit*lineEdit_6 = nullptr;
    QLineEdit*lineEdit_7 = nullptr;
    QLineEdit*lineEdit_8 = nullptr;
    QLineEdit*lineEdit_9 = nullptr;
    QLineEdit*lineEdit_10 = nullptr;
    QTextBrowser*textBrowser = nullptr;
    QTextBrowser*textBrowser_2 = nullptr;
    QTextBrowser*textBrowser_3 = nullptr;
    QComboBox*comboBox = new QComboBox();
    QPushButton*pushButton = new QPushButton();         //start
    QPushButton*pushButton_2 = new QPushButton();       //connect to COM
    QPushButton*pushButton_3 = new QPushButton();       //stop
    QPushButton*pushButton_4 = new QPushButton();       //loadFile
    QPushButton*pushButton_5 = new QPushButton();       //start from file
    QPushButton*pushButton_7 = new QPushButton();       //Go for manual trigger mode
    QPushButton*pushButton_8 = new QPushButton();       //Go for const. flux
    QPushButton*pushButton_10 = new QPushButton();      //Edge point plus
    QPushButton*pushButton_11 = new QPushButton();      //Edge point minus
    //pushButtons 6,9,12 - clear buttons

    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_3 = new QCheckBox();
    QCheckBox*checkBox_4 = new QCheckBox();
    QCheckBox*checkBox_5 = new QCheckBox();
    QCheckBox*checkBox_6 = new QCheckBox();
    QCheckBox*checkBox_7 = new QCheckBox();
    QCheckBox*checkBox_8 = new QCheckBox();

    QString pName;
    QString fileName;
    int trigCOM = 0;
    double coeff_a;
    double coeff_b;
    int measTime = 5;
    bool onFlag = false;
    int plotState = 0;
    int plotTotalState = 0;
    int rescaleTrigger = 0;
    int timeState = 0;
    double integrationTime = 100;
    double leftTime = integrationTime;
    double totalCounts = 0;
    double nFlux = 0;
    int shotCounter = 0;
    int trigMode = 0;
    int trigVal = 0;
    double edgePoint = 0;
    double tempTime = 0;
    double tempTimeShift = 0;
    int constFluxTrig = 0;
    int edgePointTrig = 0;
    int cnt1_trig = 0;
    int cnt2_trig = 0;
    int cnt3_trig = 0;
    int cnt4_trig = 0;
    int counter = 0;
    double readDelay = 10;
    double avWindow = 5;
    int xp1tot = 500, xp2tot = 999, yp1tot = 500, yp2tot = 999;
    int xp1dif = 500, xp2dif = 999, yp1dif = 500, yp2dif = 999;
    bool isActive = true;
    bool isActiveTotal = true;
    double resTime[4] {500, 500, 500, 500};
    size_t count = std::size(resTime);

    vecFill*vecData = new vecFill();
    vecFill*vecDataFile = new vecFill();
    FluxCalc*fluxCalc = new FluxCalc();

    QTimer*m_timer;
    SerialPort*esp32;
    Plotter*makePlot;
    std::vector<Plotter*> plotObjVec;

    std::map<double, std::string> resultsNew;
    int tempVar1 = 0;
    int tempVar2 = 0;
    int flushCounter = 0;

    std::pair<std::string, int> dotsFind (std::string str, std::string delim);
    std::vector<std::string> fileRead (std::string str);
    void inputProcessing(double &var, std::string inp);
    void startUpFunc();

public slots:
    void printMsg(QString msg, int num);
    void addStop();
    void connectCOM();
    void addStart();
    void setTime(QString dataEntered);
    void getCOM(QString itemName);
    void addStartFile();
    void addLoadFile();
    void plotTrigger(int st);
    void plotTriggerTotal(int stTot);
    void rescalePlotTrigger(int stRescale);
    void setFiniteTime(int stTime);
    void setCountIntTime(QString intTime);
    void startByTimer();
    void addGoTrigger();
    void autoTrigger(int st);
    void manualTrigger(int st);
    void addConstFluxGo();
    void addConstFluxTrig(int st);
    void setReadDelay(QString delay);
    void setAverageWindow(QString window);
    void edgePointPlus();
    void edgePointMinus();
    void clearNeutrons();
    void clearCounts();
    void clearMessage();
    void on_actioncounters_triggered();
    void on_actioncoefficients_triggered();
    void on_actionProcessing_triggered();
};

#endif //PIVN_APP_VER_1_CALLERMAINWINDOW_H
