#ifndef APPLICATION_H
#define APPLICATION_H
//with application you can manage all widgets in your app
#include <QApplication>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>




using namespace std;

class signwindow;
class MainWindow;


class application : public QApplication
{
    Q_OBJECT

public:
    static application *myInstance;
    static application* instace(){return application::myInstance;}
    application(int argc, char *argv[]);
    MainWindow* getlogin(){return loginPage;}
    void init();
    // void addUser(User);
    // vector<User> getAllUsers();
    // void Captcha();


private:
    MainWindow * loginPage;
    // vector<User> Users;

};
static inline application *app(){
    return application::instace();
}

#endif // APPLICATION_H
