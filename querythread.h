#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QThread>
#include <QtSql>

class QueryThread : public QThread
{
    Q_OBJECT
public:
    QueryThread();

    void startWork(QString &s, QSqlQuery &query);

signals:
    void response(const QStringList &s);

private:
    void run() override;

    QString inputStr;
    QString returnStr;
    QSqlQuery mquery;


};

#endif // QUERYTHREAD_H
