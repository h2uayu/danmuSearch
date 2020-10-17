#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir dir;
    dir.mkdir("db");    //创建db文件夹

    ckt0.startWork(0, tr("b9b6862159f3b19e85d004173c24cee839494e84"));
    ckt1.startWork(1, tr("4c2bbf2bb482c411c034ec1e02037b3fcc529ad4"));
    ckt2.startWork(2, tr("18af4df27ad077c98965ebb778a6317e23b4c0cb"));
    ckt3.startWork(3, tr("1fc0a8db905ece1a838e20356ef865fb5c36d1ca"));



    ui->pushButton_2->setEnabled(false);
    setControlEnabled(false);

    iLabel[0] = ui->label;
    iLabel[1] = ui->label_2;
    iLabel[2] = ui->label_3;
    iLabel[3] = ui->label_4;
    iLabel[1]->hide();
    iLabel[2]->hide();
    iLabel[3]->hide();

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onButton2Clicked);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::onButtonClicked);
    connect(&qt0, &QueryThread::response, this, &MainWindow::showResponse);
    connect(&qt1, &QueryThread::response, this, &MainWindow::showResponse);
    connect(&qt2, &QueryThread::response, this, &MainWindow::showResponse);
    connect(&qt3, &QueryThread::response, this, &MainWindow::showResponse);
    connect(&ckt0, &CheckThread::response, this, &MainWindow::showCheckResponse);
    connect(&ckt1, &CheckThread::response, this, &MainWindow::showCheckResponse);
    connect(&ckt2, &CheckThread::response, this, &MainWindow::showCheckResponse);
    connect(&ckt3, &CheckThread::response, this, &MainWindow::showCheckResponse);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonClicked()
{
    setControlEnabled(false);

    iLabel[1]->hide();
    iLabel[2]->hide();
    iLabel[3]->hide();
    ui->textBrowser->setText("查询中，请稍等……");

    bool ok;
    unsigned int input = ui->lineEdit->text().toUInt(&ok, 16);
    if(!ok || input > 0xFFFFFFFF)
    {
        ui->textBrowser->setText("输入有误");
        setControlEnabled(true);
        return;
    }
    qDebug()<<input;

    QSqlQuery query0(db0);
    QSqlQuery query1(db1);
    QSqlQuery query2(db2);
    QSqlQuery query3(db3);

    QString s;
    s = "SELECT uid FROM CRC32Table WHERE code = " + QString::number(input);
    qDebug()<< s;

    qt0.startWork(s, query0);
    qt1.startWork(s, query1);
    qt2.startWork(s, query2);
    qt3.startWork(s, query3);
}

void MainWindow::onButton2Clicked()
{
    setControlEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->textBrowser->setText("数据库创建中，请稍等……");

    db0.setDatabaseName("./db/CRC32Table0.dat"); //连接数据库
    if (!db0.open()) {
           qDebug()<< QObject::tr("Database Error")<< db0.lastError().text();
    }
    db1.setDatabaseName("./db/CRC32Table1.dat");
    if (!db1.open()) {
           qDebug()<< QObject::tr("Database Error")<< db1.lastError().text();
    }
    db2.setDatabaseName("./db/CRC32Table2.dat");
    if (!db2.open()) {
           qDebug()<< QObject::tr("Database Error")<< db2.lastError().text();
    }
    db3.setDatabaseName("./db/CRC32Table3.dat");
    if (!db3.open()) {
           qDebug()<< QObject::tr("Database Error")<< db3.lastError().text();
    }

    QSqlQuery query0(db0);
    QSqlQuery query1(db1);
    QSqlQuery query2(db2);
    QSqlQuery query3(db3);
    connect(&ct0, &CreateThread::response, this, &MainWindow::showCreateResponse);
    connect(&ct1, &CreateThread::response, this, &MainWindow::showCreateResponse);
    connect(&ct2, &CreateThread::response, this, &MainWindow::showCreateResponse);
    connect(&ct3, &CreateThread::response, this, &MainWindow::showCreateResponse);
    connect(&ct0, &CreateThread::add, this, &MainWindow::pushProgressBar);
    connect(&ct1, &CreateThread::add, this, &MainWindow::pushProgressBar);
    connect(&ct2, &CreateThread::add, this, &MainWindow::pushProgressBar);
    connect(&ct3, &CreateThread::add, this, &MainWindow::pushProgressBar);
    if((dbOK & (1 << 0)) == 0)      //第0位为0
    {
        k++;
        ct0.startWork(&db0, query0, 0);
    }
    if((dbOK & (1 << 1)) == 0)      //第1位为0
    {
        k++;
        ct1.startWork(&db1, query1, 1);
    }
    if((dbOK & (1 << 2)) == 0)      //第2位为0
    {
        k++;
        ct2.startWork(&db2, query2, 2);
    }
    if((dbOK & (1 << 3)) == 0)      //第3位为0
    {
        k++;
        ct3.startWork(&db3, query3, 3);
    }
    ui->progressBar->setMaximum(250 * k);
}


