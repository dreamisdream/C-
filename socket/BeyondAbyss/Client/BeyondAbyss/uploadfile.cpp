#include "uploadfile.h"
#include <QDebug>
#include <QFileInfo>
#include <QThread>

UpLoadFile::UpLoadFile(QObject *parent):QObject(parent)
{
    m_payloadSize=64*1024;
    m_totalBytes=0;
    m_bytesWritten=0;
    m_bytesToWrite=0;
}

UpLoadFile::~UpLoadFile()
{
    m_localFile->close();
    m_localFile->deleteLater();
}

void UpLoadFile::getClientObject(Client *client)
{
    m_client = client;
}

void UpLoadFile::openFile()
{
    m_filePath = QString("C:\\Users\\86183\\Desktop\\duiengineskin.zip");
       if(!m_filePath.isEmpty())
       {
            m_localFile = new QFile(m_filePath);
            bool isOk = m_localFile->open(QFile::ReadOnly);
            if(!isOk)
            {
                qDebug()<<"文件打开失败";
                return;
            }
            m_totalBytes = m_localFile->size();
            QString fileName = QFileInfo(m_filePath).fileName();
            emit setFileInfoToWidget(fileName,m_totalBytes);

            strcpy_s(newTransfer.filePath,m_filePath.toLocal8Bit());
            strcpy_s(newTransfer.fileName,fileName.toLocal8Bit());
            strcpy_s(newTransfer.fileSize,QString::number(m_totalBytes).toLocal8Bit());
       }
}

void UpLoadFile::transfer()
{
    while((m_bytesToWrite = m_totalBytes - m_bytesWritten) > 0)
    {
        m_outBlock=m_localFile->read(qMin(m_bytesToWrite,m_payloadSize));
        strcpy_s(newTransfer.fileContent,m_outBlock.data());
        qDebug()<<"文件大小"<<m_totalBytes<<"包大小："<<m_outBlock.size();
        m_client->sendToServer((char *)&newTransfer,newTransfer.head.len);

        m_bytesWritten += m_outBlock.size();
        emit updatebytesWrittenTiwidget(m_totalBytes,m_bytesWritten);
        QThread::sleep(1);
    }
    if(m_bytesToWrite == 0)
    {
        qDebug()<<"文件发送完成";
    }
}

void UpLoadFile::run()
{
    openFile();
    transfer();
}
