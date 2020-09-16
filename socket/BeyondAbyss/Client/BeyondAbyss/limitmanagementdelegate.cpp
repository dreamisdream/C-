#include "limitmanagementdelegate.h"

LimitManagementDelegate::LimitManagementDelegate(QObject* parent)
{

}

QWidget *LimitManagementDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);

    editor->addItem("超级管理员");
    editor->addItem("管理员");
    editor->addItem("钻石用户");
    editor->addItem("黄金用户");
    editor->addItem("白银用户");
    editor->addItem("普通用户");

    return editor;
}

void LimitManagementDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(str);

}

void LimitManagementDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString str = comboBox->currentText();
    model->setData(index, str, Qt::EditRole);
}

void LimitManagementDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
