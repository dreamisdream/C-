#ifndef SHAREFILEWIDGET_H
#define SHAREFILEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>

class ShareFileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShareFileWidget(QWidget *parent = nullptr);
    void init();
signals:
    void updateShareInfo();
    void ShareCodeDownLoadFile();
public slots:
    void appendRow(int x,int y,QString z);
    void updataInfo();
    void customMenuRequested(QPoint pos);
    void downloadSlot();

private:
    QPushButton * m_pBtnUpdata;
public:
    QTableWidget *m_tabWidget;
    QTableWidgetItem* selecteditem;
};

#endif // SHAREFILEWIDGET_H
