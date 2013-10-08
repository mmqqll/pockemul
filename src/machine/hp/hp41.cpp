// *********************************************************************
//    Copyright (c) 1989-2002  Warren Furlow
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// *********************************************************************

// *********************************************************************
// HP41.cpp : implementation file
// *********************************************************************
#if 0
#include <math.h>
#include "hp41.h"

// Class 0 Type A Parameter, Zenrom Pg 83
//            0 1 2  3 4 5  6 7 8 9 a  b c  d e f
word TypeA[]={3,4,5,10,8,6,11,0,2,9,7,13,1,12,0,0};

Chp41 *Chp41::pThis=NULL;


/****************************/
// Constructor
/****************************/
Chp41::Chp41()
  {
  pThis=this;

  // trace
  hLogFile=NULL;
  InstSetIndex=0;
  TEFIndex=0;
  fTrace=FALSE;
  memset(szTraceOut,0,sizeof(szTraceOut));

  // graphics
  pBMKeyboard=NULL;
  pPalKeyboard=NULL;
  pPalIndexKeyboard=NULL;
  fUsePal=FALSE;

  // keyboard and font
  pRectKeyboard=NULL;
  pRectLCD=NULL;
  pRectAnnun=NULL;
  eKeyboard=0;
  eFont=0;
  UseAltPunc=0;

  // activity dot
  Indicator=0;
  RectIndicator.left=3;
  RectIndicator.top=3;
  RectIndicator.right=10;
  RectIndicator.bottom=10;

  // ROM variables
  for (int page=0;page<=0xf;page++)
    {
    for (int bank=1;bank<=4;bank++)
      PageMatrix[page][bank-1]=NULL;
    active_bank[page]=1;
    }
  pCurPage=NULL;
  CurPage=0;
  NextActualBankGroup=1;

  // ram registers
  pRAM=(RAM_REG*)malloc(sizeof(RAM_REG)*MAX_RAM);
  memset(pRAM,0,sizeof(RAM_REG)*MAX_RAM);

  // CPU registers
  memset(A_REG,0,sizeof(A_REG));
  memset(B_REG,0,sizeof(B_REG));
  memset(C_REG,0,sizeof(C_REG));
  memset(M_REG,0,sizeof(M_REG));
  memset(N_REG,0,sizeof(N_REG));
  G_REG=0;
  F_REG=0;
  ST_REG=0;
  Q_REG=0;
  P_REG=0;
  PT_REG=&P_REG;
  PT_PREV=0;
  KEY_REG=0;
  XST_REG=0;
  FI_REG=0;
  CARRY=0;
  KEYDOWN=0;
  BATTERY=0;
  PC_LAST=0;
  PC_TRACE=0;
  PC_REG=0;
  RET_STK0=0;
  RET_STK1=0;
  RET_STK2=0;
  RET_STK3=0;
  BASE=16;

  // CPU variables
  eSleepMode=eDeepSleep;
  perph_in_control=0;
  perph_selected=0;
  ram_selected=0;
  control_perph=0;
  TytePrev=0;
  Tyte1=0;
  Modifier=0;
  FirstTEF=0;
  LastTEF=0;

  // display registers and variables
  UpdateDisplay=1;
  UpdateAnnun=1;
  Boost=0;
  DisplayOn=0;
  memset(DIS_C_REG,0,sizeof(DIS_C_REG));
  memset(DIS_B_REG,0,sizeof(DIS_B_REG));
  memset(DIS_A_REG,0,sizeof(DIS_A_REG));
  DIS_ANNUN_REG=0;
  Contrast=DEFAULT_CONTRAST;  // default halfnut value
  FontColor=0;                // black

  // instruction delay
  QueryPerformanceFrequency(&PCPerf);
  memset(PCCount,0,sizeof(PCCount));
  InstrNSec=0;

  // control and state variables
  MinCLRKEY=0;
  fEnableCLRKEY=FALSE;
  ExecuteEvent=0;
  ProcInterval=0;
  ProcCycles=0;
  MemModules=0;
  XMemModules=0;
  fPrinter=FALSE;
  fCardReader=FALSE;
  fTimer=FALSE;
  fWand=FALSE;
  fHPIL=FALSE;
  fInfrared=FALSE;
  SoundMode=eSoundNone;
  fRunEnable=FALSE;
  fBreakPtsEnable=FALSE;
  fBreak=FALSE;
  nBreakPts=0;
  }


/****************************/
// destructor
/****************************/
Chp41::~Chp41()
  {
  if (GetTrace())
    StopTrace();
  SetSoundMode(eSoundNone);

  DeInitTimer();
  ExecuteEvent=0;

  // free modules and rom pages
  while (!ModuleList.IsEmpty())
    {
    ModuleHeader *pModule=(ModuleHeader *)ModuleList.RemoveTail();
    delete pModule;
    }
  for (int page=0;page<=0xf;page++)
    for (int bank=1;bank<=4;bank++)
      FreePage(page,bank);

  free(pRAM);

  SetKeyboard(eKeyboardNone,0,0,0);
  }


/*****************************/
// wakeup if everything is ready
/*****************************/
void Chp41::Wakeup()
  {
  if ((PageMatrix[0][0]==NULL)||(eKeyboard==eKeyboardNone))     // not fully initialized yet
    return;
  if (pCurPage==NULL)
    MemoryLost();

  eSleepMode=eAwake;
  }


/*****************************/
// puts the processor to sleep if it is awake
/*****************************/
void Chp41::Sleep(int eNewMode)      // pass either Light or Deep sleep
  {
  if (eSleepMode==eAwake)
    eSleepMode=eNewMode;
  }


/*****************************/
// returns 0 if awake, nonzero if sleeping
/*****************************/
int Chp41::IsSleeping()
  {
  return(eSleepMode);
  }


/*****************************/
void Chp41::MemoryLost()
  {
  if (eKeyboard==eKeyboardNone)
    return;
  BASE=16;
  for (int page=1;page<=0xf;page++)
    active_bank[page]=1;
  SetPC(0x0232);
  ResetTimer();
  }


/*****************************/
void Chp41::EnableRun()
  {
  fRunEnable=TRUE;
  }


/*****************************/
void Chp41::DisableRun()
  {
  fRunEnable=FALSE;
  }


