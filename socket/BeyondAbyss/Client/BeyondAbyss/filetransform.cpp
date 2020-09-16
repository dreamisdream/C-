#include "filetransform.h"
#include <QDebug>

FileTransform::FileTransform(QObject *parent)
    : QTcpSocket(parent)
{
    m_readBytes = 0;
    m_sendBytes =0;
    connect(this,&QTcpSocket::connected,this,&FileTransform::connectedSuccess);
    connect(this, SIGNAL(bytesWritten(qint64)), SLOT(bytesWritten(qint64)));

    connectToHost("127.0.0.1",8888);
}

void FileTransform::sendFile(UpFile &data)
{
    this->write((char*)&data,data.head.len);
}

void FileTransform::recvFile(DownFile &data)
{
   this->write((char*)&data,data.head.len);
}

void FileTransform::close()
{
    qDebug() <<"guanbile..............";
    this->close();
}

void FileTransform::connectedSuccess()
{
    qDebug() << "连接成功...........";
    connect(this,SIGNAL(readyRead()),SLOT(readyRead()));
}

void FileTransform::bytesWritten(qint64 bytes)
{
   m_sendBytes +=bytes;
   if (m_sendBytes >= m_readBytes) {
       this->deleteLater();
   }
}

void FileTransform::readyRead()
{
    char head[HEAD_LEN] ={0};
    this->read(head,HEAD_LEN);
    int len = *(int *)(head+4);
    if (len >2000 || len <= 0)
        return ;
    char *data = (char *)malloc(len);
    memset(data,0,len);
    memcpy(data,head,HEAD_LEN);
    this->read(data+HEAD_LEN,len-HEAD_LEN);
    int type = *(int *)head;
    if (type == FileInfoType::UP_FILE_REPLY) {
        emit upFileReplySignal(*(UpFileReply*)data);
    }else if (type == FileInfoType::DOWN_FILE_REPLY) {
        emit DownFileReplySignal(*(DownFileReply*)&data);
    }
}
