//! [0] //! [1]
//!


#include "classwizard.h"
#include <qstring.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <QVBoxLayout>
#include <qdir.h>
#include <qcombobox.h>
#include <QTextCodec>
//#include <ActiveQt/QAxObject>
#include <QDebug>
#include "xlsxdocument.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>
#include "xlsxcellreference.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"

#include "xlsxformat.h"
#include <QObject>
#include <QAbstractItemView>
#include <math.h>
QTXLSX_USE_NAMESPACE

ClassWizard::ClassWizard(QWidget *parent)
    : QWizard(parent)
{
//    addPage(new IntroPage(this));        // 添加定义的五个页面
    addPage(new InfoPage(this));
    addPage(new ResultPage(this));
//    addPage(new OutputFilesPage(this));
    addPage(new ConclusionPage(this));
//! [0]
    setOption(QWizard::NoCancelButton);

//    setWizardStyle(ModernStyle); // 如果你用的是win7或vista默认的风格是AeroStyle

    setWindowTitle(tr("Class Wizard"));

    this->setParent(parent);

//    QGridLayout *layout = new QGridLayout;        // InfoPage的布局
//    layout->addWidget(this, 0, 0);
//    parent->setLayout(layout);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);hide();

    setWizardStyle(ModernStyle); // win7vistaAeroStyle
//    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/chiko/background.jpg"));
//    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/chiko/background.jpg"));
//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/chiko/background.jpg"));


//    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/background.png"));
//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/background.png"));

//    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/background.png"));

//    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

//    setPixmap(QWizard::NPixmaps, QPixmap(":/images/background.png"));


    this->loadCities("wind_eng.dat",this->citys_eng, this->ws_eng);
    this->loadCities("wind_germany.dat",this->citys_ger, this->ws_ger);
    this->loadCities("wind_japan.dat",this->citys_jap, this->ws_jap);
    this->loadCities("wind_netherlands.dat",this->citys_hol, this->ws_hol);
    this->loadCities("wind_Australia.dat",this->citys_aus, this->ws_aus);
    this->loadCities("wind_america.dat",this->citys_usa, this->ws_usa);

    this->setGeometry(0,0,1000,800);

   //! [2]
}
//! [1] //! [2]
void writeBorderStyleCell(Document &xlsx, const QString &cell, const QString &text, Format::BorderStyle bs)
{
   Format format;
   format.setBorderStyle(bs);
   format.setHorizontalAlignment(Format::AlignHCenter);
   format.setVerticalAlignment(Format::AlignVCenter);

   xlsx.write(cell, text, format);
}
void writeSolidFillCell(Document &xlsx, const QString &cell, const QColor &color)
{
   Format format;
   format.setPatternBackgroundColor(color);
   xlsx.write(cell, QVariant(), format);
}

void writePatternFillCell(Document &xlsx, const QString &cell, Format::FillPattern pattern, const QColor &color)
{
   Format format;
   format.setPatternForegroundColor(color);
   format.setFillPattern(pattern);
   xlsx.write(cell, QVariant(), format);
}
//! [3]
// 最后完成时执行该槽，将信息写入生成文件
int judgeEGType(int vnum,bool mcon,int orientation);
int componentNumber(int rowIndex,int columnIndex,int N,int M,int V,float L);
QString findImageString(QString name);


QVector<float> & ClassWizard::getCitiesVec(QString countryName)
{

    if(countryName == "Australia")
        return this->ws_aus;
    else if(countryName == "America")
        return this->ws_usa;
    else if(countryName == "England")
        return this->ws_eng;
    else if(countryName == "Germany")
        return this->ws_ger;
    else if(countryName == "Japan")
        return this->ws_jap;
    else if(countryName == "Netherlands")
        return this->ws_hol;

}

void ClassWizard::loadCities(QString fileName, QVector<QString> & vecCities ,QVector<float> & vecWSpeed)
{
    QXlsx::Document cities_xlsx(fileName);

    int i = 2;

    QString cellString ;
    QString city;
    do
    {
            char column = 'C' ;

            cellString = column + QString::number(i);

            QVariant v = cities_xlsx.read(cellString);

            city = v.toString();

            cellString = 'D' +  QString::number(i);

            v = cities_xlsx.read(cellString);

            float wSpeed = v.toFloat();

            if(!city.isEmpty())
            {
                vecCities.push_back(city);
                vecWSpeed.push_back(wSpeed);
            }

            i++;
    }while(!city.isEmpty());
}

