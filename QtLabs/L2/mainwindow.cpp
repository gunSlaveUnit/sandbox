//
// Created by gunSlaveUnit on 01.03.2022.
//

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent){
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle(WINDOW_TITLE);

    centralWidget = new QGraphicsView(this);
    centralLayout = new QGridLayout(centralWidget);
    catalog = new Table(centralWidget);
    catalog->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    model = new Model();

    setCentralWidget(centralWidget);
    catalog->setModel(model);
    centralLayout->addWidget(catalog, 0, 0);

    createRecordButton = new QPushButton("Create");
    editRecordButton = new QPushButton("Edit");
    deleteRecordButton = new QPushButton("Delete");

    contentButtonsControlGroup = new QGroupBox("Content Control");
    contentControlButtonsLayout = new QHBoxLayout;
    contentControlButtonsLayout->addWidget(createRecordButton);
    contentControlButtonsLayout->addWidget(editRecordButton);
    contentControlButtonsLayout->addWidget(deleteRecordButton);
    contentControlButtonsLayout->addStretch(1);
    contentButtonsControlGroup->setLayout(contentControlButtonsLayout);
    centralLayout->addWidget(contentButtonsControlGroup, 1, 0);

    int columnsAmount = model->columnCount();
    int columnWidth = WINDOW_WIDTH / columnsAmount;
    for(int i = 0; i < columnsAmount; ++i)
        catalog->setColumnWidth(i, columnWidth);
}

MainWindow::~MainWindow() {
    delete createRecordButton;
    delete editRecordButton;
    delete deleteRecordButton;
    delete contentControlButtonsLayout;
    delete contentButtonsControlGroup;
    delete model;
    delete catalog;
    delete centralLayout;
    delete centralWidget;
}
