#include "login.h"
#include <qlineedit.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <QBoxLayout>
#include <qpushbutton.h>
#include "mainwindow.h"
login::login(QWidget *parent) :
    QWidget(parent)
{
    this->setParent(parent);
    this->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/images/chiko/login.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);

    QLineEdit * edt_name = new QLineEdit();
    QLineEdit * edt_password = new QLineEdit();
    QLineEdit * edt_email = new QLineEdit();

//    edt_size_length->setValidator(new QIntValidator(0, 2000, this));
//    edt_size_width->setValidator(new QIntValidator(0, 2000, this));
//    edt_size_height->setValidator(new QIntValidator(0, 500, this));

    QLabel * label_name = new QLabel(QWidget::tr("User name:"));
    QLabel * label_password = new QLabel(QWidget::tr("Password:"));
    QLabel * label_email = new QLabel(QWidget::tr("Email:"));

    QPushButton * button = new QPushButton("login");

    connect(button, SIGNAL(clicked()), this, SLOT(loginButtonClicked()));

    QGroupBox *groupBox = new QGroupBox(this);

    QLabel * label_empty = new QLabel(QWidget::tr(""));
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(label_name);
    vbox->addWidget(edt_name);
    vbox->addWidget(label_password);
    vbox->addWidget(edt_password);
    vbox->addWidget(label_email);
    vbox->addWidget(edt_email);
    vbox->addWidget(label_empty);
    vbox->addWidget(button);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    groupBox->setGeometry(580,260,300,230);
}

void login::loginButtonClicked()
{
//    CHIKO ck59972267
    MainWindow * mainWindow = (MainWindow *)this->parent();
    mainWindow->loginButtonClicked();
}
