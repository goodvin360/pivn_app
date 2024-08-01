#ifndef PIVN_APP_COEFFICIENTS_H
#define PIVN_APP_COEFFICIENTS_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QtWidgets/QLineEdit"

#pragma region Docs

class Coefficients : public QWidget {
Q_OBJECT

public: Coefficients(QWidget* parent = nullptr);

    ~Coefficients();

    QCheckBox*checkBox = new QCheckBox(); //user's coeff trigger
    QCheckBox*checkBox_2 = new QCheckBox(); //14 MeV trigger
    QCheckBox*checkBox_3 = new QCheckBox(); // 2.5 MeV trigger
    QLineEdit*lineEdit = nullptr;   //coeff "a" line
    QLineEdit*lineEdit_2 = nullptr; //coeff "b" line
    QLineEdit*lineEdit_3 = nullptr; //distance line

    double coeff_a = 2671;
//    double coeff_a = 2876;
//    double coeff_a = 2840;
    double coeff_b = -450245;
//    double coeff_b = -82143;
//    double coeff_b = 13354;
    double distance = 24;
    int coefState = 0;
    int coef14 = 0;
    int coef2_5 = 0;
    void inputProcessing(double &var, std::string inp);

public slots:
    void trig14(int trig);
    void trig2_5(int trig);
    void coefTrig(int trig);
    void setCoefA(QString coef_a);
    void setCoefB(QString coef_b);
    void setDist(QString dist);
};


#endif //PIVN_APP_COEFFICIENTS_H
