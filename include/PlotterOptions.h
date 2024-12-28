#ifndef PIVN_APP_PLOTTEROPTIONS_H
#define PIVN_APP_PLOTTEROPTIONS_H

#include <iostream>
#include "QFormLayout"
#include "QWidget"
#include "QCheckBox"
#include "QtWidgets/QCheckBox"
#include "QtWidgets/QLineEdit"

#pragma region Docs

class PlotterOptions : public QWidget {
Q_OBJECT

public:
    PlotterOptions(QWidget *parent = nullptr);

    ~PlotterOptions();

    QCheckBox*checkBox = new QCheckBox();
    QCheckBox*checkBox_2 = new QCheckBox();
    QCheckBox*checkBox_3 = new QCheckBox();
    QCheckBox*checkBox_4 = new QCheckBox();
    QCheckBox*checkBox_5 = new QCheckBox();
    QCheckBox*checkBox_6 = new QCheckBox();

    void setPlotTrigVal(int val);
    void setPlotTotTrigVal(int val);
    void setPlotTrigRgVal(int val);
    void setPlotTotTrigRgVal(int val);
    void setRescaleTrigVal(int val);

signals:
    void sentPlotTrig(int val);
    void sentPlotTotTrig(int val);
    void sentPlotTrigR(int val);
    void sentPlotTotTrigR(int val);
    void sentRescaleTrig(int val);


public slots:
    void plotTrigger(int val);
    void plotTriggerTotal(int val);
    void plotTriggerRough(int val);
    void plotTriggerTotalRough(int val);
    void rescalePlotTrigger(int val);
};

#endif //PIVN_APP_PLOTTEROPTIONS_H
