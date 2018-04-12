#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "settingdialog.h"
#include "dbmanager.h"
#include "srvrclntcomm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double GetParameter(const QString &ParameterName);

protected:
    bool eventFilter(QObject *watched, QEvent *event);


private:
    Ui::MainWindow *ui;
    const QString m_dbPath = "confData";
    const QString m_strQueryAirSpeedSet = "select val from confNew where name = \"AirSv1\" and dataType = 1 limit 1";
    DbManager *m_dbm;
    SettingDialog *settingDiag = NULL;
    int m_countOfFailed = 0;
    SrvClntComm *m_scc;
    double m_fRTAS;

private slots:
    void update();
    void on_settingButton_clicked();
    void calculate_AS();
};

#endif // MAINWINDOW_H
