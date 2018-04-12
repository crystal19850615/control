#include <QtSql>

#include "editablesqlmodel.h"

EditableSqlModel::EditableSqlModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

Qt::ItemFlags EditableSqlModel::flags(
        const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlTableModel::flags(index);

    if (index.column() != 3)
        flags &= ~Qt::ItemIsEditable;
    return flags;
}

QVariant EditableSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index, role);

    if (role == Qt::FontRole && index.isValid())
    {
        return QFont("Cantarell", 16);
    }
    if (role == Qt::TextColorRole && index.column() == 1)
        return QVariant::fromValue(QColor(Qt::blue));
    if (role == Qt::DisplayRole && index.column() == 3)
    {
        QModelIndex typeIndex = index.sibling(index.row(), 2);
        QModelIndex nameIndex = index.sibling(index.row(), 1);

        QVariant dataType = index.model()->data(typeIndex, Qt::EditRole);
        for (QMap<QString, QMap<double, QString>>::const_iterator i = this->m_dataDispMap->constBegin(); i != this->m_dataDispMap->constEnd(); i++)
        {
            if (index.model()->data(nameIndex) == i.key())
            {
                QVariant mapIndexVal = index.model()->data(index, Qt::EditRole);
                double value = mapIndexVal.toDouble();
                return i.value()[value];
            }
        }
        if (dataType == 2)
        {
            if (value == 0)
            {
                return "OFF";
            }
            else
            {
                return "ON";
            }
        }
    }
    return value;
}
