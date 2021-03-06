#ifndef _PC1403_H
#define _PC1403_H

#include <stdlib.h>
#include <stdio.h>

#include "pc1401.h"
#include "sc61860.h"
#include "Keyb.h"
#include "Connect.h"

extern TransMap KeyMap1403[];
extern int KeyMap1403Lenght; 

class Cpc1403:public Cpc1401{						//PC1403 emulator main class
Q_OBJECT
public:
    const char*	GetClassName(){ return("Cpc1403");}


    virtual bool	Set_Connector(void);
    virtual bool	Get_Connector(void);


    virtual bool Chk_Adr(DWORD *d,DWORD data);
    virtual bool Chk_Adr_R(DWORD *d, DWORD *data);
    virtual UINT8 in(UINT8 address){return(1);}
    virtual UINT8 out(UINT8 address,UINT8 value){return(1);}

    BYTE out5;
    DWORD memOffset;

    Cpc1403(CPObject *parent = 0);
    virtual ~Cpc1403()
	{								//[constructor]
    }
    BYTE Get_PortA();
private:
};

class Cpc1403H:public Cpc1403{						
Q_OBJECT
public:
    const char*	GetClassName(){ return("Cpc1403H");}


    virtual bool Chk_Adr(DWORD *d,DWORD data);
    virtual bool Chk_Adr_R(DWORD *d, DWORD *data);

    Cpc1403H(CPObject *parent=0);

	~Cpc1403H()
	{								//[constructor]
	}

private:
};




#endif
