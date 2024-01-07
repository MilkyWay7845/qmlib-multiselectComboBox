#include "tagsmodel.h"

TagsModel::TagsModel(QObject *parent)
    : QAbstractListModel(parent)
{    
    this->_tags.push_back(TagsBuilder().withId("0").withName("C++").withColor(Qt::green).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("1").withName("Java").withColor(Qt::darkBlue).withTextColor(Qt::white).build());
    this->_tags.push_back(TagsBuilder().withId("2").withName("Python").withColor(Qt::darkCyan).withTextColor(Qt::white).build());
    this->_tags.push_back(TagsBuilder().withId("3").withName("Kotlin").withColor(Qt::darkMagenta).withTextColor(Qt::white).build());
    this->_tags.push_back(TagsBuilder().withId("4").withName("JavaScript").withColor(Qt::darkGreen).withTextColor(Qt::white).build());
    this->_tags.push_back(TagsBuilder().withId("5").withName("TypeScript").withColor(Qt::darkYellow).withTextColor(Qt::white).build());
    this->_tags.push_back(TagsBuilder().withId("6").withName("Haskell").withColor(Qt::yellow).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("7").withName("Ruby").withColor(Qt::red).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("8").withName("PHP").withColor(Qt::lightGray).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("9").withName("Go").withColor(Qt::blue).withTextColor(Qt::white).build());
    this->_tags.push_back(TagsBuilder().withId("10").withName("Rust").withColor(Qt::red).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("11").withName("Objective-C").withColor(Qt::cyan).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("12").withName("Swift").withColor(Qt::magenta).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("13").withName("Perl").withColor(Qt::blue).withTextColor(Qt::white).build());
    this->_tags.push_back(TagsBuilder().withId("14").withName("Cobol").withColor(Qt::gray).withTextColor(Qt::black).build());
    this->_tags.push_back(TagsBuilder().withId("15").withName("Fortran").withColor(Qt::green).withTextColor(Qt::black).build());
}

void TagsModel::setFilter(const QString text)
{
    this->_filter = text;
    return;
}

/**
 * @brief TagsModel::rowCount
 * @param parent
 * @return
 */
int TagsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->_tags.count();
}

/**
 * @brief TagsModel::columnCount
 * @param parent
 * @return
 */
int TagsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

/**
 * @brief TagsModel::parent
 * @param child
 * @return
 */
QModelIndex TagsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant retValue;
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
         switch (section) {
             case 0:         { retValue = QString("name"); } break;
             default: { retValue =  QString("Unknown"); }
         }
         return retValue;
     }
     return QVariant();
}

/**
 * @brief TagsModel::data
 * @param index
 * @param role
 * @return
 */
QVariant TagsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Tag& tag = _tags[index.row()];

    switch(role) {
        case Qt::DisplayRole: {
            return tag.name;
        }
        case roles::NAME: {
            return tag.name;
        }
        case roles::ID: {
            return tag.id;
        }
        case roles::COLOR: {
            return tag.color.name();
        }
        case roles::COLOR_HGS: {
            return tag.color.name();
        }
        case roles::SELECTED: {
            return tag.selected;
        }
        case roles::ITEM_COLOR: {
            return tag.itemColor.name();
        }       
        case roles::TEXT_COLOR: {
            return tag.textColor.name();
        }

    }

    return QVariant();
}

bool TagsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    Tag& tag = _tags[index.row()];

    switch(role) {
        case roles::SELECTED: {
            tag.selected = value.toBool();
        } break;
        case roles::ITEM_COLOR: {
            QColor color;
            color.setNamedColor(value.toString());
            tag.itemColor = color;
        } break;
        default: {
            return false;
        }
    }

    _tags.replace(index.row(), tag);
    emit dataChanged(index, index);

    return true;
}

Qt::ItemFlags TagsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    //return Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
    Qt::ItemFlags flags = QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
    return flags;
}

/**
 * @brief TagsModel::roleNames
 * @return
 */
QHash<int, QByteArray> TagsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[ID] = "id";
    roles[NAME] = "name";
    roles[WEIGHT] = "weight";
    roles[COLOR] = "color";
    roles[COLOR_HGS] = "color_hgs";
    roles[SELECTED] = "selected";
    roles[ITEM_COLOR] = "itemColor";
    roles[TEXT_COLOR] = "textColor";
    roles[POS_X] = "pos_x";
    roles[POS_Y] = "pos_y";

    return roles;
}

/**
 * @brief TagsModel::index
 * @param row
 * @param column
 * @param parent
 * @return
 */
QModelIndex TagsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    QModelIndex index = createIndex(row, column, (void*)&_tags[row]);
    return index;
}

/**
 * @brief TagsModel::insertRows
 * @param row
 * @param count
 * @param parent
 * @return
 */
bool TagsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    /* TODO: fix adding tags
        TagBuilder tagBuilder;
        QString name = reinterpret_cast<Tag*>(parent.internalPointer())->name;
        this->_tags.push_back(tagBuilder.build()->setId("14")->setName(name)->setColor(Qt::darkGray)->setTextColor(Qt::black)->create());
        qDebug() << this->_tags.size();
    */
    endInsertRows();
    return true;
}

bool TagsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    while (count--) _tags.removeAt(row);
    endRemoveRows();
    return true;
}

void TagsModel::find(QString substr)
{
    this->setFilter(substr);
}

