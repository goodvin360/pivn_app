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

    std::vector<std::vector<double>> resultsDbTotal;

    std::vector<std::vector<double>> resultsDbPart;

    std::vector<std::vector<double>> resultsDbTotalP;

    std::vector<std::vector<double>> resultsDbTotalRoughP;

    std::vector<std::vector<double>> resultsDbSeparated;

    std::vector<std::vector<double>> getData(std::string &str, int &counter, std::vector<int> cntTrig, std::vector<double> resT, int);
    void getDataTotal(std::vector<std::vector<double>> &data, double totTime, double&flux, double&coef_a, double&coef_b,
                      bool& offTrigger, int trMode, int &trVal, double &ePoint, int constFluxTr, double &tPoint, double &tPointShift, int &constTrig,
                      double backDelayTime, int window, double& lftTime, int mPulses, int clearTrig,
                      double critLvl, double intTime, int nucleus, int roughTrigger, int& skipTrig);

    void cleanUp();

    double backVal = 0;
    std::vector<double> backVec;
    std::vector<double> backVecConst;
    std::vector<double> derivativeVec;
    std::vector<double>derivativeAllVec;
    std::vector<double>derivativeAllTime;
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
    std::vector<double>resTime;
    double lmd116 = log(2)/14.1;
    double lmd116m = log(2)/3240;
    double lmd114 = log(2)/72;
    double lmdAg110 = log(2)/24.56;
    double lmdAg108 = log(2)/(60*2.382);
    int pulseCounter = 0;
    double startPoint = 0;
    bool derivativeSearch = false;
    double derMax = 0;
    double derMin = 0;
    int derMaxTime = 0;
    int derMinTime = 0;
    double last_tPoint = 0;
    double last_edgePointShift = 0;
    bool isBackForConst = false;
    bool countStartFlag = false;
    double countStartTime = 0;
    double countStartTimeReal = 0;
    double countStartTimeConst = 0;
    bool countStartFlagConst = true;
    double criticalTime = 0;
    double critTimeActual = 0;
    bool criticalChange = false;
    bool isCritical = false;
    double nCritical = 0;
    double maxCountRate = 0;
    double maxResTime = 0;
    double currentTrigTime = 0, lastTrigTime = 0;
    bool truePulse = false;
    double backOut = 0;
    double statErr = 0;
    double tempErr1 = 0;
    double tempErr2 = 0;
    double countErr = 0;
    double deltaResTime = 0.2;
//    double In116m1Val = 0.002, In114Val = 0.003, Ag108Val = 0.028;    //for 4 counters (?)
    double In116m1Val = 0.010, In114Val = 0.013, Ag108Val = 0.028; //for 6 counters (?)
    QString constDataMsg;
    QString pulseDataMsg;

    void printMessage(QString msg, int num);
    void msgFillUp();
    void calcFlux(int nucleus, double intTime, double c_a, double c_b, double totTime, double &flux, double &lftTime,
                  double &backDelayTime, int var3, int var4, double var5);
    void pulseFinish(int roughTrigger, double &leftTime, double &totalTime, int clearTrig, double backDelTime);

signals:
    void sentMessage(QString, int);

};

#endif //PIVN_APP_VER_2_VECFILL_H
