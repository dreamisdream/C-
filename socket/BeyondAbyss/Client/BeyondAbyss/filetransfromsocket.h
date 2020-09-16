#ifndef FILETRANSFROMSOCKET_H
#define FILETRANSFROMSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "data.h"

class FileTransfromSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit FileTransfromSocket(QObject *parent = nullptr);
    FileTransfromSocket(const QString& filePath, quint16 port, QObject *parent = nullptr);

signals:
    upFileContentReplySignal(UpFileReply &);
    downFileContentReplySignal(DownFileRequestReply &);
public slots:
    void connectedSlot();
    qint64 bytesWrittenSlot(qint64 bytes);
    void readyReadSlot();
    void upFileSlot(UpFile );
    void downFileSlot(DownFile );
    void closeSocket();
private:
    quint16     m_port;
    QString     m_filePath;
    qint64      m_sendBytes;
    qint64      m_readBytes;
};

#endif // FILETRANSFROMSOCKET_H
