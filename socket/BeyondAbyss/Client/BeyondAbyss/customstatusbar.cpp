#include "customstatusbar.h"

CustomStatusBar::CustomStatusBar(QWidget *parent) : QWidget(parent)
{
    this->resize(1100,80);
    this->setFont(QFont("Microsoft YaHei", 10, 50));

    layout = new QHBoxLayout(this);

    m_lable = new QLabel(this);
    m_lable->setText("beyondabyss\n跃渊");
    m_lable->setAlignment(Qt::AlignCenter);
    m_lable->setFixedSize(200,80);
    m_lable->setFont(QFont("Microsoft YaHei", 10, 50,true));

    MainWidgetButton = new QPushButton(this);
    MainWidgetButton->setText("我的网盘");
    MainWidgetButton->setFixedSize(100,80);

    transferListButton = new QPushButton(this);
    transferListButton->setText("传输列表");
    transferListButton->setFixedSize(100,80);

    shareListButton = new QPushButton(this);
    shareListButton->setText("分享列表");
    shareListButton->setFixedSize(100,80);

    logListButton = new QPushButton(this);
    logListButton->setText("日志管理");
    logListButton->setFixedSize(100,80);

    SuperSUProButton = new QPushButton(this);
    SuperSUProButton->setText("权限管理");
    SuperSUProButton->setFixedSize(100,80);

    minimizedButton = new QPushButton(this);
    QIcon icon1 = cutIcon(":/image/MainWindowImage/btn_sys_minimize.png");
    minimizedButton->setIcon(icon1);
    minimizedButton->setIconSize(minimizedButton->size());
    minimizedButton->setMinimumSize(28,24);
    minimizedButton->setMaximumSize(28,24);

    CloseButton = new QPushButton(this);
    QIcon icon2 = cutIcon(tr(":/image/MainWindowImage/btn_sys_close.png"));
    CloseButton->setIcon(icon2);
    CloseButton->setIconSize(CloseButton->size());
    CloseButton->setMinimumSize(28,24);
    CloseButton->setMaximumSize(28,24);

    layout->addWidget(m_lable);
    layout->addWidget(MainWidgetButton);
    layout->addWidget(transferListButton);
    layout->addWidget(shareListButton);
    layout->addWidget(logListButton);
    layout->addWidget(SuperSUProButton);
    layout->addStretch();
    layout->addWidget(minimizedButton,0,Qt::AlignTop);
    layout->addWidget(CloseButton,0,Qt::AlignTop);

}

void CustomStatusBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)       // 如果是鼠标左键按下
    {
        QWidget *parentWidget = this->parentWidget();
        QPoint parentPoint = parentWidget->pos();
        offset = event->globalPos() - parentPoint;
        // 获取指针位置和窗口位置的差值
    }
}

void CustomStatusBar::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {

        QWidget *parentWidget = this->parentWidget();
        QPoint temp;
        temp =event->globalPos() - offset;
        parentWidget->move(temp);
    }
}

QIcon CustomStatusBar::cutIcon(const QString & path)
{
    QPixmap icon(path);
    QIcon res(icon.copy(0,0,34,20));
    return res;
}
