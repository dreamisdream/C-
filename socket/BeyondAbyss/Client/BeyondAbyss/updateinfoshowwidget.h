#ifndef UPDATEINFOSHOWWIDGET_H
#define UPDATEINFOSHOWWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>


class UpdateInfoShowWidget : public QFrame
{
    Q_OBJECT
public:
    explicit UpdateInfoShowWidget(const QString& filePath, qint64 fileSize, QWidget *parent = nullptr);

    QIcon cutIcon(const QString & path,QRect rect = QRect(0,0,20,20));

    void setIconLable(const QPixmap &icon);
    void setFileNameLable(const QString &fileName = "filename");

    void start(int taskId, quint16 port);
private:
    QString readableFilesize( quint64 filesize);
signals:
    void updateProgress(qint64,qint64);
    void updateTextStatus(const QString& text);
public slots:
    void changeStartOrStopButtonIcon(bool checked);
    void setFileSizelable(qint64 totalbytes = 0,qint64 bytesWritten = 0);
    void setCurrentProgress(qint64 totalbytes = 0,qint64 bytesWritten = 0);
    void setCurrentStatus(const QString &status = "等待上传...");
private:
    void uploadThread(int taskId, quint16 port);
private:
    QLabel *iconLable;
    QLabel *fileNameLable;
    QLabel *fileSizelable;
    QLabel *currentProgress;
    QLabel *currentStatus;
    QPushButton *startOrStop;
    QPushButton *countermand;
    QPushButton *parentFolder;
    QVBoxLayout *layout1;
    QVBoxLayout *layou2;
    QHBoxLayout *layout3;
    QHBoxLayout *layout4;
    QString      m_filePath;
    QString      m_fileName;
    qint64       m_fileSize;
};

#endif // UPDATEINFOSHOWWIDGET_H
