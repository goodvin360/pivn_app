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
    chartView->setGeometry(1500,500,500,500);
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
//            QApplication::processEvents();
        }

//    for (int i=0; i<lsVector.size(); i++)
//    {
//        for(int k = 0; k < vecData.at(0).size(); ++k) {
//            lsVector.at(i)->append(vecData.at(0).at(k),vecData.at(2+i).at(k));
//        }
//    }


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
//    chartView->setMinimumSize(500,500);

}
