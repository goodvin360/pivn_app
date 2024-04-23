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

#pragma region Docs

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
    double deltaT = 1;
    vecFill*vecData = new vecFill();
    vecFill*vecDataFile = new vecFill();

    CallerMainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
    }

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
                        res_out << vecData->resultsDb.at(i).rbegin().operator*() << " ";
                    }
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
        std::cout << "ля ля ля" << std::endl;
        if (fileName!= NULL)
        {
            std::vector<std::string> fileData;
            std::ifstream file;
            file.open(fileName.toStdString());
            std::string line;
            std::stringstream lineData;
            double t,trig,ch1,ch2,ch3,ch4 = 0;
            while (getline(file, line))
            {
                file >> t >> trig >> ch1 >> ch2 >> ch3 >> ch4;
                lineData << t << ":" << trig << ":" << ch1 << ":" << ch2 << ":" << ch3 << ":" << ch4 << "\n";
            }
            fileData.push_back(lineData.str());
            file.close();

            int counter = 0;
            std::map<double, std::string> resultsNew;
            int tempVar1;
            int tempVar2;

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
                            vecDataFile->getData(resultsNew.rbegin()->second,counter);
                            calculateFlux(false);
                            tempVar2 = resultsNew.size();
                            if (tempVar2>tempVar1)
                                counter+=1;
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
                        vecDataFile->getData(resultsNew.rbegin()->second,counter);
                        calculateFlux(false);
                        tempVar2 = resultsNew.size();
                        if (tempVar2>tempVar1)
                            counter+=1;
                    };
                }

                if (vecDataFile->resultsDb.size()>0)
                {

                    for (int k = 0; k < vecDataFile->resultsDb.at(0).size(); k++) {
                        std::stringstream res_out;
                        for (int l=0;l<vecDataFile->resultsDb.size(); l++)
                            res_out << vecDataFile->resultsDb.at(l).at(k) << " ";
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

        for (int i = 2; i < data->resultsDb.size(); i++) {
            if(data->resultsDb.at(i).rbegin().operator*() < 5 && deltaT == 1)
            {
                fluxFlag  = false;
                return;
            }
        }

        fluxFlag = true;
        fluxTime.push_back(data->resultsDb.at(0).back());

        deltaT = fluxTime.back()-fluxTime.front();

        if (deltaT==100)
        {
            deltaT = 1;
            return;
        }

        double sum;
        for (double i = fluxTime.front(); i <= fluxTime.back(); i++) {
            for (int j=2; j<data->resultsDb.size(); j++)
            {
                sum+=data->resultsDb.at(j).at(i)/(1-data->resultsDb.at(j).at(i)*5e-4);
            }
        }

//        double flux = (sum-991.22)/(coefficient*(1-exp(-(log(2)*deltaT/halfLife))));
        double flux = (sum-991.22)/(coefficient);
        textBrowser_2->setText(QString::number(flux)+'\n');
        QApplication::processEvents();
    }
};

#endif //PIVN_APP_VER_1_CALLERMAINWINDOW_H
