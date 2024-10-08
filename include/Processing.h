#ifndef PIVN_APP_PROCESSING_H
#define PIVN_APP_PROCESSING_H

#include <iostream>
#include <QLineEdit>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"

#pragma region Docs

class Processing : public QWidget {
Q_OBJECT

public: Processing(QWidget* parent = nullptr);

    ~Processing();

    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_2 = new QCheckBox();
    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_2 = nullptr;
    int clearBackVecTrig = 0;
    int multiPulsesTrig = 0;
    void inputProcessing(double &var, std::string inp);
    double critVal = 100;
    double intTime = 100;

public slots:
    void clearBack(int trig);
    void multiPulses(int trig);
    void addCriticalVal(QString val);
    void addIntTime(QString val);

};

#endif //PIVN_APP_PROCESSING_H
