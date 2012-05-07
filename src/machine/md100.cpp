/* BASED ON Piotr Piatek emulator  (http://www.pisi.com.pl/piotr433/index.htm)
 *
 *
 *
 */

#include <QPainter>
#include <QTime>
#include <QSound>
#include <QFileDialog>
#include <QThread>
#include <QPixmap>
#include <QBitmap>

#include "common.h"

#include "md100.h"
#include "Connect.h"
#include "Inter.h"
#include "init.h"
#include "Log.h"


Cmd100::Cmd100(CPObject *parent):CPObject(parent)
{								//[constructor]
    setfrequency( 0);

    BackGroundFname	= ":/EXT/ext/md-100.png";
    setcfgfname("md100");


    pCONNECTOR	   = new Cconnector(this,30,0,Cconnector::Casio_30,"Connector 30 pins",true,QPoint(666,540));	publish(pCONNECTOR);
//    pCONNECTOR_Ext = new Cconnector(this,11,1,Cconnector::Sharp_11,"Connector 11 pins Ext.",false,QPoint(6,295));	publish(pCONNECTOR_Ext);

    pTIMER		= new Ctimer(this);

    setDX(1203);//Pc_DX	= 620;//480;
    setDY(779);//Pc_DY	= 488;//420;

    setDXmm(337);
    setDYmm(218);
    setDZmm(46);



}

Cmd100::~Cmd100() {
    delete pCONNECTOR;
}

bool Cmd100::UpdateFinalImage(void) {
    CPObject::UpdateFinalImage();

    return true;

}



bool Cmd100::init(void)
{
    CPObject::init();

    setfrequency( 0);

    WatchPoint.add(&pCONNECTOR_value,64,30,this,"Standard 30pins connector");
//    WatchPoint.add(&pCONNECTOR_Ext_value,64,11,this,"Ext 11pins connector");


    AddLog(LOG_PRINTER,tr("MD-100 initializing..."));

    if(pTIMER)	pTIMER->init();

    mdFileNotOpened = 0x80;
    mdNoRoom = 0x40;
    mdInvalidCommand = 0x20;
    mdFileFound = 0x10;
    mdRenameFailed = 0x08;
    mdNoData = 0x04;
    mdWriteProtected = 0x02;
    mdEndOfFile = 0x01;
    mdOK = 0x00;


    return true;
}

void Cmd100::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu menu(this);

    BuildContextMenu(&menu);

    menu.addSeparator();

    menu.addAction(tr("Define Directory"),this,SLOT(definePath()));

    menu.exec(event->globalPos () );
}

void Cmd100::definePath(void){
    MSG_ERROR("test")
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"));
    if ( path.isNull() == false )
    {
        directory.setPath(path);
    }
}

/*****************************************************/
/* Exit PRINTER										 */
/*****************************************************/
bool Cmd100::exit(void)
{
    AddLog(LOG_PRINTER,"MD-100 Closing...");
    AddLog(LOG_PRINTER,"done.");
    CPObject::exit();
    return true;
}


bool Cmd100::Get_Connector(void) {


    return true;
}

bool Cmd100::Set_Connector(void) {



    return true;
}


bool Cmd100::run(void)
{

    Get_Connector();

    pCONNECTOR_value = pCONNECTOR->Get_values();



    Set_Connector();
    pCONNECTOR_value = pCONNECTOR->Get_values();

    return true;
}

BYTE Cmd100::FddTransfer (BYTE DataIn) {
  return (this->*cmdtab[index]) (DataIn);
}


void Cmd100::FddOpen(void) {
  index = 0;
  cmdcode = 0;
  opstatus = 0;
  count = 1;
  bufindex = 0;
  deindex = 0;
  isdisk = fdd.DosInit();
}


void Cmd100::FddClose(void) {
    fdd.DosClose();
}

BYTE Cmd100::CnvStatus( CcasioDOS::TDosStatusCode x) {         //MD-100 operation status};
    switch(x) {
    case CcasioDOS::dsNoError:      return mdOK;
    case CcasioDOS::dsRenameFailed:	return mdRenameFailed + mdFileFound;
    case CcasioDOS::dsFileNotFound:	return mdInvalidCommand;
    case CcasioDOS::dsFileNotOpened:return mdFileNotOpened;
    case CcasioDOS::dsHandleInUse:  return mdInvalidCommand;
    case CcasioDOS::dsNoRoom:       return mdNoRoom;
    case CcasioDOS::dsHandleInvalid:return mdInvalidCommand; //should never happen}
    case CcasioDOS::dsNoData:       return mdNoData;
    case CcasioDOS::dsIoError:      return mdWriteProtected;
    default:                        return mdInvalidCommand; break; //should never happen}
  }
}

