#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "dbmanager.h"
#include "editablesqlmodel.h"
#include "controldelegate.h"
#include "keypad.h"
#include <qdebug.h>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    move(0, 0);
    m_hasAuthorized = false;
    ui->stackedWidget->setGeometry(20, 80, 440, 460);
    ui->pushButton->hide();
    KeyPad *keyPad = new KeyPad(this);
    keyPad->move(0, 580);
    QPushButton *okButton = keyPad->findChild<QPushButton *>("OKButton");
    QPushButton *exitButton = keyPad->findChild<QPushButton *>("exitButton");
    connect(okButton, SIGNAL(clicked()), this, SLOT(on_submitButton_clicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(on_exitButton_clicked()));

    keyPad->show();
    QComboBox *pageComboBox = new QComboBox(this);
    m_pageComboBox = pageComboBox;
    pageComboBox->setObjectName("pageCombo");
    pageComboBox->setMaxVisibleItems(20);
    pageComboBox->setGeometry(0, 40, 480, 40);

    QMap<double, QString> paramMap;
    paramMap[0] = "4-20Ma";
    paramMap[1] = "0-10mA";
    paramMap[2] = "0-20mA";
    paramMap[3] = "1-5v";
    paramMap[4] = "0-5v";
    paramMap[5] = "0-10v";
    m_dataDispMap["OP1"] = paramMap;
    paramMap.clear();
    paramMap[0] = "4-20Ma";
    paramMap[1] = "0-10mA";
    paramMap[2] = "0-20mA";
    paramMap[3] = "1-5v";
    paramMap[4] = "0-5v";
    paramMap[5] = "0-10v";
    m_dataDispMap["OP2"] = paramMap;
    paramMap.clear();
    paramMap[0] = "2400";
    paramMap[1] = "4800";
    paramMap[2] = "9600";
    paramMap[3] = "19200";
    m_dataDispMap["Baud"] = paramMap;
    paramMap.clear();
    paramMap[0] = "H";
    paramMap[1] = "L";
    m_dataDispMap["TemRLMod"] = paramMap;
    paramMap.clear();

    QSqlQuery *queryRes;
    m_dbm = new DbManager(m_dbPath, "queryOA");
    queryRes = m_dbm->query(m_strQueryOA);
    queryRes->next();
    m_oAValue = queryRes->value("val").toInt();
    qDebug() << "oAValue: " << m_oAValue;
    queryRes->clear();
    delete queryRes;
    m_dbm->close();
    delete m_dbm;

    db = QSqlDatabase::addDatabase("QSQLITE", "settingConn");
    db.setDatabaseName(m_dbPath);
    if(!db.open())
    {
        qDebug() << "Error: connection with confData fail...";
    }

    for (int i = 0; i < 12; i++)
    {
        QWidget *page = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(page);

        EditableSqlModel *model = new EditableSqlModel(parent, db);
        model->m_dataDispMap = &m_dataDispMap;
        model->setTable("confNew");
        model->setFilter("groupID = " + QString::number(i));
        model->setEditStrategy(EditableSqlModel::OnManualSubmit);
        model->select();

        ControlDelegate *delegate = new ControlDelegate(page);
        delegate->m_dataDispMap = &m_dataDispMap;

        QTableView *tableView = new QTableView();

        QHeaderView *verticalHeader = tableView->verticalHeader();
        verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
        verticalHeader->setDefaultSectionSize(40);

        tableView->setModel(model);
        tableView->setItemDelegate(delegate);
        tableView->hideColumn(0);
        tableView->hideColumn(2);
        tableView->hideColumn(4);
        tableView->hideColumn(5);
        tableView->hideColumn(6);
        tableView->hideColumn(7);
        tableView->hideColumn(8);
        tableView->hideColumn(9);
        tableView->hideColumn(10);

        layout->addWidget(tableView);
        ui->stackedWidget->addWidget(page);
        pageComboBox->addItem("Group" + QString::number(i + 1));
        connect(pageComboBox, SIGNAL(activated(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    }
    connect(pageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_pageCombo_currentIndexChanged(int)));
}

SettingDialog::~SettingDialog()
{
    delete this->ui->stackedWidget;
    QString connName = db.connectionName();
    if (db.isOpen())
    {
        db.close();
    }
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);
    delete ui;
}

void SettingDialog::on_pageCombo_currentIndexChanged(int index)
{
    qDebug() << "SettingDialog::on_pageCombo_currentIndexChanged index: " << index;
    if (index > 2)
    {
        if (!m_hasAuthorized)
        {
            PasswdDiag passwdDiag;
            passwdDiag.setModal(true);
            int diagCode = passwdDiag.exec();
            qDebug() << "diagCode: " << diagCode;
            if (diagCode == 1)
            {
                qDebug() << "passwdDiag.m_passwd" << passwdDiag.m_passwd;
                qDebug() << "this->m_oAValue" << this->m_oAValue;
                if (passwdDiag.m_passwd != this->m_oAValue)
                {
                    QMessageBox msgBox;
                    msgBox.setText("Password is incorrect...");
                    msgBox.exec();
                    m_pageComboBox->setCurrentIndex(0);
                }
                else
                {
                    m_hasAuthorized = true;
                }
            }
            else
            {
                m_pageComboBox->setCurrentIndex(0);
            }
        }
    }
}

void SettingDialog::on_submitButton_clicked()
{
    QTableView *tableView = ui->stackedWidget->currentWidget()->findChild<QTableView *>();

    EditableSqlModel *model = static_cast<EditableSqlModel*>(tableView->model());

    model->database().transaction();
    if (model->submitAll())
    {
        bool bCommitRes = model->database().commit();
        if (!bCommitRes)
        {
            qDebug() << "SettingDialog::on_submitButton_clicked bCommitRes: " << bCommitRes;
        }
    }
    else
    {
        if (model->lastError().isValid())
        {
            qDebug() << "model->submitAll failed: " << model->lastError();
        }
        else
        {
            qDebug() << "model->submitAll failed...";
        }
        model->database().rollback();
    }
}

void SettingDialog::on_revertButton_clicked()
{
    QTableView *tableView = ui->stackedWidget->currentWidget()->findChild<QTableView *>();

    EditableSqlModel *model = static_cast<EditableSqlModel*>(tableView->model());
    model->revertAll();
}

void SettingDialog::on_exitButton_clicked()
{
    this->done(0);
}

void SettingDialog::on_pushButton_clicked()
{

}
