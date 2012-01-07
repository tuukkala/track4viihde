/***************************************************************************************************************
 **
 ** Copyright 2011 Tuukka Lahtela. All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without modification, are
 ** permitted provided that the following conditions are met:
 **
 **    1. Redistributions of source code must retain the above copyright notice, this list of
 **       conditions and the following disclaimer.
 **
 **    2. Redistributions in binary form must reproduce the above copyright notice, this list
 **       of conditions and the following disclaimer in the documentation and/or other materials
 **       provided with the distribution.
 **
 ** THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
 ** WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 ** FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 ** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 ** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ** ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 ** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ** ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **
 ** The views and conclusions contained in the software and documentation are those of the
 ** authors and should not be interpreted as representing official policies, either expressed
 ** or implied, of Tuukka Lahtela
 **
 ****************************************************************************************************************/

import QtQuick 1.0
import com.nokia.meego 1.0
import "Track4Constants.js" as TR4UI
import "components"

PageStack {
    id: folderStack
    property string title : qsTr("Recordings")

    ErrorPage{
        id: errorPage
        subtitle: folderStack.title
    }

    Track4Page {
        id: foldersPage
        Track4TitleBar{
            id:titleBar
            subtitle: folderStack.title
        }

        Rectangle{
            id: navigationBar
            z: 50
            anchors.top: titleBar.bottom
            width:parent.width
            height: 60
            color: TR4UI.TR4_RED
            ContentNaviButtons{
                //onBack: track4Engine.previousEpgChannel()
                //onForward: track4Engine.nextEpgChannel()
            }
            Item{
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 160
                height: parent.height
                Label{
                    id: folderName
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.horizontalCenter
                    color: TR4UI.TITLE_COLOR
                    elide:Text.ElideRight
                    text: foldersModel.currentFolder()
                }
                MouseArea{
                   anchors.fill: parent
                   onReleased: {
                       menuenabler.toHide = navigationBar
                       menuenabler.toShow = folderMenu
                       menuenabler.start()
                   }
                }
            }
        }
        CustomMenu{
            id: folderMenu
            objectName: "folderMenu"
            Rectangle{
                height: 180
                width: parent.width
                y: titleBar.y + titleBar.height
                color: TR4UI.TR4_RED
                Rectangle{
                    width: parent.width
                    anchors.top: parent.top
                    color: TR4UI.TR4_RED
                    height: 150
                    ListView{
                        model: foldersModel
                        anchors.fill: parent
                        delegate: folderMenuDelegate
                    }
                }
            }
        }
        Component{
            id: folderMenuDelegate
            Rectangle{
                height: 40
                width: folderMenu.width
                color: TR4UI.TR4_RED
                MenuLabel{
                    id: folderMenuText
                    text:   name
                }
                MenuMouseArea {
                    property string temp: id
                    onReleased: {
                        titleBar.startBusyIndicator()
                        track4Engine.getFolderList(id)
                        menuenabler.toShow = navigationBar
                        menuenabler.toHide = folderMenu
                        menuenabler.start()
                    }
                }
            }
        }

        ProgramPage{
            id: recordedProgramDetails
            controls: Item {
                anchors.fill: parent
                Button {
                    visible: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    id:playButton
                    text: qsTr("Play")
                    iconSource: "image://theme/icon-m-toolbar-mediacontrol-play"
                    onClicked: track4Engine.playVideo(programDetailsModel.url())
                }
                Button {
                    visible: true
                    anchors {
                        topMargin: 20
                        top: playButton.bottom
                        horizontalCenter: parent.horizontalCenter
                    }
                    id:removeButton
                    text: qsTr("Remove")
                    iconSource:"image://theme/icon-m-toolbar-delete"
                    onClicked: {
                        track4Engine.removeProgram(programDetailsModel.programViewId())
                        recordedProgramDetails.reload()
                    }
                }
            }
            Connections {
                target: track4Engine
                onProgramDetailsReady: {
                    playButton.visible = false//programDetailsModel.url() === "" ? false : true
                }
            }
            Connections {
                target: track4Engine
                onRemoved: folderStack.pop()
            }
        }

        ListView {
            id: recListView
            model: recordedShowsModel
            anchors{
                top: navigationBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            delegate: itemDelegate
            cacheBuffer: 164
            flickDeceleration: 100
        }
        Component{
            id: itemDelegate
            RecordingItem{
                width: recListView.width
                MouseArea{
                    anchors.fill: parent
                    onReleased:{
                        track4Engine.programDetails(id)
                        folderStack.push( recordedProgramDetails )
                    }
                }
            }
        }
        Connections {
            target: track4Engine
            onRecordedShowReady: {
                titleBar.stopBusyIndicator()
                folderName.text = foldersModel.currentFolder()
            }
        }
    }
    Connections {
        target: track4Engine
        onRemoved: {
            pop()
        }
    }
    MenuAnimator {
        id: menuenabler
    }
    Connections {
        target: folderMenu
        onHide: {
            menuenabler.toShow = navigationBar
            menuenabler.toHide = folderMenu
            menuenabler.start()
        }
    }
    Connections {
        target: track4Engine
        onErrorFolders: {
            errorPage.errorMsg = track4Engine.lastErrorMessage()
            push(errorPage);
        }
    }
    Connections {
        target: errorPage
        onRetry: {
            track4Engine.getFolderList()
            titleBar.startBusyIndicator()
            pop()
        }
    }
    Component.onCompleted: {
        push(foldersPage);
        titleBar.startBusyIndicator()
    }

}
