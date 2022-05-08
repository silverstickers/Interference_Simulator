import QtQuick 2.15
import QtQuick.Controls 2.15
import controller 1.0

ApplicationWindow {
    id: root
    width: 750
    height: 575
    visible: true
    Column {
        width: parent.width
        height: parent.height
        visible: true

        Controller {
            id: backend
            onImageRendered: {
                liveImageProvider.updateImage(backend.image)
                liveImage.reload()
                console.log("Image rendered")
            }
        }

        Image {
            id: liveImage
            property bool counter: false

            asynchronous: true
            source: "image://live/image"
            //anchors.fill: parent
            //fillMode: Image.PreserveAspectFit
            width: parent.width
            height: parent.height - 75
            cache: false
            focus: true


            function reload() {
                counter = !counter
                source = "image://live/image?id=" + counter
            }

            Keys.onReleased: {
                switch(event.key) {
                case Qt.Key_Plus:
                    console.log("Plus Key released")
                    backend.zoomIn()
                    event.accepted = true
                    break
                case Qt.Key_Minus:
                    console.log("Minus Key released")
                    backend.zoomOut()
                    event.accepted = true
                    break
                case Qt.Key_Left:
                    console.log("Left Key released")
                    backend.moveLeft()
                    event.accepted = true
                    break
                case Qt.Key_Right:
                    console.log("Right Key released")
                    backend.moveRight()
                    event.accepted = true
                    break
                case Qt.Key_Down:
                    console.log("Down Key released")
                    backend.moveDown()
                    event.accepted = true
                    break
                case Qt.Key_Up:
                    console.log("Up Key released")
                    backend.moveUp()
                    event.accepted = true
                    break
                }
            }
        }

        Component.onCompleted: {
            backend.startCalculation()
        }

        Grid {
            width: parent.width
            height: 100
            rows: 2
            Row {
                Rectangle {
                    width: 25
                    height: parent.height
                    color: "gray"
                    Text {
                        width: parent.width
                        height: parent.height
                        id: textAngleSliderValue
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("0")
                    }
                }
                Slider {
                    id: angleSlider
                    from: -70
                    to: 70
                    value: 0
                    stepSize: 1
                    snapMode: Slider.SnapAlways
                    width: 200
                    height: 50

                    onMoved: {
                        textAngleSliderValue.text = String(value)
                        backend.angleSlider(value)
                    }

                }

                Rectangle {
                    width: 50
                    height: parent.height
                    color: "gray"
                }
                Rectangle {
                    width: 25
                    height: parent.height
                    color: "gray"
                    Text {
                        width: parent.width
                        height: parent.height
                        id: textNumberSliderValue
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("5")
                    }
                }
                Slider {
                    id: numberSlider
                    from: 1
                    to: 20
                    value: 5
                    stepSize: 1
                    snapMode: Slider.SnapAlways
                    width: 200
                    height: 50

                    onMoved: {
                        textNumberSliderValue.text = String(value)
                        backend.numberSlider(value)
                    }
                }

                Rectangle {
                    width: 50
                    height: parent.height
                    color: "gray"
                }
                Rectangle {
                    width: 25
                    height: parent.height
                    color: "gray"
                    Text {
                        width: parent.width
                        height: parent.height
                        id: textSpacingSliderValue
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("5")
                    }
                }
                Slider {
                    id: spacingSlider
                    from: 1
                    to: 20
                    value: 5
                    stepSize: 1
                    snapMode: Slider.SnapAlways
                    width: 200
                    height: 50

                    onMoved: {
                        textSpacingSliderValue.text = String(value)
                        backend.spacingSlider(value)
                    }
                }
            }
            Row {
                Rectangle {
                    width: 25
                    height: parent.height
                    color: "gray"
                }
                Rectangle {
                    width: angleSlider.width
                    height: 25
                    color: "gray"
                    Text {
                        id: textAngleSliderName
                        width: parent.width
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Abstrahlwinkel (deg)")
                    }
                }

                Rectangle {
                    width: 75
                    height: parent.height
                    color: "gray"
                }
                Rectangle {
                    width: angleSlider.width
                    height: 25
                    color: "gray"
                    Text {
                        id: textNumberSliderName
                        width: parent.width
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Anzahl Strahler")
                    }
                }

                Rectangle {
                    width: 75
                    height: parent.height
                    color: "gray"
                }
                Rectangle {
                    width: angleSlider.width
                    height: 25
                    color: "gray"
                    Text {
                        id: textSpacingSliderName
                        width: parent.width
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Abstand Strahler (mm)")
                    }
                }
            }
        }
    }
}
