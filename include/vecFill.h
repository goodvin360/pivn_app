#ifndef PIVN_APP_VER_2_VECFILL_H
#define PIVN_APP_VER_2_VECFILL_H

#include "QObject"
#include "vector"
#include "string"
#include "FileWriter.h"

class vecFill: public QObject {
Q_OBJECT
public:
    vecFill(QWidget*parent= nullptr);
    ~vecFill();

    std::vector<std::vector<double>> resultsDb;

    std::vector<std::vector<double>> resultsDbPart;

    std::vector<std::vector<double>> resultsDbTotal;

    std::vector<std::vector<double>> getData(std::string &str, int &counter, int cnt1, int cnt2, int cnt3, int cnt4, double resT[], size_t);
    void getDataTotal(std::vector<std::vector<double>> &data, double totTime, double&flux, double&coef_a, double&coef_b,
                      bool& offTrigger, int trMode, int &trVal, double &ePoint, int constFluxTr, double &tPoint, double &tPointShift, int &constTrig,
                      int cnt1, int cnt2, int cnt3, int cnt4, int window, double&lftTime, int mPulses, int clearTrig,
                      double critLvl, double intTime);

    void cleanUp();

    double backVal = 0;
    std::vector<double> backVec;
    std::vector<double> backVecConst;
    std::vector<double> derivativeVec;
    int currentVecSize = 0;
    int previousVecSize = 0;
    double fluxTrig = 4;
    double fluxTrigConst = 0;
    double temp = 0;
    double fluxTime = 100;
    double fluxTimeCounter = 0;
    bool isBack = true;
    bool isPulse = false;
    bool isSecondPulse = false;
    bool secondPulseCounter = false;
    double plsD = 0;
    double prePulse = 0;
    std::vector<double>prePulsesData;
    std::vector<double> pulsesTime;
    double peakVal = 0;
    double peakValClean = 0;
    double backLastVal = 0;
    double firstBackVal = 0;
    double cnt = 0;
    double totalCnt = 0;
    double totalCntClean = 0;
    double totalCntFullTime = 0;
    double minusBack = 0;
    double minusBackTrue = 0;
    double Flux = 0;
    FileWriter*fileWriter;
    int fileTimer = 0;
    bool fileTrigger = false;
    double trigDelta = 0;
    double backConstWindow = 50;
    double resTime[4] = {5e-4, 5e-4, 5e-4, 5e-4};
    double lmd116 = log(2)/14.1;
    double lmd116m = log(2)/3240;
    double lmd114 = log(2)/72;
    int pulseCounter = 0;
    double startPoint = 0;
    bool derivativeSearch = false;
    double last_tPoint = 0;
    double last_edgePointShift = 0;
    bool isBackForConst = false;
    bool countStartFlag = false;
    double countStartTime = 0;
    bool countStartFlagConst = true;
    double criticalTime = 0;
    bool criticalChange = false;
    double nCritical = 0;
    double maxCountRate = 0;
    double maxResTime = 0;
    double currentTrigTime = 0, lastTrigTime = 0;
    bool truePulse = false;

    void printMessage(QString msg, int num);

signals:
    void sentMessage(QString, int);

};

#endif //PIVN_APP_VER_2_VECFILL_H
