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

    void startReadFromFile();
    void startLoadFile();
    void editReadDelay(QString);

    QLineEdit*lineEdit = nullptr;

signals:
    void sentReadAction();
    void sentLoadAction();
    void sentDelayValue(QString);

public slots:
    void addReadAction();
    void addLoadAction();
    void addReadDelay(QString);

};

#endif //PIVN_APP_READFROMFILE_H
