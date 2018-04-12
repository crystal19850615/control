#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QKeyEvent>

class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit KeyEventFilter(QObject *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
signals:

public slots:
};

#endif // KEYEVENTFILTER_H
