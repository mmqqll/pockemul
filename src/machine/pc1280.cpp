#include "common.h"
#include "Log.h"

#include "pc1280.h"
#include "init.h"
#include "Keyb1280.h"

Cpc1280::Cpc1280(CPObject *parent)	: Cpc1360(parent)
{								//[constructor]
    setfrequency(768000/3);
    setcfgfname("pc1280");

    SessionHeader	= "PC1280PKM";
    SessionHeaderLen= 9;
    Initial_Session_Fname ="pc1280.pkm";

    BackGroundFname	= ":/PC1280/pc1280/pc-1280.png";
    LcdFname		= ":/PC1280/pc1280/1280lcd.jpg";
    SymbFname		= ":/PC1280/pc1280/1280symb.jpg";
    memsize			= 0x40000;
//		NbSlot		= 14;

    SlotList.clear();
    SlotList.append(CSlot(8 , 0x0000 ,	":/PC1280/pc1280/cpu-1280.rom"	, "pc-1280/cpu-1280.rom" , ROM , "CPU ROM"));
    SlotList.append(CSlot(8 , 0x2000 ,	""								, "pc-1280/R1-1280.ram" , RAM , "RAM"));
    SlotList.append(CSlot(16, 0x4000 ,	":/PC1280/pc1280/b0-1280.rom"	, "pc-1280/b0-1280.rom" , ROM , "BANK 1"));
    SlotList.append(CSlot(32, 0x8000 ,	""								, "pc-1280/R2-1280.ram" , RAM , "RAM"));
    SlotList.append(CSlot(16, 0x10000 ,	":/PC1280/pc1280/b0-1280.rom"	, "pc-1280/b0-1280.rom" , ROM , "BANK 1"));
    SlotList.append(CSlot(16, 0x14000 ,	":/PC1280/pc1280/b1-1280.rom"	, "pc-1280/b1-1280.rom" , ROM , "BANK 2"));
    SlotList.append(CSlot(16, 0x18000 ,	":/PC1280/pc1280/b2-1280.rom"	, "pc-1280/b2-1280.rom" , ROM , "BANK 3"));
    SlotList.append(CSlot(16, 0x1C000 ,	":/PC1280/pc1280/b3-1280.rom"	, "pc-1280/b3-1280.rom" , ROM , "BANK 4"));
    SlotList.append(CSlot(16, 0x20000 ,	":/PC1280/pc1280/b4-1280.rom"	, "pc-1280/b4-1280.rom" , ROM , "BANK 1"));
    SlotList.append(CSlot(16, 0x24000 ,	":/PC1280/pc1280/b5-1280.rom"	, "pc-1280/b5-1280.rom" , ROM , "BANK 2"));
    SlotList.append(CSlot(16, 0x28000 ,	":/PC1280/pc1280/b6-1280.rom"	, "pc-1280/b6-1280.rom" , ROM , "BANK 3"));
    SlotList.append(CSlot(16, 0x2C000 ,	":/PC1280/pc1280/b7-1280.rom"	, "pc-1280/b7-1280.rom" , ROM , "BANK 4"));
    SlotList.append(CSlot(32, 0x30000 ,	""								, "pc-1280/B0-1280.ram" , RAM , "RAM S1"));
    SlotList.append(CSlot(32, 0x38000 ,	""								, "pc-1280/B1-1280.ram" , RAM , "RAM S2"));

    KeyMap = KeyMap1280;
    KeyMapLenght = KeyMap1280Lenght;

    Pc_DX_mm = 135;
    Pc_DY_mm = 141;
    Pc_DZ_mm = 10;

    Pc_DX = 483;
    Pc_DY = 502;

    delete pLCDC;		pLCDC		= new Clcdc_pc1475(this);
    delete pKEYB;		pKEYB		= new Ckeyb(this,"pc1280.map",scandef_pc1280);


    Lcd_X		= 80;
    Lcd_Y		= 48;
    Lcd_DX		= 264;
    Lcd_DY		= 30;
    Lcd_ratio_X	= 1.22;
    Lcd_ratio_Y	= 1.22;

    Lcd_Symb_X	= 80;
    Lcd_Symb_Y	= 40;
    Lcd_Symb_DX	= 264;
    Lcd_Symb_DY	= 48;
    Lcd_Symb_ratio_X = 1.22;
    //Lcd_Symb_ratio_Y = 1.22;

     remove(pSIOCONNECTOR);
}

bool Cpc1280::init(void) {
    Cpc1360::init();
    WatchPoint.remove(&pSIOCONNECTOR_value);
    delete pSIOCONNECTOR;
}

