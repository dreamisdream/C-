#include "recyclebinfileinfo.h"

RecycleBinFileInfo::RecycleBinFileInfo(QWidget *parent,const FileInfoStruct&allFileInfo) : QWidget(parent)
{
    initOthers(allFileInfo);
    initImageMap();
    run();
}


void RecycleBinFileInfo::initImageMap()
{
    //图片根目录
    QString res = ":/image/Filelist/";

    //按钮类型
    buttonImageMap["recover"] = res+"moveout.png";
    buttonImageMap["delete"] = res+"delete.png";

    //文件类型图标
    fileIconMap["dir"] = res+"FolderType.png";
    fileIconMap["image"] = res+"ImgType.png";
    fileIconMap["text"] = res+"TxtType.png";
    fileIconMap["music"] = res+"MusicType.png";
    fileIconMap["video"] = res+"VideoType.png";
    fileIconMap["excel"] = res+"XlsType.png";
    fileIconMap["rar"] = res+"RarType.png";
    fileIconMap["pdf"] = res+"PdfType.png";
    fileIconMap["ppt"] = res+"PptType.png";
    fileIconMap["doc"] = res+"OtherType.png";
    fileIconMap["other"] = res+"OtherType.png";

    //文件图标与类型映射表
    filePictureMap[0] = "dir";
    filePictureMap[1] = "image";
    filePictureMap[2] = "text";
    filePictureMap[3] = "music";
    filePictureMap[4] = "video";
    filePictureMap[5] = "excel";
    filePictureMap[6] = "rar";
    filePictureMap[7] = "pdf";
    filePictureMap[8] = "ppt";
    filePictureMap[9] = "doc";
    filePictureMap[10] = "other";
}


void RecycleBinFileInfo::initOthers(const FileInfoStruct&allFileInfo)
{
    this->allFileInfo = allFileInfo;
    file_id = allFileInfo.fileId;
    file_type= allFileInfo.fileType;
    file_name= QString::fromLocal8Bit(allFileInfo.fileName);
    file_path= allFileInfo.filePath;
}


void RecycleBinFileInfo::run()
{
    //布局
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(5);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    //单选框
    recycleCheckBox = new QCheckBox(this);
    recycleCheckBox->setObjectName(QStringLiteral("recycleCheckBox"));
    horizontalLayout->addWidget(recycleCheckBox);

    //文件图标
    fileIcon = new QLabel(this);
    fileIcon->setObjectName(QStringLiteral("fileIcon"));
    fileIcon->setPixmap(getPixmap(filePictureMap[file_type]));
    horizontalLayout->addWidget(fileIcon);

    //文件名称
    fileName = new QLabel(this);
    fileName->setObjectName(QStringLiteral("fileName"));
    fileName->setText(file_name);
    horizontalLayout->addWidget(fileName);


    //弹簧
    horizontalSpacer = new QSpacerItem(241, 27, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    //删除按钮
    deleteButton = new MyButton(this);
    deleteButton->setObjectName(QStringLiteral("deleteButton"));
    deleteButton->setIcon(getIcon("delete"));
    deleteButton->setProperty("buttonType","delete");
    reSetButton(deleteButton);


    //恢复按钮
    recoverButton = new MyButton(this);
    recoverButton->setObjectName(QStringLiteral("recoverButton"));
    recoverButton->setIcon(getIcon("recover"));
    recoverButton->setProperty("buttonType","recover");
    reSetButton(recoverButton);


}



QIcon RecycleBinFileInfo::getIcon(const QString &buttonType,int status)
{
    QString path = buttonImageMap[buttonType];
    QPixmap icon(path);
    QIcon res(icon.copy(20*(status-1),0,20,20));
    return res;
}

QPixmap RecycleBinFileInfo::getPixmap(const QString type)
{
    QString path = fileIconMap[type];
    QPixmap pixmap(path);
    return pixmap;
}


void RecycleBinFileInfo::reSetButton(MyButton *button)
{
    button->setStyleSheet(QStringLiteral(""));
    button->setFlat(true);
    button->setProperty("file_id",file_id);
    button->setProperty("file_name",file_name);
    connect(button,SIGNAL(clicked()),this,SLOT(button_clicked()));
//    connect(button,SIGNAL(mouse_in()),this,SLOT(mouseInButton()));
//    connect(button,SIGNAL(mouse_out()),this,SLOT(mouseOutButton()));
    horizontalLayout->addWidget(button);
}


void RecycleBinFileInfo::button_clicked()
{
    QString buttonType = sender()->property("buttonType").toString();
    if(buttonType == "recover")
    {
        recoverDealMethod();
    }
    else if(buttonType == "delete")
    {
        deleteDealMethod();
    }
}


void RecycleBinFileInfo::recoverDealMethod()
{
    qDebug()<<"处理恢复逻辑";
    QString dlgTitle="文件恢复确认框";
    QString strInfo;
    strInfo =QString("是否恢复文件: %1").arg(file_name);

    QMessageBox::StandardButton  defaultBtn;
    defaultBtn = QMessageBox::NoButton; //缺省按钮

    QMessageBox::StandardButton result;//返回选择的按钮
    result=QMessageBox::question(this, dlgTitle, strInfo,
                      QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                      defaultBtn);

    if (result==QMessageBox::Yes)
    {
        emit recoveButtonSignal(file_id);
    }

}

void RecycleBinFileInfo::deleteDealMethod()
{
    qDebug()<<"处理删除逻辑逻辑";
    QString dlgTitle="文件删除确认框";
    QString strInfo;
    strInfo =QString("是否要把文件:%1 放到回收站").arg(file_name);

    QMessageBox::StandardButton  defaultBtn;
    defaultBtn = QMessageBox::NoButton; //缺省按钮

    QMessageBox::StandardButton result;//返回选择的按钮
    result=QMessageBox::question(this, dlgTitle, strInfo,
                      QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                      defaultBtn);

    if (result==QMessageBox::Yes)
    {
       emit DeleteButtonSIgnal(file_id);
    }
}

