#include "ClientHandler.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QDebug>
#include <QRegularExpression>
#include <QtConcurrent>


int ClientHandler::cntr = 0;
ClientHandler::ClientHandler(QTcpSocket *socket, QObject *parent)
    : QThread(parent), clientSocket(socket)
{

}

void ClientHandler::run()
{
    connect(clientSocket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    exec();
    cntr++;
    qDebug()<<cntr;
}

bool ClientHandler::isFieldUnique(const QString &field, const QString &value, QString fileName)
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

bool ClientHandler::isValidEmail(const QString &email)
{
    QRegularExpression emailRegex("^[a-zA-Z0-9]+\\.iut@gmail\\.com$");
    QRegularExpressionMatch match = emailRegex.match(email);
    return match.hasMatch();
}

void ClientHandler::onReadyRead()
{

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
    }else if (requestType == "student_enroll_in_course") {
        handleStudentEnrollInCourse(clientSocket, request);
    }else if (requestType == "get_courses") {
        handleGetCourses(clientSocket);
    }else if (requestType == "get_registered_courses") {
        handleGetRegisteredCourses(clientSocket, request);
    }else if (requestType == "get_exam_question") {
        handleGetExamQuestion(clientSocket, request);
    }else if (requestType == "submit_exam_answer") {
        handleSubmitExamAnswer(clientSocket, request);
    }else if (requestType == "get_homework_question") {
        handleGetHomework(clientSocket, request);
    }else if (requestType == "submit_homework_answer") {
        handleSubmitHomeWorkAnswer(clientSocket, request);
    }else if (requestType == "get_content") {
        handleGetContent(clientSocket, request);
    }else if (requestType == "get_student_grade") {
        handleGetStudentGrade(clientSocket, request);
    }else if (requestType == "remove_student_from_course") {
        handleRemoveStudentFromCourse(clientSocket, request);
    }else if (requestType == "get_participants") {
        handleGetParticipants(clientSocket, request);
    }else if (requestType == "set_student_grade") {
        handleSetStudentGrade(clientSocket, request);
    }else {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Invalid request type.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
    }
}

void ClientHandler::handleAddUser(QTcpSocket *clientSocket, const QJsonObject &userData)
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

void ClientHandler::handleEnrollInCourse(QTcpSocket *clientSocket, const QJsonObject &enrollmentData)
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

void ClientHandler::handleGetInstructors(QTcpSocket *clientSocket)
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

void ClientHandler::handleGetProfessorInfo(QTcpSocket *clientSocket, const QJsonObject &request)
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


void ClientHandler::handleGetCourseList(QTcpSocket *clientSocket, const QJsonObject &request)
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


void ClientHandler::handleUploadContent(QTcpSocket *clientSocket, const QJsonObject &request)
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


void ClientHandler::handleSubmitHomework(QTcpSocket *clientSocket, const QJsonObject &request)
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

void ClientHandler::handleSubmitExam(QTcpSocket *clientSocket, const QJsonObject &request)
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

void ClientHandler::handleLoginRequest(QTcpSocket *clientSocket, const QJsonObject &request)
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

void ClientHandler::handleGetStudentInfo(QTcpSocket *clientSocket, const QJsonObject &request){
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
    response["completed_units"] = QString::number(stdInfo["completed_units"].toInt());
    response["password"] = stdInfo["password"].toString();
    //qDebug()<<response["completed_units"].toString();

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();

}

