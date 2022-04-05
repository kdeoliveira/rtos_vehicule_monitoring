import QtQuick 2.10
import QtQuick.Controls 2.10

Item {
    id: item1
    width: 100

    height: 125
    clip: true

    Image {
        id: image
        height: 75
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        fillMode: Image.PreserveAspectFit
        sourceSize.height: 150
        sourceSize.width: 150
        source: "../images/GearNeutral.png"
    }

    Text {
        id: element
        x: 26
        y: 93
        color: "#dfdfe4"
        text: qsTr("N")
        anchors.verticalCenterOffset: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.pixelSize: 20
    }

    states: [
        State {
            name: "Gear1"

            PropertyChanges {

                target: image
                source: "qrc:/images/Gear1.png"
            }

            PropertyChanges {
                target: element
                text: qsTr("1")
            }
        },
        State {
            name: "Gear2"

            PropertyChanges {
                target: image
                source: "qrc:/images/Gear2.png"
            }

            PropertyChanges {
                target: element
                text: qsTr("2")
            }
        },
        State {
            name: "Gear3"

            PropertyChanges {
                target: image
                source: "qrc:/images/Gear3.png"
            }

            PropertyChanges {
                target: element
                text: qsTr("3")
            }
        },
        State {
            name: "Gear4"

            PropertyChanges {
                target: image
                source: "qrc:/images/Gear4.png"
            }

            PropertyChanges {
                target: element
                text: qsTr("4")
            }
        },
        State {
            name: "Gear5"

            PropertyChanges {
                target: image
                source: "qrc:/images/Gear5.png"
            }

            PropertyChanges {
                target: element
                text: qsTr("5")
            }
        },
        State {
            name: "Gear6"
            PropertyChanges {
                target: image
                source: "qrc:/images/Gear6.png"
            }

            PropertyChanges {
                target: element
                text: qsTr("6")
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;height:75;width:75}D{i:1;anchors_height:150;anchors_width:150;anchors_x:44;anchors_y:91}
}
##^##*/

