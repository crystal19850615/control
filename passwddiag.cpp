#include "passwddiag.h"
#include "ui_passwddiag.h"

PasswdDiag::PasswdDiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswdDiag)
{
    ui->setupUi(this);
    move(0, 427);
    ui->pushButton->hide();
    KeyPad *keyPad = new KeyPad(this);
    keyPad->move(0, 100);
    QPushButton *okButton = keyPad->findChild<QPushButton *>("OKButton");
    QPushButton *exitButton = keyPad->findChild<QPushButton *>("exitButton");
    connect(okButton, SIGNAL(clicked()), this, SLOT(on_buttonBox_accepted()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(on_exitButton_clicked()));
    keyPad->show();
}

void PasswdDiag::on_exitButton_clicked()
{
    this->done(0);
}

PasswdDiag::~PasswdDiag()
{
    delete ui;
}

void PasswdDiag::on_buttonBox_accepted()
{
    bool ok;
    int resInt = ui->lineEdit->text().toInt(&ok);
    if (ok)
    {
        qDebug() << "PasswdDiag::on_buttonBox_accepted this->m_passwd: " << this->m_passwd;
        this->m_passwd = resInt;
        this->done(1);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Input an invalid Value...");
        msgBox.exec();
        this->m_passwd = -1;
        this->done(0);
    }
}
