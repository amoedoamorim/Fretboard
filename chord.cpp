#include "chord.h"
#include <QString>
#include <QDebug>

Chord::Chord()
{
    rootStep = -1;
}

int Chord::charToInt(QString pitchClass)
{
    pitchClass.toUpper();
    switch (pitchClass[0].toAscii())
    {
    case 'C': return 0;
    case 'D': return 2;
    case 'E': return 4;
    case 'F': return 5;
    case 'G': return 7;
    case 'A': return 9;
    case 'B': return 11;
    default : return -1;
    }
}

void Chord::setRootStep(QString root)
{
    rootStep = charToInt(root);
}

int Chord::getRootStep(){
    return rootStep;
}

void Chord::setRootAlter(QString alter)
{
    rootStep += 12;
    rootStep += alter.toInt();
    rootStep = rootStep%12;
}

void Chord::setKind(QString kind)
{
    chordKind = kind;
}

int Chord::interval(int pitch){
    int interval;

    pitch = pitch%12;

    if (rootStep <= pitch)
        interval = pitch - rootStep;
    else
        interval = 12 - (rootStep - pitch);

    return interval;
}

QString Chord::getChordName(){
    return intToString(rootStep) + " " + chordKind;
}

QString Chord::intToString(int pitch)
{
    switch(pitch){
    case 0: return QString("C");
    case 1: return QString("C#");
    case 2: return QString("D");
    case 3: return QString("D#");
    case 4: return QString("E");
    case 5: return QString("F");
    case 6: return QString("F#");
    case 7: return QString("G");
    case 8: return QString("G#");
    case 9: return QString("A");
    case 10: return QString("A#");
    case 11: return QString("B");
    default: return QString("X");
    }
}

void Chord::setupChord(){
    intervals << 0;
    if (chordKind == "major")
        intervals << 4 << 7;
    else if (chordKind == "minor")
        intervals << 3 << 7;
    else if (chordKind == "augmented")
        intervals << 4 << 8;
    else if (chordKind == "diminished")
        intervals << 3 << 6;
    else if ((chordKind == "dominant")||(chordKind == "dominant-seventh"))
        intervals << 4 << 7 << 10;
    else if (chordKind == "major-seventh")
        intervals << 4 << 7 << 11;
    else if (chordKind == "minor-seventh")
        intervals << 3 << 7 << 10;
    else if (chordKind == "minor-major")
        intervals << 3 << 7 << 11;
    else if (chordKind == "diminished-seventh") //(diminished triad, diminished seventh)
        intervals << 3 << 6 << 9;
    else if (chordKind == "augmented-seventh") //(augmented triad, minor seventh)
        intervals << 4 << 8 << 10;
    else if (chordKind == "half-diminished") //(diminished triad, minor seventh)
        intervals << 3 << 6 << 10;
    else if (chordKind == "major-minor") //(minor triad, major seventh)
        intervals << 3 << 7 << 11;
    else if (chordKind == "major-sixth")
        intervals << 4 << 7 << 9;
    else if (chordKind == "minor-sixth")
        intervals << 3 << 7 << 9;
    else if (chordKind == "dominant-ninth")
        intervals << 2 << 4 << 7 << 10;
    else if (chordKind == "major-ninth")
        intervals << 2 << 4 << 7 << 11; //(major-seventh, major ninth)
    else if (chordKind == "minor-ninth")
        intervals << 2 << 3 << 7 << 10; //(minor-seventh, major ninth)
    else if (chordKind == "dominant-11th")
        intervals << 2 << 4 << 5 << 7 << 10;//(dominant-ninth, perfect 11th)
    else if (chordKind == "major-11th")
        intervals << 2 << 4 << 5 << 7 << 11;//(major-ninth, perfect 11th)
    else if (chordKind == "minor-11th")
        intervals << 2 << 3 << 5 << 7 << 10;//(minor-ninth, perfect 11th)
    else if (chordKind == "dominant-13th")
        intervals << 2 << 4 << 5 << 7 << 9 << 10;//(dominant-11th, major 13th)
    else if (chordKind == "major-13th")
        intervals << 2 << 4 << 5 << 7 << 9 << 11;//(major-11th, major 13th)
    else if (chordKind == "minor-13th")
        intervals << 2 << 3 << 5 << 7 << 9 << 10;//(minor-11th, major 13th)
    else if (chordKind == "suspended-second")
        intervals << 2 << 7;//(major second, perfect fifth)
    else if (chordKind == "suspended-fourth")
        intervals << 5 << 7;//(perfect fourth, perfect fifth)
    else
        qDebug() << "Chord kind not supported: " + chordKind;

    for (int i = 0; i<intervals.size(); i++)
        notes.append((rootStep+intervals[i])%12);
}

void Chord::addDegree(int degree, int alter){
    int interval;
    interval = degreeToInterval(degree);

    if (interval == -1){
        qDebug() << "Erro: degreeToInterval(): nao sabe o que retornar.";
        return;
    }

    interval += alter;
    intervals.append(interval);
    notes.append((rootStep+interval)%12);
}

void Chord::altDegree(int degree, int alter){
    int interval = degreeToInterval(degree);

    if (interval == -1){
        qDebug() << "Erro: degreeToInterval(): nao sabe o que retornar.";
        return;
    }

    int a = ((rootStep+interval)%12);
    int b = ((rootStep+interval+alter)%12);

    //qDebug() << "substituindo " << a << "por" << b;

    if (notes.contains(a)){
        notes.removeOne(a);
        notes.append(b);
    }
    else
        qDebug() << "Erro: Chord::altDegree(): Acorde nao contem a nota a ser alterada.";

    if (intervals.contains(interval)){
        intervals.removeOne(interval);
        intervals.append(interval+alter);
    }
    else
        qDebug() << "Erro: Chord::altDegree(): Acorde nao contem a nota a ser alterada.";
}

int Chord::degreeToInterval(int degree){
    switch(degree){
    case 5: return 7;
    case 7: return 10;
    case 9: return 2;
    default: return -1;
    }
}

QList <int> Chord::getNotes(){
    return notes;
}

QList <int> Chord::getIntervals(){
    return intervals;
}

int Chord::size(){
    return notes.size();
}

bool Chord::hasNote(int note){
    return notes.contains(note%12);
}

bool operator==(const Chord& c1, const Chord& c2) {
    if ((c1.rootStep == c2.rootStep)&&(c1.chordKind == c2.chordKind))
        return true;
    else
        return false;
}
