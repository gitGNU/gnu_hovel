TEMPLATE = lib
CONFIG += plugin
QT += xml
TARGET = exporthtml
INCLUDEPATH += ../../lib \

CONFIG(debug, debug|release) {
	DESTDIR = ../../build/debug/plugins
	OBJECTS_DIR = ../../build/debug/plugins/.objs
	MOC_DIR = ../../build/debug/plugins/.moc
	RCC_DIR = ../../build/debug/plugins/.rcc
	UI_DIR = ../../build/debug/plugins/.ui

	LIBS += -L../../build/debug \
		-lhovel
} else {
	DESTDIR = ../../build/release/plugins
	OBJECTS_DIR = ../../build/release/plugins/.objs
	MOC_DIR = ../../build/release/plugins/.moc
	RCC_DIR = ../../build/release/plugins/.rcc
	UI_DIR = ../../build/release/plugins/.ui

	LIBS += -L../../build/release \
		-lhovel
}
