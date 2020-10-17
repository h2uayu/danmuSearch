#include "querythread.h"

QueryThread::QueryThread()
{

}

void QueryThread::startWork(QString &s, QSqlQuery &query)
{
    inputStr = s;
    mquery = query;
    if(!isRunning())
        start();
}

void QueryThread::run()
{
    mquery.exec(inputStr);
    QStringList s;
    while (mquery.next())
    {
        s << mquery.value(0).toString();
    }
    emit response(s);
}
