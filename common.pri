CONFIG += warn_on

CONFIG(debug, debug|release) {
	DESTDIR = ../build/debug
	OBJECTS_DIR = ../build/debug/.objs
	MOC_DIR = ../build/debug/.moc
	RCC_DIR = ../build/debug/.rcc
	UI_DIR = ../build/debug/.ui
} else {
	DESTDIR = ../build/release
	OBJECTS_DIR = ../build/release/.objs
	MOC_DIR = ../build/release/.moc
	RCC_DIR = ../build/release/.rcc
	UI_DIR = ../build/release/.ui
}
