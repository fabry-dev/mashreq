#include "gamewindow.h"


#define GO_x0 330
#define GO_y0 1160
#define GO_x1 772
#define GO_y1 1250

#define SPIN_x0 270
#define SPIN_y0 1250
#define SPIN_x1 820
#define SPIN_y1 1365


#define SPIN_SCENE_x0 233
#define SPIN_SCENE_y0 672
#define SPIN_SCENE_x1 865
#define SPIN_SCENE_y1 1128


#define firstName_x0 115
#define firstName_y0 (740-25)
#define firstName_x1 530
#define firstName_y1 (770+10)

#define lastName_x0 560
#define lastName_y0 (740-25)
#define lastName_x1 970
#define lastName_y1 (770+10)

#define phone_x0 115
#define phone_y0 (895-25)
#define phone_x1 530
#define phone_y1 (925+10)

#define email_x0 560
#define email_y0 (895-25)
#define email_x1 970
#define email_y1 (925+10)

#define tryagain_x0 280
#define tryagain_y0 1250
#define tryagain_x1 815
#define tryagain_y1 1360

#define minNameSize 3
#define maxNameSize 20
#define minPhoneSize 8
#define maxPhoneSize 16
#define minEmailSize 5
#define maxEmailSize 30


#define FONTSIZE 60

QStringList vouchers = QStringList() <<"100"<<"0"<<"150"<<"25"<<"200"<<"50"<<"100"<<"0"<<"150"<<"50"<<"200"<<"100"<<"0"<<"150"<<"25"<<"200";



QString toCamelCase(const QString& s)
{
    QStringList parts = s.split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
}



