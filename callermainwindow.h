#ifndef PIVN_APP_VER_1_CALLERMAINWINDOW_H
#define PIVN_APP_VER_1_CALLERMAINWINDOW_H

#include <iostream>
#include <tchar.h>
#include "QMainWindow"
#include "QtWidgets/QLineEdit"
#include "QtWidgets/QTextBrowser"
#include "QtWidgets/QComboBox"
#include "QtWidgets/QPushButton"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include "QFileDialog"
#include "QApplication"
#include "QScrollBar"
#include "windows.h"

#include <sstream>
#include "fstream"
#include "iomanip"
#include "thread"
#include "string"
#include "SerialPort.h"
#include "map"
#include "vector"
#include "FileWriter.h"
#include "vecFill.h"
#include "math.h"
#include <QTextCodec>
#include <QTextStream>
#include "QtCharts/QChartView"
#include "QChart"
#include <QtCharts>

#pragma region Docs

using namespace QtCharts;


class CallerMainWindow : public QMainWindow {
    Q_OBJECT

public:
    QLineEdit*lineEdit = nullptr;
    QLineEdit*lineEdit_2 = nullptr;
    QTextBrowser*textBrowser = nullptr;
    QTextBrowser*textBrowser_2 = nullptr;
    QComboBox*comboBox = new QComboBox();
    QPushButton*pushButton_3 = new QPushButton();
    QPushButton*pushButton_4 = new QPushButton();
    QPushButton*pushButton_5 = new QPushButton();
    QString pName;
    QString fileName;
    double coefficient = 0.0004;
    int measTime = 5;
    double halfLife = 14.1;
    bool onFlag = false;
    bool fluxFlag = false;
    std::vector<double>fluxTime;
    double deltaT = 0;
    double fluxTrig = 0;
    double backVal = 0;
    double backCounter = 1;
    std::vector<double>backVector;
    vecFill*vecData = new vecFill();
    vecFill*vecDataFile = new vecFill();
    bool helpVar = true;
    bool helpVarFile = true;

    QChart*chart = new QChart();
    QChartView*chartView = new QChartView(chart);
    QLineSeries*series1 = new QLineSeries();
    QLineSeries*series2 = new QLineSeries();
    QLineSeries*series3 = new QLineSeries();
    QLineSeries*series4 = new QLineSeries();

    CallerMainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
    }

    ~CallerMainWindow() {};

