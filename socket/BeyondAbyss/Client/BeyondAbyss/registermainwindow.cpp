#include "registermainwindow.h"
#include "ui_registermainwindow.h"

RegisterMainWindow::RegisterMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterMainWindow)
{
    ui->setupUi(this);
    m_succeedNumber = 0;
    setRegisteWindow();
}

RegisterMainWindow::~RegisterMainWindow()
{
    delete ui;
}
//窗口参数设置
void RegisterMainWindow::setRegisteWindow()
{
     this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
     ui->UsernamelineEdit->setPlaceholderText("请设置用户名");
     ui->passwprdlineEdit->setPlaceholderText("请设置用户密码");
     ui->identityCodelineEdit->setPlaceholderText("请输入身份证号");
     ui->phonenumberlineEdit->setPlaceholderText("请输入手机号码");
     ui->EmaillineEdit->setPlaceholderText("请输入邮箱");
}

//拖动窗口
void RegisterMainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()  ==Qt::LeftButton)
    {
        //移动准备 先记录当前鼠标偏移量
        m_offset_register = event->globalPos()-this->pos();
    }
}

void RegisterMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint temp;
        temp = event->globalPos() - m_offset_register;
        move(temp);
    }
}


//登录按钮
void RegisterMainWindow::on_loginPushButton_clicked()
{
    emit registerToLoginSignals();
}

//关闭注册窗口
void RegisterMainWindow::on_closePushButton_clicked()
{
    this->close();
}


//输入参数正则匹配
void RegisterMainWindow::setInputParamaterMethod()
{
    m_succeedNumber = 0;
    registerExp.setPatternSyntax(QRegExp::RegExp);
    registerExp.setCaseSensitivity(Qt::CaseInsensitive);//对大小写字母敏感，即区分大小写
    judgeRegisterUsername();//用户名
    judgeRegisterPassward();//密码
    judgeRegisterIdentityCode();//身份证号
    judgeRegisterPhonenumber();//电话号码
    judgeRegisterEmail();//邮箱
}

//用户名匹配
void RegisterMainWindow::judgeRegisterUsername()
{
    //账号由数字和字母组成，以字母开头，总共8-14位，
    registerExp.setPattern(QString("^[A-Za-z0-9]{7,13}$"));
    judgeInputParamatermethod(ui->UsernamelineEdit, ui->usernameTiplabel);
}
//密码匹配
void RegisterMainWindow::judgeRegisterPassward()
{
    //密码由数字和字母组成，以字母开头，总共8-14位，
    registerExp.setPattern(QString("^[A-Za-z0-9]{7,13}$"));
    judgeInputParamatermethod(ui->passwprdlineEdit, ui->passwordTiplabel);
}

//身份证号匹配
void RegisterMainWindow::judgeRegisterIdentityCode()
{
    registerExp.setPattern(QString("^[0-9]{18}$"));
    judgeInputParamatermethod(ui->identityCodelineEdit, ui->identityCodeTiplabel);
}

//电话号码匹配
void RegisterMainWindow::judgeRegisterPhonenumber()
{
    registerExp.setPattern(QString("^[0-9]{11}$"));
    judgeInputParamatermethod(ui->phonenumberlineEdit, ui->phoneNumberTiplabel);
}

//邮箱匹配
void RegisterMainWindow::judgeRegisterEmail()
{
    registerExp.setPattern(QString("^[a-zA-Z0-9]+@[a-zA-Z0-9]+[\.][a-zA-Z0-9]+$"));
    judgeInputParamatermethod(ui->EmaillineEdit, ui->emailTiplabel);
}

void RegisterMainWindow::getRegisterClient(Client *client)
{
   m_registerClient = client;
}

//判断输入参数是否正确
void RegisterMainWindow::judgeInputParamatermethod(QLineEdit *line, QLabel *lable)
{

    if(line->text().isEmpty()) //检测账号输入框是不是为空
    {
        qDebug()<<"Password Or Username cant be empty";
        lable->setText("cant be empty!");
        lable->setStyleSheet("color: rgb(255, 78, 25);");
    }
    else
    {
        if( registerExp.exactMatch(line->text()))
        {
            lable->hide();
            m_succeedNumber ++;
            qDebug()<<" right";
        }
        else
        {
            lable->setText("Sorry,The format is wrong!");
            lable->setStyleSheet("color: rgb(255, 78, 25);");
            qDebug()<<"Sorry,The  format is wrong!Please reenter your password.";
        }
    }
}


QString RegisterMainWindow::getMD5(QString passWd)
{
    QString MD5;
   // QString passWd="123";
    QByteArray str;
    str = QCryptographicHash::hash(passWd.toLatin1(),QCryptographicHash::Md5);
    MD5.append(str.toHex());
    return MD5;

}



//发送数据
void RegisterMainWindow::sendReristerParamter()
{

    QString registerUserName;
    registerUserName = ui->UsernamelineEdit->text();

    QString registerPassWord;
    QString str =  ui->passwprdlineEdit->text();
    registerPassWord = getMD5(str);;//密文转换
    qDebug()<<"registerPassWord="<<registerPassWord;

    QString registerIdentityCode;
    registerIdentityCode = ui->identityCodelineEdit->text();
    QString registerPhoneNumber;
    registerPhoneNumber = ui->phonenumberlineEdit->text();
    QString registerEmail;
    registerEmail = ui->EmaillineEdit->text();

    RegisterInfo S1;
    S1.type = REGISTER;
    S1.len = sizeof(RegisterInfo);
    strcpy_s(S1.userName,registerUserName.toLocal8Bit().data());
    strcpy_s(S1.password,registerPassWord.toLocal8Bit().data());
    strcpy_s(S1.phoneNumber,registerPhoneNumber.toLocal8Bit().data());
    strcpy_s(S1.identityCode,registerIdentityCode.toLocal8Bit().data());
    strcpy_s(S1.Email,registerEmail.toLocal8Bit().data());
    m_registerClient->sendToServer((char*)&S1,S1.len);
}



void RegisterMainWindow::on_registerpushButton_clicked()
{
    setInputParamaterMethod();
    if(m_succeedNumber == 5)
    {
        sendReristerParamter();
       // emit registerSucceedToLoginSignal();
        qDebug()<<"succeedNumber"<<5<<"注册成功";
    }
}
