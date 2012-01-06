import QtQuick 1.0
import com.nokia.meego 1.0

Item {
    id: container
    width:parent.width
    signal forward()
    signal back()
    ToolIcon{
        id: backArrow
        anchors.left: parent.left
        iconId: "icon-m-toolbar-previous-white"
        onClicked: container.back()
    }
    ToolIcon{
        id: frontArrow
        anchors.right: parent.right
        iconId: "icon-m-toolbar-next-white"
        onClicked: container.forward()
    }
}
