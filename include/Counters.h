#ifndef PIVN_APP_COUNTERS_H
#define PIVN_APP_COUNTERS_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QtWidgets/QCheckBox"


#pragma region Docs

class Counters : public QWidget {
Q_OBJECT

public: Counters(QWidget* parent = nullptr);

    ~Counters();

    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_2 = new QCheckBox();
    QCheckBox*checkBox_3 = new QCheckBox();
    QCheckBox*checkBox_4 = new QCheckBox();

    int cnt1_trig = 0;
    int cnt2_trig = 0;
    int cnt3_trig = 0;
    int cnt4_trig = 0;

public slots:
    void cnt1(int val);
    void cnt2(int val);
    void cnt3(int val);
    void cnt4(int val);

};

#endif //PIVN_APP_COUNTERS_H
