#include "filelistwidget.h"
#include "sharefiledialog.h"

FileListWidget::FileListWidget(QWidget *parent) : QWidget(parent)
{
   this->setGeometry(QRect(0, 0, 900, 570));
   init();
}

void FileListWidget::init()
{
    //图片路径初始化
    initImagePath();

    //其他参数初始化
    initOtherInfo();

    //设置菜单栏
    setFileListMenu();

    //设置标题栏
    setFileListTitle();

    //文件列表区域
    initFileListView();
}

void FileListWidget::initFileListView()
{
    fileListView = new QTableView(this);
    fileListView->setGeometry(QRect(0, 80, 900, 510));
//    getAllData();
    setFileModel();
    insertTableView();
    showTableView();
//    refreshButtonClicked();
}

void FileListWidget::showTableView()
{
    //设置表头显示信息
    fileListModel->setHorizontalHeaderItem(0,new QStandardItem("文件名"));
    fileListModel->setHorizontalHeaderItem(1,new QStandardItem("修改时间"));
    fileListModel->setHorizontalHeaderItem(2,new QStandardItem("文件大小"));
    fileListModel->setHorizontalHeaderItem(3,new QStandardItem("--"));

    //设置默认行高
    fileListView->verticalHeader()->setDefaultSectionSize(45);

    //取消网格线
    fileListView->setShowGrid(false);

    //隐藏行表头
    fileListView->verticalHeader()->hide();

    //内容不可编辑
    fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //表格整行选中
    fileListView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //整行选中的关联事件
    connect(fileListView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectRow(QModelIndex)));

    //tableView  每一列设置指定列宽，应放在setModel() 和 插入数据之后。
    fileListView->setColumnWidth(0,400);
    fileListView->setColumnWidth(1,200);
    fileListView->setColumnWidth(2,150);
    fileListView->setColumnWidth(3,150);
    fileListView->show();
}

void FileListWidget::setFileModel()
{
    rowCount = vectorFileInfo.size();
    fileListModel = new QStandardItemModel(rowCount,4,this);
    fileListView->setModel(fileListModel);
}

void FileListWidget::insertTableView()
{
    for(int row=0;row<fileListModel->rowCount();row++)
    {
        allFileInfo = vectorFileInfo[row];
        insertEachLine(row, allFileInfo);
    }
}

void FileListWidget::insertEachLine(int row, FileInfoStruct &file_info)
{
    QModelIndex index = fileListModel->index(row,0);
    FileInfo* info = new FileInfo(this,file_info);
    connect(info,SIGNAL(manageButtonsOperatorSignal(QString,int)),this,SLOT(transitSignal(QString,int)));
    fileListView->setIndexWidget(index,info);
    QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(file_info.nowTime));
    fileListModel->setItem(row,1,item);
    QStandardItem* item1 = new QStandardItem(transformFileSize(file_info.fileSize));
    fileListModel->setItem(row,2,item1);
    QStandardItem* item2 = new QStandardItem("--");
    fileListModel->setItem(row,3,item2);
}

void FileListWidget::initImagePath()
{
    QString path = ":/image/Filelist/";

    buttonImageMap["upload"] = path + "upload.png";
    buttonImageMap["download"] = path + "download.png";
    buttonImageMap["share"] = path + "myshare_normal.png";
    buttonImageMap["delete"] = path + "recycle_normal.png";
    buttonImageMap["create"] = path + "createdir.png";
    buttonImageMap["download"] = path + "download.png";
    buttonImageMap["sort"] = path + "sort.png";
    buttonImageMap["menu"] = path + "thumb.png";

    buttonImageMap["left"] = path + "back.png";
    buttonImageMap["right"] = path + "NextBtn.png";
    buttonImageMap["below"] = path + "drop.png";
    buttonImageMap["refresh"] = path + "refresh.png";
    buttonImageMap["search"] = path + "search.png";
}

void FileListWidget::initOtherInfo()
{
    searchTitle = "搜索我的网盘文件";
    currentPath = "D:/BeyondAbyss/20200607/BeyondAbyss/Server/ServerSocket";

}


