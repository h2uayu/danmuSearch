#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H

#include <QThread>
#include <QFile>
#include <QtDebug>
#include <QCryptographicHash>

class CheckThread : public QThread
{
    Q_OBJECT
public:
    CheckThread();
    void startWork(uint i, const QString &ihash); //参数：文件序号，哈希

signals:
    void response(uchar ok);

private:
    void run() override;
    QString hash;
    int i;
};

#endif // CHECKTHREAD_H
