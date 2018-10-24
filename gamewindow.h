#ifndef gameWindow_H
#define gameWindow_H
#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include <QtSql>
#include "qdatetime.h"
#include "QTime"
#include "qdebug.h"
#include "qfont.h"
#include "qfontdatabase.h"
#include <QtSql>
#include <QSqlQuery>
#include "keyboard.h"
#include "qgraphicsscene.h"
#include "QGraphicsTextItem"
#include "QGraphicsView"
#include "qpixmap.h"
#include <QtWidgets>
#include "QGraphicsPixmapItem"
#include "soundplayer.h"
#include <curl/curl.h>


enum states { startgame,inputFirstName,inputLastName,inputPhone,inputEmail,addingUser,preSpin,spinning,won,tryagain };

class gameWindow:public QLabel
{
    Q_OBJECT

public:
    gameWindow(QWidget *parent = 0,QString PATH="");
    ~gameWindow();

    void goRegister();
    void addUser();
    void goSpinWindow();
    void sendSms();


private:

    QString PATH;
    QPixmap startPix,registrationPix,spinPix,tryAgainPix,winPix,aed200Pix,aed150Pix,aed100Pix;
    keyboard *enKeys;
    states gameState;
    QString firstNameTxt,lastNameTxt,phoneTxt,emailTxt;
    QFont font,font2;
    QLabel *firstNameLbl,*lastNameLbl,*phoneLbl,*emailLbl,*prizeLbl;
    QGraphicsScene * spinScene;
    std::vector <QGraphicsTextItem *> wheelTexts;
    QTimer *wheelTimer;
    ulong wheelCount;
    QString voucherValue,firstVoucherValue;
    soundplayer *sp;
    QGraphicsView *spinView ;
    int gameAttempts;

    void getCharacter(QString c);

    void startSpin();
    void endGame();
    void initDb();
    void createTable();
    void insertPlayer(QString firstname,QString lastname, QString email,QString phone);
    void getPlayers();
    void getVouchersCount();
    void writeVouchersCount();

    uint voucher100Total,voucher150Total,voucher200Total;
    uint voucher100Given,voucher150Given,voucher200Given;




protected:
    void mousePressEvent( QMouseEvent* ev );

private slots:
    void doneSpinning();
    void rotateWheel();

public slots:
    void start();

};




#endif // gameWindow_H

