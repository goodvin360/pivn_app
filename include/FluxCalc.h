#ifndef PIVN_APP_FLUXCALC_H
#define PIVN_APP_FLUXCALC_H

#include "QObject"
#include "iostream"
#include "vecFill.h"
#include "testWidget.h"
#include "../ui_caller.h"

class FluxCalc:public QObject{
Q_OBJECT
public:
    FluxCalc(QWidget*parent= nullptr);
    ~FluxCalc();

    void printMessage(QString msg, int num);
    void calculateFlux(vecFill& data, double trig);

    double backVal = 0;
    double backCounter = 1;

signals:
    void sentMessage(QString, int);

private:
    bool fluxFlag = false;
    std::vector<double>fluxTime;
    double deltaT = 0;
    double fluxTrig = 0;
    std::vector<double>backVector;
    double coefficient = 0.0004;
    double halfLife = 14.1;

    Ui::MainWindow caller;

};

#endif //PIVN_APP_FLUXCALC_H
