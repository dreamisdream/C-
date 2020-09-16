#include "loginmainwindow.h"
#include "ui_loginmainwindow.h"

LoginMainWindow::LoginMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginMainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);//取消边框
    LoginMainWindow::setUsernameAndPassword();

    //信号槽
    connect(ui->loginPushButton,SIGNAL(clicked(bool)),SLOT(loginPushButton(bool)));
}

LoginMainWindow::~LoginMainWindow()
{
    delete ui;
}
//关闭窗口
void LoginMainWindow::on_CloseButton_clicked()
{
     this->close();
}

//最小化窗口
void LoginMainWindow::on_MinWindowButton_clicked()
{
    this->showMinimized();
}

//最大化窗口
void LoginMainWindow::on_MaxWindowButton_clicked()
{
    this->showMaximized();
}

//窗口移动
void LoginMainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()  ==Qt::LeftButton)
    {
        //移动准备 先记录当前鼠标偏移量
        m_offset_login = event->globalPos()-this->pos();
    }
}

void LoginMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint temp;
        temp = event->globalPos() - m_offset_login;
        move(temp);
    }
}



//登录界面参数设置
void LoginMainWindow::setUsernameAndPassword()
{
    ui->loginPushButton->setShortcut(Qt::Key_Return);
    ui->usernamelineEdit2->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    ui->usernamelineEdit2->setPlaceholderText("请输入用户账号");
    ui->usernamelineEdit2->setFocus();
    ui->passwordlineEdit2->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    ui->passwordlineEdit2->setPlaceholderText("请输入用户密码");
    ui->passwordlineEdit2->setEchoMode(QLineEdit::Password);//密码隐式设置
}

//登录按钮
void LoginMainWindow::loginPushButton(bool checked)
{
    //TODO 登录逻辑本地屏蔽
    bool flag = false;
    if(flag)
    {
        emit loginSucceed();
    }
    else
    {
        //账号密码由数字和字母组成，总共8-14位，
        exp.setPatternSyntax(QRegExp::RegExp);
        exp.setCaseSensitivity(Qt::CaseInsensitive);//对大小写字母敏感，即区分大小写
        exp.setPattern(QString("^[A-Za-z0-9]{7,13}$"));
        QString UserName = ui->usernamelineEdit2->text();
        QString pwd = ui->passwordlineEdit2->text();
        if(pwd.isEmpty() || UserName.isEmpty()) //检测账号输入框是不是为空
        {
            qDebug()<<"Password Or Username cant be empty";
            ui->UsernameAndPasswordTipLable->setText("Password Or Username cant be empty!");
            ui->UsernameAndPasswordTipLable->setStyleSheet("color: rgb(255, 78, 25);");
        }
        else
        {
            if(exp.exactMatch(pwd) && exp.exactMatch(UserName))
            {
                ui->UsernameAndPasswordTipLable->hide();
                qDebug()<<"Password right";
                qDebug()<<"我按登录按钮了";
                sendUsernamePassword();
                //郝玉同
                //emit loginSucceed();
            }
            else
            {
                ui->UsernameAndPasswordTipLable->setText("Sorry,The password Or Username format is wrong!");
                ui->UsernameAndPasswordTipLable->setStyleSheet("color: rgb(255, 78, 25);");
                qDebug()<<"Sorry,The password format is wrong!Please reenter your password.";
            }
        }
    }
}

void LoginMainWindow::getLoginClient(Client *client)
{
    m_loginClient = client;
}



//保存密码文件
void LoginMainWindow::saveLoginPassword()
{
    QFile *fp= new QFile("saveLoginPassword.txt");
    if ( fp->exists())
    {
        return;
    }
    fp->open( QIODevice::ReadWrite | QIODevice::Text );
    QString passwordText;
    passwordText=ui->passwordlineEdit2->text();
    fp->write("passwordText");
    QString buf;
    buf=fp->readLine();
    qDebug()<<buf;
    fp->close();
    fp->deleteLater();
}


QString LoginMainWindow::getMD5(QString passWd)
{
    QString MD5;
   // QString passWd="123";
    QByteArray str;
    str = QCryptographicHash::hash(passWd.toLatin1(),QCryptographicHash::Md5);
    MD5.append(str.toHex());
    return MD5;
}




//发送密码账号
void LoginMainWindow::sendUsernamePassword()
{
    QString loginUserName;
    loginUserName = ui->usernamelineEdit2->text();

    QString loginPassWord;
    QString str= ui->passwordlineEdit2->text();
    loginPassWord = getMD5(str);

    UserInfo d1;
    d1.type = LOGIN;
    d1.len = sizeof(UserInfo);
    strcpy_s(d1.userName,loginUserName.toLocal8Bit().data());
    strcpy_s(d1.password,loginPassWord.toLocal8Bit().data());
    m_loginClient->sendToServer((char*)&d1,d1.len);
}
//跳转到注册界面
void LoginMainWindow::on_registerNewUserPushButton_clicked()
{
    emit loginToRegisterSignals();
}
