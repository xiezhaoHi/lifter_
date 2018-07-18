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
    char stringdata0[622];
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
QT_MOC_LITERAL(11, 108, 18), // "show_ui_button_zdy"
QT_MOC_LITERAL(12, 127, 15), // "get_task_result"
QT_MOC_LITERAL(13, 143, 4), // "list"
QT_MOC_LITERAL(14, 148, 5), // "times"
QT_MOC_LITERAL(15, 154, 15), // "show_ui_CgqData"
QT_MOC_LITERAL(16, 170, 7), // "strList"
QT_MOC_LITERAL(17, 178, 18), // "QMap<int,QLabel*>&"
QT_MOC_LITERAL(18, 197, 3), // "map"
QT_MOC_LITERAL(19, 201, 15), // "show_ui_BmpData"
QT_MOC_LITERAL(20, 217, 6), // "strDir"
QT_MOC_LITERAL(21, 224, 3), // "iSd"
QT_MOC_LITERAL(22, 228, 3), // "iWz"
QT_MOC_LITERAL(23, 232, 4), // "zdjl"
QT_MOC_LITERAL(24, 237, 15), // "show_ui_JdqData"
QT_MOC_LITERAL(25, 253, 17), // "QMap<QString,int>"
QT_MOC_LITERAL(26, 271, 22), // "QMap<QString,QLabel*>&"
QT_MOC_LITERAL(27, 294, 6), // "map_id"
QT_MOC_LITERAL(28, 301, 11), // "map_id_aqcd"
QT_MOC_LITERAL(29, 313, 14), // "show_ui_YlData"
QT_MOC_LITERAL(30, 328, 5), // "strTd"
QT_MOC_LITERAL(31, 334, 7), // "strData"
QT_MOC_LITERAL(32, 342, 19), // "QStandardItemModel*"
QT_MOC_LITERAL(33, 362, 18), // "show_ui_ControlRes"
QT_MOC_LITERAL(34, 381, 3), // "ret"
QT_MOC_LITERAL(35, 385, 25), // "on_pushButton_zdy_clicked"
QT_MOC_LITERAL(36, 411, 24), // "on_pushButton_sz_clicked"
QT_MOC_LITERAL(37, 436, 7), // "checked"
QT_MOC_LITERAL(38, 444, 14), // "show_ui_dyData"
QT_MOC_LITERAL(39, 459, 8), // "QString&"
QT_MOC_LITERAL(40, 468, 9), // "QLabel***"
QT_MOC_LITERAL(41, 478, 25), // "on_pushButton_dySet_right"
QT_MOC_LITERAL(42, 504, 4), // "flag"
QT_MOC_LITERAL(43, 509, 24), // "on_pushButton_dySet_left"
QT_MOC_LITERAL(44, 534, 19), // "on_action_triggered"
QT_MOC_LITERAL(45, 554, 21), // "on_action_2_triggered"
QT_MOC_LITERAL(46, 576, 21), // "on_action_3_triggered"
QT_MOC_LITERAL(47, 598, 23) // "on_action_CAN_triggered"

    },
    "Lifter_client_mscv\0SetBmqConfig\0\0d\0p\0"
    "strLifter\0strBmqID\0SetTitle\0strName\0"
    "ChangeButtonState\0ShowOnlineStatus\0"
    "show_ui_button_zdy\0get_task_result\0"
    "list\0times\0show_ui_CgqData\0strList\0"
    "QMap<int,QLabel*>&\0map\0show_ui_BmpData\0"
    "strDir\0iSd\0iWz\0zdjl\0show_ui_JdqData\0"
    "QMap<QString,int>\0QMap<QString,QLabel*>&\0"
    "map_id\0map_id_aqcd\0show_ui_YlData\0"
    "strTd\0strData\0QStandardItemModel*\0"
    "show_ui_ControlRes\0ret\0on_pushButton_zdy_clicked\0"
    "on_pushButton_sz_clicked\0checked\0"
    "show_ui_dyData\0QString&\0QLabel***\0"
    "on_pushButton_dySet_right\0flag\0"
    "on_pushButton_dySet_left\0on_action_triggered\0"
    "on_action_2_triggered\0on_action_3_triggered\0"
    "on_action_CAN_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Lifter_client_mscv[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,  114,    2, 0x0a /* Public */,
       7,    1,  123,    2, 0x0a /* Public */,
       9,    0,  126,    2, 0x0a /* Public */,
      10,    0,  127,    2, 0x0a /* Public */,
      11,    0,  128,    2, 0x0a /* Public */,
      12,    2,  129,    2, 0x0a /* Public */,
      15,    2,  134,    2, 0x0a /* Public */,
      19,    5,  139,    2, 0x0a /* Public */,
      24,    3,  150,    2, 0x0a /* Public */,
      29,    3,  157,    2, 0x0a /* Public */,
      33,    1,  164,    2, 0x0a /* Public */,
      35,    0,  167,    2, 0x0a /* Public */,
      36,    1,  168,    2, 0x0a /* Public */,
      38,    5,  171,    2, 0x0a /* Public */,
      41,    1,  182,    2, 0x0a /* Public */,
      43,    1,  185,    2, 0x0a /* Public */,
      44,    0,  188,    2, 0x08 /* Private */,
      45,    0,  189,    2, 0x08 /* Private */,
      46,    0,  190,    2, 0x08 /* Private */,
      47,    0,  191,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Int,   13,   14,
    QMetaType::Void, QMetaType::QStringList, 0x80000000 | 17,   16,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::Double, QMetaType::Double, QMetaType::Double, 0x80000000 | 17,   20,   21,   22,   23,    2,
    QMetaType::Void, 0x80000000 | 25, 0x80000000 | 26, 0x80000000 | 26,   18,   27,   28,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 32,   30,   31,    2,
    QMetaType::Void, QMetaType::Int,   34,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   37,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 39, 0x80000000 | 39, 0x80000000 | 39, 0x80000000 | 40,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool,   42,
    QMetaType::Void, QMetaType::Bool,   42,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
        case 4: _t->show_ui_button_zdy(); break;
        case 5: _t->get_task_result((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->show_ui_CgqData((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< QMap<int,QLabel*>(*)>(_a[2]))); break;
        case 7: _t->show_ui_BmpData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< QMap<int,QLabel*>(*)>(_a[5]))); break;
        case 8: _t->show_ui_JdqData((*reinterpret_cast< QMap<QString,int>(*)>(_a[1])),(*reinterpret_cast< QMap<QString,QLabel*>(*)>(_a[2])),(*reinterpret_cast< QMap<QString,QLabel*>(*)>(_a[3]))); break;
        case 9: _t->show_ui_YlData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QStandardItemModel*(*)>(_a[3]))); break;
        case 10: _t->show_ui_ControlRes((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_pushButton_zdy_clicked(); break;
        case 12: _t->on_pushButton_sz_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->show_ui_dyData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QLabel***(*)>(_a[5]))); break;
        case 14: _t->on_pushButton_dySet_right((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->on_pushButton_dySet_left((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->on_action_triggered(); break;
        case 17: _t->on_action_2_triggered(); break;
        case 18: _t->on_action_3_triggered(); break;
        case 19: _t->on_action_CAN_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
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
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
struct qt_meta_stringdata_ShowWorker_t {
    QByteArrayData data[28];
    char stringdata0[292];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ShowWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ShowWorker_t qt_meta_stringdata_ShowWorker = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ShowWorker"
QT_MOC_LITERAL(1, 11, 11), // "showCgqData"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "strList"
QT_MOC_LITERAL(4, 32, 18), // "QMap<int,QLabel*>&"
QT_MOC_LITERAL(5, 51, 3), // "map"
QT_MOC_LITERAL(6, 55, 11), // "showBmpData"
QT_MOC_LITERAL(7, 67, 6), // "strDir"
QT_MOC_LITERAL(8, 74, 3), // "iSd"
QT_MOC_LITERAL(9, 78, 3), // "iWz"
QT_MOC_LITERAL(10, 82, 4), // "zdjl"
QT_MOC_LITERAL(11, 87, 11), // "showJdqData"
QT_MOC_LITERAL(12, 99, 17), // "QMap<QString,int>"
QT_MOC_LITERAL(13, 117, 22), // "QMap<QString,QLabel*>&"
QT_MOC_LITERAL(14, 140, 6), // "map_id"
QT_MOC_LITERAL(15, 147, 11), // "map_id_aqcd"
QT_MOC_LITERAL(16, 159, 10), // "showYlData"
QT_MOC_LITERAL(17, 170, 5), // "strTd"
QT_MOC_LITERAL(18, 176, 7), // "strData"
QT_MOC_LITERAL(19, 184, 19), // "QStandardItemModel*"
QT_MOC_LITERAL(20, 204, 14), // "showControlRes"
QT_MOC_LITERAL(21, 219, 3), // "ret"
QT_MOC_LITERAL(22, 223, 10), // "showdyData"
QT_MOC_LITERAL(23, 234, 8), // "QString&"
QT_MOC_LITERAL(24, 243, 9), // "QLabel***"
QT_MOC_LITERAL(25, 253, 8), // "consumer"
QT_MOC_LITERAL(26, 262, 14), // "analyseBmqData"
QT_MOC_LITERAL(27, 277, 14) // "analyseAllData"

    },
    "ShowWorker\0showCgqData\0\0strList\0"
    "QMap<int,QLabel*>&\0map\0showBmpData\0"
    "strDir\0iSd\0iWz\0zdjl\0showJdqData\0"
    "QMap<QString,int>\0QMap<QString,QLabel*>&\0"
    "map_id\0map_id_aqcd\0showYlData\0strTd\0"
    "strData\0QStandardItemModel*\0showControlRes\0"
    "ret\0showdyData\0QString&\0QLabel***\0"
    "consumer\0analyseBmqData\0analyseAllData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShowWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       6,    5,   64,    2, 0x06 /* Public */,
      11,    3,   75,    2, 0x06 /* Public */,
      16,    3,   82,    2, 0x06 /* Public */,
      20,    1,   89,    2, 0x06 /* Public */,
      22,    5,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      25,    0,  103,    2, 0x0a /* Public */,
      26,    0,  104,    2, 0x0a /* Public */,
      27,    0,  105,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Double, QMetaType::Double, QMetaType::Double, 0x80000000 | 4,    7,    8,    9,   10,    5,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 13, 0x80000000 | 13,    5,   14,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 19,   17,   18,    2,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 23, 0x80000000 | 23, 0x80000000 | 23, 0x80000000 | 24,    2,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ShowWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ShowWorker *_t = static_cast<ShowWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showCgqData((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< QMap<int,QLabel*>(*)>(_a[2]))); break;
        case 1: _t->showBmpData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< QMap<int,QLabel*>(*)>(_a[5]))); break;
        case 2: _t->showJdqData((*reinterpret_cast< QMap<QString,int>(*)>(_a[1])),(*reinterpret_cast< QMap<QString,QLabel*>(*)>(_a[2])),(*reinterpret_cast< QMap<QString,QLabel*>(*)>(_a[3]))); break;
        case 3: _t->showYlData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QStandardItemModel*(*)>(_a[3]))); break;
        case 4: _t->showControlRes((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->showdyData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QLabel***(*)>(_a[5]))); break;
        case 6: _t->consumer(); break;
        case 7: _t->analyseBmqData(); break;
        case 8: _t->analyseAllData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QStandardItemModel* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ShowWorker::*_t)(QStringList const & , QMap<int,QLabel*> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showCgqData)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(QString , double , double , double , QMap<int,QLabel*> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showBmpData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(QMap<QString,int> , QMap<QString,QLabel*> & , QMap<QString,QLabel*> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showJdqData)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(QString const & , QString const & , QStandardItemModel * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showYlData)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showControlRes)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(int , QString & , QString & , QString & , QLabel * * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showdyData)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject ShowWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ShowWorker.data,
      qt_meta_data_ShowWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ShowWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShowWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ShowWorker.stringdata0))
        return static_cast<void*>(const_cast< ShowWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int ShowWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ShowWorker::showCgqData(QStringList const & _t1, QMap<int,QLabel*> & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ShowWorker::showBmpData(QString _t1, double _t2, double _t3, double _t4, QMap<int,QLabel*> & _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ShowWorker::showJdqData(QMap<QString,int> _t1, QMap<QString,QLabel*> & _t2, QMap<QString,QLabel*> & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ShowWorker::showYlData(QString const & _t1, QString const & _t2, QStandardItemModel * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ShowWorker::showControlRes(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ShowWorker::showdyData(int _t1, QString & _t2, QString & _t3, QString & _t4, QLabel * * * _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
