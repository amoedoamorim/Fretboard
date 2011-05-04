/********************************************************************************
** Form generated from reading UI file 'beatselector.ui'
**
** Created: Wed Jun 23 18:07:46 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEATSELECTOR_H
#define UI_BEATSELECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_BeatSelector
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label_beats;
    QLabel *label_beatType;
    QLabel *label_3;
    QLineEdit *lineEditBPM;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QDialog *BeatSelector)
    {
        if (BeatSelector->objectName().isEmpty())
            BeatSelector->setObjectName(QString::fromUtf8("BeatSelector"));
        BeatSelector->resize(388, 301);
        buttonBox = new QDialogButtonBox(BeatSelector);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 230, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_beats = new QLabel(BeatSelector);
        label_beats->setObjectName(QString::fromUtf8("label_beats"));
        label_beats->setGeometry(QRect(60, 90, 21, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label_beats->setFont(font);
        label_beatType = new QLabel(BeatSelector);
        label_beatType->setObjectName(QString::fromUtf8("label_beatType"));
        label_beatType->setGeometry(QRect(60, 110, 62, 17));
        label_beatType->setFont(font);
        label_3 = new QLabel(BeatSelector);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 40, 251, 17));
        lineEditBPM = new QLineEdit(BeatSelector);
        lineEditBPM->setObjectName(QString::fromUtf8("lineEditBPM"));
        lineEditBPM->setGeometry(QRect(50, 190, 81, 27));
        label_4 = new QLabel(BeatSelector);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(50, 170, 62, 17));
        label_5 = new QLabel(BeatSelector);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(140, 200, 62, 17));

        retranslateUi(BeatSelector);
        QObject::connect(buttonBox, SIGNAL(accepted()), BeatSelector, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BeatSelector, SLOT(reject()));

        QMetaObject::connectSlotsByName(BeatSelector);
    } // setupUi

    void retranslateUi(QDialog *BeatSelector)
    {
        BeatSelector->setWindowTitle(QApplication::translate("BeatSelector", "Dialog", 0, QApplication::UnicodeUTF8));
        label_beats->setText(QApplication::translate("BeatSelector", "4", 0, QApplication::UnicodeUTF8));
        label_beatType->setText(QApplication::translate("BeatSelector", "4", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("BeatSelector", "Your time signature is:", 0, QApplication::UnicodeUTF8));
        lineEditBPM->setText(QApplication::translate("BeatSelector", "120", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("BeatSelector", "Tempo:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("BeatSelector", "BPM", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BeatSelector: public Ui_BeatSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEATSELECTOR_H
