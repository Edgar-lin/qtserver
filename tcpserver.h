#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "tcpthread.h"
#include <DataWriter.h>


class tcpserver : public QObject
{
    Q_OBJECT
public:
    explicit tcpserver(quint16 port,QObject *parent = nullptr);
    ~tcpserver();

    QList<TcpThread *> clients;
    //QList<QTcpSocket *> clientSocket;
//    void senData();//发送数据
    //void listen();//监听指定端口信息
signals:
    void sig_ClientDisconnected(QString,quint16);
    void sig_stored(bool);//存储状态信息
public slots:
    void slt_disconnected(QTcpSocket *tcpsocket);
    void slt_disconnectedTest(QString ip,quint16 port);
private:
    QTcpServer *m_server;
    //QTcpSocket *m_tcpsocket;
    DataWriter

};

#endif // TCPSERVER_H