gameWindow::gameWindow(QWidget *parent, QString PATH):QLabel(parent),PATH(PATH)
{
    resize(1080,1920);
    startPix.load(PATH+"start.png");
    registrationPix.load(PATH+"registration.png");
    spinPix.load(PATH+"spin.png");
    tryAgainPix.load(PATH+"tryagain.png");
    winPix.load(PATH+"win.png");
    aed100Pix.load(PATH+"100aed.png");
    aed150Pix.load(PATH+"150aed.png");
    aed200Pix.load(PATH+"200aed.png");


    initDb();

    enKeys = new keyboard(PATH+"keys.cfg");

    int id = QFontDatabase::addApplicationFont(PATH+"Dubai-Regular.ttf");

    font = QFont(QFontDatabase::applicationFontFamilies(id).at(0),30);

    sp = new soundplayer(this);


    firstNameLbl = new QLabel(this);
    firstNameLbl->resize(firstName_x1-firstName_x0,firstName_y1-firstName_y0);
    firstNameLbl->move(firstName_x0,firstName_y0);
    firstNameLbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    firstNameLbl->setFont(font);
    firstNameLbl->setStyleSheet("QLabel { color : black; }");
    firstNameLbl->hide();
    //firstNameLbl->setStyleSheet("border: 1px solid black");

    lastNameLbl = new QLabel(this);
    lastNameLbl->resize(lastName_x1-lastName_x0,lastName_y1-lastName_y0);
    lastNameLbl->move(lastName_x0,lastName_y0);
    lastNameLbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    lastNameLbl->setFont(font);
    lastNameLbl->setStyleSheet("QLabel { color : black; }");
    lastNameLbl->hide();

    phoneLbl = new QLabel(this);
    phoneLbl->resize(phone_x1-phone_x0,phone_y1-phone_y0);
    phoneLbl->move(phone_x0,phone_y0);
    phoneLbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    phoneLbl->setFont(font);
    phoneLbl->setStyleSheet("QLabel { color : black; }");
    phoneLbl->hide();


    emailLbl = new QLabel(this);
    emailLbl->resize(email_x1-email_x0,email_y1-email_y0);
    emailLbl->move(email_x0,email_y0);
    emailLbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    emailLbl->setFont(font);
    emailLbl->setStyleSheet("QLabel { color : black; }");
    emailLbl->hide();



    prizeLbl = new QLabel(this);
    prizeLbl->resize(aed100Pix.size());
    prizeLbl->move((width()-prizeLbl->width())/2,1000);
    prizeLbl->hide();


    spinScene = new QGraphicsScene(this);

    spinScene->setSceneRect(QRect(0,0,SPIN_SCENE_x1-SPIN_SCENE_x0,SPIN_SCENE_y1-SPIN_SCENE_y0));

    spinView = new QGraphicsView(this);
    spinView->move(SPIN_SCENE_x0,SPIN_SCENE_y0);
    spinView->resize(SPIN_SCENE_x1-SPIN_SCENE_x0,SPIN_SCENE_y1-SPIN_SCENE_y0);
    spinView->hide();
    spinView->setScene(spinScene);
    spinView->setStyleSheet("background-color: transparent;");
    spinView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    spinView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(int i = 0;i<vouchers.size();i++)
    {
        QGraphicsTextItem * wheelText = new QGraphicsTextItem;
        int id2 = QFontDatabase::addApplicationFont(PATH+"Dubai-Bold.ttf");
        font2 = QFont(QFontDatabase::applicationFontFamilies(id2).at(0),70);
        font2.setPointSizeF((double)FONTSIZE);
        wheelText->setDefaultTextColor ( Qt::white);
        wheelText->setPlainText("AED "+vouchers[i]);
        wheelText->setFont(font2);
        wheelText->show();
        wheelText->setFlags(QGraphicsItem::ItemIsMovable);
        wheelTexts.push_back(wheelText);
        spinScene->addItem(wheelText);
        wheelText->setPos((spinScene->width()-wheelText->sceneBoundingRect().width())/2,i*wheelText->sceneBoundingRect().height());

        if(wheelText->y()>((vouchers.size())*(wheelText->boundingRect().height()))/2)
            wheelText->moveBy(0,-((vouchers.size())*(wheelText->boundingRect().height())));

        QRectF r = QRectF(wheelText->pos(),wheelText->boundingRect().size());





        wheelRects.push_back(r);



       rotationEffect(wheelText,&r);
    }

    wheelTimer = new QTimer(this);
    connect(wheelTimer,SIGNAL(timeout()),this,SLOT(rotateWheel()));
}


void gameWindow::createTable()
{

    QSqlQuery query("CREATE TABLE players (id INTEGER PRIMARY KEY AUTOINCREMENT, firstName TEXT,lastName TEXT, phone TEXT, email TEXT, date TIMESTAMP)");

    bool success = query.exec();

    if(!success)
    {
        qDebug() << query.lastError();
    }

}

void gameWindow::initDb()
{

    const QString DRIVER("QSQLITE");
    QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
    db.setHostName("hostname");
    db.setDatabaseName(PATH+"mashreqdb");
    db.setUserName("mashrequser");
    db.setPassword("mashreqpassword");

    if(!db.open())
        qWarning() << "ERROR: " << db.lastError();

    qDebug()<<db.tables();

    //qDebug()<<db.record("players");
    //dropTable();
    //createTable();
    createTable();
}

void gameWindow::insertPlayer(QString firstname,QString lastname, QString email,QString phone)
{
    QDateTime timestamp = QDateTime::currentDateTime();

    QSqlQuery query;
    query.prepare("INSERT INTO players (firstName, lastName, phone, email, date) VALUES (:firstname,:lastname,:phone,:email,:datetime)");


    query.bindValue(":firstname", firstname);
    query.bindValue(":lastname", lastname);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":datetime", timestamp.toString("yyyy-MM-dd hh:mm:ss"));

    bool success = query.exec();
    if(!success)
    {
        qDebug() << query.lastError();
    }
}

