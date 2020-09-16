#include "mybutton.h"

MyButton::MyButton(QWidget*parent):QPushButton(parent)
{

}

void MyButton::enterEvent(QEvent *e)
{
    emit mouse_in();
    QPushButton::enterEvent(e);
}

void MyButton::leaveEvent(QEvent *e)
{
    emit mouse_out();
    QPushButton::leaveEvent(e);
}


