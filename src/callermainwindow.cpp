#include <qtestsupport_core.h>
#include "callermainwindow.h"


CallerMainWindow::CallerMainWindow(QWidget *parent) : QMainWindow(parent) {

    parent = nullptr;

    cntSettings = new Counters();
    counters.setupUi(cntSettings);

    coefSettings = new Coefficients();
    coefficients.setupUi(coefSettings);

    procSetting = new Processing();
    processing.setupUi(procSetting);

    readFileSettings = new ReadFromFile();
    readfile.setupUi(readFileSettings);

    plotOptionsSetting = new PlotterOptions();
    plotter.setupUi(plotOptionsSetting);

    triggerModeSettings = new TriggerMode();
    trigger.setupUi(triggerModeSettings);

    constFluxSettings = new ConstFlux();
    constFlux.setupUi(constFluxSettings);

    settingsSaver = new SettingsSL();

    startUpFunc();

    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(fluxCalc, &FluxCalc::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecData, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecDataFile, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecDataRough, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(vecDataFileRough, &vecFill::sentMessage, this, &CallerMainWindow::printMsg);
    QObject::connect(readFileSettings, &ReadFromFile::sentReadAction, this, &CallerMainWindow::addStartFile);
    QObject::connect(readFileSettings, &ReadFromFile::sentLoadAction, this, &CallerMainWindow::addLoadFile);
    QObject::connect(readFileSettings, &ReadFromFile::sentDelayValue, this, &CallerMainWindow::setReadDelay);
    QObject::connect(plotOptionsSetting, &PlotterOptions::sentPlotTrig, this, &CallerMainWindow::plotTrigger);
    QObject::connect(plotOptionsSetting, &PlotterOptions::sentPlotTotTrig, this, &CallerMainWindow::plotTriggerTotal);
    QObject::connect(plotOptionsSetting, &PlotterOptions::sentPlotTrigR, this, &CallerMainWindow::plotTriggerRough);
    QObject::connect(plotOptionsSetting, &PlotterOptions::sentPlotTotTrigR, this, &CallerMainWindow::plotTriggerTotalRough);
    QObject::connect(plotOptionsSetting, &PlotterOptions::sentRescaleTrig, this, &CallerMainWindow::rescalePlotTrigger);
    QObject::connect(triggerModeSettings, &TriggerMode::sentGoTrigger, this, &CallerMainWindow::addGoTrigger);
    QObject::connect(triggerModeSettings, &TriggerMode::sentManualTrigger, this, &CallerMainWindow::manualTrigger);
    QObject::connect(triggerModeSettings, &TriggerMode::sentAutoTrigger, this, &CallerMainWindow::autoTrigger);
    QObject::connect(constFluxSettings, &ConstFlux::sentConstFluxGo, this, &CallerMainWindow::addConstFluxGo);
    QObject::connect(constFluxSettings, &ConstFlux::sentConstFluxTrig, this, &CallerMainWindow::addConstFluxTrig);
    QObject::connect(constFluxSettings, &ConstFlux::sentEdgePointPlus, this, &CallerMainWindow::edgePointPlus);
    QObject::connect(constFluxSettings, &ConstFlux::sentEdgePointMinus, this, &CallerMainWindow::edgePointMinus);
    QObject::connect(constFluxSettings, &ConstFlux::sentEdgePointPlusR, this, &CallerMainWindow::edgePointPlusR);
    QObject::connect(constFluxSettings, &ConstFlux::sentEdgePointMinusR, this, &CallerMainWindow::edgePointMinusR);

    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &CallerMainWindow::startByTimer);
    m_timer->setInterval(1000);
}

