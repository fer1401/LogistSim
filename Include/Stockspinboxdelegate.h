#ifndef STOCKSPINBOXDELEGATE_H
#define STOCKSPINBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QSpinBox>

class Stockspinboxdelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Stockspinboxdelegate(QObject *parent = nullptr);

    // Crea el QSpinBox
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    // Carga el valor inicial
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    // Guarda el nuevo valor en el modelo
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    // Geometría del editor
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // STOCKSPINBOXDELEGATE_H
