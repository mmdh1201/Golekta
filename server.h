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
    // bool isValidEmail(const QString &email);
    // bool isFieldUnique(const QString &field, const QString &value, QString fileName);
    // void handleEnrollInCourse(QTcpSocket *clientSocket, const QJsonObject &enrollmentData);
    // void handleAddUser(QTcpSocket *clientSocket, const QJsonObject &userData);
    // void handleGetInstructors(QTcpSocket *clientSocket);
    // void handleGetProfessorInfo(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetStudentInfo(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetCourseList(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleUploadContent(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleSubmitHomework(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleSubmitExam(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleLoginRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleStudentEnrollInCourse(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetCourses(QTcpSocket *clientSocket);
    // void handleGetRegisteredCourses(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetExamQuestion(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleSubmitExamAnswer(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetHomework(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleSubmitHomeWorkAnswer(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetContent(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetStudentGrade(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleRemoveStudentFromCourse(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleGetParticipants(QTcpSocket *clientSocket, const QJsonObject &request);
    // void handleSetStudentGrade(QTcpSocket *clientSocket, const QJsonObject &request);

private:
    QTcpServer *server;
};

#endif
