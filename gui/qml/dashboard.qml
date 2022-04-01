

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

    color: "#161616"

    title: "RTOS Vehicule Monitoring"

    ValueSource {
        id: valueSource
    }

    InputData {
        id: inputSource
    }

    // Dashboards are typically in a landscape orientation, so we need to ensure
    // our height is never greater than our width.
    Item {
        id: container
        width: root.width
        height: Math.min(root.width, root.height)
        clip: true
        anchors.centerIn: parent

        Row {
            id: gaugeRow
            spacing: container.width * 0.02
            anchors.centerIn: parent

            TurnIndicator {
                id: leftIndicator
                anchors.verticalCenter: parent.verticalCenter
                width: height
                height: container.height * 0.1 - gaugeRow.spacing

                direction: Qt.LeftArrow
                on: valueSource.turnSignal == Qt.LeftArrow
            }

            Item {
                id: element1
                width: height
                height: 140
                anchors.verticalCenter: parent.verticalCenter

                CircularGauge {
                    id: fuelGauge
                    value: valueSource.fuel
                    maximumValue: 1
                    y: parent.height / 2 - height / 2 - container.height * 0.01
                    width: parent.width
                    height: parent.height * 0.7

                    style: IconGaugeStyle {
                        id: fuelGaugeStyle

                        icon: "qrc:/images/fuel-icon.png"
                        minWarningColor: Qt.rgba(0.5, 0, 0, 1)

                        tickmarkLabel: Text {
                            color: "white"
                            visible: styleData.value === 0
                                     || styleData.value === 1
                            font.pixelSize: fuelGaugeStyle.toPixels(0.225)
                            text: styleData.value === 0 ? "E" : (styleData.value === 1 ? "F" : "")
                        }
                    }
                }

                CircularGauge {
                    id: tempGauge
                    property int maxEngineCoolantTemperature: 225 //In Fahrenheit
                    value: inputSource.engine_coolant / maxEngineCoolantTemperature
                    maximumValue: 1
                    width: parent.width
                    height: parent.height * 0.7
                    y: parent.height / 2 + container.height * 0.01

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

                Text {
                    id: fuel_consumtpion
                    x: 105
                    y: 122
                    color: "#ffffff"
                    text: inputSource.fuel_consumption.toFixed(2)
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: -20
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    font.family: "DejaVu Sans"
                    styleColor: "#f7f7f7"
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 12
                }

                Label {
                    id: label_fuel_consumtpion
                    y: 122
                    color: "#ffffff"
                    text: "Fuel Consumption:"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: -20
                    anchors.left: parent.left
                    anchors.leftMargin: -30
                    font.pointSize: 8
                    font.family: "DejaVu Sans"
                }
            }

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

                style: DashboardGaugeStyle {}
            }

            CircularGauge {
                id: tachometer
                width: height
                height: container.height * 0.25 - gaugeRow.spacing
                value: inputSource.rpm / 1000
                maximumValue: 8
                anchors.verticalCenter: parent.verticalCenter

                style: TachometerStyle {
                    gearValue: inputSource.gear
                }
            }

            TurnIndicator {
                id: rightIndicator
                anchors.verticalCenter: parent.verticalCenter
                width: height
                height: container.height * 0.1 - gaugeRow.spacing

                direction: Qt.RightArrow
                on: valueSource.turnSignal == Qt.RightArrow
            }
        }

        Row {
            id: statusRow
            x: 186
            width: 250
            height: 50

            layoutDirection: Qt.LeftToRight
            spacing: 10
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter

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

            Label {
                id: labelCycles
                height: 12
                color: "#ffffff"
                text: "Cycle:"
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 10
                horizontalAlignment: Text.AlignLeft
            }

            Tumbler {
                id: tumbler
                height: 100
                wrap: false
                font.pixelSize: 12

                enabled: false
                hoverEnabled: false
                wheelEnabled: false
                clip: false
                anchors.verticalCenter: parent.verticalCenter
                model: 8
                currentIndex: inputSource.gear
            }
        }

        Dial {
            id: dial
            x: 832
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
            x: 886

            color: "#dfdfe4"
            text: qsTr("Time Clock")
            anchors.top: parent.top
            anchors.topMargin: 25
            anchors.right: parent.right
            anchors.rightMargin: 100
        }

        Text {
            id: element2
            x: 886
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

/*##^##
Designer {
    D{i:13;anchors_y:122}D{i:14;anchors_x:"-38"}D{i:25;anchors_y:8}D{i:26;anchors_y:25}
D{i:27;anchors_y:48}
}
##^##*/

