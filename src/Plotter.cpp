#include "Plotter.h"

Plotter::Plotter() {

    chart = new QChart();
    chartView = new QChartView(chart);
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    for (int i=0; i<4; i++)
    {
        QLineSeries*series = new QLineSeries();
        lsVector.push_back(series);
    }

    for (int i=0; i<lsVector.size(); i++)
    {
        chart->addSeries(lsVector.at(i));
    }
    chart->legend()->hide();
    chart->setVisible(1);
    chart->addAxis(m_axisX, Qt::AlignBottom);
    chart->addAxis(m_axisY, Qt::AlignLeft);
    for (int i=0; i<lsVector.size(); i++)
    {
        lsVector.at(i)->attachAxis(m_axisX);
        lsVector.at(i)->attachAxis(m_axisY);
    }
};

Plotter::~Plotter() {};

void Plotter::PlotGraph(std::vector<std::vector<double>> &vecData) {

    for (int i=0; i<lsVector.size(); i++)
    {
        lsVector.at(i)->clear();
    }

    for (int k = 0; k < vecData.at(0).size(); k++) {
        for (int i=0; i<lsVector.size(); i++)
        {
            lsVector.at(i)->append(vecData.at(0).at(k),vecData.at(2+i).at(k));
        }
    }

    double max_y = *max_element(vecData.at(2).begin(), vecData.at(2).end());
    double max_x = *max_element(vecData.at(0).begin(), vecData.at(0).end());
    m_axisX->setRange(0,vecData.at(0).size()+1);
    m_axisY->setRange(0,max_y+10);


    chartView->setVisible(1);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(500,500);
    QApplication::processEvents();
}
