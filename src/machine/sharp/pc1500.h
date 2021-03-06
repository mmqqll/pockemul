#ifndef _PC1500_H
#define _PC1500_H

#include <stdlib.h>
#include <stdio.h>

#include <QMenu>
#include "common.h"

//#include "cextension.h"
#include "pcxxxx.h"
//#include "Log.h"
#include "lh5801.h"
#include "lh5810.h"
#include "pd1990ac.h"
//#include "ce150.h"
//#include "Connect.h"
#include "Keyb.h"

#include "ce152.h"

class CPD1990AC;

class CLH5810_PC1500:public CLH5810{
    Q_OBJECT
public:
	bool	init(void);						//initialize
	bool	step(void);
    const char*	GetClassName(){ return("CLH5810_PC1500");}
	CPD1990AC	*pPD1990AC;

	CLH5810_PC1500(CPObject *parent)	: CLH5810(parent)
	{
		pPD1990AC	= new CPD1990AC(parent);
    }
	~CLH5810_PC1500()
	{
		delete pPD1990AC;
    }


};


class Cpc15XX:public CpcXXXX{

Q_OBJECT

public:
    const char*	GetClassName(){ return("Cpc15XX");}

	void	ReadQuarterTape(void);
	bool	LoadConfig(QFile *file);
	bool	SaveConfig(QFile *file);
    bool	LoadConfig(QXmlStreamReader *);
    bool	SaveConfig(QXmlStreamWriter *);
	bool	InitDisplay(void);
	bool	CompleteDisplay(void);

	bool	run(void);				// emulator main
	void	Set_Port(PORTS Port,BYTE data);
	BYTE	Get_Port(PORTS Port);

	virtual bool	Mem_Mirror(DWORD *d); 
	void	TurnON(void);

	void	Regs_Info(UINT8 Type);


	bool		lh5810_write(void);
	bool		lh5810_read(void);

	virtual bool		Chk_Adr(DWORD *d,DWORD data);
    virtual bool		Chk_Adr_R(DWORD *d, DWORD *data);
    UINT8		in(UINT8 address);
    UINT8 out(UINT8 address,UINT8 value){return(1);}
	bool		Set_Connector(void);
	bool		Get_Connector(void);

	CLH5810_PC1500	*pLH5810;
	Cce152_PC15XX	*pce152;
	
	bool		lh5810_Access;
	bool		ce150_connected;
	bool		ce150_Access;

	void		InitCE150(void);
	
	void		initExtension(void);
	bool	init(void);				// initialize
	
	Cpc15XX(CPObject *parent = 0);

    virtual ~Cpc15XX();

protected slots:
	void contextMenuEvent ( QContextMenuEvent * event );
};

class Cpc1500:public Cpc15XX{						//PC1500 emulator main class
public:
    const char*	GetClassName(){ return("Cpc1500");}

	Cpc1500(CPObject *parent = 0)	: Cpc15XX(this)
	{								//[constructor]
        setcfgfname(QString("pc1500"));

		SlotList.clear();
		SlotList.append(CSlot(8 , 0x0000 ,	""								, "" , RAM , "RAM"));
		SlotList.append(CSlot(8 , 0x2000 ,	""								, "" , ROM , "ROM"));
		SlotList.append(CSlot(16, 0x4000 ,	""								, "" , RAM , "RAM"));
		SlotList.append(CSlot(8 , 0x8000 ,	""								, "" , NOTUSED , "NOT USED"));
		SlotList.append(CSlot(8 , 0xA000 ,	""								, "" , ROM , "ROM"));
        SlotList.append(CSlot(16, 0xC000 ,	P_RES(":/pc1500/SYS1500.ROM")	, "" , ROM , "SYSTEM ROM"));
		SlotList.append(CSlot(64, 0x10000 ,	""								, "" , RAM , "RAM"));
		SlotList.append(CSlot(8 , 0x20000 ,	""								, "" , ROM , "ROM"));
		SlotList.append(CSlot(8 , 0x22000 ,	""								, "" , ROM , "ROM"));
        SlotList.append(CSlot(8 , 0x24000 ,	P_RES(":/pc1500/CE-150.ROM"), "" , ROM , "CE-150 ROM"));
	}

	~Cpc1500()
	{								//[constructor]
	}
private:

};

class Cpc1500A:public Cpc15XX{						//PC1500 emulator main class
public:
    const char*	GetClassName(){ return("Cpc1500A");}


	bool Chk_Adr(DWORD *d,DWORD data);
    bool Chk_Adr_R(DWORD *d, DWORD *data);
	bool Mem_Mirror(DWORD *d); 

    Cpc1500A(CPObject *parent = 0);

	~Cpc1500A()
	{								//[constructor]
	}
private:

};

class Ctrspc2:public Cpc1500{
public:
    const char*	GetClassName(){ return("Ctrspc2");}

    Ctrspc2(CPObject *parent = 0);

	~Ctrspc2()
	{								//[constructor]
	}
};


#endif
