#include "tcpserver.h"


tcpserver::tcpserver(quint16 port,QObject *parent) : QObject(parent)
{
    qDebug()<<"主线程线程id为:"<<QThread::currentThreadId();

    //创建监听的服务器对象
    m_server = new QTcpServer(this);//指定父类对象后，析构父类对象时会首先析构子类
    if (!m_server->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server could not start. Error: " << m_server->errorString();
    }
    else
    {
        qDebug() << "Server started successfully.";
    }
    //检查到客户端发起连接
    connect(m_server,&QTcpServer::newConnection,this,[=](){
        QTcpSocket *m_tcpsocket = m_server->nextPendingConnection();//获取到tcp连接
        //创建子线程
        TcpThread *serverSubthread = new TcpThread(m_tcpsocket);

        //加入链表
        clients.append(serverSubthread);
        //clientSocket.append(m_tcpsocket);

        //启动子线程

        serverSubthread->start();
        //connect(m_tcpsocket,&QTcpSocket::disconnected,this,&tcpserver::slt_disconnected);
        connect(m_tcpsocket,&QTcpSocket::disconnected,this,[=]{
            QString ip =  m_tcpsocket->peerAddress().toString().replace(0,7,"");
            quint16 port = m_tcpsocket->peerPort();
            clients.removeOne(serverSubthread);
            //clientSocket.removeOne(m_tcpsocket);
            emit sig_ClientDisconnected(ip,port);
        });

        //connect(serverSubthread,&TcpThread::sig_tcpDisconnected(m_tcpsocket),this,&slt_);
        connect(this,SIGNAL(sig_stored(bool)),serverSubthread,SLOT(slt_changeStoredStatus(bool)));

    });//监听指定端口
    connect(this,SIGNAL(sig_ClientDisconnected(QString,quint16)),this,SLOT(slt_disconnectedTest(QString,quint16)));
}

tcpserver::~tcpserver()
{
    //    m_tcpsocket->close();
    //    m_tcpsocket->deleteLater();
    m_server->close();
    m_server->deleteLater();
    //this->deleteLater();

}

void tcpserver::slt_disconnected(QTcpSocket *tcpsocket)
{
    QString ip =  tcpsocket->peerAddress().toString().replace(0,7,"");
    quint16 port = tcpsocket->peerPort();
    //clientSocket.removeOne(tcpsocket);
    //    m_tcpsocket->close();
    //    m_tcpsocket->deleteLater();


    connect(this,SIGNAL(sig_ClientDisconnected(QString,quint16)),this,SLOT(slt_disconnectedTest(QString,quint16)));
    emit sig_ClientDisconnected(ip,port);
    //this->deleteLater();

}

void tcpserver::slt_disconnectedTest(QString ip,quint16 port)
{
    qDebug()<<"ip:"<<ip<<" port:"<<port<<" 客户端断开了连接";
}

//void tcpserver::listen()
//{
//    unsigned int port_a = 8899;
//    m_server->listen(QHostAddress::Any,port_a);
//}

//void tcpserver::senData()
//{
//    QString msg = "hello,this is server";
//    m_tcpsocket->write(msg.toUtf8());
//}


