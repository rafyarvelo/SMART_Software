/****************************************************************************
** Meta object code from reading C++ file 'bci_c_judgment_algorithm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BCI/bci_c_judgment_algorithm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bci_c_judgment_algorithm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_C_JudgmentAlgorithm_t {
    QByteArrayData data[11];
    char stringdata[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_C_JudgmentAlgorithm_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_C_JudgmentAlgorithm_t qt_meta_stringdata_C_JudgmentAlgorithm = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 12),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 17),
QT_MOC_LITERAL(4, 52, 3),
QT_MOC_LITERAL(5, 56, 6),
QT_MOC_LITERAL(6, 63, 6),
QT_MOC_LITERAL(7, 70, 4),
QT_MOC_LITERAL(8, 75, 5),
QT_MOC_LITERAL(9, 81, 11),
QT_MOC_LITERAL(10, 93, 8)
    },
    "C_JudgmentAlgorithm\0commandReady\0\0"
    "PCC_Command_Type&\0cmd\0SetRVS\0C_RVS*\0"
    "pRVS\0SetTM\0TM_Frame_t*\0pTMFrame\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_C_JudgmentAlgorithm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    1,   32,    2, 0x0a,
       8,    1,   35,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void C_JudgmentAlgorithm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        C_JudgmentAlgorithm *_t = static_cast<C_JudgmentAlgorithm *>(_o);
        switch (_id) {
        case 0: _t->commandReady((*reinterpret_cast< PCC_Command_Type(*)>(_a[1]))); break;
        case 1: _t->SetRVS((*reinterpret_cast< C_RVS*(*)>(_a[1]))); break;
        case 2: _t->SetTM((*reinterpret_cast< TM_Frame_t*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (C_JudgmentAlgorithm::*_t)(PCC_Command_Type & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&C_JudgmentAlgorithm::commandReady)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject C_JudgmentAlgorithm::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_C_JudgmentAlgorithm.data,
      qt_meta_data_C_JudgmentAlgorithm,  qt_static_metacall, 0, 0}
};


const QMetaObject *C_JudgmentAlgorithm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *C_JudgmentAlgorithm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_C_JudgmentAlgorithm.stringdata))
        return static_cast<void*>(const_cast< C_JudgmentAlgorithm*>(this));
    return QObject::qt_metacast(_clname);
}

int C_JudgmentAlgorithm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void C_JudgmentAlgorithm::commandReady(PCC_Command_Type & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