/*****************************/
// initialize or reinitialize the execution callback
/*****************************/
void Chp41::Run()
  {
  if (!fRunEnable)
    return;
  if (ExecuteEvent)
    return;
  if (!ProcInterval || !ProcCycles)  // if unintialized
    return;

  // compute delay for 1 HP41 machine cycle - necessary for correct operation of speaker sound & interval timer
  InstrNSec=(UINT64)( ((double)PCPerf.QuadPart*(double)DEFAULT_INST_SPEED/1e6));
  WaveSound.SetDuration((DWORD)DEFAULT_INST_SPEED);

  // compute delay for 1 HP41 machine cycle - necessary for correct operation of speaker sound & interval timer
  // scales delay according to ProcInterval, where the default is the same as the real machine
  InstrNSec=(UINT64)( ((double)PCPerf.QuadPart*(double)DEFAULT_INST_SPEED/1e6)*((double)ProcInterval/(double)DEFAULT_PROC_INTERVAL) );
  WaveSound.SetDuration( (DWORD)( (double)DEFAULT_INST_SPEED*(double)ProcInterval/(double)DEFAULT_PROC_INTERVAL ) );

  // validate PC_REG - important if a ROM is removed from the config while it is being executed
  uint page=(PC_REG&0xf000)>>12;
  if (PageMatrix[page][active_bank[page]-1]==NULL)           // no rom loaded here
    SetPC(0);

  ExecuteEvent=SetTimer(NULL,0,ProcInterval,ExecuteProc);
  }


/*****************************/
// stops the callback timer
/*****************************/
void Chp41::Halt()
  {
  if (ExecuteEvent)
    KillTimer(NULL,ExecuteEvent);
  ExecuteEvent=0;
  }


/*****************************/
void Chp41::SetProcParams(
  int ProcIntervalIn,            // time in milliseconds between processor runs
  int ProcCyclesIn)              // number of processor cycles to run each time
  {
  ProcInterval=ProcIntervalIn;
  ProcCycles=ProcCyclesIn;
  if (ExecuteEvent)              // restart processor with new values
    {
    Halt();
    Run();
    }
  }


/*****************************/
void Chp41::GetProcParams(
  int &ProcIntervalOut,          // time in milliseconds between processor runs
  int &ProcCyclesOut)            // number of processor cycles to run each time
  {
  ProcIntervalOut=ProcInterval;
  ProcCyclesOut=ProcCycles;
  }


/****************************/
void Chp41::SetSoundMode(int val)
  {
  if (GetVersion()&0x80000000)   // high bit set if win95/98
    Speaker(0,1);	             // be sure speaker is off

  if (val==eSoundSpeaker)
    {
    if (PCPerf.QuadPart &&           // 0 if performance counter not supported
      (GetVersion()&0x80000000) )    // high order bit set if win95/98/win32s - sound not supported on NT
      SoundMode=val;
    else
      SoundMode=eSoundNone;
    }
  else
    SoundMode=val;
  }


/****************************/
int Chp41::GetSoundMode()
  {
  return(SoundMode);
  }


/****************************/
word inline Chp41::GetNextTyte()
  {
  return(pCurPage->Image[PC_REG++&0xfff]);
  }


/****************************/
// for absolute changes - always leaves page variables valid
/****************************/
void Chp41::SetPC(word addr)
  {
  ModulePage *pPage;
  word page;

  page=(addr&0xf000)>>12;
  pPage=PageMatrix[page][active_bank[page]-1];
  if (pPage==NULL)       // there is no page there
    {
    CurPage=(PC_REG&0xf000)>>12;
    pCurPage=PageMatrix[CurPage][active_bank[CurPage]-1];
    return;
    }
  PC_REG=addr;
  CurPage=page;
  pCurPage=pPage;
  }


/****************************/
// return TRUE if RAM exists at the specified RAM address
/****************************/
int Chp41::RamExist(word addr)
  {
  if ((addr>=0x000)&&(addr<=0x00f))      // status registers
    return(1);
  if ((addr>=0x010)&&(addr<=0x03f))      // void
    return(0);
  if ((addr>=0x040)&&(addr<=0x0bf))      // extended functions - 128 regs
    return(XMemModules>=1);
  if ((addr>=0x0c0)&&(addr<=0xff))       // main memory for C
    return(1);
  if ((addr>=0x100)&&(addr<=0x13f))      // memory module 1
    return(MemModules>=1);
  if ((addr>=0x140)&&(addr<=0x17f))      // memory module 2
    return(MemModules>=2);
  if ((addr>=0x180)&&(addr<=0x1bf))      // memory module 3
    return(MemModules>=3);
  if ((addr>=0x1c0)&&(addr<=0x1ff))      // memory module 4
    return(MemModules>=4);
  // void: 200
  if ((addr>=0x201)&&(addr<=0x2ef))      // extended memory 1 - 239 regs
    return(XMemModules>=2);
  // void: 2f0-300
  if ((addr>=0x301)&&(addr<=0x3ef))      // extended memory 2 - 239 regs
    return(XMemModules>=3);
  // void: 3f0-3ff
  // end of memory: 3ff
  return(0);
  }


/*****************************/
// timer for execution loop
/****************************/
void CALLBACK Chp41::ExecuteProc(
  HWND hwnd,          // handle of window for timer messages
  UINT uMsg,          // WM_TIMER message
  UINT idEvent,       // timer identifier
  DWORD dwTime)       // current system time in millisec
  {
  for (int i=0;i<pThis->ProcCycles;i++)
    {
    if (pThis->GetSoundMode()==eSoundWave)                              // if wave mode, put must be called continuously to minimize clicks
      {
      static int SkipSound=0;
      if (SkipSound)
        SkipSound--;
      else if (WaveSound.Put(pThis->F_REG)==-1)                         // returns -1 if failed to open
        SkipSound=10000;                                                // skip sound for a while because Put() slows way down while waiting for sound board to free up
      }
    else if (pThis->IsSleeping())
      break;
    if (pThis->GetSoundMode()==eSoundSpeaker || (pThis->TMR_S[2]&0x04)) // override ProcInterval to slow down execution to match real hardware
      {
      QueryPerformanceCounter(&pThis->PCCount[0]);	                    // get current count
      pThis->Execute();                                                 // execute one instruction
      pThis->PCCount[0].QuadPart+=pThis->InstrNSec;
      do
        QueryPerformanceCounter(&pThis->PCCount[1]);
      while (pThis->PCCount[1].QuadPart < pThis->PCCount[0].QuadPart);  // delay until exact amount of time for real machine cycle has passed
      }
    else if (!pThis->IsSleeping())                                      // dont call execute while sleeping because it will execute the next instruction anyway
      pThis->Execute();                                                 // execute one instruction
    if (pThis->Boost && i+pThis->Boost>=pThis->ProcCycles)              // boost this loop for a few more cycles
      {                                                                 // to prevent it from exiting in the middle of heavy display operations
      i-=pThis->Boost;
      pThis->Boost=0;
      }
    if (pThis->fBreak)                                                  // breakpoint has been raised
      {
      theApp.pMainWnd->OpenConsole();
      theApp.pMainWnd->OpenBreakpoints();
      pThis->fBreak=FALSE;
      break;
      }
    }
  if (pThis->Indicator)
    theApp.m_pMainWnd->InvalidateRect(&pThis->RectIndicator,FALSE);     // activity indicator
  if (pThis->UpdateDisplay)
    theApp.m_pMainWnd->InvalidateRect(pThis->pRectLCD,FALSE);           // refresh LCD
  if (pThis->UpdateAnnun)
    theApp.m_pMainWnd->InvalidateRect(pThis->pRectAnnun,FALSE);         // refresh Annunciator
  }


