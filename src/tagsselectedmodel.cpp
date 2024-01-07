#include "tagsselectedmodel.h"
#include "tagsmodel.h"
#include <stdexcept>

/**
 * @brief Constructor
 * @param parent
 */
TagsSelectedModel::TagsSelectedModel(QObject *parent)
    : QAbstractProxyModel(parent)
{
}

/**
 * @brief TagsSelectedModel::mapToSource
 * @param proxyIndex
 * @return
 */
QModelIndex TagsSelectedModel::mapToSource(const QModelIndex &proxyIndex) const
{
    for(int i = 0; i < sourceModel()->rowCount(); i++) {
        if(sourceModel()->index(i, 0).internalPointer() == proxyIndex.internalPointer())  {
            return sourceModel()->index(i, 0);
         }
    }
    return QModelIndex();
}

/**
 * @brief TagsSelectedModel::mapFromSource
 * @param sourceIndex
 * @return
 */
QModelIndex TagsSelectedModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    for(int i = 0; i < _selectedIndexes.count(); i++) {
         if(_selectedIndexes.at(i).internalPointer() == sourceIndex.internalPointer() ) {
            return createIndex(i, 0, _selectedIndexes.at(i).internalPointer());
         }
    }
    return QModelIndex();
}

/**
 * @brief TagsSelectedModel::rowCount
 * @param parent
 * @return
 */
int TagsSelectedModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _selectedIndexes.size();
}

/**
 * @brief TagsSelectedModel::columnCount
 * @param parent
 * @return
 */
int TagsSelectedModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

/**
 * @brief TagsSelectedModel::data
 * @param index
 * @param role
 * @return
 */
QVariant TagsSelectedModel::data(const QModelIndex &index, int role) const
{
    QModelIndex sourceIndex = mapToSource(index);
    if(sourceIndex.isValid())
        return sourceModel()->data(mapToSource(index), role);
    Tag* tag = reinterpret_cast<Tag*>(index.internalPointer());
    switch(role) {
        case TagsModel::NAME: {
            return tag->name;
        }
        case TagsModel::COLOR: {
            return QColor(Qt::darkGray);
        }
        case TagsModel::COLOR_HGS: {
            return QColor(Qt::darkGray);
        }
        case TagsModel::SELECTED: {
            return true;
        }
        case TagsModel::ITEM_COLOR: {
            return QColor("#669900");
        }
    }

    return QVariant();
}

/**
 * @brief TagsSelectedModel::index
 * @param row
 * @param column
 * @param parent
 * @return
 */
QModelIndex TagsSelectedModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(column)
    Q_UNUSED(parent)
    return _selectedIndexes.at(row);
}

/**
 * @brief TagsSelectedModel::parent
 * @param index
 * @return
 */
QModelIndex TagsSelectedModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    QModelIndex parent;
    return parent;
}

/**
 * @brief TagsSelectedModel::selectionModel
 * @return
 */
QItemSelectionModel *TagsSelectedModel::selectionModel() const
{
    return _selectionModel;
}

/**
 * @brief TagsSelectedModel::setSelectionModel
 * @param newSelectionModel
 */
void TagsSelectedModel::setSelectionModel(QItemSelectionModel *newSelectionModel)
{
    if(_selectionModel) {
        //disconnect(_selectionModel, &QItemSelectionModel::selectionChanged, this, &TagsSelectedModel::onSelectionChanged);
    }
    _selectionModel = newSelectionModel;
    connect(_selectionModel, &QItemSelectionModel::selectionChanged, this, &TagsSelectedModel::onSelectionChanged);
}

/**
 * @brief TagsSelectedModel::onSelectionChanged
 * @param selected
 * @param deselected
 */