void Cpc1280::initExtension(void)
{

    // initialise ext_MemSlot1
    ext_MemSlot1 = new CExtensionArray("Memory Slot 1","Add memory credit card");
    ext_MemSlot1->setAvailable(ID_CE210M,true);		ext_MemSlot1->setChecked(ID_CE212M,true);
    ext_MemSlot1->setAvailable(ID_CE211M,true);
    ext_MemSlot1->setAvailable(ID_CE212M,true);
    ext_MemSlot1->setAvailable(ID_CE2H16M,true);
    ext_MemSlot1->setAvailable(ID_CE2H32M,true);
    ext_MemSlot1->setAvailable(ID_CE2H64M,true);

    addExtMenu(ext_MemSlot1);

    extensionArray[0] = ext_11pins;
    extensionArray[1] = ext_MemSlot1;

}

bool Cpc1280::Chk_Adr(DWORD *d,DWORD data)
{


    if ( (*d>=0x0000) && (*d<=0x1FFF) )	return(0);	// ROM area(0000-1fff)
    if ( (*d>=0x2000) && (*d<=0x27FF) )	return(0);
    if ( (*d>=0x2800) && (*d<=0x2B7B) ) { pLCDC->SetDirtyBuf(*d-0x2800);return(1);			/* LCDC (0200x) */	}
    if ( (*d>=0x3400) && (*d<=0x35FF) )	{ RomBank = data &0x07;	return(1); }
#if 0
    if ( (*d>=0x3C00) && (*d<=0x3DFF) )
    {
//		AddLog(LOG_MASTER,"Write Slot Register %04X=%02X",*d,mem[*d]);
        if (pCPU->fp_log) fprintf(pCPU->fp_log,"NEW BANK CHANGE =%i\n",RamBank);

        RamBank = (data == 0x04 ? 0 : 1);
        if (pCPU->fp_log) fprintf(pCPU->fp_log,"NEW BANK CHANGE =%i from data (%02x)\n",RamBank,data);
        return(1);
    }
#endif
    if ( (*d>=0x3E00) && (*d<=0x3FFF) )
    {
        int K=0;
        switch (data & 0x0F)
        {
        case 0x00: K=0x00;	break;
        case 0x01: K=0x01;	break;
        case 0x02: K=0x02;	break;
        case 0x03: K=0x04;	break;
        case 0x04: K=0x08;	break;
        case 0x05: K=0x10;	break;
        case 0x06: K=0x20;	break;
        case 0x07: K=0x40;	break;
        case 0x08: K=0x80;	break;
        case 0x09: K=0x00;	break;
        case 0x0A: K=0x00;	break;
        case 0x0B: K=0x00;	break;
        case 0x0C: K=0x00;	break;
        case 0x0D: K=0x00;	break;
        case 0x0E: K=0x00;	break;
        case 0x0F: K=0x7F;	break;
        }
        pKEYB->Set_KS( K );
        return(1);
    }




    if ( (*d>=0x2800) && (*d<=0x3FFF) )	return(1);
    if ( (*d>=0x4000) && (*d<=0x7FFF) )	{ *d += 0xC000 + ( RomBank * 0x4000 );	return(0); }


    if ( (*d>=0x8000) && (*d<=0xFFFF) )
    {
        int od = *d;
        bool ret = false;
        *d += 0x28000 + (RamBank * 0x8000);
        if ( (*d>=0x30000) && (*d<=0x33FFF) )	ret=(S1_EXTENSION_CE2H64M_CHECK | S1_EXTENSION_CE2H32M_CHECK);
        if ( (*d>=0x34000) && (*d<=0x35FFF) )	ret=(S1_EXTENSION_CE2H64M_CHECK | S1_EXTENSION_CE2H32M_CHECK | S1_EXTENSION_CE2H16M_CHECK);
        if ( (*d>=0x36000) && (*d<=0x36FFF) )	ret=(S1_EXTENSION_CE2H64M_CHECK | S1_EXTENSION_CE2H32M_CHECK | S1_EXTENSION_CE2H16M_CHECK | S1_EXTENSION_CE212M_CHECK);
        if ( (*d>=0x37000) && (*d<=0x377FF) )	ret=(S1_EXTENSION_CE2H64M_CHECK | S1_EXTENSION_CE2H32M_CHECK | S1_EXTENSION_CE2H16M_CHECK | S1_EXTENSION_CE212M_CHECK | S1_EXTENSION_CE211M_CHECK);
        if ( (*d>=0x37800) && (*d<=0x37FFF) )	ret=(S1_EXTENSION_CE2H64M_CHECK | S1_EXTENSION_CE2H32M_CHECK | S1_EXTENSION_CE2H16M_CHECK | S1_EXTENSION_CE212M_CHECK | S1_EXTENSION_CE211M_CHECK | S1_EXTENSION_CE210M_CHECK);
        if ( (*d>=0x38000) && (*d<=0x3FFFF) )	ret=(S1_EXTENSION_CE2H64M_CHECK);
        if (pCPU->fp_log) fprintf(pCPU->fp_log,"BANK (%i) ECRITURE [%04x](%05x)=%02x (%c) : %s\n",RamBank,od,*d,data,data,ret?"OK":"NO");
        return (ret);
//        if ( (*d>=0x38000) && (*d<=0x3BFFF) )	return(S2_EXTENSION_CE2H32M_CHECK);
//        if ( (*d>=0x3C000) && (*d<=0x3DFFF) )	return(S2_EXTENSION_CE2H32M_CHECK | S2_EXTENSION_CE2H16M_CHECK);
//        if ( (*d>=0x3E000) && (*d<=0x3EFFF) )	return(S2_EXTENSION_CE2H32M_CHECK | S2_EXTENSION_CE2H16M_CHECK | S2_EXTENSION_CE212M_CHECK);
//        if ( (*d>=0x3F000) && (*d<=0x3F7FF) )	return(S2_EXTENSION_CE2H32M_CHECK | S2_EXTENSION_CE2H16M_CHECK | S2_EXTENSION_CE212M_CHECK | S2_EXTENSION_CE211M_CHECK);
//        if ( (*d>=0x3F800) && (*d<=0x3FFFF) )	return(S2_EXTENSION_CE2H32M_CHECK | S2_EXTENSION_CE2H16M_CHECK | S2_EXTENSION_CE212M_CHECK | S2_EXTENSION_CE211M_CHECK | S2_EXTENSION_CE210M_CHECK);

    }

    return(0);
}

