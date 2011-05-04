#ifndef BEATSELECTOR_H
#define BEATSELECTOR_H

#include <QDialog>

namespace Ui {
    class BeatSelector;
}

class BeatSelector : public QDialog {
    Q_OBJECT
public:
    BeatSelector(QWidget *parent = 0);
    ~BeatSelector();
    void setBeats(int beats);
    void setBeatType(int beatType);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BeatSelector *ui;

private slots:
    void setBPM(int bpm);
};

#endif // BEATSELECTOR_H
