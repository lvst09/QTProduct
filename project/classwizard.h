//! [0]
//!
#include <qwizard.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <QTableWidget>
#include <QTableWidgetItem>


 using namespace std;
class Information
{
public:
    float size_length = 0;//L
    float size_width;
    float size_height;

    int vnum;//V
    int hnum;//N
    int mnum;//M

    int angle;
    float power;
    float mspace;
    int orientation;

    bool mcon;
    bool back_panel;
    bool side_panel;
    bool buttom_panel;
    bool plastic_cushion;
    bool pushload_support;

    int egType;
};

class ClassWizard : public QWizard
{
    Q_OBJECT

public:
    ClassWizard(QWidget *parent = 0);

    void accept();
    void save(QString fileName);
    QString fileName;
    Information info;
    QTableWidget *resultTable;

    QVector<QString> citys_eng;
    QVector<QString> citys_ger;
    QVector<QString> citys_aus;
    QVector<QString> citys_jap;
    QVector<QString> citys_hol;

    void loadCities(QString fileName, QVector<QString> & vec);
};
//! [0]

//! [1]
// 介绍页只有一个标签
class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    ClassWizard * parent_wizard;


    QLabel *classNameLabel;
    QLabel *baseClassLabel;
    QLabel *baseClassLabel1;
    QLineEdit *classNameLineEdit;
    QLineEdit *baseClassLineEdit;
    QLineEdit *baseClassLineEdit1;
    QCheckBox *qobjectMacroCheckBox;
    QGroupBox *groupBox;
    QRadioButton *qobjectCtorRadioButton;
    QRadioButton *qwidgetCtorRadioButton;
    QRadioButton *defaultCtorRadioButton;
    QCheckBox *copyCtorCheckBox;
    QComboBox * cbo_sex;

    void on_sel_sex(const QString &text);
    void onChanged(int index);



};

//! [1]

//! [2]
// 类信息的东西较多
class InfoPage : public QWizardPage
{
    Q_OBJECT

public:
    InfoPage(QWidget *parent = 0);

    bool validatePage ();
    void initializePage();
    int layoutImageIndex();
public slots: void refreshPreview();
public slots: void refreshLayout();
public slots: void onCountryChanged(int index);
private:
    ClassWizard * parent_wizard;

    QLabel * previewLabel;
    QWidget * layoutWidget;

    QComboBox * cbo_angle;
    QLineEdit * edt_size_length;
    QLineEdit * edt_size_width;
    QLineEdit * edt_size_height;
    QLineEdit * edt_power;
    QLineEdit * edt_weight;
    QComboBox * cbo_vnum;
    QLineEdit * edt_hnum;
    QLineEdit * edt_mnum;
    QCheckBox * chk_mcon;
    QLineEdit * edt_mspace;
    QCheckBox * chk_back_panel;
    QCheckBox * chk_side_panel;
    QCheckBox * chk_buttom_panel;
    QCheckBox * chk_plastic_cushion;
    QCheckBox * chk_pushload_support;

    QComboBox * cbo_orientation;
    QComboBox * cbo_country;
    QComboBox * cbo_city;

    QLineEdit * edt_building_L;
    QLineEdit * edt_building_W;
    QLineEdit * edt_building_H;

    void refreshCities(QString country);

};
//! [2]

//! [3]
// 代码风格页
class ResultPage : public QWizardPage
{
    Q_OBJECT

public:
    ResultPage(QWidget *parent = 0);

protected:
    void initializePage();
    //
    bool verifyConstraint(int rowIndex);
private:
    bool validatePage();
    ClassWizard *parent_wizard ;
    QTableWidget *tableWidget;

    QCheckBox *commentCheckBox;
    QCheckBox *protectCheckBox;
    QCheckBox *includeBaseCheckBox;
    QLabel *macroNameLabel;
    QLabel *baseIncludeLabel;
    QLineEdit *macroNameLineEdit;
    QLineEdit *baseIncludeLineEdit;
};
//! [3]

// 输出文件页，输出地址+头文件+实现文件
// 重写了initializePage()它在QWizard::restart或点击NEXT时被调用
class OutputFilesPage : public QWizardPage
{
    Q_OBJECT

public:
    OutputFilesPage(QWidget *parent = 0);

protected:
    void initializePage();

private:
    ClassWizard * parent_wizard;

    QLabel *outputDirLabel;
    QLabel *headerLabel;
    QLabel *implementationLabel;
    QLineEdit *outputDirLineEdit;
    QLineEdit *headerLineEdit;
    QLineEdit *implementationLineEdit;
};

// 最后的结论页，也只有个一个标签
class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);
protected:
    void initializePage();

private:
    QLabel *label;
};
