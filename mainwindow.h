#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fretboard.h"
//#include "beatselector.h"
#include <QMainWindow>
#include <QMenuBar>

//#include <QStatusBar>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QFileDialog;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    void createActions();
    void createMenus();

    Fretboard *fretb;

    QToolBar *toolBar;
    QString fileName;
    QFileDialog *fileDialog;

    QMenu *fileMenu;
    QMenu *songMenu;
    QMenu *helpMenu;

    QAction *fileOpen;
    QAction *fileClose;
    QAction *fileQuit;
    QAction *songNextNote;
    QAction *songPrevNote;
    QAction *songPlay;
    QAction *songPause;
    QAction *helpAbout;
    QAction *helpAboutQt;

private slots:
    void about();
    void closeFile();
    void open();
    void quit();
};

#endif // MAINWINDOW_H
