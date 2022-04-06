import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4

Item {
    width: 500

    height: 800


    Gauge {
        id: gauge
        x: 8
        y: 0
        width: 100

        height: 706
    }

    Text {
        id: text1
        x: 117
        y: 588
        width: 146
        height: 89
        color: "#f54848"
        text: qsTr("0")
        font.pixelSize: 100
        font.bold: true
    }

    Text {
        id: text2
        x: 117
        y: 544
        width: 408
        height: 61
        color: "#d91515"
        text: qsTr("Fuel consumption")
        font.pixelSize: 40


    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
