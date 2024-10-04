#include <qtestsupport_core.h>
#include "callermainwindow.h"


CallerMainWindow::CallerMainWindow(QWidget *parent) : QMainWindow(parent) {
    parent = nullptr;

    cntSettings = new Counters();
    counters.setupUi(cntSettings);

    coefSettings = new Coefficients();
    coefficients.setupUi(coefSettings);

    startUpFunc();

    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(fluxCalc, &FluxCalc::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecData, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecDataFile, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &CallerMainWindow::startByTimer);
    m_timer->setInterval(1000);
}

CallerMainWindow::~CallerMainWindow() {
    for (int i=0; i<plotObjVec.size(); i++)
        delete plotObjVec.at(i);
    cntSettings->close();
    coefSettings->close();
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

    cnt1_trig = cntSettings->cnt1_trig;
    cnt2_trig = cntSettings->cnt2_trig;
    cnt3_trig = cntSettings->cnt3_trig;
    cnt4_trig = cntSettings->cnt4_trig;

    resultsNew.clear();
    while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
        tempVar1 = resultsNew.size();
        token = inputValStr.substr(0, pos);
        if (token.length() > 0) {
            token.pop_back();
            if (dotsFind(token,":").second == 5)
            {
                resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(token,":").first), token));
                tempVar2 = resultsNew.size();
                if (tempVar2>tempVar1)
                {
                    vecData->getData(resultsNew.rbegin()->second,counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, resTime, count);
                    counter+=1;
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
            tempVar2 = resultsNew.size();
            if (tempVar2>tempVar1)
            {
                vecData->getData(resultsNew.rbegin()->second,counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, resTime, count);
                counter+=1;
            }
        }
    }

    if (vecData->resultsDb.size()>0)
    {
        std::stringstream res_out;
        for (int i = 0; i < vecData->resultsDb.size(); i++) {
            res_out << vecData->resultsDb.at(i).back() << " ";
        }

        vecData->getDataTotal(vecData->resultsDb, integrationTime, nFlux, coeff_a, coeff_b, true,
                              trigMode, trigVal, edgePoint, constFluxTrig, tempTime, tempTimeShift, edgePointTrig,
                              cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, avWindow, leftTime);
        lineEdit_10->setText(QString::number(leftTime));
        lineEdit_7->setText(QString::number(tempTime));
        if (plotState>0 && isActive)
            makePlot->PlotGraph(rescaleTrigger, xp1dif, xp2dif, yp1dif, yp2dif, isActive);
        if (!isActive) {
            plotState=0;
            checkBox_3->setChecked(0);
        }
        if (plotTotalState>0 && isActiveTotal)
            makePlot->PlotGraphTotal(rescaleTrigger,tempTime, xp1tot, xp2tot, yp1tot, yp2tot, isActiveTotal);
        if (!isActiveTotal) {
            plotTotalState=0;
            checkBox_4->setChecked(0);
        }
        QString showLine = QString::fromStdString(res_out.str());
        lineEdit_6->setText(QString::number(nFlux,'g',3));
        textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
        QScrollBar*sb = textBrowser->verticalScrollBar();
        sb->setValue(sb->maximum());
        flushCounter+=1;
        if (flushCounter>150)
        {
            textBrowser->clear();
            flushCounter = 0;
        }
    }

    inputValStr.clear();
    memset(inputData, 0, sizeof inputData);

    FileWriter writerLog;
    writerLog.fileWriteVec(vecData->resultsDb, "log");

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

        vecData->cleanUp();
        edgePointTrig = 0;
        tempTime=0;
        tempTimeShift = 0;

        delete esp32;

        onFlag = false;
    }
}

