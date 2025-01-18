#include "login.h"

User::User(const QString &uname, const QString &pass, const QString &fname, const QString &email, const QString &gen, const int &age):
    Uname(uname), Password(pass.toUtf8()), Fullname(fname), Email(email), Gen(gen), Age(age)
{}
QString User::getUname()const{return Uname;}
QString User::getPass()const{return Password;}
QString User::getFname()const{return Fullname;}
QString User::getEmail()const{return Email;}
QString User::getGen()const{return Gen;}
int User::getAge()const{return Age;}

bool User::operator==(const User& other)const{
    return Uname == other.Uname;
}

