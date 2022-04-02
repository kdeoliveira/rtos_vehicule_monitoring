import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: item1
    width: 300

    height: 250


    Image {
        id: image
        x: 44
        y: 91
        width: 185
        height: 151
        source: "../../../../../Documents/QtDesignStudio/ClusterTutorial/content/assets/gearbox_visual_195_150.png"
        fillMode: Image.PreserveAspectFit
    }




    states: [
        State {
            name: "Gear1"

            PropertyChanges {

                target: image
                x: 57
                y: 8
                width: 180
                height: 170
                source: "../../../../../Documents/QtDesignStudio/ClusterTutorial/content/assets/gearbox_visual_1_195_187.png"
            }
        },
        State {
            name: "Gear2"

            PropertyChanges {
                target: image
                source: "../../../../../Documents/QtDesignStudio/ClusterTutorial/content/assets/gearbox_visual_2_195_185.png"
            }
        },
        State {
            name: "Gear3"

            PropertyChanges {
                target: image
                source: "../../../../../Documents/QtDesignStudio/ClusterTutorial/content/assets/gearbox_visual_3_195_181.png"
            }
        },
        State {
            name: "Gear4"

            PropertyChanges {
                target: image
                source: "../../../../../Documents/QtDesignStudio/ClusterTutorial/content/assets/gearbox_visual_4_195_179.png"
            }
        },
        State {
            name: "Gear5"

            PropertyChanges {
                target: image
                source: "../../../../../Documents/QtDesignStudio/ClusterTutorial/content/assets/gearbox_visual_5_195_162.png"
            }
        },
        State {
            name: "Gear6"
        }

    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
