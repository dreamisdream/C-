#include "sharefiledialog.h"
#include "ui_sharefiledialog.h"
#include "data.h"
#include <QMessageBox>

ShareFileDialog::ShareFileDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFileDialog)
{
    init();
    code =nullptr;
}

ShareFileDialog::~ShareFileDialog()
{
    delete ui;
}

void ShareFileDialog::init()
{
    ui->setupUi(this);
    ui->lab2->hide();
    ui->lab4->hide();
    ui->horizontalLayout->insertSpacing(1,20);
    ui->horizontalLayout_2->insertSpacing(1,20);
    connect(ui->pushButton_2,&QPushButton::clicked,[=]{
        this->hide();
    });
    connect(ui->pushButton,&QPushButton::clicked,this,&ShareFileDialog::createLink);

    QButtonGroup m_pBtnGroup;
    this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);
    m_pBtnGroup.addButton(ui->radioButton,0);
    m_pBtnGroup.addButton(ui->radioButton_2,1);
    m_pBtnGroup.setExclusive(false);
    this->setAttribute(Qt::WA_ShowModal,true);
}

QString ShareFileDialog::getShareCode(int x, int y)
{
    int z =x*10+y*11;
    z+=12;
    z*=13456;
    z%=10000;
    code = QString::number(z);
    ui->lab4->setText(code);
    return code;
}

void ShareFileDialog::createLink()
{
    if (code == nullptr)
    {
        QMessageBox::critical(this,"缺失数据","没有获取到文件ID或者用户ID！");
        return;
    }
    ui->lab1->setText("分享链接");
    ui->lab2->show();
    ui->lab3->setText("分享码");
    ui->lab4->show();
    ui->radioButton->hide();
    ui->radioButton_2->hide();
    ui->pushButton->hide();
    ui->pushButton_2->setText("关闭");
}
