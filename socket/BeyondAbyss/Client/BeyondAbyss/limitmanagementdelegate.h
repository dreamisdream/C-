#ifndef LIMITMANAGEMENTDELEGATE_H
#define LIMITMANAGEMENTDELEGATE_H

#include   <QItemDelegate>
#include   <QComboBox>
class LimitManagementDelegate : public QItemDelegate
{
public:
    LimitManagementDelegate(QObject* parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // LIMITMANAGEMENTDELEGATE_H
