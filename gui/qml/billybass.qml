/*
    billybass, Billy Bass
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.billybass.espeak 1.0

ApplicationWindow
{
    id: billybass

    property string coverActionLeftIcon: espeak.stfu ? "image://theme/icon-cover-play" : "image://theme/icon-cover-pause"
    property string coverActionRightIcon: "image://theme/icon-cover-refresh"

    initialPage: Qt.resolvedUrl("pages/Billybass.qml")
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    function coverActionLeft()
    {
        console.log("Left cover action")
        espeak.stfu = !espeak.stfu
    }

    function coverActionRight()
    {
        console.log("Right cover action")
        espeak.replay()
    }

    Espeak
    {
        id: espeak
        Component.onCompleted: espeak.synth("billy")
    }

    ListModel
    {
        id: languages
    }

}