CallerMainWindow::~CallerMainWindow() {
    for (int i=0; i<plotObjVec.size(); i++)
        delete plotObjVec.at(i);
    cntSettings->close();
    coefSettings->close();
    procSetting->close();
    readFileSettings->close();
    plotOptionsSetting->close();
    triggerModeSettings->close();
    constFluxSettings->close();
    delete writerLog;
    on_actionSave_settings_triggered();
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

    if (cntSettings->countersNumTrig==0)
    {
        resTime.clear();
        cntTrigValues.clear();
        for (int i=0; i<4; i++) {
            resTime.push_back(std::stod(countersData.at(i)->text().toStdString())*1e-6);
            cntTrigValues.push_back(cntTriggers.at(i)->isChecked());
        }

        count = resTime.size();
    }
    else
    {
        resTime.clear();
        cntTrigValues.clear();
        for (int i=0; i<6; i++) {
            resTime.push_back(std::stod(countersData.at(i)->text().toStdString())*1e-6);
            cntTrigValues.push_back(cntTriggers.at(i)->isChecked());
        }

        count = resTime.size();
    }

    resultsNew.clear();
    while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
        tempVar1 = resultsNew.size();
        token = inputValStr.substr(0, pos);
        if (token.length() > 0) {
            token.pop_back();
            if (dotsFind(token,":").second == count+1)
            {
                resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(token,":").first), token));
                tempVar2 = resultsNew.size();
                if (tempVar2>tempVar1)
                {
                    vecData->getData(resultsNew.rbegin()->second,counter, cntTrigValues, resTime, count);
                    vecDataRough->getData(resultsNew.rbegin()->second,counter, cntTrigValues, resTime, count);
                    counter+=1;
                }
            }
        }
        inputValStr.erase(0, pos + delimiter.length());
    }

    if (inputValStr!="")
    {
        tempVar1 = resultsNew.size();
        if (dotsFind(inputValStr,":").second == count+1)
        {
            resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(inputValStr,":").first),inputValStr));
            tempVar2 = resultsNew.size();
            if (tempVar2>tempVar1)
            {
                vecData->getData(resultsNew.rbegin()->second,counter, cntTrigValues, resTime, count);
                vecDataRough->getData(resultsNew.rbegin()->second,counter, cntTrigValues, resTime, count);
                counter+=1;
            }
        }
    }

    if (vecData->resultsDb.size()>0)
    {
        if (!portIsMissing)
            writerLog->fileWriteVec(vecData->resultsDb, "log");
        else
            writerLog->fileWriteVec(vecData->resultsDb, "log portIsMissing");

        std::stringstream res_out;
        for (int i = 0; i < vecData->resultsDb.size(); i++) {
            res_out << vecData->resultsDb.at(i).back() << " ";
        }

        double var1 = trigVal;
        bool var2 = portHasBeenCrashed;

        coeff_a = coefSettings->coeff_a;
        coeff_b = coefSettings->coeff_b;
        vecData->getDataTotal(vecData->resultsDb, integrationTime, nFlux, coeff_a, coeff_b, portHasBeenCrashed,
                              trigMode, trigVal, edgePoint, constFluxTrig, tempTime, tempTimeShift, edgePointTrig,
                              procSetting->backDelay, avWindow, leftTime, procSetting->multiPulsesTrig, procSetting->clearBackVecTrig,
                              procSetting->critVal, procSetting->intTime, procSetting->nucleusTrig, 0, skipTrigger);
        if (count==6) {
            coeff_a = coefSettings->coeff_a_rough;
            coeff_b = coefSettings->coeff_b_rough;
            trigVal = var1;
            portHasBeenCrashed = var2;
            vecDataRough->getDataTotal(vecDataRough->resultsDb, integrationTime, nFlux, coeff_a, coeff_b, portHasBeenCrashed,
                                       trigMode, trigVal, edgePointR, constFluxTrig, tempTimeR, tempTimeShiftR,
                                       edgePointTrigR,
                                       procSetting->backDelay, avWindow, leftTimeR, procSetting->multiPulsesTrig,
                                       procSetting->clearBackVecTrig,
                                       procSetting->critVal, procSetting->intTime, procSetting->nucleusTrig, 1, skipTriggerR);
        }

        lineEdit_2->setText(QString::number(vecData->resultsDbTotalP.at(1).back(),'f',2));
        lineEdit_3->setText(QString::number(vecData->resultsDbTotalP.at(2).back(),'f',2));
        lineEdit_4->setText(QString::number(vecData->resultsDbTotalP.at(3).back(),'f',2));

        if (count==6) {
            lineEdit_13->setText(QString::number(vecDataRough->resultsDbTotalRoughP.at(1).back(), 'f', 2));
            lineEdit_11->setText(QString::number(vecDataRough->resultsDbTotalRoughP.at(2).back(), 'f', 2));
            lineEdit_12->setText(QString::number(vecDataRough->resultsDbTotalRoughP.at(3).back(), 'f', 2));
        }

        lineEdit_10->setText(QString::number(leftTime));
        lineEdit_7->setText(QString::number(leftTimeR));
        constFluxSettings->lineEdit->setText(QString::number(tempTime));
        constFluxSettings->lineEdit_2->setText(QString::number(tempTimeR));

        if (plotState>0 && isActive)
            makePlot->PlotGraph(rescaleTrigger, xp1dif, xp2dif, yp1dif, yp2dif, isActive);
        if (!isActive) {
            plotState=0;
            plotOptionsSetting->checkBox->setChecked(0);
        }
        if (plotTotalState>0 && isActiveTotal)
            makePlot->PlotGraphTotal(rescaleTrigger,tempTime, xp1tot, xp2tot, yp1tot, yp2tot, isActiveTotal);
        if (!isActiveTotal) {
            plotTotalState=0;
            plotOptionsSetting->checkBox_2->setChecked(0);
        }

        if (plotStateRough > 0 && isActiveRough)
            makePlotRough->PlotGraph(rescaleTrigger, xp1difR, xp2difR, yp1difR, yp2difR, isActiveRough);
        if (!isActiveRough) {
            plotStateRough=0;
            plotOptionsSetting->checkBox_3->setChecked(0);
        }
        if (plotTotalStateRough > 0 && isActiveTotalRough)
            makePlotRough->PlotGraphTotal(rescaleTrigger, tempTimeR, xp1totR, xp2totR, yp1totR, yp2totR, isActiveTotalRough);
        if (!isActiveTotalRough) {
            plotTotalStateRough=0;
            plotOptionsSetting->checkBox_4->setChecked(0);
        }

        QString showLine = QString::fromStdString(res_out.str());
        lineEdit_6->setText(QString::number(vecData->Flux,'g',3));
        if (cntSettings->countersNumTrig==1)
            lineEdit_14->setText(QString::number(vecDataRough->Flux,'g',3));
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

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        std::cout << serialPortInfo.portName().toStdString() << std::endl;
    }

    portNames.clear();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
                portNames.push_back(serialPortInfo.portName().toStdString());}

    auto it = std::find(portNames.begin(), portNames.end(), portName);
    if (it!=portNames.end())
    {
        std::cout << "port is on" << std::endl;
        if (portIsMissing)
        {
            delete esp32;
            esp32 = new SerialPort(pName.toStdString());
            portIsMissing = false;
            portHasBeenCrashed = true;
            vecData->fluxTrig=0;
            vecDataRough->fluxTrig=0;
        }
    }
    else
    {
        std::cout << "port is missing" << std::endl;
        portIsMissing = true;
    }


    if (counter>=measTime || !onFlag)
    {
        if (m_timer->isActive())
            m_timer->stop();

        FileWriter writer;
        writer.fileWriteVec(vecData->resultsDb, "full");

        textBrowser->setText(textBrowser->toPlainText()
                             + "results stored in the file" + '\n');

        if (vecData->Flux>0 || vecDataRough->Flux>0) {
            QString s(0x00B1);
            double error = 0;

            error = vecData->statErr * vecData->Flux;
            printMsg(QString::number(vecData->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 1);

            if (cntSettings->countersNumTrig==1) {
                error = vecDataRough->statErr * vecDataRough->Flux;
                printMsg(QString::number(vecDataRough->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 11);
            }

            vecData->msgFillUp();
            if (cntSettings->countersNumTrig==1)
                vecDataRough->msgFillUp();

            if (constFluxTrig==0) {
                printMsg(vecData->pulseDataMsg, 2);
                if (cntSettings->countersNumTrig==1)
                    printMsg(vecDataRough->pulseDataMsg, 22);
            }
            if (constFluxTrig>0) {
                printMsg(vecData->constDataMsg, 3);
                if (cntSettings->countersNumTrig==1)
                    printMsg(vecDataRough->constDataMsg, 33);
            }
        }

        vecData->cleanUp();
        vecDataRough->cleanUp();
        edgePointTrig = 0;
        tempTime = 0;
        tempTimeShift = 0;
        tempTimeR = 0;
        tempTimeShiftR = 0;
        edgePointTrigR = 0;
        lineEdit_7->setText(QString::number(integrationTime+procSetting->backDelay));
        lineEdit_10->setText(QString::number(integrationTime+procSetting->backDelay));
        leftTime = integrationTime+procSetting->backDelay;
        leftTimeR = integrationTime+procSetting->backDelay;

        delete esp32;

        onFlag = false;
        pushButton->setEnabled(1);
        pushButton_2->setEnabled(1);
        readfile.pushButton_2->setEnabled(1);
        readfile.pushButton->setEnabled(1);
        lineEdit_2->setText(QString::number(0));
        lineEdit_3->setText(QString::number(0));
        lineEdit_4->setText(QString::number(0));
        lineEdit_11->setText(QString::number(0));
        lineEdit_12->setText(QString::number(0));
        lineEdit_13->setText(QString::number(0));
    }
}

void CallerMainWindow::printMsg(QString msg, int num) {

/*    const int tabStop = 10;
    QFont font;
    QFontMetrics metrics(font);
    textBrowser_3->setTabStopWidth(tabStop * metrics.width(' '));*/

    if (num == 1) {
        if (msgVar1>=msgVar11 && msgVar1>=msgVar2 && msgVar1>=msgVar22 && msgVar1>=msgVar3 && msgVar2>=msgVar33)
            shotCounter+=1;
        textBrowser_2->setText(textBrowser_2->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_2->verticalScrollBar();
        sb->setValue(sb->maximum());
        msgVar1++;
    }

    if (num == 11) {
        if (msgVar11>=msgVar1 && msgVar11>=msgVar2 && msgVar11>=msgVar22 && msgVar11>=msgVar3 && msgVar11>=msgVar33)
            shotCounter+=1;
        textBrowser_5->setText(textBrowser_5->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_5->verticalScrollBar();
        sb->setValue(sb->maximum());
        msgVar11++;
    }

    if (num == 2) {
        if (msgVar2>=msgVar1 && msgVar2>=msgVar11 && msgVar2>=msgVar22 && msgVar2>=msgVar3 && msgVar2>=msgVar33)
            shotCounter+=1;
        if (shotCounter==1)
            textBrowser_3->setText(textBrowser_3->toPlainText()
            + "Cnt clean" + '\t' + "Cnt adjusted" + '\t' + "Minus back" + '\t' + "Back" + '\t'
            + "Error" + '\t'+ "Extrapolated" + '\t' + "Pulse time" + '\t' + "Start" + '\n');
        textBrowser_3->setText(textBrowser_3->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_3->verticalScrollBar();
        sb->setValue(sb->maximum());
        msgVar2++;
    }

    if (num == 22) {
        if (msgVar22>=msgVar1 && msgVar22>=msgVar11 && msgVar22>=msgVar2 && msgVar22>=msgVar3 && msgVar22>=msgVar33)
            shotCounter+=1;
        if (shotCounter==1)
            textBrowser_4->setText(textBrowser_4->toPlainText()
               + "Cnt clean" + '\t' + "Cnt adjusted" + '\t' + "Minus back" + '\t' + "Back" + '\t'
               + "Error" + '\t'+ "Extrapolated" + '\t' + "Pulse time" + '\t' + "Start" + '\n');
        textBrowser_4->setText(textBrowser_4->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_4->verticalScrollBar();
        sb->setValue(sb->maximum());
        msgVar22++;
    }

    if (num == 3) {
        if (msgVar3>=msgVar1 && msgVar3>=msgVar11 && msgVar3>=msgVar2 && msgVar3>=msgVar22 && msgVar3>=msgVar33)
            shotCounter+=1;
        if (shotCounter==1)
            textBrowser_3->setText(textBrowser_3->toPlainText()
            + "Cnt clean" + '\t' + "Cnt adjusted" + '\t' + "Minus back" + '\t' + "Back" + '\t'
            + "Error" + '\t'+ "Extrapolated" + '\t' + "irStart" + '\t' + "irEnd" + '\t' + "Start" + '\n');
        textBrowser_3->setText(textBrowser_3->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_3->verticalScrollBar();
        sb->setValue(sb->maximum());
        msgVar3++;
    }

    if (num == 33) {
        if (msgVar33>=msgVar1 && msgVar33>=msgVar11 && msgVar33>=msgVar2 && msgVar33>=msgVar22 && msgVar33>=msgVar3)
            shotCounter+=1;
        if (shotCounter==1)
            textBrowser_4->setText(textBrowser_4->toPlainText()
                                   + "Cnt clean" + '\t' + "Cnt adjusted" + '\t' + "Minus back" + '\t' + "Back" + '\t'
                                   + "Error" + '\t'+ "Extrapolated" + '\t' + "irStart" + '\t' + "irEnd" + '\t' + "Start" + '\n');
        textBrowser_4->setText(textBrowser_4->toPlainText() + QString::number(shotCounter) + ": " + msg + '\n');
        QScrollBar*sb = textBrowser_4->verticalScrollBar();
        sb->setValue(sb->maximum());
        msgVar33++;
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
    std::stringstream portNameString;
    portNameString << "COM" << portNum;
    portName = portNameString.str();

    pName = QString::fromStdString(portString.str());
}

void CallerMainWindow::connectCOM() {
    SerialPort esp32(pName.toStdString());

    if (esp32.isConnected())
    {
        textBrowser->setText(textBrowser->toPlainText()+"connected to "+pName+'\n');
        trigCOM = 1;
        pushButton->setEnabled(1);
        pushButton_2->setEnabled(0);
    }
    else
    {
        trigCOM = 0;
        return;
    }
}

void CallerMainWindow::addStop() {
    onFlag = false;
    if (trigCOM==1) {
        pushButton->setEnabled(1);
        pushButton_2->setEnabled(1);
    }
    readfile.pushButton_2->setEnabled(1);
    readfile.pushButton->setEnabled(1);
}

void CallerMainWindow::addStart() {
    pushButton->setEnabled(0);
    readfile.pushButton_2->setEnabled(0);
    readfile.pushButton->setEnabled(0);
    makePlot = new Plotter(vecData, plotOptionsSetting, xp1tot, xp2tot, yp1tot, yp2tot, xp1dif, xp2dif, yp1dif, yp2dif, 0);
    plotObjVec.push_back(makePlot);
    makePlotRough = new Plotter(vecDataRough, plotOptionsSetting, xp1totR, xp2totR, yp1totR, yp2totR, xp1difR, xp2difR, yp1difR, yp2difR, 1);
    plotObjVec.push_back(makePlotRough);
    writerLog = new FileWriter();
    onFlag = true;
    counter = 0;
    flushCounter = 0;
    shotCounter = 0;
    msgVar1 = msgVar11 = msgVar2 = msgVar22 = msgVar3 = msgVar33 = 0;

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
    pushButton->setEnabled(0);
    pushButton_2->setEnabled(0);
    readfile.pushButton_2->setEnabled(0);
    readfile.pushButton->setEnabled(0);


    if (fileName!= nullptr)
    {
        shotCounter = 0;
        msgVar1 = msgVar11 = msgVar2 = msgVar22 = msgVar3 = msgVar33 = 0;
        makePlot = new Plotter(vecDataFile, plotOptionsSetting, xp1tot, xp2tot, yp1tot, yp2tot, xp1dif, xp2dif, yp1dif, yp2dif, 0);
        plotObjVec.push_back(makePlot);
        makePlotRough = new Plotter(vecDataFileRough, plotOptionsSetting, xp1totR, xp2totR, yp1totR, yp2totR, xp1difR, xp2difR, yp1difR, yp2difR, 1);
        plotObjVec.push_back(makePlotRough);
        textBrowser->setText("\n");

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

                if (cntSettings->countersNumTrig==0)
                {
                    resTime.clear();
                    cntTrigValues.clear();
                    for (int j=0; j<4; j++) {
                        resTime.push_back(std::stod(countersData.at(j)->text().toStdString())*1e-6);
                        cntTrigValues.push_back(cntTriggers.at(j)->isChecked());
                    }

                    count = resTime.size();
                }
                else
                {
                    resTime.clear();
                    cntTrigValues.clear();
                    for (int j=0; j<6; j++) {
                        resTime.push_back(std::stod(countersData.at(j)->text().toStdString())*1e-6);
                        cntTrigValues.push_back(cntTriggers.at(j)->isChecked());
                    }

                    count = resTime.size();
                }

                tempVar1 = resultsNew.size();
                std::string inputValStr = fileData.at(i);
                std::string delimiter = "\n";
                size_t pos = 0;
                std::string token;
                std::string timeStr;
                while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
                    token = inputValStr.substr(0, pos);
                    if (token.length() > 0) {
                        if (dotsFind(token, ":").second == count+1) {
                            resultsNew.insert(
                                    std::pair<double, std::string>(std::stod(dotsFind(token, ":").first), token));
                            tempVar2 = resultsNew.size();
                            if (tempVar2 > tempVar1) {
                                vecDataFile->getData(resultsNew.rbegin()->second, counter, cntTrigValues, resTime, count);
                                vecDataFileRough->getData(resultsNew.rbegin()->second, counter, cntTrigValues, resTime, count);
                                counter += 1;
                            }
                        }
                    }
                    inputValStr.erase(0, pos + delimiter.length());
                }

                if (inputValStr != "") {
                    tempVar1 = resultsNew.size();
                    if (dotsFind(inputValStr, ":").second == count+1) {
                        resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(inputValStr, ":").first),
                                                                         inputValStr));
                        tempVar2 = resultsNew.size();
                        if (tempVar2 > tempVar1) {
                            vecDataFile->getData(resultsNew.rbegin()->second, counter, cntTrigValues, resTime, count);
                            vecDataFileRough->getData(resultsNew.rbegin()->second, counter, cntTrigValues, resTime, count);
                            counter += 1;
                        }
                    };
                }
                if (vecDataFile->resultsDb.size() > 0) {
                    coeff_a = coefSettings->coeff_a;
                    coeff_b = coefSettings->coeff_b;
                    vecDataFile->getDataTotal(vecDataFile->resultsDb, integrationTime, nFlux, coeff_a, coeff_b, portHasBeenCrashed,
                                              trigMode, trigVal, edgePoint, constFluxTrig, tempTime, tempTimeShift, edgePointTrig,
                                              procSetting->backDelay, avWindow, leftTime, procSetting->multiPulsesTrig, procSetting->clearBackVecTrig,
                                              procSetting->critVal, procSetting->intTime, procSetting->nucleusTrig, 0, skipTrigger);

                    if (count==6) {
                        coeff_a = coefSettings->coeff_a_rough;
                        coeff_b = coefSettings->coeff_b_rough;
                        vecDataFileRough->getDataTotal(vecDataFileRough->resultsDb, integrationTime, nFlux, coeff_a,
                                                       coeff_b, portHasBeenCrashed,
                                                       trigMode, trigVal, edgePointR, constFluxTrig, tempTimeR,
                                                       tempTimeShiftR, edgePointTrigR,
                                                       procSetting->backDelay, avWindow, leftTimeR,
                                                       procSetting->multiPulsesTrig, procSetting->clearBackVecTrig,
                                                       procSetting->critVal, procSetting->intTime,
                                                       procSetting->nucleusTrig, 1, skipTriggerR);
                    }

                    lineEdit_2->setText(QString::number(vecDataFile->resultsDbTotalP.at(1).back(),'f',2));
                    lineEdit_3->setText(QString::number(vecDataFile->resultsDbTotalP.at(2).back(),'f',2));
                    lineEdit_4->setText(QString::number(vecDataFile->resultsDbTotalP.at(3).back(),'f',2));

                    if (count==6) {
                        lineEdit_13->setText(QString::number(vecDataFileRough->resultsDbTotalRoughP.at(1).back(), 'f', 2));
                        lineEdit_11->setText(QString::number(vecDataFileRough->resultsDbTotalRoughP.at(2).back(), 'f', 2));
                        lineEdit_12->setText(QString::number(vecDataFileRough->resultsDbTotalRoughP.at(3).back(), 'f', 2));
                    }

                    lineEdit_10->setText(QString::number(leftTime));
                    lineEdit_7->setText(QString::number(leftTimeR));
                    constFluxSettings->lineEdit->setText(QString::number(tempTime));
                    constFluxSettings->lineEdit_2->setText(QString::number(tempTimeR));

                    if (plotState > 0 && isActive)
                        makePlot->PlotGraph(rescaleTrigger, xp1dif, xp2dif, yp1dif, yp2dif, isActive);
                    if (!isActive) {
                        plotState=0;
                        plotOptionsSetting->checkBox->setChecked(0);
                    }
                    if (plotTotalState > 0 && isActiveTotal)
                        makePlot->PlotGraphTotal(rescaleTrigger, tempTime, xp1tot, xp2tot, yp1tot, yp2tot, isActiveTotal);
                    if (!isActiveTotal) {
                        plotTotalState=0;
                        plotOptionsSetting->checkBox_2->setChecked(0);
                    }

                    if (plotStateRough > 0 && isActiveRough)
                        makePlotRough->PlotGraph(rescaleTrigger, xp1difR, xp2difR, yp1difR, yp2difR, isActiveRough);
                    if (!isActiveRough) {
                        plotStateRough=0;
                        plotOptionsSetting->checkBox_3->setChecked(0);
                    }
                    if (plotTotalStateRough > 0 && isActiveTotalRough)
                        makePlotRough->PlotGraphTotal(rescaleTrigger, tempTimeR, xp1totR, xp2totR, yp1totR, yp2totR, isActiveTotalRough);
                    if (!isActiveTotalRough) {
                        plotTotalStateRough=0;
                        plotOptionsSetting->checkBox_4->setChecked(0);
                    }

                    std::stringstream res_out;
                    for (int k = 0; k < vecDataFile->resultsDb.size(); k++) {
                        res_out << vecDataFile->resultsDb.at(k).back() << " ";}
                        QString showLine = QString::fromStdString(res_out.str());
                        lineEdit_6->setText(QString::number(vecDataFile->Flux,'g',3));
                        if (cntSettings->countersNumTrig==1)
                            lineEdit_14->setText(QString::number(vecDataFileRough->Flux,'g',3));
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
            if (vecDataFile->Flux>0 || vecDataFileRough->Flux>0) {
                QString s(0x00B1);
                double error = 0;
                error = vecDataFile->statErr * vecDataFile->Flux;
                printMsg(QString::number(vecDataFile->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 1);

                if (cntSettings->countersNumTrig==1) {
                    error = vecDataFileRough->statErr * vecDataFileRough->Flux;
                    printMsg(QString::number(vecDataFileRough->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 11);
                }

                vecDataFile->msgFillUp();

                if (cntSettings->countersNumTrig==1)
                    vecDataFileRough->msgFillUp();

                if (constFluxTrig==0) {
                    printMsg(vecDataFile->pulseDataMsg, 2);
                    if (cntSettings->countersNumTrig==1)
                        printMsg(vecDataFileRough->pulseDataMsg, 22);
                }
                if (constFluxTrig>0) {
                    printMsg(vecDataFile->constDataMsg, 3);
                    if (cntSettings->countersNumTrig==1)
                        printMsg(vecDataFileRough->constDataMsg, 33);
                }
            }

            textBrowser->setText(textBrowser->toPlainText()+"Finished"+'\n');
            vecDataFile->cleanUp();
            vecDataFileRough->cleanUp();
            edgePointTrig = 0;
            tempTime = 0;
            tempTimeShift = 0;
            tempTimeR = 0;
            tempTimeShiftR = 0;
            edgePointTrigR = 0;
            lineEdit_7->setText(QString::number(integrationTime+procSetting->backDelay));
            lineEdit_10->setText(QString::number(integrationTime+procSetting->backDelay));
            leftTime = integrationTime+procSetting->backDelay;
            leftTimeR = integrationTime+procSetting->backDelay;
            pushButton->setEnabled(1);
            if (trigCOM==0)
                pushButton_2->setEnabled(1);
            readfile.pushButton_2->setEnabled(1);
            readfile.pushButton->setEnabled(1);
            lineEdit_2->setText(QString::number(0));
            lineEdit_3->setText(QString::number(0));
            lineEdit_4->setText(QString::number(0));
            lineEdit_11->setText(QString::number(0));
            lineEdit_12->setText(QString::number(0));
            lineEdit_13->setText(QString::number(0));
        }

        if (onFlag) {

            if (vecDataFile->Flux>0 || vecDataFileRough->Flux>0) {
                QString s(0x00B1);
                double error = 0;
                error = vecDataFile->statErr * vecDataFile->Flux;
                printMsg(QString::number(vecDataFile->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 1);

                if (cntSettings->countersNumTrig==1) {
                    error = vecDataFileRough->statErr * vecDataFileRough->Flux;
                    printMsg(QString::number(vecDataFileRough->Flux, 'g', 3) + s + QString::number(error, 'g', 3), 11);
                }

                vecDataFile->msgFillUp();

                if (cntSettings->countersNumTrig==1)
                    vecDataFileRough->msgFillUp();

                if (constFluxTrig==0) {
                    printMsg(vecDataFile->pulseDataMsg, 2);
                    if (cntSettings->countersNumTrig==1)
                        printMsg(vecDataFileRough->pulseDataMsg, 22);
                }
                if (constFluxTrig>0) {
                    printMsg(vecDataFile->constDataMsg, 3);
                    if (cntSettings->countersNumTrig==1)
                        printMsg(vecDataFileRough->constDataMsg, 33);
                }
            }

            onFlag = false;
            textBrowser->setText(textBrowser->toPlainText() + "Finished" + '\n');
            vecDataFile->cleanUp();
            vecDataFileRough->cleanUp();
            edgePointTrig = 0;
            edgePointTrigR = 0;
            tempTime = 0;
            tempTimeShift = 0;
            tempTimeR = 0;
            tempTimeShiftR = 0;
            pushButton->setEnabled(1);
            if (trigCOM==0)
                pushButton_2->setEnabled(1);
            readfile.pushButton_2->setEnabled(1);
            readfile.pushButton->setEnabled(1);
            lineEdit_2->setText(QString::number(0));
            lineEdit_3->setText(QString::number(0));
            lineEdit_4->setText(QString::number(0));
            lineEdit_11->setText(QString::number(0));
            lineEdit_12->setText(QString::number(0));
            lineEdit_13->setText(QString::number(0));
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
    int sz = plotObjVec.size();
    if (!plotObjVec.empty())
        plotObjVec.at(sz-2)->chartView->isOn=true;
    if (!isActive && st >0)
        isActive = true;
    if (st == 0) {
        isActive = false;
        if (!plotObjVec.empty()) {
            plotObjVec.at(sz-2)->chartView->close();
            plotObjVec.at(sz-2)->chartView->isOn = true;
        }
    }
}

void CallerMainWindow::plotTriggerTotal(int stTot) {
    plotTotalState = stTot;
    int sz = plotObjVec.size();
    if (!plotObjVec.empty())
        plotObjVec.at(sz-2)->chartViewTot->isOn=true;
    if (!isActiveTotal && stTot>0)
        isActiveTotal = true;
    if (stTot == 0) {
        isActiveTotal = false;
        if (!plotObjVec.empty()) {
            plotObjVec.at(sz-2)->chartViewTot->close();
            plotObjVec.at(sz-2)->chartViewTot->isOn = true;
        }
    }
}

void CallerMainWindow::plotTriggerRough(int st) {
    plotStateRough = st;
    if (!plotObjVec.empty())
        plotObjVec.back()->chartView->isOn=true;
    if (!isActiveRough && st >0)
        isActiveRough = true;
    if (st == 0) {
        isActiveRough = false;
        if (!plotObjVec.empty()) {
            plotObjVec.back()->chartView->close();
            plotObjVec.back()->chartView->isOn = true;
        }
    }
}

void CallerMainWindow::plotTriggerTotalRough(int stTot) {
    plotTotalStateRough = stTot;
    if (!plotObjVec.empty())
        plotObjVec.back()->chartViewTot->isOn=true;
    if (!isActiveTotalRough && stTot>0)
        isActiveTotalRough = true;
    if (stTot == 0) {
        isActiveTotalRough = false;
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
        lineEdit_7->setText(QString::number(integrationTime+procSetting->backDelay));
        lineEdit_10->setText(QString::number(integrationTime+procSetting->backDelay));
        leftTime = integrationTime+procSetting->backDelay;
        leftTimeR = integrationTime+procSetting->backDelay;
    }
}

void CallerMainWindow::addGoTrigger() {
    trigVal++;
}

void CallerMainWindow::autoTrigger(int st) {
    if (st>0)
    {
        trigMode = 0;
        triggerModeSettings->checkBox->setChecked(false);
        triggerModeSettings->pushButton->setEnabled(0);
    }
    else
    {
        trigMode = 1;
        triggerModeSettings->checkBox->setChecked(true);
        triggerModeSettings->pushButton->setEnabled(1);
    }
}

void CallerMainWindow::manualTrigger(int st) {
    if (st>0)
    {
        trigMode = 1;
        triggerModeSettings->checkBox_2->setChecked(false);
        triggerModeSettings->pushButton->setEnabled(1);
    }
    else
    {
        trigMode = 0;
        triggerModeSettings->checkBox_2->setChecked(true);
        triggerModeSettings->pushButton->setEnabled(0);
    }
}

void CallerMainWindow::addConstFluxGo() {
        edgePointTrig++;
        edgePointTrigR++;
}

void CallerMainWindow::addConstFluxTrig(int st) {
    constFluxTrig = st;
    if (st>0) {
        constFlux.pushButton->setEnabled(1);
        constFlux.pushButton_2->setEnabled(1);
        constFlux.pushButton_3->setEnabled(1);
        constFlux.pushButton_4->setEnabled(1);
        constFlux.pushButton_5->setEnabled(1);
    }
    else {
        constFlux.pushButton->setEnabled(0);
        constFlux.pushButton_2->setEnabled(0);
        constFlux.pushButton_3->setEnabled(0);
        constFlux.pushButton_4->setEnabled(0);
        constFlux.pushButton_5->setEnabled(0);
    }
}

void CallerMainWindow::edgePointPlus() {
    tempTimeShift++;
    constFluxSettings->lineEdit->setText(QString::number(tempTime));
}

void CallerMainWindow::edgePointMinus() {
    tempTimeShift--;
    constFluxSettings->lineEdit->setText(QString::number(tempTime));
}

void CallerMainWindow::edgePointPlusR() {
    tempTimeShiftR++;
    constFluxSettings->lineEdit_2->setText(QString::number(tempTimeR));
}

void CallerMainWindow::edgePointMinusR() {
    tempTimeShiftR--;
    constFluxSettings->lineEdit_2->setText(QString::number(tempTimeR));
}

void CallerMainWindow::setReadDelay(QString delay) {
    inputProcessing(readDelay, delay.toStdString());
}

void CallerMainWindow::setAverageWindow(QString window) {
    inputProcessing(avWindow, window.toStdString());
}

void CallerMainWindow::on_actioncounters_triggered()
{
    mWinX = mWin->geometry().x();
    mWinY = mWin->geometry().y();
    cntSettings->window()->setGeometry(mWinX+mWin->width(),mWinY,256,242);
    cntSettings->show();
}

void CallerMainWindow::on_actioncoefficients_triggered()
{
    mWinX = mWin->geometry().x();
    mWinY = mWin->geometry().y();
    coefSettings->window()->setGeometry(mWinX+mWin->width(),mWinY+242+25,256,181);
    coefSettings->show();
}

void CallerMainWindow::on_actionProcessing_triggered() {
    mWinX = mWin->geometry().x();
    mWinY = mWin->geometry().y();
    procSetting->window()->setGeometry(mWinX+mWin->width(),mWinY+242+25+181+25,256,179);
    procSetting->show();
}

void CallerMainWindow::on_actionReadFile_triggered() {
    mWinX = mWin->geometry().x();
    mWinY = mWin->geometry().y();
    readFileSettings->window()->setGeometry(mWinX-256,mWinY,256,118);
    readFileSettings->show();
}

void CallerMainWindow::on_actionPlotOption_triggered() {
    mWinX = mWin->geometry().x();
    mWinY = mWin->geometry().y();
    plotOptionsSetting->window()->setGeometry(mWinX-256,mWinY+118+25,256,110);
    plotOptionsSetting->show();
}

void CallerMainWindow::on_actionTriggerMode_triggered() {
    mWinX = mWin->geometry().x();
    mWinY = mWin->geometry().y();
    triggerModeSettings->window()->setGeometry(mWinX-256,mWinY+118+25+110+25,256,83);
    triggerModeSettings->show();
}

void CallerMainWindow::on_actionConstFlux_triggered() {
    mWinX = mWin->geometry().x();
    mWinY = mWin->geometry().y();
    constFluxSettings->window()->setGeometry(mWinX-256,mWinY+118+25+110+25+83+25,256,93);
    constFluxSettings->show();
}

void CallerMainWindow::startUpFunc() {

    resTime.push_back(200);
    resTime.push_back(300);
    resTime.push_back(400);
    resTime.push_back(350);
    resTime.push_back(250);
    resTime.push_back(250);

    count = resTime.size();

    cntSettings->lineEdit = counters.lineEdit;
    cntSettings->lineEdit_2 = counters.lineEdit_2;
    cntSettings->lineEdit_3 = counters.lineEdit_3;
    cntSettings->lineEdit_4 = counters.lineEdit_4;
    cntSettings->lineEdit_5 = counters.lineEdit_5;
    cntSettings->lineEdit_6 = counters.lineEdit_6;

    cntSettings->checkBox = counters.checkBox;
    cntSettings->checkBox_2 = counters.checkBox_2;
    cntSettings->checkBox_3 = counters.checkBox_3;
    cntSettings->checkBox_4 = counters.checkBox_4;
    cntSettings->checkBox_5 = counters.checkBox_5;
    cntSettings->checkBox_6 = counters.checkBox_6;
    cntSettings->checkBox_7 = counters.checkBox_7;
    cntSettings->checkBox_8 = counters.checkBox_8;

    countersData.push_back(counters.lineEdit);
    countersData.push_back(counters.lineEdit_2);
    countersData.push_back(counters.lineEdit_3);
    countersData.push_back(counters.lineEdit_4);
    countersData.push_back(counters.lineEdit_5);
    countersData.push_back(counters.lineEdit_6);

    cntTriggers.push_back(counters.checkBox);
    cntTriggers.push_back(counters.checkBox_2);
    cntTriggers.push_back(counters.checkBox_3);
    cntTriggers.push_back(counters.checkBox_4);
    cntTriggers.push_back(counters.checkBox_5);
    cntTriggers.push_back(counters.checkBox_6);

    counters.checkBox->setChecked(true);
    counters.checkBox_2->setChecked(true);
    counters.checkBox_3->setChecked(true);
    counters.checkBox_4->setChecked(true);
    counters.checkBox_5->setChecked(true);
    counters.checkBox_6->setChecked(true);
    counters.checkBox_7->setChecked(true);
    counters.lineEdit->setText(QString::number(resTime[0]));
    counters.lineEdit_2->setText(QString::number(resTime[1]));
    counters.lineEdit_3->setText(QString::number(resTime[2]));
    counters.lineEdit_4->setText(QString::number(resTime[3]));
    counters.lineEdit_5->setText(QString::number(resTime[4]));
    counters.lineEdit_6->setText(QString::number(resTime[5]));

    coefSettings->lineEdit = coefficients.lineEdit;
    coefSettings->lineEdit_2 = coefficients.lineEdit_2;
    coefSettings->lineEdit_3 = coefficients.lineEdit_3;
    coefSettings->lineEdit_4 = coefficients.lineEdit_4;
    coefSettings->lineEdit_5 = coefficients.lineEdit_5;
    coefSettings->checkBox = coefficients.checkBox;
    coefSettings->checkBox_2 = coefficients.checkBox_2;
    coefSettings->checkBox_3 = coefficients.checkBox_3;
    coefSettings->tabWidget = coefficients.tabWidget;

    coefficients.checkBox->setChecked(true);
    coefficients.checkBox_3->setChecked(true);
    coefficients.lineEdit->setText(QString::number(coefSettings->coeff_a));
    coefficients.lineEdit_2->setText(QString::number(coefSettings->coeff_b));
    coefficients.lineEdit_4->setText(QString::number(coefSettings->coeff_a_rough));
    coefficients.lineEdit_5->setText(QString::number(coefSettings->coeff_b_rough));
    coefficients.lineEdit_3->setText(QString::number(coefSettings->distance));

    procSetting->checkBox = processing.checkBox;
    procSetting->checkBox_2 = processing.checkBox_2;
    procSetting->checkBox_3 = processing.checkBox_3;
    procSetting->checkBox_4 = processing.checkBox_4;
    processing.checkBox_4->setChecked(true);
    procSetting->lineEdit = processing.lineEdit;
    procSetting->lineEdit_2 = processing.lineEdit_2;
    procSetting->lineEdit_3 = processing.lineEdit_3;
    processing.lineEdit->setText(QString::number(procSetting->critVal));
    processing.lineEdit_2->setText(QString::number(procSetting->intTime));
    processing.lineEdit_3->setText(QString::number(procSetting->backDelay));

    readFileSettings->pushButton = readfile.pushButton;
    readFileSettings->pushButton_2 = readfile.pushButton_2;
    readFileSettings->lineEdit = readfile.lineEdit;
    readfile.lineEdit->setText(QString::number(readDelay));

    plotOptionsSetting->checkBox = plotter.checkBox;
    plotOptionsSetting->checkBox_2 = plotter.checkBox_2;
    plotOptionsSetting->checkBox_3 = plotter.checkBox_3;
    plotOptionsSetting->checkBox_4 = plotter.checkBox_4;
    plotOptionsSetting->checkBox_5 = plotter.checkBox_5;
    plotOptionsSetting->checkBox_6 = plotter.checkBox_6;
    plotter.checkBox_6->setChecked(1);

    triggerModeSettings->checkBox = trigger.checkBox;
    triggerModeSettings->checkBox_2 = trigger.checkBox_2;
    triggerModeSettings->pushButton = trigger.pushButton;
    trigger.checkBox_2->setChecked(1);

    constFluxSettings->checkBox = constFlux.checkBox;
    constFluxSettings->pushButton = constFlux.pushButton;
    constFluxSettings->pushButton_2 = constFlux.pushButton_2;
    constFluxSettings->pushButton_3 = constFlux.pushButton_3;
    constFluxSettings->pushButton_4 = constFlux.pushButton_4;
    constFluxSettings->pushButton_5 = constFlux.pushButton_5;
    constFluxSettings->lineEdit = constFlux.lineEdit;
    constFluxSettings->lineEdit_2 = constFlux.lineEdit_2;
    constFlux.lineEdit->setText(QString::number(edgePoint));
    constFlux.lineEdit_2->setText(QString::number(edgePointR));
    constFlux.lineEdit->setReadOnly(1);
    constFlux.pushButton->setEnabled(0);
    constFlux.pushButton_2->setEnabled(0);
    constFlux.pushButton_3->setEnabled(0);
    constFlux.pushButton_4->setEnabled(0);
    constFlux.pushButton_5->setEnabled(0);

    leftTime = leftTime+procSetting->backDelay;
    leftTimeR = leftTimeR+procSetting->backDelay;
}

void CallerMainWindow::clearNeutrons() {
    textBrowser_2->clear();
    textBrowser_5->clear();
}

void CallerMainWindow::clearCounts() {
    textBrowser_3->clear();
    textBrowser_4->clear();
    lineEdit_6->setText("0");
    lineEdit_14->setText("0");
    lineEdit_7->setText(QString::number(integrationTime+procSetting->backDelay));
    lineEdit_10->setText(QString::number(integrationTime+procSetting->backDelay));
}

void CallerMainWindow::clearMessage() {
    textBrowser->clear();
}

void CallerMainWindow::addSkip() {
    skipTrigger = 1;
    skipTriggerR = 1;
}

void CallerMainWindow::on_actionSave_settings_triggered() {


    settingsSaver->measTime = integrationTime;
    settingsSaver->averageTime = avWindow;

    settingsSaver->cnt1_res = counters.lineEdit->text().toDouble();
    settingsSaver->cnt2_res = counters.lineEdit_2->text().toDouble();
    settingsSaver->cnt3_res = counters.lineEdit_3->text().toDouble();
    settingsSaver->cnt4_res = counters.lineEdit_4->text().toDouble();
    settingsSaver->cnt5_res = counters.lineEdit_5->text().toDouble();
    settingsSaver->cnt6_res = counters.lineEdit_6->text().toDouble();

    settingsSaver->cnt1 = counters.checkBox->isChecked();
    settingsSaver->cnt2 = counters.checkBox_2->isChecked();
    settingsSaver->cnt3 = counters.checkBox_3->isChecked();
    settingsSaver->cnt4 = counters.checkBox_4->isChecked();
    settingsSaver->cnt5 = counters.checkBox_5->isChecked();
    settingsSaver->cnt6 = counters.checkBox_6->isChecked();

    settingsSaver->cnt_num = cntSettings->countersNumTrig;


    settingsSaver->coef_a=coefSettings->coeff_a;
    settingsSaver->coef_b=coefSettings->coeff_b;
    settingsSaver->coef_a_r=coefSettings->coeff_a_rough;
    settingsSaver->coef_b_r= coefSettings->coeff_b_rough;
    settingsSaver->MeV_14 = coefSettings->coef14;
    settingsSaver->MeV_2_5 = coefSettings->coef2_5;
    settingsSaver->userCoef = coefSettings->coefState;
    settingsSaver->distance = coefSettings->distance;

    settingsSaver->clearBack = procSetting->clearBackVecTrig;
    settingsSaver->multiPulse = procSetting->multiPulsesTrig;
    settingsSaver->intTime = procSetting->intTime;
    settingsSaver->backDelay = procSetting->backDelay;
    settingsSaver->critVal = procSetting->critVal;
    settingsSaver->nucleusTrig = procSetting->nucleusTrig;

    settingsSaver->saveSettings();
}

void CallerMainWindow::on_actionLoad_settings_triggered() {

    QString nameVar;
    nameVar = QFileDialog::getOpenFileName(this,tr("Open File"), "./settings", tr("txt files (*.txt)"));
    settingsSaver->settingsFileNameLoad=nameVar.toStdString();

    settingsSaver->loadSettings();

    integrationTime = settingsSaver->measTime;
    avWindow = settingsSaver->averageTime;
    lineEdit_5->setText(QString::number(integrationTime));
    lineEdit_7->setText(QString::number(integrationTime+procSetting->backDelay));
    lineEdit_9->setText(QString::number(avWindow));
    lineEdit_10->setText(QString::number(integrationTime+procSetting->backDelay));

    counters.lineEdit->setText(QString::number(settingsSaver->cnt1_res));
    counters.lineEdit_2->setText(QString::number(settingsSaver->cnt2_res));
    counters.lineEdit_3->setText(QString::number(settingsSaver->cnt3_res));
    counters.lineEdit_4->setText(QString::number(settingsSaver->cnt4_res));
    counters.lineEdit_5->setText(QString::number(settingsSaver->cnt5_res));
    counters.lineEdit_6->setText(QString::number(settingsSaver->cnt6_res));

    counters.checkBox->setChecked(settingsSaver->cnt1);
    counters.checkBox_2->setChecked(settingsSaver->cnt2);
    counters.checkBox_3->setChecked(settingsSaver->cnt3);
    counters.checkBox_4->setChecked(settingsSaver->cnt4);
    counters.checkBox_5->setChecked(settingsSaver->cnt5);
    counters.checkBox_6->setChecked(settingsSaver->cnt6);

    /*if (settingsSaver->cnt_num==0)
    {
        resTime.clear();
        cntTrigValues.clear();
        for (int i=0; i<4; i++) {
            resTime.push_back(std::stod(countersData.at(i)->text().toStdString())*1e-6);
            cntTrigValues.push_back(cntTriggers.at(i)->isChecked());
        }

        count = resTime.size();
    }
    else
    {
        resTime.clear();
        cntTrigValues.clear();
        for (int i=0; i<6; i++) {
            resTime.push_back(std::stod(countersData.at(i)->text().toStdString())*1e-6);
            cntTrigValues.push_back(cntTriggers.at(i)->isChecked());
        }

        count = resTime.size();
    }*/

    if (settingsSaver->cnt_num == 0) {
        cntSettings->fourCounters(1);
    }
    if (settingsSaver->cnt_num == 1) {
        cntSettings->sixCounters(1);
    }

    coefSettings->coeff_a=settingsSaver->coef_a;
    coefSettings->lineEdit->setText(QString::number(settingsSaver->coef_a));
    coefSettings->coeff_b=settingsSaver->coef_b;
    coefSettings->lineEdit_2->setText(QString::number(settingsSaver->coef_b));
    coefSettings->coeff_a_rough=settingsSaver->coef_a_r;
    coefSettings->lineEdit_4->setText(QString::number(settingsSaver->coef_a_r));
    coefSettings->coeff_b_rough=settingsSaver->coef_b_r;
    coefSettings->lineEdit_5->setText(QString::number(settingsSaver->coef_b_r));
    coefSettings->trig14(settingsSaver->MeV_14);
    coefSettings->trig2_5(settingsSaver->MeV_2_5);
    coefSettings->coefTrig(settingsSaver->userCoef);
    coefSettings->checkBox->setChecked(settingsSaver->userCoef);
    coefSettings->setDist(QString::number(settingsSaver->distance));
    coefSettings->lineEdit_3->setText(QString::number(settingsSaver->distance));

    procSetting->clearBack(settingsSaver->clearBack);
    procSetting->checkBox->setChecked(settingsSaver->clearBack);
    procSetting->multiPulses(settingsSaver->multiPulse);
    procSetting->checkBox_2->setChecked(settingsSaver->multiPulse);
    procSetting->addIntTime(QString::number(settingsSaver->intTime));
    procSetting->lineEdit_2->setText(QString::number(settingsSaver->intTime));
    procSetting->addBackDelay(QString::number(settingsSaver->backDelay));
    procSetting->lineEdit_3->setText(QString::number(settingsSaver->backDelay));
    procSetting->addCriticalVal(QString::number(settingsSaver->critVal));
    procSetting->lineEdit->setText(QString::number(settingsSaver->critVal));
    procSetting->addInTrig(settingsSaver->nucleusTrig);
}

void CallerMainWindow::loadLastSettings() {

    std::stringstream dirName;
    dirName << "./settings";
    QDir dir(QString::fromStdString(dirName.str()));
    if (dir.exists()) {
        settingsSaver->settingsFileNameLoad = "./settings/settings.txt";

        settingsSaver->loadSettings();

        integrationTime = settingsSaver->measTime;
        avWindow = settingsSaver->averageTime;
        lineEdit_5->setText(QString::number(integrationTime));
        lineEdit_7->setText(QString::number(integrationTime+procSetting->backDelay));
        lineEdit_9->setText(QString::number(avWindow));
        lineEdit_10->setText(QString::number(integrationTime+procSetting->backDelay));

        counters.lineEdit->setText(QString::number(settingsSaver->cnt1_res));
        counters.lineEdit_2->setText(QString::number(settingsSaver->cnt2_res));
        counters.lineEdit_3->setText(QString::number(settingsSaver->cnt3_res));
        counters.lineEdit_4->setText(QString::number(settingsSaver->cnt4_res));
        counters.lineEdit_5->setText(QString::number(settingsSaver->cnt5_res));
        counters.lineEdit_6->setText(QString::number(settingsSaver->cnt6_res));

        counters.checkBox->setChecked(settingsSaver->cnt1);
        counters.checkBox_2->setChecked(settingsSaver->cnt2);
        counters.checkBox_3->setChecked(settingsSaver->cnt3);
        counters.checkBox_4->setChecked(settingsSaver->cnt4);
        counters.checkBox_5->setChecked(settingsSaver->cnt5);
        counters.checkBox_6->setChecked(settingsSaver->cnt6);

        /*if (settingsSaver->cnt_num==0)
        {
            resTime.clear();
            cntTrigValues.clear();
            for (int i=0; i<4; i++) {
                resTime.push_back(std::stod(countersData.at(i)->text().toStdString())*1e-6);
                cntTrigValues.push_back(cntTriggers.at(i)->isChecked());
            }

            count = resTime.size();
        }
        else
        {
            resTime.clear();
            cntTrigValues.clear();
            for (int i=0; i<6; i++) {
                resTime.push_back(std::stod(countersData.at(i)->text().toStdString())*1e-6);
                cntTrigValues.push_back(cntTriggers.at(i)->isChecked());
            }

            count = resTime.size();
        }*/

        if (settingsSaver->cnt_num == 0) {
            cntSettings->fourCounters(1);
        }
        if (settingsSaver->cnt_num == 1) {
            cntSettings->sixCounters(1);
        }

        coefSettings->coeff_a = settingsSaver->coef_a;
        coefSettings->lineEdit->setText(QString::number(settingsSaver->coef_a));
        coefSettings->coeff_b = settingsSaver->coef_b;
        coefSettings->lineEdit_2->setText(QString::number(settingsSaver->coef_b));
        coefSettings->coeff_a_rough = settingsSaver->coef_a_r;
        coefSettings->lineEdit_4->setText(QString::number(settingsSaver->coef_a_r));
        coefSettings->coeff_b_rough = settingsSaver->coef_b_r;
        coefSettings->lineEdit_5->setText(QString::number(settingsSaver->coef_b_r));
        coefSettings->trig14(settingsSaver->MeV_14);
        coefSettings->trig2_5(settingsSaver->MeV_2_5);
        coefSettings->coefTrig(settingsSaver->userCoef);
        coefSettings->checkBox->setChecked(settingsSaver->userCoef);
        coefSettings->setDist(QString::number(settingsSaver->distance));
        coefSettings->lineEdit_3->setText(QString::number(settingsSaver->distance));

        procSetting->clearBack(settingsSaver->clearBack);
        procSetting->checkBox->setChecked(settingsSaver->clearBack);
        procSetting->multiPulses(settingsSaver->multiPulse);
        procSetting->checkBox_2->setChecked(settingsSaver->multiPulse);
        procSetting->addIntTime(QString::number(settingsSaver->intTime));
        procSetting->lineEdit_2->setText(QString::number(settingsSaver->intTime));
        procSetting->addBackDelay(QString::number(settingsSaver->backDelay));
        procSetting->lineEdit_3->setText(QString::number(settingsSaver->backDelay));
        procSetting->addCriticalVal(QString::number(settingsSaver->critVal));
        procSetting->lineEdit->setText(QString::number(settingsSaver->critVal));
        procSetting->addInTrig(settingsSaver->nucleusTrig);
    }
}

