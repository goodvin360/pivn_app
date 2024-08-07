#include <iostream>
#include "Plotter.h"

Plotter::Plotter(vecFill*data, int &xp1, int &xp2, int &yp1, int &yp2, int &xp1d, int &xp2d, int &yp1d, int &yp2d):Data(data) {

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
    chart->legend()->setVisible(true);
    chart->setVisible(true);
    chart->addAxis(m_axisX, Qt::AlignBottom);
    chart->addAxis(m_axisY, Qt::AlignLeft);
    for (int i=0; i<lsVector.size(); i++)
    {
        lsVector.at(i)->attachAxis(m_axisX);
        lsVector.at(i)->attachAxis(m_axisY);
    }
    m_axisX->setTickCount(10);
    m_axisY->setTickCount(10);
    m_axisX->setRange(0,200);
    m_axisY->setRange(0,10);
    m_axisX->setMinorTickCount(5);
    m_axisY->setMinorTickCount(5);
    m_axisX->applyNiceNumbers();
    m_axisY->applyNiceNumbers();
    chartView->setGeometry(500,500,500,500);
    chartView->setGeometry(xp1d, yp1d, xp2d-xp1d, yp2d-yp1d);

////////////////////////////////////////////////////////

    chartTot = new Chart();
    chartViewTot = new ChartView(chartTot);
    m_axisX_Tot = new QValueAxis();
    m_axisY_Tot = new QValueAxis();
    for (int i=0; i<3; i++)
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
    chartTot->legend()->setVisible(true);
    chartTot->setVisible(true);
    chartTot->addAxis(m_axisX_Tot, Qt::AlignBottom);
    chartTot->addAxis(m_axisY_Tot, Qt::AlignLeft);
    for (int i=0; i<lsVectorTot.size(); i++)
    {
        lsVectorTot.at(i)->attachAxis(m_axisX_Tot);
        lsVectorTot.at(i)->attachAxis(m_axisY_Tot);
    }
    m_axisX_Tot->setTickCount(10);
    m_axisY_Tot->setTickCount(10);
    m_axisX_Tot->setRange(0,200);
    m_axisY_Tot->setRange(0,10);
    m_axisX_Tot->setMinorTickCount(5);
    m_axisY_Tot->setMinorTickCount(5);
    m_axisX_Tot->applyNiceNumbers();
    m_axisY_Tot->applyNiceNumbers();
    chartViewTot->setGeometry(500,500,500,500);
    chartViewTot->setGeometry(xp1, yp1, xp2-xp1, yp2-yp1);
};

Plotter::~Plotter() {
    delete chart;
    delete chartView;
    delete chartTot;
    delete chartViewTot;
};

void Plotter::PlotGraph(int rescaleTrig, int &xp1, int &xp2, int &yp1, int &yp2, bool &isActive) {

        isActive = chartView->isOn;

        if (isActive) {
            std::vector<std::vector<double>> vecData = Data->resultsDb;
            chartView->geometry().getCoords(&xp1, &yp1, &xp2, &yp2);
            for (int i = 0; i < lsVector.size(); i++) {
                QVector<QPointF> points(2 * vecData.at(0).size());
                for (std::vector<int>::size_type l = 0; l != vecData.at(2 + i).size(); ++l) {
//                points[l] = QPointF(l, vecData.at(2+i)[l]);
                    for (int m = 0; m <= l; m++) {
                        if (l == m) {
                            points[l + m] = QPointF(l, vecData.at(2 + i)[l]);
                            points[l + m + 1] = QPointF(l + 1, vecData.at(2 + i)[l]);
                        }
                    }
                }
                lsVector.at(i)->replace(points);
            }

            if (rescaleTrig > 0) {
                if (vecData.at(0).back() <= 200) {
                    maxValVec.clear();
                    for (int i = 0; i < 4; i++) {
                        maxValVec.push_back(*max_element(vecData.at(i + 2).begin(), vecData.at(i + 2).end()));
                    }
                    max_y = *max_element(maxValVec.begin(), maxValVec.end());
                    m_axisX->setRange(0, 200);
                } else {
                    maxValVec.clear();
                    for (int i = 0; i < 4; i++) {
                        maxValVec.push_back(*max_element(vecData.at(i + 2).end() - 200, vecData.at(i + 2).end()));
                    }
                    max_y = *max_element(maxValVec.begin(), maxValVec.end());
                    m_axisX->setRange(vecData.at(0).back() - 200, vecData.at(0).back());
                }
                m_axisY->setRange(0, 1.2 * max_y);
                if (max_y == 0)
                    m_axisY->setRange(0, 1);
            } else {
                maxValVec.clear();
                for (int i = 0; i < 4; i++) {
                    maxValVec.push_back(*max_element(vecData.at(i + 2).begin(), vecData.at(i + 2).end()));
                }
                max_y = *max_element(maxValVec.begin(), maxValVec.end());
                m_axisX->setRange(0, vecData.at(0).size());
                m_axisY->setRange(0, 1.2 * max_y);
                if (max_y == 0)
                    m_axisY->setRange(0, 1);
            }

            chartView->setVisible(true);
            chartView->setRenderHint(QPainter::Antialiasing);
        }
    if(!chartView->isOn && !isActive) {
        chartView->isOn = true;
    }
}

