TEMPLATE = subdirs

SUBDIRS = espeak gui

OTHER_FILES = \
    rpm/billybass.spec \
    common.pri

gui.depends = espeak
