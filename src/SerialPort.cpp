#include "SerialPort.h"

SerialPort::SerialPort(std::string portName)
{
    errors = 0;
    status = {0};
    connected = false;

    handleToCOM = CreateFileA(static_cast<LPCSTR>(portName.data()), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    DWORD errMsg = GetLastError();

    if (errMsg == 2)
    {
        printf("Device not found \n");
    }
    else if (errMsg == 5)
    {
        printf("Another app is using the device \n");
    }
    else if (errMsg == 0)
    {
        DCB dcbSerialParameters = {0};

        if (!GetCommState(handleToCOM, &dcbSerialParameters))
        {
            printf("failed to get current serial parameters");
        }
        else
        {
            std::cout << dcbSerialParameters.BaudRate << std::endl;

            dcbSerialParameters.BaudRate = CBR_115200;
            dcbSerialParameters.ByteSize = 7;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handleToCOM, &dcbSerialParameters))
            {
                printf("could not set serial parameters \n");
            }
            else
            {
                connected = true;
                PurgeComm(handleToCOM, PURGE_RXCLEAR | PURGE_TXCLEAR);
//                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

SerialPort::~SerialPort()
{
    if (connected == true)
    {
        connected = false;
        CloseHandle(handleToCOM);
    }
}

int SerialPort::ReadSerialPort(char *buffer, unsigned int buf_size)
{
    DWORD bytesRead;

    toRead = 0;

    ClearCommError(handleToCOM, &errors, &status);

    if (status.cbInQue > 0)
    {
        if (status.cbInQue > buf_size)
        {
            toRead = buf_size;
        }
        else toRead = status.cbInQue;
    }

    if (ReadFile(handleToCOM, buffer, toRead, &bytesRead, NULL))
    {
        std::cout << "cbInQue " << status.cbInQue << std::endl;
        std::cout << "bytesRead  " << bytesRead << std::endl;
        std::cout << "toRead  " << toRead << std::endl;
        std::cout << "buffer  " << buffer << std::endl;

        return bytesRead;
    }

    return 0;
}

bool SerialPort::WriteSerialPort(char *buffer, unsigned int buf_size)
{
    return 0;
}

bool SerialPort::isConnected()
{
    return connected;
}