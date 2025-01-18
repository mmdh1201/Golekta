#ifndef LOGIN_H
#define LOGIN_H
#include <QCryptographicHash>
#include <QByteArray>
#include <QSettings>
#include <QString>
using namespace std;

class User{
public:
    User();
    User(const QString&,const QString&,const QString&,const QString&,const QString&,const int&);
    QString getUname()const;
    QString getPass()const;
    QString getFname()const;
    QString getEmail()const;
    QString getGen()const;
    int getAge()const;

private:
    const QString Uname;
    const QByteArray Password;
    const QString Fullname;
    const QString Email;
    const QString Gen;
    const int Age;
    bool operator==(const User&) const;

};




#endif
