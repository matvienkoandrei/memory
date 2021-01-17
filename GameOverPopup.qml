import QtQuick 2.12
import QtQuick.Controls 2.12

Flipable {
    id: gameOverFlipable

    property bool flipped: false

    visible: opacity

    front: Rectangle {
        width: gameOverFlipable.width
        height: gameOverFlipable.height
        color: root.cardBackColor

        Text {
            id: gameOverText

            anchors.fill: parent
            wrapMode: TextEdit.Wrap

            leftPadding: 15 * root.appScale
            rightPadding: leftPadding
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            font.family: root.fontFamily
            font.pixelSize: root.fontPixelSize
            lineHeight: 1.3
            color: root.textColor

            enabled: false

            Connections {
                target: gameplayManager
                onGameOver: gameOverText.text = ((attemptsCount < currentBestScore) ?
                                "It's a new record!\nYour score: " + attemptsCount :
                                "The game finished!\nYour score: " + attemptsCount + "\nRecord: " + currentBestScore)
            }
        }

        Button {
            id: restartButton
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 15 * root.appScale
            height: 50 * root.appScale
            width: height

            scale: pressed ? 1.1 : 1.0

            background: Rectangle {
                anchors.fill: parent
                color: "transparent"
            }

            Image {
                id: icon
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/ButtonIcons/check.png"
            }

            onClicked: {
                gameplayManager.restartGame()
                gameOverFlipable.flipped = false
            }
        }
    }

    back: Rectangle { width: gameOverFlipable.width; height: gameOverFlipable.height; color: root.cardBackColor }

    transform: Rotation {
        id: gameOverFlipableRotation
        origin.x: gameOverFlipable.width / 2
        origin.y: gameOverFlipable.height / 2
        axis.x: 0; axis.y: 1; axis.z: 0
        angle: 0
    }

    states: State {
        name: "back"
        PropertyChanges { target: gameOverFlipableRotation; angle: 180 }
        PropertyChanges { target: gameOverFlipable; opacity: 0.0 }
        PropertyChanges { target: gameOverFlipable; scale: 0.3 }
        when: !gameOverFlipable.flipped
    }

    state: "back"

    transitions: Transition {
        ParallelAnimation {
            NumberAnimation { target: gameOverFlipableRotation; property: "angle"; duration: 1500; easing.type: Easing.OutExpo }
            NumberAnimation { target: gameOverFlipable; property: "opacity"; duration: 1500; easing.type: Easing.OutExpo }
            NumberAnimation { target: gameOverFlipable; property: "scale"; duration: 1500; easing.type: Easing.OutExpo }
        }
    }

    Connections {
        target: gameplayManager
        onGameOver: flipped = true
    }
}
