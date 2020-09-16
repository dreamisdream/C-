#ifndef CLIENT_H
#define CLIENT_H

#include "data.h"

#include <QWidget>
#include <QTcpSocket>
#include <QList>
#include <QTimer>

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);
    ~Client();

   int sendToServer(char *,int);

private slots:
    //回收站功能关联槽
    void recycleDeleteButtonSlot(int);
    void recycleRecoveButtonSlot(int);
    void requestRecyleData();

public slots:
    void sendSelectFiles(QString );
    void manageFileListWidgetSlot(QString,int);
    void manageLogRequestSlot(int userId,int logLevle,int sortStatus);
    void getAllUserIdSlot();
    void requestCreatePathSlot(CreatePathStruct*);

public slots:
    void readData();
    void connected();
    void disconnected();
    void sendHeartPack();
private:
    QTcpSocket *pClient;
    QTimer *m_heart_pack;
signals:
   loginReplySignal(UserInfoReply &);
   registerReplySignal(registerInfoReply &);
   upFileReplySignal(UpFileRequestReply &);
   downFileReplySignal(DownFileRequestReply &);
   deleteFileReplySignal(DeleteFileReply *);
   filesReplySignal(SelectFilesReply*);
   recycleDeleteReplySignal(RecycleDeleteFileReply*);
   recycleRecoverReplySignal(RecycleRecoverFileReply*);
   sendNewLoginfoSignal(LogStruct*);
   sendLogUserIdSignal(LogUserId*);
   sendNewLoginfoSignal();
   shareFileInfo(ShareCodeReply *);
};

#endif // CLIENT_H
