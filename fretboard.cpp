#include "fretboard.h"
#include "chord.h"
//#include "beatselector.h"
#include <math.h>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QXmlStreamReader>
#include <QMainWindow>
#include <QFile>
#include <QList>
#include <QStatusBar>
//#define SEARCH_RADIUS 2

Fretboard::Fretboard(QWidget *parent) : QWidget(parent)
{
    showAllMode = true;
    loadedFile = false;

    QMainWindow *mainwindow = (QMainWindow *)parent;

    statusBar = mainwindow->statusBar();

    //statusBar->showMessage("sucesso!");

    beats  = "1";
    radius = 2;

    if(!pixmap.load("fretboardis.png")){
        qDebug() << "ERROR: File fretboardis.png not found.";
        exit(-1);
    }

    resize(pixmap.size());
    init();

    playTimer = new QTimer(this);
    connect(playTimer, SIGNAL(timeout()), this, SLOT(nextNote()));
}

QString Fretboard::getTitle(){
    if (loadedFile)
        return m_title;
    else
        return NULL;
}

void Fretboard::setRadius(int r){
    radius = r;
}

void Fretboard::setBeats(QString b){
    beats = b;
}

void Fretboard::generateCM(){
    int currentBar  = 1;
    //currentBeat = 0;
    int accum   = 0;
    QList<int> beatsWithChord;
    QString b = beats;

    while (b.size() != 0){
        switch(b.at(0).toAscii()){
        case 49: beatsWithChord.append(0); b.remove(QChar('1'),Qt::CaseInsensitive);
            break;
        case 50: beatsWithChord.append(m_divisions); b.remove(QChar('2'),Qt::CaseInsensitive);
            break;
        case 51: beatsWithChord.append(m_divisions*2); b.remove(QChar('3'),Qt::CaseInsensitive);
            break;
        case 52: beatsWithChord.append(m_divisions*3); b.remove(QChar('4'),Qt::CaseInsensitive);
            break;
        default: b.remove(0,1);
            break;
        }
    }

    for(int i = 0; i < melody.size(); i++){
        currentNote = melody.at(i);

        if (currentNote->getMeasure() != currentBar)
            qDebug() << "currentBar inconsistency!" << currentBar << currentNote->getMeasure();

        if (currentNote->getPitch() > 0){
            if(currentNote->canSyncopate(accum,m_divisions,m_beats,m_beatType)){
                currentNote->setHarmony(melody.at(i+1)->getHarmony());
                if(beats.contains('1'))
                    setupVoicing(1);
                else
                    setupVoicing(0);
            }
            else if (beatsWithChord.contains(accum)){
                setupVoicing(1);
            }
            else
                setupVoicing(0);
        }

        accum += currentNote->getDuration();

        if (accum == (m_beats * m_divisions)){
            accum = 0;
            currentBar++;
        }
    }

    clear();
    iCurrentNote = melody.size();
    currentNote = NULL;
}

void Fretboard::setupVoicing(int mode){
    QList <notePosition> voicing;

    clear();
    setNotePosition();

    int s = currentNote->getString();
    int f = currentNote->getFret();

    if(notes[s][f] == currentNote->getPitch()){
        notesToBeDisplayed[s][f] = 2;

        if (mode == 1 && currentNote->getHarmony() != NULL)
            selectClosestChordNotes(currentNote->getHarmony(),s,f);
    }

    for (int s=0; s<6; s++)
        for (int f=0; f<25; f++){
            if (notesToBeDisplayed[s][f] != 0){
                notePosition pos;
                pos.s = s;
                pos.f = f;
                voicing.append(pos);
            }
        }

    for (int i = 0; i < voicing.size(); i++)
        currentNote->voicing.append(voicing.at(i));
}

