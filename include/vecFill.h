#ifndef PIVN_APP_VER_2_VECFILL_H
#define PIVN_APP_VER_2_VECFILL_H

#include "vector"
#include "string"

class vecFill {

public:
    vecFill();
    ~vecFill();

    std::vector<std::vector<double>> resultsDb;

    std::vector<std::vector<double>> getData(std::string str, int counter);

};

#endif //PIVN_APP_VER_2_VECFILL_H