/****************************/
// executes one instruction
/****************************/
void Chp41::Execute()
  {
  if (GetTrace())
    TraceOut();
  PC_LAST=PC_REG;

  PT_PREV=*PT_REG;

  TytePrev=Tyte1;
  Tyte1=GetNextTyte();

  if (perph_in_control)
    {
    switch (control_perph)
      {
      case 9:    // printer
        exec_perph_printer();
        break;
      default:
        error_message(30);
        break;
      }
    perph_in_control=!(Tyte1&0x001);
    }
  else              /* execute mcode */
    {
    switch(Tyte1&3)
      {
      case 0 :     /* misc */
        Class0();
        break;
      case 1 :    /* long jumps */
        Class1();
        break;
      case 2 :   /* TEF */
        Class2();
        break;
      case 3 :   /* short jumps */
        Class3();
        break;
      }
    }
  // detect breakpoint
  if (fBreakPtsEnable)
    for (int i=0;i<nBreakPts;i++)
      if (PC_REG==BreakPts[i])
        fBreak=TRUE;
  }


/* ------------------------------------------------------------------------ */
/*                 CLASS 0 INSTRUCTIONS                                     */
/* ------------------------------------------------------------------------ */

/****************************/
/* execute class 0 into subclasses */
/****************************/
void Chp41::Class0()
  {
  Modifier=(Tyte1&0x03c0)>>6;
  switch ((Tyte1&0x003c)>>2)
    {
    case 0:
      Subclass0();      /*  reads Modifier and writes to inst_str */
      break;
    case 1:
      Subclass1();
      break;
    case 2:
      Subclass2();
      break;
    case 3:
      Subclass3();
      break;
    case 4:
      Subclass4();
      break;
    case 5:
      Subclass5();
      break;
    case 6:
      Subclass6();
      break;
    case 7:
      Subclass7();
      break;
    case 8:
      Subclass8();
      break;
    case 9:
      Subclass9();
      break;
    case 10:
      SubclassA();
      break;
    case 11:
      SubclassB();
      break;
    case 12:
      SubclassC();
      break;
    case 13:
      SubclassD();
      break;
    case 14:
      SubclassE();
      break;
    case 15:
      SubclassF();
      break;
    }
  }


/****************************/
void Chp41::Subclass0()
  {
  switch(Modifier)
    {
    case 0:  /* NOP */
      break;
    case 1:  /* WMLDL */
      {
      word page=C_REG[6];
      ModulePage *pPage=PageMatrix[page][active_bank[page]-1];
      if (pPage==NULL || !pPage->fRAM || pPage->fWriteProtect)
        break;
      word addr=(C_REG[5]<<8) | (C_REG[4]<<4) | (C_REG[3]);
      word Tyte=((C_REG[2]&0x03)<<8) | (C_REG[1]<<4) | (C_REG[0]);
      pPage->Image[addr]=Tyte;
      pPage->pModule->Original=0;
      break;
      }
    case 4:  /* ENBANK1 */
      Enbank(1);
      break;
    case 6:  /* ENBANK2  */
      Enbank(2);
      break;
    case 5:  /* ENBANK3  */
      Enbank(3);
      break;
    case 7:  /* ENBANK4  */
      Enbank(4);
      break;
    case 2:  /* NOT USED */
    case 3:
      /* Not Used */
      break;
    default:  /* HPIL=C 0-7 */
      break;
    }
  CARRY=0;
  }


/****************************/
void Chp41::Enbank(int BankSet)
  {
  uint page,bank;
  if (pCurPage==NULL || pCurPage->ActualBankGroup==0)
    return;
  // search for banks that match ActualBankGroup
  for (page=0;page<=0xf;page++)
    {
    for (bank=1;bank<=4;bank++)
      {
      if (PageMatrix[page][bank-1]!=NULL &&
        PageMatrix[page][BankSet-1]!=NULL &&
        PageMatrix[page][bank-1]->ActualBankGroup==pCurPage->ActualBankGroup)
        {
        active_bank[page]=BankSet;
        }
      }
    }
  SetPC(PC_REG);
  }

/****************************/
void Chp41::Subclass1()
  {
  if (Modifier==7)
    {
    /* Not Used */
    }
  else if (Modifier==15)            /* ST=0 */
    {
    ST_REG=0;
    }
  else                             /* CF 0-13 */
    {
    word nFlag=TypeA[Modifier];
    if (nFlag<=7)
      ST_REG&=~(1<<nFlag);
    else
      XST_REG&=~(1<<(nFlag-8));
    }
  CARRY=0;
  }


/****************************/
void Chp41::Subclass2()
  {
  if (Modifier==7)
    {
    /* Not Used */
    }
  else if (Modifier==15)             /* CLRKEY */
    {
    if (MinCLRKEY)
      MinCLRKEY--;
    if (fEnableCLRKEY && (MinCLRKEY==0))   // dont 0 key until enabled and this minimum number of CLRKEY instructions has passed
      {
      KEYDOWN=0;
      KEY_REG=0;
      }
    }
  else                               /* SF 0-13 */
    {
    word nFlag=TypeA[Modifier];
    if (nFlag<=7)
      ST_REG|=(1<<nFlag);
    else
      XST_REG|=1<<(nFlag-8);
    }
  CARRY=0;
  }


/****************************/
void Chp41::Subclass3()
  {
  if (Modifier==7)
    {
    /* Not Used */
    CARRY=0;
    }
  else if (Modifier==15)                   /* ?KEY */
    {
    CARRY=KEYDOWN;
    }
  else                                     /* ?FS 0-13 */
    {
    word nFlag=TypeA[Modifier];
    if (nFlag<=7)
      CARRY=(ST_REG&(1<<nFlag))?1:0;
    else
      CARRY=(XST_REG&(1<<(nFlag-8)))?1:0;
    }
  }


/****************************/
void Chp41::Subclass4()     /* LC 0-15 (HP) OR LC 0-9,A-F (OTHERS) */
  {
  C_REG[*PT_REG]=(byte)Modifier;
  if (*PT_REG==0)
    *PT_REG=13;
  else
    (*PT_REG)--;
  CARRY=0;
  }


/****************************/
void Chp41::Subclass5()
  {
  if (Modifier==7)
    {
    /* Not Used */
    CARRY=0;
    }
  else if (Modifier==15)                   /* -PT */
    {
    if (*PT_REG==0)
      *PT_REG=13;
    else
      (*PT_REG)--;
    CARRY=0;
    }
  else                                    /* ?PT 0-15 */
    {
    CARRY=(*PT_REG==TypeA[Modifier]);
    }
  }


