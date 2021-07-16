#include "mainapplication.hpp"

MainApplication::MainApplication(QObject *parent) : QObject(parent)
{
    connect(&controlPort, SIGNAL(readyRead()), this,
                            SLOT(readACM0()));
}


int MainApplication::initilizeCommandPort(QString cmdPort)
{
    sequence = 1;
    controlPort.setPortName(cmdPort);
    controlPort.setBaudRate(QSerialPort::Baud115200);
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
    QByteArray crc;
    crc.resize(2);
    crc[0] = 0x01;
    crc[1] = 0x0B;
    return writeData(functionID, crc);
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
    QByteArray crc;
    crc.resize(2);
    crc[0] = 0x22;
    crc[1] = 0x5C;
    return writeData(functionID, crc);
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
    QByteArray crc;
    crc.resize(2);
    crc[0] = 0x49;
    crc[1] = 0x66;
    return writeData(functionID, crc);
}
int MainApplication::writeData(QByteArray &data, QByteArray &crc)
{
//          Byte 0 - Start frame byte = 0x8E
//          Byte 1 - Channel ID = 0
//          Bytes 2:5 - sequence number
//          Bytes 6:9 - function ID
//          Bytes 10:13 - 0xFFFFFFFF
//          Bytes 14:15 - unsigned 16-bit CRC
//          Byte 16 - End frame byte = 0xAE
    QByteArray packet;
    packet.resize(17);
    packet[0] = 0x8E;  // Start packet
    packet[1] = 0x00;  // channel number
    packet[2] = 0x00;  // sequence number
    packet[3] = 0x00;  // sequence number
    packet[4] = 0x00;  // sequence number
    packet[5] = sequence & 0xFF;  // sequence number
    packet[6] = data[0]; // function ID
    packet[7] = data[1]; // function ID
    packet[8] = data[2]; // function ID
    packet[9] = data[3]; // function ID
    packet[10] = 0xFF;
    packet[11] = 0xFF;
    packet[12] = 0xFF;
    packet[13] = 0xFF;
    packet[14] = crc[0]; // CRC
    packet[15] = crc[1]; // CRC
    packet[16] = 0xAE; // End packet

    controlPort.write(packet);
    qDebug() << __LINE__ << __FUNCTION__ << controlPort.errorString();
    qDebug() << __LINE__ << __FUNCTION__ << packet.toHex();

    return controlPort.error();
}

void MainApplication::readACM0()
{
    QByteArray data = controlPort.readAll();
    qDebug() << __LINE__ << __FUNCTION__ << controlPort.errorString();
    qDebug() << __LINE__ << __FUNCTION__ << data.toHex();
}
// Boson is using this method: CRC-16/AUG-CCITT
// https://github.com/meetanthony/crcphp/blob/master/crc16/crc_16_aug_ccitt.php
unsigned short MainApplication::CalcBlockCRC16(unsigned int bufferlen, unsigned char *buffer)
{
  unsigned short ccitt_16Table[] =
  {
      0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5,
      0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B,
      0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, 0x1231, 0x0210,
      0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
      0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C,
      0xF3FF, 0xE3DE, 0x2462, 0x3443, 0x0420, 0x1401,
      0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B,
      0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
      0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6,
      0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738,
      0xF7DF, 0xE7FE, 0xD79D, 0xC7BC, 0x48C4, 0x58E5,
      0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
      0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969,
      0xA90A, 0xB92B, 0x5AF5, 0x4AD4, 0x7AB7, 0x6A96,
      0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC,
      0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
      0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03,
      0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD,
      0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 0x7E97, 0x6EB6,
      0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
      0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A,
      0x9F59, 0x8F78, 0x9188, 0x81A9, 0xB1CA, 0xA1EB,
      0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1,
      0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
      0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C,
      0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2,
      0x4235, 0x5214, 0x6277, 0x7256, 0xB5EA, 0xA5CB,
      0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
      0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447,
      0x5424, 0x4405, 0xA7DB, 0xB7FA, 0x8799, 0x97B8,
      0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2,
      0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
      0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9,
      0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827,
      0x18C0, 0x08E1, 0x3882, 0x28A3, 0xCB7D, 0xDB5C,
      0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
      0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0,
      0x2AB3, 0x3A92, 0xFD2E, 0xED0F, 0xDD6C, 0xCD4D,
      0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07,
      0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
      0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA,
      0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74,
      0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
  };
  unsigned int i;
  unsigned short crc = 0x1D0F;   // CRC_16_AUG_CCITT Init Value

  if( buffer!=0) {
    for (i=0; i<bufferlen; i++) {
      crc = (unsigned short) ((crc << 8) ^ ccitt_16Table[(crc >> 8) ^ buffer[i]]);
    }
  }
  return crc;
}

void MainApplication::closeACM()
{
        controlPort.close();
}
