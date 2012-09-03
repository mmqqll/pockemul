#include <QtGui>

#include "common.h"
#include "cx07.h"
#include "z80.h"
#include "Inter.h"
#include "init.h"
#include "ct6834.h"
#include "uart.h"
#include "Log.h"
#include "Lcdc_x07.h"
#include "Keyb.h"
#include "Connect.h"
#include "dialoganalog.h"


/*
           M�moire du XO7
  0000    ----------------
         |  XXXX  |  RAM  |
  2000    ----------------
         | Carte RAM 8Ko  |
  4000    ----------------
         | Prise ROM 8Ko  |
  6000    ----------------
         | ROM Carte 8Ko  |
  8000    ----------------
         | V-RAM 6Ko      |
  9800    ----------------
         | ~~~~~~~~~~~~~~ |
  A000    ----------------
         | ROM TV 4Ko     |
  B000    ----------------
         | ROM 20Ko BASIC |
  FFFF    ----------------

*/
#define PRT_DATA       0x00
#define PRT_ACK        0x01
#define MASK_PRT_DATA  0x20
#define MASK_PRT_ACK   0x40

#define INIT_T6834     0xf0
#define LEC_T6834      0xf1
#define LEC_T6834_ACK  0xf2

#define SIO_GET_PIN(n)		pSio->pSIOCONNECTOR->Get_pin(n)
#define SIO_SET_PIN(n,v)	pSio->pSIOCONNECTOR->Set_pin(n,v)


#define SIO_GNDP	1
#define SIO_SD 		2
#define SIO_RD		3
#define SIO_RS		4
#define SIO_CS		5

#define SIO_GND		7
#define SIO_CD		8
#define SIO_VC1		10
#define SIO_RR		11
#define SIO_PAK		12
#define SIO_VC2		13
#define SIO_ER		14
#define SIO_PRQ		15

Cx07::Cx07(CPObject *parent)	: CpcXXXX(parent)
{								//[constructor]
    setfrequency( (int) 3840000*3/8);
    setcfgfname(QString("x07"));

    SessionHeader	= "X07PKM";
    Initial_Session_Fname ="x07.pkm";

    BackGroundFname	= ":/x07/x07.png";
    LcdFname		= ":/x07/x07lcd.png";
    SymbFname		= "";

    memsize		= 0x10000;
    InitMemValue	= 0xff;

    SlotList.clear();
    SlotList.append(CSlot(8 , 0x0000 ,	""                  , ""	, RAM , "RAM"));
    SlotList.append(CSlot(8 , 0x2000 ,	""					, ""	, RAM , "Carte RAM 8kb"));
    SlotList.append(CSlot(8 , 0x4000 ,	""					, ""	, ROM , "Prise ROM 8kb"));
    SlotList.append(CSlot(8 , 0x6000 ,	""					, ""	, ROM , "ROM Carte 8kb"));
    SlotList.append(CSlot(6 , 0x8000 ,	""					, ""	, RAM , "Carte RAM 8kb"));
    SlotList.append(CSlot(4 , 0xA000 ,	""                  , ""	, ROM , "ROM TV"));
    SlotList.append(CSlot(20, 0xB000 ,	":/x07/rom_xo7.bin" , "x07/rom_xo7.bin" 	, ROM , "BASIC ROM"));

    KeyMap		= KeyMap1250;
    KeyMapLenght= KeyMap1250Lenght;

    PowerSwitch	= 0;
    Pc_Offset_X = Pc_Offset_Y = 0;

    setDXmm(200);//Pc_DX_mm = 135;
    setDYmm(130);//Pc_DY_mm = 70;
    setDZmm(30);//Pc_DZ_mm = 10;

    setDX(715);//Pc_DX		= 483;//409;
    setDY(465);//Pc_DY		= 252;//213;

    Lcd_X		= 67;
    Lcd_Y		= 63;
    Lcd_DX		= 120;//168;//144 ;
    Lcd_DY		= 32;
    Lcd_ratio_X	= 2.4;// * 1.18;
    Lcd_ratio_Y	= 2.8;// * 1.18;

    Lcd_Symb_X	= 55;//(int) (45 * 1.18);
    Lcd_Symb_Y	= 41;//(int) (35 * 1.18);
    Lcd_Symb_DX	= 339;
    Lcd_Symb_DY	= 5;
    Lcd_Symb_ratio_X	= 1;//1.18;

    PowerSwitch = 0;

    pLCDC		= new Clcdc_x07(this);
    pCPU		= new CZ80(this);
    pT6834      = new CT6834(this);
    pUART        = new Cuart(this);
    pTIMER		= new Ctimer(this);
    //pCONNECTOR	= new Cconnector(this,11,0,"Connector 11 pins",false,QPoint(1,87));		publish(pCONNECTOR);


    pPARConnector = new Cconnector(this,15,0,Cconnector::Canon_15,"Parrallel Connector",false,QPoint(715,50));
    publish(pPARConnector);
    pSERConnector = new Cconnector(this,9,1,Cconnector::Canon_9,"Serial Connector",false,QPoint(0,50));
    publish(pSERConnector);
    pKEYB		= new Ckeyb(this,"x07.map");


    Cpt = 0;
    Nb=0;
    Lec_K7=0;
    IT_T6834 = 0;

}

