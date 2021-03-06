#ifndef LCDC_PC1600_H
#define LCDC_PC1600_H

#include "Lcdc.h"
#include "hd61102.h"

class Clcdc_pc1600:public Clcdc{
public:
    void disp(void);				//display LCDC data to screen
    void disp_symb(void);
    const char*	GetClassName(){ return("Clcdc_pc1500");}

    int computeSL(CHD61102* pCtrl,int ord);
    int symbSL(int x);
    int x2a[100];


    Clcdc_pc1600(CPObject *parent = 0)	: Clcdc(parent){						//[constructor]
        Color_Off.setRgb(
                            (int) (95*contrast),
                            (int) (119*contrast),
                            (int) (193*contrast));
    };
    virtual ~Clcdc_pc1600()
    {						//[constructor]
    };

};

#endif // LCDC_PC1600_H
