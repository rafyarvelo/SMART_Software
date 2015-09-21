/****************************************************************************
** Meta object code from reading C++ file 'bci_c_telemetrymanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BCI/bci_c_telemetrymanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bci_c_telemetrymanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_C_TelemetryManager_t {
    QByteArrayData data[7];
    char stringdata[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_C_TelemetryManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_C_TelemetryManager_t qt_meta_stringdata_C_TelemetryManager = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 14),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 11),
QT_MOC_LITERAL(4, 47, 5),
QT_MOC_LITERAL(5, 53, 18),
QT_MOC_LITERAL(6, 72, 12)
    },
    "C_TelemetryManager\0tmFrameCreated\0\0"
    "TM_Frame_t*\0frame\0onBRSFrameReceived\0"
    "BRS_Frame_t*\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_C_TelemetryManager[] = {

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
       1,    1,   24,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    1,   27,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    4,

       0        // eod
};

void C_TelemetryManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        C_TelemetryManager *_t = static_cast<C_TelemetryManager *>(_o);
        switch (_id) {
        case 0: _t->tmFrameCreated((*reinterpret_cast< TM_Frame_t*(*)>(_a[1]))); break;
        case 1: _t->onBRSFrameReceived((*reinterpret_cast< BRS_Frame_t*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (C_TelemetryManager::*_t)(TM_Frame_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&C_TelemetryManager::tmFrameCreated)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject C_TelemetryManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_C_TelemetryManager.data,
      qt_meta_data_C_TelemetryManager,  qt_static_metacall, 0, 0}
};


const QMetaObject *C_TelemetryManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *C_TelemetryManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_C_TelemetryManager.stringdata))
        return static_cast<void*>(const_cast< C_TelemetryManager*>(this));
    return QObject::qt_metacast(_clname);
}

int C_TelemetryManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void C_TelemetryManager::tmFrameCreated(TM_Frame_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
