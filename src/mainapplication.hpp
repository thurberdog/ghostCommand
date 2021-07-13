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
    // ........ Boson Package (Before Bit Stuffing) ................
    unsigned char   aux_boson_package[768];   // Max package without bit stuffing 768
    unsigned short  aux_boson_package_len  = 0 ;   // To storage real size of package
    unsigned char sequence=0;               // This number increases in every SENT commnand.

    int initilizeCommandPort(QString cmdPort);
    int getSerialNumber();
    int writeData(QByteArray &data);
    int flatFieldCorrection();
    int getFPAtemperature();
    unsigned short CalcBlockCRC16(unsigned int bufferlen, unsigned char *buffer);
public slots:
    void readData();
signals:

};

#endif // MAINAPPLICATION_HPP
