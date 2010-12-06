#include "common.h"
#include "Log.h"

#include "pc1280.h"

void	Cpc1280::initExtension(void)
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
    pCONNECTOR->Set_pin(PIN_D_OUT	,READ_BIT(port2,2));
    pCONNECTOR->Set_pin(PIN_D_IN	,READ_BIT(port1,3));
    pCONNECTOR->Set_pin(PIN_SEL2	,READ_BIT(port2,1));
    pCONNECTOR->Set_pin(PIN_SEL1	,READ_BIT(port2,0));
    pCONNECTOR->Set_pin(PIN_MT_OUT1	,pCPU->Get_Xout());


    return(1);
}

bool Cpc1280::Get_Connector(void)
{
    Set_Port_Bit(PORT_B,4,pCONNECTOR->Get_pin(PIN_D_IN));	// DIN	:	IB8
    Set_Port_Bit(PORT_B,5,pCONNECTOR->Get_pin(PIN_ACK));	// ACK	:	IB7
    pCPU->Set_Xin(pCONNECTOR->Get_pin(PIN_MT_IN));

    return(1);
}