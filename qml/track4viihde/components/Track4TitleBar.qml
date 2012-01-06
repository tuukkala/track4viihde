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
import "../Track4Constants.js" as TR4UI

Rectangle{
    id:titleBar
    z: 100
    property string subtitle: ""
    anchors.top: parent.top
    objectName: "TitleBar"
    width: parent.width
    height: 60
    signal forward()
    signal back()

    gradient: Gradient {
        GradientStop { position: 0.0; color: TR4UI.TITLE_BG1 }
        GradientStop { position: 1.0; color: TR4UI.TITLE_BG2 }
    }
    Image{
        id: tr4Icon
        anchors.margins: 5
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        source: "../images/TRACK4Viihde_logo_plain_N9_99x41.png"
    }
    Label{
        anchors.centerIn: parent
        //font.pointSize: TR4UI.TITLE
        color: TR4UI.TITLE_COLOR
        elide:Text.ElideRight
        text: titleBar.subtitle
    }
    BusyIndicator {
        id: titleBusyIndicator
        anchors.centerIn: parent
        running: false
        visible: false
        platformStyle: BusyIndicatorStyle {
            inverted: true
        }
    }
    ToolIcon{
        id: seachIcon
        anchors.right: parent.right
        visible: true
        iconId: "icon-m-toolbar-search"
        //onClicked:
    }
    function refresh(){
        track4Engine.getChannelList()
        track4Engine.recordingModel()
        track4Engine.topTenModel()
    }
    function startBusyIndicator(){
        titleBusyIndicator.visible = true
        titleBusyIndicator.running = true
    }
    function stopBusyIndicator(){
        titleBusyIndicator.visible = false
        titleBusyIndicator.running = false
    }
}