BYTE Cmd100::SwitchCmd(BYTE x) {
      cmdcode = x;
      opstatus = mdNoData;			// disk not inserted }
      index = 0;                    // 'cmdtab' entry point }
      if (isdisk) {
        opstatus = mdOK;
        switch (x) {
        case 0x00:
        case 0x01:
        case 0x02: index = 1; break;    // read directory entry
        case 0x10:
        case 0x11: index = 43; break;   // write to file
        case 0x20:
        case 0x21: index = 14; break;   // read from file
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34: index = 7; break;    // open file
        case 0x40: index = 5; break;    // close file }
        case 0x50: index = 25; break;   // delete file }
        case 0x60: index = 32; break;	// rename file }
        case 0x70: index = 39; break;	// write sector }
        case 0x80: index = 21; break;	// read sector }
        case 0x90: if (! fdd.FormatDisk()) opstatus = mdNoData; break;
        case 0xC0: index = 50; break;	// get file size }
        default:   opstatus = mdInvalidCommand;	// unknown command }
        }
      }
      return opstatus;
}

BYTE Cmd100::ExecDir(BYTE x) {
    int i, step;
    CcasioDOS::TStorageProperty y;
    index++;
    bufindex = 0;
    switch (cmdcode) {
    case 0x00: i = -1;
        step = 1;
        break;
    case 0x01: i = deindex;
        step = 1;
        break;
    default:	//cmdcode=$02}
        i = deindex;
        step = -1;
        break;
    }
    do {
        i += step;
        y = fdd.ReadDirEntry((TDirEntry*)&buffer[1], i);
    }
    while  (y == CcasioDOS::spFree);

    if (y == CcasioDOS::spOccupied) {
        deindex = i;
        buffer[0] = mdFileFound;
        count = SIZE_DIR_ENTRY + 1;
    }
    else {
        buffer[0] = mdEndOfFile;
        count = 1;
    }
    return (count & 0xff);

}


BYTE Cmd100::ReturnCountHi(BYTE x) {
    Q_UNUSED(x);
    index++;
    return (count>>8)&0xff;
}

BYTE Cmd100::ReturnCountLo(BYTE x) {
    Q_UNUSED(x);
    index++;
    bufindex = 0;
    return (count&0xff);
}

BYTE Cmd100::ReturnBlock(BYTE x) {
    Q_UNUSED(x);
    BYTE ret=buffer[bufindex];
    if (bufindex < (BUFSIZE - 1)) bufindex++;
    count--;
    if (count <= 0) index++;
    return ret;
}

// it is allowed to close unopened files,
// actually the command always returns with opstatus = mdOK }
BYTE Cmd100::ExecCloseFile(BYTE x) {
    index++;
    fdd.CloseDiskFile(x);
    if (fdd.DosStatus == CcasioDOS::dsFileNotOpened)
        opstatus = mdOK;
    else
        opstatus = CnvStatus(fdd.DosStatus);
    return opstatus;

}

BYTE Cmd100::AcceptCountLo(BYTE x) {
    index++;
    count = x;
    bufindex = 0;
    return opstatus;
}

BYTE Cmd100::AcceptCountHi(BYTE x) {
    index++;
    count += (x << 8);
    if (count == 0) {
        opstatus = mdInvalidCommand;
        index = 0;
    }
    return opstatus;
}

// accept 'count' bytes and store them in the 'buffer' }
BYTE Cmd100::AcceptBlock(BYTE x) {
    if (bufindex < BUFSIZE) {
        buffer[bufindex] = x;
        bufindex++;
    }
    count--;
    if (count <= 0) index++;
    return opstatus;
}

BYTE Cmd100::ExecOpenFile(BYTE x) {		// + return the number of data bytes, LSB }

    qint32 file_handle, position;

    index++;
    file_handle = buffer[1] & 0x0F;
    fdd.PutDiskFileTag(file_handle, cmdcode);
    int i = -1;
    if (cmdcode != 0x30) i = fdd.OpenDiskFile( file_handle,(char*)&buffer[3]);
    if ((i < 0) && (cmdcode < 0x32)) i = fdd.CreateDiskFile ( file_handle,(char*)&buffer[3],buffer[2]);
    count = 1;
    if (i >= 0) {
        if (cmdcode != 0x30) {
            if (fdd.ReadDirEntry((TDirEntry*)&buffer[1], i) == CcasioDOS::spOccupied) {

                count = 17;
                if (cmdcode == 0x34) {
                    position = fdd.SizeOfDiskFile (file_handle);
                    if (position > 0) {
                        fdd.SeekAbsDiskFile (file_handle, position - 1);
                        count += fdd.ReadDiskFile (file_handle, (char*)&buffer[17]);
                        // trailing zeros and Ctrl-Z aren't transferred
                        while ((count > 17) && (buffer[count] == 0)) count--;
                        if ((count > 17) && (buffer[count] == 0x1A)) count--;
                    }
                };
            };
        };
    };
    opstatus = CnvStatus (fdd.DosStatus);
    if (count > 1) opstatus = opstatus | mdFileFound;
    buffer[0] = opstatus;
    bufindex = 0;
    return count & 0xff;
}


