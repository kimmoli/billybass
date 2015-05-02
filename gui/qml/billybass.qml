/*
    billybass, Billy Bass
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import billybass.Espeak 1.0

ApplicationWindow
{
    id: billybass

    property string libespeakVersion: "unknown"

    property string coverActionLeftIcon: "image://theme/icon-cover-pause"
    property string coverActionRightIcon: "image://theme/icon-cover-play"

    initialPage: Qt.resolvedUrl("pages/Billybass.qml")
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    function coverActionLeft()
    {
        console.log("Left cover action")
    }

    function coverActionRight()
    {
        console.log("Right cover action")
    }

    Espeak
    {
        id: espeak
    }



}


