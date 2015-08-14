//! [0] //! [1]
//!
 using namespace std;

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
QTXLSX_USE_NAMESPACE

ClassWizard::ClassWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage(this));        // 添加定义的五个页面
    addPage(new InfoPage(this));
    addPage(new ResultPage(this));
//    addPage(new OutputFilesPage(this));
    addPage(new ConclusionPage(this));
//! [0]
    setOption(QWizard::NoCancelButton);

    setWizardStyle(ModernStyle); // 如果你用的是win7或vista默认的风格是AeroStyle
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Class Wizard"));

    this->setParent(parent);

    QGridLayout *layout = new QGridLayout;        // InfoPage的布局
    layout->addWidget(this, 0, 0);
    parent->setLayout(layout);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);hide();

    this->setGeometry(0,0,1000,800);
//    this->resize(766,341);
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

    QString str = "Need Back Plate: " ;
    str += info.back_panel==true ? "Yes" : "Not";
    xlsx.write("E3", str);

    QString angle;
    if(info.angle == 0){
        angle = "10";
    }else if(info.angle == 1){
        angle = "15";
    }else if(info.angle == 2){
        angle = "20";
    }else if(info.angle == 3){
        angle = "30";
    }else{
        angle ="CUSTOM";
    }

    QXlsx::Format format6;
    format6.setPatternBackgroundColor(QColor(Qt::gray));

    str = "Console Angle: " ;
    str += angle;
    xlsx.write("A4", str);

    str = "Need Side Plate: " ;
    str += info.side_panel==true ? "Yes" : "Not";
    xlsx.write("E4", str);

    str = "Solar Panel Power: " ;
    str += QString::number(info.power);
    str += "W";
    xlsx.write("A5", str);

    str = "Need Ballast Plate: " ;
    str += info.buttom_panel==true ? "Yes" : "Not";
    xlsx.write("E5", str);

    str = "Quantity of Panles in Vertical Direction: " ;
    str += info.vnum == 1 ? QString::number(2) : QString::number(1);
    xlsx.write("A6", str);

    str = "Quantity of Panles in Horizontal Direction: " ;
    str +=  QString::number(info.hnum);
    xlsx.write("A7", str);

    str = "Array Quantity: " ;
    str += QString::number(info.mnum);
    xlsx.write("A8", str);

    str = "Array Connected: " ;
    str += info.mcon==true ? "Yes" : "Not";
    xlsx.write("A9", str);

    str = "Distance Between Arrays: " ;
    str += QString::number(info.mspace);
    xlsx.write("A10", str);

    str = "Need Rubber pad: " ;
    str += info.plastic_cushion==true ? "Yes" : "Not";
    xlsx.write("E6", str);

    str = "Need Ballast Rail Support: " ;
    str += info.pushload_support==true ? "Yes" : "Not";
    xlsx.write("E7", str);

    float totalNum = info.mnum * (info.vnum == 1 ? 2 :1) * info.hnum;
    str = "Solar Panel Total Quantity: " ;
    str += QString::number(totalNum);
    xlsx.write("E8", str);

    str = "Project Power: " ;
    str += QString::number(info.power * totalNum);
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
//    xlsx.setRowFormat(13, format1);

//    xlsx.mergeCells("A12:A13", format);
//    xlsx.mergeCells("B12:B13", format);
//    xlsx.mergeCells("C12:E13", format);
//    xlsx.mergeCells("F12:F13", format);
//    xlsx.mergeCells("G12:G13", format);
//    xlsx.mergeCells("H12:H13", format);
    xlsx.mergeCells("C12:E12", format);
    xlsx.setRowHeight("13",35.1*ratio);

    QXlsx::Document formatxlsx("format.xlsx");
    for(int i = 0 ; i <1; i++)
    {
        for(int j = 0 ; j < 8; j++)
        {
            char column = 'A' ;
            column += j;
            cellString = column + QString::number(i+20);

            QVariant v = formatxlsx.read(cellString);

            cellString = column + QString::number(i+13);
            writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);

            cellString = column + QString::number(i+12);
            writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);

            xlsx.write(cellString,v);
        }
    }

    for(int i = 0 ; i < resultTable->rowCount(); i++)
    {
        cellString = "C" ;
        cellString += QString::number(i+13);
        cellString += ":E";
        cellString += QString::number(i+13);

        xlsx.mergeCells(cellString, format);

        cellString = "A" +QString::number(i+13)  ;
//        xlsx.setRowHeight(cellString,43.1*ratio);
        writeBorderStyleCell(xlsx, cellString, "", Format::BorderThin);

        xlsx.write(cellString,QString::number(i+1));
        xlsx.setRowFormat(i+14, format);

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


            xlsx.write(cellString,item->text());
        }

