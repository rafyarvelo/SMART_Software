/****************************************************************************
** Meta object code from reading C++ file 'bci_c_bci_package.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "bci_c_bci_package.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bci_c_bci_package.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_C_BCI_Package_t {
    QByteArrayData data[9];
    char stringdata[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_C_BCI_Package_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_C_BCI_Package_t qt_meta_stringdata_C_BCI_Package = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 18),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 11),
QT_MOC_LITERAL(4, 46, 4),
QT_MOC_LITERAL(5, 51, 19),
QT_MOC_LITERAL(6, 71, 17),
QT_MOC_LITERAL(7, 89, 3),
QT_MOC_LITERAL(8, 93, 24)
    },
    "C_BCI_Package\0onEEGDataProcessed\0\0"
    "C_EEG_Data&\0data\0onRemoteCmdReceived\0"
    "PCC_Command_Type&\0cmd\0onEmergencyStopRequested\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_C_BCI_Package[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08,
       5,    1,   32,    2, 0x08,
       8,    0,   35,    2, 0x08,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

       0        // eod
};

void C_BCI_Package::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        C_BCI_Package *_t = static_cast<C_BCI_Package *>(_o);
        switch (_id) {
        case 0: _t->onEEGDataProcessed((*reinterpret_cast< C_EEG_Data(*)>(_a[1]))); break;
        case 1: _t->onRemoteCmdReceived((*reinterpret_cast< PCC_Command_Type(*)>(_a[1]))); break;
        case 2: _t->onEmergencyStopRequested(); break;
        default: ;
        }
    }
}

const QMetaObject C_BCI_Package::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_C_BCI_Package.data,
      qt_meta_data_C_BCI_Package,  qt_static_metacall, 0, 0}
};


const QMetaObject *C_BCI_Package::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *C_BCI_Package::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_C_BCI_Package.stringdata))
        return static_cast<void*>(const_cast< C_BCI_Package*>(this));
    return QObject::qt_metacast(_clname);
}

int C_BCI_Package::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
