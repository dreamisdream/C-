#include "beyondabysswidget.h"
#include "ui_beyondabysswidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include "filetransfromsocket.h"
#include <QFile>
#include <thread>

using namespace std;

BeyondAbyssWidget::BeyondAbyssWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BeyondAbyssWidget),
    fileId(0),
    downLoadTaskId(0)
{
    ui->setupUi(this);
    m_client = new Client(this);

    m_login = new LoginMainWindow(this);
    m_register = new RegisterMainWindow(this);
    m_login->getLoginClient(m_client);
    m_register->getRegisterClient(m_client);
    // 登陆信号关联
    connect(m_client,&Client::loginReplySignal,this,&BeyondAbyssWidget::loginInfoSlot);
    connect(m_client,&Client::registerReplySignal,this,&BeyondAbyssWidget::registerInfoSlot);

    connect(m_login,SIGNAL(loginToRegisterSignals()),this,SLOT(loginToregisterWin()));

    //TODO 登录测试（屏蔽输入验证）
    connect(m_login,SIGNAL(loginSucceed()),this,SLOT(loginSucceedDealMethod()));
    connect(m_register,SIGNAL(registerSucceedToLoginSignal()),this,SLOT(registerSucceed()));
    connect(m_register,SIGNAL(registerToLoginSignals()),this,SLOT(registerToLogin()));
     m_login->show();

    this->setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(this->width(),this->height());

    ui->stackedWidget->setCurrentIndex(0);

    //文件列表界面初始化,并默认展示
    initFileListMainWindow();
    //日志管理界面
    initLoggingMainWindwo();

    //回收站管理界面
    recycleBinWindowMethod();//haoyutong
    limitManagementWindow();
    // 初始化 分享文件界面
    initShareFile();
    connect(m_client,&Client::shareFileInfo,this,&BeyondAbyssWidget::shareInfoDisplay);
    connect(m_pShareFile,&ShareFileWidget::updateShareInfo,this,&BeyondAbyssWidget::updateShareInfoSlot);
    connect(ui->status_bar->MainWidgetButton,SIGNAL(clicked()),this,SLOT(toMainwidget()));
    connect(ui->status_bar->transferListButton,SIGNAL(clicked()),this,SLOT(toTransferListWidget()));
    connect(ui->status_bar->shareListButton,SIGNAL(clicked()),this,SLOT(toShareListWidget()));
    connect(ui->status_bar->logListButton,SIGNAL(clicked()),this,SLOT(toLogListWidget()));
    connect(ui->status_bar->SuperSUProButton,SIGNAL(clicked()),SLOT(toSuperSUProWidget()));
    connect(ui->status_bar->CloseButton,SIGNAL(clicked()),this,SLOT(closeThis()));

    connect(m_client,SIGNAL(upFileReplySignal(UpFileRequestReply &)),SLOT(startUploadThread(UpFileRequestReply &)));
    //收到下载请求回复,开始下载
    connect(m_client,SIGNAL(downFileReplySignal(DownFileRequestReply&)),SLOT(downLoadFileSlot(DownFileRequestReply&)));

    m_uploadview = new QListView(ui->page_6);
    m_uoloadModel = new QStandardItemModel(ui->page_6);
    m_uploadview->resize(900,570);
    m_uploadview->setModel(m_uoloadModel);
    m_uploadview->show();

    m_downListView = new QListView(ui->page_7);
    m_downloadModel = new QStandardItemModel(ui->page_7);
    m_downListView->resize(900,570);
    m_downListView->setModel(m_downloadModel);
    m_downListView->show();

}

BeyondAbyssWidget::~BeyondAbyssWidget()
{
    delete ui;
    m_thread->deleteLater();
}

