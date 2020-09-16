#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    MyButton(QWidget*parent = nullptr);
protected:
    void  enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

signals:
     void mouse_in();
     void mouse_out();
};

#endif // MYBUTTON_H
