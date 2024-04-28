#include "callermainwindow.h"


CallerMainWindow::CallerMainWindow(QWidget *parent) : QMainWindow(parent) {
    parent = nullptr;
    QObject::connect(fluxCalc, &FluxCalc::sentMessage, this, &CallerMainWindow::printMsg);
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &CallerMainWindow::startByTimer);
    m_timer->setInterval(1000);
}

CallerMainWindow::~CallerMainWindow() noexcept {}

void CallerMainWindow::startByTimer() {
    char inputData[INPUT_DATA_BYTES];
    esp32->ReadSerialPort(inputData, INPUT_DATA_BYTES);

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
                    vecData->getData(resultsNew.rbegin()->second,counter);
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
                vecData->getData(resultsNew.rbegin()->second,counter);
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

        if (plotState>0)
            makePlot->PlotGraph(vecData->resultsDb);

        QString showLine = QString::fromStdString(res_out.str());

        textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
        QApplication::processEvents();
        QScrollBar*sb = textBrowser->verticalScrollBar();
        sb->setValue(sb->maximum());
        QApplication::processEvents();
        flushCounter+=1;
        if (flushCounter>15 )
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
    }

    if (!m_timer->isActive()) {

        FileWriter writer;
        writer.fileWriteVec(vecData->resultsDb);

        textBrowser->setText(textBrowser->toPlainText()
                             + "results stored in the file" + '\n');
        vecData->resultsDb.clear();
        fluxCalc->backVal = 0;
        fluxCalc->backCounter = 1;

        delete esp32;
        delete makePlot;

        onFlag = false;
        QApplication::processEvents();
    }
}

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

    makePlot = new Plotter();
    onFlag = true;
    counter = 0;
    flushCounter = 0;
    textBrowser->clear();
    QApplication::processEvents();

    int endTime = measTime;
    resultsNew.clear();
    esp32 = new SerialPort(pName.toStdString());

    m_timer->start();
}

void CallerMainWindow::input(QString dataEntered) {
    if (dataEntered.length()>0)
        measTime = std::stoi(dataEntered.toStdString());
}

void CallerMainWindow::addCoef(QString coef) {
    if (coef.length()>0)
        coefficient = std::stod(coef.toStdString());
}

std::vector<std::string> CallerMainWindow::fileRead(std::string str) {

    std::vector<std::string> fileData{};
    std::ifstream file;
    std::string line;
    std::stringstream lineData;
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
        makePlot = new Plotter();
        textBrowser->setText("\n");
        QApplication::processEvents();

        std::vector<std::string> fileData{};
        fileData = fileRead(fileName.toStdString());

        counter = 0;
        resultsNew.clear();
        tempVar1 = 0;
        tempVar2 = 0;

        for (int i=0; i<fileData.size(); i++)
        {
            tempVar1 = resultsNew.size();
            std::string inputValStr = fileData.at(i);
            std::string delimiter = "\n";
            size_t pos = 0;
            std::string token;
            std::string timeStr;
            while ((pos = inputValStr.find(delimiter)) != std::string::npos) {
                token = inputValStr.substr(0, pos);
                if (token.length() > 0) {
                    if (dotsFind(token,":").second == 5)
                    {
                        resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(token,":").first), token));
                        if (vecDataFile->resultsDb.size()>0)
                        {
                            fluxTrig = vecDataFile->resultsDb.at(1).back();
                        }
                        tempVar2 = resultsNew.size();
                        if (tempVar2>tempVar1)
                        {
                            vecDataFile->getData(resultsNew.rbegin()->second,counter);
                            fluxCalc->calculateFlux(*vecDataFile, fluxTrig);
                            counter+=1;
                        }
                    }
                }
                inputValStr.erase(0, pos + delimiter.length());
            }

            if (inputValStr!="")
            {
                tempVar1 = resultsNew.size();
                if (dotsFind(inputValStr,":").second==5)
                {
                    resultsNew.insert(std::pair<double, std::string>(std::stod(dotsFind(inputValStr,":").first),inputValStr));
                    if (vecDataFile->resultsDb.size()>0)
                    {
                        fluxTrig = vecDataFile->resultsDb.at(1).back();
                    }
                    tempVar2 = resultsNew.size();
                    if (tempVar2>tempVar1)
                    {
                        vecDataFile->getData(resultsNew.rbegin()->second,counter);
                        fluxCalc->calculateFlux(*vecDataFile, fluxTrig);
                        counter+=1;
                    }
                };
            }
        }
        if (vecDataFile->resultsDb.size()>0)
        {
            if (plotState>0)
                makePlot->PlotGraph(vecDataFile->resultsDb);

            for (int k = 0; k < vecDataFile->resultsDb.at(0).size(); k++) {
                std::stringstream res_out;
                for (int l=0;l<vecDataFile->resultsDb.size(); l++)
                {
                    res_out << vecDataFile->resultsDb.at(l).at(k) << " ";
                }
                QString showLine = QString::fromStdString(res_out.str());

//                textBrowser->setText(textBrowser->toPlainText()+showLine+'\n');
//                QApplication::processEvents();
//                QScrollBar*sb = textBrowser->verticalScrollBar();
//                sb->setValue(sb->maximum());
//                QApplication::processEvents();
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