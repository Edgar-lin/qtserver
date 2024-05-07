#include <QCoreApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QString b;
//    if(b.isEmpty()){
//        b = "hello";

//    }

    quint16 port = 8899;
    tcpserver tcp(port);
    //tcp.listen();
    return a.exec();
}
