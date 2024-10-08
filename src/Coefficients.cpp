#include <qtestsupport_core.h>
#include "Coefficients.h"

Coefficients::Coefficients(QWidget *parent) : QWidget(parent) {
    parent = nullptr;
}

Coefficients::~Coefficients() {}

void Coefficients::trig14(int trig) {
    coef14 = trig;
    if (trig>0) {
        checkBox_3->setChecked(false);
        coef2_5 = 0;
    }
    else {
        checkBox_3->setChecked(true);
        coef2_5 = 1;
    }
    setDist(QString::number(distance));
}

void Coefficients::trig2_5(int trig) {
    coef2_5 = trig;
    if (trig>0) {
        checkBox_2->setChecked(false);
        coef14 = 0;
    }
    else {
        checkBox_2->setChecked(true);
        coef14 = 1;
    }
    setDist(QString::number(distance));
}

void Coefficients::coefTrig(int trig) {
    coefState = trig;

    if (trig == 0)
    {
        lineEdit->setReadOnly(true);
        lineEdit_2->setReadOnly(true);
        setDist(QString::number(distance));
    }
    else
    {
        lineEdit->setReadOnly(false);
        lineEdit_2->setReadOnly(false);
    }
}

void Coefficients::setCoefA(QString coef_a) {
    inputProcessing(coeff_a,coef_a.toStdString());
}

void Coefficients::setCoefB(QString coef_b) {
    inputProcessing(coeff_b,coef_b.toStdString());
}

void Coefficients::setDist(QString dist) {
    inputProcessing(distance,dist.toStdString());
    if (coefState==0)
    {
        if (coef14>0) {
            coeff_a = distance * 15;
            coeff_b = distance + 5;
        }
        if (coef2_5>0)
        {
            coeff_a = -497 + distance*82 + pow(distance,2)*1.857;
            coeff_b = distance*4922-53842;
        }
    }
    lineEdit->setText(QString::number(coeff_a));
    lineEdit_2->setText(QString::number(coeff_b));
}

void Coefficients::inputProcessing(double &var, std::string inp) {
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
