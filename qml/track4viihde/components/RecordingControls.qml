import QtQuick 1.0
import com.nokia.meego 1.0


Item{
    id: controls
    anchors.fill: parent
    signal refresh()

    Button {
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        id:recordButton
        text: qsTr("Record")
        iconSource: "image://theme/icon-m-camera-ongoing-recording"
        onClicked: {
            track4Engine.recordPogram(programDetailsModel.id())
            controls.refresh()
        }
    }
    Button {
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        id:removeRecButton
        text: qsTr("Remove")
        iconSource:"image://theme/icon-m-toolbar-delete"
        onClicked: {
            track4Engine.removeRecording(programDetailsModel.recordingId())
            controls.refresh()
        }
    }
    Connections {
        target: track4Engine
        onProgramDetailsReady: {
            removeRecButton.visible = programDetailsModel.recorded() ? true : false
            recordButton.visible = programDetailsModel.recorded() ? false : true
        }
    }
    Connections {
        target: track4Engine
        onRecordingDone: track4Engine.programDetails(programDetailsModel.id())
    }
}
