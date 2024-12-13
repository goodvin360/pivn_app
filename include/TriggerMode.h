#ifndef PIVN_APP_TRIGGERMODE_H
#define PIVN_APP_TRIGGERMODE_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QPushButton"
#include "QtWidgets/QCheckBox"
#include "QtWidgets/QLineEdit"

#pragma region Docs

class TriggerMode : public QWidget {
Q_OBJECT

public:
    TriggerMode(QWidget *parent = nullptr);

    ~TriggerMode();

    QPushButton*pushButton = new QPushButton();
    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_2 = new QCheckBox();

    void setGoTrigger();
    void setManualTrigger(int val);
    void setAutoTrigger(int val);

signals:
    void sentGoTrigger();
    void sentManualTrigger(int);
    void sentAutoTrigger(int);

public slots:
    void addGoTrigger();
    void manualTrigger(int val);
    void autoTrigger(int val);

};

#endif //PIVN_APP_TRIGGERMODE_H
