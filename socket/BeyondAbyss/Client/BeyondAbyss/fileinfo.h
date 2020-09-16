#ifndef FILEINFO_H
#define FILEINFO_H

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


class FileInfo : public QWidget
{
    Q_OBJECT
public:
    explicit FileInfo(QWidget *parent,
                      const FileInfoStruct&allFileInfo);

public:
    FileInfoStruct allFileInfo;
    int file_id;
    int file_type;
    QString file_name;
    QString file_path;

public:
    QHBoxLayout *horizontalLayout;
    QCheckBox *fileCheckBox;
    QLabel *fileIcon;
    QLabel *fileName;
    QSpacerItem *horizontalSpacer;
    MyButton *shareButton;
    MyButton *downButton;
    MyButton *deleteButton;

private:
    void init();
    void initOthers(const FileInfoStruct&allFileInfo);
    void initImageMap();
    void bind();
    QIcon cutIcon(const QString & path);
    QIcon getIcon(const QString &buttonType,int status=1);
    QPixmap getPixmap(const QString type="dir");
    void reSetButton(QPushButton* button);
    void deleteQuestion();
    void shareQuestion();
    void downloadQuestion();
protected:

public:

private:
    QMap<QString,QString>buttonImageMap;
    QMap<QString,QString>fileIconMap;
    QMap<int,QString> filePictureMap;
signals:
    void manageButtonsOperatorSignal(QString type_button,int file_id);
//    void delete_file(QString type_button,int file_id);
//    void share_file(QString type_button,int file_id);
//    void download_file(QString type_button,int file_id);

public slots:
    void button_clicked();
    void checkBoxSelect(int status);
    void mouseOutButton();
    void mouseInButton();
};

#endif // FILEINFO_H
