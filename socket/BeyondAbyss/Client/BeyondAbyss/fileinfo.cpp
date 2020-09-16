#include "fileinfo.h"
#include "sharefiledialog.h"


FileInfo::FileInfo(QWidget *parent,const FileInfoStruct&allFileInfo):QWidget(parent)
{

    this->setFixedSize(360, 48);
    initOthers(allFileInfo);
    init();

}

void FileInfo::initOthers(const FileInfoStruct&allFileInfo)
{
    this->allFileInfo = allFileInfo;
    file_id = allFileInfo.fileId;
    file_type= allFileInfo.fileType;
    file_name= QString::fromLocal8Bit(allFileInfo.fileName);
    file_path= QString::fromLocal8Bit(allFileInfo.filePath);
//    file_name = "跃渊文件管理系统";
}


void FileInfo::init()
{
    //图片数据初始化
    initImageMap();

    //设置布局
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

    //单选框
    fileCheckBox = new QCheckBox(this);
    fileCheckBox->setObjectName(QStringLiteral("fileCheckBox"));
    fileCheckBox->setProperty("file_id",file_id);
    connect(fileCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkBoxSelect(int)));
    horizontalLayout->addWidget(fileCheckBox);

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


    //分享按钮
    shareButton = new MyButton(this);
    shareButton->setObjectName(QStringLiteral("shareButton"));
    shareButton->setIcon(getIcon("share",1));
    shareButton->setProperty("buttonType","share");
    shareButton->setAutoDefault(false);
    reSetButton(shareButton);


    //下载按钮
    downButton = new MyButton(this);
    downButton->setObjectName(QStringLiteral("downButton"));
    downButton->setIcon(getIcon("down"));
    downButton->setProperty("buttonType","down");
    reSetButton(downButton);


    //删除按钮
    deleteButton = new MyButton(this);
    deleteButton->setObjectName(QStringLiteral("deleteButton"));
    deleteButton->setIcon(getIcon("delete"));
    deleteButton->setProperty("buttonType","delete");
    reSetButton(deleteButton);
}


void FileInfo::initImageMap()
{
    //图片根目录
    QString res = ":/image/Filelist/";

    //按钮类型
    buttonImageMap["share"] = res+"createshare.png";
    buttonImageMap["down"] = res+"download.png";
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

void FileInfo::bind()
{
    
}

QIcon FileInfo::cutIcon(const QString & path)
{
    QPixmap icon(path);
    QIcon res(icon.copy(0,0,34,20));
    return res;
}

QIcon FileInfo::getIcon(const QString &buttonType,int status)
{
    QString path = buttonImageMap[buttonType];
    QPixmap icon(path);
    QIcon res(icon.copy(20*(status-1),0,20,20));
    return res;
}

QPixmap FileInfo::getPixmap(const QString type)
{
    QString path = fileIconMap[type];
    QPixmap pixmap(path);
    return pixmap;
}

void FileInfo::reSetButton(QPushButton *button)
{
    button->setStyleSheet(QStringLiteral(""));
    button->setFlat(true);
    button->setProperty("file_id",file_id);
    button->setProperty("file_name",file_name);
    connect(button,SIGNAL(clicked()),this,SLOT(button_clicked()));
    connect(button,SIGNAL(mouse_in()),this,SLOT(mouseInButton()));
    connect(button,SIGNAL(mouse_out()),this,SLOT(mouseOutButton()));
    horizontalLayout->addWidget(button);
}


void FileInfo::button_clicked()
{
    qDebug()<<"file_id:"<<file_id;
    qDebug()<<"buttonType:"<<sender()->property("buttonType").toString();
    QString buttonType = sender()->property("buttonType").toString();

    if(buttonType=="delete")
    {
        deleteQuestion();
    }
    else if(buttonType=="share")
    {
        shareQuestion();
    }
    else if(buttonType=="down")
    {
        downloadQuestion();
    }
}

void FileInfo::checkBoxSelect(int status)
{
    qDebug()<<status;
    qDebug()<<"file_id:"<<sender()->property("file_id").toInt();
}


void FileInfo::deleteQuestion()
{
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
        emit manageButtonsOperatorSignal("delete",file_id);
    }
}

void FileInfo::shareQuestion()
{
    // 分享文件对话框 赵恺亮
//    ShareFileDialog *dia = new ShareFileDialog;
//    dia->setAttribute(Qt::WA_ShowModal,true);
//    int userId = 1;
//    dia->getShareCode(file_id,userId);
//    dia->show();
//    connect(dia,&ShareFileDialog::sendShareCodeSignal,this,&FileInfo::transformShareCodeSignal);
//    QString dlgTitle="文件分享确认框";
//    QString strInfo;
//    strInfo =QString("是否分享该文件:%1").arg(file_name);

//    QMessageBox::StandardButton  defaultBtn;
//    defaultBtn = QMessageBox::NoButton; //缺省按钮

//    QMessageBox::StandardButton result;//返回选择的按钮
//    result=QMessageBox::question(this, dlgTitle, strInfo,
//                      QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
//                      defaultBtn);

//    if (result==QMessageBox::Yes)
//    {
        emit manageButtonsOperatorSignal("share",file_id);
////        qDebug()<<"Question消息框: Yes 被选择";
//    }
}

void FileInfo::downloadQuestion()
{
    QString dlgTitle="文件下载确认框";
    QString strInfo;
    strInfo =QString("是否下载文件: %1").arg(file_name);

    QMessageBox::StandardButton  defaultBtn;
    defaultBtn = QMessageBox::NoButton; //缺省按钮

    QMessageBox::StandardButton result;//返回选择的按钮
    result=QMessageBox::question(this, dlgTitle, strInfo,
                      QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                      defaultBtn);

    if (result==QMessageBox::Yes)
    {
//        qDebug()<<"Question消息框: Yes 被选择";
        emit manageButtonsOperatorSignal("down",file_id);
    }
}

void FileInfo::mouseInButton()
{
    MyButton* button = static_cast<MyButton*>(sender());
    QString buttonType = button->property("buttonType").toString();
    button->setIcon(getIcon(buttonType,2));
}


void FileInfo::mouseOutButton()
{
    MyButton* button = static_cast<MyButton*>(sender());
    QString buttonType = button->property("buttonType").toString();
    button->setIcon(getIcon(buttonType,1));
}
