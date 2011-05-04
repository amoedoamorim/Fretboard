#include "note.h"

Note::Note()
{
    s = -1;
    f = -1;
    tie = 0;
    syncopate = false;
    harmony = NULL;
}

void Note::setPitch(int value){
    pitch = value;
}

void Note::setMeasure(int m){
    measure = m;
}

void Note::setDuration(int d){
    duration = d;
}

void Note::setHarmony(Chord *chord){
    harmony = chord;
}

void Note::startTie(){
    tie = 1;
}

void Note::stopTie(){
    tie = 2;
}

int Note::getTie(){
    return tie;
}

int Note::getPitch(){
    return pitch;
}

int Note::getFret(){
    return f;
}

int Note::getString(){
    return s;
}

int Note::getMeasure(){
    return measure;
}

int Note::getDuration(){
    return duration;
}

Chord* Note::getHarmony(){
    return harmony;
}

void Note::setPosition(int s, int f){
    this->s = s;
    this->f = f;
}

bool Note::canSyncopate(int accum, int div, int beats, int type){
    if(tie == 1){
        if( duration <= div && (div*beats)-accum <= div){
            syncopate = true;
            return true;
        }
        else return false;
    }
    else return false;
}

bool Note::isSyncopated(){
    return syncopate;
}
