#include "filetransfromsocket.h"
#include "data.h"

class debug;

FileTransfromSocket::FileTransfromSocket(QObject *parent) : QTcpSocket(parent)
{
    ;
}

FileTransfromSocket::FileTransfromSocket(const QString &filePath, quint16 port, QObject *parent)
    : QTcpSocket(parent)
    , m_filePath(filePath)
    , m_port(port)
    , m_readBytes(0)
    , m_sendBytes(0)
{
    //connect(this,SIGNAL(connected()),SLOT(connectedSlot()));
    connectToHost("192.168.0.158",m_port);
    if(this->waitForConnected())
    {
        qDebug()<<"文件传输连接成功";
        connect(this,SIGNAL(readyRead()),SLOT(readyReadSlot()));
        connect(this,SIGNAL(bytesWritten(qint64)),SLOT(bytesWrittenSlot(qint64)));

    }

//    connectToHost("192.168.43.157",m_port);
}

void FileTransfromSocket::connectedSlot()
{
    qDebug()<<"文件传输连接成功";
    connect(this,SIGNAL(readyRead()),SLOT(readyReadSlot()));
//    UpFile p;
//    this->write((char*)&p,p.head.len);
}

qint64 FileTransfromSocket::bytesWrittenSlot(qint64 bytes)
{
    m_sendBytes += bytes;
    if (m_sendBytes >= m_readBytes) {
        qDebug() << "写入数据" << m_sendBytes;
        //this->deleteLater();
    }
    return m_sendBytes;
}

void FileTransfromSocket::readyReadSlot()
{
   qDebug()<<"进入收包";

   int type = 0;
   char *recvData = nullptr;
   QByteArray totalBuff = this->readAll();
   qint64 totalSize = totalBuff.size();
   qint64 i = 0;
   while (i < totalSize) {
       type = *(int *)totalBuff.data();
       int len = *(int *)(totalBuff.data() + sizeof(int));
       recvData = new char[len];
       memcpy(recvData,totalBuff.data() + i ,len);
       i += len;
       qDebug()<<"i:"<<i<<"总长度："<<totalSize;

       if (type == FileInfoType::UP_FILE_REPLY) {
           emit upFileContentReplySignal(*(UpFileReply *)recvData);
       }else if (type == FileInfoType::DOWN_FILE_REPLY) {
           emit downFileContentReplySignal(*(DownFileRequestReply *)recvData);
       }else{
           ;
       }
   }



}

void FileTransfromSocket::upFileSlot(UpFile data)
{
    qint64 sendBytes= this->write((char*)&data,data.head.len);
    qDebug()<<"sendBytes"<<sendBytes;
}

void FileTransfromSocket::downFileSlot(DownFile data)
{
    this->write((char*)&data,data.head.len);
}

void FileTransfromSocket::closeSocket()
{
    qDebug()<<"服务器已全部接收";
    this->closeSocket();
    this->close();
    this->deleteLater();
}
