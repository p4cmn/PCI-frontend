#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>

class TcpClient : public QObject {
  Q_OBJECT

private:
  QTcpSocket *socket;

public:
  explicit TcpClient(QObject *parent = nullptr);
  void connectToServer(const QHostAddress &host, int port);
  void requestData();

signals:
  void dataReceived(const QJsonDocument &jsonData);
  void connectionError(const QString &errorMessage);

private slots:
  void onReadyRead();
  void onErrorOccurred(QAbstractSocket::SocketError socketError);
};

#endif // TCPCLIENT_H
