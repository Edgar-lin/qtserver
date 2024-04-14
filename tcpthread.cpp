#include "tcpthread.h"
#include <iostream>


TcpThread::TcpThread(QTcpSocket *tcpsocket,QObject *parent) : QThread(parent)
{
    thread_tcpsocket = tcpsocket;
}

void TcpThread::sendData(QTcpSocket *tcpsocket,const QByteArray &data)
{
    QString msg = "hello,this is server";
    tcpsocket->write(msg.toUtf8());
    tcpsocket->write(data);
}

void TcpThread::run()
{
    //检测是否可以接收数据
    connect(thread_tcpsocket,&QTcpSocket::readyRead,this,&TcpThread::onReadyRead);
//    {
//       QByteArray data = thread_tcpsocket->readAll();
//       qDebug()<<data;
//       qDebug()<<"当前线程id为:"<<QThread::currentThreadId();
//       sendData(thread_tcpsocket,data);

    //    });
}


void TcpThread::onReadyRead()
{
    //QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    // 读取固定大小的数据
    QByteArray data = thread_tcpsocket->read(1024); // 假设每次读取1024字节数据
    if (data.isEmpty())
        return;

    // 将数据放入缓冲队列中
    bufferQueue.enqueue(data);
    qDebug() << "Received data:" << data;

    // 处理缓冲队列中的数据
    handleReceiveData();

}


void TcpThread::handleReceiveData()
{
    // 处理缓冲队列中的数据
    while (!bufferQueue.isEmpty()) {
        QByteArray data = bufferQueue.dequeue();//dequeue() //出队。
        thread_tcpsocket->write(data);
    }
    thread_tcpsocket->flush();
}
