#ifndef PIVN_APP_TESTWIDGET_H
#define PIVN_APP_TESTWIDGET_H

#include "QMainWindow"
#include "iostream"

class testWidget : public QWidget{
    Q_OBJECT
public:
    testWidget(QWidget*parent = nullptr);
    ~testWidget();

    static double var;
    int a;
    double getVar();

public slots:
    void addButton();
};

#endif //PIVN_APP_TESTWIDGET_H