void Plotter::PlotGraphTotal(int rescaleTrig, double &ePoint, int &xp1, int &xp2, int &yp1, int &yp2, bool &isActiveTot) {

    isActiveTot = chartViewTot->isOn;

    if (isActiveTot) {
        std::vector<std::vector<double>> vecDataTot = Data->resultsDbTotal;
        chartViewTot->geometry().getCoords(&xp1, &yp1, &xp2, &yp2);

        for (int i = 0; i < lsVectorTot.size(); i++) {
            QVector<QPointF> points(2 * vecDataTot.at(0).size());
            if (i < 2)
                for (std::vector<int>::size_type l = 0; l != vecDataTot.at(1 + i).size(); ++l) {
//            points[l] = QPointF(l, vecDataTot.at(1+i)[l]);
                    for (int m = 0; m <= l; m++) {
                        if (l == m) {
                            points[l + m] = QPointF(l, vecDataTot.at(1 + i)[l]);
                            points[l + m + 1] = QPointF(l + 1, vecDataTot.at(1 + i)[l]);
                        }
                    }
                }
            if (i == 2)
                for (int l = 0; l != vecDataTot.at(0).size(); ++l) {
//                points[l] = QPointF(l, 0);
//                if (l == int(ePoint))
//                    points[l] = QPointF(l, max_y_tot);

                    for (int m = 0; m <= l; m++) {
                        if (l == m) {
                            points[l + m] = QPointF(l, 0);
                            points[l + m + 1] = QPointF(l + 1, 0);
                            if (l == ePoint) {
                                if (max_y_tot == 0)
                                    max_y_tot = 1;
                                points[l + m] = QPointF(l, max_y_tot);
                                points[l + m + 1] = QPointF(l + 1, max_y_tot);
                            }
                        }
                    }
                }
            lsVectorTot.at(i)->replace(points);
        }


        if (rescaleTrig > 0) {
            if (vecDataTot.at(0).back() <= 200) {
                max_y_tot = *max_element(vecDataTot.at(1).begin(), vecDataTot.at(1).end());
                m_axisX_Tot->setRange(0, 200);
            } else {
                max_y_tot = *max_element(vecDataTot.at(1).end() - 200, vecDataTot.at(1).end());
                m_axisX_Tot->setRange(vecDataTot.at(0).back() - 200, vecDataTot.at(0).back());
            }
            m_axisY_Tot->setRange(0, 1.2 * max_y_tot);
            if (max_y_tot == 0)
                m_axisY_Tot->setRange(0, 1);
        } else {
            max_y_tot = *max_element(vecDataTot.at(1).begin(), vecDataTot.at(1).end());
            m_axisX_Tot->setRange(0, vecDataTot.at(0).size());
            m_axisY_Tot->setRange(0, 1.2 * max_y_tot);
            if (max_y_tot == 0)
                m_axisY_Tot->setRange(0, 1);
        }

        chartViewTot->setVisible(true);
        chartViewTot->setRenderHint(QPainter::Antialiasing);
    }
    if(!chartViewTot->isOn && !isActiveTot) {
        chartViewTot->isOn = true;
    }
}