void Fretboard::setNotePosition(){
    int s = 0;
    int f = 24;
    static int lastFret = -1;

    if (lastFret < 0){
        while(notes[s][f] != currentNote->getPitch() && s < 3){
            f--;
            if (f < 0){
                f = 24;
                s++;
            }
        }
        currentNote->setPosition(s,f);
        lastFret = f;
    }
    else{
        notePosition pos = findNearest(lastFret,currentNote->getPitch());

        currentNote->setPosition(pos.s,pos.f);
        lastFret = pos.f;
    }
}

int Fretboard::distance(int fret1, int fret2){
    return abs(fret1 - fret2);
}

notePosition Fretboard::findNearest(int fret, int pitch){
    QList <notePosition> posList;
    notePosition nearest;

    for (int s=0; s<3; s++)
        for (int f=0; f<25; f++)
            if (notes[s][f] == pitch){
                notePosition p;
                p.s = s;
                p.f = f;
                posList.append(p);
            }

    nearest = posList.at(0);

    for (int i = 0; i < posList.size(); i++){
        if (distance(posList.at(i).f,fret) < distance(nearest.f,fret))
            nearest = posList.at(i);
    }
    return nearest;
}

void Fretboard::init(void){
    viewMode = 0;

    notes[0][0] = 76;
    notes[1][0] = 71;
    notes[2][0] = 67;
    notes[3][0] = 62;
    notes[4][0] = 57;
    notes[5][0] = 52;

    for (int s=0; s<6; s++)
        for (int f=1; f<25; f++)
            notes[s][f] = notes[s][f-1]+1;

    clear();
}

void Fretboard::clear(void){
    for (int s=0; s<6; s++)
        for (int f=0; f<25; f++)
            notesToBeDisplayed[s][f] = 0;
}



void Fretboard::selectClosestChordNotes(Chord* chord, int string, int fret){
    int s = string + 1;
    int fretL;
    int fretR;
    int notesFound = 0;
    int intervalL;
    int intervalR;
    QList <int> intervalsWanted;

    intervalsWanted = chord->getIntervals();

    if (intervalsWanted.size() > 3 && intervalsWanted.contains(chord->interval(notes[string][fret]))){
        intervalsWanted.removeOne(chord->interval(notes[string][fret]));
    }

    if (intervalsWanted.size() > 3){
        intervalsWanted.removeOne(7);
    }

    if (intervalsWanted.size() > 3){
        intervalsWanted.removeOne(0);
    }

    while (notesFound < 3 && s < 6){
        fretL = fret;
        fretR = fret;

        intervalL = -1;
        intervalR = -1;

        while(!intervalsWanted.contains(chord->interval(notes[s][fretL])) && fretL > fret-radius && fretL > 0)
            fretL--;

       if (chord->hasNote(notes[s][fretL]) && intervalsWanted.contains(chord->interval(notes[s][fretL]))){
           //ADD CANDIDATE
            notesToBeDisplayed[s][fretL] = 1;
            intervalL = chord->interval(notes[s][fretL]);
            intervalsWanted.removeOne(chord->interval(notes[s][fretL]));
            notesFound++;
        }

        while(!intervalsWanted.contains(chord->interval(notes[s][fretR])) && fretR < fret+radius)
            fretR++;

        if (chord->hasNote(notes[s][fretR]) && intervalsWanted.contains(chord->interval(notes[s][fretR]))){
            //ADD CANDIDATE
            notesToBeDisplayed[s][fretR] = 1;
            intervalR = chord->interval(notes[s][fretR]);
            intervalsWanted.removeOne(chord->interval(notes[s][fretR]));
            notesFound++;
        }

        //EVALUATE CANDIDATES
        if (intervalR >= 0 && intervalL >= 0 && intervalR != intervalL){
            //PRIMEIRO CRITERIO, PROXIMIDADE
            if (fretR - fret < fret - fretL){
                notesToBeDisplayed[s][fretL] = 0;
                intervalsWanted.append(chord->interval(notes[s][fretL]));
                notesFound--;
            }
            else if(fretR - fret > fret - fretL){
                notesToBeDisplayed[s][fretR] = 0;
                intervalsWanted.append(chord->interval(notes[s][fretR]));
                notesFound--;
            }
            //SEGUNDO CRITERIO, PREFERENCIA DE INTERVALO
            else if (intervalPreference(intervalL) > intervalPreference(intervalR)){
                notesToBeDisplayed[s][fretR] = 0;
                intervalsWanted.append(chord->interval(notes[s][fretR]));
                notesFound--;
            }
            else if (intervalPreference(intervalL) < intervalPreference(intervalR)){
                notesToBeDisplayed[s][fretL] = 0;
                intervalsWanted.append(chord->interval(notes[s][fretL]));
                notesFound--;
            }
            else if (intervalPreference(intervalL) == intervalPreference(intervalR))
                qDebug() << "Choque de Preferencia de Intervalo";
        }
        s++;
    }
}

