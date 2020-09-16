#include "downloadwidget.h"
#include "ui_downloadwidget.h"
#include <thread>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>

DownloadWidget::DownloadWidget(DownFileRequestReply req,QWidget *parent) :
    QFrame(parent),
    m_fileId(req.fileId),
    m_port(req.port),
    m_filePointer(req.filePointer),
    m_fileSize(req.fileSize),
    m_currentFile(nullptr),
    ui(new Ui::DownloadWidget)
{
    ui->setupUi(this);
    this->resize(this->parentWidget()->width() - 4,80);
    this->setFrameShape(Box);

    ui->fileNameLabel->setText(m_fileName);

    m_fileName = QFileInfo(QString::fromLocal8Bit(req.filePath)).fileName();

    m_filePath = QFileDialog::getSaveFileName(this,"请选择下载至：",QString("F:/beyondAbyssDownload/")+m_fileName);
    m_currentFile = new QFile(m_filePath);
    if(!m_currentFile->open(QIODevice::Append))
    {
        qDebug()<<"文件打开失败";
        return;
    }
    qDebug()<<"文件已打开";
    m_currentFile->seek(m_filePointer);
    qDebug()<<"文件指针移动"<<m_filePointer<<"个字节";

    connect(this,SIGNAL(setProcessSignal(qint64)),SLOT(setProcessSlot(qint64)));
    emit setProcessSignal(0);
}

DownloadWidget::~DownloadWidget()
{ 
    if(m_currentFile!=nullptr)
    {
        m_currentFile->close();
        m_currentFile->deleteLater();
        qDebug()<<"文件已关闭";
    }
    delete ui;
}

void DownloadWidget::start()
{
    std::thread t(&DownloadWidget::downloadThread,this);
    t.detach();
}

void DownloadWidget::downloadThread()
{
    QTcpSocket  *downloadSocket = new QTcpSocket;
    downloadSocket->connectToHost("127.0.0.1",m_port);
    if(!downloadSocket->waitForConnected())
    {
        qDebug()<<"连接失败";
        return;
    }
    qDebug()<<"文件下载socket已连接";

    qDebug()<<"读取到的文件大小："<<m_fileSize;
    while(true)
    {
        if(m_filePointer == 27220)
        {
            break;
        }

        DownFile fileInfo;
        fileInfo.fileId = m_fileId;
        strcpy_s(fileInfo.fileName,m_fileName.toLocal8Bit().data());
        strcpy_s(fileInfo.filePath,m_filePath.toLocal8Bit().data());
        fileInfo.filePointer = m_filePointer;
        downloadSocket->write((char*)&fileInfo,fileInfo.head.len);


        downloadSocket->waitForReadyRead();
        QByteArray package = downloadSocket->readAll();
        qint64 buffSize = package.size();
        char *recvData = package.data();
        DownFileReply *data = (DownFileReply*)recvData;
        int bytesWritten =  m_currentFile->write(data->file_content);
        m_currentFile->flush();
        m_filePointer += bytesWritten;

    }
    qDebug()<<"文件指针位置："<<m_filePointer;
    m_currentFile->close();
}

void DownloadWidget::setProcessSlot(qint64 bytesWritten)
{
    ui->statusLabel->setText(tr("正在下载..."));

    ui->fileSizeLabel->setText(tr("%1/%2").arg(readableFilesize(bytesWritten),readableFilesize(m_fileSize)));

     int res = bytesWritten*100/m_fileSize;
     ui->progressBar->setValue(res);
}

QString DownloadWidget::readableFilesize(quint64 filesize)
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
