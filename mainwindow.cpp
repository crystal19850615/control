#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QTimer>

#include <qdebug.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDate date;
    QTime time;
    QTimer *timer = new QTimer(this);
    QTimer *commTimer = new QTimer(this);
    QTimer *dispTimer = new QTimer(this);
    QString portName = "/dev/ttyUSB0";
    m_scc = new SrvClntComm(portName);

    ui->setupUi(this);
    ui->failureTip->hide();
    move(0, 0);
    if (m_scc->m_isFailure)
    {
        ui->failureTip->show();
    }

    //this->installEventFilter(this);

    QString strDate = date.currentDate().toString("M-dd");
    QString strTime = time.currentTime().toString("hh:mm");
    QString strAll = "  DATE: " + strDate + "             " + "TIME: " + strTime;
    ui->timeLabel->setText(strAll);

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->setInterval(1000);
    timer->start();
    connect(commTimer, SIGNAL(timeout()), m_scc, SLOT(updateFromClnt()));
    commTimer->setInterval(20);
    commTimer->start();
    connect(dispTimer, SIGNAL(timeout()), this, SLOT(calculate_AS()));
    dispTimer->setInterval(20);
    dispTimer->start();
}

MainWindow::~MainWindow()
{
    delete m_scc;
    delete ui;
}

void MainWindow::update()
{
    QDate date;
    QTime time;

    QString strDate = date.currentDate().toString("M-dd");
    QString strTime = time.currentTime().toString("hh:mm");
    QString strAll = "  DATE: " + strDate + "             " + "TIME: " + strTime;
    ui->timeLabel->setText(strAll);

    if (m_countOfFailed >= 3)
    {
        ui->failureTip->show();
    }
}

void MainWindow::on_settingButton_clicked()
{
    settingDiag = new SettingDialog();
    settingDiag->setAttribute(Qt::WA_DeleteOnClose);
    settingDiag->setModal(true);
    settingDiag->exec();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::WindowActivate)
    {
        QSqlQuery *queryRes;
        float fAirSpeedSet = -1.0;
        m_dbm = new DbManager(m_dbPath, "queryEventActivate");
        queryRes = m_dbm->query(m_strQueryAirSpeedSet);

        queryRes->next();
        fAirSpeedSet = queryRes->value("val").toFloat();
        queryRes->clear();
        delete queryRes;
        m_dbm->close();

        if(fAirSpeedSet > 0.0)
        {
            //qDebug() << "get fAirSpeedSet succeeded";
            static_cast<MainWindow*>(watched)->ui->airSpeedSet->setText(QString::number(fAirSpeedSet));
        }
        else
        {
            qDebug() << "get fAirSpeedSet failed";
        }
    }
    return false;
}
double MainWindow::GetParameter(const QString &ParameterName)
{
    QSqlQuery *queryRes;
    QString   strQuery;

    strQuery = "select val from confNew where name = " + ParameterName;
    queryRes = m_dbm->query(strQuery);
    queryRes->next();
    return queryRes->value("val").toDouble();
}

void MainWindow::calculate_AS()
{
    QSqlQuery *queryRes;
    QString strUpdate;
    double fASL;
    double FactorySetingUpLimit,FactorySetingLowLimit;
    double UserSetingUpLimit,UserSetingLowLimit;
    double FengSu,AirSv1,AirSv2,AirSv3;

    m_scc->val1 = 0;
    int rt = m_scc->m_rtAS;

    m_dbm = new DbManager(m_dbPath, "calculate_AS");
    QString strQuery = "select val from confNew where name = \"10V-L\"";
    queryRes = m_dbm->query(strQuery);
    queryRes->next();
    fASL = queryRes->value("val").toDouble();
    m_fRTAS = fASL;

    FactorySetingLowLimit = GetParameter("\"10V-L\"");
    FactorySetingUpLimit = GetParameter("\"10V-H\"");
    UserSetingLowLimit   = GetParameter("\"AirU-r\"");
    UserSetingUpLimit    = GetParameter("\"AirF-r\"");

    AirSv1    = GetParameter("\"AirSv1\"");
    AirSv2    = GetParameter("\"AirSv2\"");
    AirSv3    = GetParameter("\"AirSv3\"");

    queryRes->clear();
    delete queryRes;
    m_dbm->close();

    FengSu = (rt - FactorySetingLowLimit) / (FactorySetingUpLimit - FactorySetingLowLimit) * (UserSetingUpLimit - UserSetingLowLimit);
    if(FengSu < 0) FengSu = 0;
    ui->airSpeedRT->setText(QString::number(FengSu,'f',2));
    ui->airSpeedSet->setText(QString::number(AirSv1,'f',2));

    //ui->airSpeedRT->setText(QString::number(0.2%,rt));
}



void MainWindow::on_maxAirSpeedBtn_clicked()
{

}
