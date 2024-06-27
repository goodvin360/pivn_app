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

    std::vector<std::vector<double>> getData(std::string str, int counter, int cnt1, int cnt2, int cnt3, int cnt4);
    void getDataTotal(std::vector<std::vector<double>> data, double totTime, double&flux, double&coef_a, double&coef_b,
                      bool fileParting, int trMode, int trVal, double &ePoint, int constFluxTr, double &tPoint, int constTrig,
                      int cnt1, int cnt2, int cnt3, int cnt4, int window, double&lftTime, int multiPulseTrig);

    double backVal = 0;
    std::vector<double> backVec;
    std::vector<double> backVecConst;
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
    double peakVal = 0;
    double peakValClean = 0;
    double backLastVal = 0;
    double cnt = 0;
    double totalCnt = 0;
    double totalCntClean = 0;
    double minusBack = 0;
    double Flux = 0;
    FileWriter*fileWriter;
    int fileTimer = 0;
    bool fileTrigger = false;
    double trigDelta = 0;
    double backConstWindow = 50;
    double resTime[4] = {5e-4, 5e-4, 6.05e-4, 5e-4};
    double lmd116 = log(2)/14.1;
    double lmd116m = log(2)/3240;
    double lmd114 = log(2)/72;

    bool isBackForConst = false;

    void printMessage(QString msg, int num);

signals:
    void sentMessage(QString, int);

};

#endif //PIVN_APP_VER_2_VECFILL_H