Cx07::~Cx07() {
    delete pPARConnector;
    delete pSERConnector;
    delete pT6834;
    delete pUART;
}

bool Cx07::init(void)				// initialize
{

#ifndef QT_NO_DEBUG
    pCPU->logsw = true;
#endif
    CpcXXXX::init();

    memset((void*)&Port_FX,0,sizeof (Port_FX));
    memset((void *)mem ,0,0x6000);

    ((CZ80 *) pCPU)->z80.r16.pc = 0xC3C3;

    pT6834->init();
    pUART->init();
    pUART->pTIMER = pTIMER;



    WatchPoint.remove(this);
    WatchPoint.add(&pPARConnector_value,64,15,this,"Parallel Connector");
    WatchPoint.add(&pSERConnector_value,64,9,this,"Serial Connector");

    return true;
}



bool Cx07::run() {

    if (off && pT6834->General_Info.Break == 1)
    {
            pT6834->General_Info.Break = 0;
            TurnON();
            ((CZ80 *) pCPU)->z80.r16.pc = 0xC3C3;

    }

    if (pKEYB->LastKey){
        Qt::KeyboardModifiers keyModifiers = (pT6834->shift?Qt::ShiftModifier:Qt::NoModifier)|(pT6834->graph?Qt::AltModifier:Qt::NoModifier)|(pT6834->ctrl?Qt::ControlModifier:Qt::NoModifier);

        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress,pKEYB->LastKey,keyModifiers);

        pT6834->keyPress(event);
        pKEYB->LastKey = 0;
    }

    CpcXXXX::run();

    //TODO Copy data to UART: Baudrate

    pUART->Set_CS(true);        // for test purpose

    pUART->run();

    pSERConnector->Set_pin(4,pUART->Get_SD());      // TxD
    pSERConnector->Set_pin(5,pUART->Get_RD());      // RxD
    pSERConnector->Set_pin(6,pUART->Get_CS());      // CTS
    pSERConnector->Set_pin(7,pUART->Get_RS());      // RTS

    if ( ((CZ80*)pCPU)->z80.r.iff &0x01)
    {
        if (!pT6834->Clavier.isEmpty()) {
            Port_FX.R.F1 = pT6834->Clavier.at(0);
            pT6834->Clavier.remove(0,1);
            Port_FX.R.F0  = 0x00;
            Port_FX.R.F2 |= 0x01;
            IT_T6834      = 0;
            AddLog(LOG_TEMP,tr("Key:%1").arg(Port_FX.R.F1,2,16,QChar('0')));

            ((CZ80*)pCPU)->z80nsc800intr(&((CZ80*)pCPU)->z80,IT_RST_A);

        }


        if (pT6834->General_Info.Break == 1)
        {

            Port_FX.R.F0  = 0x80;
            Port_FX.R.F1  = 0x05;
            Port_FX.R.F2 |= 0x01;
            IT_T6834      = 0;
            pT6834->General_Info.Break=0;
            AddLog(LOG_TEMP,"Break");
            ((CZ80*)pCPU)->z80nsc800intr(&((CZ80*)pCPU)->z80,IT_RST_A);
            return (IT_RST_A);
        }
        if ( IT_T6834 )
        {
            switch (IT_T6834)
            {
            case 1:

                AddLog(LOG_TEMP,tr("It1_6834 0X020F = %1   0X026D: %2\n").arg(Get_8(0x020f),2,16,QChar('0')).arg(Get_8(0x026d),2,16,QChar('0')));

                ReceiveFromT6834 (LEC_T6834, &Port_FX);
                IT_T6834 = 0;

                ((CZ80*)pCPU)->z80nsc800intr(&((CZ80*)pCPU)->z80,IT_RST_A);
                return (IT_RST_A);
                break;
            case 2:

                //               fprintf (stderr,"It2_6834 0X020F = %d   0X026D: %d\n",RAM[0x020f],RAM[0x026d]);
                AddLog (LOG_TEMP,tr("It2_6834 Nb= %1\n").arg(Nb));

                Port_FX.R.F0  = 0x80;
                  /* Port_FX.R.F1  = 0x04;  Pour 4: demande de l'heure Cmd=1    */
                  /* Port_FX.R.F1  = 0x05;  Pour 5: UDKoff SPoff, Lect @C00E    */
                  /* Port_FX.R.F1  = 0x06;  Pour 6: UDKon Affiche 'Low battery' */
                  /* Port_FX.R.F1  = 0x07;  Pour 7: demande de l'heure Cmd=1    */
                  /* Port_FX.R.F1  = 0x08;  Pour 8: demande de l'heure Cmd=1    */
                  Port_FX.R.F1  = Nb;
                  Port_FX.R.F2 |= 0x01;
                  Nb++;
                  if ((Nb==4) || (Nb==7)) Nb++;
                  if (Nb>255) Nb=0;
                  IT_T6834 = 0;

                  ((CZ80*)pCPU)->z80nsc800intr(&((CZ80*)pCPU)->z80,IT_RST_A);
                  return (IT_RST_A);
                  break;
              case 3:
//                  if (Lec_K7 >= 100)
                  {
                      AddLog (LOG_TEMP,"It3_6834\n");
                      IT_T6834 = 0;
                      Lec_K7 = 0;

                      ((CZ80*)pCPU)->z80nsc800intr(&((CZ80*)pCPU)->z80,IT_RST_B);
                      return (IT_RST_B);
                  }
//                  else
//                  {
//                      Lec_K7 ++;
//                  }
                  break;
              }
          }
          //return (INT_NONE);
      }

    pPARConnector_value = pPARConnector->Get_values();
    pSERConnector_value = pSERConnector->Get_values();
}