//设置标题栏
void FileListWidget::setFileListTitle()
{
   titleWidget =  new QWidget(this);
   titleWidget->setObjectName(QStringLiteral("titleWidget"));
   titleWidget->setGeometry(QRect(0, 40, 900, 40));
   titleWidget->setStyleSheet(("background-color: rgb(255, 255, 255);"));

   this->installEventFilter(this);

   //上一级
   leftButton = new QPushButton(titleWidget);
   leftButton->setObjectName(QStringLiteral("leftButton"));
   leftButton->setGeometry(QRect(10, 10, 20, 20));
   leftButton->setFlat(true);
   leftButton->setIcon(getIcon("left"));
   connect(leftButton,SIGNAL(clicked()),this,SLOT(leftButtonClicked()));

   //下一级
   rightButton = new QPushButton(titleWidget);
   rightButton->setObjectName(QStringLiteral("rightButton"));
   rightButton->setGeometry(QRect(40, 10, 20, 20));
   rightButton->setFlat(true);
   rightButton->setIcon(getIcon("right"));
   connect(rightButton,SIGNAL(clicked()),this,SLOT(rightButtonClicked()));

   //下拉按钮
   belowButton = new QPushButton(titleWidget);
   belowButton->setObjectName(QStringLiteral("belowButton"));
   belowButton->setGeometry(QRect(70, 10, 20, 20));
   belowButton->setFlat(true);
   belowButton->setIcon(getIcon("below"));
   connect(belowButton,SIGNAL(clicked()),this,SLOT(belowButtonClicked()));

   //刷新
   refreshButton = new QPushButton(titleWidget);
   refreshButton->setObjectName(QStringLiteral("refreshButton"));
   refreshButton->setGeometry(QRect(99, 10, 20, 20));
   refreshButton->setFlat(true);
   refreshButton->setIcon(getIcon("refresh"));
   connect(refreshButton,SIGNAL(clicked()),this,SLOT(refreshButtonClicked()));

   //竖线
   line = new QFrame(titleWidget);
   line->setObjectName(QStringLiteral("line"));
   line->setGeometry(QRect(140, 10, 2, 20));
   line->setFrameShape(QFrame::VLine);
   line->setFrameShadow(QFrame::Sunken);

   //网盘按钮
   dbankButton = new QPushButton(titleWidget);
   dbankButton->setObjectName(QStringLiteral("dbankButton"));
   dbankButton->setGeometry(QRect(150, 10, 75, 20));
   dbankButton->setFlat(true);
   dbankButton->setText("我的网盘 >");
   connect(dbankButton,SIGNAL(clicked()),this,SLOT(dbankButtonClicked()));

   //竖线
   line_2 = new QFrame(titleWidget);
   line_2->setObjectName(QStringLiteral("line_2"));
   line_2->setGeometry(QRect(630, 10, 2, 20));
   line_2->setFrameShape(QFrame::VLine);
   line_2->setFrameShadow(QFrame::Sunken);

   //搜索框
//   searchLineEdit = new QLineEdit(titleWidget);
   searchLineEdit = new MyLineEdit(titleWidget);
   searchLineEdit->setObjectName(QStringLiteral("searchLineEdit"));
   searchLineEdit->setGeometry(QRect(650, 5, 200, 30));
   searchLineEdit->setText(searchTitle);
   searchLineEdit->setStyleSheet("QLineEdit{border-width:0;border-style:outset};");
    //
   connect(searchLineEdit,SIGNAL(mousePress()),this,SLOT(lineEditPress()));

   //搜索按钮
   searchButton = new QPushButton(titleWidget);
   searchButton->setObjectName(QStringLiteral("searchButton"));
   searchButton->setGeometry(QRect(860, 10, 20, 20));
   searchButton->setFlat(true);
   searchButton->setIcon(getIcon("search"));
   connect(searchButton,SIGNAL(clicked()),this,SLOT(searchButtonClicked()));
}


