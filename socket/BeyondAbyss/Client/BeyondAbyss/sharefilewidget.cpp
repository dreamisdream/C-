#include "sharefilewidget.h"
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QDebug>

ShareFileWidget::ShareFileWidget(QWidget *parent) : QWidget(parent)
{
    init();
    connect(m_pBtnUpdata,&QPushButton::clicked,this,&ShareFileWidget::updataInfo);
    connect (m_tabWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
}

void ShareFileWidget::init()
{
    this->setGeometry(QRect(200,0,900,570));
    this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);
    QVBoxLayout *m_Lyout = new QVBoxLayout(this);
    m_pBtnUpdata = new QPushButton("点击刷新数据");
    m_Lyout->addWidget(m_pBtnUpdata,Qt::AlignLeft);
    this->setLayout(m_Lyout);
    m_tabWidget = new QTableWidget(this);
    m_Lyout->addWidget(m_tabWidget);
    m_tabWidget->setColumnCount(3);
    m_tabWidget->setEditTriggers(0);
    m_tabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList headList;
    headList<<"userId"<<"fileId"<<"file_code";
    m_tabWidget->setHorizontalHeaderLabels(headList);
    m_tabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    m_tabWidget->setSelectionMode();
    m_tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}


void ShareFileWidget::appendRow(int x, int y, QString z)
{
    int row = m_tabWidget->rowCount();
    static int i=0;
    if (i<row){
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(x));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(y));
        QTableWidgetItem *item3 = new QTableWidgetItem(z);
        m_tabWidget->setItem(i,0, item1);
        m_tabWidget->setItem(i,1, item2);
        m_tabWidget->setItem(i,2, item3);
        i++;
    }else
        i=0;
}

void ShareFileWidget::updataInfo()
{
    emit updateShareInfo();
}

void ShareFileWidget::customMenuRequested(QPoint pos)
{
    selecteditem = m_tabWidget->itemAt(pos) ;	//get right click pos item

    QMenu* menu = new QMenu(this);
    QAction * action = new QAction(tr("&DownLoadFile"),this);
    action->setData("下载文件")	;	// set data, for later in slot, we can get necessary info
    menu->addAction(action);
    menu->popup(m_tabWidget->viewport()->mapToGlobal(pos)) ; 		//show the menu at right click pos
    connect(action,&QAction::triggered,this,&ShareFileWidget::downloadSlot);
//    connect (action, SIGNAL(triggered()), this, SLOT(download));
}

void ShareFileWidget::downloadSlot()
{
    QAction* action = qobject_cast<QAction*> (sender());
    if (action == 0) return;

//    qDebug() << selecteditem->text();
    QList<QTableWidgetItem*> items = m_tabWidget->selectedItems();
    int count = items.count();

    for(int i = 0; i < count; i++)
    {
        int row = m_tabWidget->row(items.at(i));
        QTableWidgetItem *item = items.at(i);
        QString text = item->text(); //获取内容
    }


}
