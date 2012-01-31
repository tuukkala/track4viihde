import QtQuick 1.0
import com.nokia.meego 1.0

import "components"
import "Track4Constants.js" as TR4UI

Track4Page{
    id: programDetails
    anchors.fill: parent
    property alias controls: controlBar.children

    Track4TitleBar{
        id: titleBar
    }

    BusyIndicator {
        id: programBusyIndicator
        anchors.centerIn: parent
        running: true
        visible: true
        platformStyle: BusyIndicatorStyle { size: "large" }
    }

    Item{
        id: detailsColumn

        anchors{
            left: parent.left
            right: parent.right
            top: titleBar.bottom
            bottom: parent.bottom
        }

        Label{
            id: name
            elide:Text.ElideRight
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 20
            font.pixelSize: TR4UI.FONT_SLARGE
        }
        Label{
            anchors{
                left: parent.left
                top: name.bottom
                right: parent.right
                margins: 15
            }
            width: parent.width
            id: description
            font.pixelSize: TR4UI.FONT_DEFAULT
        }
        Grid{
            anchors{
                margins: 15
                left: parent.left
                right: parent.right
                top: description.bottom
            }
            id: timeDetails
            columns: 2
            spacing: 5
            Label{
                font.pixelSize: TR4UI.FONT_DEFAULT
                text: "<b>"+qsTr("Time")+"</b>"
            }
            Label{
                id:starTime
                font.pixelSize: TR4UI.FONT_DEFAULT
            }
            Label{
                font.pixelSize: TR4UI.FONT_DEFAULT
                text: "<b>"+qsTr("Length")+"</b>"
            }
            Label{
                id: length
                font.pixelSize: TR4UI.FONT_DEFAULT
            }
        }
        Item{
            width: parent.width
            anchors{
                topMargin: 30
                top: timeDetails.bottom
            }
            id: controlBar
            height: childrenRect.height
        }
        Label{
            anchors.bottom: backIcon.top
            id:errorMessage
            visible:false
        }
        ToolIcon{
            id:backIcon
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 10
            iconSource: "image://theme/icon-m-toolbar-back"
            onClicked: pageStack.pop()
        }
    }
    function reload(){        
        programBusyIndicator.visible = true
        programBusyIndicator.running = true
        errorMessage.visible = false
        controlBar.visible = false
        timeDetails.visible = false
    }
    function setDetails(){
        controlBar.visible = true
        programBusyIndicator.visible = false
        programBusyIndicator.running = false
        name.text = "<b>"+programDetailsModel.name()+"</b>";
        description.text = programDetailsModel.description()
        starTime.text = programDetailsModel.startTime()
        length.text = programDetailsModel.length()
        errorMessage.visible= false
        timeDetails.visible = true

    }

    Connections {
        target: track4Engine
        onProgramDetailsReady:setDetails()
    }
    Connections {
        target: track4Engine
        onErrorRecord: {
            setDetails()
            errorMessage.text = track4Engine.lastErrorMessage()
            errorMessage.visible= true
        }
    }

}
