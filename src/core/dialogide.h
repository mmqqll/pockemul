#ifndef DIALOGIDE_H
#define DIALOGIDE_H

//
#include "ui_dialogide.h"

#include "ide/highlighter.h"
#include "lcc/lcpp.h"
#include "lcc/lcc.h"
//
class DialogIDE : public QDialog, public Ui::DialogIDE
{
Q_OBJECT
public:
    DialogIDE( QWidget * parent = 0, Qt::WFlags f = 0 );

    void setupEditor(void);

    Highlighter *highlighter;
    Clcpp *lcpp;
    Clcc *lcc;
private slots:
    void start(void);
    void output(QString,QString);
};


#endif // DIALOGIDE_H