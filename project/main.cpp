#include "mainwindow.h"
#include <QApplication>
#include "classwizard.h"
#include <QTextCodec>
#include <qtextcodec.h>
//#include "xlsxdocument.h"
#include "mainwidget.h"
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <qmessagebox.h>
#include <qdir.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString path;
    QDir dir;
    path=dir.currentPath();
//    QMessageBox::warning(0,"PATH",path,QMessageBox::Yes);

    MainWindow mainWin;
//    foreach (const QString &fileName, parser.positionalArguments())
//        mainWin.openFile(fileName);
    mainWin.show();
    return app.exec();
}
