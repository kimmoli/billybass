/*
    billybass, Billy Bass
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog
{
    id: languagedialog
    canAccept: false

    property string newLanguage : ""

    SilicaListView
    {
        id: listView
        anchors.fill: parent
        model: languages

        header: PageHeader { title: "Languages" }

        VerticalScrollDecorator {}

        delegate: ListItem
        {
            id: listItem

            onClicked:
            {
                newLanguage = voicename
                languagedialog.canAccept = true
                languagedialog.accept()
            }

            Label
            {
                x: Theme.paddingLarge
                text: voicename + "  (" + language + ")"
                anchors.verticalCenter: parent.verticalCenter
                font.capitalization: Font.Capitalize
                color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

        }
    }
}
