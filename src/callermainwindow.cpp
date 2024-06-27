#include <qtestsupport_core.h>
#include "callermainwindow.h"


CallerMainWindow::CallerMainWindow(QWidget *parent) : QMainWindow(parent) {
    parent = nullptr;
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(fluxCalc, &FluxCalc::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecData, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecDataFile, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &CallerMainWindow::startByTimer);
    m_timer->setInterval(1000);
}

CallerMainWindow::~CallerMainWindow() {
    std::cout << "destructor is called" << std::endl;
//    delete makePlot;
    for (int i=0; i<plotObjVec.size(); i++)
        delete plotObjVec.at(i);
}

void CallerMainWindow::startByTimer() {
    char inputData[INPUT_DATA_BYTES];
    esp32->ReadSerialPort(inputData, INPUT_DATA_BYTES);

    std::string inputValStr(inputData);
    std::cout << inputValStr << std::endl;

    std::string delimiter = "\n";
    size_t pos = 0;
    std::string token;
    std::string timeStr;
    resultsNew.clear();
    while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
        tempVar1 = resultsNew.size();
        token = inputValStr.substr(0, pos);
        if (token.length() > 0) {
            token.pop_back();
            if (dotsFind(token,":").second == 5)
            {
                resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(token,":").first), token));
                if (vecData->resultsDb.size()>0)
                {
                    fluxTrig = vecData->resultsDb.at(1).back();
                }
                tempVar2 = resultsNew.size();
                if (tempVar2>tempVar1)
                {
                    vecData->getData(resultsNew.rbegin()->second,counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig);
                    counter+=1;
                    fluxCalc->calculateFlux(*vecData, fluxTrig);
                }
            }
        }
        inputValStr.erase(0, pos + delimiter.length());
    }

    if (inputValStr!="")
    {
        tempVar1 = resultsNew.size();
        if (dotsFind(inputValStr,":").second == 5)
        {
            resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(inputValStr,":").first),inputValStr));
            {
                fluxTrig = vecData->resultsDb.at(1).back();
            }
            tempVar2 = resultsNew.size();
            if (tempVar2>tempVar1)
            {
                vecData->getData(resultsNew.rbegin()->second,counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig);
                fluxCalc->calculateFlux(*vecData, fluxTrig);
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
        vecData->getDataTotal(vecData->resultsDb, integrationTime, nFlux, coeff_a, coeff_b, true,
                              trigMode, trigVal, edgePoint, constFluxTrig, tempTime, edgePointTrig,
                              cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, avWindow, leftTime, multiPulse);
        lineEdit_10->setText(QString::number(leftTime));
        if (plotState>0)
            makePlot->PlotGraph(rescaleTrigger);
        if (plotTotalState>0)
            makePlot->PlotGraphTotal(rescaleTrigger,tempTime);

        QString showLine = QString::fromStdString(res_out.str());
        lineEdit_6->setText(QString::number(nFlux,'g',3));
        textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
        QApplication::processEvents();
        QScrollBar*sb = textBrowser->verticalScrollBar();
        sb->setValue(sb->maximum());
        QApplication::processEvents();
        flushCounter+=1;
        if (flushCounter>150)
        {
            textBrowser->clear();
            QApplication::processEvents();
            flushCounter = 0;
        }
    }

    inputValStr.clear();
    memset(inputData, 0, sizeof inputData);

    if (counter>=measTime || !onFlag)
    {
        if (m_timer->isActive())
            m_timer->stop();

        FileWriter writer;
        writer.fileWriteVec(vecData->resultsDb, "full");

        textBrowser->setText(textBrowser->toPlainText()
                             + "results stored in the file" + '\n');

        if (vecData->Flux>0) {
            QString s(0x00B1);
            double error = 0.09 * vecData->Flux;
            printMsg(QString::number(vecData->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 1);
            printMsg(QString::number(vecData->totalCntClean)+"   "+QString::number(vecData->totalCnt)+"   "+QString::number(vecData->minusBack),2);
        }

        vecData->resultsDb.clear();
        vecData->resultsDbTotal.clear();
        vecData->backVec.clear();
        vecData->backVal = 0;
        vecData->isBack = true;
        vecData->isPulse = false;
        vecData->fluxTimeCounter = 0;
        vecData->temp = 0;
        vecData->minusBack = 0;
        vecData->Flux = 0;
        vecData->plsD = 0;
        vecData->secondPulseCounter = false;

        fluxCalc->backVal = 0;
        fluxCalc->backCounter = 1;

        delete esp32;

        onFlag = false;
    }
}

