#ifndef KEYPAD_H
#define KEYPAD_H

#include <QWidget>
#include <QKeyEvent>
#include <QEvent>

namespace Ui {
class KeyPad;
}

class KeyPad : public QWidget
{
    Q_OBJECT

public:
    explicit KeyPad(QWidget *parent = 0);
    ~KeyPad();
    void sendKeyEvent(QObject *receiver, QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text = QString());

private slots:
    void on_num1_clicked();

    void on_num2_clicked();

    void on_num3_clicked();

    void on_num4_clicked();

    void on_num5_clicked();

    void on_num6_clicked();

    void on_num7_clicked();

    void on_num8_clicked();

    void on_num9_clicked();

    void on_dotButton_clicked();

    void on_num0_clicked();

    void on_backButton_clicked();

    void on_negButton_clicked();

    void on_OKButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::KeyPad *ui;
};

#endif // KEYPAD_H



