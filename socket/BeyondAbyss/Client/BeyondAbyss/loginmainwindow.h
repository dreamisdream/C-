#ifndef LOGINMAINWINDOW_H
#define LOGINMAINWINDOW_H

#include "registermainwindow.h"
#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTcpSocket>
#include <QRegExp>
#include <QMouseEvent>
#include <QCryptographicHash>  //md5加密的库

#include "data.h"
#include "client.h"

namespace Ui {
class LoginMainWindow;
}

class LoginMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginMainWindow(QWidget *parent = 0);
    ~LoginMainWindow();

    QFile *fp;//保存密码到本地文件对象
    void getLoginClient( Client *m_client);
    void mousePressEvent(QMouseEvent *event);//实现窗口移动
    void mouseMoveEvent(QMouseEvent *event);
    QString getMD5(QString passWd);

private slots:
    void on_CloseButton_clicked();
    void on_MinWindowButton_clicked();
    void on_MaxWindowButton_clicked();
    void on_registerNewUserPushButton_clicked();

    void loginPushButton(bool checked);



private:
    Ui::LoginMainWindow *ui;
    QTcpSocket *m_tcpSocket;
    //登录界面参数设置
    void setUsernameAndPassword();
    //保存密码文件
    void saveLoginPassword();
    //发送密码账号函数
    void sendUsernamePassword();

    Client *m_loginClient;
    QPoint m_offset_login;
    QRegExp exp;
signals:
    void loginSucceed();
    void loginToRegisterSignals();


};

#endif // LOGINMAINWINDOW_H