void BeyondAbyssWidget::initFileListMainWindow()
{
     fileListMainWindow = new FileListWidget(ui->page_3);     
     fileListMainWindow->show();

     connect(fileListMainWindow,SIGNAL(requestFileListDataSignal(QString)),m_client,SLOT(sendSelectFiles(QString)),Qt::DirectConnection);

     connect(m_client,SIGNAL(filesReplySignal(SelectFilesReply*)),fileListMainWindow,SLOT(responseFileListDataSlot(SelectFilesReply*)));
     emit fileListMainWindow->requestFileListDataSignal("11");

     connect(fileListMainWindow,SIGNAL(sendTransitSignal(QString, int)),m_client,SLOT(manageFileListWidgetSlot(QString,int)));
     connect(m_client,SIGNAL(deleteFileReplySignal(DeleteFileReply*)),fileListMainWindow,SLOT(deleteReply(DeleteFileReply*)));
     connect(this,SIGNAL(tellLoginSucceedSignal()),fileListMainWindow,SLOT(loginSucceed()));
     connect(fileListMainWindow,SIGNAL(requestCreatePathSignal(CreatePathStruct*)),m_client,SLOT(requestCreatePathSlot(CreatePathStruct*)));
     connect(m_client,SIGNAL(sendNewLoginfoSignal()),fileListMainWindow,SLOT(updatawidget()));
}

void BeyondAbyssWidget::initLoggingMainWindwo()
{
    logWidget = new LogWidget(ui->page_8);
    connect(logWidget,SIGNAL(getAllUserId()),m_client,SLOT(getAllUserIdSlot()));
    connect(logWidget,SIGNAL(logRequestDataSignal(int,int,int)),m_client,SLOT(manageLogRequestSlot(int,int,int)));
    connect(m_client,SIGNAL(sendNewLoginfoSignal(LogStruct*)),logWidget,SLOT(recvNewLoginfoSlot(LogStruct*)));
    connect(m_client,SIGNAL(sendLogUserIdSignal(LogUserId*)),logWidget,SLOT(recvLogUserIdSlot(LogUserId*)));
    logWidget->show();
}

void BeyondAbyssWidget::recycleBinWindowMethod()
{
    recycleBinMainWindow = new RecycleBin(ui->page_5);
    connect(recycleBinMainWindow,SIGNAL(recyDeleteFileSignal(int)),m_client,SLOT(recycleDeleteButtonSlot(int)));
    connect(recycleBinMainWindow,SIGNAL(recyRecoverFileSignal(int)),m_client,SLOT(recycleRecoveButtonSlot(int)));
    connect(ui->recyleBinShowButton,SIGNAL(clicked()),m_client,SLOT(requestRecyleData()));
    connect(recycleBinMainWindow,SIGNAL(requestRecycleData()),m_client,SLOT(requestRecyleData()));
    connect(m_client,SIGNAL(filesReplySignal(SelectFilesReply*)),recycleBinMainWindow,SLOT(responseFileListDataSlot(SelectFilesReply*)));
    connect(m_client,SIGNAL(recycleDeleteReplySignal(RecycleDeleteFileReply*)),recycleBinMainWindow,SLOT(recycleDeleteReplySlot(RecycleDeleteFileReply*)));
    connect(m_client,SIGNAL( recycleRecoverReplySignal(RecycleRecoverFileReply*)),recycleBinMainWindow,SLOT(recycleRecoverReplySlot(RecycleRecoverFileReply*)));
    connect(recycleBinMainWindow,SIGNAL(requestFilelistSignal()),fileListMainWindow,SLOT(loginSucceed()));
    //requestFilelistSignal()
    recycleBinMainWindow->show();
}

void BeyondAbyssWidget::limitManagementWindow()
{
    limitManager = new LimitsManagement(ui->page_9);
    limitManager->show();
}

void BeyondAbyssWidget::initShareFile()
{
    m_pShareFile = new ShareFileWidget(ui->page_2);
    m_pShareFile->show();
}

void BeyondAbyssWidget::loginInfoSlot(UserInfoReply &info)
{
    if (info.success)
    {
        m_login->hide();
        this->setProperty("userId",info.userId);
        fileListMainWindow->setProperty("userId",info.userId);
        m_client->setProperty("userId",info.userId);
        loginSucceedFunction();
        this->show();
    }else
    {
        QMessageBox::warning(m_login,"登陆失败","密码不正确");
    }
}

void BeyondAbyssWidget::registerInfoSlot(registerInfoReply &info)
{
    qDebug()<<"info.success="<<info.success;
    if(info.success ==1)
    {
        QMessageBox::information(m_register,"注册成功","请登录");
        m_login->show();
        m_register->hide();
    }
    else
    {
         QMessageBox::information(m_register,"注册失败","该用户名已存在，请直接登录");
         m_login->show();
         m_register->hide();
    }
}

void BeyondAbyssWidget::loginSucceedDealMethod()
{
    m_login->hide();
    this->show();
}

