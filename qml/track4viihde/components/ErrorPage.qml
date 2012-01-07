// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "../Track4Constants.js" as TR4UI
import com.nokia.meego 1.0

Track4Page {
    id: errorPage
    property string subtitle: ""
    property string errorMsg: ""
    signal retry()

    Track4TitleBar{subtitle: errorPage.subtitle}

    Column {
        id: errocDetails
        spacing: 10
        anchors.centerIn: parent
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            id: errorText
            text: errorMsg
        }

        Button {
            id: loginButton
            text: qsTr("Retry")
            onClicked:{
                errorPage.retry()
            }
        }
    }


}
