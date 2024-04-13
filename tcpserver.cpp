#include "tcpserver.h"



tcpserver::tcpserver(QObject *parent) : QObject(parent)
{
    //创建监听的服务器对象
    m_s = new QTcpServer(this);//指定父类对象后，析构父类对象时会首先析构子类
    unsigned int port_a = 8899;
    //m_s->listen(QHostAddress::Any,port_a);
    connect(m_s,&QTcpServer::newConnection,this,[=]{
        m_t = m_s->nextPendingConnection();//获取到tcp连接
        //检测是否可以接收数据
        connect(m_t,&QTcpSocket::readyRead,this,[=](){
           QByteArray data = m_t->readAll();
           qDebug()<<data;
           senData();
        });

    });//监听指定端口

}

tcpserver::~tcpserver()
{
    qDebug()<<"调用了析构函数";
}

void tcpserver::listen()
{
    unsigned int port_a = 8899;
    m_s->listen(QHostAddress::Any,port_a);
}

void tcpserver::senData()
{
    QString msg = "hello,this is server";
    m_t->write(msg.toUtf8());
}