void BeyondAbyssWidget::loginToregisterWin()
{
    m_login->hide();
    m_register->show();
}

void BeyondAbyssWidget::registerSucceed()
{
    m_login->show();
    m_register->hide();
}

void BeyondAbyssWidget::registerToLogin()
{
    m_login->show();
    m_register->hide();
}

void BeyondAbyssWidget::on_allFileShowButton_clicked(bool checked)
{
    if(!checked)
    {
        ui->stackedWidget_2->setCurrentIndex(0);
    }
}

void BeyondAbyssWidget::on_recyleBinShowButton_clicked(bool checked)
{
    if(!checked)
    {
        ui->stackedWidget_2->setCurrentIndex(1);
    }
}

void BeyondAbyssWidget::on_updateButton_clicked(bool checked)
{
    if(!checked)
    {
        ui->stackedWidget_3->setCurrentIndex(0);
    }
}

void BeyondAbyssWidget::on_downloadButton_clicked(bool checked)
{
    if(!checked)
    {
        ui->stackedWidget_3->setCurrentIndex(1);
    }
}

void BeyondAbyssWidget::toMainwidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BeyondAbyssWidget::toTransferListWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void BeyondAbyssWidget::toShareListWidget()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void BeyondAbyssWidget::toLogListWidget()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void BeyondAbyssWidget::toSuperSUProWidget()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void BeyondAbyssWidget::closeThis()
{
    this->close();
    QApplication::exit();
}

void BeyondAbyssWidget::on_testUploadButton_clicked()
{
    QStringList filePathList = QFileDialog::getOpenFileNames(this,"请选择一个/多个文件上传","F:\测试文件");
    for(auto i : filePathList)
    {
        QFileInfo info(i);
        m_filePathMap[fileId] = i;
        QString fileName = info.fileName();
        qint64 fileSize = info.size();
        UpFileRequest req;
        req.fileId = fileId;
        req.userId = m_client->property("userId").toInt();
        memcpy(req.filePath,i.toLocal8Bit().data(),sizeof(req.filePath));
        memcpy(req.fileName,fileName.toLocal8Bit().data(),sizeof(req.fileName));
        req.fileSize = fileSize;
        m_client->sendToServer((char *)&req,req.len);

        UpdateInfoShowWidget *m_newUpload = new UpdateInfoShowWidget(i, fileSize, ui->page_6);//这里的路径后面需要修改
        qDebug()<<"初始路径i:"<<i;
        m_UpdateInfoShowWidgetMap[fileId] = m_newUpload;
        qDebug()<<"新添加的上传任务Id："<<fileId;


        m_newUpload->setCurrentStatus("上传请求中...");
        QStandardItem *item = new QStandardItem();
        m_uoloadModel->appendRow(item);
        QModelIndex index = m_uoloadModel->indexFromItem(item);
        m_uploadview->setIndexWidget(index, m_newUpload);
        m_uploadview->setGridSize(m_newUpload->size());
        m_uploadview->setIndexWidget(m_uploadview->currentIndex(),m_newUpload);

        fileId++;
    }
}

void BeyondAbyssWidget::setFileInfo(const QString &fileName, qint64 totalBytes)
{
//    m_newUpload->setFileNameLable(fileName);
//    m_newUpload->setFileSizelable(totalBytes);
//    m_newUpload->setCurrentStatus(tr("正在上传..."));
}

void BeyondAbyssWidget::updateFileInfo(qint64 totalbytes,qint64 bytesWritten)
{
//    m_newUpload->setFileSizelable(totalbytes,bytesWritten);
    //    m_newUpload->setCurrentProgress(totalbytes,bytesWritten);
}

void BeyondAbyssWidget::startUploadThread(UpFileRequestReply &ret)
{
    qDebug()<<"收到服务器允许上传的回复 任务id："<<ret.fileId;
    auto iter = m_UpdateInfoShowWidgetMap.find(ret.fileId);
    if (iter != m_UpdateInfoShowWidgetMap.end()) {
        iter.value()->start(ret.fileId, ret.port);
    }
    else
    {
        qDebug()<<"没有找到该下载任务";
    }
}

