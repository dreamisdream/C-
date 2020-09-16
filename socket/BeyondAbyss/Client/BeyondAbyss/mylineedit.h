#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MyLineEdit(QWidget* parent = nullptr);
signals:
    void mousePress();
protected:
    void mousePressEvent(QMouseEvent *e);
};

#endif // MYLINEEDIT_H
