/****************************************************************************
** Meta object code from reading C++ file 'physicsbodyproperties.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/UserInterface/physicsbodyproperties.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'physicsbodyproperties.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PhysicsBodyPropertiesWidget_t {
    QByteArrayData data[4];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PhysicsBodyPropertiesWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PhysicsBodyPropertiesWidget_t qt_meta_stringdata_PhysicsBodyPropertiesWidget = {
    {
QT_MOC_LITERAL(0, 0, 27), // "PhysicsBodyPropertiesWidget"
QT_MOC_LITERAL(1, 28, 28), // "PhysicsBodyPropertiesUpdated"
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 27) // "UpdatePhysicsBodyProperties"

    },
    "PhysicsBodyPropertiesWidget\0"
    "PhysicsBodyPropertiesUpdated\0\0"
    "UpdatePhysicsBodyProperties"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PhysicsBodyPropertiesWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void PhysicsBodyPropertiesWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PhysicsBodyPropertiesWidget *_t = static_cast<PhysicsBodyPropertiesWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->PhysicsBodyPropertiesUpdated(); break;
        case 1: _t->UpdatePhysicsBodyProperties(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PhysicsBodyPropertiesWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PhysicsBodyPropertiesWidget::PhysicsBodyPropertiesUpdated)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject PhysicsBodyPropertiesWidget::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_PhysicsBodyPropertiesWidget.data,
      qt_meta_data_PhysicsBodyPropertiesWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PhysicsBodyPropertiesWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PhysicsBodyPropertiesWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PhysicsBodyPropertiesWidget.stringdata0))
        return static_cast<void*>(const_cast< PhysicsBodyPropertiesWidget*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int PhysicsBodyPropertiesWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PhysicsBodyPropertiesWidget::PhysicsBodyPropertiesUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
