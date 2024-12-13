#include "TriggerMode.h"

TriggerMode::TriggerMode(QWidget *parent) {
    parent = nullptr;
}

TriggerMode::~TriggerMode() noexcept {

}

void TriggerMode::setGoTrigger() {
    sentGoTrigger();
}

void TriggerMode::setManualTrigger(int val) {
    sentManualTrigger(val);
}

void TriggerMode::setAutoTrigger(int val) {
    sentAutoTrigger(val);
}

void TriggerMode::addGoTrigger() {
    setGoTrigger();
}

void TriggerMode::manualTrigger(int val) {
    setManualTrigger(val);
}

void TriggerMode::autoTrigger(int val) {
    setAutoTrigger(val);
}