void CallerMainWindow::printMsg(QString msg, int num) {
    if (num == 1) {
        shotCounter+=1;
        textBrowser_2->setText(textBrowser_2->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_2->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
    if (num == 2) {
        textBrowser_3->setText(textBrowser_3->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_3->verticalScrollBar();
        sb->setValue(sb->maximum());
    }

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
    }
    else
    {
        return;
    }
}

void CallerMainWindow::addStop() {
    onFlag = false;
}

void CallerMainWindow::addStart() {
    makePlot = new Plotter(vecData, xp1tot, xp2tot, yp1tot, yp2tot, xp1dif, xp2dif, yp1dif, yp2dif);
    plotObjVec.push_back(makePlot);
    onFlag = true;
    counter = 0;
    flushCounter = 0;
    shotCounter = 0;
    resTime[0]=std::stod(cntSettings->lineEdit->text().toStdString());
    resTime[1]=std::stod(cntSettings->lineEdit_2->text().toStdString());
    resTime[2]=std::stod(cntSettings->lineEdit_3->text().toStdString());
    resTime[3]=std::stod(cntSettings->lineEdit_4->text().toStdString());

    coeff_a=coefSettings->coeff_a;
    coeff_b=coefSettings->coeff_b;

    textBrowser->clear();

    resultsNew.clear();
    esp32 = new SerialPort(pName.toStdString());

    m_timer->start();
}

void CallerMainWindow::setTime(QString dataEntered) {
    if (dataEntered.length()>0)
        measTime = std::stoi(dataEntered.toStdString());
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
        makePlot = new Plotter(vecDataFile, xp1tot, xp2tot, yp1tot, yp2tot, xp1dif, xp2dif, yp1dif, yp2dif);
        plotObjVec.push_back(makePlot);
        textBrowser->setText("\n");

        resTime[0]=std::stod(counters.lineEdit->text().toStdString());
        resTime[1]=std::stod(counters.lineEdit_2->text().toStdString());
        resTime[2]=std::stod(counters.lineEdit_3->text().toStdString());
        resTime[3]=std::stod(counters.lineEdit_4->text().toStdString());

        coeff_a=coefSettings->coeff_a;
        coeff_b=coefSettings->coeff_b;

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
                cnt1_trig = cntSettings->cnt1_trig;
                cnt2_trig = cntSettings->cnt2_trig;
                cnt3_trig = cntSettings->cnt3_trig;
                cnt4_trig = cntSettings->cnt4_trig;

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
                            tempVar2 = resultsNew.size();
                            if (tempVar2 > tempVar1) {
                                vecDataFile->getData(resultsNew.rbegin()->second, counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, resTime, count);
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
                        tempVar2 = resultsNew.size();
                        if (tempVar2 > tempVar1) {
                            vecDataFile->getData(resultsNew.rbegin()->second, counter, cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, resTime, count);
                            counter += 1;
                        }
                    };
                }
                if (vecDataFile->resultsDb.size() > 0) {
                    vecDataFile->getDataTotal(vecDataFile->resultsDb, integrationTime, nFlux, coeff_a, coeff_b, false,
                                              trigMode, trigVal, edgePoint, constFluxTrig, tempTime, tempTimeShift, edgePointTrig,
                                              cnt1_trig, cnt2_trig, cnt3_trig, cnt4_trig, avWindow, leftTime);
                    lineEdit_10->setText(QString::number(leftTime));
                    lineEdit_7->setText(QString::number(tempTime));
                    if (plotState > 0 && isActive)
                        makePlot->PlotGraph(rescaleTrigger, xp1dif, xp2dif, yp1dif, yp2dif, isActive);
                    if (!isActive) {
                        plotState=0;
                        checkBox_3->setChecked(0);
                    }
                    if (plotTotalState > 0 && isActiveTotal)
                        makePlot->PlotGraphTotal(rescaleTrigger, tempTime, xp1tot, xp2tot, yp1tot, yp2tot, isActiveTotal);
                    if (!isActiveTotal) {
                        plotTotalState=0;
                        checkBox_4->setChecked(0);
                    }
                    std::stringstream res_out;
                    for (int k = 0; k < vecDataFile->resultsDb.size(); k++) {
                        res_out << vecDataFile->resultsDb.at(k).back() << " ";}

                        QString showLine = QString::fromStdString(res_out.str());
                        totalCounts = vecDataFile->totalCnt;
                        lineEdit_6->setText(QString::number(nFlux,'g',3));
                        textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
                        QScrollBar*sb = textBrowser->verticalScrollBar();
                        sb->setValue(sb->maximum());
                        flushCounter+=1;
                        if (flushCounter>150)
                        {
                            textBrowser->clear();
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

            textBrowser->setText(textBrowser->toPlainText()+"Finished"+'\n');
            vecDataFile->cleanUp();
            edgePointTrig = 0;
            tempTime=0;
            tempTimeShift = 0;
        }

        if (onFlag) {
            onFlag = false;
            textBrowser->setText(textBrowser->toPlainText() + "Finished" + '\n');
            vecDataFile->cleanUp();
            edgePointTrig = 0;
            tempTime=0;
            tempTimeShift = 0;
        }
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
    if (!plotObjVec.empty())
        plotObjVec.back()->chartView->isOn=true;
    if (!isActive && st >0)
        isActive = true;
    if (st == 0) {
        isActive = false;
        if (!plotObjVec.empty()) {
            plotObjVec.back()->chartView->close();
            plotObjVec.back()->chartView->isOn = true;
        }
    }
}

void CallerMainWindow::plotTriggerTotal(int stTot) {
    plotTotalState = stTot;
    if (!plotObjVec.empty())
        plotObjVec.back()->chartViewTot->isOn=true;
    if (!isActiveTotal && stTot>0)
        isActiveTotal = true;
    if (stTot == 0) {
        isActiveTotal = false;
        if (!plotObjVec.empty()) {
            plotObjVec.back()->chartViewTot->close();
            plotObjVec.back()->chartViewTot->isOn = true;
        }
    }
}

void CallerMainWindow::rescalePlotTrigger(int stRescale) {
    rescaleTrigger = stRescale;
}

void CallerMainWindow::setFiniteTime(int stTime) {
    timeState = stTime;
    if (timeState == 0)
    {   measTime = 1e9;
        lineEdit->setText("- - -");
        lineEdit->setReadOnly(true);
    }
    else
    {
        measTime = 5;
        lineEdit->setReadOnly(false);
        lineEdit->setText(QString::number(measTime));
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
        checkBox_7->setChecked(false);
    }
    else
    {
        trigMode = 1;
        checkBox_7->setChecked(true);
    }
}

void CallerMainWindow::manualTrigger(int st) {
    if (st>0)
    {
        trigMode = 1;
        checkBox_6->setChecked(false);
    }
    else
    {
        trigMode = 0;
        checkBox_6->setChecked(true);
    }
}

void CallerMainWindow::addConstFluxGo() {
        edgePointTrig++;
}

void CallerMainWindow::addConstFluxTrig(int st) {
    constFluxTrig = st;
}

void CallerMainWindow::setReadDelay(QString delay) {
    inputProcessing(readDelay, delay.toStdString());
}

void CallerMainWindow::setAverageWindow(QString window) {
    inputProcessing(avWindow, window.toStdString());
}

void CallerMainWindow::edgePointPlus() {
    tempTimeShift++;
    lineEdit_7->setText(QString::number(tempTime));
}

void CallerMainWindow::edgePointMinus() {
    tempTimeShift--;
    lineEdit_7->setText(QString::number(tempTime));
}

void CallerMainWindow::on_actioncounters_triggered()
{
    cntSettings->show();
}

void CallerMainWindow::on_actioncoefficients_triggered()
{
    coefSettings->show();
}

void CallerMainWindow::startUpFunc() {
    cntSettings->lineEdit = counters.lineEdit;
    cntSettings->lineEdit_2 = counters.lineEdit_2;
    cntSettings->lineEdit_3 = counters.lineEdit_3;
    cntSettings->lineEdit_4 = counters.lineEdit_4;
    cntSettings->checkBox = counters.checkBox;
    cntSettings->checkBox_2 = counters.checkBox_2;
    cntSettings->checkBox_3 = counters.checkBox_3;
    cntSettings->checkBox_4 = counters.checkBox_4;

    counters.checkBox->setChecked(true);
    counters.checkBox_2->setChecked(true);
    counters.checkBox_3->setChecked(true);
    counters.checkBox_4->setChecked(true);
    counters.lineEdit->setText(QString::number(resTime[0]));
    counters.lineEdit_2->setText(QString::number(resTime[1]));
    counters.lineEdit_3->setText(QString::number(resTime[2]));
    counters.lineEdit_4->setText(QString::number(resTime[3]));

    coefSettings->lineEdit = coefficients.lineEdit;
    coefSettings->lineEdit_2 = coefficients.lineEdit_2;
    coefSettings->lineEdit_3 = coefficients.lineEdit_3;
    coefSettings->checkBox = coefficients.checkBox;
    coefSettings->checkBox_2 = coefficients.checkBox_2;
    coefSettings->checkBox_3 = coefficients.checkBox_3;

    coefficients.checkBox->setChecked(true);
    coefficients.checkBox_3->setChecked(true);
    coefficients.lineEdit->setText(QString::number(coefSettings->coeff_a));
    coefficients.lineEdit_2->setText(QString::number(coefSettings->coeff_b));
    coefficients.lineEdit_3->setText(QString::number(coefSettings->distance));
}

void CallerMainWindow::clearNeutrons() {
    textBrowser_2->clear();
}

void CallerMainWindow::clearCounts() {
    textBrowser_3->clear();
    lineEdit_6->setText("0");
    lineEdit_10->setText(QString::number(integrationTime));
}

void CallerMainWindow::clearMessage() {
    textBrowser->clear();
}
