import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.1
//import io.qt.Backend 1.0


ApplicationWindow {
    visible: true
    width: 1000
    height: 800
    title: qsTr("inforum server")
    property string pageTitle: "Home"
    property string curboard: ""
    property int curpostid: 0
    property int curuserid: 0
    property QtObject curuser
    property QtObject curpost

//    Backend {
//        id: backend
//    }

    header: ToolBar {
        background: whiteback
        visible: stack.depth > 1
        height: 40

        RowLayout {
            anchors.fill: parent
            ToolButton {
                icon.source: "qrc:images/back.png"
                onClicked: {
                    if (stack.depth == 3) {
                        inboard.currentIndex = 0
                        pageTitle = "Home"
                        if (curuser.getType() == 1 || curuser.getType() == 2) {
                            addBtn.visible = true;
                        }

                        stack.pop()
//                        stack.pop()
//                        stack.push(homePage)
                    } else if (stack.depth > 3) {
                        pageTitle = curboard
                        if (curuser.getType() == 1 || curuser.getType() == 2) {
                            addBtn.visible = true;
                        }
                        stack.pop()
                    }
                }

                background: transback
                hoverEnabled: true
                visible: stack.depth > 2
            }

            SimpleComboBox {
                id: inboard
                model: ["Home", "Class", "App Dev", "Game Dev", "Server Dev", "Bump"]
                visible: stack.depth == 2
                onActivated: {
                    if (currentText != "Home") {
                        pageTitle = currentText
                        curboard = currentText
                        stack.push(boardPage)
                    } else {
                        pageTitle = "Home"
                        while (stack.depth > 1) {
                            stack.pop();
                        }
                        stack.push(homePage)
                    }
                    console.log("currentText: " + currentText + ", curboard:" + curboard)

                }
            }

            Label {
                id: boardTitle
                text: pageTitle
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                background: transback
            }
            ToolButton {
                id: addBtn
                icon.source: "qrc:images/add.png"
                onClicked: {
                    pageTitle = "Edit"
                    addBtn.visible = false
                    stack.push(editPage)
                }

                background: transback
//                visible: (curuser.getType() == 3)
                visible: pageTitle == "Home" || pageTitle ==  "Class" || pageTitle ==  "App Dev" || pageTitle ==  "Game Dev" || pageTitle ==  "Server Dev" || pageTitle == "Bump"
            }
            ToolButton {
                icon.source: "qrc:images/profile.png"
                visible: pageTitle == "Home" || pageTitle ==  "Class" || pageTitle ==  "App Dev" || pageTitle ==  "Game Dev" || pageTitle ==  "Server Dev" || pageTitle == "Bump"
                background: transback
                onClicked: {
                    profileMenu.popup()
                }
                Menu {
                    id: profileMenu
                    MenuItem {
                        text: "Profile"
                        font.family: "Microsoft Yahei UI"
                        onTriggered: {
                            pageTitle = "profile"
                            addBtn.visible = false
                            stack.push(profilePage)
                        }
                    }
                    MenuItem {
                        text: "Logout"
                        id: logoutAction
                        font.family: "Microsoft Yahei UI"
                        onTriggered: {

                            curuser.logout()
                            pageTitle = "Home"
                            curboard = ""
                            curuserid = 0
                            while (stack.depth > 1) {
                                stack.pop()
                            }
                        }
                    }
                    MenuItem {
                        text: "Manage"
                        id: manageAction
                        font.family: "Microsoft Yahei UI"
                        onTriggered: {
                            pageTitle = "Manage"
                            stack.push(managePage)
                        }
//                        visible: (curuser.getType() == 3)
//                        width: (curuser.getType() == 3)?logoutAction.width:0
//                        height: (curuser.getType() == 3)?logoutAction.height:0
                    }

                }
            }


        }
    }


    StackView {
        id: stack;
        anchors.fill: parent
        initialItem: loginPage

    }

    Component {
        id: loginPage


        Rectangle {
            TextField {
                id: username
                x: (parent.width-width)/2
                y: parent.height/2 - 120
                width: 200
                height: 48
                focus: true
                placeholderText: "username"
                maximumLength: 10
                selectByMouse: true
            }

            TextField {
                id: password
                anchors.top: username.bottom
                anchors.topMargin: 12
                anchors.horizontalCenter: username.horizontalCenter
                width: 200
                height: 48
                focus: true
                placeholderText: "password"
                maximumLength: 10
                selectByMouse: true
            }

            CustomButton {
                id: login
                label: "login"
                height: 48
                width: 200
                x: (parent.width - width)/2
                anchors.top: password.bottom
                anchors.topMargin: 24
                onButtonClick: {
                    curuserid = backend.userModel.getUserQ(curuserid).login(username.text, password.text)
                    if (curuserid >= -1) {
                        curuser = backend.userModel.getUserQ(curuserid)
                        if (curuser.getType() == 3) {
                            addBtn.visible = false
                            manageAction.visible = true
                            manageAction.width = logoutAction.width
                            manageAction.height = logoutAction.height

                        } else if (curuser.getType() == 4) {
                            addBtn.visible = false
                            manageAction.visible = false
                            manageAction.width = 0
                            manageAction.height = 0
                        } else {
                            addBtn.visible = true
                            manageAction.visible = false
                            manageAction.width = 0
                            manageAction.height = 0
                        }
                        pageTitle = "Home"
                        stack.push(homePage)
                    } else {
                        curuserid = 0
                        loginErrorDialog.open()
                    }
                }
            }

            CustomButton {
                id: register
                label: "register"
                height: 48
                width: 200
                x: (parent.width - width)/2                
                anchors.top: login.bottom
                anchors.topMargin: 12
                onButtonClick: {
                    if (username.text.length > 0 && password.text.length > 0)
                        backend.userModel.getUserQ(curuserid).register1(username.text, password.text, false, 1, "Nothing")
                }
            }
        }

    }

    Component {
        id: homePage

        ListView {
            model: backend.postModel
            delegate: homeDelegate
        }
    }

    Component {
        id: boardPage
        ListView {
            model: backend.postModel
            delegate: boardDelegate
        }
    }

    Component {
        id: postDetailPage
        ListView {
            header: Rectangle {
                id: aboveComment
                width: parent.width
                height: postBody.height + commentBox.height + 2* breakline.height + commentButton.height + 20
                Rectangle {
                    id: postBody
                    height: postTitle.height + postAuthorTime.height + postContent.height
                    width: parent.width

                    Text {
                        id: postTitle
                        text: curpost.title()
                        font.family: "Microsoft Yahei UI"
                        font.pixelSize: 20
                        wrapMode: Text.WrapAnywhere
                        leftPadding: 20
                        topPadding: 10
                        bottomPadding: 10

                    }

                    Text {
                        id: postAuthorTime
                        anchors.top: postTitle.bottom
                        text: curpost.author() + "  Published at  " + curpost.time()
                        font.family: "Microsoft Yahei UI"
                        font.pixelSize: 14
                        color: "gray"
                        wrapMode: Text.WrapAnywhere
                        leftPadding: 20
                        topPadding: 10
                        bottomPadding: 10

                    }

                    Text {
                        id: postContent
                        anchors.top: postAuthorTime.bottom
                        text: curpost.content();
                        font.family: "Microsoft Yahei UI"
                        font.pixelSize: 16
                        wrapMode: Text.WrapAnywhere
                        leftPadding: 20
                        topPadding: 10
                        bottomPadding: 10
                    }

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onClicked: {
                            if (mouse.button === Qt.RightButton && curuser.canDeletePost(curpostid))
                                contextMenu.popup()
                        }
                        onPressAndHold: {
                            if (mouse.source === Qt.MouseEventNotSynthesized && curuser.canDeletePost(curpostid))
                                contextMenu.popup()
                        }

                        Menu {
                            id: contextMenu
                            MenuItem {
                                text: "Delete"
                                font.family: "Microsoft Yahei UI"
                                onTriggered: {
                                    if (curuser.deletePost(curpostid)) {
                                        stack.pop();
                                        stack.pop();
                                        if (pageTitle == "Post in Home") {
                                            pageTitle = "Home"
                                            if (curuser.getType() == 1 || curuser.getType() == 2)
                                                addBtn.visible = true
                                            stack.push(homePage)
                                        }

                                        else {
                                            pageTitle = curboard
                                            if (curuser.getType() == 1 || curuser.getType() == 2)
                                                addBtn.visible = true
                                            stack.push(boardPage)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: breakline
                    width: parent.width
                    height: 2
                    anchors.top: postBody.bottom
                    anchors.topMargin: 5
                    border.width: 1
                    border.color: "lightsteelblue"
                }



                TextEdit {
                    id: commentBox
                    width: parent.width
                    height: ((curuser.getType() != 3) && (curuser.getType() != 4))?200:0
                    visible: ((curuser.getType() != 3) && (curuser.getType() != 4))
                    anchors.top: breakline.bottom
                    anchors.topMargin: 5
                    text: "leave your comment"
                    font.family: "Microsoft Yahei UI"
                    focus: true
                    wrapMode: Text.WrapAnywhere
                    padding: 20
                    selectByMouse: true

                }

                Rectangle {
                    id: breakline2
                    width: parent.width
                    height: 2
                    anchors.top: commentBox.bottom
                    anchors.topMargin: 5
                    border.width: 1
                    border.color: "lightsteelblue"
                }

                CustomButton {
                    id: commentButton
                    x: 20
                    visible: curuser.getType() != 3 && curuser.getType() != 4
                    height: (curuser.getType() != 3 && curuser.getType() != 4)?48:0
                    label: "Comment"
                    anchors.top: breakline2.bottom
                    anchors.topMargin: 5
                    onButtonClick: {
                        if ((curuser.getType() != 3) && (curuser.getType() != 4)) {
                            curuser.addComment(commentBox.text, curpostid)
                            addBtn.visible = false
                        }
                    }
                }

            }
            id: commentList
            model: curpost.commentModelQ()
            delegate: postDetailDelegate
        }

    }

    Component {
        id: homeDelegate
        Rectangle {
            height: 100
            width: parent.width
            Text {
                id: itemTitle
                text: title
                font.family: "Microsoft Yahei UI"
                font.pixelSize: 18
                leftPadding: 20
                topPadding: 20
            }
            Text {
                id: itemInfo
                anchors.top: itemTitle.bottom
                text: author + "  /  " + board + "  /  " + time;
                font.family: "Microsoft Yahei UI"
                font.pixelSize: 14
                color: "gray"
                leftPadding: 20
                topPadding: 10
            }
            Rectangle {
                id: breakline4
                width: parent.width
                height: 2
                anchors.top: itemInfo.bottom
                anchors.topMargin: 10
                border.width: 1
                border.color: "aliceblue"
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    curpostid = id
                    curpost = backend.postModel.getPostQ(id)
                    console.log("curpost addr:" + curpost)
                    pageTitle = "Post in Home"
                    addBtn.visible = false
                    stack.push(postDetailPage);
                }
            }
        }
    }

    Component {
        id: boardDelegate
        Rectangle {
            height: (board == curboard)?100:0
            width: (board == curboard)?parent.width:0
            Text {
                id: itemTitle
                text: title
                font.family: "Microsoft Yahei UI"
                font.pixelSize: 18
                leftPadding: 20
                topPadding: 20
                visible: (board == curboard)
            }
            Text {
                id: itemInfo
                anchors.top: itemTitle.bottom
                text: author + "  /  " + board + "  /  " + time;
                font.family: "Microsoft Yahei UI"
                font.pixelSize: 14
                color: "gray"
                leftPadding: 20
                topPadding: 10
                visible: (board == curboard)
            }
            Rectangle {
                id: breakline4
                width: parent.width
                height: 2
                anchors.top: itemInfo.bottom
                anchors.topMargin: 10
                border.width: 1
                border.color: "aliceblue"
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                        curpostid = id
//                        delete curpost
                        curpost = backend.postModel.getPostQ(id)
                        console.log("curpost addr:" + curpost)
                        pageTitle = "Post in " + board
                        addBtn.visible = false
                        stack.push(postDetailPage);
                    }
            }
        }

    }

    Component {
        id: postDetailDelegate
        Rectangle {
            width: parent.width
            height: commentText.height + breakline3.height + 10
            Rectangle {
                id: breakline3
                width: parent.width
                height: 2
                anchors.topMargin: 10
                border.width: 1
                border.color: "aliceblue"
            }
            Text {
                id: commentText
                text: author + " / "  + time + ":\n" + content;
                font.family: "Microsoft Yahei UI"
                wrapMode: Text.WrapAnywhere
                padding: 20
                anchors.top: breakline3.bottom
            }
        }
    }

    Component {
        id: editPage
        Rectangle {
            x: 20
            RowLayout {
                id: postTop
                width: parent.width
                SimpleComboBox {
                    id: selectedBoard
                    width: 100
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 20
                    model: ["Class", "App Dev", "Game Dev", "Server Dev", "Bump"]
                    padding: 20
                    onAccepted: {

                    }
                }

                CustomButton {
                    id: sendBtn
                    label: "Publish"
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 20
                    onButtonClick: {
                        curuser.addPost(postBoxTitle.text, postBox.text, selectedBoard.currentText)
//                        curuser.addPost(postBoxTitle.text, postBox.text, selectedBoard.currentText)
                        stack.pop();
                        stack.pop();
                        if (inboard.currentText == "Home") {
                            pageTitle = "Home"
                            if (curuser.getType() == 1 || curuser.getType() == 2)
                                addBtn.visible = true
                            stack.push(homePage);

                        }

                        else {
                            pageTitle = curboard;
                            if (curuser.getType() == 1 || curuser.getType() == 2)
                                addBtn.visible = true
                            stack.push(boardPage)
                        }

                    }


                }
            }

            Rectangle {
                id: breakline4
                width: parent.width
                height: 2
                anchors.top: postTop.bottom
                anchors.topMargin: 10
                border.width: 1
                border.color: "steelblue"
            }


            TextInput {
                id: postBoxTitle
                width: parent.width
                anchors.top: breakline4.bottom
                anchors.topMargin: 10
                text: "title"
                font.family: "Microsoft Yahei UI"
                focus: true
                leftPadding: 20
            }
            Rectangle {
                id: breakline5
                width: parent.width
                height: 2
                anchors.top: postBoxTitle.bottom
                anchors.topMargin: 10
                border.width: 1
                border.color: "steelblue"
            }

            TextEdit {
                id: postBox
                width: parent.width
                anchors.top: breakline5.bottom
                anchors.topMargin: 10
                text: "post your post"
                font.family: "Microsoft Yahei UI"
                focus: true
                wrapMode: Text.WrapAnywhere
                padding: 20
                selectByMouse: true
            }

        }
    }

    Component {
        id: profilePage
        Rectangle {
            RowLayout {
                id: profile_title
                anchors.horizontalCenter: parent.horizontalCenter
                y: (parent.height/2) - 100
                Text {
                    id: title_username
                    Layout.preferredWidth: 100
                    text: "USERNAME"
                    font.family: "Microsoft Yahei UI"
                    color: "steelblue"
                }
                Text {
                    id: profile_username
                    Layout.preferredWidth: 100
                    text: curuser.getUserame()
                    font.family: "Microsoft Yahei UI"
                }
            }
            RowLayout {
                id: profile_info
                anchors.top: profile_title.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    id: title_id
                    Layout.preferredWidth: 100
                    text: "USERID"
                    font.family: "Microsoft Yahei UI"
                    color: "steelblue"
                }
                Text {
                    id: profile_id
                    Layout.preferredWidth: 100
                    text: curuser.getId()
                    font.family: "Microsoft Yahei UI"
                }
            }

        }
    }

    Component {
        id: managePage

        ListView {
            model: backend.userModel
            header: Rectangle {
                height: 40
                RowLayout {
                    spacing: 0
                    Text {
                        Layout.preferredWidth: 100
                        text: qsTr("UserId")
                        font.family: "Microsoft Yahei UI"
                        padding: 10
                    }
                    Text {
                        Layout.preferredWidth: 100
                        text: qsTr("UserName")
                        font.family: "Microsoft Yahei UI"
                        padding: 10
                    }
                    Text {
                        Layout.preferredWidth: 200
                        text: qsTr("Managing Board")
                        font.family: "Microsoft Yahei UI"
                        padding: 10
                    }
                    Text {
                        Layout.preferredWidth: 200
                        text: qsTr("Set/Unset Board Manager")
                        font.family: "Microsoft Yahei UI"
                    }
                }
            }

            delegate: RowLayout {
                spacing: 0
                Text {
                    Layout.preferredWidth: 100
                    text: id
                    font.family: "Microsoft Yahei UI"
                    padding: 10
                }
                Text {
                    Layout.preferredWidth: 100
                    text: username
                    font.family: "Microsoft Yahei UI"
                    padding: 10
                }
                Text {
                    Layout.preferredWidth: 200
                    text: board_managed
                    font.family: "Microsoft Yahei UI"
                    padding: 10
                }
                SimpleComboBox {
                    id: selectedBoard1
                    Layout.preferredWidth: 200
                    model: ["Nothing", "Class", "App Dev", "Game Dev", "Server Dev", "Bump"]
                    onActivated: {
                        curuser.setBoardManager(id, selectedBoard1.currentText)
                        stack.pop()
                        pageTitle = "Manage"
                        stack.push(managePage)
                    }

                    currentIndex: (function () {
                        if (board_managed == "Class")
                            return 1;
                        else if (board_managed == "App Dev")
                            return 2;
                        else if (board_managed == "Game Dev")
                            return 3;
                        else if (board_managed == "Server Dev")
                            return 4;
                        else if (board_managed == "Bump")
                            return 5;
                        else
                            return 0;
                    })()

                }


            }
        }



    }


    Rectangle {
        id: transback
        color: "transparent"
    }
    Rectangle {
        id: whiteback
        color: "white"
    }
    Rectangle {
        id: lsbback
        color: "lightsteelblue"
    }

    MessageDialog {
        id: connectErrorDialog
        objectName: "connectError"
        title: "Error Connecting"
        text: "error connecting mysql"
        onAccepted: {
//            console.log("And of course you could only agree.")
            loginErrorDialog.close()
        }
//        function openDialog(msg){
//            loginErrorDialog.open()
//        }

//        Component.onCompleted: visible = true
    }

    MessageDialog {
        id: loginErrorDialog
        title: "Error Login"
        text: "username and password do not match"
        onAccepted: {
            loginErrorDialog.close()
        }
    }

}
