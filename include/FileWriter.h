#ifndef PIVN_APP_VER_1_FILEWRITER_H
#define PIVN_APP_VER_1_FILEWRITER_H

#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include "map"
#include "vector"

class FileWriter {
public:
    FileWriter();

    ~FileWriter();

    void fileWriteVec(std::vector<std::vector<double>> &res, std::string marker);
};

#endif //PIVN_APP_VER_1_FILEWRITER_H
