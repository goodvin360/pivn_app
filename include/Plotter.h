#ifndef PIVN_APP_PLOTTER_H
#define PIVN_APP_PLOTTER_H

#include "QtCharts/QChartView"
#include "QChart"
#include <QtCharts>

class Plotter
{
public:
    Plotter();
    ~Plotter();

    void PlotGraph(std::vector<std::vector<double>> &vecData);

private:
    QChart*chart;
    QChartView*chartView;
    QValueAxis*m_axisX;
    QValueAxis*m_axisY;
    QVector<QLineSeries*> lsVector;
};

#endif //PIVN_APP_PLOTTER_H
