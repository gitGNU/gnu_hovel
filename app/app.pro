# Copyright (C) 2011 Jonathan Crowe.
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
TARGET = hovel
INCLUDEPATH += ../lib \

!include( ../common.pri ):error( "Couldn't find common.pri" )

CONFIG(debug, debug|release) {
	LIBS += -L../build/debug -lhovel
}
else {
	LIBS += -L../build/release \
		-lhovel
}

HEADERS +=	\
			formattingtoolbar.h \
			mainwindow.h \
			multilineitemdelegate.h \
			projecttreeview.h \
			propertiesproxymodel.h \
			propertiesview.h \
			statuscomboboxitemdelegate.h \
			stringcomboboxitemdelegate.h \
			textedit.h \
			utilities.h

SOURCES +=	main.cpp \
			formattingtoolbar.cpp \
			mainwindow.cpp \
			multilineitemdelegate.cpp \
			projecttreeview.cpp \
			propertiesproxymodel.cpp \
			propertiesview.cpp \
			statuscomboboxitemdelegate.cpp \
			stringcomboboxitemdelegate.cpp \
			textedit.cpp \
			utilities.cpp

RESOURCES += hovel.qrc

OTHER_FILES +=	styles/default.qss \
				styles/scene_fullscreen.qss
FORMS +=
