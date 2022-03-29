//
// Created by gunSlaveUnit on 28.03.2022.
//

#ifndef LAB1_TEXTCOLORCHANGER_MODEL_H
#define LAB1_TEXTCOLORCHANGER_MODEL_H


#include <QAbstractTableModel>

class Model : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    static const uint32_t HEADERS_COUNT = 4, ROWS_COUNT = 3;
    const QVarLengthArray<QString, HEADERS_COUNT> HEADERS{"Title", "Genre", "Developer Company", "Platform"};
    QVector<QVarLengthArray<QString, HEADERS_COUNT>> gridData;
signals:
    void editCompleted(const QString &);
};


#endif //LAB1_TEXTCOLORCHANGER_MODEL_H
