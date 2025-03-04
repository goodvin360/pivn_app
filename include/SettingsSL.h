#ifndef PIVN_APP_SETTINGSSL_H
#define PIVN_APP_SETTINGSSL_H

#include "iostream"
#include "sstream"
#include "map"
#include "vector"
#include "fstream"
#include "iostream"

class SettingsSL
{

public:
    SettingsSL();
    ~SettingsSL();

    void saveSettings();
    void loadSettings();
    void getFileName();

    std::map<std::string, double> settingsData;
    std::map<std::string, double> settingsDataSave;
    std::vector<double>resTime;
    std::string settingsFileNameLoad;
    std::string settingFileNameSave;
    int cnt1, cnt2, cnt3, cnt4, cnt5, cnt6 = 0;
    int cnt_num = 0;
    double cnt1_res, cnt2_res, cnt3_res, cnt4_res, cnt5_res, cnt6_res = 0;
    int MeV_14, MeV_2_5 = 0;
    double coef_a, coef_b, coef_a_r, coef_b_r = 0;
    int userCoef = 0;
    int nucleusTrig = 0;
    int multiPulse = 0;
    double intTime = 0;
    double distance = 0;
    double critVal = 0;
    double backDelay = 0;
    int clearBack = 0;
    double measTime = 30;
    double averageTime = 50;
};

#endif //PIVN_APP_SETTINGSSL_H