int Fretboard::intervalPreference(int interval){
    switch(interval){
    case 0: return 0;   //root
    case 1: return 6;	//minor 2nd
    case 2: return 6;	//major 2nd
    case 3: return 2;   //minor 3rd
    case 4: return 2;   //major 3rd
    case 5: return 3;   //fourth
    case 6: return 3;   //aug 4th / dim 5th
    case 7: return 1;   //fifth
    case 8: return 5;   //minor 6th
    case 9: return 5;   //major 6th / dim 7th
    case 10: return 4;  //minor 7th
    case 11: return 4;  //major 7th
    default: return 0;
    }
}

QRect Fretboard::placeNote(int string, int fret){
    QRect rect;
    rect.setRect(30 + ((fret-1) * 40), 1 + (string * 20), 35, 16);
    return rect;
}

QColor Fretboard::getIntervalColor(int interval){
    switch(interval){
    case 0: return QColor(0,0,255);         //root
    case 1: return QColor(204,204,0);       //minor 2nd
    case 2: return QColor(102,0,153);       //major 2nd
    case 3: return QColor(0,204,153);       //minor 3rd
    case 4: return QColor(255,0,0);         //major 3rd
    case 5: return QColor(0,102,255);       //fourth
    case 6: return QColor(255,255,0);       //aug 4th / dim 5th
    case 7: return QColor(102,0,255);       //fifth
    case 8: return QColor(0,170,0);         //minor 6th
    case 9: return QColor(153,0,102);       //major 6th
    case 10: return QColor(0,255,255);      //minor 7th
    case 11: return QColor(255,102,0);      //major 7th
    default: return QColor(Qt::white);
    }
}

QString Fretboard::intToNoteName(int pitch){
    switch(pitch%12){
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
    case -1: return QString("rest");
    default: return QString("X");
    }	
}

QString Fretboard::intToLily(int pitch){
    div_t result;
    int pitchClass;
    int octave;
    QString lilyNotation;

    result = div(pitch,12);
    pitchClass = result.rem;
    octave = result.quot - 1;

    lilyNotation = intToNoteName(pitchClass).toLower();

    if (lilyNotation[1] == '#')
        lilyNotation.replace(1,1,"is");

    if (octave > 3){
        for (int i = 0; i < octave - 3; i++)
            lilyNotation.append("'");
    }
    else if (octave < 3){
        for (int i = 0; i < 3 - octave; i++)
            lilyNotation.append(",");
    }
    return lilyNotation;
}

