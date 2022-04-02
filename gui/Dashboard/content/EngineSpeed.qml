import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4

Item {
    width: 521
    height: 534

    CircularGauge {
        id: circularGauge
        x: 72
        y: 117
        width: 400
        height: 300

        Text {
            id: text1
            x: 110
            y: 62
            width: 208
            height: 36
            color: "#d91515"
            text: qsTr("Engine Speed")
            font.pixelSize: 30
        }

        Text {
            id: text2
            x: 154
            y: 90
            width: 102
            height: 38
            color: "#d91515"
            text: qsTr("RPM")
            font.pixelSize: 40
        }

        Text {
            id: text3
            x: 110
            y: 243
            width: 154
            height: 57
            color: "#f54848"
            text: qsTr("0")
            font.pixelSize: 60
            horizontalAlignment: Text.AlignHCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
