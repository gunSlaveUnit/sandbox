//
// Created by gunslaveunit on 31.03.2022.
//

#pragma once

#ifndef GAMEDIR_MODEL_H
#define GAMEDIR_MODEL_H


#include <QAbstractListModel>

struct Game {
    QString title,
            genre,
            company,
            platform;
};

class GameModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void addRow(QList<QString> &row);
    Q_INVOKABLE void setRowAt(int index, QList<QString> &row);
    Q_INVOKABLE int searchByGenre(const QString &genreName);
    Q_INVOKABLE QList<QString> getBy(int index);
    Q_INVOKABLE void deleteRow(uint32_t row);

    enum ResentRoles {
        TITLE_ROLE = Qt::DisplayRole,
        GENRE_ROLE,
        COMPANY_ROLE,
        PLATFORM_ROLE
    };

    QHash<int, QByteArray> roleNames() const override;
signals:
    void editCompleted();
private:
    QList<Game> games;
};


#endif //GAMEDIR_MODEL_H
