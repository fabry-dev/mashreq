#include "soundplayer.h"
#include "qdebug.h"
#include "mainwindow.h"
#include "qtimer.h"
#define HW true





static inline void check_error(int status)
{
    if (status < 0) {
        qDebug()<<"mpv API error: "<<mpv_error_string(status);
        //  exit(1);
    }
}


/*


static void wakeup(void *ctx)
{
    // This callback is invoked from any mpv thread (but possibly also
    // recursively from a thread that is calling the mpv API). Just notify
    // the Qt GUI thread to wake up (so that it can process events with
    // mpv_wait_event()), and return as quickly as possible.

    soundplayer *vp = (soundplayer *)ctx;
    emit vp->mpv_events();
}



*/


soundplayer::soundplayer(QWidget *parent ):QObject(parent)
{


    std::setlocale(LC_NUMERIC, "C");
    mpv = mpv_create();
    if (!mpv) {
        qDebug()<<"failed creating context";
        return;
    }
    mpv_request_log_messages(mpv,"no");


    check_error(mpv_set_option_string(mpv, "input-vo-keyboard", "no"));
    //check_error(mpv_set_option_string(mpv, "ao", "jack"));



    int val = 0;
    check_error(mpv_set_option(mpv, "osc", MPV_FORMAT_FLAG, &val));

    if(mpv_initialize(mpv)<0)
        qDebug()<<"could not initialize MPV";
    else
        qDebug()<<"mpv initialized";




}

void soundplayer::loadFile(QString filename)
{
    QByteArray ba = filename.toLatin1();
    const char *soundFile = ba.data();




    const char *cmd[] = {"loadfile", soundFile, NULL};
    mpv_command(mpv, cmd);

    int pause = 0;
   // mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &pause);
}


void soundplayer::stop()
{

    const char *cmd2[] = {"stop",NULL, NULL};
     mpv_command(mpv, cmd2);
}





void soundplayer::handle_mpv_event(mpv_event *event)
{

    switch (event->event_id) {

    case MPV_EVENT_SHUTDOWN:
    {
        qDebug() << "MPV player closed";

        //deleteLock.lock();
        //disconnect(this);
        //mpv_detach_destroy(mpv);
        //mpv=NULL;

        // QTimer::singleShot(3000,this,SLOT(deleteLater()));
        qDebug()<<"MPV player destroyed";
        break;
    }
    default: ;
        // Ignore uninteresting or unknown events.
    }
}



void soundplayer::on_mpv_events()
{

    // Process all events, until the event queue is empty.
    while (mpv) {

        deleteLock.lock();
        mpv_event *event = mpv_wait_event(mpv, 0);
        deleteLock.unlock();
        if (event->event_id == MPV_EVENT_NONE)
            break;

        handle_mpv_event(event);

    }

}




void soundplayer::closePlayer()
{


    qDebug()<<"Starting to delete video player";
    const char *cmd[] = {"quit", "1", NULL};
    //check_error(mpv_command(mpv, cmd));
    mpv_terminate_destroy(mpv);

    this->deleteLater();
}






soundplayer::~soundplayer()
{
    qDebug()<<"video player deleted";

}
