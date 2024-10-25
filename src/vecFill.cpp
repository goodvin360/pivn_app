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
        if (resTime[i]>maxResTime)
            maxResTime = resTime[i];
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

        for (int i=0; i<5; i++)
        {
            std::vector<double> smData;
            resultsDbSeparated.push_back(smData);
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
        resultsDbSeparated.at(0).push_back(resultsDb.at(0).back());
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
            if ((i-1)<5)
                resultsDbSeparated.at(i-1).push_back(var);
            if (resultsDb.at(i).back()>maxCountRate)
                maxCountRate = resultsDb.at(i).back();
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
                           bool& offTrigger, int trMode, int &trVal, double &ePoint, int constFluxTr, double &tPoint, double &tPointShift, int &constTrig,
                           double backDelayTime, int window, double &lftTime, int mPulses, int clearTrig,
                           double critLvl, double intTime) {
    fluxTime = totTime+countStartTime;
    nCritical = (critLvl/100)/maxResTime;
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

        currentVecSize=data.at(1).size();
        if (currentVecSize>previousVecSize && !offTrigger)
            previousVecSize = currentVecSize;
        if (currentVecSize>previousVecSize && offTrigger){
            previousVecSize = currentVecSize;
            offTrigger = false;
            temp = fluxTrig+1;
        }

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

            if (maxCountRate>=nCritical)
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

        /*if (!prePulsesData.empty() && resultsDbTotal.at(1).back()>prePulsesData.back()){
            prePulsesData.pop_back();
            prePulsesData.push_back(resultsDbTotal.at(1).back()-resultsDbTotal.at(2).back());
        }*/

        if (maxCountRate>=nCritical && !criticalChange)
            countStartFlag = false;

        secondPulseCounter = false;

        double lastPulseTime = 0;
        if (!pulsesTime.empty())
            lastPulseTime = pulsesTime.back();

        bool cndOne;
        if (pulsesTime.empty())
            cndOne = isBack;
        else
            cndOne = isBack && resultsDbTotal.at(0).back()-fluxTime-lastPulseTime-countStartTime>backDelayTime;

        if (cndOne) {
            if (backVec.size()>window && !backVec.empty())
            {
                for (int i=0; i<backVec.size()-window;i++)
                    backVec.erase(backVec.begin());
            }

            backVec.push_back(resultsDbTotal.at(1).back());
            for (auto it=backVec.begin(); it!=backVec.end(); it++)
            {
                backVal+=it.operator*();
            }
            backVal = backVal/double(backVec.size());
//            backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));
            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(backVal);
            backLastVal = backVal;
            if (pulseCounter==0)
                firstBackVal = backLastVal;
        }

        if (!countStartFlag)
        {
            fluxTime+=fluxTimeCounter;
            if (maxCountRate<nCritical && resultsDbTotal.at(0).back()-currentTrigTime>0)
            {
                countStartFlag = true;
                countStartTime = fluxTimeCounter;
                fluxTime = totTime+countStartTime;
                criticalChange = true;
            }
        }

        if (!pulsesTime.empty())
            countStartTimeReal = pulsesTime.back()+countStartTime;


        if (!isBack && !isSecondPulse)
        {
            double backFromPulses = 0;

            if (mPulses>0)
            {
                if (!prePulsesData.empty() && !pulsesTime.empty())
                {
                    for (int i=0; i<pulseCounter-1; i++)
                    {
                        double t = resultsDbTotal.at(0).back()-pulsesTime.at(i);
                        /*backFromPulses += prePulsesData.at(i)*exp(-lmd116*t)+
                                            0.002027726*prePulsesData.at(i)*exp(-lmd116m*t)+
                                            0.002643049*prePulsesData.at(i)*exp(-lmd114*t);*/
                        backFromPulses += prePulsesData.at(i)*exp(-lmd116*t);
                    }
                    backFromPulses+=(0.002027726*exp(-lmd116m*fluxTimeCounter) + 0.002643049*exp(-lmd114*fluxTimeCounter)) *
                                            (prePulsesData.back())/(1 + 0.002027726*exp(-lmd116m*fluxTimeCounter) +
                                                 0.002643049*exp(-lmd114*fluxTimeCounter));
                }
            }
            if (mPulses==0)
                backFromPulses=0;

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
                backOut = backVal*backVec.size();
                minusBackTrue = minusBack*((1 - exp(-(log(2) * intTime / 14.1))) /
                                           (exp(-(log(2) * countStartTime / 14.1)) - exp(-(log(2) * fluxTimeCounter / 14.1))));

                totalCntFullTime = totalCnt*((1 - exp(-(log(2) * intTime / 14.1))) /
                                             (exp(-(log(2) * countStartTime / 14.1)) - exp(-(log(2) * fluxTimeCounter / 14.1))));

                Flux = minusBackTrue * c_a + c_b;
                flux = Flux;
                statErr = sqrt(totalCnt+backOut*pow(totTime/backVec.size(),2))/minusBack;
                if (statErr<0)
                    statErr = 0;
                lftTime = fluxTime-fluxTimeCounter;

                double var1 = 0;
                double var2 = 1;

                for (int i=1; i<5; i++)
                {
                    var1=resultsDbSeparated.at(i).back()+pow(deltaResTime*resTime[i-1],2)*pow(resultsDbSeparated.at(i).back(),4);
                    var2=pow((resTime[i-1]*resultsDbSeparated.at(i).back()-1),2);
                    tempErr1+=var1/var2;
                }

                tempErr2 = 0;
                for (int i=1; i<5; i++)
                {
                    var1 = backLastVal/4+pow(deltaResTime*resTime[i-1],2)*pow(backVal/4,4);
                    var2 = pow((resTime[i-1]*backVal/4-1),2);
                    tempErr2+=var1/var2;
                }
                tempErr2=backVec.size()*tempErr2;

                countErr = sqrt(tempErr1+tempErr2*pow(totTime/backVec.size(),2))/minusBack;
                if (countErr<0)
                    countErr = 0;

            }

            fluxTimeCounter++;
        }

        if (fluxTimeCounter == fluxTime || isSecondPulse) {
            QString s(0x00B1);
            double error = statErr*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            msgFillUp();
            printMessage(pulseDataMsg,2);
            isBack = true;
            fluxTimeCounter = 0;
            countStartTime = 0;
            countStartFlag = false;
            backVal = resultsDbTotal.at(2).back();
            backLastVal = firstBackVal;
            minusBack = 0;
            minusBackTrue = 0;
            totalCnt = 0;
            totalCntClean = 0;
            totalCntFullTime = 0;
            Flux = 0;
            lftTime = totTime;
            isPulse = false;
            maxCountRate = 0;
            maxResTime = 0;
            criticalChange = false;
            pulseDataMsg.clear();
            backOut = 0;
            statErr = 0;
            tempErr1 = 0;
            tempErr2 = 0;
            countErr = 0;
            if (fluxTimeCounter==fluxTime)
                secondPulseCounter = true;
            if (isSecondPulse) {
                isPulse = true;
                isSecondPulse = false;
                isBack = false;
            }
            if (clearTrig>0)
                backVec.clear();
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

        if (resultsDbTotal.at(1).size()>1) {
            double der = 0;
            der = resultsDbTotal.at(1).back() - resultsDbTotal.at(1).at(resultsDbTotal.at(1).size() - 2);
            derivativeAllVec.push_back(der);
            derivativeAllTime.push_back(resultsDbTotal.at(0).back());
        }

        if (!isBackForConst && derivativeSearch) {

            lftTime = fluxTime-fluxTimeCounter;

            double der = 0;
            if (resultsDbTotal.at(1).size()>1)
                der = abs(resultsDbTotal.at(1).back()-resultsDbTotal.at(1).at(resultsDbTotal.at(1).size()-2));
            derivativeVec.push_back(der);
            if (!derivativeVec.empty())
                tPoint = tPointShift+startPoint+distance(derivativeVec.begin(), max_element(derivativeVec.begin(), derivativeVec.end()))+criticalTime;

            if (last_tPoint==tPoint && fluxTimeCounter==fluxTime+backDelayTime) {
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
                    tPoint = startPoint+distance(derivativeVec.begin(), max_element(derivativeVec.begin(), derivativeVec.end()))+criticalTime;
                    last_tPoint = tPoint;
                }
                tPointShift = 0;
                last_edgePointShift = 0;
            }

            if (maxCountRate>=nCritical && !criticalChange) {
                countStartFlagConst = false;
                fluxTimeCounter = 0;
                criticalTime = 0;
            }

            if (maxCountRate<nCritical) {
                if (!countStartFlagConst) {
                    criticalTime = resultsDbTotal.at(0).back()-tPoint;
                    criticalChange = true;
                    if (criticalTime>0)
                        critTimeActual = criticalTime;
                }
                countStartFlagConst = true;
            }

            if (fluxTimeCounter < fluxTime && countStartFlagConst) {
                totalCnt += resultsDbTotal.at(1).back();
                totalCntClean += resultsDbTotal.at(3).back();
            }

            if (last_tPoint != tPoint && last_edgePointShift != tPointShift && fluxTimeCounter<fluxTime) {
                totalCnt=0;
                totalCntClean=0;
                critTimeActual++;
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

        if (fluxTimeCounter == fluxTime+backConstWindow+backDelayTime) {
            isBackForConst = false;
        }

        if (isBackForConst && resultsDbTotal.at(0).back()-fluxTime-last_tPoint>backDelayTime) {

            if (backVecConst.size()>backConstWindow)
            {
                for (int i=0; i<backVecConst.size()-backConstWindow;i++)
                    backVecConst.erase(backVecConst.begin());
            }

            backVecConst.push_back(resultsDbTotal.at(1).back());
            for (auto it=backVecConst.begin(); it!=backVecConst.end(); it++)
            {
                backVal+=it.operator*();
            }
            backVal = backVal/double(backVecConst.size());
//            backVal = std::accumulate(backVecConst.begin(), backVecConst.end(), 0) / (double(backVecConst.size()));
            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(backVal);
            for (int i=0; i<=fluxTimeCounter+1; i++)
            {
                resultsDbTotal.at(2).at(tPoint+i)=0.9*backVal*exp(lmd116m*(fluxTimeCounter-i))+0.0125*backVal*exp(lmd114*(fluxTimeCounter-i));
            }
            fluxTimeCounter++;
        }

        if (!isBackForConst && (fluxTimeCounter < fluxTime || fluxTimeCounter > fluxTime+backConstWindow+backDelayTime)) {
            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(0);
        }

        if (fluxTimeCounter == fluxTime+backConstWindow+backDelayTime) {
//            minusBack = totalCnt-backVal*fluxTime;
//            backOut = backVal*backVecConst.size();
            for (int i=0; i<fluxTime;i++)
            {
                minusBack+=resultsDbTotal.at(1).at(tPoint+i)-resultsDbTotal.at(2).at(tPoint+i);
            }
            for (int i=0; i<backVecConst.size();i++)
            {
                backOut+=resultsDbTotal.at(2).at(tPoint+fluxTime+backDelayTime+i);
            }
            totalCntFullTime = minusBack*((1 - exp(-(log(2) * intTime / 14.1))) /
                                         (exp(-(log(2) * criticalTime / 14.1)) - exp(-(log(2) * fluxTimeCounter / 14.1))));
            minusBackTrue = minusBack*((1 - exp(-(log(2) * intTime / 14.1))) /
                                       (exp(-(log(2) * criticalTime / 14.1)) - exp(-(log(2) * fluxTimeCounter / 14.1))));;
            Flux = minusBackTrue *c_a + c_b;
            flux = Flux;
            statErr = sqrt(totalCnt+backOut*pow(fluxTime/backVecConst.size(),2))/minusBack;
            if (statErr<0)
                statErr = 0;

            double var1 = 0;
            double var2 = 1;

            int timePoint = resultsDbSeparated.at(0).size()-fluxTimeCounter;
            for (int i=1; i<5; i++) {
                for (int j=0; j<fluxTime; j++) {
                    var1 = resultsDbSeparated.at(i).at(j+timePoint) + pow(deltaResTime * resTime[i - 1], 2) * pow(resultsDbSeparated.at(i).at(j+timePoint), 4);
                    var2 = pow((resTime[i - 1] * resultsDbSeparated.at(i).at(j+timePoint) - 1), 2);
                    tempErr1 += var1 / var2;
                }
            }

            tempErr2 = 0;
            for (int i=1; i<5; i++) {
                var1 = backVal/4+pow(deltaResTime*resTime[i-1],2)*pow(backVal/4,4);
                var2 = pow((resTime[i-1]*backVal/4-1),2);
                tempErr2+=backVecConst.size()*var1/var2;
            }

            countErr = sqrt(tempErr1+tempErr2*pow(totTime/backVecConst.size(),2))/minusBack;
            if (countErr<0)
                countErr = 0;

        }

        if (fluxTimeCounter == fluxTime+backConstWindow+backDelayTime) {

            int tail = 400;
            if (derivativeAllVec.size()<tail)
                tail = 0;
            for (auto it=derivativeAllVec.size()-1; it!=tail; it--) {
                if (derivativeAllVec.at(it)>derMax) {
                    derMax = derivativeAllVec.at(it);
                    derMaxTime = derivativeAllTime.at(it);
                }
                if (derivativeAllVec.at(it)<derMin) {
                    derMin = derivativeAllVec.at(it);
                    derMinTime = derivativeAllTime.at(it);
                }
            }

            countStartTimeConst = resultsDbTotal.at(0).back()-backDelayTime-backConstWindow-fluxTime;

            std::cout << 100*countErr << std::endl;

            QString s(0x00B1);
            double error = statErr*Flux;
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            msgFillUp();
            printMessage(constDataMsg,3);
            isBackForConst = false;
            fluxTimeCounter = 0;
            backVal = 0;
            backVecConst.clear();
            temp = 0;
            constTrig = 0;
            minusBack = 0;
            Flux = 0;
            totalCnt = 0;
            totalCntClean = 0;
            totalCntFullTime = 0;
            lftTime = fluxTime;
            derivativeVec.clear();
            derivativeAllVec.clear();
            derivativeAllTime.clear();
            derMax=derMin=derMaxTime=derMinTime=0;
            fluxTrigConst=temp;
            derivativeSearch = false;
            countStartFlagConst = false;
            criticalTime = 0;
            criticalChange = false;
            maxCountRate = 0;
            maxResTime = 0;
            critTimeActual = 0;
            constDataMsg.clear();
            backOut = 0;
            statErr = 0;
            tempErr1 = 0;
            tempErr2 = 0;
            countErr = 0;
        }
    }

    maxCountRate = 0;
}