//      cellString = "A" +QString::number(i+13)  ;
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
//! [3] //! [4]
{
//    save();
    QDialog::accept();
    return;
    QByteArray className = field("className").toByteArray();
    QByteArray baseClass = field("baseClass").toByteArray();
    QByteArray macroName = field("macroName").toByteArray();
    QByteArray baseInclude = field("baseInclude").toByteArray();

    QString outputDir = field("outputDir").toString();
    QString header = field("header").toString();
    QString implementation = field("implementation").toString();
//! [4]

    QByteArray block;

    if (field("comment").toBool()) {
        block += "/*\n";
        block += "    " + header.toLatin1() + "\n";  // 头文件
        block += "*/\n";
        block += "\n";
    }
    if (field("protect").toBool()) {
        block += "#ifndef " + macroName + "\n";
        block += "#define " + macroName + "\n";
        block += "\n";
    }
    if (field("includeBase").toBool()) {
        block += "#include " + baseInclude + "\n";  // 引用头文件
        block += "\n";
    }

    block += "class " + className;                  // 定义类
    if (!baseClass.isEmpty())
        block += " : public " + baseClass;
    block += "\n";
    block += "{\n";

    /* qmake ignore Q_OBJECT */

    if (field("qobjectMacro").toBool()) {
        block += "    Q_OBJECT\n";
        block += "\n";
    }
    block += "public:\n";

    if (field("qobjectCtor").toBool()) {            // 定义各种风格构造函数
        block += "    " + className + "(QObject *parent = 0);\n";
    } else if (field("qwidgetCtor").toBool()) {
        block += "    " + className + "(QWidget *parent = 0);\n";
    } else if (field("defaultCtor").toBool()) {
        block += "    " + className + "();\n";
        if (field("copyCtor").toBool()) {
            block += "    " + className + "(const " + className + " &other);\n";
            block += "\n";
            block += "    " + className + " &operator=" + "(const " + className
                     + " &other);\n";
        }
    }
    block += "};\n";

    if (field("protect").toBool()) {
        block += "\n";
        block += "#endif\n";
    }

    QFile headerFile(outputDir + "/" + header);
    if (!headerFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(headerFile.fileName())
                             .arg(headerFile.errorString()));
        return;
    }
    headerFile.write(block);

    block.clear();

    if (field("comment").toBool()) {                // 一些说明
        block += "/*\n";
        block += "    " + implementation.toLatin1() + "\n";
        block += "*/\n";
        block += "\n";
    }
    block += "#include \"" + header.toLatin1() + "\"\n";
    block += "\n";

    if (field("qobjectCtor").toBool()) {             // QObject风格的构造器
        block += className + "::" + className + "(QObject *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("qwidgetCtor").toBool()) {       // QWidget风格的构造器
        block += className + "::" + className + "(QWidget *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("defaultCtor").toBool()) {        // 默认构造函数
        block += className + "::" + className + "()\n";
        block += "{\n";
        block += "    // missing code\n";
        block += "}\n";

        if (field("copyCtor").toBool()) {              // 复制构造函数
            block += "\n";
            block += className + "::" + className + "(const " + className
                     + " &other)\n";
            block += "{\n";
            block += "    *this = other;\n";
            block += "}\n";
            block += "\n";
            block += className + " &" + className + "::operator=(const "
                     + className + " &other)\n";
            block += "{\n";
            if (!baseClass.isEmpty())
                block += "    " + baseClass + "::operator=(other);\n";
            block += "    // missing code\n";
            block += "    return *this;\n";
            block += "}\n";
        }
    }

    // 执行生成文件
    QFile implementationFile(outputDir + "/" + implementation);
    if (!implementationFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(implementationFile.fileName())
                             .arg(implementationFile.errorString()));
        return;
    }
    implementationFile.write(block);

//! [5]
    QDialog::accept();
//! [5] //! [6]
}
//! [6]

//! [7]
//!
//!
//!
//void IntroPage::on_sel_sex(const QString &text)
//{
//    QString str;
//    str = "You select " + text;
////    QMessageBox::information(this, tr("Info"), str);
//}

