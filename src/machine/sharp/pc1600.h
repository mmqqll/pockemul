#ifndef PC1600_H
#define PC1600_H

//#include <stdlib.h>
//#include <stdio.h>

#include <QMenu>


#include "cextension.h"
//#include "pcxxxx.h"
//#include "Log.h"


#include "lh5803.h"

#include "lh5810.h"

#include "ce150.h"
#include "Connect.h"
#include "Keyb.h"


#include "ce152.h"
#include "hd61102.h"
#include "lu57813p.h"
#include "tc8576p.h"

extern TransMap KeyMap1600[];
extern int KeyMap1600Lenght;

class CZ80;

class CLH5810_PC1600:public CLH5810{
    Q_OBJECT
public:
    bool	init(void);						//initialize
    bool	step(void);
    const char*	GetClassName(){ return("CLH5810_PC1600");}

    CLH5810_PC1600(CPObject *parent=0)	: CLH5810(parent)
    {

    }
    ~CLH5810_PC1600()
    {
    }

};

//typedef struct{
//    bool ce_151,ce_155,ce_161,ce_159,ce_150,ce_158;
//}	TExtension;

class Cpc1600:public CpcXXXX{

Q_OBJECT

protected slots:
    void    LoadSIO(void);


public:
    const char*	GetClassName(){ return("Cpc1600");}

    void	ReadQuarterTape(void);
    void    Hack_CRVA(void);
    FILE*   fp_CRVA;

    bool	LoadConfig(QXmlStreamReader *);
    bool	SaveConfig(QXmlStreamWriter *);
    bool	InitDisplay(void);
    bool	CompleteDisplay(void);

    bool	run(void);				// emulator main
    void	Set_Port(PORTS Port,BYTE data);
    BYTE	Get_Port(PORTS Port);

    virtual bool	Mem_Mirror(DWORD *d);
    void	TurnON(void);
    void	TurnOFF(void);
    void    Reset(void);

    void	Regs_Info(UINT8 Type);
    bool	lh5810_write(void);
    bool	lh5810_read(void);

    virtual bool		Chk_Adr(DWORD *d,DWORD data);
    virtual bool		Chk_Adr_R(DWORD *d, DWORD *data);
    UINT8		in(UINT8 address);
    UINT8       out(UINT8 address,UINT8 value);
    bool		Set_Connector(void);
    bool		Get_Connector(void);
    BYTE        getKey(void);

    CZ80            *pZ80;
    CLH5803         *pLH5803;

     bool       masterCPU;
     bool       cpuSwitchPending;

    CLU57813P   *pLU57813P;

    CLH5810_PC1600	*pLH5810;
    Cce152_PC15XX	*pce152;

    CHD61102        *pHD61102_1;
    CHD61102        *pHD61102_2;

    CTC8576P        *pTC8576P;

    Cconnector	*pADCONNECTOR;		qint64 pADCONNECTOR_value;

    bool		lh5810_Access;
    bool        ce150_connected;
    bool		ce1600_connected;
    bool		ce150_Access;

    UINT8 bank1,bank2,bank3,bank4;

    void		InitCE150(void);

    void		initExtension(void);
    bool	init(void);				// initialize

    void    hack(DWORD pc);

    Cpc1600(CPObject *parent = 0);
    virtual ~Cpc1600();

protected slots:
    void contextMenuEvent ( QContextMenuEvent * event );
};



#define S1_EXTENSION_CE151				ext_MemSlot1->ExtArray[ID_CE151]
#define S1_EXTENSION_CE155				ext_MemSlot1->ExtArray[ID_CE155]
#define S1_EXTENSION_CE159				ext_MemSlot1->ExtArray[ID_CE159]
#define S1_EXTENSION_CE160				ext_MemSlot1->ExtArray[ID_CE160]
#define S1_EXTENSION_CE161				ext_MemSlot1->ExtArray[ID_CE161]
#define S1_EXTENSION_CE1600M			ext_MemSlot1->ExtArray[ID_CE1600M]
#define S1_EXTENSION_CE1601M			ext_MemSlot1->ExtArray[ID_CE1601M]

#define S1_EXTENSION_CE151_CHECK		S1_EXTENSION_CE151->IsChecked
#define S1_EXTENSION_CE155_CHECK		S1_EXTENSION_CE155->IsChecked
#define S1_EXTENSION_CE159_CHECK		S1_EXTENSION_CE159->IsChecked
#define S1_EXTENSION_CE160_CHECK		S1_EXTENSION_CE160->IsChecked
#define S1_EXTENSION_CE161_CHECK		S1_EXTENSION_CE161->IsChecked
#define S1_EXTENSION_CE1600M_CHECK		S1_EXTENSION_CE1600M->IsChecked
#define S1_EXTENSION_CE1601M_CHECK		S1_EXTENSION_CE1601M->IsChecked

#define S2_EXTENSION_CE161				ext_MemSlot2->ExtArray[ID_CE161]
#define S2_EXTENSION_CE1600M			ext_MemSlot2->ExtArray[ID_CE1600M]
#define S2_EXTENSION_CE1601M			ext_MemSlot2->ExtArray[ID_CE1601M]
#define S2_EXTENSION_CE16096			ext_MemSlot2->ExtArray[ID_CE16096]
#define S2_EXTENSION_CE16128			ext_MemSlot2->ExtArray[ID_CE16128]
#define S2_EXTENSION_CE16160			ext_MemSlot2->ExtArray[ID_CE16160]
#define S2_EXTENSION_CE16192			ext_MemSlot2->ExtArray[ID_CE16192]
#define S2_EXTENSION_CE16224			ext_MemSlot2->ExtArray[ID_CE16224]
#define S2_EXTENSION_CE16256			ext_MemSlot2->ExtArray[ID_CE16256]


#define S2_EXTENSION_CE161_CHECK		S2_EXTENSION_CE161->IsChecked
#define S2_EXTENSION_CE1600M_CHECK		S2_EXTENSION_CE1600M->IsChecked
#define S2_EXTENSION_CE1601M_CHECK		S2_EXTENSION_CE1601M->IsChecked
#define S2_EXTENSION_CE16096_CHECK		S2_EXTENSION_CE16096->IsChecked
#define S2_EXTENSION_CE16128_CHECK		S2_EXTENSION_CE16128->IsChecked
#define S2_EXTENSION_CE16160_CHECK		S2_EXTENSION_CE16160->IsChecked
#define S2_EXTENSION_CE16192_CHECK		S2_EXTENSION_CE16192->IsChecked
#define S2_EXTENSION_CE16224_CHECK		S2_EXTENSION_CE16224->IsChecked
#define S2_EXTENSION_CE16256_CHECK		S2_EXTENSION_CE16256->IsChecked

#endif // PC1600_H
