#ifndef SHAREFILEDIALOG_H
#define SHAREFILEDIALOG_H

#include <QWidget>

namespace Ui {
class ShareFileDialog;
}

class ShareFileDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ShareFileDialog(QWidget *parent = 0);
    ~ShareFileDialog();
    void init();
    QString getShareCode(int x,int y);
signals:

public slots:
    void createLink();
private:
    Ui::ShareFileDialog *ui;
    QString code;
};

#endif // SHAREFILEDIALOG_H
