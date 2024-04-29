#ifndef PIVN_APP_VER_2_VECFILL_H
#define PIVN_APP_VER_2_VECFILL_H

#include "vector"
#include "string"

class vecFill {

public:
    vecFill();
    ~vecFill();

    std::vector<std::vector<double>> resultsDb;

    std::vector<std::vector<double>> resultsDbTotal;

    std::vector<std::vector<double>> getData(std::string str, int counter);
    std::vector<std::vector<double>> getDataTotal(std::vector<std::vector<double>> data);

    double backVal = 0;
    std::vector<double> backVec;
    double fluxTrig = 4;
    double temp = 0;
    double fluxTime = 100;
    double fluxTimeCounter = 0;
    bool isBack = true;
    double peakVal = 0;
    double backLastVal = 0;
    double cnt = 0;

};

#endif //PIVN_APP_VER_2_VECFILL_H
