#ifndef NOTE_H
#define NOTE_H

#include "chord.h"

struct notePosition{
    int s;
    int f;
};

class Note
{
public:
    Note();
    void setPitch(int pitch);
    void setHarmony(Chord *chord);
    void setPosition(int s, int f);
    void setMeasure(int m);
    void setDuration(int d);
    void startTie();
    void stopTie();
    int getString();
    int getFret();
    int getPitch();
    int getMeasure();
    int getDuration();
    int getTie();
    bool canSyncopate(int accum, int div, int beats, int type);
    bool syncopate;
    bool isSyncopated();
    Chord* getHarmony();

    QList <notePosition> voicing;
    QString sduration;

private:
    int pitch;
    int s;
    int f;
    int tie;
    Chord* harmony;
    int measure;
    int duration;
};

#endif // NOTE_H
