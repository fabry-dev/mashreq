#include "keyboard.h"

keyboard::keyboard(QString configFile)
{
    QFile inputFile(configFile);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList data = line.split(" ");
            if(data.size()>=5)
            {
                bool test1,test2,test3,test4;
                int x0,y0,x1,y1;
                QString txt;


                x0 = data[0].toInt(&test1);
                y0 = data[1].toInt(&test2);
                x1 = data[2].toInt(&test3);
                y1 = data[3].toInt(&test4);
                txt= data[4];
                if(test1&&test2&&test3&&test4&&(txt!=""))
                {
                    rects.push_back(QRect(QPoint(x0,y0),QPoint(x1,y1)));
                    texts.push_back(txt);
                }



            }
        }
        inputFile.close();


    }
    qDebug()<<"keyboard file "<<configFile<<" "<<rects.size()<<" value(s) added.";
}



QString keyboard::getText(QPoint pt)
{

    for(int i = 0;i<rects.size();i++)
    {
        QRect rect = rects[i];
        if((pt.x()>=rect.topLeft().x())
                &&(pt.y()>=rect.topLeft().y())
                &&(pt.x()<=rect.bottomRight().x())
                &&(pt.y()<=rect.bottomRight().y())
                )
        {
            return texts[i];
        }

    }

    return "";



}
