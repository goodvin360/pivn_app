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
    QPushButton*pushButton_4 = new QPushButton();
    QPushButton*pushButton_5 = new QPushButton();
    QCheckBox*checkBox = new QCheckBox();
    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_2 = nullptr;

    void setConstFluxGo();
    void setConstFluxTrig(int val);
    void setEdgePointPlus();
    void setEdgePointMinus();
    void setEdgePointPlusR();
    void setEdgePointMinusR();

signals:
    void sentConstFluxGo();
    void sentConstFluxTrig(int);
    void sentEdgePointPlus();
    void sentEdgePointMinus();
    void sentEdgePointPlusR();
    void sentEdgePointMinusR();

public slots:
    void addConstFluxGo();
    void addConstFluxTrig(int val);
    void edgePointPlus();
    void edgePointMinus();
    void edgePointPlusR();
    void edgePointMinusR();

};

#endif //PIVN_APP_CONSTFLUX_H
