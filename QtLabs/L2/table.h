//
// Created by gunSlaveUnit on 28.03.2022.
//

#pragma once

#ifndef LAB1_TEXTCOLORCHANGER_TABLE_H
#define LAB1_TEXTCOLORCHANGER_TABLE_H


#include <QTableView>

class Table final : public QTableView {
    Q_OBJECT
public:
    explicit Table(QWidget *parent = nullptr);
    ~Table() final;
};


#endif //LAB1_TEXTCOLORCHANGER_TABLE_H
