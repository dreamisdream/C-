#ifndef LIMITSMANAGEMENT_H
#define LIMITSMANAGEMENT_H
#include "limitmanagementdelegate.h"
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QWindow>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QSpinBox>
#include <QComboBox>
#include <QTableView>
#include <QHeaderView>
#include <QItemDelegate>

class LimitsManagement : public QWidget
{
    Q_OBJECT
public:
    explicit LimitsManagement(QWidget *parent = nullptr);
    void run();
    void limitList();
    void limitContent();


private:
    QFrame      *Frame_List;
    QFrame      *Frame_Set;
    QFrame      *Frame_Sort;
    QPushButton * button_Set;
    QPushButton * button_Sort;
    QStandardItemModel* m_model;
    QTableView  * tableView;
    LimitManagementDelegate * delegate;


    QLabel      * namePicture;
    QLabel      * nameTitle;
    QFrame      * nameContent;
    QPushButton * button;

signals:

public slots:
    void toSetManagement();
    void toSortManagement();

};

#endif // LIMITSMANAGEMENT_H