void ClassWizard::save(QString fileName)
{
    QXlsx::Document xlsx;

    QString cellString;
    float ratio = 1.2;
    xlsx.setColumnWidth("A1",8.38*ratio);
    xlsx.setColumnWidth("B1",13.25*ratio);
    xlsx.setColumnWidth("C1",10.38*ratio);
    xlsx.setColumnWidth("D1",13.25*ratio);
    xlsx.setColumnWidth("E1",15.38*ratio);
    xlsx.setColumnWidth("F1",10*ratio);
    xlsx.setColumnWidth("G1",12.88*ratio);
    xlsx.setColumnWidth("H1",7.5*ratio);

    QImage head(":/images/chiko/head.tif");
        xlsx.insertImage(0, 0, head);

        for(int i = 0 ; i <10; i++)
        {
            for(int j = 0 ; j < 8; j++)
            {
                char column = 'A' ;
                column += j;

                cellString = column + QString::number(i+1);
                writeSolidFillCell(xlsx, cellString, Qt::lightGray);
            }
        }

    xlsx.write("A2","Project information:");
    xlsx.write("A3","Solar Panle Dimension: " + QString::number(info.size_length) + "*" +
               QString::number(info.size_width) + "*" +
               QString::number(info.size_height) + " mm");

    QString str = "Back Plate: " ;
    str += info.back_panel==true ? "Yes" : "Not";
    xlsx.write("E3", str);

    QString angle = info.angle ;

    QXlsx::Format format6;
    format6.setPatternBackgroundColor(QColor(Qt::gray));

    str = "Console Angle: " ;
    str += angle;
    str += " DEG";
    xlsx.write("A4", str);

    str = "Side Plate: " ;
    str += info.side_panel==true ? "Yes" : "Not";
    xlsx.write("E4", str);

    str = "Solar Panel Power: " ;
    str += QString::number(info.power);
    str += " W";
    xlsx.write("A5", str);

    str = "Ballast Plate: " ;
    str += info.buttom_panel==true ? "Yes" : "Not";
    xlsx.write("E5", str);

    str = "Quantity of Panles in Vertical Direction: " ;
    str += info.vnum == 1 ? QString::number(2) : QString::number(1);
    str += " PCS";
    xlsx.write("A6", str);

    str = "Quantity of Panles in Horizontal Direction: " ;
    str +=  QString::number(info.hnum);
    str += " PCS";
    xlsx.write("A7", str);

    str = "Array Quantity: " ;
    str += QString::number(info.mnum);
    xlsx.write("A8", str);

    str = "Array Connected: " ;
    str += info.mcon==true ? "Yes" : "Not";
    xlsx.write("A9", str);

    str = "Distance Between Arrays: " ;
    str += QString::number(info.mspace);
    str += " MM";
    xlsx.write("A10", str);

    str = "Rubber pad: " ;
    str += info.plastic_cushion==true ? "Yes" : "Not";
    xlsx.write("E6", str);

    str = "Ballast Rail Support: " ;
    str += info.pushload_support==true ? "Yes" : "Not";
    xlsx.write("E7", str);

    float totalNum = info.mnum * (info.vnum == 1 ? 2 :1) * info.hnum;
    str = "Solar Panel Total Quantity: " ;
    str += QString::number(totalNum);
    str += " PCS";
    xlsx.write("E8", str);

    str = "Project Power: " ;
    str += QString::number(info.power * totalNum);
    str += " W";
    xlsx.write("E9", str);

    str = "Solar Panel Orientations: " ;
    str += info.orientation == 0 ? "South" : "Back to Back";
    xlsx.write("E10", str);

    ratio = 0.8;
    xlsx.setRowHeight(1,53*ratio);
    xlsx.setRowHeight(2,20*ratio);
    xlsx.setRowHeight(3,20*ratio);
    xlsx.setRowHeight(4,20*ratio);
    xlsx.setRowHeight(5,20*ratio);
    xlsx.setRowHeight(6,20*ratio);
    xlsx.setRowHeight(7,20*ratio);
    xlsx.setRowHeight(8,20*ratio);
    xlsx.setRowHeight(9,20*ratio);
    xlsx.setRowHeight(10,20*ratio);

    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);

    QXlsx::Format format1;
    format1.setFontBold(true);

    format1.setFontSize(22);
    xlsx.setRowFormat(12, format1);

    xlsx.mergeCells("C12:E12", format);
    xlsx.setRowHeight("13",35.1*ratio);

    QXlsx::Document formatxlsx("format.dat");

    for(int j = 0 ; j < 8; j++)
    {
            char column = 'A' ;
            if(j==6)
                column+= 7;
            else if(j==7)
                column+=6;
            else
                column += j;
            cellString = column + QString::number(20);

            QVariant v = formatxlsx.read(cellString);

            cellString = column + QString::number(13);
            writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);

            cellString = column + QString::number(12);
            writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);

            xlsx.write(cellString,v);
    }


    for(int i = 0 ; i < resultTable->rowCount(); i++)
    {

        cellString = "C" ;
        cellString += QString::number(i+13);
        cellString += ":E";
        cellString += QString::number(i+13);

        xlsx.mergeCells(cellString, format);

        cellString = "A" +QString::number(i+13)  ;
//      xlsx.setRowHeight(cellString,43.1*ratio);
        writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);

        xlsx.write(cellString,QString::number(i+1));
        xlsx.setRowFormat(i+14, format);

                QString imageName ;
        for(int j = 0 ; j < resultTable->columnCount(); j++)
        {
            char column = 'B' ;

            column += j;
            cellString = column + QString::number(i+13);
            writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);



            if(j>2)
                column += 2;
            cellString = column + QString::number(i+13);
            writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);

            QTableWidgetItem * item = resultTable->item( i, j);

            if(j==0)
            {
                QString name = item->text();
                imageName = findImageString(name);
            }

            if(j==2)
            {
                QImage image(imageName);
                QImage small = image.scaled(60,60,Qt::KeepAspectRatio);

                xlsx.insertImage(i+12,5, small);
            }else
            {
                xlsx.write(cellString,item->text());
            }
        }
//      QMessageBox::warning(0,"PATH",QString::number(i),QMessageBox::Yes);

//      cellString = "A" +QString::number(i+13);
        xlsx.setRowHeight(i+13,43.1*ratio);
    }


    QImage tail(":/images/chiko/tail.tif");
        xlsx.insertImage(13 + resultTable->rowCount(), 0, tail);
//    QString outputDir = field("outputDir").toString();
//    xlsx.saveAs("result.xlsx");

//    fileName += ".xlsx";
    xlsx.saveAs(fileName);

//  xlsx_result.write("A1","test");
//    xlsx_result.saveAs("result_test.xlsx");
}


void ClassWizard::accept()
{
    QDialog::accept();
    return;
}


void InfoPage::initializePage()
{
    this->refreshPreview();

    this->refreshCities("Australia");
}

