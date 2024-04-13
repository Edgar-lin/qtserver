#include "tcpserver.h"
#include "tcpthread.h"


tcpserver::tcpserver(QObject *parent) : QObject(parent)
{
    qDebug()<<"主线程线程id为:"<<QThread::currentThreadId();
    //创建监听的服务器对象
    m_server = new QTcpServer(this);//指定父类对象后，析构父类对象时会首先析构子类
    //检查到客户端发起连接
    connect(m_server,&QTcpServer::newConnection,this,[=]{
        m_tcpsocket = m_server->nextPendingConnection();//获取到tcp连接
        //创建子线程
        TcpThread *serverSubthread = new TcpThread(m_tcpsocket);
        //启动子线程
        serverSubthread->start();


    });//监听指定端口

}

tcpserver::~tcpserver()
{
    qDebug()<<"调用了析构函数";
}

void tcpserver::listen()
{
    unsigned int port_a = 8899;
    m_server->listen(QHostAddress::Any,port_a);
}

//void tcpserver::senData()
//{
//    QString msg = "hello,this is server";
//    m_tcpsocket->write(msg.toUtf8());
//}


