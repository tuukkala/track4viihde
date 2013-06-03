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
    id: recStack

    property string title : qsTr("Next Recordings")

    ErrorPage{
        id: errorPage
        subtitle: recStack.title
    }

    Track4Page {
        id: recordingsPage
        Track4TitleBar{
            id:titleBar
            subtitle: recStack.title
        }

        ListView {
            id: recListView
            model: recordingModel
            anchors{
                top: titleBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            delegate: itemDelegate
            cacheBuffer: 164
            flickDeceleration: 100
            spacing: 30
        }
        Component{
            id: itemDelegate
            RecordingItem{
                width: recListView.width
                MouseArea{
                    anchors.fill: parent
                    onReleased:{
                        track4Engine.programDetails(id)
                        recStack.push( recProgramDetails )
                    }
                }
            }
        }
        ProgramPage{
            id: recProgramDetails
            controls: RecordingControls { id:rmControls }
            Connections {
                target: rmControls
                onRefresh: recProgramDetails.reload()
            }
        }
        Connections {
            target: track4Engine
            onRecordingsReady: {
                titleBar.stopBusyIndicator()
            }
        }
    }
    Connections {
        target: track4Engine
        onErrorRecordings: {
            errorPage.errorMsg = track4Engine.lastErrorMessage()
            push(errorPage);
        }
    }
    Connections {
        target: errorPage
        onRetry: {
            track4Engine.getRecordings()
            titleBar.startBusyIndicator()
            pop()
        }
    }


    Component.onCompleted: {
        push(recordingsPage);
        titleBar.startBusyIndicator()
    }

}
