#include <QCoreApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    tcpserver tcp;
    tcp.listen();
    return a.exec();
}
