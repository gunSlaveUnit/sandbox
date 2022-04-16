import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Window {
    id: dialogWindow
    title: qsTr("Create new record")
    width: 270
    height: 200

    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height

    GridLayout {
        id: dataGrid

        property bool isDataEdited: false
        property int indexEditedItem: -1

        rows: 5
        columns: 2

        anchors {
            fill: parent
            margins: 10
        }

        Label {
            text: qsTr("Title")
            Layout.fillWidth: true
        }

        TextField {
            id: newGameTitle
            placeholderText: qsTr("Enter a title")
            Layout.fillWidth: true
            background: Rectangle {
                border.width: 1
                radius: 5
            }
        }

        Label {
            text: qsTr("Genre")
            Layout.fillWidth: true
        }

        TextField {
            id: newGameGenre
            placeholderText: qsTr("Enter a genre")
            Layout.fillWidth: true
            background: Rectangle {
                border.width: 1
                radius: 5
            }
        }

        Label {
            text: qsTr("Company")
            Layout.fillWidth: true
        }

        TextField {
            id: newGameCompany
            placeholderText: qsTr("Enter a company")
            Layout.fillWidth: true
            background: Rectangle {
                border.width: 1
                radius: 5
            }
        }

        Label {
            text: qsTr("Platform")
            Layout.fillWidth: true
        }

        TextField {
            id: newGamePlatform
            placeholderText: qsTr("Enter a platform")
            Layout.fillWidth: true
            background: Rectangle {
                border.width: 1
                radius: 5
            }
        }

        Button {
            Layout.fillWidth: true
            text: "OK"
            onClicked: dataGrid.isDataEdited ? set() : add();

            function add() {
                gamemodel.addRow([
                    newGameTitle.text, newGameGenre.text,
                    newGameCompany.text, newGamePlatform.text
                ]);
            }

            function set() {
                gamemodel.setRowAt(
                    dataGrid.indexEditedItem, [
                    newGameTitle.text, newGameGenre.text,
                    newGameCompany.text, newGamePlatform.text
                ])
            }
        }

        Button {
            Layout.fillWidth: true
            text: "Cancel"
            onClicked: hide()
        }
    }

    function execute(itemIndex) {
        if(itemIndex !== undefined) {
            let item = gamemodel.getBy(itemIndex);
            newGameTitle.text = item[0];
            newGameGenre.text = item[1];
            newGameCompany.text = item[2];
            newGamePlatform.text = item[3];

            dataGrid.isDataEdited = true;
            dataGrid.indexEditedItem = itemIndex;

            dialogWindow.title = qsTr("Update the record");
        } else {
            dialogWindow.title = qsTr("Create new record");
        }

        show()
    }

    onClosing: clear();

    function clear() {
        newGameTitle.text = "";
        newGameGenre.text = "";
        newGameCompany.text = "";
        newGamePlatform.text = "";

        dataGrid.isDataEdited = false;
        dataGrid.indexEditedItem = -1;
    }
}
