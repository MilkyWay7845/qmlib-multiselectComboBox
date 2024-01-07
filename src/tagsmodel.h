#ifndef TAGSMODEL_H
#define TAGSMODEL_H

#include <QAbstractItemModel>
#include <QtQml>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "tag.h"


class TagsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TagsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent) override;

public:
    Q_INVOKABLE void setFilter(const QString text);


public:
    enum roles {
        ID = Qt::UserRole,
        NAME,
        WEIGHT,
        COLOR,
        COLOR_HGS,
        SELECTED,
        ITEM_COLOR,
        TEXT_COLOR,
        POS_X,
        POS_Y
    };

    Q_INVOKABLE void find(QString substr);

private:
    QVector<Tag> _tags;
    QString _filter = "";

};

#endif // TAGSMODEL_H
