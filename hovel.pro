# Copyright (C) 2010 Jonathan Crowe.
# This file is part of Hovel.
# Hovel is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Hovel is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with Hovel.  If not, see <http://www.gnu.org/licenses/>.
TEMPLATE = app
QT += xml
SOURCES += main.cpp \
    mainwindow.cpp \
    hovelmodel.cpp \
    projecttreeview.cpp \
    folderitem.cpp \
    textitem.cpp \
    projectitem.cpp \
    bookitem.cpp \
    hovelitem.cpp \
    propertiesview.cpp \
    propertiesproxymodel.cpp \
    chapteritem.cpp \
    textedit.cpp \
    export.cpp \
    utilities.cpp \
    statuscomboboxitemdelegate.cpp \
    hovelitemmimedata.cpp
HEADERS += mainwindow.h \
    hovelitem.h \
    hovelmodel.h \
    projecttreeview.h \
    serialise.h \
    folderitem.h \
    textitem.h \
    projectitem.h \
    bookitem.h \
    propertiesview.h \
    propertiesproxymodel.h \
    chapteritem.h \
    textedit.h \
    export.h \
    utilities.h \
    statuscomboboxitemdelegate.h \
    hovelitemmimedata.h
RESOURCES += hovel.qrc
OTHER_FILES += styles/default.qss \
    styles/scene_fullscreen.qss
