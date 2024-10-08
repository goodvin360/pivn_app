#include <qtestsupport_core.h>
#include "Processing.h"

Processing::Processing(QWidget *parent) : QWidget(parent) {
    parent = nullptr;
}

Processing::~Processing() {}

void Processing::clearBack(int trig) {
    clearBackVecTrig = trig;
}

void Processing::multiPulses(int trig) {
    multiPulsesTrig = trig;
}

void Processing::addCriticalVal(QString val) {
    inputProcessing(critVal, val.toStdString());
}

void Processing::addIntTime(QString val) {
    inputProcessing(intTime, val.toStdString());
}

void Processing::inputProcessing(double &var, std::string inp) {
    std::string str = inp;
    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0' || str[i] > '9') && str[i]!='-') {
            return;
        }
    }
    if (inp.length()>0)
    {
        if (str[0]=='-') {
            str.erase(0, 1);
            if (str.length()>0)
                var = -1*std::stod(str);
        }
        else
            var = std::stod(str);
    }
}

