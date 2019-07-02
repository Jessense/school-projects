import QtQuick 2.4

Rectangle {

    //identifier of the item
    id: button

    property string label: "button label"

    //these properties act as constants, useable outside this QML file
    property int buttonHeight: 48
    property int buttonWidth: 100

    //the color highlight when the mouse hovers on the rectangle
    property color onHoverColor: "gold"
    property color borderColor: "white"

    //buttonColor is set to the button's main color
    property color buttonColor: "lightsteelblue"

    //set appearance properties
    radius: 0
    smooth: true
    border{color: "transparent"; width: 0}
    width: buttonWidth; height: buttonHeight

    Text{
        id: buttonLabel
        anchors.centerIn: parent
        text: label
        font.family: "Microsoft Yahei UI"
    }

    //buttonClick() is callable and a signal handler, onButtonClick is automatically created
    signal buttonClick()
    onButtonClick: {
        console.log(buttonLabel.text + " clicked" )
    }

    //define the clickable area to be the whole rectangle
    MouseArea{
        id: buttonMouseArea
        anchors.fill: parent    //stretch the area to the parent's dimension
        onClicked: buttonClick()

        //if true, then onEntered and onExited called if mouse hovers in the mouse area
        //if false, a button must be clicked to detect the mouse hover
        hoverEnabled: true

        //display a border if the mouse hovers on the button mouse area
        onEntered: parent.color = Qt.darker(buttonColor, 1.05)
        //remove the border if the mouse exits the button mouse area
        onExited:  parent.color = buttonColor
        onPressed: parent.color = Qt.darker(buttonColor, 1.5)
        onReleased: parent.color = Qt.darker(buttonColor, 1.05)
    }

    //change the color of the button when pressed
    color: buttonColor
}
