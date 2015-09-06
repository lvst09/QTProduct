#include "mainwindow.h"
#include <QApplication>
#include "classwizard.h"
#include <QTextCodec>
#include <qtextcodec.h>
//#include "xlsxdocument.h"
#include "mainwidget.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);


    MainWindow mainWin;
//    foreach (const QString &fileName, parser.positionalArguments())
//        mainWin.openFile(fileName);
    mainWin.show();
    return app.exec();
}
