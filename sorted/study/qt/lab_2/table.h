//
// Created by gunslaveunit on 31.03.2022.
//

#pragma once

#ifndef GAMEDIR_TABLE_H
#define GAMEDIR_TABLE_H


#include <QTableView>

class Table final : public QTableView {
Q_OBJECT
public:
    explicit Table(QWidget *parent = nullptr);
    ~Table() final;
};


#endif //GAMEDIR_TABLE_H
