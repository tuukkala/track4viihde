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
import "components"

PageStack {
    id: frontStack    
    objectName: "frontStack"
    signal loginDone()

    Track4Page {
        id: loginPage
        Track4TitleBar{
            id: titleBar
            subtitle: qsTr("Log in")
        }
        BusyIndicator {
            id: loginBusyIndicator
            anchors.centerIn: parent
            running: false
            visible: false
            platformStyle: BusyIndicatorStyle { size: "large" }
        }

        Column {
            id: loginDetails
            spacing: 10            
            anchors.centerIn: parent            
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                id: errorText
                text: ""
                visible: false
            }
            Label { text: qsTr("Username") }
            TextField {
                id:userName
                anchors {left: parent.left; right: parent.right;}
                placeholderText: qsTr("Username")
                maximumLength: 80
            }

            Label { text: qsTr("Password") }
            TextField {
                id: passWord
                anchors {left: parent.left; right: parent.right;}
                placeholderText: qsTr("Password")
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            }
            Button {
                id: loginButton
                text: qsTr("Log in")
                onClicked:{
                    track4Engine.logIn(userName.text, passWord.text)
                    loginPage.logging()
                }
            }
        }
        Connections {
            target: track4Engine
            onUserLogged:  loginPage.userLogged()
            onLoginFailed: loginPage.loginFailed()
        }
        function logging(){
            loginDetails.visible = false
            loginBusyIndicator.visible = true
            loginBusyIndicator.running = true
        }
        function userLogged() {
            loginBusyIndicator.visible = false
            loginBusyIndicator.running = false
            frontStack.loginDone()
        }
        function loginFailed() {
            loginBusyIndicator.visible = false
            loginBusyIndicator.running = false
            loginDetails.visible = true
            errorText.text = track4Engine.lastErrorMessage()
            errorText.visible = true
        }
    }
    Component.onCompleted: {
        push(loginPage)
        if(track4Engine.isLoginInProgress() === true){
            loginPage.logging()
        }
    }
}

