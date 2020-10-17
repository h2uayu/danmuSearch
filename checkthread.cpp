#include "checkthread.h"

CheckThread::CheckThread()
{

}

void CheckThread::startWork(uint ii, const QString &ihash)
{
    i = ii;
    hash = ihash;
    if(!isRunning())
        start();
}
void CheckThread::run()
{
    QCryptographicHash ch(QCryptographicHash::Sha1);
    QString dir = "db/CRC32Table" + QString::number(i) + ".dat";
    QFile f(dir);
    if(f.open(QIODevice::ReadOnly))
       qDebug()<< "打开成功" ;
    else
    {
        emit response(0);
        return;
    }
    QByteArray buf;
    uint totalByte = f.size();
    uint loadSize = 1024*4;
    uint toWrite;
    toWrite = totalByte;
    while (1)
    {
        if(toWrite > 0)
        {
            buf = f.read(qMin(toWrite,loadSize));
            ch.addData(buf);
            toWrite -= buf.length();
            buf.clear();
        }
        else
        {
            break;
        }
    }
    f.close();
    QByteArray result = ch.result();
    qDebug()<< "hash:"<< result.toHex();

    if(result.toHex() == hash)
        emit response(1 << i);
    else
    {
        f.remove();             //需删除整个数据库重新创建，否则hash值会变
        emit response(0);
    }
}
