import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: 1920
    height: 1080
    color: "#241f1f"

    FuelGauge {
        id: fuelGauge
        x: 23
        y: 280
    }

    SpeedGauge {
        id: speedGauge
        x: 605
        y: 141
        width: 521
        height: 534
    }

    EngineSpeed {
        id: engineSpeed
        x: 1248
        y: 384
        width: 540
        height: 484
    }

    CoolantTemp {
        id: coolantTemp
        x: 1152
        y: 28
        width: 732
        height: 324
    }

    CurrentGear {
        id: currentGear
        x: 700
        y: 719
    }

    DIALFORPERIOD {
        id: dIALFORPERIOD
        x: 8
        y: 28
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
