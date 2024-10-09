#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client/TcpClient.h"
#include "model/PciTableModel.h"

class QTableView;
class QPushButton;

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  TcpClient *client;
  PciTableModel *model;
  QTableView *tableView;
  QPushButton *requestButton;
  QHostAddress serverHost;
  int serverPort;

public:
  explicit MainWindow(const QHostAddress &host, int port, QWidget *parent = nullptr);

private slots:
  void onRequestData();
  void onDataReceived(const QJsonDocument &jsonData);
  void onConnectionError(const QString &errorMessage);

private:
  void setupUI();

};

#endif // MAINWINDOW_H