InfoPage::InfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    parent_wizard = (ClassWizard *) parent;
    // 标题和子标题
    setStyleSheet("background-image: url(./images/chiko/background.jpg)");

    QGridLayout *layout = new QGridLayout;        // InfoPage的布局

    edt_size_length = new QLineEdit();
    edt_size_width = new QLineEdit();
    edt_size_height = new QLineEdit();

    edt_size_length->setText("1640");
    edt_size_width->setText("992");
    edt_size_height->setText("40");

    edt_size_length->setValidator(new QIntValidator(0, 2000, this));
    edt_size_width->setValidator(new QIntValidator(0, 2000, this));
    edt_size_height->setValidator(new QIntValidator(0, 500, this));

    QLabel * label_length = new QLabel(QWidget::tr("Length mm"));
    label_length->setBuddy((edt_size_length));
    label_length->adjustSize();
    QLabel * label_width = new QLabel(QWidget::tr("Width mm"));
    label_width->setBuddy((edt_size_width));
    QLabel * label_height = new QLabel(QWidget::tr("Thickness mm"));
    label_height->setBuddy(edt_size_height);

    QGridLayout *vbox_length = new QGridLayout;
    vbox_length->addWidget(label_length, 1,0,1,1);
    vbox_length->addWidget(edt_size_length, 1,1,1,1);

    QWidget * widget_length = new QWidget();
    widget_length->setParent(this);
    widget_length->setGeometry(QRect(0, 0, 650, 100));
    widget_length->setLayout(vbox_length);

    QGridLayout *vbox_width = new QGridLayout;
    vbox_width->addWidget(label_width, 1,0,1,1);
    vbox_width->addWidget(edt_size_width, 1,1,1,1);

    QWidget * widget_width = new QWidget();
    widget_width->setParent(this);
    widget_width->setGeometry(QRect(0, 0, 650, 100));
    widget_width->setLayout(vbox_width);

    QGridLayout *vbox_height = new QGridLayout;
    vbox_height->addWidget(label_height, 1,0,1,1);
    vbox_height->addWidget(edt_size_height, 1,1,1,1);

    QWidget * widget_height = new QWidget();
    widget_height->setParent(this);
    widget_height->setGeometry(QRect(0, 0, 650, 100));
    widget_height->setLayout(vbox_height);


    QLabel * label_power = new QLabel(tr("Power W"));
    edt_power = new QLineEdit();
    edt_power->setValidator(new QIntValidator(0, 1000, this));
    edt_power->setText("250");

    QLabel * label_weight = new QLabel(tr("Weight KG"));
    edt_weight = new QLineEdit();
    edt_weight->setValidator(new QIntValidator(0, 1000, this));
    edt_weight->setText("20");

    QGridLayout *vbox_weight = new QGridLayout;
    vbox_weight->addWidget(label_power, 1,0,1,1);
    vbox_weight->addWidget(edt_power, 1,1,1,1);
    vbox_weight->addWidget(label_weight, 1,2,1,1);
    vbox_weight->addWidget(edt_weight, 1,3,1,1);

    QWidget * widget_weight = new QWidget();
    widget_weight->setParent(this);
    widget_weight->setGeometry(QRect(0, 0, 650, 100));
    widget_weight->setLayout(vbox_weight);

    QGroupBox *groupBox = new QGroupBox(tr("Panel info"));
    QVBoxLayout *vbox = new QVBoxLayout;
//    vbox->addWidget(label_length);
//    vbox->addWidget(edt_size_length);
//    vbox->addWidget(label_width);
//    vbox->addWidget(edt_size_width);
//    vbox->addWidget(label_height);
//    vbox->addWidget(edt_size_height);
    vbox->addWidget(widget_length);
    vbox->addWidget(widget_width);
    vbox->addWidget(widget_height);

    vbox->addWidget(widget_weight);
//    vbox->addWidget(edt_power);
//    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    QLabel * label_vnum = new QLabel(tr("Vertical QTY"));
    cbo_vnum = new QComboBox();
    cbo_vnum->addItem(QWidget::tr("1 panel landscape"));
    cbo_vnum->addItem(QWidget::tr("2 panels landscape"));
    cbo_vnum->addItem(QWidget::tr("1 panel portrait"));

    QLabel * label_hnum = new QLabel(tr("Horizontal QTY"));
    edt_hnum = new QLineEdit();
    edt_hnum->setValidator(new QIntValidator(0, 1000, this));
    edt_hnum->setText("15");
    QLabel * label_mnum = new QLabel(tr("Array QTY"));
    edt_mnum = new QLineEdit();
    edt_mnum->setValidator(new QIntValidator(0, 1000, this));
    edt_mnum->setText("10");

    //connect(lineEdit,SIGNAL(textChanged(QString &)),this,SLOT(enableFindButton(QString &)));
    connect(edt_hnum,SIGNAL(textChanged(const QString &)),this,SLOT(edt_hnumTextChanged(const QString &)));
    connect(edt_mnum,SIGNAL(textChanged(const QString &)),this,SLOT(edt_mnumTextChanged(const QString &)));

    QLabel * label_mspace = new QLabel(tr("Spacing between every array mm"));
    edt_mspace = new QLineEdit();
    edt_mspace->setValidator(new QIntValidator(0, 1000, this));
    edt_mspace->setText("700");

    QGroupBox *groupBox_number = new QGroupBox(tr("Array info"));

    QVBoxLayout *vbox_number = new QVBoxLayout;
    vbox_number->addWidget(label_vnum);
    vbox_number->addWidget(cbo_vnum);
    vbox_number->addWidget(label_hnum);
    vbox_number->addWidget(edt_hnum);
    vbox_number->addWidget(label_mnum);
    vbox_number->addWidget(edt_mnum);
    vbox_number->addWidget(label_mspace);
    vbox_number->addWidget(edt_mspace);

    vbox_number->addStretch(1);
    groupBox_number->setLayout(vbox_number);

    cbo_angle = new QComboBox();

    cbo_angle->addItem(QWidget::tr("10"));
    cbo_angle->addItem(QWidget::tr("15"));
    cbo_angle->addItem(QWidget::tr("20"));
    cbo_angle->addItem(QWidget::tr("30"));
    cbo_angle->addItem(QWidget::tr("CUSTOM"));

    QLabel * label_angle = new QLabel(tr("Console angle"));
    label_angle->setBuddy(cbo_angle);


    QLabel * label_orientation = new QLabel(tr("Console orientation"));
    cbo_orientation = new QComboBox();
    cbo_orientation->addItem(QWidget::tr("south"));
    cbo_orientation->addItem(QWidget::tr("east/west"));

    QLabel * label_country = new QLabel(tr("Country"));
    cbo_country = new QComboBox();

    cbo_country->setFixedWidth(100);
    cbo_country->addItem(QWidget::tr("Australia"));
    cbo_country->addItem(QWidget::tr("America"));
    cbo_country->addItem(QWidget::tr("England"));
    cbo_country->addItem(QWidget::tr("Germany"));
    cbo_country->addItem(QWidget::tr("Japan"));
    cbo_country->addItem(QWidget::tr("Netherlands"));

    connect(cbo_country, SIGNAL(currentIndexChanged(int)), this, SLOT(onCountryChanged(int)));

    QLabel * label_city = new QLabel(tr("City"));
    cbo_city = new QComboBox();
    cbo_city->setFixedWidth(90);

    QGridLayout *vbox_city = new QGridLayout;
    vbox_city->addWidget(label_country, 1,0,1,1);
    vbox_city->addWidget(cbo_country, 1,1,1,1);
    vbox_city->addWidget(label_city, 1,2,1,1);
    vbox_city->addWidget(cbo_city, 1,3,1,1);

    QLabel * label_building_L = new QLabel(tr("Building "));
