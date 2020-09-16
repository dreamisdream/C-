#include "recyclebin.h"
#include <QDebug>


RecycleBin::RecycleBin(QWidget *parent) : QWidget(parent)
{
//    getData();
    run();

}

void RecycleBin::run()
{
    recycleBinTitleList();//回收标题栏
    recycleBinContentList();//回收站列表
}

void RecycleBin::initData()
{
    for(int i = 0;i<RecycleBinModel->rowCount();i++)
    {


    }
}

void RecycleBin::recycleBinTitleList()
{
    RecycleBinTitle = new QWidget(this);
    RecycleBinTitle->setGeometry(QRect(0, 0, 810, 40));
    RecycleBinTitle->setStyleSheet(QStringLiteral("background-color: #CCCCCC;"));

    //清空按钮
    button = new QPushButton(RecycleBinTitle);
    button->setObjectName(QStringLiteral("Button_clearAll"));
    button->setGeometry(QRect(700, 8, 85, 20));
    button->setFlat(true);
    button->setText("清空回收站");
    button->setFont(QFont("Microsoft YaHei", 12, 50));

    //回收站标签
    label = new QLabel(RecycleBinTitle);
    label->setObjectName(QStringLiteral("LabelRecycleBIn"));
    label->setGeometry(QRect(30, 7, 75, 30));
    label->setText("回收站");
    label->setFont(QFont("Microsoft YaHei", 12, 50));

}

void RecycleBin::recycleBinContentList()
{

    //列表
    RecycleBinView = new QTableView(this);
    RecycleBinView->setGeometry(QRect(0, 40, 810, 530));
//    RecycleBinView->setStyleSheet(QStringLiteral("background-color: rgb(125, 125, 225);"));

    rowCount = vectorCycleBinFile.size();
    qDebug()<<"vectorCycleBinFile.size() ="<<vectorCycleBinFile.size();

    RecycleBinModel = new QStandardItemModel(rowCount,4,this);
    RecycleBinView->setModel(RecycleBinModel);
    RecycleBinModel->setHorizontalHeaderItem(0,new QStandardItem("文件名"));
    RecycleBinModel->setHorizontalHeaderItem(1,new QStandardItem("大小"));
    RecycleBinModel->setHorizontalHeaderItem(2,new QStandardItem("删除时间"));
    RecycleBinModel->setHorizontalHeaderItem(3,new QStandardItem("有效时间"));


    //设置默认行高
    RecycleBinView->verticalHeader()->setDefaultSectionSize(45);

    //取消网格线
    RecycleBinView->setShowGrid(false);

    //隐藏行表头
    RecycleBinView->verticalHeader()->hide();


    //内容不可编辑
    RecycleBinView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //表格整行选中
    RecycleBinView->setSelectionBehavior(QAbstractItemView::SelectRows);

//    initData();
    insertTableView();
    RecycleBinView->setColumnWidth(0,300);
    RecycleBinView->setColumnWidth(1,170);
    RecycleBinView->setColumnWidth(2,170);
    RecycleBinView->setColumnWidth(3,170);
}

void RecycleBin::insertTableView()
{
    for(int row=0;row<RecycleBinModel->rowCount();row++)
    {
        allFileInfo = vectorCycleBinFile[row];
        insertEachLine(row, allFileInfo);
    }
}

void RecycleBin::insertEachLine(int row, FileInfoStruct &file_info)
{

    QModelIndex index = RecycleBinModel->index(row,0);
    RecycleBinFileInfo* info = new RecycleBinFileInfo(this,file_info);
    connect(info,SIGNAL(DeleteButtonSIgnal(int)),this,SLOT(recyDeleteFileSlot(int)));
    connect(info,SIGNAL(recoveButtonSignal(int)),this,SLOT(recyRecoverFileSlot(int)));
    RecycleBinView->setIndexWidget(index,info);

    QStandardItem* item = new QStandardItem(transformFileSize(file_info.fileSize));
    RecycleBinModel->setItem(row,1,item);

    QStandardItem* item1 = new QStandardItem(file_info.deleteTime);
    RecycleBinModel->setItem(row,2,item1);
//有效时间
    QStandardItem* item2 = new QStandardItem(file_info.deleteTime);
    RecycleBinModel->setItem(row,3,item2);
}

