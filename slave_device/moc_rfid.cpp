/****************************************************************************
** Meta object code from reading C++ file 'rfid.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "rfid.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rfid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RFID_t {
    QByteArrayData data[5];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RFID_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RFID_t qt_meta_stringdata_RFID = {
    {
QT_MOC_LITERAL(0, 0, 4), // "RFID"
QT_MOC_LITERAL(1, 5, 18), // "recognitionSuccess"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 2), // "id"
QT_MOC_LITERAL(4, 28, 17) // "recognitionFailed"

    },
    "RFID\0recognitionSuccess\0\0id\0"
    "recognitionFailed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RFID[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    0,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt,    3,
    QMetaType::Void,

       0        // eod
};

void RFID::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RFID *_t = static_cast<RFID *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recognitionSuccess((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: _t->recognitionFailed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RFID::*_t)(unsigned int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RFID::recognitionSuccess)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RFID::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RFID::recognitionFailed)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject RFID::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RFID.data,
      qt_meta_data_RFID,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RFID::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RFID::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RFID.stringdata0))
        return static_cast<void*>(const_cast< RFID*>(this));
    return QObject::qt_metacast(_clname);
}

int RFID::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void RFID::recognitionSuccess(unsigned int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RFID::recognitionFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