void CallerMainWindow::printMsg(QString msg, int num) {
    if (num == 1) {
        shotCounter+=1;
        textBrowser_2->setText(textBrowser_2->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
    }
    if (num == 2)
        textBrowser_3->setText(textBrowser_3->toPlainText() + QString::number(shotCounter) + ": " + msg +'\n');

}

void CallerMainWindow::getCOM(QString itemName) {
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

void CallerMainWindow::connectCOM() {
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

void CallerMainWindow::addStop() {
    onFlag = false;
    QApplication::processEvents();
}

void CallerMainWindow::addStart() {
    makePlot = new Plotter(vecData);
    plotObjVec.push_back(makePlot);
    onFlag = true;
    counter = 0;
    flushCounter = 0;
    shotCounter = 0;
    textBrowser->clear();
    QApplication::processEvents();

    resultsNew.clear();
    esp32 = new SerialPort(pName.toStdString());

    m_timer->start();
}

void CallerMainWindow::setTime(QString dataEntered) {
    if (dataEntered.length()>0)
        measTime = std::stoi(dataEntered.toStdString());
}

void CallerMainWindow::setCoefA(QString coef_a)
{
    inputProcessing(coeff_a,coef_a.toStdString());
}

void CallerMainWindow::setCoefB(QString coef_b)
{
    inputProcessing(coeff_b,coef_b.toStdString());
}

void CallerMainWindow::setDist(QString dist)
{
    inputProcessing(distance,dist.toStdString());
}

void CallerMainWindow::inputProcessing(double &var, std::string inp) {
    std::string str = inp;
    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0' || str[i] > '9') && str[i]!='-') {
            return;
        }
    }
    if (inp.length()>0)
    {
        if (str[0]=='-') {
            str.erase(0, 1);
            if (str.length()>0)
                var = -1*std::stod(str);
        }
        else
            var = std::stod(str);
    }
}

std::vector<std::string> CallerMainWindow::fileRead(std::string str) {

    std::vector<std::string> fileData{};
    std::ifstream file;
    std::string line;
    size_t pos = 0;
    file.open(str);
    while (getline(file, line))
    {
        while ((pos = line.find('\t')) != std::string::npos) {
            line.replace(line.begin()+pos,line.begin()+pos+1,":");
        }
        fileData.push_back(line);
    }
    file.close();
    return fileData;
}

void CallerMainWindow::addStartFile() {
    std::cout << fileName.toStdString() << std::endl;
    if (fileName!= nullptr)
    {
        shotCounter = 0;
        makePlot = new Plotter(vecDataFile);
        plotObjVec.push_back(makePlot);
        textBrowser->setText("\n");
        QApplication::processEvents();

        std::vector<std::string> fileData{};
        fileData = fileRead(fileName.toStdString());
        onFlag = true;
        counter = 0;
        resultsNew.clear();
        tempVar1 = 0;
        tempVar2 = 0;

        for (int i=0; i<fileData.size(); i++)
        {
            if (onFlag) {
                tempVar1 = resultsNew.size();
                std::string inputValStr = fileData.at(i);
                std::string delimiter = "\n";
                size_t pos = 0;
                std::string token;
                std::string timeStr;
                while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
                    token = inputValStr.substr(0, pos);
                    if (token.length() > 0) {
                        if (dotsFind(token, ":").second == 5) {
                            resultsNew.insert(
                                    std::pair<double, std::string>(std::stod(dotsFind(token, ":").first), token));
                            if (vecDataFile->resultsDb.size() > 0) {
                                fluxTrig = vecDataFile->resultsDb.at(1).back();
                            }
                            tempVar2 = resultsNew.size();
                            if (tempVar2 > tempVar1) {
                                vecDataFile->getData(resultsNew.rbegin()->second, counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig);
                                fluxCalc->calculateFlux(*vecDataFile, fluxTrig);
                                counter += 1;
                            }
                        }
                    }
                    inputValStr.erase(0, pos + delimiter.length());
                }

                if (inputValStr != "") {
                    tempVar1 = resultsNew.size();
                    if (dotsFind(inputValStr, ":").second == 5) {
                        resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(inputValStr, ":").first),
                                                                         inputValStr));
                        if (vecDataFile->resultsDb.size() > 0) {
                            fluxTrig = vecDataFile->resultsDb.at(1).back();
                        }
                        tempVar2 = resultsNew.size();
                        if (tempVar2 > tempVar1) {
                            vecDataFile->getData(resultsNew.rbegin()->second, counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig);
                            fluxCalc->calculateFlux(*vecDataFile, fluxTrig);
                            counter += 1;
                        }
                    };
                }
                if (vecDataFile->resultsDb.size() > 0) {
                    vecDataFile->getDataTotal(vecDataFile->resultsDb, integrationTime, nFlux, coeff_a, coeff_b, false,
                                              trigMode, trigVal, edgePoint, constFluxTrig, tempTime, edgePointTrig,
                                              cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, avWindow, leftTime, multiPulse);
                    lineEdit_10->setText(QString::number(leftTime));
                    if (plotState > 0)
                        makePlot->PlotGraph(rescaleTrigger);
                    if (plotTotalState > 0)
                        makePlot->PlotGraphTotal(rescaleTrigger, tempTime);

                    std::stringstream res_out;
                    for (int k = 0; k < vecDataFile->resultsDb.size(); k++) {
                        res_out << vecDataFile->resultsDb.at(k).back() << " ";}

                        QString showLine = QString::fromStdString(res_out.str());
                        totalCounts = vecDataFile->totalCnt;
                        lineEdit_6->setText(QString::number(nFlux,'g',3));
                        textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
                        QApplication::processEvents();
                        QScrollBar*sb = textBrowser->verticalScrollBar();
                        sb->setValue(sb->maximum());
                        QApplication::processEvents();
                        flushCounter+=1;
                        if (flushCounter>150)
                        {
                            textBrowser->clear();
                            QApplication::processEvents();
                            flushCounter = 0;
                        }
                    res_out.clear();
                }
                QTest::qWait(readDelay);
            }
        }

        if (!onFlag)
        {
            if (vecDataFile->Flux>0) {
                QString s(0x00B1);
                double error = 0.09 * vecDataFile->Flux;
                printMsg(QString::number(vecDataFile->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 1);
                printMsg(QString::number(vecDataFile->totalCntClean)+"   "+QString::number(vecDataFile->totalCnt)+"   "+QString::number(vecDataFile->minusBack),2);
            }

        }

        onFlag = false;
        textBrowser->setText(textBrowser->toPlainText()+"Finished"+'\n');
        vecDataFile->resultsDb.clear();
        vecDataFile->resultsDbTotal.clear();
        vecDataFile->backVec.clear();
        vecDataFile->backVal = 0;
        vecDataFile->isBack = true;
        vecDataFile->isPulse = false;
        vecDataFile->fluxTimeCounter = 0;
        vecDataFile->temp = 0;
        vecDataFile->minusBack = 0;
        vecDataFile->Flux = 0;
        vecDataFile->plsD = 0;
        vecDataFile->secondPulseCounter = false;

        fluxCalc->backVal = 0;
        fluxCalc->backCounter = 1;
    }
}

