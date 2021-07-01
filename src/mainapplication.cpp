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
    QByteArray functionID;
    functionID.resize(4);
    functionID[0] = 0x00;
    functionID[1] = 0x05;
    functionID[2] = 0x00;
    functionID[3] = 0x10;
    return writeData(functionID);
}
int MainApplication::writeData(QByteArray &data)
{
    controlPort.write(data);
    qDebug() << __LINE__ << __FUNCTION__ << controlPort.errorString();
    qDebug() << __LINE__ << __FUNCTION__ << data;
    controlPort.close();
    return controlPort.error();
}

void MainApplication::readData()
{
    QByteArray data = controlPort.readAll();
    qDebug() << __LINE__ << __FUNCTION__ << controlPort.errorString();
    qDebug() << __LINE__ << __FUNCTION__ << data;
}
