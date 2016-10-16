
QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = masterclass
DESTDIR = ./bin
TEMPLATE = app


SOURCES += $$PWD/src/*.cpp

HEADERS  += $$PWD/include/*.h

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
UI_DIR += ./form
FORMS    += ./ui/mainwindow.ui
OTHER_FILES += ./shader/*

# include paths for boost and openvdb
INCLUDEPATH +=	./include                       \
                /home/idris/dev/include         \
                /home/idris/dev/include/bullet  \
                /usr/include/openvdb            \
                /usr/local/include/OpenEXR      \
                /usr/local/include/Alembic


# libs required for boost and openvdb
LIBS +=	-L/home/idris/dev/lib -lBulletDynamics -lBulletCollision -lLinearMath \
        -L/usr/local/lib -L/usr/lib/x86_64/ -lopenvdb -lpthread -lHalf -ltbb \
        -lHalf -lAlembic -lImath -lIex -lIexMath -lIlmThread -pthread -lhdf5_serial \
        -L/usr/lib -lGL -lGLU -lGLEW

