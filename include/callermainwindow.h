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
#include "ReadFromFile.h"
#include "PlotterOptions.h"
#include "TriggerMode.h"
#include "ConstFlux.h"
#include "SettingsSL.h"
#include "../ui_counters.h"
#include "../ui_coefficients.h"
#include "../ui_processing.h"
#include "../ui_readFile.h"
#include "../ui_plotter.h"
#include "../ui_trigger.h"
#include "../ui_constflux.h"

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
    Ui::Form_RF readfile;
    Ui::Form_Plot plotter;
    Ui::Form_TriggerMode trigger;
    Ui::Form_ConstFlux constFlux;

    QMainWindow*mWin = new QMainWindow();

    Counters*cntSettings;
    Coefficients*coefSettings;
    Processing*procSetting;
    ReadFromFile*readFileSettings;
    PlotterOptions*plotOptionsSetting;
    TriggerMode*triggerModeSettings;
    ConstFlux*constFluxSettings;

    SettingsSL*settingsSaver;

    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_2 = nullptr;
    QLineEdit*lineEdit_3 = nullptr;
    QLineEdit*lineEdit_4 = nullptr;
    QLineEdit*lineEdit_5 = nullptr;
    QLineEdit*lineEdit_6 = nullptr;
    QLineEdit*lineEdit_9 = nullptr;
    QLineEdit*lineEdit_10 = nullptr;
    QLineEdit*lineEdit_11 = nullptr;
    QLineEdit*lineEdit_12 = nullptr;
    QLineEdit*lineEdit_13 = nullptr;
    QLineEdit*lineEdit_14 = nullptr;
    QTextBrowser*textBrowser = nullptr;
    QTextBrowser*textBrowser_2 = nullptr;
    QTextBrowser*textBrowser_3 = nullptr;
    QTextBrowser*textBrowser_4 = nullptr;
    QTextBrowser*textBrowser_5 = nullptr;
    QComboBox*comboBox = new QComboBox();
    QPushButton*pushButton = new QPushButton();         //start
    QPushButton*pushButton_2 = new QPushButton();       //connect to COM
    QPushButton*pushButton_3 = new QPushButton();       //stop
    //pushButtons 6,9,12 - clear buttons

    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_5 = new QCheckBox();
    QCheckBox*checkBox_8 = new QCheckBox();

    bool portIsMissing = false;
    bool portHasBeenCrashed = false;
    QString pName;
    std::string portName;
    std::vector <std::string> portNames;
    QString fileName;
    int trigCOM = 0;
    double coeff_a;
    double coeff_b;
    int measTime = 5;
    bool onFlag = false;
    int plotState = 0;
    int plotTotalState = 0;
    int plotStateRough = 0;
    int plotTotalStateRough = 0;
    int rescaleTrigger = 0;
    int timeState = 0;
    double integrationTime = 30;
    double leftTime = integrationTime, leftTimeR = integrationTime;
    double totalCounts = 0;
    double nFlux = 0;
    int shotCounter = 0;
    int trigMode = 0;
    int trigVal = 0;
    double edgePoint = 0, edgePointR = 0;
    double tempTime = 0, tempTimeR = 0;
    double tempTimeShift = 0, tempTimeShiftR = 0;
    int constFluxTrig = 0;
    int edgePointTrig = 0, edgePointTrigR = 0;

    int msgVar1 = 0, msgVar11 = 0, msgVar2 = 0, msgVar22 = 0, msgVar3 = 0, msgVar33 = 0;
    std::vector<int> cntTrigValues;
    int counter = 0;
    double readDelay = 10;
    double avWindow = 5;
    int xp1tot = 500, xp2tot = 999, yp1tot = 500, yp2tot = 999;
    int xp1dif = 500, xp2dif = 999, yp1dif = 500, yp2dif = 999;
    int xp1totR = 500, xp2totR = 999, yp1totR = 500, yp2totR = 999;
    int xp1difR = 500, xp2difR = 999, yp1difR = 500, yp2difR = 999;
    bool isActive = true;
    bool isActiveTotal = true;
    bool isActiveRough = true;
    bool isActiveTotalRough = true;
    std::vector<double> resTime{};
    int count = 0;
    int mWinX, mWinY = 0;
    std::vector<QLineEdit*> countersData;
    std::vector<QCheckBox*> cntTriggers;

    vecFill*vecData = new vecFill();
    vecFill*vecDataRough = new vecFill();
    vecFill*vecDataFile = new vecFill();
    vecFill*vecDataFileRough = new vecFill();
    FluxCalc*fluxCalc = new FluxCalc();

    QTimer*m_timer;
    SerialPort*esp32;
    Plotter*makePlot;
    Plotter*makePlotRough;
    std::vector<Plotter*> plotObjVec;

    std::map<double, std::string> resultsNew;
    int tempVar1 = 0;
    int tempVar2 = 0;
    int flushCounter = 0;

    std::pair<std::string, int> dotsFind (std::string str, std::string delim);
    std::vector<std::string> fileRead (std::string str);
    void inputProcessing(double &var, std::string inp);
    void startUpFunc();
    void loadLastSettings();

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
    void plotTriggerRough(int st);
    void plotTriggerTotalRough(int stTot);
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
    void on_actionReadFile_triggered();
    void on_actionPlotOption_triggered();
    void on_actionTriggerMode_triggered();
    void on_actionConstFlux_triggered();
    void on_actionSave_settings_triggered();
    void on_actionLoad_settings_triggered();
};

#endif //PIVN_APP_VER_1_CALLERMAINWINDOW_H
