#include <QApplication>
#include <QMdiArea>
#include <QTextEdit>
#include "mainWidget.h"
#include "classwizard.h"
#include <QVBoxLayout>
mainWidget::mainWidget(QWidget *parent):QMainWindow(parent){
    //创建一个 QWorkspace 对象
    workSpace =new QMdiArea(this);
    //设置主窗体的中央窗口为QWorkspace对象 实现窗口多布局方式
    setCentralWidget(workSpace);

    //创建窗口一
    QMainWindow *window1=new QMainWindow(this);
    window1->setWindowTitle(tr("window 1"));
    QTextEdit *edit1 = new QTextEdit(this);
    edit1->setText("window 1");
    window1->setCentralWidget(edit1);
//创建窗口2
    QMainWindow *window2=new QMainWindow(this);
    window2->setWindowTitle(tr("window 2"));
    QTextEdit *edit2=new QTextEdit(this);
    edit2->setText("window 2");
    window2->setCentralWidget(edit2);
//创建窗口3
    QMainWindow *window3=new QMainWindow(this);
    window3->setWindowTitle(tr("window 3"));
//    QTextEdit *edit3=new QTextEdit(this);
//    window3->setCentralWidget(edit3);

//    ClassWizard wizard(window3);
    QWidget * widget = new QWidget();
//    InfoPage * page = new InfoPage;
    ClassWizard * wizard = new ClassWizard(widget);
//    widget->resize(100,200);



    wizard->resize(1000,800);

    window3->setCentralWidget(widget);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(widget);
//    QHBoxLayout *mainLayout=new QHBoxLayout;
//    mainLayout->addLayout(layout);
    window3->setLayout(layout);

    wizard->show();

    //window3->setCentralWidget(wizard);//
//将子窗口插入到 workSpace中 实现多窗口

//    workSpace->addSubWindow(window1);/*
//    workSpace->addSubWindow(window2);*/
    workSpace->addSubWindow(window3);
}