bool Cx07::Chk_Adr(DWORD *d, DWORD data)
{
    if ( (*d>=0x0000) && (*d<=0x3FFF) )	return(true);		// RAM area()
    if ( (*d>=0x8000) && (*d<=0x97FF) )	return(true);		// RAM area()

    return false;
}

bool Cx07::Chk_Adr_R(DWORD *d, DWORD data)
{
    return true;
}

UINT8 Cx07::in(UINT8 Port)
{
    UINT8 Value=0;

     switch (Port)
      {
       case 0xF0 : /* Controle des interruptions */
                   Value = Port_FX.R.F0;
                   break;
       case 0xF1 : /* XBRR : Stockage des Informations venant du CCU */
                   Value = Port_FX.R.F1;
    /*               Port_FX.R.F2 &= 0xFE;*/
                   break;
       case 0xF2 : /* Flags sur l'etat du Canon XO7 */
    /*               if (Port_FX.W.F5 & 0x08) Port_FX.R.F2 |=0x01;
                   else                     Port_FX.R.F2 &=0xFE;*/
                   if (Port_FX.W.F5 & 0x04) Port_FX.R.F2 |=0x02;
                   else                     Port_FX.R.F2 &=0xFD;
//                   if (Port_FX.W.F5 & 0x20) Port_FX.R.F2 |=0x80;
//                   else                     Port_FX.R.F2 &=0x7F;
                   Value = Port_FX.R.F2 | 0x82;
                   break;
       case 0xF3 : /* ~~~Vide~~~ */
                   Value = Port_FX.R.F3;
                   break;
       case 0xF4 : /* Modes */
                   Value = Port_FX.R.F4;
                   break;
       case 0xF5 : /* ~~~Vide~~~ */
                   Value = Port_FX.R.F5;
                   break;
       case 0xF6 : /* Status de l'UART */
                   if (Mode_K7) Port_FX.R.F6 |= 0x05;
                   if ( (Mode_SERIE) && (Port_FX.W.F6 & 0x01)) {
//                       if (pSio->CS)
                       {
                           Port_FX.R.F6 |=0x05;
                       }
                   }
                   Value = Port_FX.R.F6;
                   break;
       case 0xF7 : /* Donn�es recu par l'UART */
                   Value = Port_FX.R.F7;
                   break;
      }

     AddLog(LOG_SIO,tr("(%1) In %2,%3").arg(((CZ80*)pCPU)->z80.r16.pc,4,16,QChar('0')).arg(Port,2,16,QChar('0')).arg(Value,2,16,QChar('0')));

     pCPU->imem[Port] = Value;
     return (Value);
}

