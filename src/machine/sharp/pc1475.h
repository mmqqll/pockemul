#ifndef _PC1475_H_
#define _PC1475_H_

#include "pc1360.h"



class Cpc1475:public Cpc1360{						//PC1350 emulator main class
Q_OBJECT
public:
    const char*	GetClassName(){ return("Cpc1475");}

    virtual bool Chk_Adr(DWORD *d,DWORD data);
    virtual bool Chk_Adr_R(DWORD *d, DWORD *data);

    Cpc1475(CPObject *parent = 0);
    virtual ~Cpc1475()
	{								//[constructor]
	}
private:
};

#endif
