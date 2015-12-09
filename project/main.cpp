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
#include <QDateTime>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString path;
    QDir dir;
    path=dir.currentPath();
//    QMessageBox::warning(0,"PATH",path,QMessageBox::Yes);

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");

        QTextCodec *xcodec = QTextCodec::codecForLocale() ;
        QString exeDir = xcodec->toUnicode( QByteArray(argv[0]) ) ;
        QString BKE_CURRENT_DIR = QFileInfo(exeDir).path() ;
        QStringList  libpath;
        libpath << BKE_CURRENT_DIR+QString::fromLocal8Bit("s/platforms");    libpath << BKE_CURRENT_DIR <<BKE_CURRENT_DIR+QString::fromLocal8Bit("/plugins/imageformats");    libpath << BKE_CURRENT_DIR+QString::fromLocal8Bit("/plugins");    libpath << QApplication::libraryPaths();    QApplication::setLibraryPaths(libpath) ;
    if(current_date.startsWith("2016"))
        return 0 ;
    MainWindow mainWin;

//    foreach (const QString &fileName, parser.positionalArguments())
//        mainWin.openFile(fileName);
    mainWin.show();
    return app.exec();
}
