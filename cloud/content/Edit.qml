import QtQuick 1.1
Rectangle {
    property alias text: edit.text
    property alias font: edit.font
    property alias textColor: edit.color
    property alias wrapMode: edit.wrapMode
    property alias readOnly: edit.readOnly
    property int nbLine: 0

    id: root

    width: 200
    height: (nbLine==0)? 100 : nbLine* edit.font.pointSize +20

    clip: true
    Rectangle {
        anchors.fill: parent
    border.width: (readOnly) ? 0 : 1
    border.color: "#4b4b4b"
    radius: 2
    }



    Flickable {
        id: flick

        anchors.centerIn: parent
        width: parent.width - 10
        height: parent.height - 10
        contentWidth: edit.paintedWidth
        contentHeight: edit.paintedHeight
        clip: true

        function ensureVisible(r)
        {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX+width <= r.x+r.width)
                contentX = r.x+r.width-width;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height;
        }



        TextEdit {
            id: edit

            width: root.width-10
//            height: root.height-10
//            anchors.centerIn: root
            focus: true
            wrapMode: TextEdit.Wrap
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
        }
    }
}