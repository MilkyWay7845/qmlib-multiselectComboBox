#ifndef TAGSFILTEREDMODEL_H
#define TAGSFILTEREDMODEL_H

#include <QtQml>
#include <QObject>
#include <QAbstractProxyModel>
#include <QSortFilterProxyModel>

class TagsFilteredModel : public QSortFilterProxyModel
{
public:
    explicit TagsFilteredModel(QObject *parent = nullptr);
};

#endif // TAGSFILTEREDMODEL_H
