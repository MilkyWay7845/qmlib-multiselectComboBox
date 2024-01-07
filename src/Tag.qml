import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15


Rectangle {

    property string name: "default"
    property string textColor: "#ffffff"
    property string backgroundColor: "#54945f"
    property string uuid: "0"
    property bool selected: false
    property int offsetText: 40
    property string fontFamilyTag: "ALS Schlange sans"
    property int fontSizeTag: 12
    property bool isBoldFont: false
    property bool isClosed: true

    signal deleted()

    id: rect
    height: 30
    Layout.alignment: Qt.AlignLeft
    border.color: backgroundColor
    color: backgroundColor
    radius: 5
    visible: true


    Text {
        id: tagText
        color: textColor
        anchors.verticalCenter: rect.verticalCenter
        horizontalAlignment: Text.AlignLeft
        leftPadding: 5
        text: name
        font.family: fontFamilyTag
        font.pointSize: fontSizeTag
        font.bold: isBoldFont
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        onContentWidthChanged: {
            (!isClosed) ? rect.implicitWidth = tagText.contentWidth + 10 : rect.implicitWidth = tagText.contentWidth + offsetText
        }
    }


    ToolButton {
        id: crossButton
        implicitHeight: rect.height
        implicitWidth: rect.height
        anchors.verticalCenter: rect.verticalCenter
        anchors.right: rect.right
        icon.source: "icons/svg-cross.svg"
        icon.color: crossMouseArea.containsMouse ? "#ffffff" : "#000000"
        icon.width: 24
        icon.height: 24

        visible: isClosed

        MouseArea {
            id: crossMouseArea
            anchors.fill: crossButton
            hoverEnabled: true
            onClicked: {
                rect.deleted()
            }

        }

        background: Rectangle {
            opacity: enabled ? 1 : 0.3
            color: crossMouseArea.containsMouse ? "#3d393c" : backgroundColor
            radius: 4

        }

    }

}
