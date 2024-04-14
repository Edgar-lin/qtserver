#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QQueue>

struct ServerProcessingMethod {
    QString storage = "storage";
    QString send = "send";
    QString receive = "receive";
};

class TcpThread : public QThread
{
    Q_OBJECT
public:
    explicit TcpThread(QTcpSocket *tcpsocket,QObject *parent = nullptr);
    void sendData(QTcpSocket *tcpsocket,const QByteArray &data);
protected:
    void run() override;

private:
    QTcpSocket *thread_tcpsocket = nullptr;
    QQueue<QByteArray> bufferQueue;

    void handleReceiveData();
signals:

public slots:
    
private slots:
    void onReadyRead();
};



#endif // TCPTHREAD_H
