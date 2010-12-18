#include <QPainter>
#include <QtGui>

#include "common.h"
#include "Log.h"
#include "sio.h"
#include "pcxxxx.h"
#include "dialogconsole.h"

#define SIO_GET_PIN(n)		pSIOCONNECTOR->Get_pin(n)
#define SIO_SET_PIN(n,v)	pSIOCONNECTOR->Set_pin(n,v)


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

void Csio::Set_Sii_bit(qint8 bit)	{ Sii_bit = bit;				}
qint8 Csio::Get_Sii_bit(void)		{ return (Sii_bit);				}

bool Csio::Get_CD(void)		{ return(CD);	}
bool Csio::Get_CS(void)		{ return(CS);	}
bool Csio::Get_RR(void)		{ return(RR);	}
bool Csio::Get_RS(void)		{ return(RS);	}
bool Csio::Get_ER(void)		{ return(ER);	}
bool Csio::Get_SD(void)		{ return(SD);	}
bool Csio::Get_RD(void)		{ return(RD);	}

void Csio::Set_SD(bool val) { SD = val;	}
void Csio::Set_CD(bool val) { CD = val;	}
void Csio::Set_CS(bool val) { CS = val;	}
void Csio::Set_RD(bool val) { RD = val;	}
void Csio::Set_RR(bool val) { RR = val;	}
void Csio::Set_ER(bool val) { ER = val;	}
void Csio::Set_RS(bool val) { RS = val;	}

void Csio::Set_BaudRate(int br) {
    baudrate = br;
    AddLog(LOG_SIO,tr("new baudrate = %1").arg(br))
}
int  Csio::Get_BaudRate(void) {return baudrate;}


void Csio::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu menu(this);

    BuildContextMenu(&menu);

    menu.addSeparator();

    menu.addAction(tr("Show console"),this,SLOT(ShowConsole()));
    menu.addAction(tr("Hide console"),this,SLOT(HideConsole()));

    menu.exec(event->globalPos () );
}

void Csio::ShowConsole(void) {
    dialogconsole->show();
}
void Csio::HideConsole(void) {
    dialogconsole->hide();
}

bool Csio::run(void)
{
    pSIOCONNECTOR_value = pSIOCONNECTOR->Get_values();

	// Read connector
	Set_SD( SIO_GET_PIN(SIO_SD) );
	Set_RR( SIO_GET_PIN(SIO_RR) );
	Set_RS( SIO_GET_PIN(SIO_RS) );
	Set_ER( SIO_GET_PIN(SIO_ER) );	
//	Set_PRQ( SIO_GET_PIN(SIO_PRQ) );	
	
	
	Sii_bit = 0;
	if (ER && CD && RR)	{ Sii_bit = inReadBit();	}
	Set_RD( Sii_bit );	
	
	if (RS) { Set_CS(1); Set_CD(1); }			
	outReadBit();

	SIO_SET_PIN(SIO_RD, Get_RD());
	SIO_SET_PIN(SIO_CS, Get_CS());
	SIO_SET_PIN(SIO_CD, Get_CD());
	SIO_SET_PIN(SIO_PAK, 0);

	return true;
}

void Csio::startTransfer(void)
{
	Sii_wait			= 0;
	Sii_ndx				= 0;
	Sii_bit				= 0;
	Sii_startbitsent	= false;
	Sii_stopbitsent		= true;
	Sii_TransferStarted = true;
	
	Set_CD(1);
	
}

void Csio::clearInput(void)
{
	Sii_ndx = 0;
	baInput.clear();
}

bool Csio::inReadBit(void)
{
	static BYTE	bit			= 0;
	static int	oldstate	= pTIMER->state;
	BYTE		data		= 0;
	int			deltastate	= 0;

	Sii_LfWait = 100;
    Sii_wait	= TICKS_BDS;
	
// If there are new data in baInput
	if (! Sii_TransferStarted) return 0;

	if ( Sii_ndx < baInput.size() )
	{
		deltastate = pTIMER->state - oldstate;

		if (deltastate < Sii_wait) return(bit);

		oldstate	= pTIMER->state;
//		oldstate	+= deltastate;

		data	= baInput.at(Sii_ndx); 

        // Update Input proressbar
        emit valueChanged((int)((Sii_ndx*100)/baInput.size()+.5));
        Refresh_Display = true;
		data = (data == 0x0A ? 0x0D : data);
		bit		= inReadBitFromByte(data);
		
		switch (bit)
		{
		case 3:	bit = 1;
                Sii_wait = TICKS_BDS;
				return(bit);		// START BIT
		case 0:
		case 1:	AddLog(LOG_SIO,tr("Envoie bit = %1").arg(bit));
                Sii_wait = TICKS_BDS;
				return(bit);		// DATA BIT

		case 2:	bit = 0;
                Sii_wait = TICKS_BDS;

				if (data == 0x0D)
				{
					Sii_wait=Sii_LfWait*pTIMER->pPC->getfrequency()/1000;
					AddLog(LOG_SIO,tr("LF found, wait %1 ms").arg(Sii_LfWait));
				}
				
				Sii_ndx++;										// Next Char

				return(bit);
		}
	}

	if (
		Sii_TransferStarted &&
		(Sii_ndx == baInput.size()) &&
		(Sii_ndx >0) )
	{
		// End of file
//		Sii_ndx				= 0;
//		Set_Sii_bit(0);
		Sii_startbitsent	= FALSE;
		Sii_stopbitsent		= TRUE;
		Sii_TransferStarted	= FALSE;
		Set_CD(0);
		clearInput();
		AddLog(LOG_SIO,tr("END TRANSMISSION"));
	}

	return(0);
}


