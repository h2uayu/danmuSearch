#include "createthread.h"

CreateThread::CreateThread()
{

}

void CreateThread::startWork(QSqlDatabase *idb, QSqlQuery &iquery, int ii)
{
    db = idb;
    query = iquery;
    i =ii;
    if(!isRunning())
        start();
}

void CreateThread::run()
{
    CRC32 c;

    query.exec("DROP TABLE CRC32Table");
    query.exec("CREATE TABLE CRC32Table ("
                       "uid INTEGER NOT NULL, "
                       "code INTEGER NOT NULL)");
    query.prepare("INSERT INTO CRC32Table (uid, code) VALUES (:uid, :code)");

    int n = i * 250000000 + 1;
    QString uid;


    db->transaction();
    while (n <= (i + 1) * 250000000)
    {
        uid = QString::number(n);
        //qString 转uchar
        uchar temp[20];
        for(int j = 0; j < uid.size(); j++)
        {
            temp[j] = (uchar)uid.at(j).toLatin1();
        }
        temp[uid.size()] = 0;
        //写入数据库
        query.bindValue(":uid", n);
        query.bindValue(":code", c.calculate_CRC32(temp, uid.size()));
        query.exec();

        if(n % 1000000 == 0)
        {
            emit add();
            qDebug()<< "n="<< n/1000000<< "million";
        }

        n++;
    }
    db->commit();

    emit response(true);
}
