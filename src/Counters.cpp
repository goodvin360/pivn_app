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

