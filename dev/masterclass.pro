
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = masterclass
DESTDIR = ./bin
TEMPLATE = app


SOURCES += $$PWD/src/*.cpp

HEADERS  += $$PWD/include/*.h \
            $$PWD/include/Visualisation/*.h \
            $$PWD/include/UserInterface/*.h \
            $$PWD/include/SpherePacking/*.h \
            $$PWD/include/Physics/*.h

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
UI_DIR += ./form
FORMS    += ./ui/mainwindow.ui
OTHER_FILES += ./shader/*

# include paths for boost and openvdb
INCLUDEPATH +=	./include                       \
                /home/idris/dev/include         \
                /home/idris/dev/include/bullet  \
                /usr/include/openvdb


# libs required for boost and openvdb
LIBS +=	-L/home/idris/dev/lib -lBulletDynamics -lBulletCollision -lLinearMath \
        -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -lopenvdb -lpthread -lHalf -ltbb \
        -lassimp \
        -lboost_filesystem -lboost_system \
        -L/usr/lib -lGL -lGLU -lGLEW

