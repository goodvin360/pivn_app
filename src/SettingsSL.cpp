#include "SettingsSL.h"

SettingsSL::SettingsSL() {}

SettingsSL::~SettingsSL() {}

void SettingsSL::saveSettings() {
    std::cout << "save settings button pushed" << std::endl;

    settingsDataSave.clear();
    settingsDataSave.insert(std::pair<std::string, double> ("cnt_1", cnt1));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt_2", cnt2));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt_3", cnt3));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt_4", cnt4));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt_5", cnt5));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt_6", cnt6));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt1_res", cnt1_res));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt2_res", cnt2_res));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt3_res", cnt3_res));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt4_res", cnt4_res));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt5_res", cnt5_res));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt6_res", cnt6_res));
    settingsDataSave.insert(std::pair<std::string, double> ("cnt_num", cnt_num));
    settingsDataSave.insert(std::pair<std::string, double> ("coef_a", coef_a));
    settingsDataSave.insert(std::pair<std::string, double> ("coef_b", coef_b));
    settingsDataSave.insert(std::pair<std::string, double> ("coef_a_r", coef_a_r));
    settingsDataSave.insert(std::pair<std::string, double> ("coef_b_r", coef_b_r));
    settingsDataSave.insert(std::pair<std::string, double> ("14_MeV", MeV_14));
    settingsDataSave.insert(std::pair<std::string, double> ("2.5_MeV", MeV_2_5));
    settingsDataSave.insert(std::pair<std::string, double> ("averageTime", averageTime));
    settingsDataSave.insert(std::pair<std::string, double> ("backDelay", backDelay));
    settingsDataSave.insert(std::pair<std::string, double> ("clearBack", clearBack));
    settingsDataSave.insert(std::pair<std::string, double> ("critLvl", critVal));
    settingsDataSave.insert(std::pair<std::string, double> ("distance", distance));
    settingsDataSave.insert(std::pair<std::string, double> ("intTime", intTime));
    settingsDataSave.insert(std::pair<std::string, double> ("measTime", measTime));
    settingsDataSave.insert(std::pair<std::string, double> ("multiPulse", multiPulse));
    settingsDataSave.insert(std::pair<std::string, double> ("nucleusTrig", nucleusTrig));
    settingsDataSave.insert(std::pair<std::string, double> ("userCoef", userCoef));

    if (!settingsDataSave.empty())
    {
        for (auto it=settingsDataSave.begin(); it!=settingsDataSave.end(); it++)
            std::cout << it->first << '\t' << it->second << std::endl;
    }
    else
        std::cout << "settings data is empty" << std::endl;

    std::stringstream dirName;
    dirName << "./settings";
    QDir dir(QString::fromStdString(dirName.str()));
    if (!dir.exists())
        dir.mkpath(".");

    for (int i=0; i<2; i++) {
        if (i==0)
            settingFileNameSave = "./settings/settings.txt";
        if (i==1)
            settingFileNameSave = "../../settings/settings.txt";
        if (!settingFileNameSave.empty()) {
            std::ofstream myFile(settingFileNameSave, std::ios::out);
            for (auto it = settingsDataSave.begin(); it != settingsDataSave.end(); it++) {
                myFile << it->first << '\t' << it->second << std::endl;
            }
        }
    }

}

void SettingsSL::loadSettings() {
    std::cout << "load settings button pushed " << settingsFileNameLoad << std::endl;

    settingsData.clear();

    int FileSize=0;
    std::ifstream file;
    file.open(settingsFileNameLoad);
    std::string line;
    while (getline(file, line))
    {
        FileSize++;
    }
    file.close();

    std::string keyName;
    double keyVal;

    file.open(settingsFileNameLoad);
    for (int i = 0; i < FileSize; i++)
    {
        file >> keyName >> keyVal;
        settingsData.insert(std::pair<std::string, double> (keyName, keyVal));
    }
    file.close();

    cnt1 = int(settingsData.find("cnt_1")->second);
    cnt2 = int(settingsData.find("cnt_2")->second);
    cnt3 = int(settingsData.find("cnt_3")->second);
    cnt4 = int(settingsData.find("cnt_4")->second);
    cnt5 = int(settingsData.find("cnt_5")->second);
    cnt6 = int(settingsData.find("cnt_6")->second);
    cnt1_res = settingsData.find("cnt1_res")->second;
    cnt2_res = settingsData.find("cnt2_res")->second;
    cnt3_res = settingsData.find("cnt3_res")->second;
    cnt4_res = settingsData.find("cnt4_res")->second;
    cnt5_res = settingsData.find("cnt5_res")->second;
    cnt6_res = settingsData.find("cnt6_res")->second;
    cnt_num = int(settingsData.find("cnt_num")->second);
    coef_a = settingsData.find("coef_a")->second;
    coef_b = settingsData.find("coef_b")->second;
    coef_a_r = settingsData.find("coef_a_r")->second;
    coef_b_r = settingsData.find("coef_b_r")->second;
    MeV_14 = int(settingsData.find("14_MeV")->second);
    MeV_2_5 = int(settingsData.find("2.5_MeV")->second);
    averageTime = settingsData.find("averageTime")->second;
    backDelay = settingsData.find("backDelay")->second;
    clearBack = int(settingsData.find("clearBack")->second);
    critVal = settingsData.find("critLvl")->second;
    distance = settingsData.find("distance")->second;
    intTime = settingsData.find("intTime")->second;
    measTime = settingsData.find("measTime")->second;
    multiPulse = int(settingsData.find("multiPulse")->second);
    nucleusTrig = int(settingsData.find("nucleusTrig")->second);
    userCoef = int(settingsData.find("userCoef")->second);
}

void SettingsSL::getFileName() {

}

/*
cnt_1 1 "0" - off, "1" - on
cnt_2 1 "0" - off, "1" - on
cnt_3 1 "0" - off, "1" - on
cnt_4 1 "0" - off, "1" - on
cnt_5 0 if cnt_num==1 then "0" - off, "1" - on
cnt_6 0 if cnt_num==1 then "0" - off, "1" - on
cnt_num 0   //"0" - only sensitive channel (counters 1-4), "1" - sensitive and rough channels (counters 1-4 and 5-6)
cnt1_res 200
cnt2_res 300
cnt3_res 400
cnt4_res 350
cnt5_res 250
cnt6_res 250
intTime 100
critLvl 50
backDelay 70
measTime	30
averageTime	50
clearBack 0
multiPulse 0
nucleusTrig 1 //"0" - Ag, "1" - In
userCoef 1
2.5_MeV 1
14_MeV 0
coef_a 2840
coef_b 13354
coef_a_r 2840
coef_b_r 13354
distance 24
 */