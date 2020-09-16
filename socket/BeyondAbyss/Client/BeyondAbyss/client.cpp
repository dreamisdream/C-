#include "client.h"
#include "filelistwidget.h"
#include "sharefiledialog.h"

#define PORT 8888
#include "filelistwidget.h"
#include <QDebug>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , pClient(new QTcpSocket(this))
    , m_heart_pack(new QTimer(this))
{
    pClient->connectToHost(IP,PORT);
    m_heart_pack->setInterval(10000);
    m_heart_pack->start();
    connect(pClient,&QTcpSocket::connected,this,&Client::connected);
    connect(pClient,&QTcpSocket::disconnected,this,&Client::disconnected);
    connect(m_heart_pack,&QTimer::timeout,this,&Client::sendHeartPack);
}

Client::~Client()
{
}


int Client::sendToServer(char *data,int len)
{
    int ret = pClient->write(data,len);
    if (ret == -1)
    {
        qDebug() << pClient->errorString();
    }
    return ret;
}

//回收站删除按钮关联槽 hao
void Client::recycleDeleteButtonSlot(int file_id)
{
    int user_id = this->property("userId").toInt();
    RecycleDeleteFileStruct data;
    data.fileId = file_id;
    data.head.userId = user_id;
    sendToServer((char*)&data,data.head.len);
}

//回收站恢复按钮关联槽
void Client::recycleRecoveButtonSlot(int file_id)
{
    RecyRecoverDataStruct  data;
    data.fileid=file_id;
    data.head.userId=this->property("userId").toInt();
    sendToServer((char *)&data,data.head.len);

}

//回收站界面获取服务端数据
void Client::requestRecyleData()
{
    qDebug()<<"requestRecyleData";
    RequestRecyleDataStruct data;
    data.head.userId = this->property("userId").toInt();
    sendToServer((char*)&data,data.head.len);
}

void Client::readData()
{
    //类型4 长度4 数据�?
    QByteArray data = pClient->readAll();
    const char* retData = data.data();
    int type = *(int*)retData;
    int len = *(int*)(retData + sizeof(int));

/*
    switch (type)
    {
    case FileInfoType::LOGIN_REPLY:
        {
            UserInfoReply* info = (UserInfoReply*)ptr;
            qDebug() << "登录信息" << info->userId << info->success;
        }
        break;
    }
    */
    switch (type) {
    case FileInfoType::LOGIN_REPLY :
        emit loginReplySignal(*(UserInfoReply*)retData);
        break;
    case FileInfoType::REGISTER_REPLY:
        emit registerReplySignal(*(registerInfoReply*)retData);
        break;
    case FileInfoType::UP_FILE_REQUEST_REPLY:
        emit upFileReplySignal(*(UpFileRequestReply*)retData);
        break;
    case FileInfoType::DOWN_FILE_REQUEST_REPLY:
        emit downFileReplySignal(*(DownFileRequestReply*)retData);
        break;
    case FileInfoType::DELETE_FILE_REPLY:
       emit deleteFileReplySignal((DeleteFileReply*)retData);
        break;
    case FileInfoType::SELECT_FILES_REPLY: 
        //文件列表 回收�? 响应数据公共接口
        emit filesReplySignal((SelectFilesReply*)retData);
        break;
    case FileInfoType::RECYCLE_DELETE_REPLE:
        //回收站删除响�?
        emit recycleDeleteReplySignal((RecycleDeleteFileReply*)retData);
        break;
        //回复回收站恢复响�?
    case FileInfoType::RECYCLE_RECOVER_REPLE:
        emit recycleRecoverReplySignal((RecycleRecoverFileReply*)retData);
        break;
    case FileInfoType::LOG_ALLINFO:
        emit sendNewLoginfoSignal((LogStruct*)retData);
        break;
    case FileInfoType::LOG_ALLUSER_ID:
        qDebug()<<((LogUserId*)retData)->count;
        emit sendLogUserIdSignal((LogUserId*)retData);
        break;
	case FileInfoType::UPDATE_WIDGET_INFO:
        emit sendNewLoginfoSignal();
        break;
    case FileInfoType::SHARE_REPLY:
        emit shareFileInfo((ShareCodeReply*)retData);
        break;
    default:
        break;
    }
}

void Client::connected()
{
    qDebug() << "���������ӳɹ�";
    connect(pClient,&QTcpSocket::readyRead,this,&Client::readData);
}

void Client::disconnected()
{
    pClient->close();
}

void Client::sendHeartPack()
{
    HeartPack up;
    pClient->write((char*)&up,up.head.len);
}



void Client::manageFileListWidgetSlot(QString type_button, int file_id)
{
    int userId = this->property("userId").toInt();
    if(type_button=="delete")
    {
        DeleteFileStruct data;
        data.fileId = file_id;
        data.userId = userId;
        sendToServer((char*)&data,data.head.len);
    }
	else if(type_button=="down")
    {
        DownFileRequest request;
        request.fileId = file_id;
        sendToServer((char*)&request,request.len);
        qDebug()<<"���������ѷ���";
    }
     else if (type_button == "share"){
        ShareFileDialog *dia = new ShareFileDialog;
        dia->setAttribute(Qt::WA_ShowModal,true);
        QString code = dia->getShareCode(file_id,userId);
        //
        ShareCodeReq req;
        req.head.type = FileInfoType::SHARE;
        req.fileId = file_id;
        req.head.userId =userId;
        strcpy_s(req.code,code.toLocal8Bit().data());
        int ret = sendToServer((char*)&req,req.head.len);
        if(ret <= 0)
            return ;
        dia->show();
    }
}

void Client::manageLogRequestSlot(int userId, int logLevle, int sortStatus)
{
    LogRequestStruct data;
    data.selectUserId = userId;
    data.logLevle = logLevle;
    data.sortStatus = sortStatus;
    sendToServer((char*)&data,data.head.len);
}

void Client::getAllUserIdSlot()
{
    LogUserId data;
    sendToServer((char*)&data,data.len);
}

void Client::sendSelectFiles(QString path)
{
    SelectFiles data;
    data.head.userId = this->property("userId").toInt();
    strcpy_s(data.path,path.toLocal8Bit().data());
    sendToServer((char*)&data,data.head.len);
}

//�ͻ����½�Ŀ¼
void Client::requestCreatePathSlot(CreatePathStruct *pathStruct)
{
    qDebug()<<"pathStruct = "<<QString::fromLocal8Bit(pathStruct->newPath);
    sendToServer((char*)pathStruct,pathStruct->len);
}
