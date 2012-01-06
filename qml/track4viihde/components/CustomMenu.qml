import QtQuick 1.0

Rectangle {
    anchors.fill: parent
    id: menuArea
    z: 25
    color: "#80000000"
    opacity: 0
    signal hide()
    MouseArea {
      anchors.fill: parent
      onPressed: menuArea.hide()
    }
}
