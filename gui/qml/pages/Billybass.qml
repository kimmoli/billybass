/*
    billybass, Billy Bass
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    id: page

    SilicaFlickable
    {
        anchors.fill: parent

        PullDownMenu
        {
            MenuItem
            {
                text: "About..."
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"),
                                          { "version": espeak.version,
                                              "year": "2014",
                                              "name": "Billy Bass",
                                              "imagelocation": "/usr/share/icons/hicolor/86x86/apps/billybass.png"} )
            }
        }

        contentHeight: column.height

        Column
        {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader
            {
                title: "Billybass"
            }
            Label
            {
                x: Theme.paddingLarge
                text: "Hello you"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
            Button
            {
                text: "test"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: espeak.test()
            }
        }
    }

}


