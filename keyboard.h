#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "qobject.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qrect.h"
class keyboard
{
public:
    keyboard(QString configFile);
    QString getText(QPoint pt);
private:
    std::vector<QRect> rects;
    std::vector<QString> texts;

};

#endif // KEYBOARD_H