//    edt_building_L = new QLineEdit();
//    edt_building_L->setValidator(new QIntValidator(0, 1000, this));
//    edt_building_L->setText("10");
//    QLabel * label_building_W = new QLabel(tr("W"));
//    edt_building_W = new QLineEdit();
//    edt_building_W->setValidator(new QIntValidator(0, 1000, this));
//    edt_building_W->setText("10");
//    QLabel * label_building_H = new QLabel(tr("H"));

    cbo_building_H = new QComboBox();

    cbo_building_H->addItem(QWidget::tr("5"));
    cbo_building_H->addItem(QWidget::tr("10"));
    cbo_building_H->addItem(QWidget::tr("15"));
    cbo_building_H->addItem(QWidget::tr("20"));
    cbo_building_H->addItem(QWidget::tr("25"));
    cbo_building_H->addItem(QWidget::tr("30"));
    cbo_building_H->addItem(QWidget::tr("35"));
    cbo_building_H->addItem(QWidget::tr("40"));

//  cbo_building_H = new QLineEdit();
//  edt_building_H->setValidator(new QIntValidator(0, 1000, this));
//  edt_building_H->setText("10");
    QLabel * label_building_MM = new QLabel(tr("mm"));

    QGridLayout *vbox_building = new QGridLayout;
    vbox_building->addWidget(label_building_L, 1,0,1,1);
    vbox_building->addWidget(cbo_building_H, 1,1,1,1);
    vbox_building->addWidget(label_building_MM, 1,2,1,1);

    QWidget * widget_city = new QWidget();
    widget_city->setParent(this);
    widget_city->setGeometry(QRect(0, 0, 650, 50));
    widget_city->setLayout(vbox_city);

    QWidget * widget_building = new QWidget();
    widget_building->setGeometry(QRect(0, 0, 650, 120));
    widget_building->setParent(this);
    widget_building->setLayout(vbox_building);

    QGroupBox *groupBox_value = new QGroupBox(tr("Ballast I Console info"));

    QVBoxLayout *vbox_value = new QVBoxLayout;
    vbox_value->addWidget(widget_city);
    vbox_value->addWidget(widget_building);
    vbox_value->addWidget(label_orientation);
    vbox_value->addWidget(cbo_orientation);
    vbox_value->addWidget(label_angle);
    vbox_value->addWidget(cbo_angle);

    vbox_value->addStretch(1);
    groupBox_value->setLayout(vbox_value);

    chk_mcon = new QCheckBox(tr("All array connected?"));
    chk_back_panel = new QCheckBox(tr("back wind plate?"));
    chk_side_panel = new QCheckBox(tr("side wind plate?"));
    chk_pushload_support = new QCheckBox(tr("Ballast support rails?"));
    chk_buttom_panel = new QCheckBox(tr("Back plate?"));
    chk_base_plate = new QCheckBox(tr("Base ballast plate?"));
    chk_plastic_cushion = new QCheckBox(tr("Rubber pad?"));

    chk_mcon->toggle();
    chk_pushload_support->toggle();

    chk_back_panel->setChecked(true);
    chk_side_panel->setChecked(true);

    connect(chk_mcon, SIGNAL(clicked()), this, SLOT(refreshPreview()));
    connect(cbo_orientation,SIGNAL(currentIndexChanged(int)), this, SLOT(refreshPreview()));
    connect(cbo_vnum, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshPreview()));

    QGroupBox *groupBox_checkbox = new QGroupBox(tr("Ballast I console optional function"));

    QVBoxLayout *vbox_checkbox = new QVBoxLayout;
    vbox_checkbox->addWidget(chk_mcon);

    vbox_checkbox->addWidget(chk_pushload_support);
    vbox_checkbox->addWidget(chk_buttom_panel);
    vbox_checkbox->addWidget(chk_base_plate);
    vbox_checkbox->addWidget(chk_plastic_cushion);

    vbox_checkbox->addStretch(1);
    groupBox_checkbox->setLayout(vbox_checkbox);

    registerField("edt_size_length", edt_size_length);
    registerField("edt_size_width", edt_size_width);
    registerField("edt_size_height", edt_size_height);

    registerField("cbo_country", cbo_country);
    registerField("cbo_city", cbo_city);

    registerField("cbo_vnum", cbo_vnum);
    registerField("edt_hnum", edt_hnum);
    registerField("edt_mnum", edt_mnum);

    registerField("cbo_angle", cbo_angle);
    registerField("edt_power", edt_power);
    registerField("edt_weight", edt_weight);

    registerField("cbo_building_H", cbo_building_H);

    registerField("edt_mspace", edt_mspace);
    registerField("cbo_orientation", cbo_orientation);

    registerField("chk_mcon", chk_mcon);
    registerField("chk_back_panel", chk_back_panel);
    registerField("chk_side_panel", chk_side_panel);
    registerField("chk_buttom_panel", chk_buttom_panel);
    registerField("chk_base_plate", chk_base_plate);
    registerField("chk_plastic_cushion", chk_plastic_cushion);
    registerField("chk_pushload_support", chk_pushload_support);

    layout->addWidget(groupBox_value, 1,0,1,1);
    layout->addWidget(groupBox, 1,1,1,1);

    layout->addWidget(groupBox_number, 2,0,1,1);
    layout->addWidget(groupBox_checkbox, 2,1,1,1);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);

    QPixmap pix(":/images/chiko/background.jpg");
    QPixmap resPix = pix.scaled(1000,600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QLabel *lblTest = new QLabel;

    lblTest->setGeometry(0,0,1000,600);
    lblTest->setPixmap(resPix);

    lblTest->setAlignment(Qt::AlignCenter);
    lblTest->setParent(this);

    previewLabel = new QLabel;
    previewLabel->setGeometry(690,60,280,180);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setParent(this);

    layoutWidget = Q_NULLPTR;
    QWidget * widget = new QWidget();
    widget->setParent(this);
    widget->setGeometry(QRect(0, 50, 650, 500));

    widget->setLayout(layout);
    this->resize(766,341);
}