//设置菜单栏
void FileListWidget::setFileListMenu()
{
    menuWidget = new QWidget(this);
    menuWidget->setObjectName(QStringLiteral("menuWidget"));
    menuWidget->setGeometry(QRect(0, 0, 900, 40));
    menuWidget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

    //上传组合按钮
    uploadButton = new QPushButton(menuWidget);
    uploadButton->setObjectName(QStringLiteral("uploadButton"));
    uploadButton->setGeometry(QRect(8, 0, 60, 40));
    uploadButton->setFlat(true);
    uploadLabel = new QLabel(uploadButton);
    uploadLabel->setObjectName(QStringLiteral("uploadLabel"));
    uploadLabel->setGeometry(QRect(6, 5, 20, 30));
    uploadLabel->setPixmap(getPixmap("upload"));
    uploadTitle = new QLabel(uploadButton);
    uploadTitle->setObjectName(QStringLiteral("uploadTitle"));
    uploadTitle->setGeometry(QRect(26, 5, 30, 30));
    uploadTitle->setText("上传");
    connect(uploadButton,SIGNAL(clicked()),this,SLOT(uploadButtonClicked()));

    //下载组合按钮
    downloadButton = new QPushButton(menuWidget);
    downloadButton->setObjectName(QStringLiteral("downloadButton"));
    downloadButton->setGeometry(QRect(86, 0, 70, 40));
    downloadButton->setFlat(true);
    downloadLabel = new QLabel(downloadButton);
    downloadLabel->setObjectName(QStringLiteral("downloadLabel"));
    downloadLabel->setGeometry(QRect(6, 5, 20, 30));
    downloadLabel->setPixmap(getPixmap("download"));
    downloadTitle = new QLabel(downloadButton);
    downloadTitle->setObjectName(QStringLiteral("downloadTitle"));
    downloadTitle->setGeometry(QRect(26, 5, 30, 30));
    downloadTitle->setText("下载");
    connect(downloadButton,SIGNAL(clicked()),this,SLOT(downloadButtonClicked()));

    //分享组合按钮
    shareButton = new QPushButton(menuWidget);
    shareButton->setObjectName(QStringLiteral("shareButton"));
    shareButton->setGeometry(QRect(165, 0, 70, 40));
    shareButton->setFlat(true);
    shareLabel = new QLabel(shareButton);
    shareLabel->setObjectName(QStringLiteral("shareLabel"));
    shareLabel->setGeometry(QRect(6, 5, 20, 30));
    shareLabel->setPixmap(getPixmap("share"));
    shareTitle = new QLabel(shareButton);
    shareTitle->setObjectName(QStringLiteral("shareTitle"));
    shareTitle->setGeometry(QRect(26, 5, 30, 30));
    shareTitle->setText("分享");
    connect(shareButton,SIGNAL(clicked()),this,SLOT(shareButtonClicked()));

    //删除组合按钮
    deleteButton = new QPushButton(menuWidget);
    deleteButton->setObjectName(QStringLiteral("deleteButton"));
    deleteButton->setGeometry(QRect(241, 0, 70, 40));
    deleteButton->setFlat(true);
    deleteLabel = new QLabel(deleteButton);
    deleteLabel->setObjectName(QStringLiteral("deleteLabel"));
    deleteLabel->setGeometry(QRect(11, 5, 20, 30));
    deleteLabel->setPixmap(getPixmap("delete"));
    deleteTitle = new QLabel(deleteButton);
    deleteTitle->setObjectName(QStringLiteral("deleteTitle"));
    deleteTitle->setGeometry(QRect(31, 5, 30, 30));
    deleteTitle->setText("删除");
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteButtonClicked()));

    //新建组合按钮
    createButton = new QPushButton(menuWidget);
    createButton->setObjectName(QStringLiteral("createButton"));
    createButton->setGeometry(QRect(324, 1, 100, 40));
    createButton->setFlat(true);
    createLabel = new QLabel(createButton);
    createLabel->setObjectName(QStringLiteral("createLabel"));
    createLabel->setGeometry(QRect(11, 6, 20, 30));
    createLabel->setPixmap(getPixmap("create"));
    createTitle = new QLabel(createButton);
    createTitle->setObjectName(QStringLiteral("createTitle"));
    createTitle->setGeometry(QRect(31, 6, 60, 30));
    createTitle->setText("新建文件夹");
    connect(createButton,SIGNAL(clicked()),this,SLOT(createButtonClicked()));

    //离线组合按钮
    offlineDownButton = new QPushButton(menuWidget);
    offlineDownButton->setObjectName(QStringLiteral("offlineDownButton"));
    offlineDownButton->setGeometry(QRect(437, 0, 90, 40));
    offlineDownButton->setFlat(true);
    offlineDownLabel = new QLabel(offlineDownButton);
    offlineDownLabel->setObjectName(QStringLiteral("offlineDownLabel"));
    offlineDownLabel->setGeometry(QRect(11, 5, 20, 30));
    offlineDownLabel->setPixmap(getPixmap("download"));
    offlineDownTitle = new QLabel(offlineDownButton);
    offlineDownTitle->setObjectName(QStringLiteral("offlineDownTitle"));
    offlineDownTitle->setGeometry(QRect(31, 5, 50, 30));
    offlineDownTitle->setText("离线下载");
    connect(offlineDownButton,SIGNAL(clicked()),this,SLOT(offlineDownButtonClicked()));

    //排序按钮
    sortButton = new QPushButton(menuWidget);
    sortButton->setObjectName(QStringLiteral("sortButton"));
    sortButton->setGeometry(QRect(839, 12, 16, 16));
    sortButton->setIcon(getIcon("sort"));
    sortButton->setFlat(true);
    connect(sortButton,SIGNAL(clicked()),this,SLOT(sortButtonClicked()));

    //菜单按钮
    menuButton = new QPushButton(menuWidget);
    menuButton->setObjectName(QStringLiteral("menuButton"));
    menuButton->setGeometry(QRect(867, 12, 16, 16));
    menuButton->setIcon(getIcon("menu"));
    menuButton->setFlat(true);
    connect(menuButton,SIGNAL(clicked()),this,SLOT(menuButtonClicked()));
}