/****************************/
void Chp41::Subclass6()
  {
  switch(Modifier)
    {
    case 1:  /* G=C */
      {
      // 2 special cases: David Assembler pg 59,60 and NEWT Microprocessor Tech Manual pg 84
      if (*PT_REG==13)
        {
        if (TytePrev!=0x2d4 && TytePrev!=0x3d4 && TytePrev!=0x3dc)       // case 1c: PT is 13 and last inst was not: PT=13, -PT, +PT
          {
          G_REG=(C_REG[13]<<4)|(C_REG[0]);
          }
        else                                                             // case 2c: PT is 13 and last inst was: PT=13, -PT, +PT
          {
          G_REG=(C_REG[13]<<4)|((G_REG&0xf0)>>4);
          }
        }
      else                                                               // normal case
        {
        G_REG=(C_REG[*PT_REG+1]<<4)|(C_REG[*PT_REG]);
        }
      break;
      }
    case 2:  /* C=G */
      {
      // 3 special cases: David Assembler pg 59,60 and NEWT Microprocessor Tech Manual pg 58,59
      if (*PT_REG==13)
        {
        if (TytePrev!=0x2d4 && TytePrev!=0x3d4 && TytePrev!=0x3dc)       // case 1b: PT is 13 and last inst was not: PT=13, -PT, +PT
          {
          C_REG[13]=(byte)((G_REG&0xf0)>>4);
          C_REG[0]=(byte)(G_REG&0x0f);
          }
        else                                                             // case 2b: PT is 13 and last inst was: PT=13, -PT, +PT
          {
          C_REG[13]=(byte)(G_REG&0x0f);
          G_REG=((G_REG&0x0f)<<4)|((G_REG&0xf0)>>4);
          }
        }
      else if (PT_PREV==13 && (TytePrev==0x3d4 ||                       // case 3b: PT was 13 but last inst was -PT or PT=1..12
        TytePrev&0x01c || TytePrev&0x05c || TytePrev&0x09c || TytePrev&0x0dc || TytePrev&0x11c || TytePrev&0x15c || TytePrev&0x19c || TytePrev&0x21c || TytePrev&0x25c || TytePrev&0x29c || TytePrev&0x31c || TytePrev&0x35c || TytePrev&0x39c))
        {                                                               // this case was incompletely described in David Assm. Newt describes it as an additional case but the end result is same
        G_REG=(((G_REG&0x0f)<<4)|((G_REG&0xf0)>>4));
        C_REG[*PT_REG+1]=(byte)((G_REG&0xf0)>>4);
        C_REG[*PT_REG]=(byte)(G_REG&0x0f);
        C_REG[0]=C_REG[*PT_REG+1];
        }
      else                                                              // normal case
        {
        C_REG[*PT_REG+1]=(byte)((G_REG&0xf0)>>4);
        C_REG[*PT_REG]=(byte)(G_REG&0x0f);
        }
      break;
      }
    case 3:  /* C<>G */
      {
      word TMP_REG;
      // 3 special cases: David Assembler pg 59,60 and NEWT Microprocessor Tech Manual pg 66,67
      if (*PT_REG==13)
        {
        if (TytePrev!=0x2d4 && TytePrev!=0x3d4 && TytePrev!=0x3dc)      // case 1a: PT is 13 and last inst was not: PT=13, -PT, +PT
          {
          TMP_REG=G_REG;
          G_REG=((C_REG[13]<<4)|C_REG[0]);
          C_REG[13]=(byte)((TMP_REG&0xf0)>>4);
          C_REG[0]=(byte)(TMP_REG&0x0f);
          }
        else                                                            // case 2a: PT is 13 and last inst was: PT=13, -PT, +PT
          {
          TMP_REG=G_REG;
          G_REG=((C_REG[13]<<4)|((G_REG&0xf0)>>4));
          C_REG[13]=(byte)(TMP_REG&0x0f);
          }
        }
      else if (PT_PREV==13 && (TytePrev==0x3d4 ||                       // case 3a: PT was 13 but last inst was -PT or PT=1..12
        TytePrev&0x01c || TytePrev&0x05c || TytePrev&0x09c || TytePrev&0x0dc || TytePrev&0x11c || TytePrev&0x15c || TytePrev&0x19c || TytePrev&0x21c || TytePrev&0x25c || TytePrev&0x29c || TytePrev&0x31c || TytePrev&0x35c || TytePrev&0x39c))
        {                                                               // this case was incompletely described in David Assm. Newt describes it as an additional case but the end result is same
        TMP_REG=G_REG;
        G_REG=((C_REG[*PT_REG+1]<<4)|C_REG[*PT_REG]);
        C_REG[*PT_REG+1]=C_REG[0];
        C_REG[*PT_REG]=(byte)((TMP_REG&0xf0)>>4);
        C_REG[0]=(byte)(TMP_REG&0x0f);
        }
      else                                                              // normal case
        {
        TMP_REG=G_REG;
        G_REG=((C_REG[*PT_REG+1]<<4)|C_REG[*PT_REG]);
        C_REG[*PT_REG+1]=(byte)((TMP_REG&0xf0)>>4);
        C_REG[*PT_REG]=(byte)(TMP_REG&0x0f);
        }
      break;
      }
    case 5:  /* M=C */
      {
      memcpy(M_REG,C_REG,14);
      break;
       }
    case 6:  /* C=M */
      {
      memcpy(C_REG,M_REG,14);
      break;
      }
    case 7:  /* C<>M */
      {
      byte TMP_REG[14];
      memcpy(TMP_REG,M_REG,14);
      memcpy(M_REG,C_REG,14);
      memcpy(C_REG,TMP_REG,14);
      break;
      }
    case 9:  /* F=ST */
      {
      F_REG=ST_REG;
      if (SoundMode==eSoundSpeaker)
        Speaker(F_REG,1);
      break;
      }
    case 10:  /* ST=F */
      {
      ST_REG=F_REG;
      break;
      }
    case 11:  /* ST<>F */
      {
      // this is used to create tones.  Normally FF and 00 are switched back and forth.
      // one machine cycle is about 158 microseconds.
      // frequency = 1/((number of FFh cycles + number of 00h cycles)x 158 10E-6
      // Hepax Vol II pg 111,131, Zenrom Manual pg. 81, HP41 Schematic
      word TMP_REG;
      TMP_REG=F_REG;
      F_REG=ST_REG;
      ST_REG=TMP_REG;
      if (SoundMode==eSoundSpeaker)
        Speaker(F_REG,1);
      break;
      }
    case 13:  /* ST=C */
      {
      ST_REG=(C_REG[1]<<4)|(C_REG[0]);
      break;
      }
    case 14:  /* C=ST */
      {
      C_REG[1]=(byte)((ST_REG&0xf0)>>4);
      C_REG[0]=(byte)(ST_REG&0x0f);
      break;
      }
    case 15:  /* C<>ST */
      {
      word tmp1=C_REG[1];
      word tmp0=C_REG[0];
      C_REG[1]=(byte)((ST_REG&0xf0)>>4);
      C_REG[0]=(byte)(ST_REG&0x0f);
      ST_REG=(tmp1<<4)|tmp0;
      break;
      }
    default :  /* NOT USED */
      {
      /* Not Used */
      break;
      }
    }
  CARRY=0;
  }


