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

std::vector<std::vector<double>> vecFill::getData(std::string &str, int &counter, int cnt1, int cnt2, int cnt3, int cnt4, double resT[], size_t counters) {
    cnt = counter;
    for (size_t i{}; i < counters; i++)
    {
        resTime[i] = resT[i]*1e-6;
    }
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
    if (!resultsDb.at(0).empty())
    {
        resultsDbTotal.at(0).push_back(resultsDb.at(0).back());
        double sum = 0;
        double sum_clean = 0;
        for (int i = 2; i < resultsDb.size(); i++) {
            double var = 0;
            if (resultsDb.at(i).back() >= 1 / resTime[i - 2])
                var = 1 / resTime[i - 2] - 1;
            else var = resultsDb.at(i).back();
            if (cnt1 == 0)
                resultsDb.at(2).back() = 0;
            if (cnt2 == 0)
                resultsDb.at(3).back() = 0;
            if (cnt3 == 0)
                resultsDb.at(4).back() = 0;
            if (cnt4 == 0)
                resultsDb.at(5).back() = 0;
            sum += var / (1 - var * resTime[i - 2]);
            sum_clean += var;
        }
        resultsDbTotal.at(1).push_back(sum);
        resultsDbTotal.at(2).push_back(0);
            if (resultsDbTotal.at(2).size()>2)
            {
                double last_val = resultsDbTotal.at(2).at(resultsDbTotal.at(2).size()-2);
                if (last_val>0) {
                    resultsDbTotal.at(2).pop_back();
                    resultsDbTotal.at(2).push_back(last_val);
                }
            }
        resultsDbTotal.at(3).push_back(sum_clean);
    }
    return resultsDb;
}

