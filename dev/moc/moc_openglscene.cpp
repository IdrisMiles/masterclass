/****************************************************************************
** Meta object code from reading C++ file 'openglscene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/openglscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'openglscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OpenGLScene_t {
    QByteArrayData data[14];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OpenGLScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OpenGLScene_t qt_meta_stringdata_OpenGLScene = {
    {
QT_MOC_LITERAL(0, 0, 11), // "OpenGLScene"
QT_MOC_LITERAL(1, 12, 9), // "ToggleSim"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 12), // "setXRotation"
QT_MOC_LITERAL(4, 36, 5), // "angle"
QT_MOC_LITERAL(5, 42, 12), // "setYRotation"
QT_MOC_LITERAL(6, 55, 12), // "setZRotation"
QT_MOC_LITERAL(7, 68, 15), // "setXTranslation"
QT_MOC_LITERAL(8, 84, 1), // "x"
QT_MOC_LITERAL(9, 86, 15), // "setYTranslation"
QT_MOC_LITERAL(10, 102, 1), // "y"
QT_MOC_LITERAL(11, 104, 15), // "setZTranslation"
QT_MOC_LITERAL(12, 120, 1), // "z"
QT_MOC_LITERAL(13, 122, 7) // "cleanup"

    },
    "OpenGLScene\0ToggleSim\0\0setXRotation\0"
    "angle\0setYRotation\0setZRotation\0"
    "setXTranslation\0x\0setYTranslation\0y\0"
    "setZTranslation\0z\0cleanup"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenGLScene[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    1,   55,    2, 0x0a /* Public */,
       5,    1,   58,    2, 0x0a /* Public */,
       6,    1,   61,    2, 0x0a /* Public */,
       7,    1,   64,    2, 0x0a /* Public */,
       9,    1,   67,    2, 0x0a /* Public */,
      11,    1,   70,    2, 0x0a /* Public */,
      13,    0,   73,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

void OpenGLScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OpenGLScene *_t = static_cast<OpenGLScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ToggleSim(); break;
        case 1: _t->setXRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setYRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setZRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setXTranslation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setYTranslation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setZTranslation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->cleanup(); break;
        default: ;
        }
    }
}

const QMetaObject OpenGLScene::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_OpenGLScene.data,
      qt_meta_data_OpenGLScene,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OpenGLScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGLScene::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGLScene.stringdata0))
        return static_cast<void*>(const_cast< OpenGLScene*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int OpenGLScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
