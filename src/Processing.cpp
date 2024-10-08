#include <qtestsupport_core.h>
#include "Processing.h"

Processing::Processing(QWidget *parent) : QWidget(parent) {
    parent = nullptr;
}

Processing::~Processing() {}

void Processing::clearBack(int trig) {
    clearBackVecTrig = trig;
}

void Processing::multiPulses(int trig) {
    multiPulsesTrig = trig;
}

