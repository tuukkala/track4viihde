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
    id: epgStack
    objectName: "epgStack"
    property string title : qsTr("Week's programs")

    ErrorPage{
        id: errorPage
        subtitle: epgStack.title
    }

    Track4Page {
        objectName: "epgpage"
        id: epgPage
        anchors.fill: parent

        Track4TitleBar{
            id: titleBar
            subtitle: epgStack.title
        }
        Rectangle{
            id: navigationBar
            z: 50
            anchors.top: titleBar.bottom
            width:parent.width
            height: 60
            color: TR4UI.TR4_RED
            ContentNaviButtons{
                onBack: track4Engine.previousEpgChannel()
                onForward: track4Engine.nextEpgChannel()
            }
            Item{
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 160
                height: parent.height
                MouseArea{
                    anchors.left: parent.left
                    width: parent.width/2
                    height: parent.height
                    onReleased: {
                        menuenabler.toHide = navigationBar
                        menuenabler.toShow = channelMenu
                        menuenabler.start()
                    }
                    Label{
                        id: currentChannel
                        anchors.rightMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: TR4UI.TITLE_COLOR
                        elide:Text.ElideRight
                        text: track4Engine.currentChannel()
                        font.pixelSize: TR4UI.FONT_SLARGE
                    }
                }
                MouseArea{
                    anchors.right: parent.right
                    width: parent.width/2
                    height: parent.height
                    onReleased: {
                        menuenabler.toHide = navigationBar
                        menuenabler.toShow = programDateMenu
                        menuenabler.start()
                    }
                    Label{
                        id: currentDate
                        anchors.leftMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: TR4UI.TITLE_COLOR
                        elide:Text.ElideRight
                        font.pixelSize: TR4UI.FONT_SLARGE
                    }
                }
            }
        }

        CustomMenu{
            id: channelMenu
            objectName: "channelMenu"
            anchors{
                top: titleBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            Rectangle{                
                id: listContainer
                anchors.fill: parent
                color: TR4UI.TR4_RED                
                ListView{
                    id: channelList
                    model: channelModel
                    anchors.fill: parent
                    delegate: menuDelegate
                    spacing: 15
                }
            }
        }
        Component{
            id: menuDelegate
            Rectangle{
                height: 80
                width: channelMenu.width
                color: TR4UI.TR4_RED
                MenuLabel{
                    id: channelMenuText
                    text: display
                    font.pixelSize: TR4UI.FONT_GIANT
                }
                MenuMouseArea {
                    onReleased: {
                        track4Engine.changeChannel(channelMenuText.text)
                        menuenabler.toShow = navigationBar
                        menuenabler.toHide = channelMenu
                        menuenabler.start()
                    }
                }
            }
        }

        CustomMenu{
            id: programDateMenu
            objectName: "programDateMenu"
            anchors{
                top: titleBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            Rectangle{
                id: programListContainer
                anchors.fill: parent
                color: TR4UI.TR4_RED
                ListView{
                    id: programDataList
                    model: programDateModel
                    anchors.fill: parent
                    delegate: dateMenuDelegate
                    spacing: 15
                }
            }
        }
        Component{
            id: dateMenuDelegate
            Rectangle{
                height: 80
                width: programDateMenu.width
                color: TR4UI.TR4_RED
                MenuLabel{
                    id:dateMenuText
                    text: display
                    font.pixelSize: TR4UI.FONT_GIANT
                }
                MenuMouseArea {
                    onReleased: {
                        programList.positionViewAtIndex(epgModel.getFirstIndexOfDate(dateMenuText.text), ListView.Beginning)
                        menuenabler.toShow = navigationBar
                        menuenabler.toHide = programDateMenu
                        menuenabler.start()
                    }
                }
            }
        }
        ListView {
            id: programList
            anchors{
                top: navigationBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            model: epgModel
            delegate: itemDelegate
            cacheBuffer: 540
            flickDeceleration: 100
            spacing: 15
        }

        Component {
            id: itemDelegate
            Rectangle{
                id: itemContainer
                objectName: "programItem"
                width: programList.width
                height: 150
                color: TR4UI.MAIN_BG
                property string programId: id        
                MouseArea{
                    anchors.fill: parent
                    onReleased:{
                        programDetails.reload()
                        track4Engine.programDetails(parent.programId)
                        epgStack.push( programDetails )
                    }
                }
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
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: itemContainer.bottom
                    anchors.top: divider.bottom
                    anchors.leftMargin: TR4UI.DIVIDER_MARGIN
                    anchors.rightMargin: TR4UI.DIVIDER_MARGIN
                    color: recorded ? "#0cff0c" : TR4UI.MAIN_BG
                    Label {
                        id: programName
                        height: 31
                        anchors.topMargin: TR4UI.TEXT_MARGIN
                        anchors.leftMargin: TR4UI.TEXT_MARGIN
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width;
                        elide:Text.ElideRight
                        text: "<b>"+name+"</b>"
                    }
                    Label {
                        id: timeLabel
                        height: 28
                        anchors.top: programName.bottom
                        anchors.right: parent.right
                        anchors.rightMargin: TR4UI.TEXT_MARGIN
                        font.pixelSize: TR4UI.FONT_LSMALL
                        text: time
                    }
                    Label {
                        height: 56
                        anchors.top: timeLabel.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: TR4UI.TEXT_MARGIN
                        width: parent.width;
                        font.pixelSize: TR4UI.FONT_LSMALL
                        elide:Text.ElideRight
                        maximumLineCount: 2
                        text: description
                    }
                }
            }
        }
        ProgramPage{
            id: programDetails
            controls: RecordingControls { id:recControls}
            Connections {
                target: recControls
                onRefresh: programDetails.reload()
            }
        }

        MenuAnimator {
            id: menuenabler
        }

        Connections {
            target: channelMenu
            onHide: {
                menuenabler.toShow = navigationBar
                menuenabler.toHide = channelMenu
                menuenabler.start()
            }
        }
        Connections {
            target: programDateMenu
            onHide: {
                menuenabler.toShow = navigationBar
                menuenabler.toHide = programDateMenu
                menuenabler.start()
            }
        }
        Connections {
            target: track4Engine
            onEpgReady: {                                    
                titleBar.stopBusyIndicator()
                currentChannel.text = track4Engine.currentChannel()
                navigationBar.visible = true

            }
        }
        Connections {
            target: track4Engine
            onEpgDateChanged: {
                currentDate.text = track4Engine.getEpgDate();
            }
        }        
    }

    Connections {
        target: track4Engine
        onErrorEpg: {
            errorPage.errorMsg = track4Engine.lastErrorMessage()
            push(errorPage);
        }
    }
    Connections {
        target: errorPage
        onRetry: {
            track4Engine.getEpg()
            navigationBar.visible = false
            titleBar.startBusyIndicator()
            pop()
        }
    }
    Component.onCompleted: {
        push(epgPage);
        if(track4Engine.isEpgReady() === false){
            navigationBar.visible = false
            titleBar.startBusyIndicator()
        }
    }
}
