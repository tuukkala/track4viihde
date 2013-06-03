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

Window {

    id: appWindow
    objectName: "mainWindow"

    StatusBar {
        id: statusbar
        anchors.top: parent.top
    }

    ToolBar{
        id:commonToolBar
        z: 100
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        tools: ToolBarLayout {
            visible: true
            ButtonRow {                
                TabButton {                    
                    iconSource: "images/icon-epg.png"
                    tab: epgTab
                }
                TabButton {                    
                    iconSource: "image://theme/icon-m-common-video"
                    tab: recTab
                }
                TabButton {
                    iconSource: "image://theme/icon-m-toolbar-directory"
                    tab: folderTab
                }
                TabButton {
                    iconSource: "image://theme/icon-m-toolbar-settings"
                    tab: settingsTab
                }
            }
        }
    }

    LoginPage{
        id: loginView
        anchors {
            top: statusbar.bottom
            bottom: commonToolBar.top
            left: parent.left
            right: parent.right
        }
        onLoginDone: appWindow.enableClient()
    }
    Dialog {
        id:errorDialog
        width: appWindow.width
        content: Item {
            anchors.centerIn:parent
            Label{
                anchors.horizontalCenter: parent.horizontalCenter
                color:TR4UI.TITLE_COLOR
                id: errorTitle

            }
            ButtonRow {
                anchors.topMargin: 20
                anchors.top: errorTitle.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                id: errorButtons
                style: ButtonStyle { }
                Button {
                    text: qsTr("Try again")
                    onClicked:{
                        track4Engine.refreshAll()
                        errorDialog.accept()
                    }
                }

                Button{
                    text: qsTr("Cancel")
                    onClicked: errorDialog.reject()
                }
            }            
        }
    }
    TabGroup {
        id: tabGroup
        objectName: "tabGroup"
        visible: false
        anchors {
            top: statusbar.bottom
            bottom: commonToolBar.top
            left: parent.left
            right: parent.right
        }        
        EpgPage {
            anchors.fill: parent
            id: epgTab
        }
        RecordingsPage {
            anchors.fill: parent
            id: recTab
        }
        FoldersPage {
            anchors.fill: parent
            id: folderTab
        }
        Track4Page {
            id: settingsTab
            Track4TitleBar{subtitle: qsTr("Settings")}
            Label{
                anchors.centerIn: parent
                text: "Track4Viihde 0.0.7"
            }
        }
    }  
    Connections {
        target: track4Engine
        onError: {
            errorTitle.text = track4Engine.lastErrorMessage()
            errorDialog.open()
        }
    }
    function enableClient(){
        loginView.visible = false
        tabGroup.visible = true
        tabGroup.currentTab = epgTab
    }
}
