#ifndef MAINAPPLICATION_HPP
#define MAINAPPLICATION_HPP

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QByteArray>

class MainApplication : public QObject
{
    Q_OBJECT
public:
    explicit MainApplication(QObject *parent = nullptr);
    QSerialPort controlPort;
    int returnCode;
    int initilizeCommandPort(QString cmdPort);
    int getSerialNumber();
    int writeData(QByteArray &data);
    int flatFieldCorrection();
    int getFPAtemperature();
public slots:
    void readData();
signals:

};

#endif // MAINAPPLICATION_HPP
