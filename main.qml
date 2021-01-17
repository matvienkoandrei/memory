import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.12

Window {
    id: root

    visible: true
    title: "Memory"

    minimumWidth: 800
    minimumHeight: 850

    property double appScale: Math.min(height / minimumHeight, width / minimumWidth)

    property color headerColor: "#303030"
    property color backgroundColor: "#202020"
    property color cardBackColor: "#303030"
    property color textColor: "#f5deb3"
    property string fontFamily: "helvetica"
    property real fontPixelSize: 22 * root.appScale

    Rectangle {
        id: backgroundRect

        anchors.fill: parent

        color: root.backgroundColor
    }

    Rectangle {
        id: header

        z: 1

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 50 * root.appScale

        color: root.headerColor

        Text {
            id: attemptsCountLabel
            anchors.left: parent.left
            anchors.leftMargin: 15 * root.appScale
            anchors.verticalCenter: parent.verticalCenter

            font.family: root.fontFamily
            font.pixelSize: root.fontPixelSize
            color: root.textColor
            text: "Attempts: " + gameplayManager.attemptsCount
        }

        Text {
            id: bestScoreCountLabel
            anchors.right: parent.right
            anchors.rightMargin: 15 * root.appScale
            anchors.verticalCenter: parent.verticalCenter

            font.family: root.fontFamily
            font.pixelSize: root.fontPixelSize
            color: root.textColor
            text: "Best score: " + ((gameplayManager.bestScore === 0) ? "--" : gameplayManager.bestScore)
        }
    }

    DropShadow {
        z: 1
        anchors.fill: header
        source: header
        transparentBorder: true
        cached: true
        radius: 15.0
        samples: radius * 2 + 1
        verticalOffset: 3
        color: "black"
    }

    Item {
        id: cardsFieldArea

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        GameOverPopup {
            id: gameOverPopup
            anchors.centerIn: parent
            width: Math.min(cardsFieldArea.width, cardsFieldArea.height) * 0.4
            height: width

            z: 2
        }

        GridView {
            id: cardsField

            anchors.centerIn: parent

            width: Math.min(cardsFieldArea.width, cardsFieldArea.height)
            height: width

            model: cardsFieldModel

            cellHeight: height / Math.round(Math.sqrt(cardsField.count))
            cellWidth: cellHeight

            interactive: false

            Connections {
                target: gameplayManager
                onChangeFieldEnabled: cardsField.enabled = enabled
            }

            delegate: Item {
                width: cardsField.cellWidth
                height: cardsField.cellHeight

                Flipable {
                    id: delegateFlipable
                    anchors.margins: 1 * root.appScale
                    anchors.fill: parent

                    property bool flipped: model.shown

                    front: Image {
                       width: delegateFlipable.width;
                       height: delegateFlipable.height;
                       source: "qrc:/Cards/" + model.id + ".jpg";
                       anchors.centerIn: parent
                    }
                    back: Rectangle {
                        width: delegateFlipable.width;
                        height: delegateFlipable.height;
                        color: root.cardBackColor
                        anchors.centerIn: parent
                    }

                    transform: Rotation {
                        id: delegateRotation
                        origin.x: delegateFlipable.width / 2
                        origin.y: delegateFlipable.height / 2
                        axis.x: 0; axis.y: 1; axis.z: 0
                        angle: 0
                    }

                    states: State {
                        name: "back"
                        PropertyChanges { target: delegateRotation; angle: 180 }
                        when: !delegateFlipable.flipped
                    }

                    transitions: Transition {
                        NumberAnimation { target: delegateRotation; property: "angle"; duration: 500; easing.type: Easing.OutExpo }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: if (!model.shown) { model.shown = true }
                    }
                }
            }
        }
    }
}
