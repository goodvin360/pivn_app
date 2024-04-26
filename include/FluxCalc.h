#ifndef PIVN_APP_FLUXCALC_H
#define PIVN_APP_FLUXCALC_H

#include "QObject"
#include "iostream"
#include "vecFill.h"

class FluxCalc:public QObject{
Q_OBJECT
public:
    FluxCalc();
    ~FluxCalc();

    void printMessage(QString msg);
    void calculateFlux(vecFill& data, double trig);

    double backVal = 0;
    double backCounter = 1;

signals:
    void sentMessage(QString);

private:
    bool fluxFlag = false;
    std::vector<double>fluxTime;
    double deltaT = 0;
    double fluxTrig = 0;
    std::vector<double>backVector;
    double coefficient = 0.0004;
    double halfLife = 14.1;
};

#endif //PIVN_APP_FLUXCALC_H
