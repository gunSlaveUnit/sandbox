//
// Created by gunslaveunit on 31.03.2022.
//

#include "GameModel.h"

GameModel::GameModel(QObject *parent)
        : QAbstractListModel(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("gamerdir.sqlite3");
    if(!db.open())
        throw std::runtime_error("Can't connect to the database");

    QSqlQuery query;
    if (query.exec("SELECT * FROM games;")) {
        beginInsertRows({}, 0, query.size());
        while (query.next())
            games.append({query.value(0).toString(),
                          query.value(1).toString(),
                          query.value(2).toString(),
                          query.value(3).toString()});
        endInsertRows();
    }
}

int GameModel::rowCount(const QModelIndex & /*parent*/) const
{
    return games.size();
}


QVariant GameModel::data(const QModelIndex &index, int role) const
{
    const Game item = games.at(index.row());
    switch (role) {
        case TITLE_ROLE:
            return QVariant(item.title);
        case GENRE_ROLE:
            return QVariant(item.genre);
        case COMPANY_ROLE:
            return QVariant(item.company);
        case PLATFORM_ROLE:
            return QVariant(item.platform);
    }

    return {};
}

void GameModel::addRow(QList<QString> &row) {
    QSqlQuery query;
    query.prepare("INSERT INTO games (title, genre, company, platform) "
                 "VALUES (:title, :genre, :company, :platform)");
    query.bindValue(":title", row.at(0));
    query.bindValue(":genre", row.at(1));
    query.bindValue(":company", row.at(2));
    query.bindValue(":platform", row.at(3));
    query.exec();

    beginInsertRows(QModelIndex(), games.size(), games.size());
    games.append(Game {
                     row.at(0),
                     row.at(1),
                     row.at(2),
                     row.at(3),
                 });
    endInsertRows();
}

void GameModel::setRowAt(int index, QList<QString> &row)
{
    Game editedItem {row[0], row[1], row[2], row[3]};
    games[index] = editedItem;
    auto modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex);
}

int GameModel::searchByGenre(const QString &genreName)
{
    int amount = 0;

    for(const auto &game : games)
        if (game.genre == genreName)
            ++amount;

    return amount;
}

QList<QString> GameModel::getBy(int index)
{
    auto item = games.at(index);
    QList<QString> data = {item.title,
                          item.genre, item.company, item.platform};
    return data;
}

void GameModel::deleteRow(uint32_t row) {
    auto game = games[row];
    QSqlQuery query;
    query.prepare("DELETE FROM games WHERE title = :title;");
    query.bindValue(":title", game.title);
    query.exec();

    beginRemoveRows(QModelIndex(), row, row);
    games.remove(row);
    endRemoveRows();
}

QHash<int, QByteArray> GameModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TITLE_ROLE] = "gametitle";
    names[GENRE_ROLE] = "gamegenre";
    names[COMPANY_ROLE] = "gamecompany";
    names[PLATFORM_ROLE] = "gameplatform";
    return names;
}
