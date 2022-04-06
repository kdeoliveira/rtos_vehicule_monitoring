import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4

Item {
    width: 521
    height: 534
    property alias circularGaugeValue: circularGauge.value
    property alias circularGaugeMaximumValue: circularGauge.maximumValue
    property alias gaugeText: text2.text

    CircularGauge {
        id: circularGauge
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Text {
            id: text1
            y: 394
            width: 187
            height: 106
            color: "#f54848"
            text: "0"
            font.pixelSize: 80
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenterOffset: -6
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: text2
            y: 160
            color: "#d91515"
            text: "SPEED"
            font.pixelSize: 70
            anchors.horizontalCenterOffset: 3

            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66;height:200;width:200}
}
##^##*/

