import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4

Item {
    width: 200
    height: 200
    property alias circularGaugeValue: circularGauge.value
    property alias circularGaugeMaximumValue: circularGauge.maximumValue
    property alias gaugeText: text2.text

    CircularGauge {
        id: circularGauge
        anchors.fill: parent

        Text {
            id: text1
            y: 50
            color: "#ffffff"
            text: String(parent.value)
            font.pixelSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: text2
            y: 71
            color: "#ffffff"
            text: "LABEL"
            font.pixelSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.5;height:200;width:200}
}
##^##*/

