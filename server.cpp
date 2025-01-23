#include "server.h"//;
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QDebug>
#include <QRegularExpression>

Server::Server(QObject *parent)
    : QObject(parent), server(new QTcpServer(this))
{
    if (!server->listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server could not start!";
        return;
    }
    qDebug() << "Server started on port 12345";

    connect(server, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

Server::~Server()
{
    server->close();
}

bool Server::isFieldUnique(const QString &field, const QString &value, QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return false;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject object = document.object();

    for (const QString &key :object.keys()) {
        QJsonObject obj = object[key].toObject();

        if (obj[field] == value) {
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

bool Server::isValidEmail(const QString &email)
{
    QRegularExpression emailRegex("^[a-zA-Z0-9]+\\.iut@gmail\\.com$");
    QRegularExpressionMatch match = emailRegex.match(email);
    return match.hasMatch();
}

void Server::handleNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
        QByteArray data = clientSocket->readAll();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject()) {
            clientSocket->write("Invalid data received.");
            clientSocket->disconnectFromHost();
            return;
        }

        QJsonObject request = doc.object();
        QString requestType = request["type"].toString();

        if (requestType == "add_user") {
            handleAddUser(clientSocket, request);
        } else if (requestType == "get_instructors") {
            handleGetInstructors(clientSocket);
        }else if (requestType == "enroll_in_course") {
            handleEnrollInCourse(clientSocket, request);
        }else if (requestType == "get_professor_info") {
            handleGetProfessorInfo(clientSocket, request);
        }else if (requestType == "get_course_list") {
            handleGetCourseList(clientSocket, request);
        }else if (requestType == "upload_content") {
            handleUploadContent(clientSocket, request);
        }else if (requestType == "upload_homework") {
            handleUploadContent(clientSocket, request);
        }else if (requestType == "submit_homework") {
            handleSubmitHomework(clientSocket, request);
        }else if (requestType == "submit_exam") {
            handleSubmitExam(clientSocket, request);
        }else if (requestType == "login_request") {
            handleLoginRequest(clientSocket, request);
        }else if(requestType == "get_student_info"){
            handleGetStudentInfo(clientSocket,request);
        }else {
            QJsonObject errorMessage;
            errorMessage["type"] = "error";
            errorMessage["message"] = "Invalid request type.";
            QJsonDocument errorDoc(errorMessage);
            clientSocket->write(errorDoc.toJson());
            clientSocket->disconnectFromHost();
        }
    });
}

void Server::handleAddUser(QTcpSocket *clientSocket, const QJsonObject &userData)
{
    QString email = userData["email"].toString();
    QString username = userData["username"].toString();
    QString officePhone = userData["office_phone"].toString();
    QString officeNumber = userData["office_number"].toString();

    if (!isValidEmail(email)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Invalid email format. The email must be in the format name.iut@gmail.com.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    if (!isFieldUnique("username", username, "users.json")) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Username already exists.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    if (!isFieldUnique("email", email, "users.json")) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Email already exists.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    if (!isFieldUnique("office_phone", officePhone, "users.json")) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Office phone already exists.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    if (!isFieldUnique("office_number", officeNumber, "users.json")) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Office number already exists.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QFile file("users.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Error opening users.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error saving user data.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject usersObject;

    if (!document.isNull()) {
        usersObject = document.object();
    }

    usersObject[username] = userData;

    QJsonDocument newDoc(usersObject);
    file.resize(0);
    file.write(newDoc.toJson());
    file.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "User added successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void Server::handleEnrollInCourse(QTcpSocket *clientSocket, const QJsonObject &enrollmentData)
{

    QString name = enrollmentData["course_name"].toString();
    QString units = enrollmentData["units"].toString();
    QString courseNumber = enrollmentData["course_number"].toString();
    QString courseGroup = enrollmentData["course_group"].toString();
    QString courseCapacity =  enrollmentData["course_capacity"].toString();
    QString courseLocation = enrollmentData["course_location"].toString();
    QString  courseDays = enrollmentData ["course_days"].toString();
    QString courseTime = enrollmentData["course_time"].toString();
    QString courseFinalDate = enrollmentData["course_final_date"].toString();
    QString courseMasterName = enrollmentData["course_master_name"].toString();
    QString courseId = enrollmentData["course_id"].toString();
    QFile file("courses.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Error opening courses.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error saving course data.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }
    if (!isFieldUnique("course_id", courseId, "courses.json")) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Course id already exists.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }
    if (!isFieldUnique("course_location", courseLocation, "courses.json") && isFieldUnique("course_time", courseTime, "courses.json") && isFieldUnique("course_days", courseDays, "courses.json")) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Location conflict.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }
    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject coursesObject;

    if (!document.isNull()) {
        coursesObject = document.object();
    }
    coursesObject[courseId] = enrollmentData;

    QJsonDocument newDoc(coursesObject);
    file.resize(0);
    file.write(newDoc.toJson());
    file.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Course added successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void Server::handleGetInstructors(QTcpSocket *clientSocket)
{
    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening users.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error reading user data.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject usersObject = document.object();

    QJsonArray instructorsArray;
    for (const QString &key : usersObject.keys()) {
        QJsonObject user = usersObject[key].toObject();
        if (user["role"].toString() == "master") {
            QString fullName = user["first_name"].toString() + " " + user["last_name"].toString();
            instructorsArray.append(fullName);
        }
    }

    QJsonObject response;
    response["type"] = "instructors_list";
    response["instructors"] = instructorsArray;
    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void Server::handleGetProfessorInfo(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString username = request["username"].toString();

    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening users.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error reading user data.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject usersObject = document.object();

    if (!usersObject.contains(username)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Professor not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject professorInfo = usersObject[username].toObject();
    QJsonObject response;
    response["type"] = "professor_info";
    response["name"] = professorInfo["first_name"].toString() + " " + professorInfo["last_name"].toString();
    response["faculty"] = professorInfo["faculty"].toString();
    response["email"] = professorInfo["email"].toString();
    response["office_phone"] = professorInfo["office_phone"].toString();
    response["office_number"] = professorInfo["office_number"].toString();

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}
void Server::handleGetStudentInfo(QTcpSocket *clientSocket, const QJsonObject &request){
    QString username = request["username"].toString();

    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening users.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error reading user data.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject usersObject = document.object();


    if (!usersObject.contains(username)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Student not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject stdInfo = usersObject[username].toObject();
    QJsonObject response;
    response["type"] = "student_info";
    response["name"] = stdInfo["first_name"].toString() + " " + stdInfo["last_name"].toString();
    response["faculty"] = stdInfo["faculty"].toString();
    response["email"] = stdInfo["email"].toString();
    response["completed_units"] = stdInfo["completed_units"].toString();
    response["password"] = stdInfo["password"].toString();
    qDebug()<<response["type"].toString();

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();

}


void Server::handleGetCourseList(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString professorName = request["professor_name"].toString();
    QFile file("courses.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening courses.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error reading course data.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject coursesObject = document.object();
    //qDebug()<<coursesObject.keys();

    QJsonArray coursesListArray;

    for (const QString &key : coursesObject.keys()) {

        QJsonObject course = coursesObject[key].toObject();
        QString master = course["course_master_name"].toString();


        if (master == professorName) {
            coursesListArray.append(course["course_name"].toString());
        }
    }

    QJsonObject response;
    response["type"] = "course_list";

    response["courses"] = coursesListArray;

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}


void Server::handleUploadContent(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseName = request["course_name"].toString();
    QString professorName = request["professor_name"].toString();
    QString fileName = request["file_name"].toString();

    QByteArray fileData = clientSocket->readAll();

    QFile file("content.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Error opening content.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error saving content.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileDataArray = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileDataArray);
    QJsonObject contentObject = document.object();

    contentObject[courseName + "_" + professorName] = QJsonObject{
        {"file_name", fileName}
    };

    QJsonDocument contentDoc(contentObject);
    file.resize(0);
    file.write(contentDoc.toJson());
    file.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Content uploaded successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}


void Server::handleSubmitHomework(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseName = request["course_name"].toString();
    QString professorName = request["professor_name"].toString();
    QString homeworkText = request["homework_text"].toString();

    QFile file("homework.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Error opening homework.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error saving homework.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject homeworkObject = document.object();

    homeworkObject[courseName + "_" + professorName] = QJsonObject{
        {"homework_text", homeworkText}
    };

    QJsonDocument homeworkDoc(homeworkObject);
    file.resize(0);
    file.write(homeworkDoc.toJson());
    file.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Homework submitted successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void Server::handleSubmitExam(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseName = request["course_name"].toString();
    QString professorName = request["professor_name"].toString();
    QString examQuestion = request["exam_question"].toString();
    QString startingTime = request["starting_time"].toString();
    QString endingTime = request["ending_time"].toString();

    QFile file("exams.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Error opening exams.json";
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Error saving exam data.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject examsObject = document.object();

    examsObject[courseName + "_" + professorName] = QJsonObject{
        {"exam_question", examQuestion},
        {"starting_time", startingTime},
        {"ending_time", endingTime}
    };

    QJsonDocument examsDoc(examsObject);
    file.resize(0);
    file.write(examsDoc.toJson());
    file.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Exam details submitted successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void Server::handleLoginRequest(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString username = request["username"].toString();
    QString password = request["password"].toString();

    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open users.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject usersObject = document.object();
    file.close();

    if (!usersObject.contains(username)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Username not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject user = usersObject[username].toObject();

    if (user["password"].toString() != password) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Incorrect password.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QString role = user["role"].toString();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Login successful.";
    successMessage["role"] = role;
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}



