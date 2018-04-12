#include "keyeventfilter.h"

KeyEventFilter::KeyEventFilter(QObject *parent) : QObject(parent)
{

}

bool KeyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Period)
        {
            return true;
        }
    }
}
