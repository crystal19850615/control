#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QtSql>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QKeyEvent>
#include "dbmanager.h"
#include "passwddiag.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    QMap<QString, QMap<double, QString>> m_dataDispMap;

    const QString m_dbPath = "confData";

private slots:
    void on_pageCombo_currentIndexChanged(int index);

    void on_submitButton_clicked();

    void on_revertButton_clicked();

    void on_exitButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::SettingDialog *ui;
    QSqlDatabase db;
    DbManager *m_dbm;
    QComboBox *m_pageComboBox;
    int m_oAValue;
    bool m_hasAuthorized;
    const QString m_strQueryOA = "select val from confNew where name = \"oA\" and dataType = 0 limit 1";
};

#endif // TESTDIALOG_H
