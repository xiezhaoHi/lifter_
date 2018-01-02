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
    QByteArrayData data[37];
    char stringdata0[461];
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
QT_MOC_LITERAL(10, 91, 18), // "show_ui_button_zdy"
QT_MOC_LITERAL(11, 110, 15), // "get_task_result"
QT_MOC_LITERAL(12, 126, 4), // "list"
QT_MOC_LITERAL(13, 131, 5), // "times"
QT_MOC_LITERAL(14, 137, 15), // "show_ui_CgqData"
QT_MOC_LITERAL(15, 153, 7), // "strList"
QT_MOC_LITERAL(16, 161, 15), // "show_ui_BmpData"
QT_MOC_LITERAL(17, 177, 6), // "strDir"
QT_MOC_LITERAL(18, 184, 3), // "iSd"
QT_MOC_LITERAL(19, 188, 3), // "iWz"
QT_MOC_LITERAL(20, 192, 4), // "zdjl"
QT_MOC_LITERAL(21, 197, 15), // "show_ui_JdqData"
QT_MOC_LITERAL(22, 213, 17), // "QMap<QString,int>"
QT_MOC_LITERAL(23, 231, 3), // "map"
QT_MOC_LITERAL(24, 235, 14), // "show_ui_YlData"
QT_MOC_LITERAL(25, 250, 5), // "strTd"
QT_MOC_LITERAL(26, 256, 7), // "strData"
QT_MOC_LITERAL(27, 264, 25), // "on_pushButton_zdy_clicked"
QT_MOC_LITERAL(28, 290, 24), // "on_pushButton_sz_clicked"
QT_MOC_LITERAL(29, 315, 7), // "checked"
QT_MOC_LITERAL(30, 323, 21), // "create_choose_acction"
QT_MOC_LITERAL(31, 345, 19), // "on_action_triggered"
QT_MOC_LITERAL(32, 365, 21), // "on_action_2_triggered"
QT_MOC_LITERAL(33, 387, 21), // "on_action_3_triggered"
QT_MOC_LITERAL(34, 409, 23), // "on_action_CAN_triggered"
QT_MOC_LITERAL(35, 433, 13), // "on_choose_one"
QT_MOC_LITERAL(36, 447, 13) // "on_choose_two"

    },
    "Lifter_client_mscv\0SetBmqConfig\0\0d\0p\0"
    "strLifter\0strBmqID\0SetTitle\0strName\0"
    "ChangeButtonState\0show_ui_button_zdy\0"
    "get_task_result\0list\0times\0show_ui_CgqData\0"
    "strList\0show_ui_BmpData\0strDir\0iSd\0"
    "iWz\0zdjl\0show_ui_JdqData\0QMap<QString,int>\0"
    "map\0show_ui_YlData\0strTd\0strData\0"
    "on_pushButton_zdy_clicked\0"
    "on_pushButton_sz_clicked\0checked\0"
    "create_choose_acction\0on_action_triggered\0"
    "on_action_2_triggered\0on_action_3_triggered\0"
    "on_action_CAN_triggered\0on_choose_one\0"
    "on_choose_two"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Lifter_client_mscv[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,  104,    2, 0x0a /* Public */,
       7,    1,  113,    2, 0x0a /* Public */,
       9,    0,  116,    2, 0x0a /* Public */,
      10,    0,  117,    2, 0x0a /* Public */,
      11,    2,  118,    2, 0x0a /* Public */,
      14,    1,  123,    2, 0x0a /* Public */,
      16,    4,  126,    2, 0x0a /* Public */,
      21,    1,  135,    2, 0x0a /* Public */,
      24,    2,  138,    2, 0x0a /* Public */,
      27,    0,  143,    2, 0x0a /* Public */,
      28,    1,  144,    2, 0x0a /* Public */,
      30,    0,  147,    2, 0x0a /* Public */,
      31,    0,  148,    2, 0x08 /* Private */,
      32,    0,  149,    2, 0x08 /* Private */,
      33,    0,  150,    2, 0x08 /* Private */,
      34,    0,  151,    2, 0x08 /* Private */,
      35,    0,  152,    2, 0x08 /* Private */,
      36,    0,  153,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Int,   12,   13,
    QMetaType::Void, QMetaType::QStringList,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::Double, QMetaType::Double, QMetaType::Double,   17,   18,   19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   25,   26,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
        case 3: _t->show_ui_button_zdy(); break;
        case 4: _t->get_task_result((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->show_ui_CgqData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 6: _t->show_ui_BmpData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 7: _t->show_ui_JdqData((*reinterpret_cast< QMap<QString,int>(*)>(_a[1]))); break;
        case 8: _t->show_ui_YlData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->on_pushButton_zdy_clicked(); break;
        case 10: _t->on_pushButton_sz_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->create_choose_acction(); break;
        case 12: _t->on_action_triggered(); break;
        case 13: _t->on_action_2_triggered(); break;
        case 14: _t->on_action_3_triggered(); break;
        case 15: _t->on_action_CAN_triggered(); break;
        case 16: _t->on_choose_one(); break;
        case 17: _t->on_choose_two(); break;
        default: ;
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
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
struct qt_meta_stringdata_ShowWorker_t {
    QByteArrayData data[18];
    char stringdata0[162];
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
QT_MOC_LITERAL(4, 32, 11), // "showBmpData"
QT_MOC_LITERAL(5, 44, 6), // "strDir"
QT_MOC_LITERAL(6, 51, 3), // "iSd"
QT_MOC_LITERAL(7, 55, 3), // "iWz"
QT_MOC_LITERAL(8, 59, 4), // "zdjl"
QT_MOC_LITERAL(9, 64, 11), // "showJdqData"
QT_MOC_LITERAL(10, 76, 17), // "QMap<QString,int>"
QT_MOC_LITERAL(11, 94, 3), // "map"
QT_MOC_LITERAL(12, 98, 10), // "showYlData"
QT_MOC_LITERAL(13, 109, 5), // "strTd"
QT_MOC_LITERAL(14, 115, 7), // "strData"
QT_MOC_LITERAL(15, 123, 8), // "consumer"
QT_MOC_LITERAL(16, 132, 14), // "analyseBmqData"
QT_MOC_LITERAL(17, 147, 14) // "analyseAllData"

    },
    "ShowWorker\0showCgqData\0\0strList\0"
    "showBmpData\0strDir\0iSd\0iWz\0zdjl\0"
    "showJdqData\0QMap<QString,int>\0map\0"
    "showYlData\0strTd\0strData\0consumer\0"
    "analyseBmqData\0analyseAllData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShowWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    4,   52,    2, 0x06 /* Public */,
       9,    1,   61,    2, 0x06 /* Public */,
      12,    2,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,   69,    2, 0x0a /* Public */,
      16,    0,   70,    2, 0x0a /* Public */,
      17,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,

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
        case 0: _t->showCgqData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->showBmpData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 2: _t->showJdqData((*reinterpret_cast< QMap<QString,int>(*)>(_a[1]))); break;
        case 3: _t->showYlData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->consumer(); break;
        case 5: _t->analyseBmqData(); break;
        case 6: _t->analyseAllData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ShowWorker::*_t)(QStringList const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showCgqData)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(QString , double , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showBmpData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(QMap<QString,int> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showJdqData)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ShowWorker::*_t)(QString const & , QString const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowWorker::showYlData)) {
                *result = 3;
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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ShowWorker::showCgqData(QStringList const & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ShowWorker::showBmpData(QString _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ShowWorker::showJdqData(QMap<QString,int> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ShowWorker::showYlData(QString const & _t1, QString const & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
