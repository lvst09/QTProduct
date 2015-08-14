#ifndef MAINDWIDGET_H
#define MAINDWIDGET_H
#include <QMainWindow>
#include <QObject>
class QMdiArea;
class mainWidget:public QMainWindow{
    Q_OBJECT
public:
    mainWidget(QWidget *parent=0);

private :
        QMdiArea *workSpace;

};

#endif // MAINDWIDGET_H
