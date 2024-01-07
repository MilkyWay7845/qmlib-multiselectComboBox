#ifndef TAGSSELECTEDMODEL_H
#define TAGSSELECTEDMODEL_H

#include <QtQml>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QObject>

class TagsSelectedModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    explicit TagsSelectedModel(QObject *parent = nullptr);

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index = QModelIndex()) const override;

    QItemSelectionModel *selectionModel() const;

    Q_INVOKABLE void setSelectionModel(QItemSelectionModel *newSelectionModel);
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE void removeLast();
    Q_INVOKABLE void addItem(QString name);
    Q_INVOKABLE uint tagCount();
    Q_INVOKABLE void clear();


public slots:
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

public:
    Q_INVOKABLE QStringList getIds() const;
    Q_INVOKABLE QStringList getNames() const;

private:
    QItemSelectionModel* _selectionModel;
    QModelIndexList _selectedIndexes;
};

#endif // TAGSSELECTEDMODEL_H
