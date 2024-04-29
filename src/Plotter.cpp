#include <iostream>
#include "Plotter.h"

Plotter::Plotter() {

    chart = new Chart();
    chartView = new ChartView(chart);
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    for (int i=0; i<4; i++)
    {
        QLineSeries*series = new QLineSeries();
        series->setName(QString("channel " + QString::number(i+1)));
        lsVector.push_back(series);
    }

    for (int i=0; i<lsVector.size(); i++)
    {
        chart->addSeries(lsVector.at(i));
    }
    chart->legend()->setVisible(1);
    chart->setVisible(1);
    chart->addAxis(m_axisX, Qt::AlignBottom);
    chart->addAxis(m_axisY, Qt::AlignLeft);
    for (int i=0; i<lsVector.size(); i++)
    {
        lsVector.at(i)->attachAxis(m_axisX);
        lsVector.at(i)->attachAxis(m_axisY);
    }
    m_axisX->setTickCount(10);
    m_axisX->setRange(0,100);
    m_axisY->setRange(0,10);
    chartView->setGeometry(525,500,500,500);

////////////////////////////////////////////////////////

    chartTot = new Chart();
    chartViewTot = new ChartView(chartTot);
    m_axisX_Tot = new QValueAxis();
    m_axisY_Tot = new QValueAxis();
    for (int i=0; i<2; i++)
    {
        QLineSeries*series = new QLineSeries();
        if (i==0)
            series->setName(QString("total counts"));
        if (i==1)
            series->setName(QString("background"));
        lsVectorTot.push_back(series);
    }

    for (int i=0; i<lsVectorTot.size(); i++)
    {
        chartTot->addSeries(lsVectorTot.at(i));
    }
    chartTot->legend()->setVisible(1);
    chartTot->setVisible(1);
    chartTot->addAxis(m_axisX_Tot, Qt::AlignBottom);
    chartTot->addAxis(m_axisY_Tot, Qt::AlignLeft);
    for (int i=0; i<lsVectorTot.size(); i++)
    {
        lsVectorTot.at(i)->attachAxis(m_axisX_Tot);
        lsVectorTot.at(i)->attachAxis(m_axisY_Tot);
    }
    m_axisX_Tot->setTickCount(10);
    m_axisX_Tot->setRange(0,100);
    m_axisY_Tot->setRange(0,10);
    chartViewTot->setGeometry(1525,500,500,500);
};

Plotter::~Plotter() {
//    delete chart;
//    delete chartView;
//    for (int i=0; i<lsVector.size(); i++)
//    {
//        lsVector.at(i)->clear();
//    }
};

void Plotter::PlotGraph(std::vector<std::vector<double>> &vecData) {

//    for (int i=0; i<lsVector.size(); i++)
//    {
//        lsVector.at(i)->clear();
//    }

        for (int i=0; i<lsVector.size(); i++)
        {
            QVector<QPointF> points(vecData.at(0).size());
            for(std::vector<int>::size_type l = 0; l != vecData.at(2+i).size(); ++l) {
                points[l] = QPointF(l, vecData.at(2+i)[l]);
            }
            lsVector.at(i)->replace(points);
        }


//    for (int i=0; i<lsVector.size(); i++)
//    {
//        lsVector.at(i)->append(vecData.at(0).back(),vecData.at(2+i).back());
//    }

//    if (vecData.at(0).back()<=100) {
//        max_y = *max_element(vecData.at(2).begin(), vecData.at(2).end());
//        m_axisX->setRange(0, 100);
//    }
//    else {
//        max_y = *max_element(vecData.at(2).end() - 100, vecData.at(2).end());
//        m_axisX->setRange(vecData.at(0).back()-100, vecData.at(0).back());
//    }

    max_y = *max_element(vecData.at(2).begin(), vecData.at(2).end());
    m_axisX->setRange(0,vecData.at(0).size());

    m_axisY->setRange(-1,max_y+1);

    chartView->setVisible(1);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void Plotter::PlotGraphTotal(std::vector<std::vector<double>> &vecDataTot) {

//    for (int i=0; i<lsVector.size(); i++)
//    {
//        lsVector.at(i)->clear();
//    }

    for (int i=0; i<lsVectorTot.size(); i++)
    {
        QVector<QPointF> points(vecDataTot.at(0).size());
        for(std::vector<int>::size_type l = 0; l != vecDataTot.at(1+i).size(); ++l) {
            points[l] = QPointF(l, vecDataTot.at(1+i)[l]);
        }
        lsVectorTot.at(i)->replace(points);
    }


//    for (int i=0; i<lsVectorTot.size(); i++)
//    {
//        lsVectorTot.at(i)->append(vecDataTot.at(0).back(),vecDataTot.at(2+i).back());
//    }

//    if (vecDataTot.at(0).back()<=100) {
//        max_y_tot = *max_element(vecDataTot.at(2).begin(), vecDataTot.at(2).end());
//        m_axisX_Tot->setRange(0, 100);
//    }
//    else {
//        max_y_tot = *max_element(vecDataTot.at(2).end() - 100, vecDataTot.at(2).end());
//        m_axisX_Tot->setRange(vecDataTot.at(0).back()-100, vecDataTot.at(0).back());
//    }

    max_y_tot = *max_element(vecDataTot.at(1).begin(), vecDataTot.at(1).end());
    m_axisX_Tot->setRange(0,vecDataTot.at(0).size());

    m_axisY_Tot->setRange(-1,max_y_tot+1);

    chartViewTot->setVisible(1);
    chartViewTot->setRenderHint(QPainter::Antialiasing);
}
