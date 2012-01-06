import QtQuick 1.0
import com.nokia.meego 1.0
import "Track4Constants.js" as TR4UI

Item{
    objectName: "recordingItem"
    height: 92
    Rectangle {
        id: divider
        width: parent.width
        anchors.left: parent.left
        anchors.right: parent.right
        height: 2
        anchors.margins: TR4UI.DIVIDER_MARGIN
        color: TR4UI.TR4_RED
    }
    Rectangle {
        id: textArea
        width: parent.width;
        height: 90
        anchors.top: divider.bottom
        color: TR4UI.MAIN_BG
        Label {
            id: programName
            anchors.topMargin: TR4UI.TEXT_MARGIN
            anchors.leftMargin: TR4UI.TEXT_MARGIN
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width * 0.7
            elide:Text.ElideRight
            text: "<b>"+name+"</b>"
        }
        Label {
            anchors.topMargin: TR4UI.TEXT_MARGIN
            anchors.rightMargin: TR4UI.TEXT_MARGIN
            anchors.top: parent.top
            anchors.right: parent.right
            text: time
        }
        Label {
            anchors.top: programName.bottom
            anchors.left: parent.left
            font.pixelSize: TR4UI.FONT_LSMALL
            elide:Text.ElideRight
            anchors.leftMargin: TR4UI.TEXT_MARGIN
            maximumLineCount: 2
            text: channel
        }
        Label {
            anchors.top: programName.bottom
            anchors.right: parent.right
            anchors.rightMargin: TR4UI.TEXT_MARGIN
            font.pixelSize: TR4UI.FONT_LSMALL
            text: length
        }
    }
}






