//
// Created by gunslaveunit on 31.03.2022.
//

#include "dialog.h"

Dialog::Dialog(const QVarLengthArray<QString, HEADERS_COUNT> &record) {
    gridLayout = new QGridLayout(this);

    if(!record.isEmpty()) {
        this->setWindowTitle("Update the record");

        for(int i = 0; i < HEADERS_COUNT; ++i)
            data[i] = new QLineEdit(record[i]);
    } else {
        this->setWindowTitle("Create new record");

        for(int i = 0; i < HEADERS_COUNT; ++i)
            data[i] = new QLineEdit("");
    }

    for(int i = 0; i < HEADERS_COUNT; ++i)
        fieldTitles[i] = new QLabel(HEADERS[i]);

    for(int i = 0; i < HEADERS_COUNT; ++i) {
        gridLayout->addWidget(fieldTitles[i], i, 0, Qt::AlignCenter);
        gridLayout->addWidget(data[i], i, 1, Qt::AlignCenter);
    }

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    gridLayout->addWidget(okButton,4, 0, Qt::AlignCenter);
    gridLayout->addWidget(cancelButton,4, 1, Qt::AlignCenter);
}

QVarLengthArray<QString, 4> Dialog::getData() {
    QVarLengthArray<QString, 4> rec;

    rec.push_back(data[0]->text());
    rec.push_back(data[1]->text());
    rec.push_back(data[2]->text());
    rec.push_back(data[3]->text());

    return rec;
}

Dialog::~Dialog() {
    for(int i = 0; i < HEADERS_COUNT; ++i)
        delete data[i];
    for(int i = 0; i < HEADERS_COUNT; ++i)
        delete fieldTitles[i];
    delete okButton;
    delete cancelButton;
    delete gridLayout;
}