void Fretboard::displayNotes()
{
    if (currentNote == NULL){
        return;
    }

    if ((showAllMode) && (currentNote->getHarmony() != NULL)){
        for (int s=0; s<6; s++)
            for (int f=0; f<25; f++){
                if (currentNote->getHarmony()->hasNote(notes[s][f]))
                    notesToBeDisplayed[s][f] = 1;
                else
                    notesToBeDisplayed[s][f] = 0;
                if (notes[s][f] == currentNote->getPitch())
                    notesToBeDisplayed[s][f] = 2;
            }
    }

    else{
        clear();
        for (int i=0; i<currentNote->voicing.size(); i++){
            notesToBeDisplayed[currentNote->voicing.at(i).s][currentNote->voicing.at(i).f] = 1;
        }
    }

    QPainter painter;
    QPen pen;
    painter.begin(this);

    //draw chord notes
    for (int s=0; s<6; s++){
        for (int f=0; f<25; f++){
            if (notesToBeDisplayed[s][f] == 1){
                if (currentNote->getHarmony() != NULL)
                    painter.setBrush(QBrush(getIntervalColor(currentNote->getHarmony()->interval(notes[s][f])),Qt::SolidPattern));
                    painter.drawEllipse( placeNote(s,f) );
                    painter.setPen(pen);

                if (viewMode == 0)
                    painter.drawText( placeNote(s,f),Qt::AlignCenter,intToNoteName(notes[s][f]) );
                else
                    painter.drawText( placeNote(s,f),Qt::AlignCenter,QString ("%1").arg(notes[s][f]) );
            }
        }
    }

    //draw melody note
    for (int s=0; s<6; s++){
        for (int f=0; f<25; f++){
            if ((notesToBeDisplayed[s][f] == 2) || (notesToBeDisplayed[s][f] == 1 && notes[s][f] == currentNote->getPitch()))
            {
                if (currentNote->getHarmony() == NULL)
                    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(getIntervalColor(currentNote->getHarmony()->interval(notes[s][f])),Qt::SolidPattern));

                pen.setColor(Qt::red);
                pen.setWidth(7);
                painter.setPen(pen);
                painter.drawEllipse( placeNote(s,f) );

                pen.setColor(Qt::white);
                pen.setWidth(3);
                painter.setPen(pen);
                painter.drawEllipse( placeNote(s,f) );

                pen.setColor(Qt::black);
                painter.setPen(pen);

                if (viewMode == 0)
                    painter.drawText( placeNote(s,f),Qt::AlignCenter,intToNoteName(notes[s][f]) );
                else
                    painter.drawText( placeNote(s,f),Qt::AlignCenter,QString ("%1").arg(notes[s][f]) );
            }
        }
    }
    painter.end();
}

void Fretboard::paintEvent(QPaintEvent * /*event*/){
    QPainter painter;

    painter.begin(this);
    painter.drawPixmap(0,0,pixmap);

    for (int i=0; i<6; i++){
        painter.drawText( 5, (i+1)*20 - 6, intToNoteName(notes[i][0]));
    }

    painter.end();
    displayNotes();
}

void Fretboard::toggleShowAll(){
    showAllMode = !showAllMode;
}

void Fretboard::swapViewMode(){
    if (viewMode == 0)
        viewMode = 1;
    else
        viewMode = 0;
    repaint();
}

void Fretboard::play(){
    playTimer->start(500);
}

void Fretboard::stop(){
    playTimer->stop();
}

void Fretboard::nextNote(){
    if (!loadedFile){
        return;
    }
    iCurrentNote++;

    if (iCurrentNote >= melody.size())
        iCurrentNote = 0;

    currentNote = melody.at(iCurrentNote);

    if (currentNote->getHarmony() == NULL){
        statusBar->showMessage(QString("Measure: %1 | %2 over ").arg(currentNote->getMeasure()).arg(intToNoteName(currentNote->getPitch())) + "N/C");
        qDebug() << QString("%1: %2 (%3) over ").arg(currentNote->getMeasure()).arg(currentNote->getPitch()).arg(currentNote->getDuration()) + "N/C";
    }
    else{
        statusBar->showMessage(QString("Measure: %1 | %2 over ").arg(currentNote->getMeasure()).arg(intToNoteName(currentNote->getPitch())) + currentNote->getHarmony()->getChordName());
        qDebug() << QString("%1: %2 (%3) over ").arg(currentNote->getMeasure()).arg(currentNote->getPitch()).arg(currentNote->getDuration()) + currentNote->getHarmony()->getChordName() << currentNote->getHarmony()->getNotes();
    }
    ////////////////////////////////////////////////////////
    // transformar isto em setNotesToBeDisplayed()
    clear();
    for (int i=0; i<currentNote->voicing.size(); i++){
        notesToBeDisplayed[currentNote->voicing.at(i).s][currentNote->voicing.at(i).f] = 1;
    }
    ////////////////////////////////////////////////////////
    repaint();
}

