#ifndef PIVN_APP_PROCESSING_H
#define PIVN_APP_PROCESSING_H

#include <iostream>
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
    int clearBackVecTrig = 0;
    int multiPulsesTrig = 0;

public slots:
    void clearBack(int trig);
    void multiPulses(int trig);

};

#endif //PIVN_APP_PROCESSING_H
