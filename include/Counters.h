#ifndef PIVN_APP_COUNTERS_H
#define PIVN_APP_COUNTERS_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QtWidgets/QCheckBox"
#include "QtWidgets/QLineEdit"

#pragma region Docs

class Counters : public QWidget {
Q_OBJECT

public: Counters(QWidget* parent = nullptr);

    ~Counters();

    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_2 = new QCheckBox();
    QCheckBox*checkBox_3 = new QCheckBox();
    QCheckBox*checkBox_4 = new QCheckBox();
    QCheckBox*checkBox_5 = new QCheckBox();
    QCheckBox*checkBox_6 = new QCheckBox();
    QCheckBox*checkBox_7 = new QCheckBox();
    QCheckBox*checkBox_8 = new QCheckBox();

    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_2 = nullptr;
    QLineEdit*lineEdit_3 = nullptr;
    QLineEdit*lineEdit_4 = nullptr;
    QLineEdit*lineEdit_5 = nullptr;
    QLineEdit*lineEdit_6 = nullptr;

    int cnt1_trig = 0;
    int cnt2_trig = 0;
    int cnt3_trig = 0;
    int cnt4_trig = 0;
    int cnt5_trig = 0;
    int cnt6_trig = 0;
    int countersNumTrig = 0;


public slots:
    void cnt1(int val);
    void cnt2(int val);
    void cnt3(int val);
    void cnt4(int val);
    void cnt5(int val);
    void cnt6(int val);
    void fourCounters(int val);
    void sixCounters(int val);
};

#endif //PIVN_APP_COUNTERS_H
