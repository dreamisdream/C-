#include "logwidget.h"


LogWidget::LogWidget(QWidget *parent) : QWidget(parent)
{
    this->setParent(parent);
    this->resize(850, 600);
    this->move(215,0);
    getAllData();
    init();
    insertData();
    setUserCombox();
    setLevelCombox();
}

void LogWidget::initOther()
{
    levelVector[1] = "INFO";
    levelVector[2] = "DEBUG";
    levelVector[3] = "ERROE";
    levelVector[4] = "WARNING";
    levelVector[5] = "FATAL";
}

void LogWidget::init()
{
    this->setStyleSheet(("background-color: rgb(255, 255, 255);"));
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    centralWidget->resize(this->size());

    titleWidget = new QWidget(centralWidget);
    titleWidget->resize(centralWidget->size().width(),40);
    titleWidget->setObjectName(QStringLiteral("titleWidget"));
    titleWidget->setStyleSheet(("background-color:#F7F7F7;"));

    userBox = new QComboBox(titleWidget);
    userBox->setObjectName(QStringLiteral("userBox"));
    userBox->setGeometry(QRect(0, 0, 150, 40));
    connect(userBox,SIGNAL(currentIndexChanged(int)),this,SLOT(userBoxChange(int)));

    levelBox = new QComboBox(titleWidget);
    levelBox->setObjectName(QStringLiteral("levelBox"));
    levelBox->setGeometry(QRect(userBox->x()+userBox->size().width(), 0, 150, 40));
    connect(levelBox,SIGNAL(currentIndexChanged(int)),this,SLOT(levelBoxChange(int)));

    timeButton = new QPushButton(titleWidget);
    timeButton->setObjectName(QStringLiteral("timeButton"));
    timeButton->setGeometry(QRect(levelBox->x()+levelBox->size().width(), 0, 200, 40));
    timeButton->setFlat(true);

    timeLabel = new QLabel(timeButton);
    timeLabel->setObjectName(QStringLiteral("timeLabel"));
    timeLabel->setGeometry(QRect(50,10,50,20));
    timeLabel->setText("操作时间");

    timeSort = new QLabel(timeButton);
    timeSort->setObjectName(QStringLiteral("timeSort"));
    timeSort->setGeometry(QRect(120,10,18,18));
    timeSort->setPixmap(getPixmap(0));

    connect(timeButton,SIGNAL(clicked()),this,SLOT(timeButtonCliecked()));

    logInfoLable = new QLabel(titleWidget);
    logInfoLable->setObjectName(QStringLiteral("logInfoLable"));
    logInfoLable->setGeometry(QRect(timeButton->x()+timeButton->size().width(), 0, 300, 40));
    logInfoLable->setText("               操作内容");

    logTable = new QTableWidget(centralWidget);
    logTable->setObjectName(QStringLiteral("logTable"));
    logTable->setGeometry(QRect(titleWidget->x(),
                                titleWidget->y()+titleWidget->size().height(),
                                centralWidget->size().width(),
                                410));

    logTable->setRowCount(10);
    logTable->setColumnCount(4);
    logTable->setColumnWidth(0,150);
    logTable->setColumnWidth(1,150);
    logTable->setColumnWidth(2,200);
    logTable->setColumnWidth(3,340);
    logTable->setShowGrid(true);
    logTable->verticalHeader()->setDefaultSectionSize(40);
    logTable->horizontalHeader()->setVisible(false);
    logTable->verticalHeader()->setVisible(false);
    logTable->verticalHeader()->setCascadingSectionResizes(false);
    logTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    logTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    buttonsWidget = new QWidget(centralWidget);
    buttonsWidget->setObjectName(QStringLiteral("buttonsWidget"));
    buttonsWidget->setGeometry(logTable->x(),
                               logTable->y()+logTable->size().height(),
                               logTable->size().width(),
                               centralWidget->size().height()-titleWidget->size().height()-logTable->size().height()
                               );
    buttonsWidget->setStyleSheet(("background-color:#E5F1FB;"));


    int b_x = 0;
    int b_y = 50;
    int b_width = 35;
    int b_height = 25;

    pageTextLabel = new QLabel(buttonsWidget);
    pageTextLabel->setObjectName(QStringLiteral("pageTextLabel"));
    pageTextLabel->setGeometry(QRect(90, b_y,200 , b_height));
    pageTextLabel->setText("共 10 页  当前第 1 页");

    b_x = 440;
    pageUpButton = new QPushButton(buttonsWidget);
    pageUpButton->setObjectName(QStringLiteral("pageUpButton"));
    pageUpButton->setGeometry(QRect(b_x, b_y, 40, b_height));
    pageUpButton->setText("首页");
    pageUpButton->setFlat(true);

    b_x = pageUpButton->size().width()+pageUpButton->x() + 20;
    forwardButton = new QPushButton(buttonsWidget);
    forwardButton->setObjectName(QStringLiteral("forwardButton"));
    forwardButton->setGeometry(QRect(b_x, b_y, 40, b_height));
    forwardButton->setText("上一页");
    forwardButton->setFlat(true);

    b_x = forwardButton->size().width()+forwardButton->x() + 20;
    currentPageLabel = new QLabel(buttonsWidget);
    currentPageLabel->setObjectName(QStringLiteral("currentPageLabel"));
    currentPageLabel->setGeometry(QRect(b_x, b_y, 30, b_height));
    currentPageLabel->setText("- 1 -");

    b_x = currentPageLabel->size().width()+currentPageLabel->x() + 20;
    nextButton = new QPushButton(buttonsWidget);
    nextButton->setObjectName(QStringLiteral("nextButton"));
    nextButton->setGeometry(QRect(b_x, b_y, 40, b_height));
    nextButton->setText("下一页");
    nextButton->setFlat(true);

    b_x = nextButton->size().width()+nextButton->x() + 20;
    pageDownButton = new QPushButton(buttonsWidget);
    pageDownButton->setObjectName(QStringLiteral("pageDownButton"));
    pageDownButton->setGeometry(QRect(b_x, b_y, 40, b_height));
    pageDownButton->setText("末页");
    pageDownButton->setFlat(true);
}


