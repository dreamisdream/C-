#ifndef FILETRANSFORM_H
#define FILETRANSFORM_H

#include <QTcpSocket>
#include <QMap>
#include "data.h"

class FileTransform : public QTcpSocket
{
    Q_OBJECT
public:
    FileTransform(QObject *parent = nullptr);
    void close();

    // 信号
    void sendFile(UpFile &);
    void recvFile(DownFile &);

signals:
    void upFileReplySignal(UpFileReply &);
    void DownFileReplySignal(DownFileReply &);
public slots:
    void connectedSuccess();
    void bytesWritten(qint64 byte);
    void readyRead();

private:
    qint64      m_sendBytes;
    qint64      m_readBytes;
};

#endif // FILETRANSFORM_H
