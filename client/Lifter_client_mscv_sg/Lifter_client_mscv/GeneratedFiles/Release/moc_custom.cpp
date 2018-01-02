/****************************************************************************
** Meta object code from reading C++ file 'custom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../custom/custom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'custom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_custom_t {
    QByteArrayData data[18];
    char stringdata0[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_custom_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_custom_t qt_meta_stringdata_custom = {
    {
QT_MOC_LITERAL(0, 0, 6), // "custom"
QT_MOC_LITERAL(1, 7, 22), // "deletselectItem_signal"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "row"
QT_MOC_LITERAL(4, 35, 10), // "sendresult"
QT_MOC_LITERAL(5, 46, 7), // "clicked"
QT_MOC_LITERAL(6, 54, 4), // "flag"
QT_MOC_LITERAL(7, 59, 14), // "clicked_delete"
QT_MOC_LITERAL(8, 74, 11), // "itemChanged"
QT_MOC_LITERAL(9, 86, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(10, 103, 4), // "item"
QT_MOC_LITERAL(11, 108, 20), // "itemSelectionChanged"
QT_MOC_LITERAL(12, 129, 17), // "currentRowChanged"
QT_MOC_LITERAL(13, 147, 20), // "deletselectItem_slot"
QT_MOC_LITERAL(14, 168, 12), // "dodeleteitme"
QT_MOC_LITERAL(15, 181, 11), // "editText_xc"
QT_MOC_LITERAL(16, 193, 3), // "str"
QT_MOC_LITERAL(17, 197, 14) // "button_clicked"

    },
    "custom\0deletselectItem_signal\0\0row\0"
    "sendresult\0clicked\0flag\0clicked_delete\0"
    "itemChanged\0QListWidgetItem*\0item\0"
    "itemSelectionChanged\0currentRowChanged\0"
    "deletselectItem_slot\0dodeleteitme\0"
    "editText_xc\0str\0button_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_custom[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    2,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   77,    2, 0x0a /* Public */,
       7,    1,   80,    2, 0x0a /* Public */,
       8,    1,   83,    2, 0x0a /* Public */,
      11,    0,   86,    2, 0x0a /* Public */,
      12,    1,   87,    2, 0x0a /* Public */,
      13,    1,   90,    2, 0x0a /* Public */,
      14,    0,   93,    2, 0x0a /* Public */,
      15,    1,   94,    2, 0x0a /* Public */,
      17,    1,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Bool,    6,

       0        // eod
};

void custom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        custom *_t = static_cast<custom *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deletselectItem_signal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sendresult((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->clicked_delete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->itemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->itemSelectionChanged(); break;
        case 6: _t->currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->deletselectItem_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->dodeleteitme(QPrivateSignal()); break;
        case 9: _t->editText_xc((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->button_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (custom::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&custom::deletselectItem_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (custom::*_t)(QStringList , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&custom::sendresult)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject custom::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_custom.data,
      qt_meta_data_custom,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *custom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *custom::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_custom.stringdata0))
        return static_cast<void*>(const_cast< custom*>(this));
    return QDialog::qt_metacast(_clname);
}

int custom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void custom::deletselectItem_signal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void custom::sendresult(QStringList _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
