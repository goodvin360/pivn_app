#include "callermainwindow.h"

CallerMainWindow::CallerMainWindow(QWidget *parent) : QMainWindow(parent) {
    parent = nullptr;
    QObject::connect(fluxCalc, &FluxCalc::sentMessage, this, &CallerMainWindow::printMsg);
}

CallerMainWindow::~CallerMainWindow() noexcept {}

void CallerMainWindow::printMsg(QString msg) {
    textBrowser_2->setText(msg);
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
    Plotter*makePlot = new Plotter();

    onFlag = true;
    char inputData[INPUT_DATA_BYTES];

    textBrowser->setText("\n");
    QApplication::processEvents();

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
                    if (vecData->resultsDb.size()>0)
                    {
                        fluxTrig = vecData->resultsDb.at(1).back();
                    }
                    tempVar2 = resultsNew.size();
                    if (tempVar2>tempVar1)
                    {
                        vecData->getData(resultsNew.rbegin()->second,counter);
                        counter+=1;
//                            calculateFluxFo(true);
                        fluxCalc->calculateFlux(*vecData, fluxTrig);
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
//                        calculateFluxFo(true);
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

            makePlot->PlotGraph(vecData->resultsDb);

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
    fluxCalc->backVal = 0;
    fluxCalc->backCounter = 1;
}

void CallerMainWindow::input(QString dataEntered) {
    if (dataEntered.length()>0)
        measTime = std::stoi(dataEntered.toStdString());
}

void CallerMainWindow::addCoef(QString coef) {
    if (coef.length()>0)
        coefficient = std::stod(coef.toStdString());
}

void CallerMainWindow::addStartFile() {
    std::cout << fileName.toStdString() << std::endl;
    if (fileName!= NULL)
    {
        Plotter*makePlot = new Plotter();
        textBrowser->setText("\n");
        QApplication::processEvents();

        std::vector<std::string> fileData{};
        std::ifstream file;
        std::string line;
        std::stringstream lineData;
        double t,trig,ch1,ch2,ch3,ch4 = 0;
        size_t pos = 0;
        file.open(fileName.toStdString());
        while (getline(file, line))
        {
            while ((pos = line.find('\t')) != std::string::npos) {
                line.replace(line.begin()+pos,line.begin()+pos+1,":");
            }
            fileData.push_back(line);
        }

        file.close();

        int counter = 0;
        std::map<double, std::string> resultsNew{};
        int tempVar1 = 0;
        int tempVar2 = 0;

        for (int i=0; i<fileData.size(); i++)
        {
            tempVar1 = resultsNew.size();
            std::string inputValStr = fileData.at(i);
//                inputValStr+='\n';

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
//                                calculateFluxFo(false);
                            fluxCalc->calculateFlux(*vecDataFile, fluxTrig);
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
//                            calculateFluxFo(false);
                        fluxCalc->calculateFlux(*vecDataFile, fluxTrig);
                        counter+=1;
                    }
                };
            }
        }
        if (vecDataFile->resultsDb.size()>0)
        {
            makePlot->PlotGraph(vecDataFile->resultsDb);

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
    }
    vecDataFile->resultsDb.clear();
    fluxCalc->backVal = 0;
    fluxCalc->backCounter = 1;
}

void CallerMainWindow::addLoadFile() {
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "../", tr("txt files (*.txt)"));
}
