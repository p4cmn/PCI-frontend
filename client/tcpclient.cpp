#include "client\TcpClient.h"
#include <QDebug>

TcpClient::TcpClient(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)) {
  connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
  connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
          this, &TcpClient::onErrorOccurred);
}

void TcpClient::connectToServer(const QHostAddress &host, int port) {
  socket->connectToHost(host, port);
  if (!socket->waitForConnected(3000)) {
    emit connectionError("Failed to connect to server");
  }
}

void TcpClient::requestData() {
  if (socket->state() == QTcpSocket::ConnectedState) {
    socket->write("GET_PCI_DEVICES");
    socket->flush();
  } else {
    emit connectionError("Not connected to the server");
  }
}

void TcpClient::onReadyRead() {
  QByteArray responseData = socket->readAll();
  QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
  emit dataReceived(jsonResponse);
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError socketError) {
  Q_UNUSED(socketError);
  emit connectionError(socket->errorString());
}
