
#include "tcpthread.h"

#include <QDateTime>
#include <QFile>


TcpThread::TcpThread(QTcpSocket *tcpsocket,QObject *parent) : QThread(parent)
{
    thread_tcpsocket = tcpsocket;

}

TcpThread::~TcpThread()
{
    thread_tcpsocket->close();
    thread_tcpsocket->deleteLater();
    //this->deleteLater();
}

void TcpThread::sendData(const QByteArray &data)
{
    //    QString msg = "hello,this is server";
    //    thread_tcpsocket->write(msg.toUtf8());
    thread_tcpsocket->write(data);
}

void TcpThread::run()
{
    //检测是否可以接收数据
    init();
    connect(thread_tcpsocket,&QTcpSocket::readyRead,this,&TcpThread::onReadyRead);
    connect(thread_tcpsocket,&QTcpSocket::disconnected,this,[=]{
        //emit sig_tcpDisconnected(thread_tcpsocket);
        this->deleteLater();

    });
}

void TcpThread::init()
{
    storedenable = true;
    clientInfo.netInfo.ip = thread_tcpsocket->peerAddress().toString().replace(0,7,"");
    clientInfo.netInfo.port = thread_tcpsocket->peerPort();
    preHeadByteArray = QByteArray::fromHex(preHeadhexString.toLatin1());
}


void TcpThread::onReadyRead()
{
    //QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    while(thread_tcpsocket->bytesAvailable()>0){
        QByteArray data = thread_tcpsocket->readAll();
        receiveBuffer.append(data);
        dataSubpackage();
    }
}

void TcpThread::slt_changeStoredStatus(bool b)
{
    qDebug()<<"是否存储:"<<b;
    storedenable = b;
}


void TcpThread::handleReceiveData()
{
    // 处理缓冲队列中的数据
    QByteArray data;
    while (!clientInfo.bufferQueue.isEmpty()) {
        data.append(clientInfo.bufferQueue.dequeue());//dequeue()出队
    }
    if(storedenable){
        //dataPersistence(data.size(),data);
        // 将数据任务加入到任务队列中
        DataTask *task = new DataTask();
        task->data = data;
        emit sig_AddTask(task);
    }

    //    thread_tcpsocket->write(data);
    //    thread_tcpsocket->flush();
}

void  TcpThread::createNewFileName()
{
    // 获取当前时间作为文件名
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    currentFileName = clientInfo.netInfo.ip+"_"+QString::number(clientInfo.netInfo.port)+"_" + timestamp + storedFormat;

    //currentFileSize = 0;
    //return currentFileName;
}

void TcpThread::dataPersistence(qint64 dataSize,QByteArray &data)
{
    currentFileSize = getCurrentFileSize();
    // 检查数据大小
    if (currentFileSize + dataSize > maxFileSize || currentFileSize==-1) {
        // 超过文档大小限制，创建新文档
        createNewFileName();
    }
    // 将数据写入当前文档
    QFile file(currentFileName);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        // 文件打开失败，执行异常处理操作
        qDebug() << "Failed to open file for writing!";
        // 可以在这里添加其他异常处理逻辑，比如记录日志或者显示错误信息给用户
    } else {
        // 文件打开成功，可以继续写入数据
        file.write(data);
        file.close();
    }

}

qint64 TcpThread::getCurrentFileSize()
{
    // 检查文件是否存在
    if (QFile::exists(currentFileName)) {
        QFile file(currentFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qint64 fileSize = file.size();
            file.close();
            return fileSize;
        }

    }
    return -1;

}

void TcpThread::dataSubpackage()
{
//    if(receiveBuffer.size()<headLen){
//        return;
//    }

//    int index = receiveBuffer.indexOf(preHeadByteArray,index+preHeadByteArray.size());
//    if(index ==-1){
//        return;
//    }

    //如果找到了结束包
    if(receiveBuffer.size()<100){
        return;
    }

    receiveSecondBuffer.append(receiveBuffer);


    // 将数据放入缓冲队列中
    clientInfo.bufferQueue.enqueue(receiveSecondBuffer);
    if(storedenable){
        storedBuffer.enqueue(receiveSecondBuffer);
    }
    receiveSecondBuffer.clear();
    // 处理缓冲队列中的数据
    while(!storedBuffer.isEmpty()){
        handleReceiveData();
        storedBuffer.clear();
    }

    receiveSecondBuffer.clear();
}
