#ifndef LCDC_HP41_H
#define LCDC_HP41_H

#include <QTimer>

#include "Lcdc.h"
#include "hp/hp41.h"
#include "hp/hp41Cpu.h"

class Clcdc_hp41:public Clcdc{
Q_OBJECT
public:
    void disp(void);				//display LCDC data to screen
    void disp_symb(void);
    const char*	GetClassName(){ return("Clcdc_hp41");}


    Clcdc_hp41(CPObject *parent = 0)	: Clcdc(parent){						//[constructor]
        Color_Off.setRgb(
                            (int) (111*contrast),
                            (int) (117*contrast),
                            (int) (108*contrast));

        hp41 = (Chp41*) parent;
        hp41cpu = hp41->hp41cpu;
    }
    virtual ~Clcdc_hp41()
    {						//[constructor]
    }

    Chp41 *hp41;
    Chp41Cpu *hp41cpu;


    QString GetLCD();
};



#endif // LCDC_HP41_H
