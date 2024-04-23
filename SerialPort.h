#ifndef PIVN_APP_VER_1_SERIALPORT_H
#define PIVN_APP_VER_1_SERIALPORT_H

#include <iostream>
#pragma once

#define ARDUINO_WAIT_TIME 2000
#define INPUT_DATA_BYTES 4096

#include "windows.h"
#include "stdio.h"
#include "stdlib.h"

class SerialPort
{
private:

    bool connected;
    COMSTAT status;
    DWORD errors;

public:
    SerialPort(std::string portName);
    ~SerialPort();

    int ReadSerialPort(char* buffer, unsigned int buf_size);
    bool WriteSerialPort(char* buffer, unsigned  int buf_size);
    bool isConnected();
    unsigned int toRead = 0;
    HANDLE handleToCOM;
};

#endif //PIVN_APP_VER_1_SERIALPORT_H
