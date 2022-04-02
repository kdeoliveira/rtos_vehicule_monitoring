import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Studio.Components 1.0

Item {
    id: item1
    width: 750
    height: 300


    ProgressBar {
        id: progressBar
        x: 107
        y: 180
        width: 593
        height: 128
        value: 0

        Text {
            id: text1
            x: 11
            y: -127
            width: 572
            height: 53
            color: "#d91515"
            text: qsTr("COOLANT TEMPERATURE")
            font.pixelSize: 45
        }
    }

    Text {
        id: text2
        x: 287
        y: 124
        width: 176
        height: 96
        color: "#f54848"
        text: qsTr("0")
        font.pixelSize: 60
        horizontalAlignment: Text.AlignHCenter
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.25}
}
##^##*/
