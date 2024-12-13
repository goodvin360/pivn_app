#include "ReadFromFile.h"

ReadFromFile::ReadFromFile(QWidget *parent) : QWidget(parent) {
    parent = nullptr;
}

ReadFromFile::~ReadFromFile() {}

void ReadFromFile::startReadFromFile() {
    emit sentReadAction();
}

void ReadFromFile::startLoadFile() {
    emit sentLoadAction();
}

void ReadFromFile::editReadDelay(QString delay) {
    emit sentDelayValue(delay);
}

void ReadFromFile::addReadAction() {
    startReadFromFile();
}

void ReadFromFile::addLoadAction() {
    startLoadFile();
}

void ReadFromFile::addReadDelay(QString delay) {
    editReadDelay(delay);
}