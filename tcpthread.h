#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>

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
    void sendData(QTcpSocket *tcpsocket);
protected:
    void run() override;

private:
    QTcpSocket *thread_tcpsocket = nullptr;

signals:

public slots:
};



#endif // TCPTHREAD_H
