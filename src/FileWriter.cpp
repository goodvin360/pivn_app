#include "FileWriter.h"
#include <iomanip>
#include <fstream>

FileWriter::FileWriter() {}

FileWriter::~FileWriter() {}

void FileWriter::fileWrite(std::map<std::string, int> &res) {
    auto tme = std::time(nullptr);
    auto tm = *std::localtime(&tme);

    if (res.size() > 0) {
        auto outData = new double *[res.size()]();
        for (int i = 0; i < res.size(); i++) {
            outData[i] = new double[6]();
        }

        std::cout << "after mapping " << std::endl;
        int m = 0;
        int t = 0;
        std::string var;
        std::string delimiter2 = ":";
        size_t pos = 0;
        std::string token;
        double startTime;
        bool flag = true;
        for (auto it = res.begin(); it != res.end(); it++) {
            std::cout << it->first << '\t' << it->second << std::endl;
            var = it->first;
            var += '\n';
            while ((pos = var.find(delimiter2)) != std::string::npos) {
                token = var.substr(0, pos);
                for (int k = 0; k < token.size(); k++) {
                    if (token[k] < '0' || token[k] > '9') {
                        flag = false;
                    }
                }
                if (m == 0 && flag)
                    outData[t][m] = 0.001 * stod(token);
                else
                    outData[t][m] = stod(token);

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
                outData[t][m] = stod(var);
            m = 0;
            t += 1;
            flag = true;
        }

        startTime = outData[0][0];

        std::stringstream res_out;
        res_out << "../outData " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ".txt";
        std::ofstream myFile(res_out.str(), std::ios::out);
        for (int i = 0; i < res.size(); i++) {
            for (int j = 0; j < 6; j++) {
                if (j == 0 && outData[i][j] >= startTime)
                    myFile << outData[i][j] - startTime << '\t';
                else if (j > 0 && j < 5)
                    myFile << outData[i][j] << '\t';
                else if (j == 5)
                    myFile << outData[i][j];
                else if (j == 0 && i > 0 && outData[i][j] < outData[i - 1][j])
                    break;
            }
            myFile << std::endl;
        }

        for (unsigned i{}; i < res.size(); i++) {
            delete[] outData[i];
        };
    };
}

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