import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 500
    height: 200

    Dial {
        id: dial
        x: 35
        y: 15
        width: 177
        height: 170
    }

    Text {
        id: text1
        x: 223
        y: 103
        width: 112
        height: 52
        color: "#f54848"
        text: qsTr("0")
        font.pixelSize: 40
    }

    Text {
        id: text2
        x: 218
        y: 44
        width: 188
        height: 47
        color: "#d91515"
        text: qsTr("Dial Period")
        font.pixelSize: 35
    }
}
