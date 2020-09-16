#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QInputDialog>
#include <QDebug>
#include <QMap>
#include <QVector>
#include <QString>
#include "mylineedit.h"
#include "fileinfo.h"


class FileListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileListWidget(QWidget *parent = nullptr);
private:   //文件展示列表相关属性定义
    QWidget* fileLisWidget;
    QTableView* fileListView;
    QStandardItemModel* fileListModel;
private://文件展示列表相关方法定义
    void init();
    void initImagePath();
    void initOtherInfo();
    void initFileListView();
    void showTableView();
    void setFileModel();
    void insertTableView();
    void insertEachLine(int row,FileInfoStruct&file_info);
    void setFileListTitle();
    void setFileListMenu();
    QIcon getIcon(const QString &buttonType,int status=1);
    QPixmap getPixmap(const QString &buttonType,int status=1);
    QString transformFileSize(qint64);
    bool thePathIsExist(QString newPath);
protected:
    QMap<QString,QString>buttonImageMap;
    bool eventFilter(QObject *wcg, QEvent *event);

public:
//    FileInfo* info;
    int rowCount;
    QString currentPath;
    QVector<FileInfoStruct>vectorFileInfo;
    FileInfoStruct allFileInfo;  //temp

signals:
    //请求文件列表数据信号
    void sendTransitSignal(QString type_button,int file_id);
    void requestFileListDataSignal(QString currentPath);
    //请求服务端新建目录结构体
    void requestCreatePathSignal(CreatePathStruct* pathStruct);
public slots:
    void responseFileListDataSlot(SelectFilesReply* allData);
    void deleteReply(DeleteFileReply* allData);
    void loginSucceed();
    void leftButtonClicked();
    void rightButtonClicked();
    void belowButtonClicked();
    void refreshButtonClicked();
    void dbankButtonClicked();
    void searchButtonClicked();
    void uploadButtonClicked();
    void downloadButtonClicked();
    void shareButtonClicked();
    void deleteButtonClicked();
    void createButtonClicked();
    void offlineDownButtonClicked();
    void sortButtonClicked();
    void menuButtonClicked();
    void selectRow(QModelIndex);
    void lineEditPress();
    void searchLineLoseFocus();
    void transitSignal(QString type_button,int file_id);
    void updatawidget();
private:
    QWidget *centralWidget;
    QWidget *titleWidget;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *belowButton;
    QPushButton *refreshButton;
    QFrame *line;
    QPushButton *dbankButton;
    QFrame *line_2;
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QWidget* menuWidget;
    QPushButton *uploadButton;
    QLabel *uploadLabel;
    QLabel *uploadTitle;
    QPushButton *downloadButton;
    QLabel *downloadTitle;
    QLabel *downloadLabel;
    QPushButton *shareButton;
    QLabel *shareTitle;
    QLabel *shareLabel;
    QPushButton *deleteButton;
    QLabel *deleteLabel;
    QLabel *deleteTitle;
    QPushButton *createButton;
    QLabel *createLabel;
    QLabel *createTitle;
    QPushButton *offlineDownButton;
    QLabel *offlineDownLabel;
    QPushButton *sortButton;
    QLabel *offlineDownTitle;
    QPushButton *menuButton;
    QString searchTitle;
};

#endif // FILELISTWIDGET_H
