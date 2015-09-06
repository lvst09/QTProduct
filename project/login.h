#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

class login : public QWidget
{
    Q_OBJECT
public:
    explicit login(QWidget *parent = 0);

signals:

public slots:
    void loginButtonClicked();
};

#endif // LOGIN_H
