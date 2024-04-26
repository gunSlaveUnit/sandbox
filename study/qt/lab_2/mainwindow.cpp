//
// Created by gunslaveunit on 31.03.2022.
//

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent){
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle(WINDOW_TITLE);

    centralWidget = new QGraphicsView(this);
    centralLayout = new QGridLayout(centralWidget);
    catalog = new Table(centralWidget);
    model = new Model();

    editDelegate = new Delegate();

    setCentralWidget(centralWidget);
    catalog->setModel(model);
    catalog->setEditTriggers(QAbstractItemView::NoEditTriggers);
    catalog->setItemDelegate(editDelegate);
    centralLayout->addWidget(catalog, 0, 0);

    createRecordButton = new QPushButton("Create");
    deleteRecordButton = new QPushButton("Delete");

    contentButtonsControlGroup = new QGroupBox("Content Control");
    contentControlButtonsLayout = new QHBoxLayout;
    contentControlButtonsLayout->addWidget(createRecordButton);
    contentControlButtonsLayout->addWidget(deleteRecordButton);
    contentControlButtonsLayout->addStretch(1);
    contentButtonsControlGroup->setLayout(contentControlButtonsLayout);
    centralLayout->addWidget(contentButtonsControlGroup, 1, 0);

    int columnsAmount = model->columnCount();
    int columnWidth = WINDOW_WIDTH / columnsAmount;
    for(int i = 0; i < columnsAmount; ++i)
        catalog->setColumnWidth(i, columnWidth - 6);

    connect(createRecordButton, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(deleteRecordButton, SIGNAL(clicked()), this, SLOT(deleteSelectedRow()));
}

MainWindow::~MainWindow() {
    delete editDelegate;
    delete createRecordButton;
    delete deleteRecordButton;
    delete contentControlButtonsLayout;
    delete contentButtonsControlGroup;
    delete model;
    delete catalog;
    delete centralLayout;
    delete centralWidget;
}

void MainWindow::addRow() {
    auto* createDialog = new Dialog(QVarLengthArray<QString, 4>());
    if (createDialog->exec() == QDialog::Accepted) {
        auto newRow = createDialog->getData();
        model->addRow(newRow);
    }

    createDialog->deleteLater();
}

void MainWindow::deleteSelectedRow() {
    QModelIndex selectedModelIndex = catalog->currentIndex();
    model->deleteRow(selectedModelIndex.row());
}