void TagsSelectedModel::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndexList list = selected.indexes();
    for(auto&& srcIndex: list) {
        QModelIndex proxyIndex = mapFromSource(srcIndex);
        if(!proxyIndex.isValid()) {
            beginInsertRows(QModelIndex(), _selectedIndexes.size(), _selectedIndexes.size());
            _selectedIndexes.push_back(createIndex(_selectedIndexes.size(),0, (void*)srcIndex.internalPointer()));
            endInsertRows();
        }
    }
    list = deselected.indexes();
    for(auto&& deseleIt: list) {
        QModelIndex proxyIndex = mapFromSource(deseleIt);
        if(proxyIndex.isValid()) {
            beginRemoveRows(QModelIndex(), proxyIndex.row(),proxyIndex.row());
            _selectedIndexes.removeAt(proxyIndex.row());
            endRemoveRows();
        }
    }
    //emit this->dataChanged(0, _selectedSrcIndexes.size() - 1);
}

/**
 * @brief TagsSelectedModel::getIds
 * @return
 */
QStringList TagsSelectedModel::getIds() const
{
    QStringList list;
    for(auto&& index: _selectedIndexes) {
        if(index.data(TagsModel::roles::ID).toString() != "") {
            list << index.data(TagsModel::roles::ID).toString();
        }
    }
    return list;
}

/**
 * @brief TagsSelectedModel::getNames
 * @return
 */
QStringList TagsSelectedModel::getNames() const
{
    QStringList list;
    for(auto&& index: _selectedIndexes) {
        list << index.data(TagsModel::roles::NAME).toString();
    }
    return list;
}

/**
 * @brief TagsSelectedModel::removeRows
 * @param row
 * @param count
 * @param parent
 * @return
 */
bool TagsSelectedModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool result = false;
    for(int i = 0; i < count; i++) {
        QModelIndex index = this->index(row, 0 + i, parent);
        QModelIndex sourceIndex = mapToSource(index);
        if(sourceIndex.isValid()) {
            this->selectionModel()->select(sourceIndex, QItemSelectionModel::Deselect);
            result = true;
        } else {
            if(row < _selectedIndexes.size()) {
                beginRemoveRows(parent, row, row );
                delete static_cast<Tag*>(_selectedIndexes[row].internalPointer());
                _selectedIndexes.removeAt(row);
                endRemoveRows();
                result = true;
            }
        }
    }

    return result;
}

void TagsSelectedModel::removeLast()
{
    int tagsCount = _selectedIndexes.count();
    if (tagsCount == 0) throw std::invalid_argument("no tags");
    QModelIndex index = this->index(tagsCount - 1, 0, parent());
    QModelIndex sourceIndex = mapToSource(index);
    if(sourceIndex.isValid()) {
        sourceModel()->setData(sourceIndex, "#669900", TagsModel::ITEM_COLOR);
        this->selectionModel()->select(sourceIndex, QItemSelectionModel::Deselect);
    } else {
        if(tagsCount > 0) {
            beginRemoveRows(parent(), tagsCount - 1, tagsCount - 1);
            Tag* willDeletedTag = static_cast<Tag*>(_selectedIndexes.last().internalPointer());
            delete willDeletedTag;
            _selectedIndexes.removeLast();
            endRemoveRows();
        }
    }

}

/**
 * @brief TagsSelectedModel::addItem
 * @param name
 */
void TagsSelectedModel::addItem(QString name)
{
    int count = this->sourceModel()->rowCount();
    QModelIndex index;
    for(int i = 0; i < count; i++) {
        if(this->sourceModel()->index(i, 0).data(Qt::DisplayRole).toString() == name) {
            index = this->sourceModel()->index(i, 0);
            this->selectionModel()->select(index, QItemSelectionModel::Select);
        }
    }

    if(!index.isValid()) {
        Tag* tag = new Tag(TagsBuilder().withId("16").withName(name).withColor(Qt::darkGray).build());
        index = createIndex(_selectedIndexes.size(), 0, tag);
        beginResetModel();
        /* TODO
            this->sourceModel()->insertRows(_selectedIndexes.size(), 1, index);
        */
        _selectedIndexes.push_back(index);
        endResetModel();
    }


}

uint TagsSelectedModel::tagCount()
{
    return _selectedIndexes.size();
}

/**
 * @brief Removes all items from model
 */
void TagsSelectedModel::clear()
{
    int count = this->rowCount();
    this->removeRows(0, count);
}