void vecFill::getDataTotal(std::vector<std::vector<double>> &data, double totTime, double &flux, double&c_a, double&c_b,
                           bool fileParting, int trMode, int &trVal, double &ePoint, int constFluxTr, double &tPoint, double &tPointShift, int &constTrig,
                           int cnt1, int cnt2, int cnt3, int cnt4, int window, double &lftTime, int multiPulseTrig) {
    fluxTime = totTime+countStartTime;
    if (data.at(0).size()==1) {
        if (trMode==0)
            fluxTrig = data.at(1).back();
        else
            fluxTrig = trVal;
    }

    if (data.at(0).size()>0 && constFluxTr == 0)
    {
        if (trMode==0)
            temp = data.at(1).back();
        else
            temp = trVal;

        if (temp>fluxTrig)
        {
            currentTrigTime = resultsDbTotal.at(0).back();
            if (currentTrigTime>lastTrigTime+2)
            {
                lastTrigTime = currentTrigTime;
            }
            truePulse = true;
            fluxTrig = temp;
        }

        if (truePulse && lastTrigTime==currentTrigTime)
        {
            trigDelta = temp-fluxTrig;
            isBack = false;
            fluxTrig=temp;
            truePulse = false;
            pulsesTime.push_back(resultsDbTotal.at(0).back());
            prePulse = resultsDbTotal.at(1).at(resultsDbTotal.at(1).size()-1)-resultsDbTotal.at(1).at(resultsDbTotal.at(2).size()-2)-0*trigDelta;
            prePulsesData.push_back(prePulse);
            pulseCounter++;

            if (data.at(2).back()>=nCritical)
                countStartFlag = false;

            if (!countStartFlag)
            {
                prePulsesData.pop_back();
                prePulsesData.push_back(0);
            }

            if (isPulse)
                isSecondPulse = true;

            if (secondPulseCounter)
                isSecondPulse = true;

            isPulse = true;
        }

        secondPulseCounter = false;

        if (isBack) {
            if (backVec.size()>window && !backVec.empty())
            {
                for (int i=0; i<backVec.size()-window;i++)
                    backVec.erase(backVec.begin());
            }

            backVec.push_back(resultsDbTotal.at(1).back());
            backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));
            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(backVal);
            backLastVal = backVal;
            if (pulseCounter==0)
                firstBackVal = backLastVal;
        }

        if (!countStartFlag)
        {
            fluxTime+=fluxTimeCounter;
            if (data.at(4).back()<nCritical)
            {
                countStartFlag = true;
                countStartTime = fluxTimeCounter;
                fluxTime = totTime+countStartTime;
            }
        }


        if (!isBack && !isSecondPulse)
        {
            double backFromPulses = 0;

            if (!prePulsesData.empty() && !pulsesTime.empty())
            {
                for (int i=0; i<pulseCounter-1; i++)
                {
                    double t = resultsDbTotal.at(0).back()-pulsesTime.at(i);
                    backFromPulses += prePulsesData.at(i)*exp(-lmd116*t)+
                                        0.002027726*prePulsesData.at(i)*exp(-lmd116m*t)+
                                        0.002643049*prePulsesData.at(i)*exp(-lmd114*t);
                }
                backFromPulses+=(0.002027726*exp(-lmd116m*fluxTimeCounter) + 0.002643049*exp(-lmd114*fluxTimeCounter)) *
                                        (prePulsesData.back())/(1 + 0.002027726*exp(-lmd116m*fluxTimeCounter) +
                                             0.002643049*exp(-lmd114*fluxTimeCounter));
            }

            if (pulseCounter>=2)
            {
                if (resultsDbTotal.at(0).back()-pulsesTime.at(pulsesTime.size()-2)>=84)
                    backVal = backLastVal+backFromPulses;
                else
                    backVal = firstBackVal+backFromPulses;
            } else if (pulseCounter==1)
                backVal = backLastVal+  (0.002027726*exp(-lmd116m*fluxTimeCounter) + 0.002643049*exp(-lmd114*fluxTimeCounter)) *
                                        (prePulsesData.back())/(1 + 0.002027726*exp(-lmd116m*fluxTimeCounter) +
                                             0.002643049*exp(-lmd114*fluxTimeCounter));


            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(backVal);

            if (countStartFlag) {
                totalCnt += resultsDbTotal.at(1).back();
                totalCntClean += resultsDbTotal.at(3).back();

                minusBack += (resultsDbTotal.at(1).back() - resultsDbTotal.at(2).back());

                Flux = minusBack *
                       ((1 - exp(-(log(2) * 100 / 14.1))) /
                        (exp(-(log(2) * countStartTime / 14.1)) - exp(-(log(2) * fluxTimeCounter / 14.1)))) * c_a +
                       c_b;
                flux = Flux;

                lftTime = fluxTime-fluxTimeCounter;
            }


            fluxTimeCounter++;
        }

        if (fluxTimeCounter == fluxTime || isSecondPulse) {
            QString s(0x00B1);
            double error = 0.09*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            printMessage(QString::number(totalCntClean)+"   "+QString::number(totalCnt)+"   "+QString::number(minusBack),2);
            isBack = true;
            fluxTimeCounter = 0;
            countStartTime = 0;
            countStartFlag = true;
            backVal = resultsDbTotal.at(2).back();
            backLastVal = firstBackVal;
            backVec.clear();
            minusBack = 0;
            totalCnt = 0;
            totalCntClean = 0;
            Flux = 0;
            lftTime = totTime;
            isPulse = false;
            if (fluxTimeCounter==fluxTime)
                secondPulseCounter = true;
            if (isSecondPulse) {
                isPulse = true;
                isSecondPulse = false;
                isBack = false;
            }

        }

       /* if (multiPulseTrig==0)
            isSecondPulse = false;

        if (isPulse && temp>fluxTrig)
        {
            if (multiPulseTrig>0)
                isSecondPulse = true;
            t_last_pulse = resultsDbTotal.at(0).back();
        }

        if (fluxTimeCounter == fluxTime+1)
        {
            isPulse = false;
            plsD = fluxTimeCounter;
            fluxTimeCounter = 0;
        }

        if (isSecondPulse)
        {
            QString s(0x00B1);
            double error = 0.09*Flux;
            if (isPulse) {
                printMessage(QString::number(Flux, 'g', 3) + s + QString::number(error, 'g', 3), 1);
                printMessage(QString::number(totalCntClean) + "   " + QString::number(totalCnt) + "   " + QString::number(minusBack), 2);
            }
//            plsD = fluxTimeCounter;
            fluxTimeCounter = 0;
            minusBack = 0;
            Flux = 0;
            isSecondPulse = false;
            secondPulseCounter = true;
            prePulse = resultsDbTotal.at(1).at(resultsDbTotal.at(1).size()-2)-1*resultsDbTotal.at(2).at(resultsDbTotal.at(2).size()-2);
            pulseCounter++;
        }

        if (temp > fluxTrig && !isPulse) {
            trigDelta = temp - fluxTrig;
            isBack = false;
            isPulse = true;
            fileTrigger = true;
            t_last_pulse = resultsDbTotal.at(0).back();
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
            if (backVec.size()>window && !backVec.empty())
            {
                for (int i=0; i<backVec.size()-window;i++)
                    backVec.erase(backVec.begin());
            }

            backVec.push_back(resultsDbTotal.at(1).back());
            backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));
            resultsDbTotal.at(2).pop_back();
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
        double t = resultsDbTotal.at(0).back()-t_last_pulse;

        if (!isBack && fluxTimeCounter>0) {

            if (!secondPulseCounter && pulseCounter==0)
            backVal = backLastVal*1+
                    (0.002027726*exp(-lmd116m*fluxTimeCounter) + 0.002643049*exp(-lmd114*fluxTimeCounter)) *
                                    (peakVal - backLastVal)/(1 + 0.002027726*exp(-lmd116m*fluxTimeCounter) +
                    0.002643049*exp(-lmd114*fluxTimeCounter));

            if (!secondPulseCounter && pulseCounter>0)
                backVal = backLastVal*0+
                          (exp(-lmd116*t)/(exp(-lmd116*t)+0.002027726*exp(-lmd116m*t)+0.002643049*exp(-lmd114*t)))*
                          backLastVal*exp(-lmd116*fluxTimeCounter)+
                          (0.002027726*exp(-lmd116m*t)/(exp(-lmd116*t)+0.002027726*exp(-lmd116m*t)+0.002643049*exp(-lmd114*t)))*
                          backLastVal*exp(-lmd116m*fluxTimeCounter)+
                          (0.002643049*exp(-lmd114*t)/(exp(-lmd116*t)+0.002027726*exp(-lmd116m*t)+0.002643049*exp(-lmd114*t)))*
                          backLastVal*exp(-lmd114*fluxTimeCounter)+
                          (0.002027726*exp(-lmd116m*fluxTimeCounter) + 0.002643049*exp(-lmd114*fluxTimeCounter)) *
                          (peakVal - backLastVal)/(1 + 0.002027726*exp(-lmd116m*fluxTimeCounter) +
                                                 0.002643049*exp(-lmd114*fluxTimeCounter));

            if (secondPulseCounter)
                backVal = backLastVal*0 +
                        (exp(-lmd116*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                                backLastVal*exp(-lmd116*fluxTimeCounter)+
                        (0.002027726*exp(-lmd116m*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                                backLastVal*exp(-lmd116m*fluxTimeCounter)+
                        (0.002643049*exp(-lmd114*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                                backLastVal*exp(-lmd114*fluxTimeCounter)+
                        (0.002027726*exp(-lmd116m*fluxTimeCounter) + 0.002643049*exp(-lmd114*fluxTimeCounter)) *
                        (peakVal - backLastVal)/(1 + 0.002027726*exp(-lmd116m*fluxTimeCounter) +
                                                 0.002643049*exp(-lmd114*fluxTimeCounter))+
                        (exp(-lmd116*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                        prePulse*exp(-lmd116*fluxTimeCounter)+
                        (0.002027726*exp(-lmd116m*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                        prePulse*exp(-lmd116m*fluxTimeCounter)+
                        (0.002643049*exp(-lmd114*plsD)/(exp(-lmd116*plsD)+0.002027726*exp(-lmd116m*plsD)+0.002643049*exp(-lmd114*plsD)))*
                        prePulse*exp(-lmd114*fluxTimeCounter);

//            backVal = backLastVal;

            resultsDbTotal.at(2).pop_back();
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

        if (fluxTimeCounter == fluxTime+1) {
            QString s(0x00B1);
            double error = 0.09*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            printMessage(QString::number(totalCntClean)+"   "+QString::number(totalCnt)+"   "+QString::number(minusBack),2);
            isBack = true;
//            isPulse = false;
//            fluxTimeCounter = 0;
            backVal = 0;
            backVec.clear();
            temp = 0;
            minusBack = 0;
            Flux = 0;
            lftTime = fluxTime;
            plsD = 0;
            secondPulseCounter = false;
            pulseCounter++;
        }*/

    }

    if (!data.at(0).empty() && constFluxTr > 0)
    {
        backConstWindow = window;

        temp = constTrig;

        if (!isBackForConst && !derivativeSearch && temp > fluxTrigConst) {
            fluxTrigConst = temp;
            startPoint = resultsDbTotal.at(0).back();
            derivativeSearch = true;
        }

        if (!isBackForConst && derivativeSearch) {

            lftTime = fluxTime-fluxTimeCounter;

            double der = 0;
            if (resultsDbTotal.at(1).size()>1)
                der = abs(resultsDbTotal.at(1).back()-resultsDbTotal.at(1).at(resultsDbTotal.at(1).size()-2));
            derivativeVec.push_back(der);
            if (!derivativeVec.empty())
                tPoint = tPointShift+startPoint+distance(derivativeVec.begin(), max_element(derivativeVec.begin(), derivativeVec.end()));

            if (last_tPoint==tPoint && fluxTimeCounter==fluxTime) {
                derivativeSearch = false;
                isBackForConst = true;
                lftTime = 0;
                fluxTimeCounter--;
            }

            fluxTimeCounter++;

            if (last_tPoint != tPoint && last_edgePointShift==tPointShift) {
                totalCnt = 0;
                totalCntClean = 0;
                fluxTimeCounter = 0;
                last_tPoint = tPoint;
                if (abs(tPointShift)>=0 && fluxTimeCounter < fluxTime && !derivativeVec.empty())
                {
                    tPoint = startPoint+distance(derivativeVec.begin(), max_element(derivativeVec.begin(), derivativeVec.end()));
                    last_tPoint = tPoint;
                }
                tPointShift = 0;
                last_edgePointShift = 0;
            }

            if (fluxTimeCounter < fluxTime) {
                totalCnt += resultsDbTotal.at(1).back();
                totalCntClean += resultsDbTotal.at(3).back();
            }

            if (last_tPoint != tPoint && last_edgePointShift != tPointShift) {
                totalCnt=0;
                totalCntClean=0;
                double tmp = 0;
                for (int i=int(tPoint);i<resultsDbTotal.at(1).size();i++) {
                    totalCnt += resultsDbTotal.at(1).at(i);
                    totalCntClean += resultsDbTotal.at(3).at(i);
                    tmp++;
                }
                fluxTimeCounter = tmp-1;
                last_tPoint = tPoint;
                last_edgePointShift=tPointShift;
            }
        }


        if (fluxTimeCounter == fluxTime+backConstWindow) {
            isBackForConst = false;
        }

        if (isBackForConst) {

            if (backVecConst.size()>backConstWindow)
            {
                for (int i=0; i<backVecConst.size()-backConstWindow;i++)
                    backVecConst.erase(backVecConst.begin());
            }

            backVecConst.push_back(resultsDbTotal.at(1).back());
            backVal = std::accumulate(backVecConst.begin(), backVecConst.end(), 0) / (double(backVecConst.size()));
            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(backVal);
            for (int i=0; i<=fluxTimeCounter; i++)
            {
//                double bv = backVal*0.1;
//                double et = fluxTimeCounter;
//                resultsDbTotal.at(2).at(tPoint+i)=backVal*0+(exp(-lmd116*et)/(exp(-lmd116*et)+0.002027726*exp(-lmd116m*et)+0.002643049*exp(-lmd114*et)))*
//                        bv*exp(lmd116*(fluxTimeCounter-i))+
//                        (0.002027726*exp(-lmd116m*et)/(exp(-lmd116*et)+0.002027726*exp(-lmd116m*et)+0.002643049*exp(-lmd114*et)))*
//                        bv*exp(lmd116m*(fluxTimeCounter-i))+
//                        (0.002643049*exp(-lmd114*et)/(exp(-lmd116*et)+0.002027726*exp(-lmd116m*et)+0.002643049*exp(-lmd114*et)))*
//                        bv*exp(lmd114*(fluxTimeCounter-i));

                resultsDbTotal.at(2).at(tPoint+i)=backVal;
            }
            fluxTimeCounter++;
        }

        if (!isBackForConst && (fluxTimeCounter <= fluxTime || fluxTimeCounter >= fluxTime+backConstWindow)) {
            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(0);
        }

        if (fluxTimeCounter == fluxTime+backConstWindow) {
            minusBack = totalCnt-backVal*fluxTime;
            Flux = minusBack * c_a + c_b;
            flux = Flux;
        }

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
            constTrig = 0;
            minusBack = 0;
            Flux = 0;
            lftTime = fluxTime;
            derivativeVec.clear();
            fluxTrigConst=temp;
            derivativeSearch = false;
        }
    }
}

void vecFill::cleanUp() {
    resultsDb.clear();
    resultsDbTotal.clear();
    backVec.clear();
    backVal = 0;
    backLastVal = 0;
    isBack = true;
    isPulse = false;
    fluxTimeCounter = 0;
    countStartTime = 0;
    countStartFlag = true;
    temp = 0;
    minusBack = 0;
    Flux = 0;
    plsD = 0;
    secondPulseCounter = false;
    pulseCounter = 0;
    derivativeVec.clear();
    derivativeSearch= false;
    fluxTrigConst=0;
    isBackForConst=false;
    last_edgePointShift = 0;
    prePulsesData.clear();
    pulsesTime.clear();
    lastTrigTime = currentTrigTime = 0;
}