void Fretboard::previousNote(){
    if (!loadedFile){
        return;
    }
    iCurrentNote--;

    if (iCurrentNote < 0)
        iCurrentNote = melody.size()-1;

    currentNote = melody.at(iCurrentNote);

    if (currentNote->getHarmony() == NULL)
        qDebug() << QString("%1: %2 (%3) over ").arg(currentNote->getMeasure()).arg(currentNote->getPitch()).arg(currentNote->getDuration()) + "N/C";
    else
        qDebug() << QString("%1: %2 (%3) over ").arg(currentNote->getMeasure()).arg(currentNote->getPitch()).arg(currentNote->getDuration()) + currentNote->getHarmony()->getChordName() << currentNote->getHarmony()->getNotes();

    clear();
    for (int i=0; i<currentNote->voicing.size(); i++){
        notesToBeDisplayed[currentNote->voicing.at(i).s][currentNote->voicing.at(i).f] = 1;
    }
    repaint();    
}

void Fretboard::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
        swapViewMode();
}

void Fretboard::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_A){
            toggleShowAll();
    }
    else if (event->key() == Qt::Key_M){
        swapViewMode();
    }
    /* //eventos tratados na MainWindow
    else if (event->key() == Qt::Key_Right){

        //nextNote();
    }
    else if (event->key() == Qt::Key_Left){

        //previousNote();
    }

    else if (event->key() == Qt::Key_Escape){
        exit(0);
    }
    */
    repaint();
}

