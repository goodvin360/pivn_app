#include <numeric>
#include "vecFill.h"
#include "iostream"

vecFill::vecFill(QWidget *parent) {
    parent = nullptr;
    backVec.clear();
}

vecFill::~vecFill() {}

void vecFill::printMessage(QString msg) {
    emit sentMessage(msg);
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

        for (int i=0; i<3; i++)
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
    m = 0;
    flag = true;

    return resultsDb;
}

void vecFill::getDataTotal(std::vector<std::vector<double>> data, double totTime, double &flux) {
    fluxTime = totTime;
    if (data.at(0).size()==1)
        fluxTrig = data.at(1).back();

    if (data.at(0).size()>0)
    {
        resultsDbTotal.at(0)=data.at(0);
        double sum = 0;
        for (int i = 2; i < data.size(); i++) {
            sum += data.at(i).back()/(1-data.at(i).back()*5e-4);
        }
        resultsDbTotal.at(1).push_back(sum);
        sum = 0;

        temp = data.at(1).back();

        if (temp > fluxTrig) {
            isBack = false;
            fluxTrig = temp;
        }

        if (isBack) {
            backVec.push_back(resultsDbTotal.at(1).back());
            backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (backVec.size());
            resultsDbTotal.at(2).push_back(backVal);
        }

        if (!isBack && fluxTimeCounter == 0) {
            peakVal = resultsDbTotal.at(1).back();
            backLastVal = backVal;
            totalCnt=peakVal;
            fluxTimeCounter++;
        }

        if (!isBack && fluxTimeCounter>0) {
//            backVal = backLastVal + (0.000*exp(-(log(2)*fluxTimeCounter/3240)) + 0.0313*exp(-(log(2)*fluxTimeCounter/72))) *
//                                             (peakVal - backLastVal)/(1 + 0.000*exp(-(log(2)*fluxTimeCounter/3240)) +
//                                                         0.0313*exp(-(log(2)*fluxTimeCounter/72)));
            backVal = backLastVal + (0.005 + 0.000313) *
                                    (resultsDbTotal.at(1).back() - backLastVal)/(1 + 0.005 +
                                                             0.000313);
            resultsDbTotal.at(2).push_back(backVal);
            if (fluxTimeCounter>1)
                totalCnt += resultsDbTotal.at(1).back();
            fluxTimeCounter++;
            if (resultsDbTotal.at(1).back()>=resultsDbTotal.at(2).back())
                minusBack+=(resultsDbTotal.at(1).back()-resultsDbTotal.at(2).back());
        }

//        std::cout << "time: " << fluxTimeCounter << " diff value: " << resultsDbTotal.at(1).back()-resultsDbTotal.at(2).back() << std::endl;

        if (fluxTimeCounter>0)
            Flux = (minusBack+353)/(0.0004035)*
               ((1-exp(-(log(2)*fluxTime/14.1)))/(1-exp(-(log(2)*fluxTimeCounter/14.1))));

        if (fluxTimeCounter == fluxTime) {
            printMessage(QString::number(Flux));
            isBack = true;
            fluxTimeCounter = 0;
            backVal = 0;
            backVec.clear();
            temp = 0;
            minusBack = 0;
            Flux = 0;
        }
    }

//    return resultsDbTotal;
}