/****************************/
void Chp41::Subclass7()
  {
  if (Modifier==7)
    {
    /* Not Used */
    }
  else if (Modifier==15)                   /* +PT */
    {
    if (*PT_REG==13)
      *PT_REG=0;
    else
      (*PT_REG)++;
    }
  else                                    /* PT 0-15 */
    {
    *PT_REG=TypeA[Modifier];
    }
  CARRY=0;
  }


/****************************/
/* MISC */
/****************************/
void Chp41::Subclass8()
  {
  switch(Modifier)
    {
    case 0:  /* CLRRTN */
      {
      PopReturn();
      CARRY=0;
      break;
      }
    case 1:  /* POWOFF */
      {
      for (int page=0;page<=0xf;page++)      // thanks to Antonio Lagana for this which fixed a problem with Hepax
        active_bank[page]=1;
      SetPC(0);
      if (DisplayOn)              // display is on, go into light sleep
        {
        CARRY=0;
        Sleep(eLightSleep);
        }
      else                        // display is off, go into deep sleep
        {
        CARRY=1;
        Sleep(eDeepSleep);        // deep sleep
        }
      break;
      }
    case 2:  /* PT=P */
      {
      PT_REG=&P_REG;
      CARRY=0;
      break;
      }
    case 3:  /* PT=Q */
      {
      PT_REG=&Q_REG;
      CARRY=0;
      break;
      }
    case 4:  /* ?P=Q */
      {
      CARRY=(P_REG==Q_REG);
      break;
      }
    case 5:  /* ?BAT */
      {
      CARRY=BATTERY;
      break;
      }
    case 6:  /* ABC=0 */
      {
      memset(A_REG,0,14);
      memset(B_REG,0,14);
      memset(C_REG,0,14);
      CARRY=0;
      break;
      }
    case 7:  /* GTOC */
      {
      SetPC((C_REG[6]<<12)|(C_REG[5]<<8)|(C_REG[4]<<4)|(C_REG[3]));
      CARRY=0;
      break;
      }
    case 8:   /* C=KEY */
      {
      C_REG[4]=(byte)((KEY_REG&0x00f0)>>4);
      C_REG[3]=(byte)(KEY_REG&0x000f);
      CARRY=0;
      break;
      }
    case 9:  /* SETHEX */
      {
      BASE=16;
      CARRY=0;
      break;
      }
    case 10:  /* SETDEC */
      {
      BASE=10;
      CARRY=0;
      break;
      }
    case 11:  /* DISOFF */
      {
      DisplayOn=0;
      UpdateDisplay=1;
      Boost=DISPLAY_BOOST;
      UpdateAnnun=1;
      CARRY=0;
      break;
      }
    case 12:  /* DISTOG */
      {
      DisplayOn^=1;
      UpdateDisplay=1;
      UpdateAnnun=1;
      CARRY=0;
      break;
      }
    case 13:   /* CRTN */
      {
      if (CARRY)
        SetPC(PopReturn());
      CARRY=0;
      break;
      }
    case 14:  /* NCRTN */
      {
      if (!CARRY)
        SetPC(PopReturn());
      CARRY=0;
      break;
      }
    case 15:  /* RTN */
      {
      SetPC(PopReturn());
      CARRY=0;
      break;
      }
    }
  }


/****************************/
void Chp41::Subclass9()
  {
  CARRY=0;
  if (Modifier!=9)
    {
    error_message(30);
    }
  else          /* 0-15 (HP) OR PERTCT 0-9,A-F (OTHERS) */
    {
    if (PageMatrix[6][0]==NULL)  // is printer ROM loaded
      return;
    perph_in_control=1;
    control_perph=Modifier;
    }
  }


/****************************/
void Chp41::SubclassA()
  {
  if (perph_selected==0 || ram_selected<=0x00F || ram_selected>=0x020)
    {
    ram_selected=(ram_selected&0x3f0)|Modifier;
    wdata();
    }
  else switch(perph_selected)
    {
    case 0:      /* REG=C - already wrote to ram */
      break;
    case 0x10:   /* halfnut display */
      {
      HalfnutWrite();
      break;
      }
    case 0xfb:   /* timer write */
      {
      if (fTimer)
        TimerWrite();
      break;
      }
    case 0xfc:   /* card reader */
      {
      error_message(30);
      break;
      }
    case 0xfd:   /* lcd display */
      {
      DisplayWrite();
      break;
      }
    case 0xfe:   /* wand */
      {
      error_message(30);
      break;
      }
    default:
      {
      error_message(31);
      break;
      }
    }
  CARRY=0;
  }


/****************************/
void Chp41::SubclassB()
  {
  if ((Modifier==7) || (Modifier==15))    // Not Used
    CARRY=0;
  else                                    // ?PF 0-13
//RB++
// since we currently support only one device (the timer) feeding FI, we can do it this way...
// if we use other peripherals, we have to catch all the flag 13s from each device AND there own flags
// (here 12 for TIMER) ONLY IF DEVICE IS SELECTED. Should be done at a central location like ::Execute()
// it should be done like this:
//
// FI_REG = 0;
// for( n = 0; n < peripheralcount; n++ )
// {
//   if (perph_selected==PERFID[n])        // peripheral is selected, take all FO_BITS
//     FI_REG |= FO_REG[n];                // each peripheral has its own FO_REG
//   else                                  // peripheral is not selected, take only FO_REG bit 13
//     FI_REG |= (FO_REG[n]&(1<<13));      // each peripheral has its own FO_REG
// }
    CARRY=(FI_REG&(1<<TypeA[Modifier]))?1:0;
//RB--
  }


