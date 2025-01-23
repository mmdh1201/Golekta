#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QDebug>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

private slots:
    void handleNewConnection();
    bool isValidEmail(const QString &email);
    bool isFieldUnique(const QString &field, const QString &value, QString fileName);
    void handleEnrollInCourse(QTcpSocket *clientSocket, const QJsonObject &enrollmentData);
    void handleAddUser(QTcpSocket *clientSocket, const QJsonObject &userData);
    void handleGetInstructors(QTcpSocket *clientSocket);
    void handleGetProfessorInfo(QTcpSocket *clientSocket, const QJsonObject &request);
    void handleGetStudentInfo(QTcpSocket *clientSocket, const QJsonObject &request);
    void handleGetCourseList(QTcpSocket *clientSocket, const QJsonObject &request);
    void handleUploadContent(QTcpSocket *clientSocket, const QJsonObject &request);
    void handleSubmitHomework(QTcpSocket *clientSocket, const QJsonObject &request);
    void handleSubmitExam(QTcpSocket *clientSocket, const QJsonObject &request);
    void handleLoginRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    void handleLogin(QTcpSocket *clientSocket,const QJsonObject &requested);

private:
    QTcpServer *server;
};

#endif
