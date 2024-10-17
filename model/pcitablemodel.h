#ifndef PCITABLEMODEL_H
#define PCITABLEMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>

class PciTableModel : public QAbstractTableModel {
  Q_OBJECT

private:
  QJsonArray pciData;

public:
  explicit PciTableModel(QObject *parent = nullptr);
  void setPciData(const QJsonArray &data);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // PCITABLEMODEL_H
