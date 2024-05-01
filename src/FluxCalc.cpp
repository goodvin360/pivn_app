#include "FluxCalc.h"

FluxCalc::FluxCalc(QWidget *parent) {
    parent = nullptr;
//    wdg = new testWidget(parent);
}

FluxCalc::~FluxCalc() noexcept {}

void FluxCalc::printMessage(QString msg, int num) {
    emit sentMessage(msg, num);
}

void FluxCalc::calculateFlux(vecFill &data, double trig) {
    fluxTrig = trig;

    if (data.resultsDb.at(1).back()>fluxTrig)
    {
        fluxFlag = true;
    }

    if(deltaT == 0 && !fluxFlag)
    {
        for (int i = 2; i < data.resultsDb.size(); i++)
        {
            backVal+=data.resultsDb.at(i).back()/(1-data.resultsDb.at(i).back()*5e-4);
        }

        /*if (backVector.size()<10)
            backVector.push_back(backVal);
        else
        {
            backVector.erase(backVector.begin());
            backVector.push_back(backVal);
        }*/

        backVal = backVal/backCounter;
        backCounter+=1;

        /*if (backVector.size()>0)
        {
            double backSum=0;
            for (int i=0; i<backVector.size(); i++)
                backSum+=backVector.at(i);

            backVal = backSum/backVector.size();
            std::cout << backVal << std::endl;
        }*/

        return;
    }

    if (deltaT>=100)
    {
        deltaT = 0;
        fluxFlag  = false;
        fluxTime.clear();
        backVal = 0;
        backCounter = 1;
        backVector.clear();
        return;
    }

    fluxTime.push_back(data.resultsDb.at(0).back());
    deltaT = fluxTime.back()-fluxTime.front();

    double sum = 0;
    for (int i = fluxTime.front(); i <= fluxTime.back(); i++) {
        for (int j=2; j<data.resultsDb.size(); j++)
        {
            sum+=data.resultsDb.at(j).at(i)/(1-data.resultsDb.at(j).at(i)*5e-4)-backVal;
        }
    }
    double flux = 0;
//        double flux = (sum-991.22)/(coefficient*(1-exp(-(log(2)*deltaT/halfLife))));
    if (deltaT>0)
        flux = (sum-300)/(coefficient)*((1-exp(-(log(2)*100/halfLife)))/(1-exp(-(log(2)*deltaT/halfLife))));
//    printMessage(QString::number(flux));
    sum = 0;
    flux = 0;
//    std::cout << caller.widget->getVar() << std::endl;
}
