#include "limitsmanagement.h"

LimitsManagement::LimitsManagement(QWidget *parent) : QWidget(parent)
{
    run();
}

void LimitsManagement::run()
{
    limitList();
    limitContent();
}

void LimitsManagement::limitList()
{
    //权限设置窗口
    Frame_Set = new QFrame(this);
    Frame_Set->setObjectName("Frame_Set");
    Frame_Set->setGeometry(QRect(200,0,900,570));
    Frame_Set->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

    //权限分类窗口
    Frame_Sort = new QFrame(this);
    Frame_Sort->setObjectName("Frame_Sort");
    Frame_Sort->setGeometry(QRect(200,0,900,570));
    Frame_Sort->setStyleSheet(QStringLiteral("background-color: rgb(255, 255,255);"));

    //权限分类按钮
    button_Sort = new QPushButton(this);
    button_Sort->setGeometry(QRect(0,40,200,40));
    button_Sort->setStyleSheet(QStringLiteral("#ffffff"));
    button_Sort->setText("权限分类");
    button_Sort->setFont(QFont("Microsoft YaHei", 12, 50));
    connect(button_Sort,SIGNAL(clicked()),this,SLOT(toSortManagement()));

    //权限设置按钮
    button_Set = new QPushButton(this);
    button_Set->setGeometry(QRect(0,0,200,40));
    button_Set->setText("权限设置");
    button_Set->setFont(QFont("Microsoft YaHei", 12, 50));
    button_Set->setStyleSheet(QStringLiteral("#ffffff"));
    connect(button_Set,SIGNAL(clicked()),this,SLOT(toSetManagement()));
}

void LimitsManagement::limitContent()
{

    m_model= new QStandardItemModel( 8,8,Frame_Set);
    delegate = new LimitManagementDelegate(this);
    for(int i=0;i<=8;i++)
    {
        for(int j =1;j<=8;++j)
        {
            m_model->setItem(i,j,new QStandardItem(QString::number(10)));
        }
    }
    tableView  = new QTableView(Frame_Set);

    //取消网格线
    tableView->setShowGrid(false);

    //隐藏行表头
    tableView->verticalHeader()->hide();

    tableView->resize(Frame_Set->size());
    tableView->setModel(m_model);
    m_model->setHorizontalHeaderItem(0,new QStandardItem("用户角色"));
    m_model->setHorizontalHeaderItem(1,new QStandardItem("上传文件权限"));
    m_model->setHorizontalHeaderItem(2,new QStandardItem("下载文件权限"));
    m_model->setHorizontalHeaderItem(3,new QStandardItem("删除文件权限"));
    m_model->setHorizontalHeaderItem(4,new QStandardItem("查看日志权限"));
    m_model->setHorizontalHeaderItem(5,new QStandardItem("分享文件权限"));
    m_model->setHorizontalHeaderItem(6,new QStandardItem("删除文件权限"));
    m_model->setHorizontalHeaderItem(7,new QStandardItem("查看日志权限"));
    m_model->setHorizontalHeaderItem(8,new QStandardItem("分享文件权限"));
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setItemDelegateForColumn(0,delegate);
}

void LimitsManagement::toSetManagement()
{
    Frame_Set->show();
    Frame_Sort->hide();

}

void LimitsManagement::toSortManagement()
{
    Frame_Set->hide();
    Frame_Sort->show();
}




