#ifndef REGISTERMAINWINDOW_H
#define REGISTERMAINWINDOW_H


#include <QMainWindow>
#include "loginmainwindow.h"
#include <QCryptographicHash>  //md5加密的库
#include <QMouseEvent>
#include <QRegExp>
#include <QLineEdit>
#include <QLabel>
#include <client.h>

namespace Ui {
class RegisterMainWindow;
}

class RegisterMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterMainWindow(QWidget *parent = 0);
    ~RegisterMainWindow();
    QRegExp registerExp;
    void setRegisteWindow();//设置窗口参数
    void mousePressEvent(QMouseEvent *event);//实现窗口移动
    void mouseMoveEvent(QMouseEvent *event);

    void setInputParamaterMethod();//匹配窗口输入
    void judgeInputParamatermethod(QLineEdit *line,QLabel *lable);
    void judgeRegisterUsername();
    void judgeRegisterPassward();
    void judgeRegisterIdentityCode();
    void judgeRegisterPhonenumber();
    void judgeRegisterEmail();

    void getRegisterClient(Client  *client);

    void sendReristerParamter();

    QString getMD5(QString passWd);



private slots:
    void on_loginPushButton_clicked();
    void on_closePushButton_clicked();
    void on_registerpushButton_clicked();

private:
    Ui::RegisterMainWindow *ui;
    QPoint m_offset_register;
    int m_succeedNumber;
    Client *m_registerClient;


signals:
    void registerToLoginSignals();
    void registerSucceedToLoginSignal();


};

#endif // REGISTERMAINWINDOW_H
