//
// Created by gunslaveunit on 31.03.2022.
//

#pragma once

#ifndef GAMEDIR_MAINWINDOW_H
#define GAMEDIR_MAINWINDOW_H


#include <QMainWindow>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>

#include "table.h"
#include "model.h"
#include "dialog.h"
#include "delegate.h"


class MainWindow final : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() final;
public slots:
    void addRow();
    void deleteSelectedRow();
private:
    const int32_t WINDOW_WIDTH = 600, WINDOW_HEIGHT = 400;
    const QString WINDOW_TITLE = "GAMER_DIR";

    QGraphicsView *centralWidget;
    QGridLayout *centralLayout;
    QHBoxLayout *contentControlButtonsLayout;
    QGroupBox *contentButtonsControlGroup;
    QPushButton *createRecordButton;
    QPushButton *deleteRecordButton;

    Table* catalog;
    Model* model;
    Delegate* editDelegate;
};


#endif //GAMEDIR_MAINWINDOW_H