void gameWindow::getPlayers()
{
    QSqlQuery query;
    query.prepare("SELECT id,firstName,lastName,phone,email,date FROM players");
    query.exec();

    qDebug()<<query.size()<<" player(s)";
    while (query.next()) {
        int id = query.value(0).toInt();
        QString firstname = query.value(1).toString();
        QString lastname =  query.value(2).toString();
        QString phone = query.value(3).toString();
        QString email = query.value(4).toString();

        QDateTime date = query.value(7).toDateTime();
        qDebug()<<id<<firstname<<lastname<<phone<<email;
    }
}

void gameWindow::start()
{
    gameState = startgame;
    setPixmap(startPix);
    firstNameLbl->hide();
    lastNameLbl->hide();
    emailLbl->hide();
    phoneLbl->hide();
    prizeLbl->hide();
    spinView->hide();
    showFullScreen();
    sp->stop();

    // getPlayers();

    getVouchersCount();

}

void gameWindow::goRegister()
{
    gameState = inputFirstName;
    setPixmap(registrationPix);
    firstNameLbl->show();
    lastNameLbl->show();
    emailLbl->show();
    phoneLbl->show();
    showFullScreen();

    firstNameTxt="";
    lastNameTxt="";
    phoneTxt="+";
    emailTxt="";


    firstNameLbl->setText(firstNameTxt);
    lastNameLbl->setText(lastNameTxt);
    phoneLbl->setText(phoneTxt);
    emailLbl->setText(emailTxt);

}

void gameWindow::addUser()
{

    gameState = addingUser;
    qDebug()<<firstNameTxt<<lastNameTxt<<emailTxt<<phoneTxt;
    gameAttempts = 0;
    goSpinWindow();

    insertPlayer(firstNameTxt,lastNameTxt,emailTxt,phoneTxt);

}

void gameWindow::goSpinWindow()
{

    gameState = preSpin;
    setPixmap(spinPix);


    firstNameLbl->hide();
    lastNameLbl->hide();
    emailLbl->hide();
    phoneLbl->hide();
    prizeLbl->hide();
    spinView->show();
    showFullScreen();

    sp->stop();

}

void gameWindow::startSpin()
{
    gameState = spinning;

    gameAttempts ++;


    wheelCount = 0;

    qsrand(QTime::currentTime().msec());


    uint voucher100left = voucher100Total-voucher100Given;
    uint voucher150left = voucher150Total-voucher150Given;
    uint voucher200left = voucher200Total-voucher200Given;
    uint vouchersleft = voucher100left+voucher150left+voucher200left;

    int val = qrand()%vouchersleft;


    if(val < voucher100left)
        voucherValue = "100";
    else if(val <voucher100left+voucher150left)
        voucherValue = "150";
    else
        voucherValue = "200";



   // voucherValue = "100";



    qDebug()<<"win: "<<voucherValue;




    wheelTimer->start(1);




}

void gameWindow::doneSpinning()
{
    spinView->hide();

    if(voucherValue == "200")
    {
        setPixmap(winPix);
        prizeLbl->setPixmap(aed200Pix);
        prizeLbl->show();
        gameState = won;
        sp->loadFile(PATH+"bigprice.wav");
        endGame();
    }
    else
    {
        if(gameAttempts>=2)
        {
            setPixmap(winPix);
            gameState = won;

            if(firstVoucherValue=="150")//let's keep the bigger value
                voucherValue = firstVoucherValue;

            if(voucherValue=="150")
                prizeLbl->setPixmap(aed150Pix);
            else
                prizeLbl->setPixmap(aed100Pix);

            prizeLbl->show();
            endGame();
        }
        else
        {
            firstVoucherValue = voucherValue;
            setPixmap(tryAgainPix);
            gameState = tryagain;
            if(voucherValue=="150")
                prizeLbl->setPixmap(aed150Pix);
            else
                prizeLbl->setPixmap(aed100Pix);
            prizeLbl->show();
        }
        sp->loadFile(PATH+"smallprice.wav");
    }
}

