//
// Created by gunslaveunit on 31.03.2022.
//

#pragma once

#ifndef GAMEDIR_DELEGATE_H
#define GAMEDIR_DELEGATE_H


#include <QStyledItemDelegate>
#include <QEvent>

#include "dialog.h"

class Delegate : public QStyledItemDelegate {
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;
};


#endif //GAMEDIR_DELEGATE_H
