/****************************************************************************
** Meta object code from reading C++ file 'bci_c_eeg_io_debug.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "bci_c_eeg_io_debug.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bci_c_eeg_io_debug.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_C_EEG_IO_DEBUG_t {
    QByteArrayData data[3];
    char stringdata[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_C_EEG_IO_DEBUG_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_C_EEG_IO_DEBUG_t qt_meta_stringdata_C_EEG_IO_DEBUG = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 13),
QT_MOC_LITERAL(2, 29, 0)
    },
    "C_EEG_IO_DEBUG\0fetchEEGFrame\0\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_C_EEG_IO_DEBUG[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a,

 // slots: parameters
    QMetaType::Bool,

       0        // eod
};

void C_EEG_IO_DEBUG::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        C_EEG_IO_DEBUG *_t = static_cast<C_EEG_IO_DEBUG *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->fetchEEGFrame();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject C_EEG_IO_DEBUG::staticMetaObject = {
    { &C_EEG_IO::staticMetaObject, qt_meta_stringdata_C_EEG_IO_DEBUG.data,
      qt_meta_data_C_EEG_IO_DEBUG,  qt_static_metacall, 0, 0}
};


const QMetaObject *C_EEG_IO_DEBUG::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *C_EEG_IO_DEBUG::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_C_EEG_IO_DEBUG.stringdata))
        return static_cast<void*>(const_cast< C_EEG_IO_DEBUG*>(this));
    if (!strcmp(_clname, "C_Singleton<C_EEG_IO_DEBUG>"))
        return static_cast< C_Singleton<C_EEG_IO_DEBUG>*>(const_cast< C_EEG_IO_DEBUG*>(this));
    return C_EEG_IO::qt_metacast(_clname);
}

int C_EEG_IO_DEBUG::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = C_EEG_IO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
