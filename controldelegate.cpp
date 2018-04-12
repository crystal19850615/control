#include "controldelegate.h"

ControlDelegate::ControlDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget* ControlDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QModelIndex typeIndex = index.sibling(index.row(), 2);
    QModelIndex nameIndex = index.sibling(index.row(), 1);

    for (QMap<QString, QMap<double, QString>>::const_iterator i = this->m_dataDispMap->constBegin(); i != this->m_dataDispMap->constEnd(); i++)
    {
        if (index.model()->data(nameIndex) == i.key())
        {
            QStringList choices;
            for (QMap<double, QString>::const_iterator j = i.value().constBegin(); j != i.value().constEnd(); j++)
            {
                choices << j.value();
            }
            QComboBox *editor = new QComboBox(parent);
            editor->addItems(choices);
            return editor;
        }
    }
    if (index.model()->data(typeIndex) == 2)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("OFF");
        editor->addItem("ON");
        return editor;
    }
    else
    {
        QLineEdit *editor = new QLineEdit(parent);
        if (index.model()->data(typeIndex) == 0)
        {
            KeyEventFilter *filter = new KeyEventFilter(parent);
            editor->installEventFilter(filter);
        }
        return editor;
    }
}

void ControlDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QModelIndex typeIndex = index.sibling(index.row(), 2);
    QModelIndex nameIndex = index.sibling(index.row(), 1);

    for (QMap<QString, QMap<double, QString>>::const_iterator i = this->m_dataDispMap->constBegin(); i != this->m_dataDispMap->constEnd(); i++)
    {
        if (index.model()->data(nameIndex) == i.key())
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            QVariant indexVal = index.model()->data(index, Qt::EditRole);
            int value = indexVal.toInt();
            comboBox->setCurrentIndex(value);
            return;
        }
    }
    if (index.model()->data(typeIndex) == 2)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QVariant indexVal = index.model()->data(index, Qt::EditRole);
        int value = indexVal.toInt();
        comboBox->setCurrentIndex(value);
        return;
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
        return;
    }
}

void ControlDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QModelIndex typeIndex = index.sibling(index.row(), 2);
    QModelIndex nameIndex = index.sibling(index.row(), 1);
    QModelIndex minIndex = index.sibling(index.row(), 5);
    QModelIndex maxIndex = index.sibling(index.row(), 6);
    QVariant minValue = index.model()->data(minIndex, Qt::EditRole);
    QVariant maxValue = index.model()->data(maxIndex, Qt::EditRole);

    for (QMap<QString, QMap<double, QString>>::const_iterator i = this->m_dataDispMap->constBegin(); i != this->m_dataDispMap->constEnd(); i++)
    {
        if (index.model()->data(nameIndex) == i.key())
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            double value = comboBox->currentIndex();
            model->setData(index, value, Qt::EditRole);
            return;
        }
    }
    if (index.model()->data(typeIndex) == 2)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        double value = comboBox->currentIndex();
        if (value <= maxValue.toDouble() && value >= minValue.toDouble())
        {
            model->setData(index, value);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Invalid Value...");
            msgBox.exec();
        }
    }
    else
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString strVal = lineEdit->text();
        bool ok;
        double value = strVal.toDouble(&ok);
        if (ok)
        {
            if (value <= maxValue.toDouble() && value >= minValue.toDouble())
            {
                model->setData(index, value, Qt::EditRole);
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("Invalid Value...");
                msgBox.exec();
            }
        }
    }
}
