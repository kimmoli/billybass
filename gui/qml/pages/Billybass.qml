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
                                              "year": "2015",
                                              "name": "Billy Bass",
                                              "imagelocation": "/usr/share/icons/hicolor/86x86/apps/harbour-billybass.png"} )
            }
            MenuItem
            {
                text: "Change language..."
                onClicked:
                {
                    languages.clear()

                    var tmp = espeak.getVoices()

                    for (var i=0 ; i<tmp.length; i++)
                    {
                        console.log(i + " = " + tmp[i]["language"] + " - " + tmp[i]["voicename"] )

                        languages.append({"language": tmp[i]["language"], "voicename": tmp[i]["voicename"]})
                    }

                    var dialog = pageStack.push(Qt.resolvedUrl("LanguageSelector.qml"))

                    dialog.accepted.connect(function()
                    {
                        espeak.setLanguage(dialog.newLanguage)
                        voiceVariant.value = 0
                    })
                }
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
                title: "Big Mouth Billy Bass"
            }

            Label
            {
                text: espeak.language
                font.capitalization: Font.Capitalize
                anchors.horizontalCenter: parent.horizontalCenter
            }

            TextField
            {
                id: textfield
                focus: false
                width: parent.width
                label: "Synthesize this:"
                font.pixelSize: Theme.fontSizeExtraLarge
                color: Theme.primaryColor
                placeholderText: "Write here"
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: espeak.synth(text)
            }

            Slider
            {
                id: voiceVariant
                value: 0
                minimumValue: -5
                maximumValue: 5
                stepSize: 1
                width: parent.width - 2*Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                valueText: variantName(value)

                onValueChanged: espeak.setVariant(variantName(value))

                function variantName(val)
                {
                    if (val < 1)
                        return "m" + (-1*val)
                    else
                        return "f" + val
                }
            }

            TextSwitch
            {
                x: Theme.paddingLarge
                text: "Audio boost"
                checked: espeak.boost
                onCheckedChanged: espeak.boost = !espeak.boost
            }

        }
    }

}


