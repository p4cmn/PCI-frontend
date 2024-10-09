#include "MainWindow.h"
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(const QHostAddress &host, int port, QWidget *parent)
    : QMainWindow(parent), client(new TcpClient(this)), model(new PciTableModel(this)),
    serverHost(host), serverPort(port) {
  setupUI();

  connect(client, &TcpClient::dataReceived, this, &MainWindow::onDataReceived);
  connect(client, &TcpClient::connectionError, this, &MainWindow::onConnectionError);

  // Применяем стили
  QString styleSheet = R"(
        QTableView {
            background-color: #2B2B2B;
            color: #E0E0E0;
            gridline-color: #3E3E3E;
            selection-background-color: #4A90E2;
            selection-color: #FFFFFF;
            font-size: 14px;
        }
        QHeaderView::section {
            background-color: #3C3F41;
            color: #DADADA;
            padding: 4px;
            border: 1px solid #6D6D6D;
            font-size: 14px;
        }
        QPushButton {
            background-color: #4A90E2;
            color: #FFFFFF;
            border-radius: 5px;
            padding: 6px 12px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #357ABD;
        }
        QPushButton:pressed {
            background-color: #2A6496;
        }
    )";
  setStyleSheet(styleSheet);
}


void MainWindow::setupUI() {
  tableView = new QTableView(this);
  tableView->setModel(model);
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setAlternatingRowColors(true);
  tableView->horizontalHeader()->setStretchLastSection(true); // Растягиваем последний столбец
  tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягиваем все столбцы

  requestButton = new QPushButton("Request PCI Data", this);
  connect(requestButton, &QPushButton::clicked, this, &MainWindow::onRequestData);

  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  layout->addWidget(tableView);
  layout->addWidget(requestButton);
  layout->setContentsMargins(0, 0, 0, 0); // Убираем отступы
  layout->setSpacing(0); // Убираем расстояние между виджетами

  setCentralWidget(centralWidget);

  setWindowTitle("PCI Device Viewer");
  resize(800, 600);
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
