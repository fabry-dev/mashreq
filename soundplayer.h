#ifndef soundplayer_H
#define soundplayer_H

#include "qwidget.h"

#include "qevent.h"
#include <QObject>
#include <mpv/client.h>
#include "qmutex.h"
#include "qopengl.h"


class soundplayer : public QObject
{
    Q_OBJECT
public:
    soundplayer( QWidget *parent = 0);
    ~soundplayer() ;
    void closePlayer();
    void loadFile(QString filename);
    void stop();

private:
    void handle_mpv_event(mpv_event *event);
    mpv_handle *mpv;
    QMutex deleteLock;

private slots:
    void on_mpv_events();

signals:
    void mpv_events(void);

};



#endif // soundplayer_H
