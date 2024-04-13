#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#define port = 8899;
class tcpserver : public QObject
{
    Q_OBJECT
public:
    explicit tcpserver(QObject *parent = nullptr);
    ~tcpserver();
    void senData();//发送数据
    void listen();//监听指定端口信息
signals:

public slots:


private:
    QTcpServer *m_s;
    QTcpSocket *m_t;
};

#endif // TCPSERVER_H
