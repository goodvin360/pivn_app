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

std::vector<std::vector<double>> vecFill::getData(std::string &str, int &counter, std::vector<int> cntTrig, std::vector<double>resT, int counters) {
    cnt = counter;
    resTime = resT;
    for (int i=0; i < counters; i++)
    {
        if (resT[i]>maxResTime)
            maxResTime = resT[i];
    }
    if (counter==0)
    {
        for (int i=0; i<counters+2; i++)
        {
            std::vector<double> smData;
            resultsDb.push_back(smData);
        }

/*        for (int i=0; i<6; i++)
        {
            std::vector<double> smData;
            resultsDbPart.push_back(smData);
        }*/

        for (int i=0; i<4; i++)
        {
            std::vector<double> smData;
            resultsDbTotalP.push_back(smData);
        }

        for (int i=0; i<4; i++)
        {
            std::vector<double> smData;
            resultsDbTotalRoughP.push_back(smData);
        }

        for (int i=0; i<counters+1; i++)
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
//            if (m==2 && cnt1 == 0)
//                token = "0";
//            if (m==3 && cnt2 == 0)
//                token = "0";
//            if (m==4 && cnt3 == 0)
//                token = "0";
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
//        if (m==5 && cnt4 == 0)
//            var = "0";
        resultsDb.at(m).push_back(stod(var));
    }

    if (!resultsDb.at(0).empty())
    {
        resultsDbTotalP.at(0).push_back(resultsDb.at(0).back());
        resultsDbTotalRoughP.at(0).push_back(resultsDb.at(0).back());
        resultsDbSeparated.at(0).push_back(resultsDb.at(0).back());
        double sum = 0;
        double sum_clean = 0;
        double sumRough = 0;
        double sum_cleanRough = 0;
        for (int i = 2; i < resultsDb.size(); i++) {
            double var = 0;
            if (cntTrig.size()==resultsDb.size()-2)
            {
                if (cntTrig[i-2]==0)
                    resultsDb.at(i).back() = 0;
            }
//            if (cnt1 == 0)
//                resultsDb.at(2).back() = 0;
//            if (cnt2 == 0)
//                resultsDb.at(3).back() = 0;
//            if (cnt3 == 0)
//                resultsDb.at(4).back() = 0;
//            if (cnt4 == 0)
//                resultsDb.at(5).back() = 0;
            if (resultsDb.at(i).back() >= 1 / resT[i - 2])
                var = 1 / resT[i - 2] - 1;
            else var = resultsDb.at(i).back();
            if (i<6) {
                sum += var / (1 - var * resT[i - 2]);
                sum_clean += var;
            }
            if (i>5) {
                sumRough += var / (1 - var * resT[i - 2]);
                sum_cleanRough += var;
            }
            if ((i-1)<resultsDbSeparated.size())
                resultsDbSeparated.at(i-1).push_back(var);
        }
        resultsDbTotalP.at(1).push_back(sum);
        resultsDbTotalP.at(2).push_back(0);
            if (resultsDbTotalP.at(2).size()>2)
            {
                double last_val = resultsDbTotalP.at(2).at(resultsDbTotalP.at(2).size()-2);
                if (last_val>0) {
                    resultsDbTotalP.at(2).pop_back();
                    resultsDbTotalP.at(2).push_back(last_val);
                }
            }
        resultsDbTotalRoughP.at(1).push_back(sumRough);
        resultsDbTotalRoughP.at(2).push_back(0);
            if (resultsDbTotalRoughP.at(2).size()>2)
            {
                double last_val = resultsDbTotalRoughP.at(2).at(resultsDbTotalRoughP.at(2).size()-2);
                if (last_val>0) {
                    resultsDbTotalRoughP.at(2).pop_back();
                    resultsDbTotalRoughP.at(2).push_back(last_val);
                }
            }
        resultsDbTotalP.at(3).push_back(sum_clean);
        resultsDbTotalRoughP.at(3).push_back(sum_cleanRough);
    }
    return resultsDb;
}

