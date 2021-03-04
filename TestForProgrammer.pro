TEMPLATE = app 
TARGET = TestForProgrammer

QT += core \
      gui \
      widgets \
      charts
    
FORMS += \
    ui/datatableform.ui \
    ui/mainwindow.ui
HEADERS += \
    src/chart.h \
    src/mainwindow.h \
    src/datatableform.h \
    src/parameters.h
SOURCES += src/main.cpp \
    src/chart.cpp \
    src/mainwindow.cpp \
    src/datatableform.cpp \
    src/parameters.cpp

UI_DIR = ui_src
DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj

RESOURCES += 
OTHER_FILES += 

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare# -Wno-attributes