void InfoPage::refreshCities(QString country)
{
    QVector<QString> vec ;
    if(country == "Australia")
        vec = this->parent_wizard->citys_aus;
    else if(country == "England")
        vec = this->parent_wizard->citys_eng;
    else if(country == "Germany")
        vec = this->parent_wizard->citys_ger;
    else if(country == "Japan")
        vec = this->parent_wizard->citys_jap;
    else if(country == "Netherlands")
        vec = this->parent_wizard->citys_hol;
    else if(country == "America")
        vec = this->parent_wizard->citys_usa;

    cbo_city->clear();

    for (int i = 0; i < vec.size(); ++i)
    {
        QString city = vec.at(i);
        cbo_city->addItem(city);
    }

}
void InfoPage::edt_hnumTextChanged(const QString &)
{
    int origin_hnum = parent_wizard->info.hnum;
    parent_wizard->info.hnum = field("edt_hnum").toInt();//N
    if( (origin_hnum - 11.99) * (parent_wizard->info.hnum - 11.99) <= 0 )
        this->refreshLayout();
}

void InfoPage::edt_mnumTextChanged(const QString &)
{
    int origin_mnum = parent_wizard->info.mnum;
    parent_wizard->info.mnum = field("edt_mnum").toInt();//M
    if( (origin_mnum - 6.99) * (parent_wizard->info.mnum - 6.99) <= 0 )
        this->refreshLayout();
}

void InfoPage::onCountryChanged(int index)
{
    QString country = cbo_country->currentText();

    this->refreshCities(country);
}