void vecFill::cleanUp() {
    resultsDb.clear();
    resultsDbTotal.clear();
    resultsDbSeparated.clear();
    backVec.clear();
    backVal = 0;
    backLastVal = 0;
    isBack = true;
    isPulse = false;
    truePulse = false;
    fluxTimeCounter = 0;
    countStartTime = 0;
    countStartFlag = false;
    temp = 0;
    minusBack = 0;
    minusBackTrue = 0;
    Flux = 0;
    plsD = 0;
    secondPulseCounter = false;
    pulseCounter = 0;
    derivativeVec.clear();
    derivativeAllVec.clear();
    derivativeAllTime.clear();
    derMax=derMin=derMaxTime=derMinTime=0;
    derivativeSearch= false;
    fluxTrigConst=0;
    isBackForConst=false;
    last_edgePointShift = 0;
    prePulsesData.clear();
    pulsesTime.clear();
    lastTrigTime = currentTrigTime = 0;
    countStartFlagConst = false;
    criticalTime = 0;
    criticalChange = false;
    maxCountRate = 0;
    maxResTime = 0;
    totalCnt = 0;
    totalCntClean = 0;
    totalCntFullTime = 0;
    critTimeActual = 0;
    constDataMsg.clear();
    pulseDataMsg.clear();
    backOut = 0;
    statErr = 0;
    tempErr1 = 0;
    tempErr2 = 0;
    countErr = 0;
}

void vecFill::msgFillUp() {

    statErr = 100*statErr;
    countErr = 100*countErr;

    if(!pulsesTime.empty())
    pulseDataMsg = QString::number(totalCntClean)+'\t'+QString::number(totalCnt)+'\t'+QString::number(minusBack)+
                    '\t'+QString::number(backOut)+ '\t'+QString::number(countErr,'f',2)+ '\t'+QString::number(minusBackTrue)+
                   '\t'+QString::number(pulsesTime.back())+'\t'+QString::number(countStartTimeReal);

    constDataMsg = QString::number(totalCntClean)+'\t'+QString::number(totalCnt)+'\t'+QString::number(minusBack)+
                    '\t'+QString::number(backOut)+ '\t'+QString::number(countErr,'f',2)+ '\t'+QString::number(minusBackTrue)+
                   '\t'+QString::number(derMaxTime)+'\t'+QString::number(derMinTime)+'\t'+QString::number(countStartTimeConst);
}