/****************************/
void Chp41::SubclassC()
  {
  switch(Modifier)
    {
    case 0:
      {
      // ROMBLK - Eramco pg. 125
      // Moves Hepax ROM to page specified in C[0]- only known to work with HEPAX module
      // HEPAX may be on top of a RAM page and when it moves the RAM (alternate) becomes visible
      word dest=C_REG[0],src,bank;
      for (src=5;src<0xf;src++)       // find hepax
        {
        for (bank=1;bank<=4;bank++)
          {
          if (PageMatrix[src][bank-1]==NULL || !PageMatrix[src][bank-1]->fHEPAX || PageMatrix[src][bank-1]->fRAM)    // only move hepax ROM and not RAM
            continue;
          ModulePage *pDestAltPage=PageMatrix[dest][bank-1];          // if there is something at the dest page, save it as the dest alt page (does not normally happen)
          PageMatrix[dest][bank-1]=PageMatrix[src][bank-1];
          PageMatrix[src][bank-1]=PageMatrix[src][bank-1]->pAltPage;  // move src alt page to primary now that hepax is moved off it
          PageMatrix[dest][bank-1]->pAltPage=pDestAltPage;
          PageMatrix[dest][bank-1]->ActualPage=(byte)dest;
          }
        }
      break;
      }
    case 1:  /* N=C */
      {
      memcpy(N_REG,C_REG,14);
      break;
      }
    case 2:  /* C=N */
      {
      memcpy(C_REG,N_REG,14);
      break;
      }
    case 3:  /* C<>N */
      {
      byte TMP_REG[14];
      memcpy(TMP_REG,N_REG,14);
      memcpy(N_REG,C_REG,14);
      memcpy(C_REG,TMP_REG,14);
      break;
      }
    case 4:  /* LDI */
      {
      Tyte2=GetNextTyte();
      C_REG[2]=(byte)((Tyte2&0x0300)>>8);
      C_REG[1]=(byte)((Tyte2&0x00f0)>>4);
      C_REG[0]=(byte)(Tyte2&0x000f);
      break;
      }
    case 5:  /* STK=C */
      {
      PushReturn((C_REG[6]<<12)|(C_REG[5]<<8)|(C_REG[4]<<4)|(C_REG[3]));
      break;
      }
    case 6:  /* C=STK */
      {
      word addr=PopReturn();
      C_REG[6]=(byte)((addr&0xf000)>>12);
      C_REG[5]=(byte)((addr&0x0f00)>>8);
      C_REG[4]=(byte)((addr&0x00f0)>>4);
      C_REG[3]=(byte)(addr&0x000f);
      break;
      }
    case 7:   /* WPTOG - Toggles write protection on HEPAX RAM at page C[0] */
      {
      word page=C_REG[0];
      if (PageMatrix[page][0]==NULL || !PageMatrix[page][0]->fHEPAX || !PageMatrix[page][0]->fRAM )
        break;
      PageMatrix[page][0]->fWriteProtect=!PageMatrix[page][0]->fWriteProtect;
      break;
      }
    case 8:  /* GTOKEY */
      {
      SetPC((PC_REG&0xff00)|KEY_REG);
      break;
      }
    case 9:  /* RAMSLCT */
      {
      ram_selected=((C_REG[2]&0x03)<<8) | (C_REG[1]<<4) | (C_REG[0]);
      break;
      }
    case 10:   /* NOT USED  */
      {
      break;
      }
    case 11:  /* WDATA */
      {
      switch(perph_selected)
        {
        case 0:       // RAM
          wdata();
          break;
        case 0xfd:    // Display
          AnnunWrite();
          break;
        case 0xfb:    // Timer
          wdata();
          break;
        default:
          error_message(30);
          break;
        }
      break;
      }
    case 12:  /* RDROM */
      {
      word page=C_REG[6];
      word addr=(C_REG[5]<<8) | (C_REG[4]<<4) | (C_REG[3]);
      ModulePage *pPage=PageMatrix[page][active_bank[page]-1];
      word Tyte;
      if (pPage==NULL) // read 0 from nonexistant location
        Tyte=0;
      else
        Tyte=pPage->Image[addr];
      C_REG[2]=(byte)((Tyte&0x0300)>>8);
      C_REG[1]=(byte)((Tyte&0x00f0)>>4);
      C_REG[0]=(byte)(Tyte&0x000f);
      break;
      }
    case 13:  /* C=CORA */
      {
      byte *C_REGpt=C_REG;
      byte *A_REGpt=A_REG;
      while (C_REGpt<C_REG+14)
        {
        *C_REGpt|=*A_REGpt;
        C_REGpt++;
        A_REGpt++;
        }

      // David Assembler pg 60 CPU errors
      // Not sure if Case 5 is included.  Strict reading of description does not include it.
      if (ArithCarry(TytePrev))                         // if previous instruction was arithmetic inst. that could set a carry
        {
        word Modifier=(Tyte1&0x001c)>>2;                 // TEF Modifier
        if ( (Modifier==3) ||                            // Case 1 - ALL
          (Modifier==7) ||                               // Case 2 - S
          (Modifier==0 && *PT_REG==13) ||                // Case 3 - PT and PT=13
          (Modifier==4 && P_REG<=Q_REG && Q_REG==13) ||  // Case 4 - PQ and Q=13 [Q:P]
          //(Modifier==4 && P_REG>Q_REG) ||              // Case 5 - PQ and P>Q [13:P]
          (Modifier==2 && *PT_REG==13) )                 // Case 6 - WPT and PT=13
          {
          C_REG[13]=C_REG[0];
          A_REG[13]=C_REG[0];
          }
        }
      break;
      }
    case 14:  /* C=CANDA */
      {
      byte *C_REGpt=C_REG;
      byte *A_REGpt=A_REG;
      while (C_REGpt<C_REG+14)
        {
        *C_REGpt&=*A_REGpt;
        C_REGpt++;
        A_REGpt++;
        }
      // David Assembler pg 60 CPU errors
      // Not sure if Case 5 is included.  Strict reading of description does not include it.
      if (ArithCarry(TytePrev))                         // if previous instruction was arithmetic inst. that could set a carry
        {
        word Modifier=(Tyte1&0x001c)>>2;                 // TEF Modifier
        if ( (Modifier==3) ||                            // Case 1 - ALL
          (Modifier==7) ||                               // Case 2 - S
          (Modifier==0 && *PT_REG==13) ||                // Case 3 - PT and PT=13
          (Modifier==4 && P_REG<=Q_REG && Q_REG==13) ||  // Case 4 - PQ and Q=13 [Q:P]
          //(Modifier==4 && P_REG>Q_REG) ||              // Case 5 - PQ and P>Q [13:P]
          (Modifier==2 && *PT_REG==13) )                 // Case 6 - WPT and PT=13
          {
          C_REG[13]=C_REG[0];
          A_REG[13]=C_REG[0];
          }
        }
      break;
      }
    case 15:  /* PERSLCT */
      {
      perph_selected=(C_REG[1]<<4) | (C_REG[0]);
      break;
      }
    }
  CARRY=0;
  }


/****************************/
void Chp41::SubclassD()
  {
  /* Not Used */
  CARRY=0;
  }


/****************************/
void Chp41::SubclassE()
  {
  if (perph_selected==0 || ram_selected<=0x00F || ram_selected>=0x020)
    {
    if (Modifier)  /* C=REG */
      ram_selected=(ram_selected&0x3f0)|Modifier;
    rdata();
    }
  else switch(perph_selected)
    {
    case 0:     /* RDATA/C=REG - already took care of ram read*/
      break;
    case 0x10:  /* halfnut display - only known to work for contrast */
      {
      HalfnutRead();
      break;
      }
    case 0xfb:   /* timer */
      {
      if (fTimer)
        TimerRead();
      break;
      }
    case 0xfc:  /* card reader */
      {
      error_message(30);
      break;
      }
    case 0xfd: /* lcd display */
      {
      DisplayRead();
      break;
      }
    case 0xfe:   /* wand */
      {
      error_message(30);
      break;
      }
    default:
      {
      error_message(31);
      break;
      }
    }
  CARRY=0;
  }