void InfoPage::refreshLayout()
{
    if(layoutWidget!= Q_NULLPTR)
    {
        delete layoutWidget;
        layoutWidget = Q_NULLPTR;
    }

    layoutWidget = new QWidget(this);

    int layoutType = this->layoutImageIndex();

    QString layoutImageName = ":/images/chiko/layout";
    layoutImageName += QString::number(layoutType);
    layoutImageName += ".png";

    QPixmap pix(layoutImageName);
    QPixmap resPix = pix.scaled(31,20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap linepix(":/images/chiko/line.png");
    QPixmap lineResPix = linepix.scaled(15,1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap vertical_linepix(":/images/chiko/vertical_line.png");

    //QGridLayout *vbox_layout = new QGridLayout;


    for(int i = 0 ; i< 7 ; i++)
    {
        for(int j = 0 ; j< 13 ; j+=1)
        {
            QLabel * label = new QLabel(layoutWidget);
            if(parent_wizard->info.hnum >= 12 && j == 11){

                if(parent_wizard->info.mnum >= 7 && i==5)
                    continue;

                label->setPixmap(vertical_linepix);
                label->setGeometry(i* (31 + 15)+30,j * 20,1,21);


                continue;
            }else if(parent_wizard->info.mnum >= 7 && i==5){

                label->setPixmap(lineResPix);
                label->setGeometry(i* (31 + 15)+ 0.5,j * 20 + 19,15,1);
                label = new QLabel(layoutWidget);
                label->setPixmap(lineResPix);
                label->setGeometry(i* (31 + 15) + 15.2,j * 20 + 19,15,1);


            }else{
                label->setPixmap(resPix);
                label->setGeometry(i* (31 + 15),j * 20,31,20);
            }

//            label->setGeometry(i* (31 + 15),j * 20,31,20);


//            vbox_layout->addWidget(label, i,j,1,1);

            if(i==6)
                continue;
            if(this->parent_wizard->info.mcon)
            {
                label = new QLabel(layoutWidget);
                label->setPixmap(lineResPix);
                label->setGeometry(i* (31 + 15)+31,j * 20 + 19,15,1);
            }

//            vbox_layout->addWidget(label, i,j+1,1,1,Qt::AlignBottom);
        }
    }

//    layoutWidget->setLayout(vbox_layout);
    layoutWidget->setGeometry(QRect(665, 260, 350, 300));
    layoutWidget->show();
}

void InfoPage::refreshPreview()
{
    if(this->validatePage())
    {
        int egType = parent_wizard->info.egType;
        QString imageName = ":/images/chiko/eg";
        imageName += QString::number(egType);
        imageName += ".png";
        QPixmap pix(imageName);
        QPixmap resPix = pix.scaled(280,180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        previewLabel->setPixmap(resPix);

        this->refreshLayout();
    }
}

int InfoPage::layoutImageIndex()
{

    Information info = parent_wizard->info;
    if(info.vnum == 0 && info.orientation == 0 )
        return 1;
    else if(info.vnum == 0 && info.orientation == 1 )
        return 3;
    else if(info.vnum == 1 && info.orientation == 0 )
        return 2;
    else if(info.vnum == 1 && info.orientation == 1 )
        return 4;
    else if(info.vnum == 2 && info.orientation == 0 )
        return 3;
    else if(info.vnum == 2 && info.orientation == 1 )
        return 4;
}

bool InfoPage::validatePage()
{
    parent_wizard->info.size_length = field("edt_size_length").toFloat();//L
    parent_wizard->info.size_width = field("edt_size_width").toFloat();
    parent_wizard->info.size_height = field("edt_size_height").toFloat();

    parent_wizard->info.country =this->cbo_country->currentText();
    parent_wizard->info.city = this->cbo_city->currentText();

    QVector<float> wsVec = this->parent_wizard->getCitiesVec( parent_wizard->info.country);
    parent_wizard->info.windSpeed = wsVec[this->cbo_city->currentIndex()];

    parent_wizard->info.buildingHeight = this->cbo_building_H->currentText().toInt();
    parent_wizard->info.vnum = field("cbo_vnum").toInt();//V
    parent_wizard->info.hnum = field("edt_hnum").toInt();//N
    parent_wizard->info.mnum = field("edt_mnum").toInt();//M



    parent_wizard->info.angle = this->cbo_angle->currentText();
//    QMessageBox::warning(0,"PATH", this->cbo_angle->currentText(),QMessageBox::Yes);
//    QMessageBox::warning(0,"PATH",parent_wizard->info.angle,QMessageBox::Yes);
    parent_wizard->info.power = field("edt_power").toFloat();
    parent_wizard->info.weight = field("edt_weight").toFloat();
    parent_wizard->info.mspace = field("edt_mspace").toFloat();
    parent_wizard->info.orientation = field("cbo_orientation").toInt();

    parent_wizard->info.mcon = field("chk_mcon").toBool();
    parent_wizard->info.back_panel = field("chk_back_panel").toBool();
    parent_wizard->info.side_panel = field("chk_side_panel").toBool();
    parent_wizard->info.buttom_panel = field("chk_buttom_panel").toBool();
    parent_wizard->info.base_plate = field("chk_base_plate").toBool();
    parent_wizard->info.plastic_cushion = field("chk_plastic_cushion").toBool();
    parent_wizard->info.pushload_support = field("chk_pushload_support").toBool();

    parent_wizard->info.egType = judgeEGType(parent_wizard->info.vnum,parent_wizard->info.mcon,parent_wizard->info.orientation);

    if(edt_hnum->text().isEmpty() || edt_mnum->text().isEmpty() || edt_mspace->text().isEmpty()
            || edt_power->text().isEmpty()||edt_size_height->text().isEmpty()||edt_size_length->text().isEmpty()
            ||edt_size_width->text().isEmpty()){
        return false;
    }
    return true;
}

bool ResultPage::validatePage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    parent_wizard->fileName, tr("xlsx(*.xlsx)"));
    if (fileName.isEmpty())
        return false ;
    parent_wizard->save(fileName);

    return true;
}

bool ResultPage::verifyConstraint(int rowIndex)
{
    switch(rowIndex)
    {
    case 0:
        return parent_wizard->info.vnum == 0  && parent_wizard->info.orientation == 0;
    case 1:
        return parent_wizard->info.vnum == 0  && parent_wizard->info.orientation == 1;
    case 2:
        return parent_wizard->info.vnum == 1  && parent_wizard->info.orientation == 0;
    case 3:
        return parent_wizard->info.vnum == 1  &&parent_wizard-> info.orientation == 1;
    case 4:
        return  parent_wizard->info.side_panel;
    case 5:
        return  parent_wizard->info.back_panel  && parent_wizard->info.buttom_panel;
    case 6:
        return  parent_wizard->info.side_panel;
    case 7:
        return  parent_wizard->info.back_panel;
    case 8:
    case 9:
        return true;
    case 10:
        return parent_wizard->info.pushload_support  ;
    case 11:
        return parent_wizard->info.pushload_support;
    case 12:
        return parent_wizard->info.mcon && parent_wizard->info.pushload_support && parent_wizard->info.base_plate;
    case 13:
        return parent_wizard->info.plastic_cushion;
    case 14:
        return parent_wizard->info.mcon ;
    }
}

QString findImageString(QString name)
{
    QString iconStr;
    if(name.indexOf("CK-BT21-B")==0){
        iconStr=(":/images/chiko/CK-BT21-B.jpg");
    }else if(name.indexOf("CK-BT21-S")==0){
         iconStr=(":/images/chiko/CK-BT21-S.jpg");
    }else if(name.indexOf("CK-BT21-")==0){
         iconStr=(":/images/chiko/CK-BT21-.jpg");
    }else if(name.indexOf("CK-BT23-")==0){
         iconStr=(":/images/chiko/CK-BT23-.gif");
    }else if(name.indexOf("CK-BT31-B")==0){
         iconStr=(":/images/chiko/CK-BT31-B.jpg");
    }else if(name.indexOf("CK-BT31-S")==0){
         iconStr=(":/images/chiko/CK-BT31-S.jpg");
    }else if(name.indexOf("CK-BT31-")==0){
         iconStr=(":/images/chiko/CK-BT31-.tif");
    }else if(name.indexOf("CK-BT33-")==0){
         iconStr=(":/images/chiko/CK-BT33-.gif");
    }else if(name.indexOf("CK-BT-BSK")==0){
         iconStr=(":/images/chiko/CK-BT-BSK.jpg");
    }else if(name.indexOf("CK-BT-R3500")==0){
         iconStr=(":/images/chiko/CK-BT-R3500.jpg");
    }else if(name.indexOf("CK-BT-RSK")==0){
         iconStr=(":/images/chiko/CK-BT-RSK.jpg");
    }else if(name.indexOf("CK-FPA-BT1000")==0){
         iconStr=(":/images/chiko/CK-FPA-BT1000.jpg");
    }else if(name.indexOf("CK-FZE")==0){
         iconStr=(":/images/chiko/CK-FTE-F.jpg");
    }else if(name.indexOf("CK-FZM")==0){
         iconStr=(":/images/chiko/CK-FTM-F.jpg");
    }else if(name.indexOf("CK-BT-BSP")==0){
        iconStr=(":/images/chiko/CK-BT-BSP.jpg");
    }else if(name.indexOf("CK-CB-")==0){
        iconStr=(":/images/chiko/CK-CB-.jpg");
    }


    return iconStr;
}

void ResultPage::caculateBallastWight()
{
    int height = this->parent_wizard->info.buildingHeight;
    float ratio = 0;
    switch(height)
    {
    case 5:
        ratio = 0.79;
        break;
    case 10:
        ratio = 0.93;
        break;
    case 15:
        ratio = 1;
        break;
    case 20	:
        ratio = 1.03;
        break;
    case 25	:
        ratio = 1.05;
        break;
    case 30	:
        ratio = 1.07;
        break;
    case 35	:
        ratio = 1.085;
        break;
    case 40 :
        ratio = 1.1;
        break;
    defaut:
        ratio = 0;
        break;
    }

    //design wind speed
    float dws = this->parent_wizard->info.windSpeed * ratio;

//    QMessageBox::warning(0,"PATH",QString::number(this->parent_wizard->info.windSpeed),QMessageBox::Yes);
//    QMessageBox::warning(0,"PATH",QString::number(ratio),QMessageBox::Yes);

//    (0.5*设计风速*设计风速*1.274*COS角度*L/1000*W/1000-G*9.8-14.5)/9.8
//    ((0.5*设计风速*设计风速*1.274*COS角度*L/1000*W/1000-G*9.8-14.5)-(0.5*设计风速*设计风速*1.274*0.27*1.655*COS60度))/9.8
    float ballastWeight;
    float angle = this->parent_wizard->info.angle.toFloat();
    float L = this->parent_wizard->info.size_length;
    float W = this->parent_wizard->info.size_width;
    float G = this->parent_wizard->info.weight;

//    QMessageBox::warning(0,"PATH",QString::number(angle),QMessageBox::Yes);
//    QMessageBox::warning(0,"PATH",QString::number(L),QMessageBox::Yes);
//    QMessageBox::warning(0,"PATH",QString::number(W),QMessageBox::Yes);
//    QMessageBox::warning(0,"PATH",QString::number(G),QMessageBox::Yes);

    if(!this->parent_wizard->info.buttom_panel)
    {
//        QMessageBox::warning(0,"PATH",QString::number(1),QMessageBox::Yes);

        ballastWeight = (0.5 * dws * dws * 1.274 *cos(angle *  3.1415926 / 180)
                *L / 1000
                *W / 1000
                -G*9.8 ) / 9.8;
    }else
    {
//        QMessageBox::warning(0,"PATH",QString::number(2),QMessageBox::Yes);

        ballastWeight = ((0.5 * dws * dws * 1.274 *cos(angle *  3.1415926 / 180)*L / 1000*W / 1000-G*9.8 )
                -(0.5 * dws * dws * 1.274 * 0.34 * 1.655 *cos(60 *  3.1415926 / 180 )))/9.8 ;
    }
//     QMessageBox::warning(0,"PATH",QString::number(this->parent_wizard->info.hnum),QMessageBox::Yes);
//     QMessageBox::warning(0,"PATH",QString::number(this->parent_wizard->info.vnum),QMessageBox::Yes);

     int vnum = this->parent_wizard->info.vnum == 1 ? 2:1;

     ballastWeight *= this->parent_wizard->info.hnum * vnum;

    this->parent_wizard->info.ballastWeight = ballastWeight;
}

void ResultPage::initializePage()
{
    setTitle(tr("Result"));
    setSubTitle(tr("According to "
                   "your information,the components list as following for your reference."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

    this->caculateBallastWight();

    int colum = parent_wizard->info.egType + 4;
    int row = 5;

    int M = parent_wizard->info.mnum;
    int N = parent_wizard->info.hnum;
    int V = parent_wizard->info.vnum;

    if(parent_wizard->info.vnum == 1){
        V = 2;
    }else{
        V = 1;
    }

    float L = parent_wizard->info.size_length;
    int rowNum = 0;

    QXlsx::Document xlsx("rule.dat");

    tableWidget->clear();
    tableWidget->setRowCount(12);     //设置行数为10
    tableWidget->setColumnCount(5);   //设置列数为5
    QStringList header;
    header<<"Item no"<<"Description"<<"Image"<<"Qty"<<"Unit";
    tableWidget->setHorizontalHeaderLabels(header);

    for(int i = 0; i<15; i++)
    {
        row = 5 + i;
        QVariant v = xlsx.read(row,colum);

        QString angle =  parent_wizard->info.angle ;

        float result = 0;
        if(v.toString() != "N" && verifyConstraint(i))
        {
            result = componentNumber(i,colum,N,M,V,L);
            QString name = xlsx.read(row,1).toString();
            QString desc = xlsx.read(row,2).toString();
            if(row >= 5 && row <=12)
            {
                name = name.left(name.length() -2);
                desc = desc.replace(QRegExp("\\s\\d+\\s"), " " + angle + " ");
                name += angle;
            }else if(row>=13 && row <=14){
                name = name.left(name.length() - 4);
                name += QString::number(parent_wizard->info.size_height);
                name += "NB";
                desc = desc.replace(QRegExp("\\s\\d+\\s"), " " + QString::number(parent_wizard->info.size_height) + " ");
            }else if(row == 19)
            {
                name = name.left(name.length() - 3);
                name += QString::number(parent_wizard->info.mspace);
                float length = parent_wizard->info.mspace + 120 * 2;
                desc = desc.replace(QRegExp("\\d+"),  QString::number(length));
            }
            QString iconStr = findImageString(name);
            QPixmap pix(iconStr);
            QPixmap resPix = pix.scaled(27,27, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            QLabel *lblTest = new QLabel;
            lblTest->setPixmap(resPix);
            lblTest->setAlignment(Qt::AlignCenter);

            tableWidget->setItem(rowNum,0,new QTableWidgetItem(name));
            tableWidget->setItem(rowNum,1,new QTableWidgetItem(desc));
//          tableWidget->setItem(rowNum,2,new QTableWidgetItem(icon, ""));
            tableWidget->setCellWidget(rowNum,2,lblTest);
            QTableWidgetItem * item = new QTableWidgetItem(QString::number(result));
            item->setTextAlignment(0x0084);
            tableWidget->setItem(rowNum,3,item);

            item = new QTableWidgetItem("sets");
            item->setTextAlignment(0x0084);
            tableWidget->setItem(rowNum,4,item);

            rowNum ++;
        }
    }

    QString iconStr = findImageString("CK-CB-");
    QPixmap pix(iconStr);
    QPixmap resPix = pix.scaled(27,27, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QLabel *lblTest = new QLabel;
    lblTest->setPixmap(resPix);
    lblTest->setAlignment(Qt::AlignCenter);

    tableWidget->setItem(rowNum,0,new QTableWidgetItem(""));
    tableWidget->setItem(rowNum,1,new QTableWidgetItem("Ballast Weight for one array"));
    tableWidget->setItem(rowNum,2,new QTableWidgetItem(""));
    tableWidget->setCellWidget(rowNum,2,lblTest);
    QTableWidgetItem * item = new QTableWidgetItem(QString::number(this->parent_wizard->info.ballastWeight));
    item->setTextAlignment(0x0084);
    tableWidget->setItem(rowNum,3,item);
    item = new QTableWidgetItem("KG");
    item->setTextAlignment(0x0084);
    tableWidget->setItem(rowNum,4,item);

    rowNum ++;

    tableWidget->setEditTriggers(QTableView::NoEditTriggers);
    tableWidget->setRowCount(rowNum);
    tableWidget->resizeColumnToContents(0);
    tableWidget->resizeColumnToContents(1);
    tableWidget->setColumnWidth(1,527);
}

int judgeEGType(int vnum,bool mcon,int orientation)
{
    if( vnum==0 && mcon && orientation==0){
        return 1;
    }else if(vnum==0 && !mcon && orientation==0){
        return 2;
    }else if( vnum==0 && mcon && orientation==1){
        return 3;
    }else if( vnum==0 && !mcon && orientation==1){
        return 4;
    }else if( vnum==1 && mcon && orientation==0){
        return 5;
    }else if( vnum==1 && !mcon && orientation==0){
        return 6;
    }else if( vnum==1 && mcon && orientation==1){
        return 7;
    }else if( vnum==1 && !mcon && orientation==1){
        return 8;
    }else if( vnum==2 && mcon && orientation==0){
        return 9;
    }else if( vnum==2 && !mcon && orientation==0){
        return 10;
    }else if( vnum==2 && mcon && orientation==1){
        return 11;
    }else if( vnum==2 && !mcon && orientation==1){
        return 12;
    }
}

int componentNumber(int rowIndex,int columnIndex,int N,int M,int V,float L)
{
    int result;
    switch(rowIndex)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    {
        result = (N + 1)*M;
        break;
    }
    case 4:
    {
        if(columnIndex != 7 && columnIndex != 8){
            result = M * 2;
        }else{
            result = M * 4;
        }

        break;
    }
    case 5:
    case 7:{
        result = N*M;
        break;
    }
    case 6:{
        if(columnIndex == 11 || columnIndex == 12){
            result = M * 4;
        }else{
            result = M * 2;
        }
        break;
    }
    case 8:
    {
         result = (N-1)*2*V *M;
//        if(columnIndex>=5 && columnIndex <=8){
//            result = (N-1)*2*V *M;
//        }else if(columnIndex >=9 && columnIndex <=12){
//            result = V;
//        }else{
//            result = V*M;
//        }
        break;
    }
    case 9:
    {
        result = 4*M*V;
        break;
    }
    case 10:{
        result = qRound((L+30)*N/3500) * 2*M;
        break;
    }
    case 11:{
        result = (qRound((L+30)*N/3500) - 1) * 2*M;
        if(result < 0)
            result = 0;
        break;
    }
    case 12:{
        result = (N+1)*(M-1);
        break;
    }
    case 13:{
        result = (N+1)*M;
        break;
    }
    case 14:{
        result = (N+1)*(M-1);
        break;
    }
    default:{
        result = 0;
        break;
    }
    }
    return result;
}
ResultPage::ResultPage(QWidget *parent)
    : QWizardPage(parent)

{
    parent_wizard = (ClassWizard *) parent;
    tableWidget = new QTableWidget; // 构造了一个QTableWidget的对象，并且设置为10行，5列

    this->setAutoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(73,144,186));
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
    this->setPalette(palette);

    parent_wizard->resultTable = tableWidget;
    //    也可用下面的方法构造QTableWidget对象
    //    QTableWidget *tableWidget = new QTableWidget;

    tableWidget->setWindowTitle("QTableWidget & Item");
    tableWidget->resize(350, 200);  //设置表格

    QGridLayout *layout = new QGridLayout;   // 布局
    layout->setColumnMinimumWidth(0, 20);    // 设置第一列的最小宽20像素
    layout->addWidget(tableWidget, 0, 0, 1, 1);
//! [15]
    setLayout(layout);
    setButtonText(QWizard::NextButton, "Save >");
}
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
    : QWizardPage(parent)
{
    parent_wizard = (ClassWizard *) parent;
    // 标题 子标题 logo
    setTitle(tr("Output Files"));
    setSubTitle(tr("Specify where you want the wizard to put the generated "
                   "skeleton code."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo3.png"));

    // 创建页中的窗体部件
    outputDirLabel = new QLabel(tr("&Output directory:"));
    outputDirLineEdit = new QLineEdit;
    outputDirLabel->setBuddy(outputDirLineEdit);

    headerLabel = new QLabel(tr("&Header file name:"));
    headerLineEdit = new QLineEdit;
    headerLabel->setBuddy(headerLineEdit);

    implementationLabel = new QLabel(tr("&Implementation file name:"));
    implementationLineEdit = new QLineEdit;
    implementationLabel->setBuddy(implementationLineEdit);

    // 注册域
    registerField("outputDir*", outputDirLineEdit);
    registerField("header*", headerLineEdit);
    registerField("implementation*", implementationLineEdit);

    // 布局
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(outputDirLabel, 0, 0);
    layout->addWidget(outputDirLineEdit, 0, 1);
    layout->addWidget(headerLabel, 1, 0);
    layout->addWidget(headerLineEdit, 1, 1);
    layout->addWidget(implementationLabel, 2, 0);
    layout->addWidget(implementationLineEdit, 2, 1);
    setLayout(layout);
}

//! [17]
void OutputFilesPage::initializePage()
{
    QString className = field("className").toString();
    headerLineEdit->setText(className.toLower() + ".h");  // 头文件
    implementationLineEdit->setText(className.toLower() + ".cpp"); // 实现文件
    // 输出路径初始化为系统temp的路径
    // convertSeparators（）是解决不同系统中分隔符的问题
//    outputDirLineEdit->setText(QDir::convertSeparators(QDir::tempPath()));
}
//! [17]

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Finish"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark2.png"));

    label = new QLabel;  // 创建label
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("The result list has been saved to your computer.") // label的内容
                   .arg(finishText));
}
