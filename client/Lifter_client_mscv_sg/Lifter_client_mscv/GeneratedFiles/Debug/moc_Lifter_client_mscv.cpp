/****************************************************************************
** Meta object code from reading C++ file 'Lifter_client_mscv.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Lifter_client_mscv.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Lifter_client_mscv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TaskThread_t {
    QByteArrayData data[3];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TaskThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TaskThread_t qt_meta_stringdata_TaskThread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TaskThread"
QT_MOC_LITERAL(1, 11, 15), // "show_button_zdy"
QT_MOC_LITERAL(2, 27, 0) // ""

    },
    "TaskThread\0show_button_zdy\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TaskThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void TaskThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TaskThread *_t = static_cast<TaskThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->show_button_zdy(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TaskThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TaskThread::show_button_zdy)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject TaskThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TaskThread.data,
      qt_meta_data_TaskThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TaskThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaskThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TaskThread.stringdata0))
        return static_cast<void*>(const_cast< TaskThread*>(this));
    return QThread::qt_metacast(_clname);
}

int TaskThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void TaskThread::show_button_zdy()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Lifter_client_mscv_t {
    QByteArrayData data[48];
    char stringdata0[753];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Lifter_client_mscv_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Lifter_client_mscv_t qt_meta_stringdata_Lifter_client_mscv = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Lifter_client_mscv"
QT_MOC_LITERAL(1, 19, 12), // "SetBmqConfig"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 1), // "d"
QT_MOC_LITERAL(4, 35, 1), // "p"
QT_MOC_LITERAL(5, 37, 9), // "strLifter"
QT_MOC_LITERAL(6, 47, 8), // "strBmqID"
QT_MOC_LITERAL(7, 56, 8), // "SetTitle"
QT_MOC_LITERAL(8, 65, 7), // "strName"
QT_MOC_LITERAL(9, 73, 17), // "ChangeButtonState"
QT_MOC_LITERAL(10, 91, 16), // "ShowOnlineStatus"
QT_MOC_LITERAL(11, 108, 20), // "ChangeButtonStateJDQ"
QT_MOC_LITERAL(12, 129, 18), // "show_ui_button_zdy"
QT_MOC_LITERAL(13, 148, 15), // "get_task_result"
QT_MOC_LITERAL(14, 164, 15), // "show_ui_CgqData"
QT_MOC_LITERAL(15, 180, 18), // "QMap<int,QLabel*>*"
QT_MOC_LITERAL(16, 199, 15), // "show_ui_BmpData"
QT_MOC_LITERAL(17, 215, 9), // "BmqDataS*"
QT_MOC_LITERAL(18, 225, 7), // "bmqData"
QT_MOC_LITERAL(19, 233, 15), // "show_ui_JdqData"
QT_MOC_LITERAL(20, 249, 17), // "QMap<QString,int>"
QT_MOC_LITERAL(21, 267, 22), // "QMap<QString,QLabel*>*"
QT_MOC_LITERAL(22, 290, 12), // "QPushButton*"
QT_MOC_LITERAL(23, 303, 17), // "show_ui_JdqDataDO"
QT_MOC_LITERAL(24, 321, 27), // "QMap<QPushButton*,QString>*"
QT_MOC_LITERAL(25, 349, 32), // "QMap<QPushButton*,BtAttribute*>*"
QT_MOC_LITERAL(26, 382, 14), // "show_ui_YlData"
QT_MOC_LITERAL(27, 397, 19), // "QStandardItemModel*"
QT_MOC_LITERAL(28, 417, 18), // "show_ui_ControlRes"
QT_MOC_LITERAL(29, 436, 25), // "on_pushButton_zdy_clicked"
QT_MOC_LITERAL(30, 462, 14), // "show_ui_dyData"
QT_MOC_LITERAL(31, 477, 9), // "QLabel***"
QT_MOC_LITERAL(32, 487, 25), // "on_pushButton_dySet_right"
QT_MOC_LITERAL(33, 513, 24), // "on_pushButton_dySet_left"
QT_MOC_LITERAL(34, 538, 19), // "on_action_triggered"
QT_MOC_LITERAL(35, 558, 21), // "on_action_2_triggered"
QT_MOC_LITERAL(36, 580, 21), // "on_action_3_triggered"
QT_MOC_LITERAL(37, 602, 23), // "on_action_CAN_triggered"
QT_MOC_LITERAL(38, 626, 21), // "on_action_zhouqiRenwu"
QT_MOC_LITERAL(39, 648, 4), // "flag"
QT_MOC_LITERAL(40, 653, 12), // "show_ui_test"
QT_MOC_LITERAL(41, 666, 3), // "map"
QT_MOC_LITERAL(42, 670, 21), // "QMap<QString,QLabel*>"
QT_MOC_LITERAL(43, 692, 6), // "map_id"
QT_MOC_LITERAL(44, 699, 11), // "map_id_aqcd"
QT_MOC_LITERAL(45, 711, 13), // "show_ui_test2"
QT_MOC_LITERAL(46, 725, 13), // "show_ui_test3"
QT_MOC_LITERAL(47, 739, 13) // "show_ui_test4"

    },
    "Lifter_client_mscv\0SetBmqConfig\0\0d\0p\0"
    "strLifter\0strBmqID\0SetTitle\0strName\0"
    "ChangeButtonState\0ShowOnlineStatus\0"
    "ChangeButtonStateJDQ\0show_ui_button_zdy\0"
    "get_task_result\0show_ui_CgqData\0"
    "QMap<int,QLabel*>*\0show_ui_BmpData\0"
    "BmqDataS*\0bmqData\0show_ui_JdqData\0"
    "QMap<QString,int>\0QMap<QString,QLabel*>*\0"
    "QPushButton*\0show_ui_JdqDataDO\0"
    "QMap<QPushButton*,QString>*\0"
    "QMap<QPushButton*,BtAttribute*>*\0"
    "show_ui_YlData\0QStandardItemModel*\0"
    "show_ui_ControlRes\0on_pushButton_zdy_clicked\0"
    "show_ui_dyData\0QLabel***\0"
    "on_pushButton_dySet_right\0"
    "on_pushButton_dySet_left\0on_action_triggered\0"
    "on_action_2_triggered\0on_action_3_triggered\0"
    "on_action_CAN_triggered\0on_action_zhouqiRenwu\0"
    "flag\0show_ui_test\0map\0QMap<QString,QLabel*>\0"
    "map_id\0map_id_aqcd\0show_ui_test2\0"
    "show_ui_test3\0show_ui_test4"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Lifter_client_mscv[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,  144,    2, 0x0a /* Public */,
       7,    1,  153,    2, 0x0a /* Public */,
       9,    0,  156,    2, 0x0a /* Public */,
      10,    0,  157,    2, 0x0a /* Public */,
      11,    0,  158,    2, 0x0a /* Public */,
      12,    0,  159,    2, 0x0a /* Public */,
      13,    2,  160,    2, 0x0a /* Public */,
      14,    2,  165,    2, 0x0a /* Public */,
      16,    2,  170,    2, 0x0a /* Public */,
      19,    4,  175,    2, 0x0a /* Public */,
      23,    3,  184,    2, 0x0a /* Public */,
      26,    3,  191,    2, 0x0a /* Public */,
      28,    1,  198,    2, 0x0a /* Public */,
      29,    0,  201,    2, 0x0a /* Public */,
      30,    5,  202,    2, 0x0a /* Public */,
      32,    1,  213,    2, 0x0a /* Public */,
      33,    1,  216,    2, 0x0a /* Public */,
      34,    0,  219,    2, 0x08 /* Private */,
      35,    0,  220,    2, 0x08 /* Private */,
      36,    0,  221,    2, 0x08 /* Private */,
      37,    0,  222,    2, 0x08 /* Private */,
      38,    1,  223,    2, 0x08 /* Private */,
      40,    3,  226,    2, 0x08 /* Private */,
      45,    1,  233,    2, 0x08 /* Private */,
      46,    1,  236,    2, 0x08 /* Private */,
      47,    0,  239,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QStringList, 0x80000000 | 15,    2,    2,
    QMetaType::Void, 0x80000000 | 17, 0x80000000 | 15,   18,    2,
    QMetaType::Void, 0x80000000 | 20, 0x80000000 | 21, 0x80000000 | 21, 0x80000000 | 22,    2,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 20, 0x80000000 | 24, 0x80000000 | 25,    2,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 27,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 31,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   39,
    QMetaType::Void, 0x80000000 | 20, 0x80000000 | 42, 0x80000000 | 42,   41,   43,   44,
    QMetaType::Void, 0x80000000 | 21,   44,
    QMetaType::Void, 0x80000000 | 20,   41,
    QMetaType::Void,

       0        // eod
};