void IntroPage::onChanged(int index)
{
  QMessageBox::warning(this, "Message", cbo_sex->itemText(index), QMessageBox::Ok);
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    parent_wizard = (ClassWizard *) parent;
    // 设置标题
    setTitle(tr("CHIKO WELCOM...."));
    setSubTitle(tr("Thank you to use CHIKO ballast system calculator"));
    // 设置图片
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    classNameLabel = new QLabel(tr("User name:"));
    classNameLineEdit = new QLineEdit;
    classNameLineEdit->setText("chiko");
    classNameLabel->setBuddy(classNameLineEdit);

    baseClassLabel = new QLabel(tr("Email Address:"));
    baseClassLineEdit = new QLineEdit;
    baseClassLabel->setBuddy(baseClassLineEdit);

    baseClassLabel1 = new QLabel(tr("Nation:"));
    baseClassLineEdit1 = new QLineEdit;
    baseClassLabel1->setBuddy(baseClassLineEdit1);

    cbo_sex = new QComboBox();

    cbo_sex->addItem(QWidget::tr("CHN"));
    cbo_sex->addItem(QWidget::tr("ENG"));
    cbo_sex->addItem(QWidget::tr("USA"));
//    cbo_sex->insertItem(2, tr("Insert item"/*
//    cbo_sex->insertSeparator(2);           */

//    gridLayout->addWidget(cbo_sex, 0, 1);

//    connect(cbo_sex, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(IntroPage::on_sel_sex(const QString &)));

    connect(cbo_sex, SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));

    // 展示标签

    qobjectMacroCheckBox = new QCheckBox(tr("Can not be sent to out mail box?"));

    registerField("className*", classNameLineEdit);
    registerField("baseClass", baseClassLineEdit);

    registerField("qobjectMacro", qobjectMacroCheckBox);

    QGridLayout *layout = new QGridLayout;        // InfoPage的布局
    layout->addWidget(classNameLabel, 0, 0);
    layout->addWidget(classNameLineEdit, 0, 1);
    layout->addWidget(baseClassLabel, 2, 0);
    layout->addWidget(baseClassLineEdit, 2, 1);
    layout->addWidget(baseClassLabel1, 4, 0);
    layout->addWidget(cbo_sex, 4, 1);

    layout->addWidget(qobjectMacroCheckBox, 6, 0, 1, 2);
    setLayout(layout);
}
//! [7]

