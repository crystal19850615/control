#ifndef PASSWDDIAG_H
#define PASSWDDIAG_H

#include <QDialog>
#include <QMessageBox>
#include <qdebug.h>
#include "keypad.h"

namespace Ui {
class PasswdDiag;
}

class PasswdDiag : public QDialog
{
    Q_OBJECT

public:
    explicit PasswdDiag(QWidget *parent = 0);
    ~PasswdDiag();
    int m_passwd;

private slots:
    void on_buttonBox_accepted();
    void on_exitButton_clicked();

private:
    Ui::PasswdDiag *ui;
};

#endif // PASSWDDIAG_H