void MainWindow::showResponse(const QStringList &uid)
{
    static int j = 0, n;
    j++;
    static QString s;
    if(1 == j)
    {
        n = 0;
        s.clear();
    }
    if(!uid.isEmpty())
    {
        for (int i = 0; i < uid.size(); ++i)
        {
            n++;
            s += uid.at(i) + "\n";
            iLabel[i+1]->setText("<html><head/><body><p><a href=""https://space.bilibili.com/"
                                 + uid.at(i)
                                 + """><span style="" text-decoration: none; color:#00FFCC;"">进入空间</span></a></p></body></html>");
        }
    }
    if(4 == j)
    {
        if(s.isEmpty())
            ui->textBrowser->setText("无结果");
        else
        {
            //显示所有有效链接
            ui->textBrowser->setText(s);
            for(int i = 0; i < n; i++)
            {
                iLabel[i+1]->show();
            }
        }
        setControlEnabled(true);
        j = 0;
        n = 0;
    }
}

void MainWindow::showCreateResponse(const bool ok)
{
    static int i = 0;
    if(ok)
        i++;
    if(i == k)
    {
        i = 0;

        ui->textBrowser->setText("数据库创建完毕");
        ui->pushButton_2->setEnabled(false);
        setControlEnabled(true);
    }
}

void MainWindow::showCheckResponse(uchar ok)
{
    static int i = 0;
    i++;
    dbOK |= ok;
    if(4 == i)
    {
        i = 0;
        if (dbOK == 15) //二进制1111
        {
            db0.setDatabaseName("./db/CRC32Table0.dat"); //连接数据库
            if (!db0.open()) {
                   qDebug()<< QObject::tr("Database Error")<< db0.lastError().text();
            }
            db1.setDatabaseName("./db/CRC32Table1.dat");
            if (!db1.open()) {
                   qDebug()<< QObject::tr("Database Error")<< db1.lastError().text();
            }
            db2.setDatabaseName("./db/CRC32Table2.dat");
            if (!db2.open()) {
                   qDebug()<< QObject::tr("Database Error")<< db2.lastError().text();
            }
            db3.setDatabaseName("./db/CRC32Table3.dat");
            if (!db3.open()) {
                   qDebug()<< QObject::tr("Database Error")<< db3.lastError().text();
            }

            ui->textBrowser->setText("数据库完整");
            ui->pushButton_2->setEnabled(false);
            setControlEnabled(true);
        }
        else
        {
            ui->textBrowser->setText("数据库不完整,请创建数据库");
            ui->pushButton_2->setEnabled(true);
            setControlEnabled(false);
        }
    }
}
void MainWindow::setControlEnabled(bool enable)
{
    ui->pushButton->setEnabled(enable);
    ui->lineEdit->setEnabled(enable);
}

void MainWindow::pushProgressBar()
{
    static int value = 0;
    value++;
    if(1001 == value)
        value = 1;
    qDebug()<< "value="<< value;
    ui->progressBar->setValue(value);
}
