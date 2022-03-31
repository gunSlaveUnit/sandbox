//
// Created by ellmac on 31.03.2022.
//

#include "delegate.h"

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return nullptr;
}

bool Delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                           const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonDblClick) {
        auto data = QVarLengthArray<QString, 4>{
            model->data(model->index(index.row(), 0, index)).toString(),
            model->data(model->index(index.row(), 1, index)).toString(),
            model->data(model->index(index.row(), 2, index)).toString(),
            model->data(model->index(index.row(), 3, index)).toString(),
        };

        auto* createDialog = new Dialog(data);
        if (createDialog->exec() == QDialog::Accepted) {
            auto newRow = createDialog->getData();

            for(int c = 0; c < 4; ++c)
                model->setData(model->index(index.row(), c, index), newRow[c]);
        }

        createDialog->deleteLater();

        return true;
    }
    return false;
}
