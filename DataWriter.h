#ifndef DATAWRITER_H
#define DATAWRITER_H

#include <QMutex>
#include <QObject>

// 数据任务类
class DataTask {
public:
    QByteArray data;
};


class DataWriter : public QObject
{
    Q_OBJECT
public:
    explicit DataWriter(QQueue<DataTask *> *taskQueue, QMutex *mutex,
                        QObject *parent) : QObject(parent),
        m_taskQueue(taskQueue), m_mutex(mutex);
    //explicit DataWriter(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DATAWRITER_H
