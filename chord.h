#ifndef CHORD_H
#define CHORD_H

#include <QChar>
#include <QString>
#include <QList>

class Chord
{
friend bool operator==(const Chord& c1, const Chord& c2);

public:
    Chord();
    int size();
    int getRootStep();
    int interval(int pitch);
    void setRootStep(QString rootStep);
    void setRootAlter(QString rootAlter);
    void setKind(QString kind);
    void setupChord();
    void addDegree(int degree, int alter);
    void altDegree(int degree, int alter);
    int degreeToInterval(int degree);
    bool hasNote(int note);
    QList<int> getNotes();
    QList<int> getIntervals();
    QString getChordName();

private:
    int rootStep;
    int charToInt(QString pitchClass);
    QString intToString(int pitchClass);
    QString chordKind;
    QList <int> notes;
    QList <int> intervals;

};

bool operator==(const Chord& c1, const Chord& c2);

#endif // CHORD_H
