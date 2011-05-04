/****************************************************************************
** Meta object code from reading C++ file 'fretboard.h'
**
** Created: Wed 4. May 23:18:46 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fretboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fretboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Fretboard[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      18,   10,   10,   10, 0x0a,
      25,   10,   10,   10, 0x0a,
      36,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Fretboard[] = {
    "Fretboard\0\0play()\0stop()\0nextNote()\0"
    "previousNote()\0"
};

const QMetaObject Fretboard::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Fretboard,
      qt_meta_data_Fretboard, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Fretboard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Fretboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Fretboard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Fretboard))
        return static_cast<void*>(const_cast< Fretboard*>(this));
    return QWidget::qt_metacast(_clname);
}

int Fretboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: play(); break;
        case 1: stop(); break;
        case 2: nextNote(); break;
        case 3: previousNote(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
