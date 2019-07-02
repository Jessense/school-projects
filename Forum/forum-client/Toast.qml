import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import "."

Rectangle {
    property bool isShow: false
    property alias text: t.text     // 显示的文字
    width: childrenRect.width
    height: childrenRect.height
    //z: 100
    color: "#666666"
    opacity: isShow ? 1 : 0
    //border.width: units.dp(1)
    //border.color: "white"
    radius: width/2

    Behavior on opacity {
        NumberAnimation { duration: 1000 }
    }


    ColumnLayout {
        Label {
            id: t;
            Layout.margins: 15
            color: "white"
            text: ""
            font.family: "Microsoft Yahei UI"
            font.pixelSize: 20
        }
    }

    Timer {
        id: toastTimer
        interval: 1000
        onTriggered: isShow = false
    }

    // 显示toast函数
    function showToast(text) {
        t.text = text;
        isShow = true;
        toastTimer.restart();
    }
}
