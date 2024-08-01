#ifndef PIVN_APP_FLUXCALC_H
#define PIVN_APP_FLUXCALC_H

#include "QObject"
#include "iostream"
#include "vecFill.h"
#include "../ui_caller.h"

class FluxCalc:public QObject{
Q_OBJECT
public:
    FluxCalc(QWidget*parent= nullptr);
    ~FluxCalc();

    void printMessage(QString msg, int num);
    void calculateFlux(vecFill& data, double trig);

signals:
    void sentMessage(QString, int);

private:

};

#endif //PIVN_APP_FLUXCALC_H
