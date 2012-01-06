import QtQuick 1.0

SequentialAnimation {
    running: false
    property Item toHide
    property Item toShow
    NumberAnimation { target: menuenabler.toHide; property: "opacity"; to: 0; duration: 200}
    NumberAnimation { target: menuenabler.toShow; property: "opacity"; to: 1.0; duration: 200}
}
