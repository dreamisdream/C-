#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QFrame>
#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include "data.h"

namespace Ui {
class DownloadWidget;
}

class DownloadWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DownloadWidget(DownFileRequestReply req,QWidget *parent = 0);
    ~DownloadWidget();
    void start();
    QString readableFilesize(quint64 filesize);

signals:
    void setProcessSignal(qint64);
private:
    void downloadThread();

private slots:
    void setProcessSlot(qint64 bytesWritten);

private:
    Ui::DownloadWidget                   *ui;
    QString                       m_filePath;
    QString                       m_fileName;
    int                             m_fileId;
    qint16                            m_port;
    qint64                     m_filePointer;
    qint64                        m_fileSize;
    QFile                     *m_currentFile;
};

#endif // DOWNLOADWIDGET_H
