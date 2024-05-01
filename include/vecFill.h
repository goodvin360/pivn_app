#ifndef PIVN_APP_VER_2_VECFILL_H
#define PIVN_APP_VER_2_VECFILL_H

#include "QObject"
#include "vector"
#include "string"

class vecFill: public QObject {
Q_OBJECT
public:
    vecFill(QWidget*parent= nullptr);
    ~vecFill();

    std::vector<std::vector<double>> resultsDb;

    std::vector<std::vector<double>> resultsDbTotal;

    std::vector<std::vector<double>> getData(std::string str, int counter);
    void getDataTotal(std::vector<std::vector<double>> data, double totTime, double&flux, double&coef_a, double&coef_b);

    double backVal = 0;
    std::vector<double> backVec;
    double fluxTrig = 4;
    double temp = 0;
    double fluxTime = 100;
    double fluxTimeCounter = 0;
    bool isBack = true;
    double peakVal = 0;
    double peakValClean = 0;
    double backLastVal = 0;
    double cnt = 0;
    double totalCnt = 0;
    double totalCntClean = 0;
    double minusBack = 0;
    double Flux = 0;

    void printMessage(QString msg, int num);

signals:
    void sentMessage(QString, int);

};

#endif //PIVN_APP_VER_2_VECFILL_H