UINT8 Cx07::out(UINT8 Port, UINT8 Value)
{

 if ((Port!=0xf0) && (Value!=0x44)) {
     AddLog(LOG_SIO,tr("(%1) Out %2,%3").arg(((CZ80*)pCPU)->z80.r16.pc,4,16,QChar('0')).arg(Port,2,16,QChar('0')).arg(Value,2,16,QChar('0')));
}

 switch (Port)
  {
   case 0xBB : /* Validation des interuptions du NSC800 */
               Int_nsc800_BB = Value;
               break;

   case 0xF0 : /* Controle des interruptions */
               Port_FX.W.F0 = Value;
               break;
   case 0xF1 : /* XBTR : Stockage des Informations pour CCU */
               Port_FX.W.F1 = Value;
               break;
   case 0xF2 : /* Controle de BAUDS (poids faible) */
               Port_FX.W.F2 = Value;
               break;
   case 0xF3 : /* Controle de BAUDS (poids fort) */
               Port_FX.W.F3 = Value;
               break;
   case 0xF4 : /* Modes */
               Port_FX.W.F4 = Value;
               Port_FX.R.F4 = Value;

               if (Value & 0x80) {
                   AddLog(LOG_SIO,tr("Reglage Bauds : %1 - %2  = %3").arg(Port_FX.W.F2,2,16,QChar('0')).arg(Port_FX.W.F3,2,16,QChar('0')).arg(((Port_FX.W.F3&0x0F)<<8) | (Port_FX.W.F2),2,16,QChar('0')));
                   int divisor = ((Port_FX.W.F3&0x0F)<<8) | (Port_FX.W.F2) + 1;
                   if (divisor) pUART->Set_BaudRate(24000/divisor);
               }

               if (Value & 0x40) {
                   AddLog(LOG_SIO,tr("Bauds Counter START"));
               }
               else {
                   AddLog(LOG_SIO,tr("Bauds Counter STOP"));
               }
               Mode_K7 = ((Value & 0x0C) == 0x08) ? 1 : 0;
               Mode_BUZ= ((Value & 0x0E) == 0x0E) ? 1 : 0;
               Mode_SERIE=((Value & 0x0C)== 0x04) ? 1 : 0;

               if (Mode_SERIE) {
                   AddLog(LOG_CANON,"MODE SERIE");
               }

               if (Port_FX.W.F5 & 0x04) {
                   if (Mode_K7) {
//                       Receive_from_K7 (&Port_FX);
                   }
                   if (Mode_SERIE ) {//&& !(Port_FX.W.F6 & 0x20)) {
                       ReceiveFromSerial(&Port_FX);
                   }
               }

               break;
   case 0xF5 : /* Interruptions (RESET) */
               Port_FX.W.F5 = Value;
               /* Reception d'un octet venant du T6834 *
                *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x01)
                ReceiveFromT6834 (LEC_T6834_ACK,&Port_FX);

               /* Envoie d'un octet a destination du T6834 *
                *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x02)
                SendToT6834 (&Port_FX);

               /* Reception d'un octet EN PROVENANCE du lecteur de K7 *
                *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x04) {
                   AddLog(LOG_CANON,tr("Interruption F5 & 0x04 = %1").arg(Value,2,16,QChar('0')));
                   if (Mode_K7) {
//                   Receive_from_K7 (&Port_FX);
                   }
                   if (Mode_SERIE ){//&& !(Port_FX.W.F6 & 0x20)) {
                       ReceiveFromSerial(&Port_FX);
                   }
               }

               /* Envoie d'un octet a destination du lecteur de K7 *
                *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x08) {
                   AddLog(LOG_CANON,tr("Interruption F5 & 0x08 = %1").arg(Value,2,16,QChar('0')));
                   if (Mode_K7) {
//                   Send_to_K7 (&Port_FX);
                   }
                   if (Mode_SERIE ){//&& (Port_FX.W.F6 & 0x20)) {
                       SendToSerial(&Port_FX);
                   }
               }

               /* R?????????  SERIAL ??????? *
               *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x10) {
                   AddLog(LOG_CANON,tr("Interruption F5 & 0x10 = %1").arg(Value,2,16,QChar('0')));
               }

               /* Envoie d'un bit sur le port imprimante  *
                *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x20) {
                   AddLog(LOG_CANON,"PRINTER");
                   Print (PRT_DATA,&Port_FX);
               }

               /* R?????????  SERIAL ??????? *
               *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x40) {
                   AddLog(LOG_CANON,tr("Interruption F5 & 0x40 = %1").arg(Value,2,16,QChar('0')));
               }
               /* R?????????  ALARM ??????? *
               *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
               if (Value & 0x80) {
                   AddLog(LOG_CANON,tr("Interruption F5 & 0x80 = %1").arg(Value,2,16,QChar('0')));
               }
               break;
   case 0xF6 : /* Configuration de L'UART */
               Port_FX.W.F6 = Value;
//               Port_FX.R.F6 = Value;
               break;
   case 0xF7 : /* Donn�es �mises par l'UART */
               Port_FX.W.F7 = Value;
               break;
  }

 return 0;
}

