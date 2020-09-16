#ifndef UPLOADFILE_H
#define UPLOADFILE_H

#include <QFile>
#include <QObject>
#include "client.h"

class UpLoadFile:public QObject
{
    Q_OBJECT
public:
    explicit UpLoadFile(QObject *parent = nullptr);
    ~UpLoadFile();
    void getClientObject(Client *client);
    void openFile();
    void transfer();

signals:
    void setFileInfoToWidget(const QString &fileName,qint64 totalBytes);
    void updatebytesWrittenTiwidget(qint64 totalBytes,qint64 bytesWritten);
public slots:
    void run();
private:
       Client *m_client;
       QFile *m_localFile;
       qint64 m_totalBytes;
       qint64 m_bytesWritten;
       qint64 m_bytesToWrite;
       qint64 m_payloadSize;
       QString m_filePath;
       QByteArray m_outBlock;
       UpFile newTransfer;
};

#endif // UPLOADFILE_H
