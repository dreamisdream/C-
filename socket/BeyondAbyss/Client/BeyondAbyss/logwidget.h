#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QVector>
#include <QDebug>

#include "data.h"


class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = nullptr);
    void initOther();
    void init();

    void getAllData();
    void setUserCombox();
    void setLevelCombox();
private:
    int sortStatus;
    int logLevle;
    int userId;

    QPixmap getPixmap(int sortType = 0);
    QVector<LogInfo> logInfos;
    QMap<int,QString> levelVector;
    void insertData();
public:
    QWidget *centralWidget;
    QWidget *titleWidget;
    QWidget *buttonsWidget;
    QComboBox *userBox;
    QComboBox *levelBox;
    QPushButton* timeButton;
    QLabel* timeLabel;
    QLabel* timeSort;
    QTableWidget *logTable;
    QPushButton *pageUpButton;
    QPushButton *forwardButton;
    QPushButton *nextButton;
    QPushButton *pageDownButton;
    QLabel *logInfoLable;
    QLabel *pageTextLabel;
    QLabel *currentPageLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
signals:    
    void logRequestDataSignal(int,int,int);
    void getAllUserId();
public slots:
    void userBoxChange(int index);
    void levelBoxChange(int index);
    void resetDataSlot();
    void timeButtonCliecked();
    void recvNewLoginfoSlot(LogStruct* allData);
    void recvLogUserIdSlot(LogUserId* allData);
};

#endif // LOGWIDGET_H
