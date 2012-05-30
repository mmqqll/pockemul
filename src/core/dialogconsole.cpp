#include <QTextCursor>
#include <QCloseEvent>

#include "dialogconsole.h"
#include "common.h"
#include "pcxxxx.h"
#include "Inter.h"
#include "sio.h"

//
DialogConsole::DialogConsole( QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	setFocusPolicy(Qt::StrongFocus);

    connect(pbSend, SIGNAL(clicked()), this, SLOT(sendData())); 
    connect(baudCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeBaudrate(QString)));
    connect(pbStop,SIGNAL(clicked()),this,SLOT(stopStream()));

    connect(lEdit_CD,SIGNAL(textChanged(QString)),this,SLOT(updateMapCD(QString)));
    connect(lEdit_CS,SIGNAL(textChanged(QString)),this,SLOT(updateMapCS(QString)));
    connect(lEdit_ER,SIGNAL(textChanged(QString)),this,SLOT(updateMapER(QString)));
    connect(lEdit_RD,SIGNAL(textChanged(QString)),this,SLOT(updateMapRD(QString)));
    connect(lEdit_RR,SIGNAL(textChanged(QString)),this,SLOT(updateMapRR(QString)));
    connect(lEdit_RS,SIGNAL(textChanged(QString)),this,SLOT(updateMapRS(QString)));
    connect(lEdit_SD,SIGNAL(textChanged(QString)),this,SLOT(updateMapSD(QString)));

    pSIO = (Csio *) parent;
    currentIndex=0;
	
}
//
void DialogConsole::refresh( void)
{

	
	if (! pSIO) return;
    if (pSIO->pTIMER) ConnectLbl->setText("Connected to : " + pSIO->pTIMER->pPC->getName());
	if (pSIO->baOutput.size() > currentIndex)
	{
		int len = pSIO->baOutput.size()-currentIndex;
		textEdit_out->textCursor().setPosition(currentIndex);
		
		for (int i=0 ; i < len ; i++)
		{
			QString add;
			const unsigned char loc = pSIO->baOutput.at(currentIndex + i);
			if ((loc < 0x20) || (loc > 0x7F)) add = QString(" [0x%1] ").arg(loc,2,16,QChar('0'));
				else add = QString(loc);
			if ( cbCRLF->isChecked() && (loc == 0x0D) ) add = QString(loc); // TO DO : Manage CR
			textEdit_out->insertPlainText(add);
		}
		textEdit_out->textCursor().setPosition(currentIndex);

//	SCROOL TO BOTTOM
		QTextCursor cursor(textEdit_out->textCursor());
		cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
		textEdit_out->setTextCursor(cursor);
		
		currentIndex += len;
	}
}

void DialogConsole::changeBaudrate(QString baud) {
    bool ok;
    pSIO->Set_BaudRate(baud.toInt(&ok));
}

void DialogConsole::updateMapCD(QString val)
{
    pSIO->signalMap[Csio::S_CD] = val.toInt();
}

void DialogConsole::updateMapCS(QString val)
{
    pSIO->signalMap[Csio::S_CS] = val.toInt();
}

void DialogConsole::updateMapER(QString val)
{
    pSIO->signalMap[Csio::S_ER] = val.toInt();
}

void DialogConsole::updateMapRD(QString val)
{
    pSIO->signalMap[Csio::S_RD] = val.toInt();
}

void DialogConsole::updateMapRR(QString val)
{
    pSIO->signalMap[Csio::S_RR] = val.toInt();
}

void DialogConsole::updateMapRS(QString val)
{
    pSIO->signalMap[Csio::S_RS] = val.toInt();
}

void DialogConsole::updateMapSD(QString val)
{
    pSIO->signalMap[Csio::S_SD] = val.toInt();
}

void DialogConsole::sendData( void)
{
	
	if (! pSIO) return;
	
	pSIO->clearInput();
	pSIO->baInput.append(textEdit_in->toPlainText () );
	pSIO->baInput.append(0x1A);
    pSIO->startTransfer();
}

void DialogConsole::stopStream()
{
    if (!pSIO) return;
    pSIO->clearInput();
    pSIO->init();
}

void DialogConsole::closeEvent(QCloseEvent *event)
{
	pSIO->ToDestroy = true;
	event->accept();
}

