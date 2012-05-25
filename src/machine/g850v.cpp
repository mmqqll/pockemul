
#include "g850v.h"
#include "Connect.h"

#include "z80.h"
#include "Inter.h"
#include "Keyb.h"

Cg850v::Cg850v(CPObject *parent)	: CpcXXXX(this)
{								//[constructor]
#ifndef QT_NO_DEBUG
    if (!fp_log) fp_log=fopen("g850.log","wt");	// Open log file
#endif

    setfrequency( (int) 8000000);
    ioFreq = 0;
    setcfgfname(QString("g850"));

    SessionHeader	= "G850PKM";
    SessionHeaderLen= 7;
    Initial_Session_Fname ="g850.pkm";
    BackGroundFname	= ":/G850V/g850v.png";
    LcdFname		= ":/G850V/g850vlcd.png";
    SymbFname		= ":/G850V/g850vsymb.png";
    memsize			= 0x020000;
    InitMemValue	= 0x00;

    SlotList.clear();


    Pc_Offset_X = Pc_Offset_Y = 0;


    setDXmm(196);//Pc_DX_mm = 195;
    setDYmm(95);//Pc_DY_mm = 86;
    setDZmm(20);//Pc_DZ_mm = 25;

    setDX(683);//Pc_DX		= 679; //572;
    setDY(330);//Pc_DY		= 299;//254;

    Lcd_X		= 60;
    Lcd_Y		= 50;
    Lcd_DX		= 156;
    Lcd_DY		= 32;
    Lcd_ratio_X	= 2;
    Lcd_ratio_Y	= 2;

    Lcd_Symb_X	= 60;
    Lcd_Symb_Y	= 40;
    Lcd_Symb_DX	= 317;
    Lcd_Symb_DY	= 5;

    SoundOn			= false;

//    pLCDC		= new Clcdc_pc1600(this);
//    pLCDC->Color_Off.setRgb(
//                        (int) (95*pLCDC->contrast),
//                        (int) (119*pLCDC->contrast),
//                        (int) (103*pLCDC->contrast));


    pCPU        = new CZ80(this);
    pTIMER		= new Ctimer(this);

    pCONNECTOR	= new Cconnector(this,11,0,Cconnector::Sharp_11,"Connector 11 pins",false,QPoint(0,60));	publish(pCONNECTOR);
    pSIOCONNECTOR=new Cconnector(this,15,1,Cconnector::Sharp_15,"Connector 15 pins",false,QPoint(679,190));	publish(pSIOCONNECTOR);


    pKEYB		= new Ckeyb(this,"g850.map");

    Tape_Base_Freq=2500;

    SlotList.clear();

    SlotList.append(CSlot(16, 0x08000 ,	":/G850V/rom00.bin"         , "" , ROM , "ROM BANK 00"));
    SlotList.append(CSlot(16, 0x0C000 ,	":/G850V/rom01.bin"         , "" , ROM , "ROM BANK 01"));

    SlotList.append(CSlot(16, 0x10000 ,	":/G850V/rom02.bin"         , "" , ROM , "ROM BANK 02"));
    SlotList.append(CSlot(16, 0x14000 ,	":/G850V/rom03.bin"         , "" , ROM , "ROM BANK 03"));
    SlotList.append(CSlot(16, 0x18000 ,	":/G850V/rom04.bin"         , "" , ROM , "ROM BANK 04"));
    SlotList.append(CSlot(16, 0x1C000 ,	":/G850V/rom05.bin"         , "" , ROM , "ROM BANK 05"));

    SlotList.append(CSlot(16, 0x10000 ,	":/G850V/rom06.bin"         , "" , ROM , "ROM BANK 06"));
    SlotList.append(CSlot(16, 0x14000 ,	":/G850V/rom07.bin"         , "" , ROM , "ROM BANK 07"));
    SlotList.append(CSlot(16, 0x18000 ,	":/G850V/rom08.bin"         , "" , ROM , "ROM BANK 08"));
    SlotList.append(CSlot(16, 0x1C000 ,	":/G850V/rom09.bin"         , "" , ROM , "ROM BANK 09"));

    SlotList.append(CSlot(16, 0x10000 ,	":/G850V/rom0a.bin"         , "" , ROM , "ROM BANK 0a"));
    SlotList.append(CSlot(16, 0x14000 ,	":/G850V/rom0b.bin"         , "" , ROM , "ROM BANK 0b"));
    SlotList.append(CSlot(16, 0x18000 ,	":/G850V/rom0c.bin"         , "" , ROM , "ROM BANK 0c"));
    SlotList.append(CSlot(16, 0x1C000 ,	":/G850V/rom0d.bin"         , "" , ROM , "ROM BANK 0d"));

    SlotList.append(CSlot(16, 0x10000 ,	":/G850V/rom0e.bin"         , "" , ROM , "ROM BANK 0e"));
    SlotList.append(CSlot(16, 0x14000 ,	":/G850V/rom0f.bin"         , "" , ROM , "ROM BANK 0f"));
    SlotList.append(CSlot(16, 0x18000 ,	":/G850V/rom10.bin"         , "" , ROM , "ROM BANK 10"));
    SlotList.append(CSlot(16, 0x1C000 ,	":/G850V/rom11.bin"         , "" , ROM , "ROM BANK 11"));

    SlotList.append(CSlot(16, 0x10000 ,	":/G850V/rom12.bin"         , "" , ROM , "ROM BANK 12"));
    SlotList.append(CSlot(16, 0x14000 ,	":/G850V/rom13.bin"         , "" , ROM , "ROM BANK 13"));
    SlotList.append(CSlot(16, 0x18000 ,	":/G850V/rom14.bin"         , "" , ROM , "ROM BANK 14"));
    SlotList.append(CSlot(16, 0x1C000 ,	":/G850V/rom15.bin"         , "" , ROM , "ROM BANK 15"));


}

Cg850v::~Cg850v()
{

}


bool Cg850v::init()
{
}


bool Cg850v::Set_Connector()
{
}

bool Cg850v::Get_Connector()
{
}


void Cg850v::TurnON()
{
}

void Cg850v::Reset()
{
}

bool Cg850v::Chk_Adr(DWORD *d, DWORD data)
{
}

bool Cg850v::Chk_Adr_R(DWORD *d, DWORD data)
{
}

UINT8 Cg850v::in(UINT8 address)
{
}

UINT8 Cg850v::out(UINT8 address, UINT8 value)
{
}


bool Cg850v::Mem_Mirror(DWORD *d)
{
}


bool Cg850v::run()
{
}

void Cg850v::Set_Port(PORTS Port, BYTE data)
{
}

BYTE Cg850v::Get_Port(PORTS Port)
{
}


bool Cg850v::LoadConfig(QXmlStreamReader *)
{
}

bool Cg850v::SaveConfig(QXmlStreamWriter *)
{
}

bool Cg850v::InitDisplay()
{
    CpcXXXX::InitDisplay();

    return(1);
}

bool Cg850v::CompleteDisplay()
{
}
