#ifndef RECYCLEBINFILEINFO_H
#define RECYCLEBINFILEINFO_H

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMap>
#include <QDebug>
#include <data.h>
#include "mybutton.h"


class RecycleBinFileInfo : public QWidget
{
    Q_OBJECT
public:
    explicit RecycleBinFileInfo(QWidget *,const FileInfoStruct&allFileInfo);

public:
    void run();
signals:
    void DeleteButtonSIgnal(int file_id);//回收站删除信号
    void recoveButtonSignal(int file_id);
public slots:
    void button_clicked();


private:
    void initImageMap();
    void initOthers(const FileInfoStruct&allFileInfo);
    void reSetButton(MyButton *button);
    QIcon getIcon(const QString &buttonType,int status=1);
    QPixmap getPixmap(const QString type="dir");

    void recoverDealMethod();
    void deleteDealMethod();



public:
    FileInfoStruct allFileInfo;
    int file_id;
    int file_type;
    QString file_name;
    QString file_path;
    QMap<QString,QString>buttonImageMap;
    QMap<QString,QString>fileIconMap;
    QMap<int,QString> filePictureMap;

private:
    QHBoxLayout *horizontalLayout;
    QCheckBox *recycleCheckBox;
    QLabel *fileIcon;
    QLabel *fileName;
    QSpacerItem *horizontalSpacer;
    MyButton *recoverButton;
    MyButton *deleteButton;
};

#endif // RECYCLEBINFILEINFO_H