//! [8] //! [9]
void InfoPage::initializePage()
{
    ;
}
InfoPage::InfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    parent_wizard = (ClassWizard *) parent;
    // 标题和子标题
    setTitle(tr("Array Information"));
    setSubTitle(tr("Basic Information "
                   "of your project to generate all components list for your reference."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png")); // Logo

    QGridLayout *layout = new QGridLayout;        // InfoPage的布局

    edt_size_length = new QLineEdit();
    edt_size_width = new QLineEdit();
    edt_size_height = new QLineEdit();

    edt_size_length->setText("10");
    edt_size_width->setText("10");
    edt_size_height->setText("10");

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

    QLabel * label_power = new QLabel(tr("power"));
    edt_power = new QLineEdit();
    edt_power->setValidator(new QIntValidator(0, 1000, this));
    edt_power->setText("10");
    QGroupBox *groupBox = new QGroupBox(tr("Panel info"));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(label_length);
    vbox->addWidget(edt_size_length);
    vbox->addWidget(label_width);
    vbox->addWidget(edt_size_width);
    vbox->addWidget(label_height);
    vbox->addWidget(edt_size_height);
    vbox->addWidget(label_power);
    vbox->addWidget(edt_power);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    QLabel * label_vnum = new QLabel(tr("Vertical QTY"));
    cbo_vnum = new QComboBox();
    cbo_vnum->addItem(QWidget::tr("1 panel landscape"));
    cbo_vnum->addItem(QWidget::tr("2 panels landscape"));
    cbo_vnum->addItem(QWidget::tr("1 panel portrait"));


    QLabel * label_hnum = new QLabel(tr("Horizontal QTY"));
    edt_hnum = new QLineEdit();
    edt_hnum->setValidator(new QIntValidator(0, 1000, this));
    edt_hnum->setText("10");
    QLabel * label_mnum = new QLabel(tr("Array QTY"));
    edt_mnum = new QLineEdit();
    edt_mnum->setValidator(new QIntValidator(0, 1000, this));
    edt_mnum->setText("10");
    QGroupBox *groupBox_number = new QGroupBox(tr("Array info"));

    QVBoxLayout *vbox_number = new QVBoxLayout;
    vbox_number->addWidget(label_vnum);
    vbox_number->addWidget(cbo_vnum);
    vbox_number->addWidget(label_hnum);
    vbox_number->addWidget(edt_hnum);
    vbox_number->addWidget(label_mnum);
    vbox_number->addWidget(edt_mnum);
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

    QLabel * label_mspace = new QLabel(tr("Spacing between every array mm"));
    edt_mspace = new QLineEdit();
    edt_mspace->setValidator(new QIntValidator(0, 1000, this));
    edt_mspace->setText("10");
    QLabel * label_orientation = new QLabel(tr("Console orientation"));
    cbo_orientation = new QComboBox();
    cbo_orientation->addItem(QWidget::tr("south"));
    cbo_orientation->addItem(QWidget::tr("east/west"));

    QGroupBox *groupBox_value = new QGroupBox(tr("Ballast I Console info"));

    QVBoxLayout *vbox_value = new QVBoxLayout;
    vbox_value->addWidget(label_orientation);
    vbox_value->addWidget(cbo_orientation);
    vbox_value->addWidget(label_angle);
    vbox_value->addWidget(cbo_angle);
    vbox_value->addWidget(label_mspace);
    vbox_value->addWidget(edt_mspace);

    vbox_value->addStretch(1);
    groupBox_value->setLayout(vbox_value);

    chk_mcon = new QCheckBox(tr("All array connected?"));
    chk_back_panel = new QCheckBox(tr("Need back wind plate?"));
    chk_side_panel = new QCheckBox(tr("Need side wind plate?"));
    chk_pushload_support = new QCheckBox(tr("Need ballast support rails?"));
    chk_buttom_panel = new QCheckBox(tr("Need ballast plate?"));
    chk_plastic_cushion = new QCheckBox(tr("Need rubber pad?"));

    QGroupBox *groupBox_checkbox = new QGroupBox(tr("Ballast I console optional function"));

    QVBoxLayout *vbox_checkbox = new QVBoxLayout;
    vbox_checkbox->addWidget(chk_mcon);
    vbox_checkbox->addWidget(chk_back_panel);
    vbox_checkbox->addWidget(chk_side_panel);
    vbox_checkbox->addWidget(chk_pushload_support);
    vbox_checkbox->addWidget(chk_buttom_panel);
    vbox_checkbox->addWidget(chk_plastic_cushion);

    vbox_checkbox->addStretch(1);
    groupBox_checkbox->setLayout(vbox_checkbox);

    registerField("edt_size_length", edt_size_length);
    registerField("edt_size_width", edt_size_width);
    registerField("edt_size_height", edt_size_height);

    registerField("cbo_vnum", cbo_vnum);
    registerField("edt_hnum", edt_hnum);
    registerField("edt_mnum", edt_mnum);

    registerField("cbo_angle", cbo_angle);
    registerField("edt_power", edt_power);
    registerField("edt_mspace", edt_mspace);
    registerField("cbo_orientation", cbo_orientation);

    registerField("chk_mcon", chk_mcon);
    registerField("chk_back_panel", chk_back_panel);
    registerField("chk_side_panel", chk_side_panel);
    registerField("chk_buttom_panel", chk_buttom_panel);
    registerField("chk_plastic_cushion", chk_plastic_cushion);
    registerField("chk_pushload_support", chk_pushload_support);

    layout->addWidget(groupBox_value, 1,0,1,1);
    layout->addWidget(groupBox, 1,1,1,1);

    layout->addWidget(groupBox_number, 2,0,1,1);
    layout->addWidget(groupBox_checkbox, 2,1,1,1);

    setLayout(layout);

    this->resize(766,341);
}


//! [14]
bool InfoPage::validatePage()
{
    parent_wizard->info.size_length = field("edt_size_length").toFloat();//L
    parent_wizard->info.size_width = field("edt_size_width").toFloat();
    parent_wizard->info.size_height = field("edt_size_height").toFloat();

    parent_wizard->info.vnum = field("cbo_vnum").toInt();//V
    parent_wizard->info.hnum = field("edt_hnum").toInt();//N
    parent_wizard->info.mnum = field("edt_mnum").toInt();//M

    parent_wizard->info.angle = field("cbo_angle").toInt();
    parent_wizard->info.power = field("edt_power").toFloat();
    parent_wizard->info.mspace = field("edt_mspace").toFloat();
    parent_wizard->info.orientation = field("cbo_orientation").toInt();

    parent_wizard->info.mcon = field("chk_mcon").toBool();
   parent_wizard-> info.back_panel = field("chk_back_panel").toBool();
    parent_wizard->info.side_panel = field("chk_side_panel").toBool();
    parent_wizard->info.buttom_panel = field("chk_buttom_panel").toBool();
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
        return  parent_wizard->info.back_panel;
    case 6:
        return  parent_wizard->info.side_panel;
    case 7:
        return  parent_wizard->info.back_panel;
    case 8:
    case 9:
        return true;
    case 10:
        return parent_wizard->info.pushload_support ;
    case 11:
        return parent_wizard->info.pushload_support;
    case 12:
        return parent_wizard->info.mcon && parent_wizard->info.pushload_support;
    case 13:
        return parent_wizard->info.plastic_cushion;
    case 14:
        return parent_wizard->info.mcon ;
    }

}

