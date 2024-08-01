#include "FluxCalc.h"

FluxCalc::FluxCalc(QWidget *parent) {
    parent = nullptr;
}

FluxCalc::~FluxCalc() noexcept {}

void FluxCalc::printMessage(QString msg, int num) {
    emit sentMessage(msg, num);
}

void FluxCalc::calculateFlux(vecFill &data, double trig) {
//    printMessage(QString::number(flux));
}
