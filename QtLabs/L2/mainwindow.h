//
// Created by gunSlaveUnit on 01.03.2022.
//

#pragma once

#ifndef LAB1_TEXTCOLORCHANGER_MAINWINDOW_H
#define LAB1_TEXTCOLORCHANGER_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>

#include "table.h"
#include "model.h"


class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() final;
private:
    const int32_t WINDOW_WIDTH = 600, WINDOW_HEIGHT = 400;
    const QString WINDOW_TITLE = "GAMER_DIR";

    QGraphicsView *centralWidget;
    QGridLayout *centralLayout;
    QHBoxLayout *contentControlButtonsLayout;
    QGroupBox *contentButtonsControlGroup;
    QPushButton *createRecordButton;
    QPushButton *editRecordButton;
    QPushButton *deleteRecordButton;

    Table* catalog;
    Model* model;
};


#endif //LAB1_TEXTCOLORCHANGER_MAINWINDOW_H
