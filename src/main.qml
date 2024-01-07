import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import com.synaps.tagsmodel 1.0

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("MultiselectComboBox Demo")

    property real currentTheme: Material.Light
    Material.theme: currentTheme

    /* Models */
    TagsModel {
        id: tagsModel
    }

    MultiselectComboBox {
        id: selectBox
        width: 300
        height: 50
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        rootModel: tagsModel

        onSelectedItemChanged: {
            console.log("item changed!")
        }

        onTextChanged: (text) => {
            console.log(text)
        }
    }

    Row {
        id: rowButtons
        anchors.centerIn: parent
        spacing: 10

        RoundButton {
            id: showInfoButton
            icon.source: "qrc:/icons/svg-eye.svg"

            onClicked: {
                console.log(selectBox.getTagsNameList())
            }
        }

        RoundButton {
            id: themeButton
            icon.source: currentTheme === Material.Light ? "qrc:/icons/svg-moon.svg" : "qrc:/icons/svg-sun.svg"

            onClicked: {
                currentTheme === Material.Light ? currentTheme = Material.Dark : currentTheme = Material.Light
            }
        }
    }
}
