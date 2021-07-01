#include <QCoreApplication>
#include <QDebug>
#include "mainapplication.hpp"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainApplication * mainApplication = new MainApplication();
    int  returnCode = mainApplication->initilizeCommandPort("/dev/tty1");
    if (returnCode == 1) {
       qDebug() << __LINE__ << __FUNCTION__ << "Command Port Open";
    } else {
       qDebug() << __LINE__ << __FUNCTION__ << "Command Port Failed to Open " << returnCode;
    }
    mainApplication->getSerialNumber();
    return a.exec();
}
