//
// Created by gunslaveunit on 31.03.2022.
//

#pragma once

#ifndef GAMEDIR_MODEL_H
#define GAMEDIR_MODEL_H


#include <QAbstractTableModel>

class Model : public QAbstractTableModel {
Q_OBJECT
    static const uint32_t HEADERS_COUNT = 4;
    uint32_t ROWS_COUNT = 0;
public:
    explicit Model(QObject *parent = nullptr);
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
    void addRow(QVarLengthArray<QString, HEADERS_COUNT>& row);
    void deleteRow(uint32_t row);
private:
    const QVarLengthArray<QString, HEADERS_COUNT> HEADERS{"Title", "Genre", "Developer Company", "Platform"};
    QVector<QVarLengthArray<QString, HEADERS_COUNT>> gridData;
signals:
    void editCompleted(const QString &);
};


#endif //GAMEDIR_MODEL_H