void BeyondAbyssWidget::sendFileToSetver(int fileId, quint16 port)
{
    //int fileId = ret.fileId;
    QString filePath = m_filePathMap[fileId];
    QFile *localFile = new QFile(filePath);
    bool isOk = localFile->open(QFile::ReadOnly);
    if(!isOk)
    {
        qDebug()<<"文件打开失败";
        return;
    }
    qint64 totalBytes = localFile->size();
    QString fileName = QFileInfo(filePath).fileName();
    //emit setFileInfoToWidget(fileName,m_totalBytes);


    qint64 bytesWritten = 0;
    qint64 bytesToWrite = totalBytes;
    qint64 payloadSize = 10*1024;
    while((bytesToWrite = totalBytes - bytesWritten) > 0)
    {
        QByteArray data = localFile->read(qMin(bytesToWrite,payloadSize));

        UpFile packge;
        memcpy(packge.filePath,filePath.toLocal8Bit().data(),filePath.size());
        memcpy(packge.fileName,fileName.toLocal8Bit(),fileName.size());
        packge.fileSize = (__int64)totalBytes;
        memcpy(packge.fileContent,data.data(),data.size());
        packge.fileContentSize = data.size();
        packge.fileId = fileId;
        qDebug()<<"文件大小"<<totalBytes<<"内容长度："<<packge.fileContentSize<<"类型"<<packge.head.type<<packge.head.len;
        //qDebug()<<"包类型："<<packge.head.type<<"包长度："<<packge.head.len<<"包内容:"
              // <<QString::fromLocal8Bit((packge.fileContent));


        bytesWritten += data.size();
        emit upFileSignalToFileSocket(packge);
        //fileSocket->upFileSlot(packge);
    }
    localFile->close();
    localFile->deleteLater();
}

void BeyondAbyssWidget::testSlot(UpFileReply &ret)
{


    qDebug()<<ret.head.type<<ret.head.len<<ret.success<<ret.recvBytes<<ret.totalSize<<ret.fileId;
    if(ret.recvBytes == ret.totalSize)
    {
        //emit compelted();

    }
    //emit updateFileInfoToWidget(ret.totalSize,ret.recvBytes);
    m_UpdateInfoShowWidgetMap[ret.fileId]->setFileSizelable(ret.totalSize,ret.recvBytes);
    m_UpdateInfoShowWidgetMap[ret.fileId]->setCurrentProgress(ret.totalSize,ret.recvBytes);
}


//登录成功后，再从服务器获取相关界面数据；
void BeyondAbyssWidget::loginSucceedFunction()
{
    fileListMainWindow->refreshButtonClicked();
    emit logWidget->getAllUserId();
    emit tellLoginSucceedSignal();
}

void BeyondAbyssWidget::downLoadFileSlot(DownFileRequestReply &ret)
{

    DownloadWidget *newDownload = new DownloadWidget(ret,ui->page_7);
    m_downloadWidgetMap[downLoadTaskId] = newDownload;
    newDownload->start();
    newDownload->show();

    QStandardItem *item = new QStandardItem();
    m_downloadModel->appendRow(item);
    QModelIndex index = m_downloadModel->indexFromItem(item);
    m_downListView->setIndexWidget(index, newDownload);
    m_downListView->setGridSize(newDownload->frameRect().size());
    m_downListView->setIndexWidget(m_downListView->currentIndex(),newDownload);

    downLoadTaskId++;


}

void BeyondAbyssWidget::on_download_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
    DownFileRequestReply test;
    test.fileId = 1;
    //strcpy_s(test.filePath , "data.txt");
    test.filePointer = 0;
    test.fileSize = 500500;
    test.port =  1000;
    downLoadFileSlot(test);
}
void BeyondAbyssWidget::updateShareInfoSlot()
{
    ShareCodeReq req;
    req.head.type =FileInfoType::SHARE_INFOS;
    m_client->sendToServer((char*)&req,req.head.len);
}
void BeyondAbyssWidget::shareInfoDisplay(ShareCodeReply *info)
{
    m_pShareFile->m_tabWidget->setRowCount(info->size);
    for(int i=0;i<info->size;++i){
        qDebug()<<"-------------------";
        QString::fromLocal8Bit(info->code[i].code).trimmed();
        if(info->code[i].code == nullptr) {
          continue ;
       }
        qDebug()<<"-------------------"<< info->code[i].code;
        m_pShareFile->appendRow(info->code[i].userId,info->code[i].fileId,info->code[i].code);
    }
}
