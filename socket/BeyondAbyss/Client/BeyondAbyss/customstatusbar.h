#ifndef CUSTOMSTATUSBAR_H
#define CUSTOMSTATUSBAR_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>

class CustomStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit CustomStatusBar(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QIcon cutIcon(const QString & path);
signals:

public slots:

public:
   QLabel *m_lable;
   QPushButton *MainWidgetButton;
   QPushButton *transferListButton;
   QPushButton *shareListButton;
   QPushButton *logListButton;
   QPushButton *SuperSUProButton;
   QPushButton *minimizedButton;
   QPushButton *CloseButton;
   QHBoxLayout *layout;
   QPoint offset;
};

#endif // CUSTOMSTATUSBAR_H