void CallerMainWindow::addLoadFile() {
    if (!fileName.isEmpty())
        fileName = QFileDialog::getOpenFileName(this,tr("Open File"), fileName, tr("txt files (*.txt)"));
    else
        fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "../", tr("txt files (*.txt)"));
}

std::pair<std::string, int> CallerMainWindow::dotsFind(std::string str, std::string delim) {
    std::pair<std::string, int> myPair;
    int checkVar = 0;
    std::string timeStr;
    size_t pos = 0;
    while ((pos = str.find(delim)) != std::string::npos) {
        if (checkVar == 0)
        {
            timeStr = str.substr(0, pos);
        }
        str.erase(0, pos + delim.length());
        checkVar += 1;
    }
    myPair.first = timeStr;
    myPair.second = checkVar;
    return myPair;
}

void CallerMainWindow::plotTrigger(int st) {
    plotState = st;
    QApplication::processEvents();
}

void CallerMainWindow::plotTriggerTotal(int stTot) {
    plotTotalState = stTot;
    QApplication::processEvents();
}

void CallerMainWindow::rescalePlotTrigger(int stRescale) {
    rescaleTrigger = stRescale;
    QApplication::processEvents();
}

void CallerMainWindow::setFiniteTime(int stTime) {
    timeState = stTime;
    QApplication::processEvents();
    if (timeState == 0)
    {   measTime = 1e10;
        lineEdit->setText("- - -");
        lineEdit->setReadOnly(1);
    }
    else
    {
        measTime = 5;
        lineEdit->setReadOnly(0);
        lineEdit->setText(QString::number(measTime));
    }
}

