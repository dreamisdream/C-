#ifndef RECYCLEBIN_H
#define RECYCLEBIN_H

#include <QWidget>
#include <QTableView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QLabel>
#include <vector>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

#include "data.h"
#include "fileinfo.h"
#include "recyclebinfileinfo.h"

class RecycleBin : public QWidget
{
    Q_OBJECT
public:
    explicit RecycleBin(QWidget *parent = nullptr);
    void run();
    void initData();
    void recycleBinTitleList();
    void recycleBinContentList();
    void insertTableView();
    void insertEachLine(int row, FileInfoStruct &file_info);
    QString transformFileSize(qint64);


private:
    void setFileModel();
    void showTableView();
private:
    int rowCount;
    QWidget* RecycleBinTitle;
    QTableView* RecycleBinView;
    QStandardItemModel* RecycleBinModel;
    void getData();
    QHBoxLayout *horizontalLayout;
    QCheckBox *fileCheckBox;
    QPushButton *button;
    QLabel *label;
    vector<FileInfoStruct> vectorCycleBinFile;
    FileInfoStruct  allFileInfo;

signals:
    void recyDeleteFileSignal(int);
    void recyRecoverFileSignal(int);
    void requestRecycleData();
    void requestFilelistSignal();

public slots:
    void recyDeleteFileSlot(int file_id);
    void recyRecoverFileSlot(int file_id);
    void responseFileListDataSlot(SelectFilesReply* allData);
    void recycleDeleteReplySlot(RecycleDeleteFileReply* allData);
    void recycleRecoverReplySlot(RecycleRecoverFileReply *allData);
};

#endif // RECYCLEBIN_H
