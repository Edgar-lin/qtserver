#include "tcpthread.h"
#include <iostream>


TcpThread::TcpThread(QTcpSocket *tcpsocket,QObject *parent) : QThread(parent)
{
    thread_tcpsocket = tcpsocket;
}

void TcpThread::sendData(QTcpSocket *tcpsocket)
{
    QString msg = "hello,this is server";
    tcpsocket->write(msg.toUtf8());
}

void TcpThread::run()
{
    qDebug()<<"当前线程id为:"<<QThread::currentThreadId();
    //检测是否可以接收数据
    connect(thread_tcpsocket,&QTcpSocket::readyRead,this,[=](){
       QByteArray data = thread_tcpsocket->readAll();
       qDebug()<<data;
       sendData(thread_tcpsocket);
    });
}
