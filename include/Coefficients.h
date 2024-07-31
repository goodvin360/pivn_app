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

    QCheckBox*checkBox = new QCheckBox();
    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_2 = nullptr;
    QLineEdit*lineEdit_3 = nullptr;

public slots:


};


#endif //PIVN_APP_COEFFICIENTS_H
