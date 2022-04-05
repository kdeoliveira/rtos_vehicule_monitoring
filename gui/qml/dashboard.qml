

/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Extras 1.4
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import qnx.rtos 1.0
import Qt.labs.calendar 1.0
import QtGraphicalEffects 1.0

Window {
    id: root
    visible: true
    width: 1024
    height: 600
    maximumWidth: root.width
    maximumHeight: root.height
    minimumHeight: root.height
    minimumWidth: root.width

    color: "#161616"

    title: "RTOS Vehicule Monitoring"

    ValueSource {
        id: valueSource
    }

    InputData {
        id: inputSource
    }
    Image {
        id: image
        anchors.fill: parent
        opacity: 0.5
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/images/background.png"
    }

    // Dashboards are typically in a landscape orientation, so we need to ensure
    // our height is never greater than our width.
    Item {
        id: container
        width: root.width
        height: Math.min(root.width, root.height)
        clip: true
        anchors.centerIn: parent

        Item {
            id: gaugeRow
            width: 1000
            height: 400
            anchors.verticalCenterOffset: -75
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            CircularGauge {
                id: speedometer
                value: inputSource.speed
                /*valueSource.kph*/
                anchors.verticalCenter: parent.verticalCenter
                maximumValue: 280
                // We set the width to the height, because the height will always be
                // the more limited factor. Also, all circular controls letterbox
                // their contents to ensure that they remain circular. However, we
                // don't want to extra space on the left and right of our gauges,
                // because they're laid out horizontally, and that would create
                // large horizontal gaps between gauges on wide screens.
                width: height
                height: container.height * 0.5
                anchors.horizontalCenter: parent.horizontalCenter

                style: DashboardGaugeStyle {}
            }

            CircularGauge {
                id: tachometer
                width: 140
                height: 140
                anchors.verticalCenterOffset: 0
                anchors.right: parent.right
                anchors.rightMargin: 100
                value: inputSource.rpm / 1000
                maximumValue: 8
                anchors.verticalCenter: parent.verticalCenter

                style: TachometerStyle {
                    tachValue: (inputSource.rpm).toFixed(0)
                }
            }
            Item {
                id: element1
                width: 140
                height: 200
                anchors.verticalCenterOffset: 50
                anchors.left: parent.left
                anchors.leftMargin: 100
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: element3
                    x: 12
                    y: 87
                    color: "#dfdfe4"
                    text: qsTr("Coolant Temperature")
                    anchors.verticalCenterOffset: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                CircularGauge {
                    id: tempGauge
                    property int maxEngineCoolantTemperature: 225 //In Fahrenheit
                    value: inputSource.engine_coolant / maxEngineCoolantTemperature
                    maximumValue: 1
                    width: parent.width
                    height: parent.height * 0.7
                    anchors.verticalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    style: IconGaugeStyle {
                        id: tempGaugeStyle

                        icon: "qrc:/images/temperature-icon.png"
                        maxWarningColor: Qt.rgba(0.5, 0, 0, 1)

                        tickmarkLabel: Text {
                            color: "white"
                            visible: styleData.value === 0
                                     || styleData.value === 1
                            font.pixelSize: tempGaugeStyle.toPixels(0.225)
                            text: styleData.value === 0 ? "C" : (styleData.value === 1 ? "H" : "")
                        }
                    }
                }
            }

            CurrentGear {
                function setState(arg) {
                    switch (arg) {
                    case 0:
                        return "base state"
                    case 1:
                        return "Gear2"
                    case 2:
                        return "Gear3"
                    case 3:
                        return "Gear4"
                    case 4:
                        return "Gear5"
                    case 5:
                        return "Gear6"
                    case 14:
                        return "base state"
                    default:
                        return "base state"
                    }
                }

                id: currentGear
                x: 793
                y: 276
                width: 75
                height: 125
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 133
                state: setState(inputSource.gear)
            }
        }

        Item {
            id: fuel_consumption_items
            x: 362
            y: 404
            width: 300
            height: 150
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 25

            Gauge {
                id: gauge
                x: 119
                y: -82
                height: 238
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                rotation: 90
                maximumValue: 100
                value: inputSource.fuel_consumption / 100
            }

            Text {
                id: fuel_consumtpion
                x: 16
                y: 91
                color: "#ffffff"
                text: inputSource.fuel_consumption.toFixed(2)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                font.family: "DejaVu Sans"
                styleColor: "#f7f7f7"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 12
            }

            Label {
                id: label_fuel_consumtpion
                x: 83
                y: 72
                color: "#ffffff"
                text: "Fuel Consumption x100:"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 25
                font.pointSize: 8
                font.family: "DejaVu Sans"
            }
        }

        Row {
            id: statusRow
            width: 100
            height: 50
            anchors.left: parent.left
            anchors.leftMargin: 100

            layoutDirection: Qt.LeftToRight
            spacing: 10
            anchors.top: parent.top
            anchors.topMargin: 20

            Label {
                id: element
                color: "#ffffff"
                text: qsTr("Sensors Status:")
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 10
            }
            StatusIndicator {
                id: statusIndicator
                width: 20
                height: 20
                color: "#056136"
                anchors.verticalCenter: parent.verticalCenter
                active: inputSource.bufferStatus
            }
        }

        Item {
            id: element4
            width: 200
            height: 125
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 100

            Dial {
                id: dial
                x: 120
                y: 0
                width: 80
                height: 80
                stepSize: 1.0
                from: 1
                to: 1000
                value: inputSource.dial
                onValueChanged: inputSource.dial = dial.value
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 8
            }

            Label {
                id: label
                x: 34
                y: 17

                color: "#dfdfe4"
                text: qsTr("Dial Period")
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.right: parent.right
                anchors.rightMargin: 100
            }

            Text {
                id: element2
                x: 34
                y: 40
                width: 74
                height: 15
                color: "#e6e1e1"
                text: String(dial.value.toFixed(0)) + " ms"
                horizontalAlignment: Text.AlignLeft
                anchors.right: parent.right
                anchors.rightMargin: 100
                anchors.top: parent.top
                anchors.topMargin: 48
                font.pixelSize: 12
            }
        }
    }
}

/*##^##
Designer {
    D{i:3;anchors_height:100;anchors_width:100;anchors_x:0;anchors_y:0}D{i:7;anchors_y:122}
D{i:17;anchors_x:12}D{i:18;anchors_x:186}D{i:19;anchors_x:186}D{i:16;anchors_x:12}
D{i:20;anchors_x:186}
}
##^##*/

