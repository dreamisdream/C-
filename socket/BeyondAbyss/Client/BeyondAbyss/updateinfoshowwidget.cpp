#include "updateinfoshowwidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QTcpSocket>
#include <thread>
#include "data.h"

UpdateInfoShowWidget::UpdateInfoShowWidget(const QString& filePath, qint64 fileSize, QWidget *parent)
    : QFrame(parent)
    ,m_filePath(filePath)
    ,m_fileSize(fileSize)
{
    this->setMinimumSize(896,80);
    this->setMaximumSize(896,80);
    this->setFrameShape(Box);
    this->show();

    iconLable = new QLabel(this);
    iconLable->setFixedSize(50,50);
    iconLable->setText("logo");

    QFileInfo info(filePath);
    m_fileName = info.fileName();

    fileNameLable = new QLabel(this);
    fileNameLable->setFixedSize(400,30);
    fileNameLable->setText(m_fileName);

    fileSizelable = new QLabel(this);
    fileSizelable->setFixedSize(200,25);
    setFileSizelable(fileSize);

    currentProgress = new QLabel(this);
    currentProgress->setFixedSize(220,30);

    currentStatus = new QLabel(this);
    currentProgress->setFixedSize(150,25);


    startOrStop = new QPushButton(this);
    startOrStop->setFixedSize(28,24);
    //startOrStop->setFlat(true);
    //connect(startOrStop,SIGNAL(clicked(bool)),this,SLOT(changeStartOrStopButtonIcon(bool)));

    countermand = new QPushButton(this);
    countermand->setFixedSize(28,24);

    parentFolder = new QPushButton(this);
    QIcon icon3 = cutIcon(tr(":/image/MainWindowImage/openfolder.png"));
    parentFolder->setIcon(icon3);
    parentFolder->setIconSize(parentFolder->size());
    parentFolder->setFixedSize(28,24);
    parentFolder->setFlat(true);

    layout1 = new QVBoxLayout;
    layou2 = new QVBoxLayout;
    layout3 = new QHBoxLayout(this);
    layout4 = new QHBoxLayout;

    layout1->addWidget(fileNameLable);
    layout1->addWidget(fileSizelable);

    layou2->addWidget(currentProgress);
    layou2->addWidget(currentStatus);

    layout4->addWidget(startOrStop,0,Qt::AlignLeft);
    layout4->addWidget(countermand,0,Qt::AlignLeft);
    layout4->addWidget(parentFolder,0,Qt::AlignLeft);

    layout3->addWidget(iconLable,0,Qt::AlignLeft);
    layout3->addLayout(layout1);
    layout3->addLayout(layou2);
    layout3->addStretch();
    layout3->addLayout(layout4);
    layout3->addStretch();

    connect(this, SIGNAL(updateProgress(qint64,qint64)), SLOT(setCurrentProgress(qint64,qint64)));
    connect(this, SIGNAL(updateTextStatus(const QString&)), SLOT(setCurrentStatus(const QString&)));

}

QIcon UpdateInfoShowWidget::cutIcon(const QString &path,QRect rect)
{
    QPixmap icon(path);
    QIcon res(icon.copy(rect));
    return res;
}

void UpdateInfoShowWidget::setIconLable(const QPixmap &icon)
{

}

void UpdateInfoShowWidget::setFileNameLable(const QString &fileName)
{
    fileNameLable->setText(fileName);
}

void UpdateInfoShowWidget::start(int taskId, quint16 port)
{
    qDebug()<<"开启上传文件线程";
    std::thread t1(&UpdateInfoShowWidget::uploadThread, this, taskId, port);
    t1.detach();
}

void UpdateInfoShowWidget::setFileSizelable(qint64 totalbytes,qint64 bytesWritten)
{
    fileSizelable->setText(tr("%1/%2").arg(readableFilesize(bytesWritten),readableFilesize(totalbytes)));
    if(bytesWritten > 0)
    {
        currentStatus->setText(tr("正在上传..."));
    }
    if(totalbytes == bytesWritten)
    {
        currentStatus->setText(tr("传输完成"));
    }
}

void UpdateInfoShowWidget::setCurrentProgress(qint64 totalbytes,qint64 bytesWritten)
{
    qreal res = (qreal)bytesWritten/totalbytes*100;
    currentProgress->setText(tr("正在读取文件...%1%").arg(QString::number(res,'f',1)));
    fileSizelable->setText(tr("%1/%2").arg(readableFilesize(bytesWritten),readableFilesize(totalbytes)));
    if(bytesWritten > 0)
    {
        currentStatus->setText(tr("正在上传..."));
    }
    if(totalbytes == bytesWritten)
    {
        currentStatus->setText(tr("传输完成"));
    }
}

void UpdateInfoShowWidget::setCurrentStatus(const QString &status)
{
    currentStatus->setText(status);
}

void UpdateInfoShowWidget::uploadThread(int taskId, quint16 port)
{
    QTcpSocket client;
    client.connectToHost("127.0.0.1", port);
    if ( !client.waitForConnected() ) {
        qDebug() << " 连接失败";
        return ;
    }

    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray fileName = m_fileName.toLocal8Bit();
    QByteArray filePath = m_filePath.toLocal8Bit();
    UpFile* fileInfo = new UpFile;
    fileInfo->fileId = taskId;
    fileInfo->fileSize = m_fileSize;
    qDebug()<<"传进自定义控件后的路径："<<m_filePath;
    memcpy(fileInfo->filePath, filePath.data(),filePath.size());
    memcpy(fileInfo->fileName,fileName.data(), fileName.size());

    emit updateProgress(m_fileSize,0);
    emit updateTextStatus("正在上传...");
    qint64 writeBytes = 0;
    while(true) {
        int bytes = (int)file.read(fileInfo->fileContent, 10 * 1024);
        if (bytes <= 0) {
            break;
        }

        fileInfo->fileContentSize = bytes;
        client.write((const char*)fileInfo, sizeof(UpFile));
        client.waitForBytesWritten();
        writeBytes += bytes;
        emit updateProgress(m_fileSize,writeBytes);

//        client.waitForReadyRead();
//        client.readAll();
    }

    //emit updateProgress(m_fileSize, m_fileSize);
    emit updateTextStatus("上传完成");
    this_thread::sleep_for(std::chrono::seconds(20));
    client.close();
    qDebug() << "文件上传成功.";
}

void UpdateInfoShowWidget::changeStartOrStopButtonIcon(bool checked)
{

}

QString UpdateInfoShowWidget::readableFilesize(quint64 filesize)
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

