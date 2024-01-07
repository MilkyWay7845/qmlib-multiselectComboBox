#include "tag.h"
#include <QDebug>

TagsBuilder& TagsBuilder::withId(QString id)
{
    impl.id = id;
    return *this;
}

TagsBuilder &TagsBuilder::withName(QString name)
{
    impl.name = name;
    return *this;
}

TagsBuilder &TagsBuilder::withColor(QColor color)
{
    impl.color = color;
    return *this;
}

TagsBuilder &TagsBuilder::withSelected(bool selected)
{
    impl.selected = selected;
    return *this;
}

TagsBuilder &TagsBuilder::withItemColor(QColor itemColor)
{
    impl.itemColor = itemColor;
    return *this;
}

TagsBuilder &TagsBuilder::withTextColor(QColor textColor)
{
    impl.textColor = textColor;
    return *this;
}

Tag TagsBuilder::build()
{
    return impl;
}
