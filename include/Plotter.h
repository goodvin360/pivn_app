#ifndef PIVN_APP_PLOTTER_H
#define PIVN_APP_PLOTTER_H

#include "QtCharts/QChartView"
#include "QChart"
#include <QtCharts>
#include "Chart.h"
#include "ChartView.h"
#include "vecFill.h"

class Plotter
{
public:
    Plotter(vecFill*data);
    ~Plotter();

    void PlotGraph();
    void PlotGraphTotal();

private:
    Chart*chart;
    ChartView*chartView;
    QValueAxis*m_axisX;
    QValueAxis*m_axisY;
    QVector<QLineSeries*> lsVector;
    double max_y = 10;
    double max_x;

    Chart*chartTot;
    ChartView*chartViewTot;
    QValueAxis*m_axisX_Tot;
    QValueAxis*m_axisY_Tot;
    QVector<QLineSeries*> lsVectorTot;
    double max_y_tot = 10;
    vecFill*Data;
};

#endif //PIVN_APP_PLOTTER_H
