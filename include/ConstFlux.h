#ifndef PIVN_APP_CONSTFLUX_H
#define PIVN_APP_CONSTFLUX_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QPushButton"
#include "QtWidgets/QCheckBox"
#include "QtWidgets/QLineEdit"

#pragma region Docs

class ConstFlux : public QWidget {
Q_OBJECT

public:
    ConstFlux(QWidget *parent = nullptr);

    ~ConstFlux();

    QPushButton*pushButton = new QPushButton();
    QPushButton*pushButton_2 = new QPushButton();
    QPushButton*pushButton_3 = new QPushButton();
    QCheckBox*checkBox = new QCheckBox();
    QLineEdit*lineEdit = nullptr;

    void setConstFluxGo();
    void setConstFluxTrig(int val);
    void setEdgePointPlus();
    void setEdgePointMinus();

signals:
    void sentConstFluxGo();
    void sentConstFluxTrig(int);
    void sentEdgePointPlus();
    void sentEdgePointMinus();

public slots:
    void addConstFluxGo();
    void addConstFluxTrig(int val);
    void edgePointPlus();
    void edgePointMinus();

};

#endif //PIVN_APP_CONSTFLUX_H
