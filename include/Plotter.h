#ifndef PIVN_APP_PLOTTER_H
#define PIVN_APP_PLOTTER_H

#include "QtCharts/QChartView"
#include "QChart"
#include <QtCharts>
#include "Chart.h"
#include "ChartView.h"

class Plotter
{
public:
    Plotter();
    ~Plotter();

    void PlotGraph(std::vector<std::vector<double>> &vecData);

private:
    Chart*chart;
    ChartView*chartView;
    QValueAxis*m_axisX;
    QValueAxis*m_axisY;
    QVector<QLineSeries*> lsVector;
    double max_y = 10;
    double max_x;
};

#endif //PIVN_APP_PLOTTER_H