bool Cpc1280::Chk_Adr_R(DWORD *d,DWORD data)
{
    if ( (*d>=0x0000) && (*d<=0x1FFF) )	return(1);	// ROM area(0000-1fff)

    if ( (*d>=0x4000) && (*d<=0x7FFF) )	{ *d += 0xC000 + ( RomBank * 0x4000 ); return(1); }	// Manage ROM Bank
    if (pCPU->fp_log) fprintf(pCPU->fp_log,"LECTURE [%04x]\n",*d);
    if ( (*d>=0x8000) && (*d<=0xFFFF) )	{ *d += 0x28000 + ( RamBank * 0x8000 );}	// Manage ram bank

    return(1);
}

bool Cpc1280::Set_Connector(void)
{
// ces 2 ports sont bien acc�d�s par le 1280
    int port1 = Get_8(0x3800);
    int port2 = Get_8(0x3A00);

    pCONNECTOR->Set_pin(PIN_MT_OUT2	,0);
    pCONNECTOR->Set_pin(PIN_VGG		,1);
    pCONNECTOR->Set_pin(PIN_BUSY	,READ_BIT(port1,0));
    if (!pCONNECTOR->Get_pin(PIN_ACK)) pCONNECTOR->Set_pin(PIN_D_OUT	,READ_BIT(port2,2));
    pCONNECTOR->Set_pin(PIN_MT_OUT1	,pCPU->Get_Xout());
    if (!pCONNECTOR->Get_pin(PIN_ACK)) pCONNECTOR->Set_pin(PIN_D_IN	,READ_BIT(port2,3));
    if (!pCONNECTOR->Get_pin(PIN_ACK)) pCONNECTOR->Set_pin(PIN_SEL2	,READ_BIT(port2,1));
    if (!pCONNECTOR->Get_pin(PIN_ACK)) pCONNECTOR->Set_pin(PIN_SEL1	,READ_BIT(port2,0));
    pCONNECTOR->Set_pin(PIN_MT_OUT1	,pCPU->Get_Xout());


    return(1);
}

bool Cpc1280::Get_Connector(void)
{
    Set_Port_Bit(PORT_B,1,pCONNECTOR->Get_pin(PIN_SEL1));	// DIN	:	IB1
    Set_Port_Bit(PORT_B,2,pCONNECTOR->Get_pin(PIN_SEL2));	// DIN	:	IB2
    Set_Port_Bit(PORT_B,3,pCONNECTOR->Get_pin(PIN_D_OUT));	// DIN	:	IB2


    Set_Port_Bit(PORT_B,4,pCONNECTOR->Get_pin(PIN_D_IN));	// DIN	:	IB8
    Set_Port_Bit(PORT_B,5,pCONNECTOR->Get_pin(PIN_ACK));	// ACK	:	IB7
    pCPU->Set_Xin(pCONNECTOR->Get_pin(PIN_MT_IN));

    return(1);
}