/****************************/
void Chp41::SubclassF()
  {
  if (Modifier==7)
    {
    /* Not Used */
    }
  else if (Modifier==15)
    {
    /* LCD Compensation instruction: Display Driver Spec Pg 5 */
    }
  else                                   /* RCR 0-13 */
    {
    int i;
    int num;
    byte TMP_REG[14];
    memcpy(TMP_REG,C_REG,14);
    num=TypeA[Modifier];
    for (i=0;i<14;i++)
      {
      if (i+num>13)
        C_REG[i]=TMP_REG[num+i-14];
      else
        C_REG[i]=C_REG[num+i];
      }
    }
  CARRY=0;
  }


/****************************/
/* WDATA */
/****************************/
void Chp41::wdata()
  {
  if (RamExist(ram_selected))
    {
    int i,j;
    for (i=0,j=0;i<7;i++,j+=2)
      pRAM[ram_selected].Reg[i]=(C_REG[j+1]<<4) | (C_REG[j]);
    }
  }

/****************************/
/* RDATA */
/****************************/
void Chp41::rdata()
  {
  if (RamExist(ram_selected))
    {
    int i,j;
    for (i=0,j=0;i<7;i++,j+=2)
      {
      C_REG[j]=(pRAM[ram_selected].Reg[i])&0x0f;
      C_REG[j+1]=((pRAM[ram_selected].Reg[i])&0xf0)>>4;
      }
    }
  else
    {
    memset(C_REG,0,14);
    }
  }

/* ------------------------------------------------------------------------ */
/*                           CLASS 1 INSTRUCTIONS                           */
/* ------------------------------------------------------------------------ */


/****************************/
/* execute class 1 into long jumps */
/****************************/
void Chp41::Class1()
  {
  Tyte2=GetNextTyte();
  word addr=((Tyte2&0x03fc)<<6|(Tyte1&0x03fc)>>2);
  switch (Tyte2&0x0003)
    {
    case 0:              /* NCXQ */
      {
      if (!CARRY)
        LongJump(addr,TRUE);
      break;
      }
    case 1:              /* CQX */
      {
      if (CARRY)
        LongJump(addr,TRUE);
      break;
      }
    case 2:              /* NCGO */
      {
      if (!CARRY)
        LongJump(addr,FALSE);
      break;
      }
    case 3:              /* CGO */
      {
      if (CARRY)
        LongJump(addr,FALSE);
      break;
      }
    }
  CARRY=0;
  }


/****************************/
void Chp41::LongJump(word addr,flag fPush)
  {
  word page=(addr&0xf000)>>12;
  ModulePage *pPage=PageMatrix[page][active_bank[page]-1];
  if (pPage==NULL)                       // there is no page there
    return;
  if (pPage->Image[addr&0x0fff]==0)      // long jumps dont jump to NOP
    return;
  if (fPush)
    PushReturn(PC_REG);
  PC_REG=addr;
  CurPage=page;
  pCurPage=pPage;
  }


/****************************/
void Chp41::PushReturn(word addr)
  {
  RET_STK3=RET_STK2;
  RET_STK2=RET_STK1;
  RET_STK1=RET_STK0;
  RET_STK0=addr;
  }

/****************************/
word Chp41::PopReturn()
  {
  word addr=RET_STK0;
  RET_STK0=RET_STK1;
  RET_STK1=RET_STK2;
  RET_STK2=RET_STK3;
  RET_STK3=0;
  return(addr);
  }


/* ------------------------------------------------------------------------ */
/*                           CLASS 2 INSTRUCTIONS                           */
/* ------------------------------------------------------------------------ */


/****************************/
/* execute class 2 into subclasses */
/****************************/
void Chp41::Class2(void)
  {
  int i;
  CARRY=0;
  Modifier=(Tyte1&0x001c)>>2;
  ConvertTEF();
  switch ((Tyte1&0x03e0)>>5)
    {
    case 0:              // A=0
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=0;
      break;
      }
    case 1:             // B=0
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        B_REG[i]=0;
      break;
      }
    case 2:             // C=0
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=0;
      break;
      }
    case 3:            // A<>B
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        {
        byte TMP_REG=A_REG[i];
        A_REG[i]=B_REG[i];
        B_REG[i]=TMP_REG;
        }
      break;
      }
    case 4:            // B=A
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        B_REG[i]=A_REG[i];
      break;
      }
    case 5:            // A<>C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        {
        byte TMP_REG=A_REG[i];
        A_REG[i]=C_REG[i];
        C_REG[i]=TMP_REG;
        }
      break;
      }
    case 6:            // C=B
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=B_REG[i];
      break;
      }
    case 7:            // B<>C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        {
        byte TMP_REG=B_REG[i];
        B_REG[i]=C_REG[i];
        C_REG[i]=TMP_REG;
        }
      break;
      }
    case 8:            // A=C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=C_REG[i];
      break;
      }
    case 9:            // A=A+B
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=Adder(A_REG[i],B_REG[i]);
      break;
      }
    case 10:           // A=A+C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=Adder(A_REG[i],C_REG[i]);
      break;
      }
    case 11:           // A=A+1
      {
      CARRY=1;
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=Adder(A_REG[i],0);
      break;
      }
    case 12:           // A=A-B
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=Subtractor(A_REG[i],B_REG[i]);
      break;
      }
    case 13:           // A=A-1
      {
      CARRY=1;
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=Subtractor(A_REG[i],0);
      break;
      }
    case 14:           // A=A-C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=Subtractor(A_REG[i],C_REG[i]);
      break;
      }
    case 15:           // C=C+C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=Adder(C_REG[i],C_REG[i]);
      break;
      }
    case 16:           // C=A+C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=Adder(A_REG[i],C_REG[i]);
      break;
      }
    case 17:           // C=C+1
      {
      CARRY=1;
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=Adder(C_REG[i],0);
      break;
      }
    case 18:           // C=A-C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=Subtractor(A_REG[i],C_REG[i]);
      break;
      }
    case 19:           // C=C-1
      {
      CARRY=1;
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=Subtractor(C_REG[i],0);
      break;
      }
    case 20:           // C=-C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=Subtractor(0,C_REG[i]);
      break;
      }
    case 21:           // C=-C-1
      {
      CARRY=1;
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=Subtractor(0,C_REG[i]);
      break;
      }
    case 22:           // ?B#0
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        {
        if (B_REG[i])
          {
          CARRY=1;
          break;
          }
        }
      break;
      }
    case 23:           // ?C#0
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        {
        if (C_REG[i])
          {
          CARRY=1;
          break;
          }
        }
      break;
      }
    case 24:           // ?A<C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        Subtractor(A_REG[i],C_REG[i]);
      break;
      }
    case 25:           // ?A<B
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        Subtractor(A_REG[i],B_REG[i]);
      break;
      }
    case 26:           // ?A#0
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        {
        if (A_REG[i])
          {
          CARRY=1;
          break;
          }
        }
      break;
      }
    case 27:           // ?A#C
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        {
        if (A_REG[i]!=C_REG[i])
          {
          CARRY=1;
          break;
          }
        }
      break;
      }
    case 28:           // ASR
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        A_REG[i]=(i==LastTEF)?0:A_REG[i+1];
      break;
      }
    case 29:           // BSR
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        B_REG[i]=(i==LastTEF)?0:B_REG[i+1];
      break;
      }
    case 30:           // CSR
      {
      for (i=FirstTEF;i<=LastTEF;i++)
        C_REG[i]=(i==LastTEF)?0:C_REG[i+1];
      break;
      }
    case 31:           // ASL
      {
      for (i=LastTEF;i>=(int)FirstTEF;i--)
        A_REG[i]=(i==FirstTEF)?0:A_REG[i-1];
      break;
      }
    }
  }


