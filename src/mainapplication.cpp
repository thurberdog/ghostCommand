#include "mainapplication.hpp"

MainApplication::MainApplication(QObject *parent) : QObject(parent)
{
    connect(&controlPort, SIGNAL(readyRead()), this,
                            SLOT(readData()));
}


int MainApplication::initilizeCommandPort(QString cmdPort)
{
    controlPort.setPortName(cmdPort);
    controlPort.setBaudRate(QSerialPort::Baud9600);
    controlPort.setDataBits(QSerialPort::Data8);
    controlPort.setStopBits(QSerialPort::OneStop);
    controlPort.setParity(QSerialPort::NoParity);
    controlPort.setFlowControl(QSerialPort::NoFlowControl);
    returnCode = controlPort.open(QSerialPort::ReadWrite);
    qDebug() << __LINE__ << __FUNCTION__ << controlPort.errorString();
    return returnCode;
}
int MainApplication::getSerialNumber()
{
    // Get the camera serial number

    QByteArray functionID;
    functionID.resize(4);
    functionID[0] = 0x00;
    functionID[1] = 0x05;
    functionID[2] = 0x00;
    functionID[3] = 0x02;
    return writeData(functionID);
}

int MainApplication::flatFieldCorrection()
{
    // Manually request a flat field correction (FFC)
    QByteArray functionID;
    functionID.resize(4);
    functionID[0] = 0x00;
    functionID[1] = 0x05;
    functionID[2] = 0x00;
    functionID[3] = 0x07;
    return writeData(functionID);
}

int MainApplication::getFPAtemperature()
{
    //  Get the current focal plane array (FPA) temperature in Celsius.

//    Returns
//    -------
//        float
//            FPA temperature in Celsius
    QByteArray functionID;
    functionID.resize(4);
    functionID[0] = 0x00;
    functionID[1] = 0x05;
    functionID[2] = 0x00;
    functionID[3] = 0x30;
    return writeData(functionID);
}
int MainApplication::writeData(QByteArray &data)
{
//    Start frame byte = 0x8E
//          Channel ID = 0
//          Bytes 0:3 - sequence number
//          Bytes 4:7 - function ID
//          Bytes 8:11 - 0xFFFFFFFF
//          Bytes 12: - payload (optional)
//          CRC bytes - unsigned 16-bit CRC
//          End frame byte = 0xAE
    QByteArray packet;
    packet.resize(18);
    packet[0] = 0x8E;  // Start packet
    packet[1] = 0x00;  // channel number
    packet[2] = 0x00;  // sequence number
    packet[3] = 0x00;  // sequence number
    packet[4] = 0x00;  // sequence number
    packet[5] = 0x01;  // sequence number
    packet[6] = data[0]; // function ID
    packet[7] = data[1]; // function ID
    packet[8] = data[2]; // function ID
    packet[9] = data[3]; // function ID
    packet[10] = 0xFF;
    packet[11] = 0xFF;
    packet[12] = 0xFF;
    packet[13] = 0xFF;
    packet[14] = 0x00; // payload
    packet[15] = 0x00; // CRC
    packet[16] = 0x00; // CRC
    packet[17] = 0xAE; // End packet

    controlPort.write(packet);
    qDebug() << __LINE__ << __FUNCTION__ << controlPort.errorString();
    qDebug() << __LINE__ << __FUNCTION__ << packet;
    controlPort.close();
    return controlPort.error();
}

void MainApplication::readData()
{
    QByteArray data = controlPort.readAll();
    qDebug() << __LINE__ << __FUNCTION__ << controlPort.errorString();
    qDebug() << __LINE__ << __FUNCTION__ << data;
}
