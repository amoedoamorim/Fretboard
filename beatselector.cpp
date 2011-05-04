#include "beatselector.h"
#include "ui_beatselector.h"
#include "fretboard.h"
#include <QtGui>

BeatSelector::BeatSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BeatSelector)
{
    ui->setupUi(this);

    Fretboard *f;

    f = (Fretboard*)parent;
    f->sayEAH();
}

BeatSelector::~BeatSelector()
{
    delete ui;
}

void setBeats(int beats){

}

void BeatSelector::setBPM(int bpm){
    qDebug() << bpm;
}

void setBeatType(int beatType){
}

void BeatSelector::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
