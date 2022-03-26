/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 2.15
import QtQuick.Controls 2.15
import Dashboard 1.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.11

Rectangle {
    width: Constants.width
    height: Constants.height
    color: "#181818"


    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        RowLayout {
            id: rowLayout
            width: 100
            height: 100
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            MainGauge {
                id: mainGauge
            }
        }

        RowLayout {
            id: rowLayout1
            width: 100
            height: 100
        }
    }
}
