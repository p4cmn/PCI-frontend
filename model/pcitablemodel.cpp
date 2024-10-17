#include "model/PciTableModel.h"
#include <QJsonObject>
#include <QJsonArray>

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
  return 16;
}

QVariant PciTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole) {
    return QVariant();
  }
  const QJsonObject device = pciData[index.row()].toObject();
  switch (index.column()) {
    case 0: return QString::number(device["bus"].toInt()).toUpper();
    case 1: return QString::number(device["slot"].toInt()).toUpper();
    case 2: return QString::number(device["function"].toInt()).toUpper();
    case 3: return QString::number(device["vendorId"].toInt(), 16).toUpper();
    case 5: return ( (static_cast<unsigned int>(device["deviceId"].toInt()) != 0xFFFFFFFF && static_cast<unsigned int>(device["deviceId"].toInt()) != 0 ) ? QString::number(device["deviceId"].toInt(), 16).toUpper() : "None");
    case 7: return ( (static_cast<unsigned int>(device["subVendorId"].toInt()) != 0xFFFFFFFF && static_cast<unsigned int>(device["subVendorId"].toInt()) != 0) ? QString::number(device["subVendorId"].toInt(), 16).toUpper() : "None");
    case 8: return ( (static_cast<unsigned int>(device["subDeviceId"].toInt()) != 0xFFFFFFFF && static_cast<unsigned int>(device["subDeviceId"].toInt()) != 0) ? QString::number(device["subDeviceId"].toInt(), 16).toUpper() : "None");
    case 10: return (static_cast<unsigned int>(device["classCode"].toInt()) != 0xFFFFFFFF ? QString::number(device["classCode"].toInt(), 16).toUpper() : "None");
    case 12: return (static_cast<unsigned int>(device["subClassCode"].toInt()) != 0xFFFFFFFF ? QString::number(device["subClassCode"].toInt(), 16).toUpper() : "None");
    case 14: return (static_cast<unsigned int>(device["progIf"].toInt()) != 0xFFFFFFFF ? QString::number(device["progIf"].toInt(), 16).toUpper() : "None");
    case 4: return device["vendorName"].toString();
    case 6: return device["deviceName"].toString();
    case 9: return device["subsystemName"].toString();
    case 11: return device["className"].toString();
    case 13: return device["subClassName"].toString();
    case 15: return device["progIfName"].toString();
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
      case 5: return "Device ID";
      case 7: return "SubVendor ID";
      case 8: return "SubDevice ID";
      case 10: return "Class ID";
      case 12: return "SubClass ID";
      case 14: return "ProgIf";
      case 4: return "Vendor Name";
      case 6: return "Device Name";
      case 9: return "Subsystem Name";
      case 11: return "Class Name";
      case 13: return "SubClass Name";
      case 15: return "ProgIf Name";
      default: return QVariant();
    }
  }
  return QVariant();
}