void CallerMainWindow::coefTrigger(int trig) {
    coefState = trig;
    QApplication::processEvents();
    if (coefState == 0)
    {
        lineEdit_2->setReadOnly(1);
        lineEdit_3->setReadOnly(1);
    }
    else
    {
        lineEdit_2->setReadOnly(0);
        lineEdit_3->setReadOnly(0);
    }
}

void CallerMainWindow::setCountIntTime(QString intTime) {
    if (intTime.length()>0) {
        integrationTime = std::stod(intTime.toStdString());
        lineEdit_10->setText(QString::number(integrationTime));
        leftTime = integrationTime;
    }
}

void CallerMainWindow::addGoTrigger() {
    trigVal++;
}

void CallerMainWindow::autoTrigger(int st) {
    if (st>0)
    {
        trigMode = 0;
        checkBox_7->setChecked(0);
    }
    else
    {
        trigMode = 1;
        checkBox_7->setChecked(1);
    }
}

void CallerMainWindow::manualTrigger(int st) {
    if (st>0)
    {
        trigMode = 1;
        checkBox_6->setChecked(0);
    }
    else
    {
        trigMode = 0;
        checkBox_6->setChecked(1);
    }
}

void CallerMainWindow::addConstFluxGo() {
    if (constFluxTrig>0)
    {
        tempTime = edgePoint;
        lineEdit_7->setText(QString::number(tempTime));
        edgePointTrig++;
    }
}

void CallerMainWindow::addConstFluxTrig(int st) {
    constFluxTrig = st;
}

void CallerMainWindow::setEdgePoint(QString ePoint) {
    inputProcessing(edgePoint, ePoint.toStdString());
    tempTimeSet = edgePoint;
}

void CallerMainWindow::setReadDelay(QString delay) {
    inputProcessing(readDelay, delay.toStdString());
}

void CallerMainWindow::setAverageWindow(QString window) {
    inputProcessing(avWindow, window.toStdString());
}

void CallerMainWindow::addSetEdgePoint() {
    tempTime = tempTimeSet;
}

void CallerMainWindow::cnt1(int val) {
    cnt1_trig = val;
}

void CallerMainWindow::cnt2(int val) {
    cnt2_trig = val;
}

void CallerMainWindow::cnt3(int val) {
    cnt3_trig = val;
}

void CallerMainWindow::cnt4(int val) {
    cnt4_trig = val;
}

void CallerMainWindow::multiPulseTrigger(int st) {
    multiPulse = st;
}