/****************************/
// returns 1 if this is an arithmetic instruction that can set a carry
/****************************/
int Chp41::ArithCarry(unsigned short Tyte)
  {
  if ((Tyte&3)!=2)  // if not a class 2 TEF inst
    return(0);
  switch ((Tyte&0x03e0)>>5)
    {
    case 0:     // A=0
    case 1:     // B=0
    case 2:     // C=0
    case 3:     // A<>B
    case 4:     // B=A
    case 5:     // A<>C
    case 6:     // C=B
    case 7:     // B<>C
    case 8:     // A=C
      return(0);
    case 9:     // A=A+B
    case 10:    // A=A+C
    case 11:    // A=A+1
    case 12:    // A=A-B
    case 13:    // A=A-1
    case 14:    // A=A-C
    case 15:    // C=C+C
    case 16:    // C=A+C
    case 17:    // C=C+1
    case 18:    // C=A-C
    case 19:    // C=C-1
    case 20:    // C=-1
    case 21:    // C=-C-1
    case 22:    // ?B#0
    case 23:    // ?C#0
    case 24:    // ?A<C
    case 25:    // ?A<B
    case 26:    // ?A#0
    case 27:    // ?A#C
      return(1);
    case 28:    // ASR
    case 29:    // BSR
    case 30:    // CSR
    case 31:    // ASL
      return(0);
    }
  return(0);
  }

/****************************/
/* convert tef bits to start and end nybbles */
/* starting from the least significant digit - FirstTEF is to the right of LastTEF */
/****************************/
void Chp41::ConvertTEF()
  {
  switch(Modifier)
    {
    case 0:   /* PT */
      {
      FirstTEF=*PT_REG;
      LastTEF=*PT_REG;
      break;
      }
    case 1:   /* X */
      {
      FirstTEF=0;
      LastTEF=2;
      break;
      }
    case 2:   /* WPT */
      {
      FirstTEF=0;
      LastTEF=*PT_REG;
      break;
      }
    case 3:   /* ALL */
      {
      FirstTEF=0;
      LastTEF=13;
      break;
      }
    case 4:  /* PQ */
      {
      FirstTEF=P_REG;
      if (P_REG<=Q_REG)
        LastTEF=Q_REG;
      else
        LastTEF=13;
      break;
      }
    case 5:   /* XS */
      {
      FirstTEF=2;
      LastTEF=2;
      break;
      }
    case 6:   /* M */
      {
      FirstTEF=3;
      LastTEF=12;
      break;
      }
    case 7:   /* S */
      {
      FirstTEF=13;
      LastTEF=13;
      break;
      }
    }
  }


/****************************/
// digit Adder
/****************************/
byte Chp41::Adder(
  byte nib1,
  byte nib2)
  {
  byte result=nib1+nib2+CARRY;
  if (result>=BASE)
    {
    result-=BASE;
    CARRY=1;
    }
  else
    {
    CARRY=0;
    }
  return(result&=0x0f);
  }


/****************************/
// digit subtractor
/****************************/
byte Chp41::Subtractor(
  byte nib1,
  byte nib2)
  {
  char result=nib1-nib2-CARRY;
  if (result<0)
    {
    result+=BASE;
    CARRY=1;
    }
  else
    {
    CARRY=0;
    }
  return(result&0x0f);
  }


/* ------------------------------------------------------------------------ */
/*                           CLASS 3 INSTRUCTIONS                           */
/* ------------------------------------------------------------------------ */

/****************************/
void Chp41::Class3()
  {
  short displacement;
  if (Tyte1&0x0200)
    displacement=((Tyte1>>3)&0x03f)-64;
  else
    displacement=(Tyte1>>3)&0x03f;
  if (displacement==0)
    displacement=1;
  if (((Tyte1>>2)&1)==CARRY)               // if carry bit == CARRY flag, then jump
    SetPC(PC_REG+displacement-1);          // carry bit==1 for GOC/JC, ==0 for GONC/JNC
  CARRY=0;
  }


/****************************/
void Chp41::error_message(short num)
  {
  return;
  switch (num)
    {
    case 1:
      {
      AfxMessageBox("HPIL Termination");
      break;
      }
    case 2:
      {
      AfxMessageBox("HPIL Error");
      break;
      }
    case 30:
      {
      AfxMessageBox("ERROR (M30): Unknown result");
      break;
      }
    case 31:
      {
      AfxMessageBox("ERROR (M31): Unknown Peripheral");
      break;
      }
    case 33:
      {
      AfxMessageBox("ERROR (M33): Card reader not implemented");
      break;
      }
    case 34:
      {
      AfxMessageBox("ERROR (M34): Printer not implemented");
      break;
      }
    case 35:
      {
      AfxMessageBox("ERROR: (M35) Instruction is not used");
      break;
      }
    default:
      {
      AfxMessageBox("ERROR: Unknown");
      break;
      }
    }
  }



/****************************/
// _inp() and _outp() do not work on WinNT
// by Zharkoi Oleg
/****************************/
void Chp41::Speaker(short Freq, int Duration)
  {
  ulong Count;
  byte off,on;
  byte spk;

  spk=_inp(0x61);      // get current status
  off=spk&0xFC;
  on=spk|0x03;         // turn on bits 0 and 1

  if (Freq!=0)
    {
    Count=(unsigned long)(PCPerf.QuadPart/Freq);  // determine the timer frequency
    if (!(spk&0x03))
      {
      _outp(0x61,on);          // turn on the speaker
      _outp(0x43,0xB6);        // set up the timer
      }
    _outp(0x42,Count&0xff);
    _outp(0x42,Count>>8);
    }
  else
    {
    _outp(0x61,off);           // turn off the speaker
    }
  }



#endif