/*
    billybass, Billy Bass
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground
{
    CoverPlaceholder
    {
        text: "Billybass"
        icon.source: "/usr/share/icons/hicolor/86x86/apps/billybass.png"
    }

    CoverActionList
    {
        CoverAction
        {
            iconSource: coverActionLeftIcon
            onTriggered: coverActionLeft()
        }

        CoverAction
        {
            iconSource: coverActionRightIcon
            onTriggered: coverActionRight()
        }
    }
}


