#ifndef CONTROLDELEGATE_H
#define CONTROLDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QModelIndex>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QEvent>
#include <QMessageBox>

#include "keyeventfilter.h"

class ControlDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit ControlDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QMap<QString, QMap<double, QString>> *m_dataDispMap;
};

#endif // CONTROLDELEGATE_H
