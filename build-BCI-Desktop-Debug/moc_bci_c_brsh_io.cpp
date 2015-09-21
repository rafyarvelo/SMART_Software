/****************************************************************************
** Meta object code from reading C++ file 'bci_c_brsh_io.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BCI/bci_c_brsh_io.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bci_c_brsh_io.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_C_BRSH_IO_t {
    QByteArrayData data[13];
    char stringdata[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_C_BRSH_IO_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_C_BRSH_IO_t qt_meta_stringdata_C_BRSH_IO = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 16),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 12),
QT_MOC_LITERAL(4, 41, 8),
QT_MOC_LITERAL(5, 50, 21),
QT_MOC_LITERAL(6, 72, 17),
QT_MOC_LITERAL(7, 90, 3),
QT_MOC_LITERAL(8, 94, 22),
QT_MOC_LITERAL(9, 117, 13),
QT_MOC_LITERAL(10, 131, 11),
QT_MOC_LITERAL(11, 143, 11),
QT_MOC_LITERAL(12, 155, 6)
    },
    "C_BRSH_IO\0BRSFrameReceived\0\0BRS_Frame_t*\0"
    "brsFrame\0remoteCommandReceived\0"
    "PCC_Command_Type&\0cmd\0EmergencyStopRequested\0"
    "fetchBRSFrame\0SendTMFrame\0TM_Frame_t*\0"
    "pFrame\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_C_BRSH_IO[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06,
       5,    1,   42,    2, 0x06,
       8,    0,   45,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       9,    0,   46,    2, 0x0a,
      10,    1,   47,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void C_BRSH_IO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        C_BRSH_IO *_t = static_cast<C_BRSH_IO *>(_o);
        switch (_id) {
        case 0: _t->BRSFrameReceived((*reinterpret_cast< BRS_Frame_t*(*)>(_a[1]))); break;
        case 1: _t->remoteCommandReceived((*reinterpret_cast< PCC_Command_Type(*)>(_a[1]))); break;
        case 2: _t->EmergencyStopRequested(); break;
        case 3: { bool _r = _t->fetchBRSFrame();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->SendTMFrame((*reinterpret_cast< TM_Frame_t*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (C_BRSH_IO::*_t)(BRS_Frame_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&C_BRSH_IO::BRSFrameReceived)) {
                *result = 0;
            }
        }
        {
            typedef void (C_BRSH_IO::*_t)(PCC_Command_Type & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&C_BRSH_IO::remoteCommandReceived)) {
                *result = 1;
            }
        }
        {
            typedef void (C_BRSH_IO::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&C_BRSH_IO::EmergencyStopRequested)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject C_BRSH_IO::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_C_BRSH_IO.data,
      qt_meta_data_C_BRSH_IO,  qt_static_metacall, 0, 0}
};


const QMetaObject *C_BRSH_IO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *C_BRSH_IO::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_C_BRSH_IO.stringdata))
        return static_cast<void*>(const_cast< C_BRSH_IO*>(this));
    if (!strcmp(_clname, "C_ConnectedDevice"))
        return static_cast< C_ConnectedDevice*>(const_cast< C_BRSH_IO*>(this));
    return QObject::qt_metacast(_clname);
}

int C_BRSH_IO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void C_BRSH_IO::BRSFrameReceived(BRS_Frame_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void C_BRSH_IO::remoteCommandReceived(PCC_Command_Type & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void C_BRSH_IO::EmergencyStopRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
