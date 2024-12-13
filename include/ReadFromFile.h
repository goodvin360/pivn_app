#ifndef PIVN_APP_READFROMFILE_H
#define PIVN_APP_READFROMFILE_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QtWidgets/QCheckBox"
#include "QtWidgets/QLineEdit"

#pragma region Docs

class ReadFromFile : public QWidget {
Q_OBJECT

public:
    ReadFromFile(QWidget *parent = nullptr);

    ~ReadFromFile();

    void printMessage();

signals:
    void sentMessage();

public slots:
    void addAction();

};

#endif //PIVN_APP_READFROMFILE_H