void vecFill::getDataTotal(std::vector<std::vector<double>> &data, double totTime, double &flux, double&c_a, double&c_b,
                           bool& offTrigger, int trMode, int &trVal, double &ePoint, int constFluxTr, double &tPoint, double &tPointShift, int &constTrig,
                           double backDelayTime, int window, double &lftTime, int mPulses, int clearTrig,
                           double critLvl, double intTime, int nucleus, int roughTrigger, int& skipTrig) {
    int var3 = 0;
    int var4 = 0;
    double var5 = 0;

    if (roughTrigger == 1) {
        resultsDbTotal = resultsDbTotalRoughP;
        for (int i = 6; i < 8; i++) {
            if (resultsDb.at(i).back()>maxCountRate)
                maxCountRate = resultsDb.at(i).back();
        }
        var3 = 5;   // the first counter in array number
        var4 = 7;   // the last counter in array number
        var5 = 2;   // amount of counters in array
    }
    if (roughTrigger == 0) {
        resultsDbTotal = resultsDbTotalP;
        for (int i = 2; i < 6; i++) {
            if (resultsDb.at(i).back()>maxCountRate)
                maxCountRate = resultsDb.at(i).back();
        }
        var3 = 1;
        var4 = 5;
        var5 = 4;
    }

    fluxTime = totTime+countStartTimeReal;
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

        if (fluxTrig>temp)
            fluxTrig = temp;

        if (mPulses==0) {
            if (temp > fluxTrig) {
                if (isBack)
                    pulsesTime.push_back(resultsDbTotal.at(0).back());
                isBack = false;
                fluxTrig = temp;
            }

            if (isBack) {
                if (backVec.size() > window && !backVec.empty()) {
                    for (int i = 0; i < backVec.size() - window; i++)
                        backVec.erase(backVec.begin());
                }

                backVec.push_back(resultsDbTotal.at(1).back());
                backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));
                resultsDbTotal.at(2).pop_back();
                resultsDbTotal.at(2).push_back(backVal);
                backLastVal = backVal;
            }

            if (!isBack) {

                if (maxCountRate <= nCritical && !criticalChange && !isCritical) {
                    if (!isPulse) {
                        countStartTimeReal = resultsDbTotal.at(0).back();
                        prePulsesData.push_back(resultsDbTotal.at(1).back() - backLastVal);
                        isPulse = true;
                    }

/*                if (!prePulsesData.empty())
                {
                    if (nucleus==1)
                    {
                        backVal = backLastVal+  (In116m1Val*exp(-lmd116m*fluxTimeCounter) + In114Val*exp(-lmd114*fluxTimeCounter)) *
                                                (prePulsesData.back())/(1 + In116m1Val*exp(-lmd116m*fluxTimeCounter) +
                                                                        In114Val*exp(-lmd114*fluxTimeCounter));
                    }
                    else
                    {
                        backVal = backLastVal+  (Ag108Val*exp(-lmdAg108*fluxTimeCounter)) *
                                                (prePulsesData.back())/(1 + Ag108Val*exp(-lmdAg108*fluxTimeCounter));
                    }

                    if (backVec.size()>window && !backVec.empty())
                    {
                        for (int i=0; i<backVec.size()-window;i++)
                            backVec.erase(backVec.begin());
                    }
                    backVec.push_back(backVal);
                    resultsDbTotal.at(2).pop_back();
                    resultsDbTotal.at(2).push_back(backVal);
                }*/

                    resultsDbTotal.at(2).pop_back();
                    resultsDbTotal.at(2).push_back(backLastVal);


                    lftTime = totTime + backDelayTime + window - fluxTimeCounter;

                    if (fluxTimeCounter > totTime + backDelayTime) {
                        if (backVec.size() > window && !backVec.empty()) {
                            for (int i = 0; i < backVec.size() - window; i++)
                                backVec.erase(backVec.begin());
                        }

                        backVec.push_back(resultsDbTotal.at(1).back());
                        backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));

                        resultsDbTotal.at(2).pop_back();
                        resultsDbTotal.at(2).push_back(backVal);

                        for (int i = 0; i < fluxTimeCounter; i++) {
                            if (nucleus == 1)
                                resultsDbTotal.at(2).at(int(countStartTimeReal) + i) = backVal;
//                            resultsDbTotal.at(2).at(int(countStartTimeReal)+i)=backLastVal+0.8*(backVal-backLastVal)*exp(lmd116m*(fluxTimeCounter-i))+0.1125*(backVal-backLastVal)*exp(lmd114*(fluxTimeCounter-i));
                            else
                                resultsDbTotal.at(2).at(int(countStartTimeReal) + i) = backLastVal +
                                                                                       0.8 * (backVal - backLastVal) *
                                                                                       exp(lmdAg108 *
                                                                                           (fluxTimeCounter - i));
                        }
                        ///////////
                        calcFlux(nucleus, intTime, c_a, c_b, totTime, flux, lftTime, backDelayTime, var3, var4, var5);
                        //////////
                    }