void LogWidget::getAllData()
{
    for(int i=0;i<50;i++)
    {
        LogInfo info;
        logInfos.append(info);
    }
}

void LogWidget::setUserCombox()
{
    QIcon userIcon;
    userIcon.addFile(":/image/Filelist/user.png");
    userBox->clear();
    userBox->addItem(userIcon,"全部用户",-1);
    for(int i =0;i<5;i++)
    {
        userBox->addItem(userIcon,QString::asprintf("userId_%d",i),i);
    }
}


void LogWidget::setLevelCombox()
{
    QIcon levelIcon;
    levelIcon.addFile(":/image/Filelist/level.png");
    levelBox->clear();
    levelBox->addItem(levelIcon,"所有级别",-1);
    levelBox->addItem(levelIcon,"INFO",1);
    levelBox->addItem(levelIcon,"DEBUG",2);
    levelBox->addItem(levelIcon,"ERROE",3);
    levelBox->addItem(levelIcon,"WARNING",4);
    levelBox->addItem(levelIcon,"FATAL",5);
}

void LogWidget::insertData()
{
    LogInfo info;
    for(int i=0;i<10;i++)
    {
        info = logInfos[i];
        int user_id = info.user_id;
        int level = info.level;
        QString log_time = QString::fromLocal8Bit(info.log_time);
        QString log_info = QString::fromLocal8Bit(info.log_info);

        logTable->setItem(i,0,new QTableWidgetItem(QString::number(user_id)));
        logTable->setItem(i,1,new QTableWidgetItem(QString::number(level)));
        logTable->setItem(i,2,new QTableWidgetItem(log_time));
        logTable->setItem(i,3,new QTableWidgetItem(log_info));
    }
}

void LogWidget::resetDataSlot()
{
    emit logRequestDataSignal(userId,logLevle,sortStatus);
}


QPixmap LogWidget::getPixmap(int sortType)
{
    QString path = ":/image/Filelist/log_sort.png";
    QPixmap icon(path);
    QPixmap res(icon.copy(18*(sortType),0,18,18));
    sortStatus = sortType;
    return res;
}


void LogWidget::timeButtonCliecked()
{
    if(sortStatus==1)
    {
        sortStatus=0;
    }
    else
    {
        sortStatus=1;
    }
    timeSort->setPixmap(getPixmap(sortStatus));
    resetDataSlot();
}


//收到服务器最新数据
void LogWidget::recvNewLoginfoSlot(LogStruct *allData)
{

}

void LogWidget::recvLogUserIdSlot(LogUserId *allData)
{
    for(int i=0;i<allData->count;i++)
    {
        qDebug()<<"userId = "<<allData->allId[i];
    }
}

void LogWidget::userBoxChange(int index)
{
    int result = userBox->itemData(index).toInt();
    userId = result;
    resetDataSlot();
}

void LogWidget::levelBoxChange(int index)
{
    int result = levelBox->itemData(index).toInt();
    logLevle = result;
    resetDataSlot();
}