int Fretboard::charToInt(QString name){
    name.toUpper();
    switch (name[0].toAscii()){
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

int Fretboard::noteOctaveToMidi(int note, int octave){
    return note+(12*(octave+1));
}

void Fretboard::evaluate(){
    QFile arquivo(m_title + "-b" + beats + "-r" + QString("%1").arg(radius) + ".txt");
    arquivo.open(QIODevice::WriteOnly | QIODevice::Text);
    arquivo.write("PRESENÇA DOS GRAUS RELEVANTES NA FORMACAO DO ACORDE\n\n");
    arquivo.write("Harmonia\tFormação\tMelodia\tGraus Presentes\tSatisfação\tObs.\n");
    arquivo.write("---------------------------------------------------------------------------------------------------\n");

    int numChords  = 0;
    int percentSum = 0;

    for(int i = 0; i < melody.size(); i++){
        QString out;

        if (melody.at(i)->voicing.size() > 1){
            numChords++;

            out.append(melody.at(i)->getHarmony()->getChordName());
            out.append("\t");

            QList<int> chordIntervals = melody.at(i)->getHarmony()->getIntervals();

            for(int j = 0; j < chordIntervals.size(); j++){
                out.append(QString("%1,").arg(chordIntervals.at(j)));
            }

            out.append("\t");
            out.append(intToNoteName(melody.at(i)->getPitch()));
            out.append(QString(" (%1)\t").arg(melody.at(i)->getPitch()).toAscii());

            QList<int> chordVoicing;

            for(int k = 0; k < melody.at(i)->voicing.size(); k++){
                chordVoicing << melody.at(i)->getHarmony()->interval(notes[melody.at(i)->voicing.at(k).s][melody.at(i)->voicing.at(k).f]);
                out.append(QString("%1,").arg(chordVoicing.at(k)));
            }

            int percent = 0;
            int count   = 0;

            for(int l = 0; l < chordVoicing.size(); l++){
                if (chordIntervals.contains(chordVoicing.at(l)))
                    count++;
            }

            percent = ((float)count/chordIntervals.size())*100;

            if (percent > 100)
                percent = 100;

            out.append(QString("\t%1%").arg(percent));
            percentSum += percent;
            out.append("\n");
        }
        arquivo.write(out.toAscii());
    }
    arquivo.write("---------------------------------------------------------------------------------------------------\n");
    arquivo.write(QString("MEDIA:\t\t\t\t%1\n\n").arg(percentSum/numChords).toAscii());
    arquivo.write("ATENDIMENTO DAS SINCOPES\n\n");
    for(int i = 0; i < melody.size(); i++){
        if(melody.at(i)->isSyncopated()){
            arquivo.write(QString("Sincope no compasso %1\n").arg(melody.at(i)->getMeasure()).toAscii());
        }
    }
    arquivo.close();
}

void Fretboard::lyExport(){
    QFile arquivo("out.ly");
    arquivo.open(QIODevice::WriteOnly | QIODevice::Text);
    arquivo.write("\\version \"2.12.0\"\n\\score {\n    {\n        \\set Staff.midiInstrument = #\"acoustic guitar (nylon)\"\n        \\tempo 4 = 100\n        \\time ");

    if (m_beats != 0 && m_beatType != 0)
        arquivo.write(QString("%1/%2\n").arg(m_beats).arg(m_beatType).toAscii());

    for(int i = 0; i < melody.size(); i++){
        if (melody.at(i)->getPitch() == -1)
            arquivo.write("        r");
        else{
            arquivo.write("        < ");
            for(int j = 0; j < melody.at(i)->voicing.size(); j++){
                int pitch = notes[melody.at(i)->voicing.at(j).s][melody.at(i)->voicing.at(j).f];
                arquivo.write(intToLily(pitch).toAscii());
                arquivo.write(" ");
            }

            arquivo.write(">");
        }
        arquivo.write(melody.at(i)->sduration.toAscii());

        if(melody.at(i)->isSyncopated() || melody.at(i)->getTie() == 1)
            arquivo.write("~");

        arquivo.write("\n");
    }
    arquivo.write("    }\n    \\midi{}\n    \\layout{}\n}\n");
    arquivo.close();
}

int Fretboard::unloadFile(){

    stop();

    if (loadedFile){

        currentNote = NULL;

        while (chordList.size() > 0){
            delete chordList.takeLast();
        }

        while (melody.size() > 0){
            delete melody.takeLast();
        }

        loadedFile = false;
        qDebug() << m_title << " unloaded sucessfully";
        m_title = "";
        repaint();
    }

    return 0;
}

int Fretboard::loadFile(QString fileName){

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file: " << fileName;
        return -1;
    }

    int lowestNote = 127;
    int highestNote = 0;
    int measure = -1;
    int pitch = -1;
    int degree = -1;
    int degAlter = 0;
    float durationInBeats;
    int divisions = 0;
    Note *melodyNote = NULL;
    Chord *acorde = NULL;
    QXmlStreamReader xml(&file);

    while (!xml.atEnd()){
        xml.readNext();

        if(xml.name() == "movement-title" && xml.isStartElement()){
            m_title = xml.readElementText();
            qDebug() << "Title: " + m_title;      
        }
        if(xml.name() == "measure"){
            if(xml.isStartElement()){
                measure = xml.attributes().value("number").toString().toInt();
            }
            if(xml.isEndElement()){
                measure = -1;
            }
        }
        if(xml.name() == "divisions" && xml.isStartElement()){
            divisions = xml.readElementText().toInt();
            m_divisions = divisions;
        }
        if(xml.name() == "beats" && xml.isStartElement()){
            m_beats = xml.readElementText().toInt();
        }
        if(xml.name() == "beat-type" && xml.isStartElement()){
            m_beatType = xml.readElementText().toInt();
        }
        else if (xml.name() == "note"){
            if(xml.isStartElement()){
                melodyNote = new Note();
                melodyNote->setMeasure(measure);
            }
            if(xml.isEndElement()){
                if (!chordList.isEmpty())
                    melodyNote->setHarmony(chordList.last());

                melody.append(melodyNote);
                melodyNote = NULL;
            }
        }
        else if (xml.name() == "rest"){
            melodyNote->setPitch(-1);
        }
        else if (xml.name() == "duration"){
            if(xml.isStartElement()){
                QString sduration;
                int duration = xml.readElementText().toInt();

                durationInBeats = (float)duration/divisions;

                if(durationInBeats == 4)
                    sduration = "1";
                else if (durationInBeats == 3)
                    sduration = "2.";
                else if (durationInBeats == 2)
                    sduration = "2";
                else if (durationInBeats == 1.5)
                    sduration = "4.";
                else if (durationInBeats == 1)
                    sduration = "4";
                else if (durationInBeats == .75)
                    sduration = "8.";
                else if (durationInBeats == .5)
                    sduration = "8";
                else if (durationInBeats == .375)
                    sduration = "16.";
                else if (durationInBeats == .25)
                    sduration = "16";
                else if (durationInBeats == .1875)
                    sduration = "32.";
                else if (durationInBeats == .125)
                    sduration = "32";
                else if (durationInBeats == .09375)
                    sduration = "64.";
                else if (durationInBeats == .0625)
                    sduration = "64";
                else{
                    sduration = "8"; //Só influencia no Lilypond mesmo...
                    durationInBeats = floor(durationInBeats*100)/100; //truncate to 2 decimal places
                    qDebug() << "ERRO: Fretboard::loadFile(): Duracao de nota nao suportada" << durationInBeats;
                }
                melodyNote->sduration = sduration;
                melodyNote->setDuration(duration);
            }
        }
        else if (xml.name() == "step"){
            if(xml.isStartElement()){
                pitch = charToInt(xml.readElementText());
            }
        }
        else if (xml.name() == "alter"){
            if(xml.isStartElement()){
                pitch += xml.readElementText().toInt();
            }
        }
        else if (xml.name() == "octave"){
            if(xml.isStartElement()){
                int value = 12 + noteOctaveToMidi(pitch,xml.readElementText().toInt());
                if (value > highestNote)
                    highestNote = value;
                else if (value < lowestNote)
                    lowestNote = value;

                melodyNote->setPitch(value);
            }
        }
        else if (xml.name() == "tie"){
            if(xml.isStartElement() && (xml.attributes().value("type") == "start")){
                melodyNote->startTie();
            }
            if(xml.isStartElement() && (xml.attributes().value("type") == "stop")){
                melodyNote->stopTie();
            }
        }
        else if(xml.name() == "harmony"){
            if(xml.isStartElement()){
                acorde = new Chord();
            }
            if(xml.isEndElement()){
                chordList.append(acorde);
                acorde = NULL;
            }
        }
        else if (xml.name() == "root-step"){
            if(xml.isStartElement()){
                acorde->setRootStep(xml.readElementText());
            }
        }
        else if (xml.name() == "root-alter"){
            if(xml.isStartElement()){
                acorde->setRootAlter(xml.readElementText());
            }
        }
        else if (xml.name() == "kind"){
            if(xml.isStartElement()){
                acorde->setKind(xml.readElementText());
                acorde->setupChord();
            }
        }        
        else if (xml.name() == "degree-value" && xml.isStartElement()){
            degree = xml.readElementText().toInt();
            //qDebug() << xml.name() << degree << "measure " << measure;
        }
        else if (xml.name() == "degree-alter" && xml.isStartElement()){
            degAlter = xml.readElementText().toInt();
            //qDebug() << xml.name() << degAlter;
        }
        else if (xml.name() == "degree-type" && xml.isStartElement()){
            QString degType = xml.readElementText();
            //qDebug() << xml.name() << degType;

            if (degType == "add")
                acorde->addDegree(degree,degAlter);
            else if (degType == "alter")
                acorde->altDegree(degree,degAlter);
        }
    }

    if (lowestNote < 67 || highestNote > 100){
        qDebug() << "AVISO: Notas da melodia extrapolam valores ideais!!!";
        qDebug() << "lowestNote < 67: " << lowestNote;
        qDebug() << "highestNote > 100: " << highestNote;
        return -1;
    }

    loadedFile = true;
    file.close();
    return 1;
}
