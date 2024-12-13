#ifndef PIVN_APP_COEFFICIENTS_H
#define PIVN_APP_COEFFICIENTS_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QtWidgets/QLineEdit"
#include "QTabWidget"

#pragma region Docs

class Coefficients : public QWidget {
Q_OBJECT

public: Coefficients(QWidget* parent = nullptr);

    ~Coefficients();

    QCheckBox*checkBox = new QCheckBox(); //user's coeff trigger
    QCheckBox*checkBox_2 = new QCheckBox(); //14 MeV trigger
    QCheckBox*checkBox_3 = new QCheckBox(); // 2.5 MeV trigger
    QLineEdit*lineEdit = nullptr;   //coeff "a" line for sens chan
    QLineEdit*lineEdit_2 = nullptr; //coeff "b" line for sens chan
    QLineEdit*lineEdit_3 = nullptr; //distance line
    QLineEdit*lineEdit_4 = nullptr; //coeff "a" line for rough chan
    QLineEdit*lineEdit_5 = nullptr; //coeff "b" line for rough chan
    QTabWidget*tabWidget = new QTabWidget();


//    double coeff_a = 2671;
//    double coeff_a = 2876;
    double coeff_a = 2840;
//    double coeff_b = -450245;
//    double coeff_b = -82143;
    double coeff_b = 13354;

    double coeff_a_rough = 2840;
    double coeff_b_rough = 13354;

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
    void setCoefARough(QString coef_a);
    void setCoefBRough(QString coef_b);
    void setDist(QString dist);
};


#endif //PIVN_APP_COEFFICIENTS_H