void ResultPage::initializePage()
{
    setTitle(tr("Result"));
    setSubTitle(tr("According to "
                   "your information,the components list as following for your reference."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

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

    QXlsx::Document xlsx("rule.xlsx");
    xlsx.saveAs("rule_test.xlsx");
    tableWidget->clear();
    QStringList header;
    header<<"Item no"<<"Description"<<"Image"<<"Unit"<<"Qty";
    tableWidget->setHorizontalHeaderLabels(header);

    for(int i = 0; i<15; i++)
    {
        row = 5 + i;
        QVariant v = xlsx.read(row,colum);

        QString angle;
        if(parent_wizard->info.angle == 0){
            angle = "10";
        }else if(parent_wizard->info.angle == 1){
            angle = "15";
        }else if(parent_wizard->info.angle == 2){
            angle = "20";
        }else if(parent_wizard->info.angle == 3){
            angle = "30";
        }else{
            angle ="CUSTOM";
        }

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
            }else if(name.indexOf("CK-FTE-F")==0){
                 iconStr=(":/images/chiko/CK-FTE-F.jpg");
            }else if(name.indexOf("CK-FTM-F")==0){
                 iconStr=(":/images/chiko/CK-FTM-F.jpg");
            }

            tableWidget->setItem(rowNum,0,new QTableWidgetItem(name));
            tableWidget->setItem(rowNum,1,new QTableWidgetItem(desc));
            tableWidget->setItem(rowNum,2,new QTableWidgetItem(QIcon(QPixmap(iconStr)), ""));
            tableWidget->setItem(rowNum,3,new QTableWidgetItem("sets"));
            tableWidget->setItem(rowNum,4,new QTableWidgetItem(QString::number(result)));
            rowNum ++;
        }
    }

    tableWidget->setRowCount(rowNum);
    tableWidget->resizeColumnToContents(0);
    tableWidget->resizeColumnToContents(1);
    /*
       tableWidget->resizeColumnsToContents();
    xlsx.write("A1", v);
    xlsx.saveAs("Test.xlsx");*/

//    commentCheckBox = new QCheckBox(tr("&Start generated files with a "
////! [14]
//                                       "comment"));
//    commentCheckBox->setChecked(true);    // 初始化为勾上

//    protectCheckBox = new QCheckBox(tr("&Protect header file against multiple "
//                                       "inclusions"));
//    protectCheckBox->setChecked(true);   // 初始化为勾上

//    macroNameLabel = new QLabel(tr("&Macro name:"));  // label和lineEdit
//    macroNameLineEdit = new QLineEdit;
//    macroNameLabel->setBuddy(macroNameLineEdit);

//    // 跟上一页的基类关联
//    includeBaseCheckBox = new QCheckBox(tr("&Include base class definition"));
//    baseIncludeLabel = new QLabel(tr("Base class include:"));
//    baseIncludeLineEdit = new QLineEdit;
//    baseIncludeLabel->setBuddy(baseIncludeLineEdit);

//    // protectCheckBox和它下方的macroNameLabel,macroNameLineEdit保持同步
//    connect(protectCheckBox, SIGNAL(toggled(bool)),
//            macroNameLabel, SLOT(setEnabled(bool)));
//    connect(protectCheckBox, SIGNAL(toggled(bool)),
//            macroNameLineEdit, SLOT(setEnabled(bool)));
//    // includeBaseCheckBox和它右边的baseIncludeLabel,baseIncludeLineEdit保持同步
//    connect(includeBaseCheckBox, SIGNAL(toggled(bool)),
//            baseIncludeLabel, SLOT(setEnabled(bool)));
//    connect(includeBaseCheckBox, SIGNAL(toggled(bool)),
//            baseIncludeLineEdit, SLOT(setEnabled(bool)));

//    // 注册域
//    registerField("comment", commentCheckBox);
//    registerField("protect", protectCheckBox);
//    registerField("macroName", macroNameLineEdit);
//    registerField("includeBase", includeBaseCheckBox);
//    registerField("baseInclude", baseIncludeLineEdit);

//    QGridLayout *layout = new QGridLayout;   // 布局
//    layout->setColumnMinimumWidth(0, 20);    // 设置第一列的最小宽20像素
//    layout->addWidget(commentCheckBox, 0, 0, 1, 3);
//    layout->addWidget(protectCheckBox, 1, 0, 1, 3);
//    layout->addWidget(macroNameLabel, 2, 1);
//    layout->addWidget(macroNameLineEdit, 2, 2);
//    layout->addWidget(includeBaseCheckBox, 3, 0, 1, 3);
//    layout->addWidget(baseIncludeLabel, 4, 1);
//    layout->addWidget(baseIncludeLineEdit, 4, 2);
////! [15]
//    setLayout(layout);
}

