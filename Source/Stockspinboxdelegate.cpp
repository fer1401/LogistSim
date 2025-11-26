#include "Stockspinboxdelegate.h"

Stockspinboxdelegate::Stockspinboxdelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

// 1. Crea el editor (QSpinBox)
QWidget *Stockspinboxdelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setRange(0, 9999);
    return editor;
}

// 2. Carga el valor actual en el QSpinBox
void Stockspinboxdelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    if (spinBox) {
        spinBox->setValue(value);
    }
}

// 3. Guarda el valor del QSpinBox de vuelta en el modelo (y luego será guardado en el Warehouse)
void Stockspinboxdelegate::setModelData(QWidget *editor,
                                        QAbstractItemModel *model,
                                        const QModelIndex &index) const
{
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    if (spinBox) {
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
}

// 4. Coloca el QSpinBox correctamente
void Stockspinboxdelegate::updateEditorGeometry(QWidget *editor,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
