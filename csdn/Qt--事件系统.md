# Qt�¼�ϵͳ
    ����⣺
        qt�����¼�ϵͳ�ǻ����¼�ѭ����,Qt���������һ���¼�ѭ��exec(),ͨ��exit()�˳��¼�ѭ��,�¼�������¼�������,������¼��ͻ᲻ͣ�Ĵ���ȥ����,���û���¼��ͻ�����,��socket���첽ģ����Щ����;
1.Qt�е��¼����źŵ�����
    **����ֵ**:�¼��������������true,������¼��������,QApplication�����������һ���¼�,�������false,��ô�¼����������Ѱ����һ�����Դ�����¼��ĵ�ע�᷽��;���źŵķ���ֵ��û���õ�;
    **���ȼ�**:�¼���Ϊ�����봰����ص�,�����¼��ص����Ǵӵ�ǰ���ڿ�ʼ,һ��һ�������ɷ�,֪����һ�����ڷ���true;
    ���źŵĴ���˳�����źŲ۹�����˳���й�(QT);
    **����ʽ**:�¼�ʹ����һ���¼�������ά��,���µ��¼�����ӵ���β,���������,�����´���ͷ��������;���źŴ��������̻ص���;
2.�¼��Ľ��պʹ����� 
    // ����˳�����·� 
    bool QCoreAppliction::sendEvent(QObject *receiver,QEvent *event); //ͬ��
    void QCoreAppliction::postEvent(QObject* receiver,QEvent *event);   //�����¼����� �첽
    bool QCoreApplication::notify(QObject *receiver,QEvent *event);
    bool QObject::eventFilter(QObject *watched,QEvent *event);
    bool QObject::event(QEvent *event);
    void QWidget::keyPressEvent(QKeyEvent *event);
    
    �¼���������ַ�ʽ:
    ---
        // 345 �Ƚϳ���
        1.��дnotify() 
        2.��QCoreAppliction installEventFilter(); 
        3.������Ŀؼ�installEventFilter();
        4.��дQEvent();
        5.��д������¼�keyPressEvent();
    ---
    ##��д������¼�:
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
    ##��дQEvent()
    // ��갴�¶�̬��ʾ ����λ��
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
    ##��װ�¼������� eventfilter()
    
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
    ## �Զ����¼�
    
    

    









































    �ο����ӣ�https://www.cnblogs.com/weizhixiang/p/5861138.html