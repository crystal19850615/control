#include "keypad.h"
#include "ui_keypad.h"

KeyPad::KeyPad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyPad)
{
    ui->setupUi(this);
    ui->num1->setFocusPolicy(Qt::NoFocus);
    ui->num2->setFocusPolicy(Qt::NoFocus);
    ui->num3->setFocusPolicy(Qt::NoFocus);
    ui->num4->setFocusPolicy(Qt::NoFocus);
    ui->num5->setFocusPolicy(Qt::NoFocus);
    ui->num6->setFocusPolicy(Qt::NoFocus);
    ui->num7->setFocusPolicy(Qt::NoFocus);
    ui->num8->setFocusPolicy(Qt::NoFocus);
    ui->num9->setFocusPolicy(Qt::NoFocus);
    ui->num0->setFocusPolicy(Qt::NoFocus);
    ui->backButton->setFocusPolicy(Qt::NoFocus);
    ui->dotButton->setFocusPolicy(Qt::NoFocus);
    ui->negButton->setFocusPolicy(Qt::NoFocus);
}

KeyPad::~KeyPad()
{
    delete ui;
}

void KeyPad::sendKeyEvent(QObject *receiver, QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text)
{
    QKeyEvent *keyEvent = new QKeyEvent(type, key, modifiers, text);
    qApp->postEvent((QObject*)receiver, (QEvent*)keyEvent);
}

void KeyPad::on_num1_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_1, Qt::NoModifier, "1");
}

void KeyPad::on_num2_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_2, Qt::NoModifier, "2");
}

void KeyPad::on_num3_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_3, Qt::NoModifier, "3");
}

void KeyPad::on_num4_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_4, Qt::NoModifier, "4");
}

void KeyPad::on_num5_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_5, Qt::NoModifier, "5");
}

void KeyPad::on_num6_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_6, Qt::NoModifier, "6");
}

void KeyPad::on_num7_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_7, Qt::NoModifier, "7");
}

void KeyPad::on_num8_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_8, Qt::NoModifier, "8");
}

void KeyPad::on_num9_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_9, Qt::NoModifier, "9");
}

void KeyPad::on_dotButton_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier, ".");
}

void KeyPad::on_num0_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_0, Qt::NoModifier, "0");
}

void KeyPad::on_backButton_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
}

void KeyPad::on_negButton_clicked()
{
    QWidget *widget = QApplication::focusWidget();
    sendKeyEvent((QObject*)widget, QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier, "-");
}

void KeyPad::on_OKButton_clicked()
{

}

void KeyPad::on_exitButton_clicked()
{

}