void Cx07::Print (UINT8 Cmd, PorT_FX *Port)
{
 static UINT8 Cpt_Bit=0;
 static UINT8 Data=0;

 switch (Cmd)
  {
   case PRT_DATA: if (!(Port->R.F4 & MASK_PRT_DATA)) Data |= 0x80;
//                    AddLog(LOG_PRINTER,tr("bit:%1   data=%2").arg(Data&1).arg(Data,2,16,QChar('0')));

                  if (Cpt_Bit == 7)
                   {
                       AddLog(LOG_PRINTER,tr("Prt_data=0x%1 : %2").arg(Data,2,16,QChar('0')).arg(QChar(Data)));
                       qint64 val = pPARConnector->Get_values();
                       val &= 0x7E01;
                       val |= Data << 1;
                       val |= 0x01;   // fire the strobe;
                       pPARConnector->Set_values(val);
                       if (mainwindow->dialoganalogic) mainwindow->dialoganalogic->setMarker(1);
                    Cpt_Bit = 0;
                    Data = 0;

                   }
                  else {
                      Data >>= 1;
                      Cpt_Bit ++;
                      if (Cpt_Bit == 5) {
                          pPARConnector->Set_pin(1,false);
                          if (mainwindow->dialoganalogic) mainwindow->dialoganalogic->setMarker(2);
                      }
                  }
                  break;
   case PRT_ACK:
                  break;
  }
}

void Cx07::ReceiveFromT6834(UINT8 Cmd, PorT_FX *Port)
{

 switch (Cmd)
  {
   case INIT_T6834    : memset ((void*)Rsp,0,sizeof(Rsp));
                        pt=0;
                        lng_rsp = Get_8(0x020F); // RAM [0x026D];
                        Ordre = pT6834->Send_Cmd_T6834[0];

                        AddLog(LOG_TEMP,tr("INIT_T6834: Ordre=%1 Lng_Rsp=%2 (%3)").arg(Ordre,2,16,QChar('0')).arg(lng_rsp,2,16,QChar('0')).arg(pT6834->Cmd_T6834[Ordre].lng_rsp,2,16,QChar('0')));

                        lng_rsp = pT6834->InitReponseT6834(Ordre, Rsp, Port);
                        if (lng_rsp)
                         {
                          IT_T6834 = 1;

                          AddLog (LOG_TEMP,"Attente reponse...");

                         }
                        break;

   case LEC_T6834     : /*if (RAM[0x020F] != 0)*/
                         {
                          Port_FX.R.F0  = 0x40;
                          Port_FX.R.F1  = Rsp[pt];
                          Port_FX.R.F2 |= 0x01;

                          AddLog(LOG_TEMP,tr("LEC_T6834: Data=%1").arg(Rsp[pt],2,16,QChar('0')));

                         }
                        break;

   case LEC_T6834_ACK : pt++;
                        AddLog(LOG_TEMP,"LEC_T6834_ACK: IT_");
                        if (pt < lng_rsp)
                        {
                            IT_T6834 = 1;
                            AddLog(LOG_TEMP,"ON\n");
                        }
                        else
                            AddLog(LOG_TEMP,"OFF\n");

                        Port_FX.R.F2 &= 0xFE;

                        break;

   default : AddLog(LOG_TEMP,tr("Commande au T6834 inconnue[%1]!!!\n").arg(Get_8(0x020F),2,16,QChar('0')));
             break;
  }
}

