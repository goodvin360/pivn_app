#ifndef PIVN_APP_PLOTTER_H
#define PIVN_APP_PLOTTER_H

#include "QtCharts/QChartView"
#include "QChart"
#include <QtCharts>
#include "Chart.h"
#include "ChartView.h"
#include "vecFill.h"
#include "vector"
#include "cmath"

class Plotter
{
public:
    Plotter(vecFill*data, int &xp1, int &xp2, int &yp1, int &yp2, int &xp1d, int &xp2d, int &yp1d, int &yp2d);
    ~Plotter();

    void PlotGraph(int rescaleTrig, int &xp1, int &xp2, int &yp1, int &yp2, bool &isActive);
    void PlotGraphTotal(int rescaleTrig, double &ePoint, int &xp1, int &xp2, int &yp1, int &yp2, bool &isActiveTot);
    Chart*chart;
    ChartView*chartView;
    Chart*chartTot;
    ChartView*chartViewTot;
private:

    QValueAxis*m_axisX;
    QValueAxis*m_axisY;
    QVector<QLineSeries*> lsVector;
    double max_y = 10;
    double max_x;
    QValueAxis*m_axisX_Tot;
    QValueAxis*m_axisY_Tot;
    QVector<QLineSeries*> lsVectorTot;
    double max_y_tot = 10;
    vecFill*Data;
    std::vector<double> maxValVec;
    int rescaleSize = 200;

};

#endif //PIVN_APP_PLOTTER_H
