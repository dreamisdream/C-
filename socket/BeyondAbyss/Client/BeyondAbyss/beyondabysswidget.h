#ifndef BEYONDABYSSWIDGET_H
#define BEYONDABYSSWIDGET_H


#include <QWidget>
#include <QMouseEvent>
#include <QListView>
#include <QTableView>
#include <QStandardItemModel>
#include "customstatusbar.h"
#include "loginmainwindow.h"
#include "registermainwindow.h"
#include "filelistwidget.h"
#include "data.h"
#include "client.h"
#include "updateinfoshowwidget.h"
#include <QThread>
#include "recyclebin.h"
#include "logwidget.h"
#include "limitsmanagement.h"
#include "sharefilewidget.h"
#include "downloadwidget.h"

namespace Ui {
class BeyondAbyssWidget;
}

class BeyondAbyssWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BeyondAbyssWidget(QWidget *parent = 0);
    ~BeyondAbyssWidget();
    void sendFileToSetver(int fileId, quint16 port);

private:
    //文件展示列表相关方法定义
    void initFileListMainWindow();
    //日志显示界面
    void initLoggingMainWindwo();
    //回收站界面
    void recycleBinWindowMethod();
    void limitManagementWindow();
    // 文件分享
    void initShareFile();
signals:
    void loginSucceed();
    void tellLoginSucceedSignal();
    void upFileSignalToFileSocket(UpFile);
    void compelted();
    void updateFileInfoToWidget(qint64,qint64);
private slots:
    // 登录的处理
    void loginInfoSlot(UserInfoReply &);
    // 注册处理
    void registerInfoSlot(registerInfoReply &);
    void loginSucceedDealMethod();

    void loginToregisterWin();
    void registerSucceed();
    void registerToLogin();

    void on_allFileShowButton_clicked(bool checked);

    void on_recyleBinShowButton_clicked(bool checked);

    void on_updateButton_clicked(bool checked);

    void on_downloadButton_clicked(bool checked);

    void toMainwidget();

    void toTransferListWidget();

    void toShareListWidget();

    void toLogListWidget();

    void toSuperSUProWidget();

    void closeThis();

    void on_testUploadButton_clicked();

    void setFileInfo(const QString &fileName,qint64 totalBytes);

    void updateFileInfo(qint64 totalbytes,qint64 bytesWritten);

    void startUploadThread(UpFileRequestReply & ret);

    void testSlot(UpFileReply&);

	void downLoadFileSlot(DownFileRequestReply& ret);

    void on_download_clicked();
    //登录成功之后，相关界面开始从服务器获取数据
    void loginSucceedFunction();

    void shareInfoDisplay(ShareCodeReply *);
    void updateShareInfoSlot();
private:
    Ui::BeyondAbyssWidget *ui;
    QPoint offset;
    QListView *m_uploadview;
    QStandardItemModel *m_uoloadModel;
    LoginMainWindow *m_login;
    RegisterMainWindow *m_register;
    Client *m_client;
    FileListWidget * fileListMainWindow;
    LogWidget* logWidget;
    RecycleBin *recycleBinMainWindow;
    LimitsManagement *limitManager;
    UpdateInfoShowWidget *m_newUpload;
    QThread *m_thread;
    QMap<int,QString>  m_filePathMap;
    int fileId;
    QMap<int,UpdateInfoShowWidget *> m_UpdateInfoShowWidgetMap;
    ShareFileWidget *m_pShareFile;
	QMap<int,DownloadWidget*> m_downloadWidgetMap;
    int downLoadTaskId;
    QListView  *m_downListView;
    QStandardItemModel *m_downloadModel;
    QVBoxLayout *m_downloadLayout;
};

#endif // BEYONDABYSSWIDGET_H
