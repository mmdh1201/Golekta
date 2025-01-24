#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(qintptr socketDescriptor, QObject *parent = nullptr);
    ~Worker();

signals:
    void finished();

public slots:
    void process();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;

    void handleRequest(const QJsonObject &request);
    void sendResponse(const QJsonObject &response);
};

#endif // WORKER_H
