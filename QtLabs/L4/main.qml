import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 400
    height: 700
    visible: true
    title: qsTr("GAMER'S ROOM")

    Control {
        anchors {
            fill: parent
            centerIn: parent
            margins: 10
        }

        Page {
            id: page
            anchors.fill: parent
            background: Rectangle {
                radius: 10
            }

            ListView {
                id: gamelist
                anchors.fill: parent
                spacing: 10

                model: gamemodel
                delegate: Rectangle {
                    id: gameitem
                    width: ListView.view.width
                    height: 110
                    radius: 10
                    color: mouseAreaGameItem.containsMouse ?
                               Qt.darker("steelblue", 1.4) : "steelblue";

                    property int itemIndex: index

                    Control {
                        anchors {
                            fill: parent
                            centerIn: parent
                            margins: 10
                        }

                        MouseArea {
                            id: mouseAreaGameItem
                            anchors.fill: parent
                            hoverEnabled: true
                        }

                        Text {
                            id: gameTitle
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                            }
                            text: gametitle
                            font.pointSize: 20
                            font.bold: true
                            color: "white"
                        }

                        Text {
                            id: gameGenre
                            anchors {
                                top: parent.top
                                right: editButton.left
                                rightMargin: 30
                            }
                            text: gamegenre
                            font.pointSize: 15
                            color: "white"
                        }

                        Text {
                            id: gameCompany
                            anchors {
                                top: gameGenre.bottom
                                right: editButton.left
                                rightMargin: 30
                            }
                            text: gamecompany
                            font.pointSize: 15
                            color: "white"
                        }

                        Text {
                            anchors {
                                top: gameCompany.bottom
                                right: editButton.left
                                rightMargin: 30
                            }
                            text: gameplatform
                            font.pointSize: 15
                            color: "white"
                        }

                        RoundButton {
                            id: editButton
                            icon.width: 25
                            icon.height: 25
                            icon.color: "transparent"
                            icon.source: "qrc:/icons/pen.png"
                            width: 40
                            height: 40
                            radius: 20
                            text: ""
                            anchors {
                                top: parent.top
                                right: parent.right
                                margins: 5
                            }
                            onClicked: dialog.execute(itemIndex)
                        }

                        RoundButton {
                            id: deleteButton
                            icon.width: 25
                            icon.height: 25
                            icon.color: "transparent"
                            icon.source: "qrc:/icons/trash.png"
                            width: 40
                            height: 40
                            radius: 20
                            text: ""
                            anchors {
                                top: editButton.bottom
                                right: parent.right
                                margins: 5
                            }
                            onClicked: gamemodel.deleteRow(itemIndex)
                        }
                    }
                }
            }
        }

        Label {
            id: counter
            property int amountFoundedItems: 0
            anchors {
                left: parent.left
                verticalCenter: searchButton.verticalCenter
            }
            text: "Found: " + amountFoundedItems
        }

        TextField {
            id: genreForSearch
            width: 140
            anchors {
                left: counter.right
                verticalCenter: searchButton.verticalCenter
                leftMargin: 10
            }
            background: Rectangle {
                border.width: 1
                radius: 5
            }
            placeholderText: "Enter a genre to search ..."
        }

        RoundButton {
            id: searchButton
            icon.width: 30
            icon.height: 30
            icon.color: "transparent"
            icon.source: "qrc:/icons/search.png"
            width: 50
            height: width
            radius: width / 2
            anchors {
                left: genreForSearch.right
                bottom: parent.bottom
                leftMargin: 10
            }
            onClicked: {
                let amount = gamemodel.searchByGenre(genreForSearch.text);
                counter.amountFoundedItems = amount;
            }
        }

        RoundButton {
            id: addButton
            icon.width: 30
            icon.height: 30
            icon.color: "transparent"
            icon.source: "qrc:/icons/plus.png"
            width: 50
            height: width
            radius: width / 2
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            onClicked: dialog.execute()
        }

        GameDialog {
            id: dialog
        }
    }
}
