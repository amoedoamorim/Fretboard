#include <QtGui>
#include <QFileDialog>

#include "mainwindow.h"

#define VERSION_STRING "Fretboard 0.0.2a"

MainWindow::MainWindow()
{
    QString beats;
    QString radius;

    toolBar = new QToolBar();
    fretb = new Fretboard(this);

    setWindowTitle(VERSION_STRING);
    setCentralWidget(fretb);
    statusBar()->showMessage("Load a MusicXML Lead Sheet to get started");
    addToolBar(toolBar);
    fretb->setFocus(Qt::OtherFocusReason);
    setMinimumSize(QSize(990,200));
    adjustSize();
    createActions();
    createMenus();

    if (qApp->argc() == 4){
        radius = qApp->arguments().at(1);
        beats = qApp->arguments().at(2);
        fileName = qApp->arguments().at(3);

        fretb->setBeats(beats);
        fretb->setRadius(radius.toInt());

        if (!fileName.endsWith(".xml",Qt::CaseInsensitive)){
            fileName = QFileDialog::getOpenFileName(this, tr("Open File"),".",tr("MusicXML Lead Sheets (*.xml)"));
        }
            if (fretb->loadFile(fileName) == 1){
                fretb->generateCM();
                fretb->evaluate();
                fretb->lyExport();
            }
    }
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileOpen);
    fileMenu->addAction(fileClose);
    fileMenu->addAction(fileQuit);

    songMenu = menuBar()->addMenu(tr("&Song"));
    songMenu->addAction(songPlay);
    songMenu->addAction(songPause);
    songMenu->addSeparator();
    songMenu->addAction(songNextNote);
    songMenu->addAction(songPrevNote);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAbout);
    helpMenu->addAction(helpAboutQt);

    toolBar->addAction(songPlay);
    toolBar->addAction(songPause);
}

void MainWindow::createActions()
{
    fileOpen = new QAction(tr("&Open Leadsheet"), this);
    fileOpen->setShortcuts(QKeySequence::Open);
    fileOpen->setStatusTip(tr("Open a MusicXML file"));
    connect(fileOpen, SIGNAL(triggered()), this, SLOT(open()));

    fileClose = new QAction(tr("&Close File"), this);
    //fileClose->setShortcuts(QKeySequence::Open);
    fileClose->setStatusTip(tr("Close current file"));
    connect(fileClose, SIGNAL(triggered()), this, SLOT(closeFile()));

    fileQuit = new QAction(tr("&Quit"), this);
    fileQuit->setShortcuts(QKeySequence::Quit);
    fileQuit->setStatusTip(tr("Quit Fretboard"));
    connect(fileQuit, SIGNAL(triggered()), this, SLOT(quit()));

    songNextNote = new QAction(tr("&Next Note"), this);
    songNextNote->setShortcuts(QKeySequence::MoveToNextChar);
    songNextNote->setStatusTip(tr("Displays next note on the melody"));
    connect(songNextNote, SIGNAL(triggered()), fretb, SLOT(nextNote()));

    songPrevNote = new QAction(tr("&Previous Note"), this);
    songPrevNote->setShortcuts(QKeySequence::MoveToPreviousChar);
    songPrevNote->setStatusTip(tr("Displays previous note on the melody"));
    connect(songPrevNote, SIGNAL(triggered()), fretb, SLOT(previousNote()));

    songPlay = new QAction(tr("&Play"), this);
    songPlay->setStatusTip(tr("Play"));
    connect(songPlay, SIGNAL(triggered()), fretb, SLOT(play()));

    songPause = new QAction(tr("&Pause"), this);
    songPause->setStatusTip(tr("Pause"));
    connect(songPause, SIGNAL(triggered()), fretb, SLOT(stop()));

    helpAboutQt = new QAction(tr("&About Qt"), this);
    helpAboutQt->setStatusTip(tr("About Qt"));
    connect(helpAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    helpAbout = new QAction(tr("&About Fretboard"), this);
    helpAbout->setStatusTip(tr("About Fretboard"));
    connect(helpAbout, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::about(){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Author: Alexandre Amoedo Amorim\n<amoedoamorim@gmail.com>\n\nThis program is provided \"as is\" without warranties of any kind, either expressed or implied, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose. Use it at your own risk.");
    msgBox.exec();
}

void MainWindow::open() {
    if (fretb->loadFile(QFileDialog::getOpenFileName(this, tr("Open Leadsheet"),".",tr("MusicXML Lead Sheets (*.xml)"))) == 1){
        setWindowTitle(fretb->getTitle() + " - " + VERSION_STRING);
        fretb->generateCM();
        //fretb->evaluate();
        fretb->lyExport();
    }
}

void MainWindow::closeFile(){
    if (fretb->unloadFile() == 0){
        setWindowTitle(VERSION_STRING);
    }
    statusBar()->showMessage("Load a MusicXML Lead Sheet to get started");
}

void MainWindow::quit() {
    fretb->unloadFile();
    qApp->quit();
}
