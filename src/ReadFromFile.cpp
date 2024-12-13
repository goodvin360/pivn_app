#include "ReadFromFile.h"

ReadFromFile::ReadFromFile(QWidget *parent) : QWidget(parent) {
    parent = nullptr;
}

ReadFromFile::~ReadFromFile() {}

void ReadFromFile::printMessage() {
    emit sentMessage();
}

void ReadFromFile::addAction() {
    printMessage();
    std::cout << "i've done" << std::endl;
}