void Cx07::SendToT6834 (PorT_FX *Port)
{

    // int i;

    if (!Cpt)
    {
        if (pT6834->Locate_OnOff &&
                ((Port->W.F1 & 0x7F) != 0x24) &&
                ((Port->W.F1) >= 0x20) &&
                ((Port->W.F1) <  0x80))
        {
            pT6834->General_Info.Curs_X ++;
            //pT6834-->General_Info.Curs_Y ++;
            //     fputc (Port->W.F1,stderr);
            fprintf (stderr,"(%02X) %c",Port->W.F1,Port->W.F1);
            pT6834->AffCar (pT6834->General_Info.Curs_X,pT6834->General_Info.Curs_Y,Port->W.F1);
        }
        else
        {
            pT6834->Locate_OnOff = 0;
            //     Loc_X = Loc_Y = 0xff;
            if (((Port->W.F1) & 0x7F) < 0x47)
            {
                pT6834->Send_Cmd_T6834 [Cpt++] = Port->W.F1 & 0x7F;
                Lng_Cmd = CT6834::Cmd_T6834[Port->W.F1 & 0x7F].lng_send;
                Lng_Rsp = CT6834::Cmd_T6834[Port->W.F1 & 0x7F].lng_rsp;
                AddLog (LOG_TEMP,tr("0x%1: Send:%2 Rsp:%3 (%4)\n").arg(Port->W.F1,2,16,QChar('0')).arg(Lng_Cmd,2,16,QChar('0')).arg(Lng_Rsp,2,16,QChar('0')).arg(Cpt,2,16,QChar('0')));
            }
            else {
                AddLog(LOG_TEMP,tr("Cmd T6834 inconnu (%1)[%2]").arg(Port->W.F1,2,16,QChar('0')).arg(Get_8(0x020F),2,16,QChar('0')));
            }
        }
    }
 else
  {
   pT6834->Send_Cmd_T6834 [Cpt++] = Port->W.F1;
   if ((Lng_Cmd & 0x80) && (Cpt>(Lng_Cmd&0x7F)) && (!Port->W.F1))
    {
     fprintf (stderr,"0x%02X: Send:%02X Rsp:%d (%d)[%d]\n",pT6834->Send_Cmd_T6834 [0],Lng_Cmd,Lng_Rsp,Cpt,Port->W.F1);
     Lng_Cmd = Cpt;
    }
   if (Cpt == 2)
    switch (pT6834->Send_Cmd_T6834 [0])
     {
      case 0x0C : if (pT6834->Send_Cmd_T6834 [1] == 0xB0)
                   {
                    pT6834->ClrScr ();
                    pT6834->Send_Cmd_T6834 [0] = pT6834->Send_Cmd_T6834 [1]&0x7f;
                    Lng_Cmd = CT6834::Cmd_T6834[pT6834->Send_Cmd_T6834 [0] & 0x7F].lng_send;
                    Lng_Rsp = CT6834::Cmd_T6834[pT6834->Send_Cmd_T6834 [0] & 0x7F].lng_rsp;
                    Cpt --;
                   }
                  break;
      case 0x07 : if (pT6834->Send_Cmd_T6834 [1] > 4)
                   {
                    fputc (pT6834->Send_Cmd_T6834[0],stderr);
                    pT6834->Send_Cmd_T6834 [0] = pT6834->Send_Cmd_T6834 [1]&0x7f;
                    Lng_Cmd = CT6834::Cmd_T6834[pT6834->Send_Cmd_T6834 [0] & 0x7F].lng_send;
                    Lng_Rsp = CT6834::Cmd_T6834[pT6834->Send_Cmd_T6834 [0] & 0x7F].lng_rsp;
                    Cpt --;
                   }
                  break;
     }

  }

 if (Cpt && (Cpt == Lng_Cmd))
  {

      QString s = "Cmd - "+QString(CT6834::Cmd_T6834[pT6834->Send_Cmd_T6834 [0] & 0x7F].Str_Cmd);
   for (int i=0; i < Cpt; i++)
       s+=QString(" %1 ").arg(pT6834->Send_Cmd_T6834[i],2,16,QChar('0'));

   AddLog(LOG_CANON,s);
#if AFF_STR_SND_T6834
   fprintf (stderr,"(%s) \n",Cmd_T6834[pT6834->Send_Cmd_T6834[0]].Str_Cmd);
#endif
   ReceiveFromT6834 (INIT_T6834,Port);
    Cpt = 0;
  }
}


