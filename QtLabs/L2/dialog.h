//
// Created by gunslaveunit on 31.03.2022.
//

#pragma once

#ifndef GAMEDIR_DIALOG_H
#define GAMEDIR_DIALOG_H


#include <QVarLengthArray>
#include <QString>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

class Dialog final : public QDialog {
    static const uint32_t HEADERS_COUNT = 4;
public:
    explicit Dialog(const QVarLengthArray<QString, HEADERS_COUNT>& record);
    QVarLengthArray<QString, HEADERS_COUNT> getData();
    ~Dialog() override;
private:
    QGridLayout* gridLayout;
    QPushButton* okButton;
    QPushButton* cancelButton;

    const QVarLengthArray<QString, HEADERS_COUNT> HEADERS{"Title", "Genre", "Developer Company", "Platform"};
    QVarLengthArray<QLabel*, HEADERS_COUNT> fieldTitles;
    QVarLengthArray<QLineEdit*, HEADERS_COUNT> data;
};


#endif //GAMEDIR_DIALOG_H