public slots:

    void addStop() {
        onFlag = false;
        QApplication::processEvents();
    };

    void connectCOM() {
        SerialPort esp32(pName.toStdString());

        if (esp32.isConnected())
        {
            textBrowser->setText(textBrowser->toPlainText()+"connected to "+pName+'\n');
            QApplication::processEvents();
        }
        else
        {
            return;
        }
    }

    void addStart() {

        onFlag = true;
        char inputData[INPUT_DATA_BYTES];

        int endTime = measTime;
        int counter = 0;

        SerialPort esp32(pName.toStdString());

        std::map<double, std::string> resultsNew;
        int tempVar1;
        int tempVar2;
        while (esp32.isConnected() && counter < endTime && onFlag)
        {
            series1->clear();
            series2->clear();
            series3->clear();
            series4->clear();

            esp32.ReadSerialPort(inputData, INPUT_DATA_BYTES);
            std::string inputValStr(inputData);
            std::cout << inputValStr << std::endl;

            std::string delimiter = "\n";
            size_t pos = 0;
            std::string token;
            std::string timeStr;

            while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
                tempVar1 = resultsNew.size();
                token = inputValStr.substr(0, pos);
                if (token.length() > 0) {
                    int checkVar = 0;
                    std::string checkStr = token;
                    std::string delimiter3 = ":";
                    size_t pos = 0;
                    while ((pos = checkStr.find(delimiter3)) != std::string::npos) {
                        if (checkVar == 0)
                        {
                            timeStr = checkStr.substr(0, pos);
                        }
                        checkStr.erase(0, pos + delimiter3.length());
                        checkVar += 1;
                    }
                    token.pop_back();
                    if (checkVar == 5)
                    {
                        resultsNew.insert(std::pair<double, std::string>(std::stod(timeStr), token));
                        if (vecData->resultsDb.size()>0)
                        {
                            fluxTrig = vecData->resultsDb.at(1).back();
                        }
                        tempVar2 = resultsNew.size();
                        if (tempVar2>tempVar1)
                        {
                            vecData->getData(resultsNew.rbegin()->second,counter);
                            counter+=1;
                            calculateFlux(true);
                        }
                    }
                }
                inputValStr.erase(0, pos + delimiter.length());
            }

            if (inputValStr!="")
            {
                tempVar1 = resultsNew.size();
                int checkVar = 0;
                std::string checkStr = inputValStr;
                std::string delimiter3 = ":";
                size_t pos = 0;
                while ((pos = checkStr.find(delimiter3)) != std::string::npos) {
                    if (checkVar==0)
                    {
                        timeStr = checkStr.substr(0, pos);
                    }
                    checkStr.erase(0, pos + delimiter3.length());
                    checkVar+=1;
                }
                if (checkVar==5)
                {
                    resultsNew.insert(std::pair<double, std::string>(std::stod(timeStr),inputValStr));
                    {
                        fluxTrig = vecData->resultsDb.at(1).back();
                    }
                    tempVar2 = resultsNew.size();
                    if (tempVar2>tempVar1)
                    {
                        vecData->getData(resultsNew.rbegin()->second,counter);
                        calculateFlux(true);
                        counter+=1;
                    }
                };
            }

            if (vecData->resultsDb.size()>0)
            {
                std::stringstream res_out;
                for (int i = 0; i < vecData->resultsDb.size(); i++) {
                        res_out << vecData->resultsDb.at(i).back() << " ";
                    }

                for (int k = 0; k < vecData->resultsDb.at(0).size(); k++) {
                    series1->append(vecData->resultsDb.at(0).at(k),vecData->resultsDb.at(2).at(k));
                    series2->append(vecData->resultsDb.at(0).at(k),vecData->resultsDb.at(3).at(k));
                    series3->append(vecData->resultsDb.at(0).at(k),vecData->resultsDb.at(4).at(k));
                    series4->append(vecData->resultsDb.at(0).at(k),vecData->resultsDb.at(5).at(k));
                }

                chart->legend()->hide();
                chart->setVisible(1);

                if (helpVar)
                {chart->addSeries(series1);
                    chart->addSeries(series2);
                    chart->addSeries(series3);
                    chart->addSeries(series4);
                    helpVar = false;}
                chart->createDefaultAxes();
                double max_y = *max_element(vecData->resultsDb.at(2).begin(), vecData->resultsDb.at(2).end());
                double max_x = *max_element(vecData->resultsDb.at(0).begin(), vecData->resultsDb.at(0).end());
                chart->axes(Qt::Vertical).first()->setRange(0, 1.2*max_y);
                chart->axes(Qt::Horizontal).first()->setRange(0, max_x+2);
                chartView->setVisible(1);
                chartView->setRenderHint(QPainter::Antialiasing);
                chartView->setMinimumSize(500,500);
                QApplication::processEvents();

                QString showLine = QString::fromStdString(res_out.str());

                textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
                QApplication::processEvents();
                QScrollBar*sb = textBrowser->verticalScrollBar();
                sb->setValue(sb->maximum());
                QApplication::processEvents();
            }

            inputValStr.clear();
            memset(inputData, 0, sizeof inputData);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        FileWriter writer;
        writer.fileWriteVec(vecData->resultsDb);

        textBrowser->setText(textBrowser->toPlainText()
                             +"results stored in the file"+'\n');
        vecData->resultsDb.clear();
        backVal = 0;
        backCounter = 1;
    };

    void input(QString dataEntered) {
        if (dataEntered.length()>0)
            measTime = std::stoi(dataEntered.toStdString());
    }

    void getComm(QString itemName) {

        std::string tempStr = itemName.toStdString();
        for (int i=0; i<tempStr.size(); i++)
        {
            if (tempStr[i]<'0'|| tempStr[i]>'9')
                tempStr.erase(0, tempStr.find(tempStr[i])+1);
        }

        int portNum = std::stoi(tempStr);

        std::stringstream portString;
        if (portNum>1 && portNum<=9)
        {
            portString << "COM" << portNum;
        }
        else if (portNum>9)
        {
            portString << "\\\\.\\COM" << portNum;
        }

        pName = QString::fromStdString(portString.str());
    }

    void addCoef(QString coef) {
        if (coef.length()>0)
            coefficient = std::stod(coef.toStdString());
    }

    void addStartFile() {
        std::cout << fileName.toStdString() << std::endl;
        if (fileName!= NULL)
        {
            std::vector<std::string> fileData{};
            std::ifstream file;
            std::string line;
            std::stringstream lineData;
            double t,trig,ch1,ch2,ch3,ch4 = 0;
            file.open(fileName.toStdString());
            while (getline(file, line))
            {
                file >> t >> trig >> ch1 >> ch2 >> ch3 >> ch4;
                lineData << t << ":" << trig << ":" << ch1 << ":" << ch2 << ":" << ch3 << ":" << ch4 << "\n";
            }
            fileData.push_back(lineData.str());
            file.close();

            int counter = 0;
            std::map<double, std::string> resultsNew{};
            int tempVar1 = 0;
            int tempVar2 = 0;

            for (int i=0; i<fileData.size(); i++)
            {
                tempVar1 = resultsNew.size();
                std::string inputValStr = fileData.at(i);

//                std::cout << inputValStr << std::endl;

                std::string delimiter = "\n";
                size_t pos = 0;
                std::string token;
                std::string timeStr;
                while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
                    token = inputValStr.substr(0, pos);
                    if (token.length() > 0) {
                        int checkVar = 0;
                        std::string checkStr = token;
                        std::string delimiter3 = ":";
                        size_t pos = 0;
                        while ((pos = checkStr.find(delimiter3)) != std::string::npos) {
                            if (checkVar == 0)
                            {
                                timeStr = checkStr.substr(0, pos);
                            }
                            checkStr.erase(0, pos + delimiter3.length());
                            checkVar += 1;
                        }
//                        token.pop_back();
                        if (checkVar == 5)
                        {
                            resultsNew.insert(std::pair<double, std::string>(std::stod(timeStr), token));
                            if (vecDataFile->resultsDb.size()>0)
                            {
                                fluxTrig = vecDataFile->resultsDb.at(1).back();
                            }
                            tempVar2 = resultsNew.size();
                            if (tempVar2>tempVar1)
                            {
                                vecDataFile->getData(resultsNew.rbegin()->second,counter);
                                calculateFlux(false);
                                counter+=1;
                            }
                        }
                    }
                    inputValStr.erase(0, pos + delimiter.length());
                }

                if (inputValStr!="")
                {
                    int checkVar = 0;
                    std::string checkStr = inputValStr;
                    std::string delimiter3 = ":";
                    size_t pos = 0;
                    while ((pos = checkStr.find(delimiter3)) != std::string::npos) {
                        if (checkVar==0)
                        {
                            timeStr = checkStr.substr(0, pos);
                        }
                        checkStr.erase(0, pos + delimiter3.length());
                        checkVar+=1;
                    }
                    if (checkVar==5)
                    {
                        resultsNew.insert(std::pair<double, std::string>(std::stod(timeStr),inputValStr));
                        if (vecDataFile->resultsDb.size()>0)
                        {
                            fluxTrig = vecDataFile->resultsDb.at(1).back();
                        }
                        tempVar2 = resultsNew.size();
                        if (tempVar2>tempVar1)
                        {
                            vecDataFile->getData(resultsNew.rbegin()->second,counter);
                            calculateFlux(false);
                            counter+=1;
                        }
                    };
                }

                if (vecDataFile->resultsDb.size()>0)
                {
                    series1->clear();
                    series2->clear();
                    series3->clear();
                    series4->clear();

                    for (int k = 0; k < vecDataFile->resultsDb.at(0).size(); k++) {
                        series1->append(vecDataFile->resultsDb.at(0).at(k),vecDataFile->resultsDb.at(2).at(k));
                        series2->append(vecDataFile->resultsDb.at(0).at(k),vecDataFile->resultsDb.at(3).at(k));
                        series3->append(vecDataFile->resultsDb.at(0).at(k),vecDataFile->resultsDb.at(4).at(k));
                        series4->append(vecDataFile->resultsDb.at(0).at(k),vecDataFile->resultsDb.at(5).at(k));
                    }
                    chart->legend()->hide();
                    chart->setVisible(1);

                    if (helpVarFile)
                    {chart->addSeries(series1);
                    chart->addSeries(series2);
                    chart->addSeries(series3);
                    chart->addSeries(series4);
                    helpVarFile = false;}
                    chart->createDefaultAxes();
                    double max = *max_element(vecDataFile->resultsDb.at(2).begin(), vecDataFile->resultsDb.at(2).end());
                    chart->axes(Qt::Vertical).first()->setRange(0, 1.2*max);
                    chartView->setVisible(1);
                    chartView->setRenderHint(QPainter::Antialiasing);
                    chartView->setMinimumSize(500,500);

                    for (int k = 0; k < vecDataFile->resultsDb.at(0).size(); k++) {
                        std::stringstream res_out;
                        for (int l=0;l<vecDataFile->resultsDb.size(); l++)
                        {
                            res_out << vecDataFile->resultsDb.at(l).at(k) << " ";
                        }
                        QString showLine = QString::fromStdString(res_out.str());

                        textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
                        QApplication::processEvents();
                        QScrollBar*sb = textBrowser->verticalScrollBar();
                        sb->setValue(sb->maximum());
                        QApplication::processEvents();
                        res_out.clear();
                    }
                }

                inputValStr.clear();
//                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        vecDataFile->resultsDb.clear();
        backVal = 0;
        backCounter = 1;
    }

    void addLoadFile() {
        fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "../", tr("txt files (*.txt)"));
    }

    void calculateFlux(bool flag) {

        vecFill*data = new vecFill;

        if (flag)
            data = vecData;
        else
            data = vecDataFile;

        if (data->resultsDb.at(1).back()>fluxTrig)
        {
            fluxFlag = true;
        }

        if(deltaT == 0 && !fluxFlag)
        {
//            backVal = 0;
            for (int i = 2; i < data->resultsDb.size(); i++)
            {
                backVal+=data->resultsDb.at(i).back()/(1-data->resultsDb.at(i).back()*5e-4);
            }

            if (backVector.size()<5)
                backVector.push_back(backVal);
            else
            {
                backVector.erase(backVector.begin());
                backVector.push_back(backVal);
            }

            backVal = backVal/backCounter;
            backCounter+=1;

            /*if (backVector.size()>0)
            {
                double backSum=0;
                for (int i=0; i<backVector.size(); i++)
                    backSum+=backVector.at(i);

                backVal = backSum/backVector.size();
                std::cout << backVal << std::endl;
            }*/

            return;
        }

//        for (int i = 2; i < data->resultsDb.size(); i++) {
//            if(data->resultsDb.at(i).rbegin().operator*() < 5 && deltaT == 1 && !fluxFlag)
//            {
//                return;
//            }
//        }


        if (deltaT>=100)
        {
            deltaT = 0;
            fluxFlag  = false;
            fluxTime.clear();
            backVal = 0;
            backCounter = 1;
            backVector.clear();
            return;
        }

        fluxTime.push_back(data->resultsDb.at(0).back());
        deltaT = fluxTime.back()-fluxTime.front();

        double sum = 0;
        for (int i = fluxTime.front(); i <= fluxTime.back(); i++) {
            for (int j=2; j<data->resultsDb.size(); j++)
            {
                sum+=data->resultsDb.at(j).at(i)/(1-data->resultsDb.at(j).at(i)*5e-4)-backVal;
            }
        }

//        double flux = (sum-991.22)/(coefficient*(1-exp(-(log(2)*deltaT/halfLife))));
        double flux = (sum-300)/(coefficient);
        textBrowser_2->setText(QString::number(flux)+'\n');
        QApplication::processEvents();
        sum = 0;
        flux = 0;
    }
};

#endif //PIVN_APP_VER_1_CALLERMAINWINDOW_H
