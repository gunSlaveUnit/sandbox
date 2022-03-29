//
// Created by gunSlaveUnit on 28.03.2022.
//

#include "model.h"

Model::Model(QObject *parent)
        : QAbstractTableModel(parent) {
    gridData.resize(ROWS_COUNT);
}

int Model::rowCount(const QModelIndex & /*parent*/) const
{
    return ROWS_COUNT;
}

int Model::columnCount(const QModelIndex & /*parent*/) const
{
    return HEADERS_COUNT;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
        return gridData[index.row()][index.column()];

    return {};
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return HEADERS[section];

    return QVariant();
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        gridData[index.row()][index.column()] = value.toString();
        QString result;
        for (auto & row : gridData)
            for (const auto & col : row)
                result += col + ' ';

        emit editCompleted(result);

        return true;
    }

    return false;
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const {
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