void ClientHandler::handleGetCourses(QTcpSocket *clientSocket)
{
    QFile file("courses.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open courses.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject coursesObject = document.object();
    file.close();

    QJsonObject response;
    response["type"] = "course_list";
    response["courses"] = coursesObject;

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}


void ClientHandler::handleStudentEnrollInCourse(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseId = request["course_id"].toString();
    QString studentUsername = request["student_username"].toString();

    QFile coursesFile("courses.json");
    if (!coursesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open courses.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray coursesData = coursesFile.readAll();
    QJsonDocument coursesDoc = QJsonDocument::fromJson(coursesData);
    QJsonObject coursesObject = coursesDoc.object();
    coursesFile.close();

    if (!coursesObject.contains(courseId)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Course not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject course = coursesObject[courseId].toObject();
    QString courseName = course["course_name"].toString();
    QString courseMasterName = course["course_master_name"].toString();

    QString courseKey = courseName + "_" + courseMasterName;

    QFile participantsFile("participants.json");
    if (!participantsFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open participants.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray participantsData = participantsFile.readAll();
    QJsonDocument participantsDoc = QJsonDocument::fromJson(participantsData);
    QJsonObject participantsObject = participantsDoc.object();

    if (!participantsObject.contains(courseKey)) {
        participantsObject[courseKey] = QJsonObject{
            {"participants", QJsonObject()}
        };
    }

    QJsonObject courseObject = participantsObject[courseKey].toObject();
    QJsonObject participants = courseObject["participants"].toObject();

    if (participants.contains(studentUsername)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Student already enrolled in this course.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    participants[studentUsername] = -1;
    courseObject["participants"] = participants;
    participantsObject[courseKey] = courseObject;

    participantsFile.resize(0);
    participantsFile.write(QJsonDocument(participantsObject).toJson());
    participantsFile.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Student enrolled successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}


void ClientHandler::handleGetRegisteredCourses(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString studentUsername = request["student_username"].toString();

    QFile participantsFile("participants.json");
    if (!participantsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open participants.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = participantsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject participantsObject = document.object();
    participantsFile.close();

    QJsonArray registeredCourses;
    for (const QString &key : participantsObject.keys()) {
        QJsonObject courseObject = participantsObject[key].toObject();
        QJsonObject participants = courseObject["participants"].toObject();

        if (participants.contains(studentUsername)) {
            QJsonObject courseInfo;
            courseInfo["course_key"] = key;
            courseInfo["grade"] = participants[studentUsername].toInt();
            registeredCourses.append(courseInfo);
        }
    }

    QJsonObject response;
    response["type"] = "registered_courses";
    response["courses"] = registeredCourses;

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}


void ClientHandler::handleGetExamQuestion(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();

    QFile examsFile("exams.json");
    if (!examsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open exams.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = examsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject examsObject = document.object();
    examsFile.close();

    if (!examsObject.contains(courseKey)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Exam question not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject exam = examsObject[courseKey].toObject();
    QJsonObject response;
    response["type"] = "exam_question";
    response["exam_question"] = exam["exam_question"].toString();

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleSubmitExamAnswer(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();
    QString studentUsername = request["student_username"].toString();
    QString answer = request["answer"].toString();

    QFile answersFile("answers.json");
    if (!answersFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open answers.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = answersFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject answersObject = document.object();

    if (!answersObject.contains(courseKey)) {
        answersObject[courseKey] = QJsonObject();
    }

    QJsonObject courseAnswers = answersObject[courseKey].toObject();
    courseAnswers[studentUsername] = answer;
    answersObject[courseKey] = courseAnswers;

    answersFile.resize(0);
    answersFile.write(QJsonDocument(answersObject).toJson());
    answersFile.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Answer submitted successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleGetHomework(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();

    QFile examsFile("homework.json");
    if (!examsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open homework.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = examsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject examsObject = document.object();
    examsFile.close();

    if (!examsObject.contains(courseKey)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Homework question not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject exam = examsObject[courseKey].toObject();
    QJsonObject response;
    response["type"] = "homework_text";
    response["homework_text"] = exam["homework_text"].toString();
    qDebug()<< response["homework_text"];
    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleSubmitHomeWorkAnswer(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();
    QString studentUsername = request["student_username"].toString();
    QString answer = request["answer"].toString();

    QFile answersFile("hw_answers.json");
    if (!answersFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open hw_answers.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = answersFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject answersObject = document.object();

    if (!answersObject.contains(courseKey)) {
        answersObject[courseKey] = QJsonObject();
    }

    QJsonObject courseAnswers = answersObject[courseKey].toObject();
    courseAnswers[studentUsername] = answer;
    answersObject[courseKey] = courseAnswers;

    answersFile.resize(0);
    answersFile.write(QJsonDocument(answersObject).toJson());
    answersFile.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Answer submitted successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleGetContent(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();

    QFile examsFile("content.json");
    if (!examsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open content.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = examsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject examsObject = document.object();
    examsFile.close();

    if (!examsObject.contains(courseKey)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "content not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject exam = examsObject[courseKey].toObject();
    QJsonObject response;
    response["type"] = "file_name";
    response["file_name"] = exam["file_name"].toString();
    qDebug()<< response["file_name"];
    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleGetStudentGrade(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();
    QString studentUsername = request["student_username"].toString();

    QFile participantsFile("participants.json");
    if (!participantsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open participants.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = participantsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject participantsObject = document.object();
    participantsFile.close();

    if (!participantsObject.contains(courseKey)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Course not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject courseObject = participantsObject[courseKey].toObject();
    QJsonObject participants = courseObject["participants"].toObject();

    if (!participants.contains(studentUsername)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Student not enrolled in this course.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    int grade = participants[studentUsername].toInt();

    QJsonObject response;
    response["type"] = "student_grade";
    response["grade"] = grade;

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleRemoveStudentFromCourse(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();
    QString studentUsername = request["student_username"].toString();

    QFile participantsFile("participants.json");
    if (!participantsFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open participants.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = participantsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject participantsObject = document.object();
    participantsFile.close();

    if (!participantsObject.contains(courseKey)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Course not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject courseObject = participantsObject[courseKey].toObject();
    QJsonObject participants = courseObject["participants"].toObject();

    if (!participants.contains(studentUsername)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Student not enrolled in this course.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    participants.remove(studentUsername);
    courseObject["participants"] = participants;
    participantsObject[courseKey] = courseObject;

    participantsFile.open(QIODevice::WriteOnly | QIODevice::Text);
    participantsFile.resize(0);
    participantsFile.write(QJsonDocument(participantsObject).toJson());
    participantsFile.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Student successfully removed from course.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleGetParticipants(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();

    QFile participantsFile("participants.json");
    if (!participantsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open participants.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = participantsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject participantsObject = document.object();
    participantsFile.close();

    if (!participantsObject.contains(courseKey)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Course not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject courseObject = participantsObject[courseKey].toObject();
    QJsonObject participants = courseObject["participants"].toObject();

    QJsonArray participantArray;
    for (const QString &student : participants.keys()) {
        QJsonObject participantInfo;
        participantInfo["username"] = student;
        participantInfo["grade"] = participants[student].toInt();
        participantArray.append(participantInfo);
    }

    QJsonObject response;
    response["type"] = "participants_list";
    response["participants"] = participantArray;

    QJsonDocument responseDoc(response);
    clientSocket->write(responseDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}

void ClientHandler::handleSetStudentGrade(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString courseKey = request["course_key"].toString();
    QString studentUsername = request["student_username"].toString();
    int grade = request["grade"].toInt();

    QFile participantsFile("participants.json");
    if (!participantsFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Failed to open participants.json.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QByteArray fileData = participantsFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(fileData);
    QJsonObject participantsObject = document.object();
    participantsFile.close();

    if (!participantsObject.contains(courseKey)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Course not found.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    QJsonObject courseObject = participantsObject[courseKey].toObject();
    QJsonObject participants = courseObject["participants"].toObject();

    if (!participants.contains(studentUsername)) {
        QJsonObject errorMessage;
        errorMessage["type"] = "error";
        errorMessage["message"] = "Student not enrolled in this course.";
        QJsonDocument errorDoc(errorMessage);
        clientSocket->write(errorDoc.toJson());
        clientSocket->disconnectFromHost();
        return;
    }

    participants[studentUsername] = grade;
    courseObject["participants"] = participants;
    participantsObject[courseKey] = courseObject;

    participantsFile.open(QIODevice::WriteOnly | QIODevice::Text);
    participantsFile.resize(0);
    participantsFile.write(QJsonDocument(participantsObject).toJson());
    participantsFile.close();

    QJsonObject successMessage;
    successMessage["type"] = "success";
    successMessage["message"] = "Grade updated successfully.";
    QJsonDocument successDoc(successMessage);
    clientSocket->write(successDoc.toJson());
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}




