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

std::vector<std::vector<double>> vecFill::getData(std::string str, int counter, int cnt1, int cnt2, int cnt3, int cnt4) {
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
            if (m==2 && cnt1 == 0)
                token = "0";
            if (m==3 && cnt2 == 0)
                token = "0";
            if (m==4 && cnt3 == 0)
                token = "0";
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
        if (m==5 && cnt4 == 0)
            var = "0";
        resultsDb.at(m).push_back(stod(var));
    }

    return resultsDb;
}

void vecFill::getDataTotal(std::vector<std::vector<double>> data, double totTime, double &flux, double&c_a, double&c_b,
                           bool fileParting, int trMode, int trVal, double &ePoint, int constFluxTr, double &tPoint, int constTrig,
                           int cnt1, int cnt2, int cnt3, int cnt4, int window, double &lftTime, int multiPulseTrig) {
    fluxTime = totTime;
    if (data.at(0).size()==1) {
        if (trMode==0)
            fluxTrig = data.at(1).back();
        else
            fluxTrig = trVal;
    }

    if (data.at(0).size()>0 && constFluxTr == 0)
    {
        resultsDbTotal.at(0).push_back(data.at(0).back());
        ePoint = resultsDbTotal.at(0).back();
        double sum = 0;
        double sum_clean = 0;
        for (int i = 2; i < data.size(); i++) {
            if (data.at(i).back()>=1/resTime[i-2])
                data.at(i).back()=1/resTime[i-2]-1;
            if (cnt1==0)
                data.at(2).back()=0;
            if (cnt2==0)
                data.at(3).back()=0;
            if (cnt3==0)
                data.at(4).back()=0;
            if (cnt4==0)
                data.at(5).back()=0;
            sum += data.at(i).back()/(1-data.at(i).back()*resTime[i-2]);
            sum_clean += data.at(i).back();
        }
        resultsDbTotal.at(1).push_back(sum);
        resultsDbTotal.at(3).push_back(sum_clean);

        if (trMode==0)
            temp = data.at(1).back();
        else
            temp = trVal;

        if (multiPulseTrig==0)
            isSecondPulse = false;

        if (isPulse && temp>fluxTrig)
        {
            if (multiPulseTrig>0)
                isSecondPulse = true;
        }

        if (isSecondPulse)
        {
            QString s(0x00B1);
            double error = 0.09*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            printMessage(QString::number(totalCntClean)+"   "+QString::number(totalCnt)+"   "+QString::number(minusBack),2);
            plsD = fluxTimeCounter;
            fluxTimeCounter = 0;
            minusBack = 0;
            Flux = 0;
            isSecondPulse = false;
            secondPulseCounter = true;
            prePulse = resultsDbTotal.at(1).at(resultsDbTotal.at(1).size()-2);
        }

        if (temp > fluxTrig && !isPulse) {
            trigDelta = temp - fluxTrig;
            isBack = false;
            isPulse = true;
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
            if (backVec.size()>window)
            {
                for (int i=0; i<backVec.size()-window;i++)
                    backVec.erase(backVec.begin());
            }

            backVec.push_back(resultsDbTotal.at(1).back());
            backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));
            resultsDbTotal.at(2).push_back(backVal);
        }

        if (!isBack && fluxTimeCounter < 1) {
            peakVal = resultsDbTotal.at(1).back()-trigDelta*0;
            resultsDbTotal.at(1).back() = peakVal;
            peakValClean = resultsDbTotal.at(3).back()-trigDelta*0;
            resultsDbTotal.at(3).back() = peakValClean;
            backLastVal = backVal;
            totalCnt=peakVal;
            totalCntClean=peakValClean;
            fluxTimeCounter++;
        }
        double t = resultsDbTotal.at(0).back();

        if (!isBack && fluxTimeCounter>0) {

//            backVal = backLastVal + (0.008*exp(-(log(2)*fluxTimeCounter/3240)) + 0.0213*exp(-(log(2)*fluxTimeCounter/72))) *
//                                             (peakVal - backLastVal)/(1 + 0.008*exp(-(log(2)*fluxTimeCounter/3240)) +
//                                                         0.0213*exp(-(log(2)*fluxTimeCounter/72)));

            if (!secondPulseCounter)
            backVal = backLastVal*0+
                    (exp(-lmd116*t)/(exp(-lmd116*t)+0.002027726*exp(-lmd116m*t)+0.002643049*exp(-lmd114*t)))*
                    backLastVal*exp(-lmd116*fluxTimeCounter)+
                    (0.002027726*exp(-lmd116m*t)/(exp(-lmd116*t)+0.002027726*exp(-lmd116m*t)+0.002643049*exp(-lmd114*t)))*
                    backLastVal*exp(-lmd116m*fluxTimeCounter)+
                    (0.002643049*exp(-lmd114*t)/(exp(-lmd116*t)+0.002027726*exp(-lmd116m*t)+0.002643049*exp(-lmd114*t)))*
                    backLastVal*exp(-lmd114*fluxTimeCounter)+
                    (0.002027726*exp(-(log(2)*fluxTimeCounter/3240)) + 0.002643049*exp(-(log(2)*fluxTimeCounter/72))) *
                                    (peakVal - backLastVal)/(1 + 0.002027726*exp(-(log(2)*fluxTimeCounter/3240)) +
                    0.002643049*exp(-(log(2)*fluxTimeCounter/72)));

            if (secondPulseCounter)
                backVal = backLastVal*0 +
                        (exp(-lmd116*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                                backLastVal*exp(-lmd116*fluxTimeCounter)+
                        (0.002027726*exp(-lmd116m*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                                backLastVal*exp(-lmd116m*fluxTimeCounter)+
                        (0.002643049*exp(-lmd114*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                                backLastVal*exp(-lmd114*fluxTimeCounter)+
                        (0.002027726*exp(-(log(2)*fluxTimeCounter/3240)) + 0.002643049*exp(-(log(2)*fluxTimeCounter/72))) *
                                        (peakVal-prePulse)/(1 + 0.002027726*exp(-(log(2)*fluxTimeCounter/3240)) +
                                                                 0.002643049*exp(-(log(2)*fluxTimeCounter/72)))+
                        (exp(-lmd116*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                        prePulse*exp(-lmd116*fluxTimeCounter)+
                        (0.002027726*exp(-lmd116m*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                        prePulse*exp(-lmd116m*fluxTimeCounter)+
                        (0.002643049*exp(-lmd114*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                        prePulse*exp(-lmd114*fluxTimeCounter);

//            backVal = backLastVal + (0.005 + 0.00313) *
//                                    (resultsDbTotal.at(1).back() - backLastVal)/(1 + 0.005 +
//                                                             0.00313);

//            backVal = backLastVal;

            resultsDbTotal.at(2).push_back(backVal);

            if (fluxTimeCounter>1) {
                totalCnt += resultsDbTotal.at(1).back();
                totalCntClean += resultsDbTotal.at(3).back();
            }

            if (fluxTimeCounter>0)
                minusBack+=(resultsDbTotal.at(1).back()-resultsDbTotal.at(2).back());

            if (fluxTimeCounter>0) {
                Flux = minusBack *
                       ((1-exp(-(log(2)*100/14.1)))/(exp(-(log(2)*0/14.1)) - exp(-(log(2)*fluxTimeCounter/14.1)))) * c_a +
                       c_b;
                flux = Flux;
            }

            lftTime = fluxTime-fluxTimeCounter;

            fluxTimeCounter++;
        }


        if (fluxTimeCounter == fluxTime) {
            QString s(0x00B1);
            double error = 0.09*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            printMessage(QString::number(totalCntClean)+"   "+QString::number(totalCnt)+"   "+QString::number(minusBack),2);
            isBack = true;
            isPulse = false;
            fluxTimeCounter = 0;
            backVal = 0;
//            backVec.clear();
            temp = 0;
            minusBack = 0;
            Flux = 0;
            lftTime = fluxTime;
            plsD = 0;
            secondPulseCounter = false;
        }

    }

    if (data.at(0).size()>0 && constFluxTr > 0)
    {
        resultsDbTotal.at(0).push_back(data.at(0).back());
        ePoint = resultsDbTotal.at(0).back();
        double sum = 0;
        double sum_clean = 0;
        for (int i = 2; i < data.size(); i++) {
            if (data.at(i).back()>=1/resTime[i-2])
                data.at(i).back()=1/resTime[i-2]-1;
            if (cnt1==0)
                data.at(2).back()=0;
            if (cnt2==0)
                data.at(3).back()=0;
            if (cnt3==0)
                data.at(4).back()=0;
            if (cnt4==0)
                data.at(5).back()=0;
            sum += data.at(i).back()/(1-data.at(i).back()*resTime[i-2]);
            sum_clean += data.at(i).back();
        }
        resultsDbTotal.at(1).push_back(sum);
        resultsDbTotal.at(3).push_back(sum_clean);

        temp = constTrig;

        if (!isBackForConst && fluxTimeCounter == 0 && temp > fluxTrigConst) {
            peakVal = resultsDbTotal.at(1).back();
            peakValClean = resultsDbTotal.at(3).back();
            totalCnt=peakVal;
            totalCntClean=peakValClean;
            fluxTimeCounter++;
        }

        if (!isBackForConst && fluxTimeCounter > 0 && fluxTimeCounter <= fluxTime+backConstWindow) {

            if (fluxTimeCounter>1 && fluxTimeCounter < fluxTime) {
                totalCnt += resultsDbTotal.at(1).back();
                totalCntClean += resultsDbTotal.at(3).back();
            }
            fluxTimeCounter++;
        }


        if (fluxTimeCounter >= fluxTime) {
            isBackForConst = true;
        }

        if (fluxTimeCounter > fluxTime+backConstWindow) {
            isBackForConst = false;
        }

        if (isBackForConst) {
            backVecConst.push_back(resultsDbTotal.at(1).back());
//            if (!backVecConst.empty())
                backVal = std::accumulate(backVecConst.begin(), backVecConst.end(), 0) / (double(backVecConst.size()));
            resultsDbTotal.at(2).push_back(backVal);
            fluxTimeCounter++;
        }

        if (!isBackForConst && (fluxTimeCounter < fluxTime || fluxTimeCounter > fluxTime+backConstWindow))
            resultsDbTotal.at(2).push_back(0);

        fluxTrigConst = temp;

        if (fluxTimeCounter == fluxTime+backConstWindow) {
            minusBack = totalCnt-backVal*fluxTime;

            Flux = minusBack * c_a + c_b;
            flux = Flux;
        }

        lftTime = fluxTime-fluxTimeCounter;

        if (fluxTimeCounter == fluxTime+backConstWindow) {
            QString s(0x00B1);
            double error = 0.09*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            printMessage(QString::number(totalCntClean)+"   "+QString::number(totalCnt)+"   "+QString::number(minusBack),2);
            isBackForConst = false;
            fluxTimeCounter = 0;
            backVal = 0;
            backVecConst.clear();
            temp = 0;
            minusBack = 0;
            Flux = 0;
            lftTime = fluxTime;
        }
    }
}