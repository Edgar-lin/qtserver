#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QQueue>
#include <QHostAddress>
#include "DataWriter.h"
class TcpThread : public QThread
{
    Q_OBJECT
public:
    explicit TcpThread(QTcpSocket *tcpsocket,QObject *parent = nullptr);
    ~TcpThread();
    void sendData(const QByteArray &data);

    struct NetInfo{
        QString ip;
        quint16 port;
    };

    struct ClientInfo{
        NetInfo netInfo;
        QQueue<QByteArray> bufferQueue;
    };
    ClientInfo clientInfo;
protected:
    void run() override;

private:
    //========================================================
    //@Description: 初始化
    //@Param:
    //@return:
    //========================================================
    void init();

    //========================================================
    //@Description: 接收数据的处理
    //@Param:
    //@return:
    //========================================================
    void handleReceiveData();

    //========================================================
    //@Description: 创建新文档名字
    //@Param:
    //@return:
    //========================================================
    void createNewFileName();

    //========================================================
    //@Description: 数据持久化
    //@Param:
    //@return:
    //========================================================
    void dataPersistence(qint64 dataSize,QByteArray &data);

    //========================================================
    //@Description: 获取文档大小
    //@Param:
    //@return:
    //========================================================
    qint64 getCurrentFileSize();

    //========================================================
    //@Description: 数据分包
    //@Param:
    //@return:
    //========================================================
    void dataSubpackage();

    //========================================================
    //@Description: 接收部分
    //========================================================
    QTcpSocket *thread_tcpsocket = nullptr;
    QByteArray receiveBuffer;//一级缓存
    QByteArray receiveSecondBuffer;//二级缓存
    QString preHeadhexString = "feef9ff9";
    QByteArray preHeadByteArray;
    //int headLen=32;

    //========================================================
    //@Description: 存储部分
    //========================================================
    bool storedenable;
    qint64 maxFileSize = 1000000;// 1 MB
    QString currentFileName;
    qint64 currentFileSize = 0;
    QQueue<QByteArray> storedBuffer;
    QString storedFormat=".txt";
signals:
    void sig_tcpDisconnected(QTcpSocket *thread_tcpsocket);
public slots:
    
private slots:
    void onReadyRead();
    void slt_changeStoredStatus(bool b);
};



#endif // TCPTHREAD_H