BYTE Cmd100::ExecReadFile(BYTE x) {		// + return the number of data bytes, LSB }
    qint32 file_handle, position;
    index++;
    file_handle = buffer[1] & 0x0F;
    if (cmdcode == 0x21) {
        position = buffer[2] + (buffer[3] << 8);
        if (position == 0) position = 1;
        fdd.SeekAbsDiskFile(file_handle, position - 1);
    }
    count = fdd.ReadDiskFile (file_handle,(char*)&buffer[1]); // pointer to the data buffer
    fdd.SeekRelDiskFile (file_handle, 1);
    if (fdd.DosStatus == CcasioDOS::dsNoError) {
        if (fdd.IsEndOfDiskFile (file_handle)) {
            // for the last record skip the trailing zeros and the trailing Ctrl-Z
            while ((count > 0) && (buffer[count] == 0)) count--;
            if ((count > 0) && (buffer[count] == 0x1A)) count--;
            opstatus = mdEndOfFile;
        }
    }
    else opstatus = CnvStatus(fdd.DosStatus);
    count++;
    buffer[0] = opstatus;
    bufindex = 0;
    return  (count & 0x0f);
}


BYTE Cmd100::ExecReadSector(BYTE x) {	// + get the sector number
    // the track number specified in 'count' }

    index++;
    count = fdd.DosSecRead(count * SEC_COUNT + x - SEC_BASE,(char*)&buffer[0]);
    if (count == 0) {
        opstatus = mdInvalidCommand;
        index = 0;
    }
    bufindex = 0;
    return opstatus;
}

BYTE Cmd100::ExecKillFile(BYTE x) {		// + return the number of data bytes, LSB }
    index++;
    fdd.DeleteDiskFile ((char*)&buffer[2]);
    if (fdd.DosStatus == CcasioDOS::dsNoError) opstatus = mdFileFound;
    else if (fdd.DosStatus == CcasioDOS::dsFileNotFound) opstatus = mdOK;
    else opstatus = CnvStatus (fdd.DosStatus);
    count = 1;
    buffer[0] = opstatus;
    bufindex = 0;
    return (count & 0xff);
}

BYTE Cmd100::ExecRenameFile(BYTE x) {	// + return the number of data bytes, LSB }
    index++;
    fdd.RenameDiskFile((char*)&buffer[2] , (char*)&buffer[19] );
    if (fdd.DosStatus == CcasioDOS::dsNoError) opstatus = mdFileFound;
    else if (fdd.DosStatus == CcasioDOS::dsFileNotFound) opstatus = mdOK;
    else opstatus = CnvStatus (fdd.DosStatus);
    count = 1;
    buffer[0] = opstatus;
    bufindex = 0;
    return (count & 0xff);
}

BYTE Cmd100::ExecWriteSector(BYTE x) {
    if (bufindex < BUFSIZE) {
        buffer[bufindex] = x;
        bufindex++;
    }
    count--;
    if (count <= 0) {
        index++;
        if (fdd.DosSecWrite (buffer[1] * SEC_COUNT + buffer[2] - SEC_BASE, (char*)&buffer[3]) == 0) {
            opstatus = mdWriteProtected;
        }
        bufindex = 0;
        count = 0;
    }
    return opstatus;

}

