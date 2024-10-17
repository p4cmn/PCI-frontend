#include "view/MainWindow.h"
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QJsonObject>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(const QHostAddress &host, int port, QWidget *parent)
    : QMainWindow(parent), client(new TcpClient(this)), model(new PciTableModel(this)),
    serverHost(host), serverPort(port) {
  setupUI();

  connect(client, &TcpClient::dataReceived, this, &MainWindow::onDataReceived);
  connect(client, &TcpClient::connectionError, this, &MainWindow::onConnectionError);
}

void MainWindow::setupUI() {
  tableView = new QTableView(this);
  tableView->setModel(model);
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setAlternatingRowColors(true);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tableView->setSortingEnabled(true);

  requestButton = new QPushButton("Request PCI Data", this);
  connect(requestButton, &QPushButton::clicked, this, &MainWindow::onRequestData);
  connect(tableView, &QTableView::clicked, this, &MainWindow::onCellClicked);  // Добавляем обработчик кликов

  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  layout->addWidget(tableView);
  layout->addWidget(requestButton);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  setCentralWidget(centralWidget);

  setWindowTitle("PCI Device Viewer");
  resize(1000, 800);
}

void MainWindow::onRequestData() {
  client->connectToServer(serverHost, serverPort);
  client->requestData();
}

void MainWindow::onDataReceived(const QJsonDocument &jsonData) {
  QJsonArray devicesArray = jsonData.object().value("devices").toArray();
  model->setPciData(devicesArray);
}

void MainWindow::onConnectionError(const QString &errorMessage) {
  QMessageBox::critical(this, "Connection Error", errorMessage);
}

void MainWindow::onCellClicked(const QModelIndex &index) {
  if (index.isValid()) {
    QString cellData = model->data(index, Qt::DisplayRole).toString();
    QMessageBox::information(this, "Cell Content", cellData);
  }
}
