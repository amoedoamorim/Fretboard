#ifndef FRETBOARD_H
#define FRETBOARD_H

#include "chord.h"
#include "note.h"
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QString>
#include <QXmlStreamReader>
#include <QTimer>
#include <QStatusBar>

class Fretboard : public QWidget
{
    Q_OBJECT

    public:
    Fretboard(QWidget *parent = 0);
    int loadFile(QString fileName);
    int unloadFile();
    QString getTitle();
    void generateCM();
    void evaluate();
    void lyExport();
    void setRadius(int r);
    void setBeats(QString b);

    public slots:
    void play();
    void stop();
    void nextNote();
    void previousNote();

    protected:
    void paintEvent(QPaintEvent *event);
	
    private:
    QPixmap pixmap;
    QColor getIntervalColor(int interval);
    QList <Chord*> chordList;
    QList <Note*> melody;
    QRect placeNote(int string, int fret);
    QString intToNoteName(int pitch);
    QString intToLily(int pitch);
    QXmlStreamReader xml;
    QString beats;
    QTimer *playTimer;
    QStatusBar *statusBar;

    QString m_title;
    int m_divisions;
    int m_beats;
    int m_beatType;
    int m_bpm;

    int radius;
    int viewMode;
    bool showAllMode;
    int notesToBeDisplayed[6][25];
    int notes[6][25];
    int iCurrentNote;
    Note *currentNote;

    bool loadedFile;

    int charToInt(QString name);
    int noteOctaveToMidi(int note, int octave);
    int intervalPreference(int interval);
    int distance(int fret1, int fret2);
    void setupVoicing(int mode);
    void toggleShowAll();
    void selectClosestChordNotes(Chord* chord, int string, int fret);
    void displayNotes();
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void swapViewMode();
    void clear();
    void init();
    void setNotePosition();
    notePosition findNearest(int fret, int pitch);
};

#endif