// 上一页类信息的基类和宏影响这一页，上一页点NEXT时触发本槽

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

void caculateResult(int egType)
{
//    switch (egType){
//    case:1
//     case:2
//     case:3
//     case:4
//     case:5
//     case:6
//     case:7
//     case:8
//     case:9
//     case:10
//     case:11
//     case:12
// default:
//     break;

//    }

}

void excelTest(){

    QXlsx::Document xlsx("rule.xlsx");

    QVariant v = xlsx.read(5,5);
    xlsx.write("A1", v);
    xlsx.saveAs("Test.xlsx");
//    xlsx.read()
//    QAxObject excel("");

//    excel.setProperty("Visible", true);
//    QAxObject *work_books = excel.querySubObject("WorkBooks");
//    work_books->dynamicCall("Open (const QString&)", QString("rule.xlsx"));

//    QVariant title_value = excel.property("Caption");  //获取标题
//    qDebug()<<QString("excel title : ")<<title_value;
//    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
//    QAxObject *work_sheets = work_book->querySubObject("Sheets");  //Sheets也可换用WorkSheets


//    int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目
//    qDebug()<<QString("sheet count : ")<<sheet_count;
//    for(int i=1; i<=sheet_count; i++)
//    {
//    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", i);  //Sheets(int)也可换用Worksheets(int)
//    QString work_sheet_name = work_sheet->property("Name").toString();  //获取工作表名称
//    QString message = QString("sheet ")+QString::number(i, 10)+ QString(" name");
//    qDebug()<<message<<work_sheet_name;
//    }
//    if(sheet_count > 0)
//    {
//    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
//    QAxObject *used_range = work_sheet->querySubObject("UsedRange");
//    QAxObject *rows = used_range->querySubObject("Rows");
//    QAxObject *columns = used_range->querySubObject("Columns");
//    int row_start = used_range->property("Row").toInt();  //获取起始行
//    int column_start = used_range->property("Column").toInt();  //获取起始列
//    int row_count = rows->property("Count").toInt();  //获取行数
//    int column_count = columns->property("Count").toInt();  //获取列数
//    for(int i=row_start; i < row_count ; i++)
//    {
//    for(int j=column_start; j < column_count ; j++)
//    {
//    QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
//    QVariant cell_value = cell->property("Value");  //获取单元格内容
//    QString message = QString("row-")+QString::number(i, 10)+QString("-column-")+QString::number(j, 10)+QString(":");
//    qDebug()<<message<<cell_value;
//    }
//    }
//    }

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

    parent_wizard->resultTable = tableWidget;
    //    也可用下面的方法构造QTableWidget对象
    //    QTableWidget *tableWidget = new QTableWidget;
    tableWidget->setRowCount(12);     //设置行数为10
    tableWidget->setColumnCount(5);   //设置列数为5
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
