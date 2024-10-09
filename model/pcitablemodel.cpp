#include "model/PciTableModel.h"
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>
#include <iostream>

PciTableModel::PciTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

void PciTableModel::setPciData(const QJsonArray &data) {
  beginResetModel();
  pciData = data;
  sortPciData();
  endResetModel();
}

void PciTableModel::sortPciData() {
  std::vector<QJsonValue> sortedData(pciData.begin(), pciData.end());

  std::sort(sortedData.begin(), sortedData.end(), [](const QJsonValue &a, const QJsonValue &b) {
    const QJsonObject objA = a.toObject();
    const QJsonObject objB = b.toObject();
    int busA = objA["bus"].toInt();
    int busB = objB["bus"].toInt();
    if (busA != busB) {
      return busA < busB;
    }
    int slotA = objA["slot"].toInt();
    int slotB = objB["slot"].toInt();
    if (slotA != slotB) {
      return slotA < slotB;
    }
    int functionA = objA["function"].toInt();
    int functionB = objB["function"].toInt();
    return functionA < functionB;
  });
  QJsonArray sortedJsonArray;
  for (const auto &value : sortedData) {
    sortedJsonArray.append(value);
  }

  pciData = sortedJsonArray;
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
    case 0: return QString::number(device["bus"].toInt()).toUpper();
    case 1: return QString::number(device["slot"].toInt()).toUpper();
    case 2: return QString::number(device["function"].toInt()).toUpper();
    case 3: return QString::number(device["vendorId"].toInt(), 16).toUpper();
    case 4: return (device["deviceId"].toInt() ? QString::number(device["deviceId"].toInt(), 16).toUpper() : "None");
    case 5: return (device["subVendorId"].toInt() ? QString::number(device["subVendorId"].toInt(), 16).toUpper() : "None");
    case 6: return (device["subDeviceId"].toInt() ? QString::number(device["subDeviceId"].toInt(), 16).toUpper() : "None");
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
