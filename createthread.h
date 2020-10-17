#ifndef CREATETHREAD_H
#define CREATETHREAD_H

#include <QThread>
#include <QtSql>
#include "crc32.h"

class CreateThread : public QThread
{
    Q_OBJECT
public:
    CreateThread();
    void startWork(QSqlDatabase *db, QSqlQuery &query, int i);   //参数：数据库,query,序号

signals:
    void response(const bool ok);
    void add();     //进度条+0.1%

private:
    void run() override;
    QSqlDatabase *db;
    QSqlQuery query;
    int i;
};

#endif // CREATETHREAD_H
