#include "mylineedit.h"

MyLineEdit::MyLineEdit(QWidget* parent):QLineEdit(parent)
{

}

void MyLineEdit::mousePressEvent(QMouseEvent *e)
{
    emit mousePress();
    QLineEdit::mousePressEvent(e);
}