void Lifter_client_mscv::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Lifter_client_mscv *_t = static_cast<Lifter_client_mscv *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetBmqConfig((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 1: _t->SetTitle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->ChangeButtonState(); break;
        case 3: _t->ShowOnlineStatus(); break;
        case 4: _t->ChangeButtonStateJDQ(); break;
        case 5: _t->show_ui_button_zdy(); break;
        case 6: _t->get_task_result((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->show_ui_CgqData((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< QMap<int,QLabel*>*(*)>(_a[2]))); break;
        case 8: _t->show_ui_BmpData((*reinterpret_cast< BmqDataS*(*)>(_a[1])),(*reinterpret_cast< QMap<int,QLabel*>*(*)>(_a[2]))); break;
        case 9: _t->show_ui_JdqData((*reinterpret_cast< QMap<QString,int>(*)>(_a[1])),(*reinterpret_cast< QMap<QString,QLabel*>*(*)>(_a[2])),(*reinterpret_cast< QMap<QString,QLabel*>*(*)>(_a[3])),(*reinterpret_cast< QPushButton*(*)>(_a[4]))); break;
        case 10: _t->show_ui_JdqDataDO((*reinterpret_cast< QMap<QString,int>(*)>(_a[1])),(*reinterpret_cast< QMap<QPushButton*,QString>*(*)>(_a[2])),(*reinterpret_cast< QMap<QPushButton*,BtAttribute*>*(*)>(_a[3]))); break;
        case 11: _t->show_ui_YlData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QStandardItemModel*(*)>(_a[3]))); break;
        case 12: _t->show_ui_ControlRes((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_pushButton_zdy_clicked(); break;
        case 14: _t->show_ui_dyData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QLabel***(*)>(_a[5]))); break;
        case 15: _t->on_pushButton_dySet_right((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->on_pushButton_dySet_left((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->on_action_triggered(); break;
        case 18: _t->on_action_2_triggered(); break;
        case 19: _t->on_action_3_triggered(); break;
        case 20: _t->on_action_CAN_triggered(); break;
        case 21: _t->on_action_zhouqiRenwu((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->show_ui_test((*reinterpret_cast< QMap<QString,int>(*)>(_a[1])),(*reinterpret_cast< QMap<QString,QLabel*>(*)>(_a[2])),(*reinterpret_cast< QMap<QString,QLabel*>(*)>(_a[3]))); break;
        case 23: _t->show_ui_test2((*reinterpret_cast< QMap<QString,QLabel*>*(*)>(_a[1]))); break;
        case 24: _t->show_ui_test3((*reinterpret_cast< QMap<QString,int>(*)>(_a[1]))); break;
        case 25: _t->show_ui_test4(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QPushButton* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QStandardItemModel* >(); break;
            }
            break;
        }
    }
}

const QMetaObject Lifter_client_mscv::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Lifter_client_mscv.data,
      qt_meta_data_Lifter_client_mscv,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Lifter_client_mscv::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Lifter_client_mscv::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Lifter_client_mscv.stringdata0))
        return static_cast<void*>(const_cast< Lifter_client_mscv*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Lifter_client_mscv::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