void gameWindow::endGame()
{
    if(voucherValue == "200")
        voucher200Given++;
    else if(voucherValue == "150")
        voucher150Given++;
    else if(voucherValue == "100")
        voucher100Given++;

    writeVouchersCount();
    sendSms();
    QTimer::singleShot(10000,this,SLOT(start()));
}

void gameWindow::rotationEffect(QGraphicsTextItem *txt,QRectF* rect)
{

   //txt->setPos(rect->topLeft());


    if((txt->y()>=-txt->boundingRect().height())&&(txt->y()<spinScene->height()))
    {
        double T = txt->boundingRect().height()+spinScene->height();
        double f = abs(sin((double)3.1416*(txt->y()+txt->boundingRect().height())/T));

        int h0 = txt->sceneBoundingRect().height();
        font2.setPointSizeF((double)FONTSIZE*(0.5+f));
        txt->setFont(font2);

        float y2 = rect->y()+(rect->height()-txt->boundingRect().height())/2;

        txt->setPos((spinScene->width()-txt->sceneBoundingRect().width())/2,y2);
       // txt->setPos(rect->topLeft());
    }
    else
    {
        font2.setPointSizeF((double)FONTSIZE);
        txt->setFont(font2);

    }
}









void gameWindow::rotateWheel()
{
    QString active = "";


    for(int i = 0;i<wheelRects.size();i++)
    {
        QRectF *rect=&(wheelRects[i]);
        QGraphicsTextItem * txt = wheelTexts[i];

        rect->moveTo(QPoint(rect->x(),rect->y()+4));

        if(rect->y()>((wheelRects.size())*(rect->height()))/2)
            rect->moveTo(QPoint(rect->x(),rect->y()-((wheelRects.size())*(rect->height()))));







        //txt->boundingRect().moveTo(rect->topLeft());
        txt->setPos(rect->topLeft());

        rotationEffect(txt,rect);


        if((abs((rect->y())-(spinScene->height()-rect->height())/2)<8))
        {
            active = txt->toPlainText();

            if(active!=lastActive)
            {
                lastActive=active;
                sp->loadFile(PATH+"top.wav");
                 qDebug()<<"top"<<active;
            }
        }




    }


   //qDebug()<<wheelTexts[0]->boundingRect();



    for (auto txt:wheelTexts)
    {
        /*txt->moveBy(0,4);
        if(txt->y()>((wheelTexts.size())*(txt->boundingRect().height()))/2)
            txt->moveBy(0,-((wheelTexts.size())*(txt->boundingRect().height())));
        */


/*

        rotationEffect(txt);


        if((abs((txt->y())-(spinScene->height())/2+(txt->boundingRect().height()/2))<5))
        {
            active = txt->toPlainText();

            if(active!=lastActive)
            {
                lastActive=active;
                sp->loadFile(PATH+"top.wav");
                // qDebug()<<"top"<<active;
            }
        }
*/













    }

    wheelCount++;

    if (wheelCount >  50)
    {
        wheelTimer->setInterval(wheelTimer->interval()+1);
        wheelCount = 0;
    }


    if((wheelTimer->interval()>15)&&(active == "AED "+voucherValue))
    {
        sp->stop();
        wheelTimer->stop();
        QTimer::singleShot(2000,this,SLOT(doneSpinning()));
        //QTimer::singleShot(2000,this,SLOT(startSpin()));
    }
}









void gameWindow::sendSms()
{
    CURL *curl;
    CURLcode res;


    QString text = "Congratulations! Open a Mashreq Neo account here https://www.mashreqneo.com/FST to redeem your DFC reward card!";


    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.smsglobal.com/http-api.php");
        /* Now specify the POST data */

        QString str;
        char *s;
        str = "action=sendsms&user=2q6c96fp&password=iXEFANhW&from=Mashreq Neo&to="+phoneTxt+"&text="+text+"&maxsplit=400";
        s=(char *)malloc(str.length());
        QByteArray ba=str.toLatin1();  // str is my json input data
        strcpy(s,ba.data());

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,s);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

}