/*                ///////////
                if (fluxTimeCounter<=totTime)
                    calcFlux(nucleus, intTime, c_a, c_b, totTime, flux, lftTime, backDelayTime, var3, var4, var5);
                //////////*/

                    if (fluxTimeCounter == totTime + backDelayTime + window) {
                        isBack = true;
                        isPulse = false;
                        fluxTimeCounter = 0;
                        //////////
                        pulseFinish(roughTrigger, lftTime, totTime, clearTrig, backDelayTime);
                        /////////
                    }
                }

                if (maxCountRate > nCritical && !criticalChange) {
                    isCritical = true;
                    totalCnt = 0;
                    totalCntClean = 0;
                    resultsDbTotal.at(2).pop_back();
                    resultsDbTotal.at(2).push_back(backLastVal);
                    lftTime = totTime + backDelayTime + window;
                    if (fluxTimeCounter > 0)
                        fluxTimeCounter = 0;
                }

                if (maxCountRate <= nCritical && isCritical) {
                    criticalChange = true;
                    countStartTimeReal = resultsDbTotal.at(0).back();
                    isCritical = false;
                }

                if (criticalChange) {
                    resultsDbTotal.at(2).pop_back();
                    resultsDbTotal.at(2).push_back(backLastVal);

                    lftTime = totTime + backDelayTime + window - fluxTimeCounter;

                    if (fluxTimeCounter > totTime + backDelayTime) {
                        if (backVec.size() > window && !backVec.empty()) {
                            for (int i = 0; i < backVec.size() - window; i++)
                                backVec.erase(backVec.begin());
                        }

                        backVec.push_back(resultsDbTotal.at(1).back());
                        backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));

                        resultsDbTotal.at(2).pop_back();
                        resultsDbTotal.at(2).push_back(backVal);

                        for (int i = 0; i < fluxTimeCounter; i++) {
                            if (nucleus == 1)
                                resultsDbTotal.at(2).at(int(countStartTimeReal) + i) = backLastVal +
                                                                                       0.8 * (backVal - backLastVal) *
                                                                                       exp(lmd116m *
                                                                                           (fluxTimeCounter - i)) +
                                                                                       0.1125 *
                                                                                       (backVal - backLastVal) *
                                                                                       exp(lmd114 *
                                                                                           (fluxTimeCounter - i));
                            else
                                resultsDbTotal.at(2).at(int(countStartTimeReal) + i) = backLastVal +
                                                                                       0.8 * (backVal - backLastVal) *
                                                                                       exp(lmdAg108 *
                                                                                           (fluxTimeCounter - i));
                        }
                        ///////////
                        calcFlux(nucleus, intTime, c_a, c_b, totTime, flux, lftTime, backDelayTime, var3, var4, var5);
                        //////////
                    }

                    if (fluxTimeCounter == totTime + backDelayTime + window) {
                        isBack = true;
                        isCritical = false;
                        criticalChange = false;
                        fluxTimeCounter = 0;
                        //////////
                        pulseFinish(roughTrigger, lftTime, totTime, clearTrig, backDelayTime);
                        /////////
                    }
                }

                fluxTimeCounter++;
            }
        }

        if (mPulses>0)
        {
            if (temp > fluxTrig) {
                pulsesTime.push_back(resultsDbTotal.at(0).back());
                isBack = false;
                fluxTrig = temp;
                if (isPulse)
                    isSecondPulse = true;
            }

            if (isSecondPulse)
            {
                fluxTimeCounter = 0;
                isCritical = false;
                criticalChange = false;
                isPulse = false;
                backLastVal = resultsDbTotal.at(1).at(resultsDbTotal.at(1).size()-2)-backVal;
                pulseFinish(roughTrigger, lftTime, totTime, clearTrig, backDelayTime);
                isSecondPulse = false;
            }

            if (isBack) {
                if (backVec.size() > window && !backVec.empty()) {
                    for (int i = 0; i < backVec.size() - window; i++)
                        backVec.erase(backVec.begin());
                }

                backVec.push_back(resultsDbTotal.at(1).back());
                backVal = std::accumulate(backVec.begin(), backVec.end(), 0) / (double(backVec.size()));
                resultsDbTotal.at(2).pop_back();
                resultsDbTotal.at(2).push_back(backVal);
                backLastVal = backVal;
            }

            if (!isBack) {

                if (maxCountRate <= nCritical && !criticalChange && !isCritical) {
                    if (!isPulse) {
                        countStartTimeReal = resultsDbTotal.at(0).back();
                        prePulsesData.push_back(resultsDbTotal.at(1).back() - backLastVal);
                        isPulse = true;
                    }

                    for (int i = 0; i < fluxTimeCounter; i++) {
                        double tempVar = backLastVal*exp(-lmd116*i)+(In116m1Val*exp(-lmd116m*i) + In114Val*exp(-lmd114*i)) *
                                                                    (prePulsesData.back())/(1 + In116m1Val*exp(-lmd116m*i) +
                                                                                            In114Val*exp(-lmd114*i));
                        if (nucleus == 1)
                        {
                            if (tempVar>=backVal)
                                resultsDbTotal.at(2).at(int(countStartTimeReal)+i) = tempVar;
                            else
                                resultsDbTotal.at(2).at(int(countStartTimeReal)+i) = backVal;
                        }
                        else
                            resultsDbTotal.at(2).at(int(countStartTimeReal)+i) = backLastVal*exp(-lmdAg110*i);
                    }

                    lftTime = totTime - fluxTimeCounter;

                    ///////////
                    if (fluxTimeCounter<=totTime)
                        calcFlux(nucleus, intTime, c_a, c_b, totTime, flux, lftTime, backDelayTime, var3, var4, var5);
                    //////////

                    if (fluxTimeCounter == totTime) {
                        isBack = true;
                        isPulse = false;
                        fluxTimeCounter = 0;
                        //////////
                        pulseFinish(roughTrigger, lftTime, totTime, clearTrig, backDelayTime);
                        /////////
                    }
                }

                if (maxCountRate > nCritical && !criticalChange) {
                    isCritical = true;
                    totalCnt = 0;
                    totalCntClean = 0;
                    resultsDbTotal.at(2).pop_back();
                    resultsDbTotal.at(2).push_back(backLastVal);
                    lftTime = totTime;
                    if (fluxTimeCounter > 0)
                        fluxTimeCounter = 0;
                }

                if (maxCountRate <= nCritical && isCritical) {
                    criticalChange = true;
                    countStartTimeReal = resultsDbTotal.at(0).back();
                    isCritical = false;
                    prePulsesData.push_back(resultsDbTotal.at(1).back() - backLastVal);
                }

                if (criticalChange) {

                    for (int i = 0; i < fluxTimeCounter; i++) {
                        double tempVar = backLastVal*exp(-lmd116*i)+(In116m1Val*exp(-lmd116m*i) + In114Val*exp(-lmd114*i)) *
                                                                                                      (prePulsesData.back())/(1 + In116m1Val*exp(-lmd116m*i) +
                                                                                                                              In114Val*exp(-lmd114*i));
                        if (nucleus == 1)
                        {
                            if (tempVar>=backLastVal)
                                resultsDbTotal.at(2).at(int(countStartTimeReal)+i) = tempVar;
                            else
                                resultsDbTotal.at(2).at(int(countStartTimeReal)+i) = backLastVal;
                        }
                        else
                            resultsDbTotal.at(2).at(int(countStartTimeReal)+i) = backLastVal*exp(-lmdAg110*i);;
                    }

                    lftTime = totTime - fluxTimeCounter;

                    ///////////
                    if (fluxTimeCounter<=totTime)
                        calcFlux(nucleus, intTime, c_a, c_b, totTime, flux, lftTime, backDelayTime, var3, var4, var5);
                    //////////

                    if (fluxTimeCounter == totTime) {
                        isBack = true;
                        isCritical = false;
                        criticalChange = false;
                        fluxTimeCounter = 0;
                        //////////
                        pulseFinish(roughTrigger, lftTime, totTime, clearTrig, backDelayTime);
                        /////////
                    }
                }

                fluxTimeCounter++;
            }
        }

        if (skipTrig>0)
        {
            isBack = true;
            isPulse = false;
            isCritical = false;
            criticalChange = false;
            if (!prePulsesData.empty())
                prePulsesData.pop_back();
            if(!pulsesTime.empty())
                pulsesTime.pop_back();
            fluxTimeCounter = 0;
            lftTime = totTime+backDelayTime;
            flux = 0;
            totalCnt = 0;
            totalCntClean = 0;
            minusBack = 0;
            skipTrig = 0;
        }


        ///////old algo
        /*currentVecSize=data.at(1).size();
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
//            for (auto it=backVec.begin(); it!=backVec.end(); it++)
//            {
//                backVal+=it.operator*();
//            }
//            backVal = backVal/double(backVec.size());
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

                        if (nucleus==1)
                            backFromPulses += prePulsesData.at(i)*exp(-lmd116*t);
                        else
                            backFromPulses += prePulsesData.at(i)*exp(-lmdAg110*t);
                    }
                    if (nucleus==1)
                    {
                        backFromPulses+=(In116m1Val*exp(-lmd116m*fluxTimeCounter) + In114Val*exp(-lmd114*fluxTimeCounter)) *
                        (prePulsesData.back())/(1 + In116m1Val*exp(-lmd116m*fluxTimeCounter) +
                             In114Val*exp(-lmd114*fluxTimeCounter));
                    }
                    else
                    {
                        backFromPulses+=(0.028*exp(-lmdAg108*fluxTimeCounter)) *
                                        (prePulsesData.back())/(1 + 0.028*exp(-lmdAg108*fluxTimeCounter));
                    }
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
            } else if (pulseCounter==1){
                if (nucleus==1)
                {
                    backVal = backLastVal+  (In116m1Val*exp(-lmd116m*fluxTimeCounter) + In114Val*exp(-lmd114*fluxTimeCounter)) *
                    (prePulsesData.back())/(1 + In116m1Val*exp(-lmd116m*fluxTimeCounter) +
                                            In114Val*exp(-lmd114*fluxTimeCounter));
                }
                else
                {
                    backVal = backLastVal+  (0.028*exp(-lmdAg108*fluxTimeCounter)) *
                                            (prePulsesData.back())/(1 + 0.028*exp(-lmdAg108*fluxTimeCounter));
                }
            }


            resultsDbTotal.at(2).pop_back();
            resultsDbTotal.at(2).push_back(backVal);

            if (countStartFlag) {
                totalCnt += resultsDbTotal.at(1).back();
                totalCntClean += resultsDbTotal.at(3).back();

                minusBack += (resultsDbTotal.at(1).back() - resultsDbTotal.at(2).back());
                backOut = backVal*backVec.size();
                if (nucleus==1)
                {
                    minusBackTrue = minusBack*((1 - exp(-(lmd116*intTime))) /
                                           (exp(-(lmd116*countStartTime)) - exp(-(lmd116*fluxTimeCounter))));

                    totalCntFullTime = totalCnt*((1 - exp(-(lmd116*intTime))) /
                                             (exp(-(lmd116*countStartTime)) - exp(-(lmd116*fluxTimeCounter))));
                }
                else
                {
                    minusBackTrue = minusBack*((1 - exp(-(lmdAg110*intTime))) /
                                               (exp(-(lmdAg110*countStartTime)) - exp(-(lmdAg110*fluxTimeCounter))));

                    totalCntFullTime = totalCnt*((1 - exp(-(lmdAg110*intTime))) /
                                                 (exp(-(lmdAg110*countStartTime)) - exp(-(lmdAg110*fluxTimeCounter))));
                }


                Flux = minusBackTrue * c_a + c_b;
                flux = Flux;
                statErr = sqrt(totalCnt+backOut*pow(totTime/backVec.size(),2))/minusBack;
                if (statErr<0)
                    statErr = 0;
                lftTime = fluxTime-fluxTimeCounter;

                double var1 = 0;
                double var2 = 1;

                for (int i=var3; i<var4; i++)
                {
                    var1=resultsDbSeparated.at(i).back()+pow(deltaResTime*resTime[i-1],2)*pow(resultsDbSeparated.at(i).back(),4);
                    var2=pow((resTime[i-1]*resultsDbSeparated.at(i).back()-1),2);
                    tempErr1+=var1/var2;
                }

                tempErr2 = 0;
                for (int i=var3; i<var4; i++)
                {
                    var1 = backLastVal/var5+pow(deltaResTime*resTime[i-1],2)*pow(backVal/var5,4);
                    var2 = pow((resTime[i-1]*backVal/var5-1),2);
                    tempErr2+=var1/var2;
                }
                tempErr2=backVec.size()*tempErr2;

                countErr = sqrt(tempErr1+tempErr2*pow(totTime/backVec.size(),2))/minusBack;
                if (countErr<0)
                    countErr = 0;

            }

            fluxTimeCounter++;


            if (fluxTimeCounter == fluxTime || isSecondPulse) {
            QString s(0x00B1);
            double error = statErr*Flux;
            msgFillUp();
            if (roughTrigger==0) {
                printMessage(pulseDataMsg, 2);
                printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
            }
            else {
                printMessage(pulseDataMsg, 22);
                printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),11);
            }
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
                if (nucleus==1)
                    resultsDbTotal.at(2).at(tPoint+i)=0.9*backVal*exp(lmd116m*(fluxTimeCounter-i))+0.0125*backVal*exp(lmd114*(fluxTimeCounter-i));
                else
                    resultsDbTotal.at(2).at(tPoint+i)=0.9*backVal*exp(lmdAg108*(fluxTimeCounter-i));
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

            if (nucleus==1) {
                totalCntFullTime = minusBack * ((1 - exp(-(lmd116 * intTime)))/
                                                (exp(-(lmd116 * criticalTime)) - exp(-(lmd116 * fluxTimeCounter))));
                minusBackTrue = minusBack * ((1 - exp(-(lmd116 * intTime)))/
                                             (exp(-(lmd116 * criticalTime)) - exp(-(lmd116 * fluxTimeCounter))));
            }
            else{
                totalCntFullTime = minusBack*((1 - exp(-(lmdAg110*intTime)))/
                                          (exp(-(lmdAg110*criticalTime)) - exp(-(lmdAg110*fluxTimeCounter))));
                minusBackTrue = minusBack*((1 - exp(-(lmdAg110*intTime))) /
                                       (exp(-(lmdAg110*criticalTime)) - exp(-(lmdAg110*fluxTimeCounter))));
            }

            Flux = minusBackTrue *c_a + c_b;
            flux = Flux;
            statErr = sqrt(totalCnt+backOut*pow(fluxTime/backVecConst.size(),2))/minusBack;
            if (statErr<0)
                statErr = 0;

            double var1 = 0;
            double var2 = 1;

            int timePoint = resultsDbSeparated.at(0).size()-fluxTimeCounter;
            for (int i=var3; i<var4; i++) {
                for (int j=0; j<fluxTime; j++) {
                    var1 = resultsDbSeparated.at(i).at(j+timePoint) + pow(deltaResTime * resTime[i - 1], 2) * pow(resultsDbSeparated.at(i).at(j+timePoint), 4);
                    var2 = pow((resTime[i - 1] * resultsDbSeparated.at(i).at(j+timePoint) - 1), 2);
                    tempErr1 += var1 / var2;
                }
            }

            tempErr2 = 0;
            for (int i=var3; i<var4; i++) {
                var1 = backVal/var5+pow(deltaResTime*resTime[i-1],2)*pow(backVal/var5,4);
                var2 = pow((resTime[i-1]*backVal/var5-1),2);
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
            else
                tail = derivativeAllVec.size()-400;
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
            msgFillUp();
            if (roughTrigger==0) {
                printMessage(constDataMsg, 3);
                printMessage(QString::number(Flux, 'g', 3) + s + QString::number(error, 'g', 3), 1);
            }
            else {
                printMessage(constDataMsg, 33);
                printMessage(QString::number(Flux, 'g', 3) + s + QString::number(error, 'g', 3), 11);
            }
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
            lftTime = fluxTime+backDelayTime+window;
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

    if (roughTrigger == 1)
        resultsDbTotalRoughP = resultsDbTotal;
    if (roughTrigger == 0)
        resultsDbTotalP = resultsDbTotal;
}

void vecFill::cleanUp() {
    resultsDb.clear();
    resultsDbTotalP.clear();
    resultsDbTotalRoughP.clear();
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

void vecFill::calcFlux(int nucleus, double intTime, double c_a, double c_b, double totalTime, double &flux, double &leftTime,
                       double &backDelayTime, int var3, int var4, double var5) {

    minusBack = 0;
    totalCnt = 0;
    totalCntClean = 0;
    double fluxTimeVar = fluxTimeCounter;
/*    if (fluxTimeCounter<=totalTime+backDelayTime)
        fluxTimeVar = fluxTimeCounter;
    else
        fluxTimeVar = fluxTimeCounter-totalTime-backDelayTime;*/

    if (fluxTimeVar>totalTime)
        fluxTimeVar=totalTime;

    for (int i=int(countStartTimeReal); i<countStartTimeReal+fluxTimeVar; i++)
    {
        minusBack += (resultsDbTotal.at(1).at(i) - resultsDbTotal.at(2).at(i));
        totalCnt+=resultsDbTotal.at(1).at(i);
        totalCntClean+=resultsDbTotal.at(3).at(i);
    }

    backOut = backVal*backVec.size();
    if (nucleus==1)
    {
        minusBackTrue = minusBack*((1 - exp(-(lmd116*intTime))) /
                                   (exp(-(lmd116*(countStartTimeReal-pulsesTime.back()))) - exp(-(lmd116*(fluxTimeVar+(countStartTimeReal-pulsesTime.back()))))));

        totalCntFullTime = totalCnt*((1 - exp(-(lmd116*intTime))) /
                                     (exp(-(lmd116*(countStartTimeReal-pulsesTime.back()))) - exp(-(lmd116*(fluxTimeVar+(countStartTimeReal-pulsesTime.back()))))));
    }
    else
    {
        minusBackTrue = minusBack*((1 - exp(-(lmdAg110*intTime))) /
                                   (exp(-(lmdAg110*(countStartTimeReal-pulsesTime.back()))) - exp(-(lmdAg110*(fluxTimeVar+(countStartTimeReal-pulsesTime.back()))))));

        totalCntFullTime = totalCnt*((1 - exp(-(lmdAg110*intTime))) /
                                     (exp(-(lmdAg110*(countStartTimeReal-pulsesTime.back()))) - exp(-(lmdAg110*(fluxTimeVar+(countStartTimeReal-pulsesTime.back()))))));
    }


    Flux = minusBackTrue * c_a + c_b;
    flux = Flux;
    statErr = sqrt(totalCnt+backOut*pow(totalTime/double(backVec.size()),2))/minusBack;
    if (statErr<0)
        statErr = 0;

    double var1 = 0;
    double var2 = 1;

    for (int i=var3; i<var4; i++)
    {
        var1=resultsDbSeparated.at(i).back()+pow(deltaResTime*resTime[i-1],2)*pow(resultsDbSeparated.at(i).back(),4);
        var2=pow((resTime[i-1]*resultsDbSeparated.at(i).back()-1),2);
        tempErr1+=var1/var2;
    }

    tempErr2 = 0;
    for (int i=var3; i<var4; i++)
    {
        var1 = backLastVal/var5+pow(deltaResTime*resTime[i-1],2)*pow(backVal/var5,4);
        var2 = pow((resTime[i-1]*backVal/var5-1),2);
        tempErr2+=var1/var2;
    }
    tempErr2=backVec.size()*tempErr2;

    countErr = sqrt(tempErr1+tempErr2*pow(totalTime/backVec.size(),2))/minusBack;
    if (countErr<0)
        countErr = 0;

}

void vecFill::pulseFinish(int roughTrigger, double& leftTime, double& totTime, int clearTrig, double backDelTime) {

        QString s(0x00B1);
        double error = statErr*Flux;
        msgFillUp();
        if (roughTrigger==0) {
            printMessage(pulseDataMsg, 2);
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),1);
        }
        else {
            printMessage(pulseDataMsg, 22);
            printMessage(QString::number(Flux,'g',3)+s+QString::number(error,'g',3),11);
        }

        minusBack = 0;
        minusBackTrue = 0;
        totalCnt = 0;
        totalCntClean = 0;
        totalCntFullTime = 0;
        Flux = 0;
        leftTime = totTime+backDelTime;
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
        if (clearTrig>0)
            backVec.clear();
}
