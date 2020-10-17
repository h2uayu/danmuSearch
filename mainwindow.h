#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QString>
#include <QLabel>
#include <QtSql>
#include <QThread>
#include "checkthread.h"
#include "querythread.h"
#include "createthread.h"
#include "crc32.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db0 = QSqlDatabase::addDatabase("QSQLITE", "db0");
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QSQLITE", "db1");
    QSqlDatabase db2 = QSqlDatabase::addDatabase("QSQLITE", "db2");
    QSqlDatabase db3 = QSqlDatabase::addDatabase("QSQLITE", "db3");
    QLabel *iLabel[4];

    QueryThread qt0, qt1, qt2, qt3;     //查询进程
    CreateThread ct0, ct1, ct2, ct3;    //创建数据库进程
    CheckThread ckt0, ckt1, ckt2, ckt3; //校验数据库进程
    uchar dbOK = 0;     //数据库完整性标志
    int k = 0;         //需创建的数据库个数
    void setControlEnabled(bool enable);

private slots:
    void onButtonClicked();
    void onButton2Clicked();
    void showResponse(const QStringList &s);
    void showCreateResponse(const bool ok);
    void showCheckResponse(uchar ok);
    void pushProgressBar();
};




#endif // MAINWINDOW_H
