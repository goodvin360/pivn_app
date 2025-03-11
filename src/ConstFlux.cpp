#include "ConstFlux.h"

ConstFlux::ConstFlux(QWidget *parent) {
    parent = nullptr;
}

ConstFlux::~ConstFlux() noexcept {

}

void ConstFlux::setConstFluxGo() {
    sentConstFluxGo();
}

void ConstFlux::setConstFluxTrig(int val) {
    sentConstFluxTrig(val);
}

void ConstFlux::setEdgePointPlus() {
    sentEdgePointPlus();
}

void ConstFlux::setEdgePointMinus() {
    sentEdgePointMinus();
}

void ConstFlux::setEdgePointPlusR() {
    sentEdgePointPlusR();
}

void ConstFlux::setEdgePointMinusR() {
    sentEdgePointMinusR();
}

void ConstFlux::addConstFluxGo() {
    setConstFluxGo();
}

void ConstFlux::addConstFluxTrig(int val) {
    setConstFluxTrig(val);
}

void ConstFlux::edgePointPlus() {
    setEdgePointPlus();
}

void ConstFlux::edgePointMinus() {
    setEdgePointMinus();
}

void ConstFlux::edgePointPlusR() {
    setEdgePointPlusR();
}

void ConstFlux::edgePointMinusR() {
    setEdgePointMinusR();
}