void Cx07::keyReleaseEvent(QKeyEvent *event)
{
    pT6834->keyRelease(event);
}

void Cx07::keyPressEvent(QKeyEvent *event)
{

    pT6834->keyPress(event);

}


void Cx07::TurnOFF(void) {
    mainwindow->saveAll = YES;
    CpcXXXX::TurnOFF();
    mainwindow->saveAll = ASK;
    AddLog(LOG_TEMP,"TURN OFF");
    pT6834->General_Info.LcdOn = false;
}

void Cx07::TurnON(void){
    CpcXXXX::TurnON();
    pCPU->Reset();
    ((CZ80 *) pCPU)->z80.r16.pc = 0xC3C3;
    pT6834->General_Info.LcdOn = true;

    AddLog(LOG_CANON,"TURN ON");

}


void Cx07::Reset()
{

    CpcXXXX::Reset();
    pT6834->Reset();
    ((CZ80 *) pCPU)->z80.r16.pc = 0xC3C3;
}

bool Cx07::LoadConfig(QXmlStreamReader *xmlIn)
{
    pT6834->Load_Internal(xmlIn);
    return true;
}

bool Cx07::SaveConfig(QXmlStreamWriter *xmlOut)
{
    pT6834->save_internal(xmlOut);
    return true;
}

/*
void Send_to_K7 (PorT_FX *Port)
{
 if ((Port->R.F4 & 0x09) == 0x09)
  {
   fprintf (stderr,"%02X ",Port->W.F7);
  }
}

void Receive_from_K7 (PorT_FX *Port)
{
// static int Cpt=0;

// if (!Presence_k7)
//  {
//   Fichier = fopen ("fichiers.xo7/DOMINO.LST","r");
//   Fichier = fopen ("fichiers.xo7/BURGER.LST","r");
//   Fichier = fopen ("fichiers.xo7/KARATE.LST","r");
//   Fichier_k7 = fopen ("fichiers.xo7/AVENTUR.LST","r");
//   Presence_k7 = 1;
//  }

 if (Presence_k7 && ((Port->R.F4 & 0x09) == 0x09))
  {
   if (feof(Fichier_k7))
    {
     rewind (Fichier_k7);
    }
   Port->R.F7  = fgetc (Fichier_k7);
   Port->R.F6 |= 0x02;
   IT_T6834 = 3;
//   fprintf (stderr,"Receive from K7 %02X ",Port->R.F7);
//   if (Cpt>=60)
//    Cpt=0;
  }
}

*/

void Cx07::ReceiveFromSerial(PorT_FX *Port) {
    if ((Port->W.F6 & 0x04) && pUART->isInputByte()) {
        AddLog(LOG_SIO,tr("POP BYTE:%1 - %2").arg(pUART->getInputByte(),2,16,QChar('0')).arg(QChar(pUART->getInputByte())));
        Port->R.F7  = pUART->popInputByte();
        Port->R.F6 |= 0x02;
        Port->R.F6 &= ~0x04;
        IT_T6834 = 3;
    }
}

void Cx07::SendToSerial(PorT_FX *Port)
{
 if ((Port->R.F4 & 0x0C) == 0x04)
  {
    pUART->sendByte(Port->W.F7);
    pUART->startTransfer();
  }
}

void Cx07::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu menu(this);

    BuildContextMenu(&menu);

    menu.addSeparator();

    QMenu *menuUart = menu.addMenu(tr("Serial communication"));
    menuUart->addAction(tr("Show console"),pUART,SLOT(ShowConsole()));
    menuUart->addAction(tr("Hide console"),pUART,SLOT(HideConsole()));

    menu.exec(event->globalPos () );
}