QIcon FileListWidget::getIcon(const QString &buttonType,int status)
{
    QString path = buttonImageMap[buttonType];
    QPixmap icon(path);
    QIcon res(icon.copy(16*(status-1),0,16,16));
    return res;
}

QPixmap FileListWidget::getPixmap(const QString &buttonType, int status)
{
    QString path = buttonImageMap[buttonType];
    QPixmap icon(path);
    QPixmap res(icon.copy(20*(status-1),0,20,20));
    return res;
}

QString FileListWidget::transformFileSize(qint64 filesize)
{
    QStringList units;
        units << "Bytes" << "KB" << "MB" << "GB" << "TB" << "PB";
        double mod  = 1024.0;
        double size = filesize;
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



bool FileListWidget::thePathIsExist(QString newPath)
{
   for(int i=0;i<vectorFileInfo.size();i++)
   {
       if(vectorFileInfo[i].isFile==0)
       {
           QString path = QString::fromLocal8Bit(vectorFileInfo[i].fileName);
           if(path==newPath)
           {
               return true;
           }
       }
   }
    return false;
}

bool FileListWidget::eventFilter(QObject *wcg, QEvent *event)
{
    if(wcg==this && event->type()==QEvent::MouseButtonPress)
    {
        if(searchLineEdit->hasFocus())
        {
            searchLineLoseFocus();
        }

    }
    return QWidget::eventFilter(wcg,event);
}

//收到列表展示数据槽 dong
void FileListWidget::responseFileListDataSlot(SelectFilesReply* allData)
{
    if(allData->file_status)
    {
        qDebug()<<"文件列表界面收到服务器返回数据……";
        int len = allData->len;
        vectorFileInfo.clear();
        for(int i=0;i<len;i++)
        {
            vectorFileInfo.append(allData->file[i]);
        }
        setFileModel();
        insertTableView();
        showTableView();
    }
}

void FileListWidget::deleteReply(DeleteFileReply* allData)
{
    int result = allData->isDelete;
    if(result)
    {
       //删除成功，再调用刷新按钮逻辑
       refreshButtonClicked();
    }
}

void FileListWidget::loginSucceed()
{
    qDebug()<<"loginSucceed()";
    refreshButtonClicked();
}


void FileListWidget::leftButtonClicked()
{
    qDebug()<<"leftButtonClicked()";
}

void FileListWidget::rightButtonClicked()
{
//    emit requestFileListDataSignal(currentPath);
    qDebug()<<"rightButtonClicked()";
}

void FileListWidget::belowButtonClicked()
{
    qDebug()<<"belowButtonClicked()";
}

void FileListWidget::refreshButtonClicked()
{
    emit requestFileListDataSignal(currentPath);
}

void FileListWidget::dbankButtonClicked()
{
    qDebug()<<"dbankButtonClicked()";
}

void FileListWidget::searchButtonClicked()
{
    qDebug()<<"searchButtonClicked()";
    QString searchInfo = searchLineEdit->text();
    qDebug()<<"searchInfo："<<searchInfo;
}

void FileListWidget::uploadButtonClicked()
{
    qDebug()<<"uploadButtonClicked()";
}

void FileListWidget::downloadButtonClicked()
{
    qDebug()<<"downloadButtonClicked()";
}

void FileListWidget::shareButtonClicked()
{
    qDebug()<<"shareButtonClicked()";
}

void FileListWidget::deleteButtonClicked()
{
    qDebug()<<"deleteButtonClicked()";
}

void FileListWidget::createButtonClicked()
{
    QString dlgTitle="新建用户目录确认框";
    QString txtLabel="请输入目录名";
    QString defaultInput="新建文件夹";
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;

    bool ok=false;
    QString newPath = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
    if (ok && !newPath.isEmpty())
    {
       if(thePathIsExist(newPath))
       {
           qDebug()<<"该目录已存在弹窗提示";
       }
       else
       {
           qDebug()<<"该目录不存在";
           CreatePathStruct pathStruct;
           pathStruct.userId = this->property("userId").toInt();
           strcpy_s(pathStruct.path,currentPath.toLocal8Bit().data());
           strcpy_s(pathStruct.newPath,newPath.toLocal8Bit().data());
           emit requestCreatePathSignal(&pathStruct);
       }
    }
    else
    {
        qDebug()<<"不新建";
    }
}

void FileListWidget::offlineDownButtonClicked()
{
    qDebug()<<"offlineDownButtonClicked()";
}

void FileListWidget::sortButtonClicked()
{
    qDebug()<<"sortButtonClicked()";
}

void FileListWidget::menuButtonClicked()
{
    qDebug()<<"menuButtonClicked()";
}

void FileListWidget::selectRow(QModelIndex index)
{
    qDebug()<<"selectedRow :"<<index.row();
    qDebug()<<"sender():"<<sender();
    int fileid = vectorFileInfo[index.row()].fileId;
    int isfile = vectorFileInfo[index.row()].isFile;
    qDebug()<<"current select file_id: "<<fileid;
    if(isfile)
    {
        qDebug()<<"the selected row is a file";
    }
    else
    {
        qDebug()<<"the selected row is a path";
    }
}


void FileListWidget::lineEditPress()
{
    QString info = searchLineEdit->text();
    if(info==searchTitle)
    {
        searchLineEdit->clear();
    }
    qDebug()<<"searchInfo: "<<info;
}

void FileListWidget::searchLineLoseFocus()
{
    searchLineEdit->clearFocus();
    QString info = searchLineEdit->text();
    if(info=="")
    {
        searchLineEdit->setText(searchTitle);
    }
}

void FileListWidget::transitSignal(QString type_button, int file_id)
{
    emit sendTransitSignal(type_button, file_id);
}

void FileListWidget::updatawidget()
{
    refreshButtonClicked();
}

