#include "FileWriter.h"
#include <iomanip>
#include <fstream>

FileWriter::FileWriter() {}

FileWriter::~FileWriter() {}

void FileWriter::fileWriteVec(std::vector<std::vector<double>> &res, std::string marker) {
    auto tme = std::time(nullptr);
    auto tm = *std::localtime(&tme);

    if (res.size() > 0) {

        std::stringstream res_out;
        res_out << "../outDataVec " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << " " << marker << ".txt";
        std::ofstream myFile(res_out.str(), std::ios::out);
        for (int i = 0; i < res.at(0).size(); i++) {
            for (int j = 0; j < res.size(); j++) {
                if (j<5)
                    myFile << res.at(j).at(i) << '\t';
                if (j==5)
                    myFile << res.at(j).at(i);
            }
            myFile << std::endl;
        }
    };
}