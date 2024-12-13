#include "PlotterOptions.h"

PlotterOptions::PlotterOptions(QWidget *parent) {
    parent = nullptr;
}

PlotterOptions::~PlotterOptions() noexcept {

}


void PlotterOptions::setPlotTrigVal(int val) {
    sentPlotTrig(val);
}

void PlotterOptions::setPlotTotTrigVal(int val) {
    sentPlotTotTrig(val);
}

void PlotterOptions::setPlotTrigRgVal(int val) {
    sentPlotTrigR(val);
}

void PlotterOptions::setPlotTotTrigRgVal(int val) {
    sentPlotTotTrigR(val);
}

void PlotterOptions::setRescaleTrigVal(int val) {
    sentRescaleTrig(val);
}


void PlotterOptions::plotTrigger(int val) {
    setPlotTrigVal(val);
}

void PlotterOptions::plotTriggerTotal(int val) {
    setPlotTotTrigVal(val);
}

void PlotterOptions::plotTriggerRough(int val) {
    setPlotTrigRgVal(val);
}

void PlotterOptions::plotTriggerTotalRough(int val) {
    setPlotTotTrigRgVal(val);
}

void PlotterOptions::rescalePlotTrigger(int val) {
    setRescaleTrigVal(val);
}