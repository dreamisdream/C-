# Qt事件系统
    简单理解：
        qt整个事件系统是基于事件循环的,Qt主界面就是一个事件循环exec(),通过exit()退出事件循环,事件会放在事件队列里,如果有事件就会不停的触发去处理,如果没有事件就会阻塞,和socket的异步模型有些类似;
1.Qt中的事件与信号的区别
    **返回值**:事件处理函数如果返回true,则这个事件处理完成,QApplication会继续处理下一个事件,如果返回false,那么事件会继续向上寻找下一个可以处理该事件的的注册方法;而信号的返回值是没有用的;
    **优先级**:事件因为都是与窗口相关的,所有事件回调都是从当前窗口开始,一级一级向上派发,知道有一个窗口返回true;
    而信号的处理顺序与信号槽关联的顺序有关(QT);
    **处理方式**:事件使用了一个事件队列来维护,有新的事件就添加到队尾,处理完队列,就重新从列头继续处理;而信号处理是立刻回调的;
2.事件的接收和处理方法 
    // 按照顺序逐级下发 
    bool QCoreAppliction::sendEvent(QObject *receiver,QEvent *event); //同步
    void QCoreAppliction::postEvent(QObject* receiver,QEvent *event);   //插入事件队列 异步
    bool QCoreApplication::notify(QObject *receiver,QEvent *event);
    bool QObject::eventFilter(QObject *watched,QEvent *event);
    bool QObject::event(QEvent *event);
    void QWidget::keyPressEvent(QKeyEvent *event);
    
    事件处理的五种方式:
    ---
        // 345 比较常用
        1.重写notify() 
        2.给QCoreAppliction installEventFilter(); 
        3.给具体的控件installEventFilter();
        4.重写QEvent();
        5.重写具体的事件keyPressEvent();
    ---
    ##重写具体的事件:
    ---
    #include <QKeyEvent>
    void keyPressEvent(QKeyEvent *event){
        if(0x30 <= event->key() && 0x39 >=event->key()){
            m_pLab->setStyleSheet("color:red");
            m_pLab->setText(QString("key is %1").arg(event->key()-48));
        }else{
            m_pLab->setText("Please enter from 0 to 9");
        }
    }
    void keyReleaseEvent(QKeyEvent *event){
        if(event->key()){
            m_pLab->setStyleSheet("color:red;font:20px");
        }
    }
    ---
    ##重写QEvent()
    // 鼠标按下动态显示 鼠标的位置
    #include <QEvent>
    #include <QMouseEvent>
    bool widget::event(QEvent *event)
    {
        if(event->type() == QEvent::MouseMove){
            QMouseEvent *e = static_cast<QMouseEvent *>(event);
            m_pBtnTxt->setText(QString("QkeyPress:%1,%2").arg(e->x()).arg(e->y()));
            return true;
        }
        return QWidget::event(event);
    }
    ##安装事件过滤器 eventfilter()
    
    m_pLE->installEventFilter(this);    
    bool widget::eventFilter(QObject *watched, QEvent *event)
    {
        if(watched == this->m_pLE){
            QMouseEvent *e = static_cast<QMouseEvent *>(event);
            if(e->type() == QEvent::MouseButtonDblClick)
            {
                QString temp = m_pLE->text();
                temp+="Enter";
                m_pLE->setText(temp);
                return true;
            }
        }
        return QWidget::eventFilter(watched,event);
    }
    ## 自定义事件
    
    

    









































    参考链接：https://www.cnblogs.com/weizhixiang/p/5861138.html