QString RecycleBin::transformFileSize(qint64 fileSize)
{
    QStringList units;
        units << "Bytes" << "KB" << "MB" << "GB" << "TB" << "PB";
        double mod  = 1024.0;
        double size = fileSize;
        //qDebug() << size;
        int i = 0;
        long rest = 0;
        while (size >= mod && i < units.count()-1 )
        {
            rest= (long)size % (long)mod ;
            size /= mod;
            i++;
        }
        QString szResult = QString::number(floor(size));
        if( rest > 0)
        {
           szResult += QString(".") + QString::number(rest).left(2);
        }
        szResult += units[i];
        return  szResult;
}

void RecycleBin::setFileModel()
{
    rowCount = vectorCycleBinFile.size();
    RecycleBinModel = new QStandardItemModel(rowCount,4,this);
    RecycleBinView->setModel(RecycleBinModel);
}

void RecycleBin::showTableView()
{
    //设置表头显示信息
    RecycleBinModel->setHorizontalHeaderItem(0,new QStandardItem("文件名"));
    RecycleBinModel->setHorizontalHeaderItem(1,new QStandardItem("大小"));
    RecycleBinModel->setHorizontalHeaderItem(2,new QStandardItem("删除时间"));
    RecycleBinModel->setHorizontalHeaderItem(3,new QStandardItem("有效时间"));

    //设置默认行高
    RecycleBinView->verticalHeader()->setDefaultSectionSize(45);

    //取消网格线
    RecycleBinView->setShowGrid(false);

    //隐藏行表头
    RecycleBinView->verticalHeader()->hide();

    //内容不可编辑
    RecycleBinView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //表格整行选中
    RecycleBinView->setSelectionBehavior(QAbstractItemView::SelectRows);

//    //整行选中的关联事件
//    connect(fileListView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectRow(QModelIndex)));

    //tableView  每一列设置指定列宽，应放在setModel() 和 插入数据之后。
    RecycleBinView->setColumnWidth(0,300);
    RecycleBinView->setColumnWidth(1,170);
    RecycleBinView->setColumnWidth(2,170);
    RecycleBinView->setColumnWidth(3,170);
    RecycleBinView->show();
}

void RecycleBin::getData()
{
    vectorCycleBinFile.clear();
    for(int i =0;i<10;i++)
    {
        FileInfoStruct temp;
        vectorCycleBinFile.push_back(temp);
    }
}
//中转删除信号
void RecycleBin::recyDeleteFileSlot(int file_id)
{

    emit recyDeleteFileSignal(file_id);
}


//中转恢复信号
void RecycleBin::recyRecoverFileSlot(int file_id)
{
    emit recyRecoverFileSignal(file_id);
}

//收到服务器返回回收站数据
void RecycleBin::responseFileListDataSlot(SelectFilesReply* allData)
{
    if(allData->file_status==0)
    {
        qDebug()<<"文件列表界面收到服务器返回数据……";
        int len = allData->len;
        vectorCycleBinFile.clear();
        for(int i=0;i<len;i++)
        {
            vectorCycleBinFile.push_back(allData->file[i]);
        }
        setFileModel();
        insertTableView();
        showTableView();
    }
}

//删除文件收到服务器的状态
void RecycleBin::recycleDeleteReplySlot(RecycleDeleteFileReply *allData)
{
    if(allData->succeed)
    {
        emit requestRecycleData();
    }
}

void RecycleBin::recycleRecoverReplySlot(RecycleRecoverFileReply *allData)
{
    if(allData->succeed)
    {
        emit requestRecycleData();
        emit requestFilelistSignal();
    }
}
