#include <numeric>
#include "vecFill.h"
#include "iostream"

vecFill::vecFill(QWidget *parent) {
    parent = nullptr;
    backVec.clear();
    fileWriter = new FileWriter();
}

vecFill::~vecFill() {}

void vecFill::printMessage(QString msg, int num) {
    emit sentMessage(msg, num);
}

std::vector<std::vector<double>> vecFill::getData(std::string str, int counter) {
    cnt = counter;
    if (counter==0)
    {
        for (int i=0; i<6; i++)
        {
            std::vector<double> smData;
            resultsDb.push_back(smData);
        }

        for (int i=0; i<6; i++)
        {
            std::vector<double> smData;
            resultsDbPart.push_back(smData);
        }

        for (int i=0; i<4; i++)
        {
            std::vector<double> smData;
            resultsDbTotal.push_back(smData);
        }
    }

    int m = 0;
    std::string var;
    std::string delimiter2 = ":";
    size_t pos = 0;
    std::string token;
    bool flag = true;
    var = str;
    var += '\n';
    while ((pos = var.find(delimiter2)) != std::string::npos) {
        token = var.substr(0, pos);
        for (int k = 0; k < token.size(); k++) {
            if (token[k] < '0' || token[k] > '9') {
                flag = false;
            }
        }
        if (m==0 && flag)
        {
//            resultsDb.at(m).push_back(0.001*stod(token));
            resultsDb.at(m).push_back(counter);
        }
        else if (flag)
        {
            resultsDb.at(m).push_back(stod(token));
        }
        var.erase(0, pos + delimiter2.length());
        m++;
        flag = true;
    }
    for (int k = 0; k < var.size()-1; k++) {
        if (var[k] < '0' || var[k] > '9') {
            flag = false;
        }
    }
    if (flag)
    {
        resultsDb.at(m).push_back(stod(var));
    }

    return resultsDb;
}

void vecFill::getDataTotal(std::vector<std::vector<double>> data, double totTime, double &flux, double&c_a, double&c_b, bool fileParting) {
    fluxTime = totTime;
    if (data.at(0).size()==1)
        fluxTrig = data.at(1).back();

    if (data.at(0).size()>0)
    {
        resultsDbTotal.at(0).push_back(data.at(0).back());
        double sum = 0;
        double sum_clean = 0;
        for (int i = 2; i < data.size(); i++) {
            if (data.at(i).back()>=2000)
                data.at(i).back()=1999;
            sum += data.at(i).back()/(1-data.at(i).back()*5e-4);
            sum_clean += data.at(i).back();
        }
        resultsDbTotal.at(1).push_back(sum);
        resultsDbTotal.at(3).push_back(sum_clean);

        temp = data.at(1).back();

        if (temp > fluxTrig) {
            isBack = false;
            fileTrigger = true;
        }

        fluxTrig = temp;

        if (fileTrigger)
            fileTimer++;

        if (fileTimer>=150 && fileParting)
        {
            int start = 0;
            if (resultsDb.at(0).size()>200)
                start = resultsDb.at(0).back()-200;
            for (int i = start; i<resultsDb.at(0).size(); i++)
            {
                for (int j=0; j<resultsDb.size(); j++)
                {
                    resultsDbPart.at(j).push_back(resultsDb.at(j).at(i));
                }
            }

            fileWriter->fileWriteVec(resultsDbPart, "part");

            for (int i=0; i<resultsDbPart.size(); i++)
                resultsDbPart.at(i).clear();

            fileTimer = 0;
            fileTrigger = false;
        }

        if (isBack) {
            backVec.push_back(resultsDbTotal.at(1).back());
            backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (backVec.size());
            resultsDbTotal.at(2).push_back(backVal);
        }

        if (!isBack && fluxTimeCounter == 0) {
            peakVal = resultsDbTotal.at(1).back();
            peakValClean = resultsDbTotal.at(3).back();
            backLastVal = backVal;
            totalCnt=peakVal;
            totalCntClean=peakValClean;
            fluxTimeCounter++;
        }

        if (!isBack && fluxTimeCounter>0) {
//            backVal = backLastVal + (0.008*exp(-(log(2)*fluxTimeCounter/3240)) + 0.0213*exp(-(log(2)*fluxTimeCounter/72))) *
//                                             (peakVal - backLastVal)/(1 + 0.008*exp(-(log(2)*fluxTimeCounter/3240)) +
//                                                         0.0213*exp(-(log(2)*fluxTimeCounter/72)));
            backVal = backLastVal + (0.005 + 0.000313) *
                                    (resultsDbTotal.at(1).back() - backLastVal)/(1 + 0.005 +
                                                             0.000313);
            resultsDbTotal.at(2).push_back(backVal);
            if (fluxTimeCounter>1) {
                totalCnt += resultsDbTotal.at(1).back();
                totalCntClean += resultsDbTotal.at(3).back();
            }
            fluxTimeCounter++;
//            if (resultsDbTotal.at(1).back()>=resultsDbTotal.at(2).back())
                minusBack+=(resultsDbTotal.at(1).back()-resultsDbTotal.at(2).back());
        }

//        std::cout << "time: " << fluxTimeCounter << " diff value: " << resultsDbTotal.at(1).back()-resultsDbTotal.at(2).back() << std::endl;

        if (fluxTimeCounter>0) {
            Flux = minusBack *
                   ((1 - exp(-(log(2) * 100 / 14.1))) / (1 - exp(-(log(2) * fluxTimeCounter / 14.1)))) * c_a +
                   c_b;
            flux = Flux;
        }

        if (fluxTimeCounter == fluxTime) {
            QString s(0x00B1);
            double error = 0.09*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            printMessage(QString::number(totalCntClean)+"   "+QString::number(totalCnt)+"   "+QString::number(minusBack),2);
            isBack = true;
            fluxTimeCounter = 0;
            backVal = 0;
            backVec.clear();
            temp = 0;
            minusBack = 0;
            Flux = 0;
        }
    }
}