void gameWindow::getCharacter(QString c)
{
    QRegExp re("\\d*");  // a digit (\d), zero or more times (*)

    if(gameState == inputFirstName)
    {
        if(c == "PLAY")
        {
            if(firstNameTxt.size()>minNameSize)
                gameState = inputLastName;
            return;
        }
        else if(c == "BACK")
        {
            if(firstNameTxt.size()>0)
            {
                firstNameTxt = firstNameTxt.remove(firstNameTxt.size()-1,1);
                firstNameLbl->setText(firstNameTxt);
                updLbl(firstNameLbl);
            }
            return;
        }
        else if(c == "SPACE")
        {
            if(firstNameTxt.size()<maxNameSize)
            {
                firstNameTxt.append(" ");
                firstNameLbl->setText(firstNameTxt);
                updLbl(firstNameLbl);
            }
            return;
        }
        else
        {
            if(firstNameTxt.size()<maxNameSize)
            {
                firstNameTxt.append(c);
                firstNameTxt = toCamelCase(firstNameTxt);
                firstNameLbl->setText(firstNameTxt);
                updLbl(firstNameLbl);
            }
            return;
        }
    }
    else  if(gameState == inputLastName)
    {
        if(c == "PLAY")
        {
            if(lastNameTxt.size()>minNameSize)
                gameState = inputPhone;
            return;
        }
        else if(c == "BACK")
        {
            if(lastNameTxt.size()>0)
            {
                lastNameTxt = lastNameTxt.remove(lastNameTxt.size()-1,1);
                lastNameLbl->setText(lastNameTxt);
                updLbl(lastNameLbl);
            }
            return;
        }
        else if(c == "SPACE")
        {
            if(lastNameTxt.size()<maxNameSize)
            {
                lastNameTxt.append(" ");
                lastNameLbl->setText(lastNameTxt);
                updLbl(lastNameLbl);
            }
            return;
        }
        else
        {
            if(lastNameTxt.size()<maxNameSize)
            {
                lastNameTxt.append(c);
                lastNameTxt = toCamelCase(lastNameTxt);
                lastNameLbl->setText(lastNameTxt);
                updLbl(lastNameLbl);
            }
            return;
        }
    }
    else  if(gameState == inputPhone)
    {
        if(c == "PLAY")
        {
            if(phoneTxt.size()>minPhoneSize)

            {
                gameState = inputEmail;

            }
            return;
        }
        else if(c == "BACK")
        {
            if(phoneTxt.size()>1)
            {
                phoneTxt = phoneTxt.remove(phoneTxt.size()-1,1);
                phoneLbl->setText(phoneTxt);
                updLbl(phoneLbl);
            }
            return;
        }
        else if(c == "SPACE")
        {
            return;
        }
        else
        {
            if(phoneTxt.size()<maxPhoneSize)
            {
                if (re.exactMatch(c))
                {
                    phoneTxt.append(c);
                    phoneLbl->setText(phoneTxt);
                    updLbl(phoneLbl);
                }
            }
            return;
        }
    }
    else  if(gameState == inputEmail)
    {
        if(c == "PLAY")
        {
            if(emailTxt.size()>minEmailSize)
                addUser();
            return;
        }
        else if(c == "BACK")
        {
            if(emailTxt.size()>0)
            {
                emailTxt = emailTxt.remove(emailTxt.size()-1,1);
                emailLbl->setText(emailTxt);
                updLbl(emailLbl);
            }
            return;
        }
        else if(c == "SPACE")
        {
            return;
        }
        else
        {
            if(emailTxt.size()<maxEmailSize)
            {
                emailTxt.append(c);
                emailLbl->setText(emailTxt);
                updLbl(emailLbl);
            }
            return;
        }
    }


}


