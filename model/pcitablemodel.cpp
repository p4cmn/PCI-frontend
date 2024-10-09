#include "model\PciTableModel.h"

PciTableModel::PciTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

void PciTableModel::setPciData(const QJsonArray &data) {
  beginResetModel();
  pciData = data;
  endResetModel();
}

int PciTableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return pciData.size();
}

int PciTableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 10;
}

QVariant PciTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole) {
    return QVariant();
  }

  const QJsonObject device = pciData[index.row()].toObject();
  switch (index.column()) {
    case 0: return QString::number(device["bus"].toInt(), 16).toUpper();
    case 1: return QString::number(device["slot"].toInt(), 16).toUpper();
    case 2: return QString::number(device["function"].toInt(), 16).toUpper();
    case 3: return QString::number(device["vendorId"].toInt(), 16).toUpper();
    case 4: return QString::number(device["deviceId"].toInt(), 16).toUpper();
    case 5: return QString::number(device["subVendorId"].toInt(), 16).toUpper();
    case 6: return QString::number(device["subDeviceId"].toInt(), 16).toUpper();
    case 7: return device["vendorName"].toString();
    case 8: return device["deviceName"].toString();
    case 9: return device["subsystemName"].toString();
    default: return QVariant();
  }
}

QVariant PciTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0: return "Bus";
      case 1: return "Slot";
      case 2: return "Function";
      case 3: return "Vendor ID";
      case 4: return "Device ID";
      case 5: return "SubVendor ID";
      case 6: return "SubDevice ID";
      case 7: return "Vendor Name";
      case 8: return "Device Name";
      case 9: return "Subsystem Name";
      default: return QVariant();
    }
  }
  return QVariant();
}
