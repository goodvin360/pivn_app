#include <qtestsupport_core.h>
#include "Counters.h"

Counters::Counters(QWidget *parent) : QWidget(parent) {
    parent = nullptr;
}

Counters::~Counters() {}

void Counters::cnt1(int val) {
    cnt1_trig = val;
    std::cout << cnt1_trig << std::endl;
}

void Counters::cnt2(int val) {
    cnt2_trig = val;
}

void Counters::cnt3(int val) {
    cnt3_trig = val;
}

void Counters::cnt4(int val) {
    cnt4_trig = val;
}

void Counters::cnt5(int val) {
    cnt5_trig = val;
}

void Counters::cnt6(int val) {
    cnt6_trig = val;
}

void Counters::fourCounters(int val) {
    if (val>0)
    {
        countersNumTrig = 0;
        checkBox_8->setChecked(false);
    }
    else{
        countersNumTrig = 1;
        checkBox_8->setChecked(true);
    }
}

void Counters::sixCounters(int val) {
    if (val>0)
    {
        countersNumTrig = 1;
        checkBox_7->setChecked(false);
    }
    else{
        countersNumTrig = 0;
        checkBox_7->setChecked(true);
    }
}