//
// Take in account number of start, stop and parity bits
//
qint8 Csio::inReadBitFromByte(qint8 data)
{
	static bool Start_Bit_Sent	= FALSE;
	qint8			bit			= 0;

		if (!Start_Bit_Sent)
		{
			Start_Bit_Sent = TRUE;

			AddLog(LOG_SIO,tr("START BIT : %1").arg(data,2,16,QChar('0')));

			return(3);  // Startbit = 1
		}
		else
		if (inBitNb<8)
		{
			bit = ( ((data >> inBitNb) & 0x01) ? 0 : 1 );
			inBitNb++;
			return(bit);
		}
		else
		if (inBitNb == 8)
		{
			inBitNb = 0;
			AddLog(LOG_SIO,tr("STOP BIT"));
			Start_Bit_Sent	= FALSE;
			return(2);	// STOPBIT : To be converted to 0
		}
	return(3);
}


void Csio::ExportBit(bool data) {}

void Csio::ExportByte(qint8 data)
{
	baOutput.append( (char) data);

    // Emit signal new data
    emit newData(data);

	Refresh_Display = true;
}


void Csio::outReadBit(void)
{
static unsigned char	t=0,c=0,waitbitstart=1,waitbitstop=0;
int deltastate=0;
static long oldstate	= pTIMER->state;
//	0	START BIT
//	0-1	b0 0=set,1=not set	
//	0-1	b1 0=set,1=not set
//	0-1	b2 0=set,1=not set
//	0-1	b3 0=set,1=not set
//	0-1	b4 0=set,1=not set
//	0-1	b5 0=set,1=not set
//	0-1	b6 0=set,1=not set
//	0-1	b7 0=set,1=not set
//	1	STOP BIT

	deltastate = pTIMER->state - oldstate;
	if (deltastate < Sii_wait) return;
		
	if (!(ER && RS)) 
	{
		oldstate	= pTIMER->state;
		Sii_wait	= 0;
		return;
	}	
//	Sii_wait	= TICKS_BDS;
	oldstate	= pTIMER->state;		
//	oldstate	+= Sii_wait;		
		

	if (waitbitstop && (SD==0))
	{
		waitbitstop = 0;waitbitstart=1;
//		Bit STOP
		AddLog(LOG_SIO,tr("STOP BIT"));
		Sii_wait = 0;
	}
	else if (waitbitstart && SD) 
	{
		waitbitstart = 0;
//		Bit START
		AddLog(LOG_SIO,tr("START BIT"));
        Sii_wait	= TICKS_BDS;
		
	}
	else if (!waitbitstart)
	{
		t>>=1;
		if(SD==0) t|=0x80;
		AddLog(LOG_SIO,tr("Bit = %1").arg(SD));
		if((c=(++c)&7)==0)
		{
			AddLog(LOG_SIO,tr("Byte = %1").arg(t,2,16,QChar('0')));
			ExportByte(t);
			t=0;
			waitbitstop = 1;
		}
	}
};


/*****************************************************************************/
/* Initialize SIO															 */
/*****************************************************************************/
bool Csio::init(void)
{

	AddLog(LOG_MASTER,"SIO initializing...");

	CD = CS = ER = RD = RR = RS = SD = 0;

	Pc_DX	= 195;
	Pc_DY	= 145;

	CPObject::init();

    dialogconsole = new DialogConsole(this);
    dialogconsole->show();

    connect(this,SIGNAL(valueChanged(int)),dialogconsole->inputProgressBar,SLOT(setValue(int)));

	AddLog(LOG_MASTER,"done.\n");

    WatchPoint.add(&pSIOCONNECTOR_value,64,15,this,"Serial 15pins connector");

	return true;
}

/*****************************************************************************/
/* Exit SIO																	 */
/*****************************************************************************/
bool Csio::exit(void)
{
    if (dialogconsole) {
        dialogconsole->close();
        dialogconsole = 0;
    }
	return true;
}


/*****************************************************************************/
/* Set data to RxD port from file											 */
/*****************************************************************************/
void Csio::Set_SI(void)
{
}

/*****************************************************************************/
/* Put data to file from TxD												 */
/*****************************************************************************/
void Csio::Put_SO(void)
{
}

 
void Csio::paintEvent(QPaintEvent *event)
{
	CPObject::paintEvent(event);
	dialogconsole->refresh();
}
