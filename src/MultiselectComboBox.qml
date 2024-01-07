import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQml.Models 2.15
import com.synaps.tagsmodel 1.0
import com.synaps.tagsselectedmodel 1.0
import com.synaps.tagsfilteredmodel 1.0
import com.synaps.tagsselectionmodel 1.0


ComboBox {

    property QtObject rootModel

    /* Models */
    TagsFilteredModel {
        id: filteredModel
        sourceModel: rootModel
    }

    TagsSelectionModel {
        id: selectionTagsModel
        model: rootModel
    }

    TagsSelectedModel {
        id: selectedModel
        sourceModel: rootModel

        Component.onCompleted: {
            selectedModel.setSelectionModel(selectionTagsModel)
        }

        onRowsInserted: {
            selectedItemChanged() // emit
        }

        onRowsRemoved:  {
            selectedItemChanged() // emit
        }
    }

    /* API attrubute */

    property color borderColor: "#b3a6a6"
    property color borderHoveredColor: "#b3a6a6"
    property int borderHoveredWidth: 2
    property int borderWidth: 1
    property int borderRadius: 5
    property string fontTags: "ALS Schlange sans"
    property int fontSizeTags: 12

    property int marginsTag: 5

    /* API methods */
    function getTagsNameList() {
        return selectedModel.getNames()
    }
    function getTagsIdList() {
        return selectedModel.getIds()
    }
    function setTagsNameList(list) {
        selectedModel.clear()
        list.forEach((tagsName) => {
            selectedModel.addItem(tagsName)
        })
    }

    /* signals */
    signal selectedItemChanged()
    signal textChanged(string text)

    id: multiselectComboBox

    width: 300
    height: 50

    font.family: fontTags
    font.pointSize: fontSizeTags


    background: Rectangle {

        color: currentTheme === Material.Light ? "white" : "#0f2741"
        border.color: multiselectComboBox.pressed ? borderHoveredColor : borderColor
        border.width: multiselectComboBox.hovered ? borderHoveredWidth : borderWidth
        radius: borderRadius

    }

    Flow {
        id: container
        anchors.fill: multiselectComboBox
        anchors.topMargin: marginsTag + 5
        anchors.bottomMargin: marginsTag + 5
        anchors.leftMargin: marginsTag
        anchors.rightMargin: marginsTag + 20
        spacing: 5
        clip: true

        onImplicitHeightChanged: {
            multiselectComboBox.height = container.implicitHeight === 0 ? multiselectComboBox.height : container.implicitHeight + 20
        }

        Repeater {
            id: repeaterTags
            model: selectedModel


            Tag {
                id: tag
                selected: model.selected
                name: model.name
                textColor: model.textColor ? model.textColor : "#ffffff"
                backgroundColor: model.color
                uuid: model.id ? model.id : "0000-0000-0000-0000"

                onDeleted: {
                    model.selected = false;
                    model.itemColor = "#669900"
                    selectedModel.removeRows(index, 1);
                }
            }
        }

        TextInput {
            id: findField
            height: 30
            width: 5 /* width changed on displayTextChanged handler */
            verticalAlignment: TextInput.AlignVCenter
            color: currentTheme === Material.Light ? "black" : "white"
            font.pointSize: 10
            font.family: "Segoe UI Semibold"
            focus: true
            selectByMouse: true


            Keys.onReturnPressed: {
                selectedModel.addItem(findField.text)
                findField.clear()
            }

            Keys.onDeletePressed: {
                if (findField.displayText.length == 0 && selectedModel.tagCount())
                   selectedModel.removeLast()
            }

            Keys.onPressed: (event) => {
                 if (event.key === Qt.Key_Backspace) {
                     if (findField.displayText.length == 0 && selectedModel.tagCount())
                        selectedModel.removeLast()
                 }
            }


            onFocusChanged: {
                forceActiveFocus()
            }

            /* use for android */
            onDisplayTextChanged: {
                let stepWidthTextInput = 15
                let startStepWidthTextInput = 5
                findField.width = findField.displayText.length ? findField.displayText.length * stepWidthTextInput : startStepWidthTextInput
                filteredModel.setFilterFixedString(findField.displayText)

                multiselectComboBox.textChanged(findField.displayText) // emit

                if (!popup.visible) popup.visible = true
            }

        }

    }

    model: filteredModel

    delegate: ItemDelegate {
        id: itemDelegate
        width: multiselectComboBox.width
        height: checkDelegate.height

        function toggle() { checkDelegate.toggle() }

        background: Rectangle {
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: currentTheme === Material.Light ? "#ffffff" : "#000000" }
                GradientStop { position: 0.63; color: model.itemColor }
                GradientStop { position: 0.73; color: model.itemColor }
                GradientStop { position: 1.0; color: model.itemColor }
            }
        }

        CheckDelegate {
            id: checkDelegate
            anchors.fill: parent


            Text {
                id: textTags
                anchors.fill: checkDelegate
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                leftPadding: 5
                text: model.name
                font.family: multiselectComboBox.fontTags
                font.pointSize: multiselectComboBox.fontSizeTags
                color: currentTheme === Material.Light ? "#000000" : "#ffffff"
            }

            indicator: Rectangle {
                visible: false
            }


            highlighted: multiselectComboBox.highlightedIndex === index
            checked: model.selected

            onClicked: {
                model.selected = checked
                selectionTagsModel.select(filteredModel.mapToSource(filteredModel.index(index, 0)), ItemSelectionModel.Toggle );
                if (selectionTagsModel.isSelected(filteredModel.mapToSource(filteredModel.index(index, 0)))) {
                    model.itemColor = "#b30000"
                } else {
                    model.itemColor = "#669900"
                }
                findField.clear()
            }

            onDoubleClicked: {
                model.selected = !checked
                findField.clear()
            }

        }
    }

    // override space key handling to toggle items when the popup is visible
    Keys.onSpacePressed: {
        if (multiselectComboBox.popup.visible) {
            var currentItem = multiselectComboBox.popup.contentItem.currentItem
            if (currentItem) {
                currentItem.toggle()
                event.accepted = true
            }
        }
    }

    Keys.onReleased: {
        if (multiselectComboBox.popup.visible)
            event.accepted = (event.key === Qt.Key_Space)
    }


    /* arrow indicator by combobox */
    indicator: Image {
        id: indicator
        x: multiselectComboBox.width - width - multiselectComboBox.rightPadding
        y: multiselectComboBox.topPadding + (multiselectComboBox.availableHeight - height) / 2
        width: 12
        height: 12
        source: currentTheme === Material.Light ? "icons/arrow-down.png" : "icons/arrow-down-white.png"

        RotationAnimation {
            target: indicator
             id: rotateCritter
             duration: 200
             property real lastAngle: 0
             onStarted: {
                 lastAngle = to;
             }
             onStopped: {
                 from = lastAngle;
             }
         }
    }

    popup: Popup {

        property int maxVisibleItem: 4

        id: popup
        y: multiselectComboBox.height - 1
        width: multiselectComboBox.width
        height: (list.count > maxVisibleItem) ? 200 : list.count * 50
        padding: 1
        property bool isOpen: false


        contentItem: ListView {

            id: list
            clip: true
            implicitHeight: contentHeight
            model: multiselectComboBox.popup.visible ? multiselectComboBox.delegateModel : null
            currentIndex: multiselectComboBox.highlightedIndex
            ScrollBar.vertical: ScrollBar {
                id: scrollBarTags
                anchors.right: parent.right
                active: true

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 100
                    radius: width / 2
                    color: scrollBarTags.pressed ? "#81e889" : "#c2f4c6"
                }

            }

        }

        onVisibleChanged: {
            rotateCritter.to = (!isOpen) ? 180 : 0
            isOpen = !isOpen
            rotateCritter.start()
        }


        background: Rectangle {
            border.color: "#b3a6a6"
            radius: 3
        }
    }

}

