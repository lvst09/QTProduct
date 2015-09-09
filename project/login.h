#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <qlineedit.h>
class login : public QWidget
{
    Q_OBJECT
public:
    explicit login(QWidget *parent = 0);

private:
    QLineEdit * edt_name;
    QLineEdit * edt_password;
signals:

public slots:
    void loginButtonClicked();
};

#endif // LOGIN_H