BYTE Cmd100::ExecWriteFile(BYTE x) {
    qint32 file_handle, position, i;
    if (bufindex < BUFSIZE) {
        buffer[bufindex] = x;
        bufindex++;
    }
    count--;

    if (count <= 0) {
        // last record
        index++;
        if (cmdcode == 0) {
            file_handle = buffer[1] & 0x0F;
            if (fdd.IsEndOfDiskFile (file_handle) && (bufindex < BUFSIZE - 1)) {
                buffer[bufindex] = 0x1A;
                bufindex++;
            }
            memset((char*)&buffer[bufindex],0x00,BUFSIZE - bufindex);
            i = 2;
            while (i < bufindex) {
                if (fdd.WriteDiskFile (file_handle, (char*)&buffer[i]) != SIZE_RECORD) break;
                i+= SIZE_RECORD;
                if (i <= bufindex) fdd.SeekRelDiskFile (file_handle, 1);
            }
            if (opstatus == mdOK) opstatus = CnvStatus (fdd.DosStatus);
        }
        buffer[0] = opstatus;
        count = 1;
    }

    else {
        // not the last record
        if ((cmdcode == 0) && (bufindex == SIZE_RECORD + 2)) {
            bufindex = 2;
            file_handle = buffer[1] & 0x0F;
            if (fdd.WriteDiskFile (file_handle,(char*)&buffer[2]) != SIZE_RECORD) {
                if (opstatus == mdOK) opstatus = CnvStatus (fdd.DosStatus);
            }
            fdd.SeekRelDiskFile (file_handle, 1);
        }
        else if ((cmdcode == 0x10) && (bufindex == 2)) {
            cmdcode = 0;
        }
        else if ((cmdcode == 0x11) && (bufindex == 4)) {
            cmdcode = 0;
            bufindex = 2;
            file_handle = buffer[1] & 0x0F;
            position = buffer[2] + (buffer[3] << 8);
            if (position == 0) position = 1;
            fdd.SeekAbsDiskFile (file_handle, position - 1);
        }
    }

    return mdOK;
}

// expects file handle in buffer[0], returns file size in 'count' }
BYTE Cmd100::ExecGetSize(BYTE x) {
    index++;
    count = fdd.SizeOfDiskFile (buffer[0]);
    if (count == 0) opstatus = mdFileNotOpened;
    else if ( (fdd.GetDiskFileTag(buffer[0]) & 0x01) != 0) count--;
    return opstatus;
}

const Cmd100::funcPtr Cmd100::cmdtab[55] = {
    // index 0: entry point }
    &Cmd100::SwitchCmd,
    // index 1: read directory entry }
    &Cmd100::ExecDir,		// + return the number of data bytes, LSB }
    &Cmd100::ReturnCountHi,
    &Cmd100::ReturnBlock,
    &Cmd100::SwitchCmd,
    // index 5: close file }
    &Cmd100::ExecCloseFile,
    &Cmd100::SwitchCmd,
    // index 7: open file }
    &Cmd100::AcceptCountLo,
    &Cmd100::AcceptCountHi,
    &Cmd100::AcceptBlock,
    &Cmd100::ExecOpenFile,		// + return the number of data bytes, LSB }
    &Cmd100::ReturnCountHi,
    &Cmd100::ReturnBlock,
    &Cmd100::SwitchCmd,
    // index 14: read from file }
    &Cmd100::AcceptCountLo,
    &Cmd100::AcceptCountHi,
    &Cmd100::AcceptBlock,
    &Cmd100::ExecReadFile,		// + return the number of data bytes, LSB }
    &Cmd100::ReturnCountHi,
    &Cmd100::ReturnBlock,
    &Cmd100::SwitchCmd,
    // index 21: read sector }
    &Cmd100::AcceptCountLo,		// get the track number }
    &Cmd100::ExecReadSector,	// + get the sector number }
    &Cmd100::ReturnBlock,
    &Cmd100::SwitchCmd,
    // index 25: delete file }
    &Cmd100::AcceptCountLo,
    &Cmd100::AcceptCountHi,
    &Cmd100::AcceptBlock,
    &Cmd100::ExecKillFile,		// + return the number of data bytes, LSB }
    &Cmd100::ReturnCountHi,
    &Cmd100::ReturnBlock,
    &Cmd100::SwitchCmd,
    // index 32: rename file }
    &Cmd100::AcceptCountLo,
    &Cmd100::AcceptCountHi,
    &Cmd100::AcceptBlock,
    &Cmd100::ExecRenameFile,	// + return the number of data bytes, LSB }
    &Cmd100::ReturnCountHi,
    &Cmd100::ReturnBlock,
    &Cmd100::SwitchCmd,
    // index 39: write sector }
    &Cmd100::AcceptCountLo,
    &Cmd100::AcceptCountHi,
    &Cmd100::ExecWriteSector,
    &Cmd100::SwitchCmd,
    // index 43: write to file }
    &Cmd100::AcceptCountLo,
    &Cmd100::AcceptCountHi,
    &Cmd100::ExecWriteFile,
    &Cmd100::ReturnCountLo,
    &Cmd100::ReturnCountHi,
    &Cmd100::ReturnBlock,
    &Cmd100::SwitchCmd,
    // index 50: get file size }
    &Cmd100::AcceptCountLo,		// get the file handle }
    &Cmd100::ExecGetSize,
    &Cmd100::ReturnCountLo,
    &Cmd100::ReturnCountHi,
    &Cmd100::SwitchCmd	};


