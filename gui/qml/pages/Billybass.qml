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
                                              "imagelocation": "/usr/share/icons/hicolor/86x86/apps/harbour-billybass.png"} )
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
                title: "Billy Bass"
            }
            Label
            {
                x: Theme.paddingLarge
                text: "Big Mouth"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
            Label
            {
                text: espeak.libespeakVersion
                anchors.horizontalCenter: parent.horizontalCenter
            }
            TextField
            {
                id: textfield
                focus: false
                width: parent.width
                label: "Sano jotain"
                font.pixelSize: Theme.fontSizeExtraLarge
                color: Theme.primaryColor
                placeholderText: "kirjoita tähän"
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: espeak.synth(text)
            }

            Button
            {
                text: "sano ny"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: espeak.synth(textfield.text)
            }
        }
    }

}