void gameWindow::updLbl(QLabel *lbl)
{
    QFontMetrics metrics(font);
    QSize size = metrics.size(0, lbl->text()); //Get size of text
    float factorw = (lbl->width()) / (float)size.width(); //Get the width factor
    float factorh = (lbl->height()) / (float)size.height(); //Get the height factor
    float factor = qMin(factorw, factorh); //To fit contents in the screen select as factor
    //the minimum factor between width and height
    font.setPointSizeF(font.pointSizeF() * factor); //Set font size*/
    lbl->setFont(font);
}



void gameWindow::getVouchersCount()
{


    //voucher100Total,voucher150Total,voucher200Total;
    //   uint voucher100Given,voucher150Given,voucher200Given;

    QFile file(PATH+"vouchers.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no voucher file!";
        exit(0);

    }
    else
    {

        QTextStream in(&file);
        QStringList  lines;

        while(!in.atEnd()) {
            lines.append(in.readLine());
        }

        if(lines.size()<6)
        {
            qDebug()<<"voucher file error!";
            exit(0);
        }

        voucher100Total = lines[0].toInt();
        voucher150Total = lines[1].toInt();
        voucher200Total = lines[2].toInt();
        voucher100Given = lines[3].toInt();
        voucher150Given = lines[4].toInt();
        voucher200Given = lines[5].toInt();

        uint voucher100left = voucher100Total-voucher100Given;
        uint voucher150left = voucher150Total-voucher150Given;
        uint voucher200left = voucher200Total-voucher200Given;
        uint vouchersleft = voucher100left+voucher150left+voucher200left;
        qDebug()<<"vouchers left:"<<vouchersleft<<"("<<voucher100left<<voucher150left<<voucher200left<<")";
        file.close();
        if(vouchersleft<=0)
        {
            qDebug()<<"resetting vouchers count!";
            voucher100Given=voucher150Given=voucher200Given=0;
            writeVouchersCount();

        }
    }






}

void gameWindow::writeVouchersCount()
{
    QFile file(PATH+"vouchers.cfg");

    if(!file.open(QIODevice::WriteOnly)) {
        qDebug()<<"cannot write  to voucher file!";
        exit(0);
    }
    else
    {
        QTextStream in(&file);
        in << QString::number(voucher100Total) << endl;
        in << QString::number(voucher150Total) << endl;
        in << QString::number(voucher200Total) << endl;
        in << QString::number(voucher100Given) << endl;
        in << QString::number(voucher150Given) << endl;
        in << QString::number(voucher200Given) << endl;
    }
    file.close();
}


gameWindow::~gameWindow()
{


}


void gameWindow::mousePressEvent( QMouseEvent* ev )
{
    QPoint pt = ev->pos();


    if((pt.x()>GO_x0)&&(pt.y()>GO_y0)&&(pt.x()<GO_x1)&&(pt.y()<GO_y1)&&(gameState==startgame))
    {
        goRegister();
        return;
    }
    else if((pt.x()>SPIN_x0)&&(pt.y()>SPIN_y0)&&(pt.x()<SPIN_x1)&&(pt.y()<SPIN_y1)&&(gameState==preSpin))
    {
        startSpin();
        return;
    }
    else if((pt.x()>tryagain_x0)&&(pt.y()>tryagain_y0)&&(pt.x()<tryagain_x1)&&(pt.y()<tryagain_y1)&&(gameState==tryagain))
    {
        goSpinWindow();
        return;
    }
    else if((gameState==inputFirstName)||(gameState==inputLastName)||(gameState==inputPhone)||(gameState==inputEmail))
    {
        QString key;
        key = enKeys->getText(pt);
        if(key!="")
        {
            getCharacter(key);
            return;
        }

    }





    qDebug()<<ev->pos();

}
