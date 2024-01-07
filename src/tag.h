#ifndef TAG_H
#define TAG_H

#include <QString>
#include <QColor>

/**
 * @brief The Tag struct
 * Tag contains information about the selected item in the comboBox.
 * You can add your fields as needed.
 */
struct Tag {
    QString id = "0000-0000-0000-0000";
    QString name = "not-name";
    QColor color = Qt::green;
    bool selected = false;
    QColor itemColor = QColor("#669900");
    QColor textColor = Qt::white;
};

class TagsBuilder {
public:
    TagsBuilder() = default;

    TagsBuilder& withId(QString id);
    TagsBuilder& withName(QString name);
    TagsBuilder& withColor(QColor color);
    TagsBuilder& withSelected(bool selected);
    TagsBuilder& withItemColor(QColor itemColor);
    TagsBuilder& withTextColor(QColor textColor);

    Tag build();
private:
    Tag impl{};
};

#endif // TAG_H
