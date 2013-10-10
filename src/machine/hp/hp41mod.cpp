/*=======================================================================

Author: Warren Furlow (email: warren@furlow.org)

License: PUBLIC DOMAIN - May be freely copied and incorporated into any work

Description:  Contains routines for conversion between HP-41 ROM image file formats

Background:
Each HP-41 ROM page is 4096 words in size and each word is 10 bits.  There are
16 pages in the address space, but pages can be bank switched by up to 4 banks.
The pages 0-7 are for system use and pages that go there are hard coded to that
location.  Pages 8-F are for plug-in module usage through the four physical
ports.  Each port takes up two pages (Page8=Port1 Lower,Page9=Port1 Upper, etc.).
Note that some plug-in modules and peripherals are hard coded to map into certain
system pages (ex: time module).

Supported File Formats:
ROM - This format is used by V41 Release 7 and prior (Warren Furlow).
      It is always 8192 bytes with the High 2 bits followed by the Low 8 bits.

BIN - This format is used by Emu41 (J-F Garnier) and HP41EPC (HrastProgrammer).
      Note: HP41EPC uses BIN format but names them .ROM files.
      All bits are packed into 5120 bytes, but several consecutive pages may
      occupy the same file, so the file size could be a multiple of 5120.
      4 machine words are packed into 5 bytes:
      Byte0=Word0[7-0]
      Byte1=Word1[5-0]<<2 | Word0[9-8]
      Byte2=Word2[3-0]<<4 | Word1[9-6]
      Byte3=Word3[1-0]<<6 | Word2[9-4]
      Byte4=Word3[9-2]

LST - This format is a text file dump of the disassembled machine code generated
      by the Zenrom MCED utility.  The first 4 digit hex number is the absolute
      address and the second 3 digit hex number is the machine word.  The mnemonic
      normally appears after that.  For the purposes of file conversion, only the
      machine word is actually used and only the first page is read from the file
      with the provided routines.
      Example:
      8000 158 M=C
      8001 398 C=ST
      8002 056 C=0    XS
      8003 284 CF     7

MOD - MOD File format is a more advanced multi-page format for containing an entire module or
      all operating system pages.  See MODFile.h  This format is used by V41 Release 8.

Sample usage: convert a lst file to a bin file:
  word *ROM;
  ROM=read_lst_file("test.lst");
  if (ROM==NULL)
    return;
  write_bin_file("test.bin",ROM);
  free(ROM);

Convert a bin file to a rom file:
  ROM=read_bin_file("test.bin",0);
  if (ROM==NULL)
    return;
  write_rom_file("test.rom",ROM);
  free(ROM);

MODULE FILE LOADER - for emulators etc
The exact loading procedure will be dependent on the emulator's implementation.  V41 uses a
three pass process to find empty pages and ensure that the ROM attributes are correctly followed.
Feel free to copy and adapt the algorithm in LoadMOD() to your software.

First Pass:  Validate variables, go through each page in the mod file.  If there any PageGroups,
count the number of pages in each group using the appropriate array (LowerGroup[8], UpperGroup[0], etc).
This count is stored as a negative number.  In the second pass this value will be replaced with
a positive number which will represent the actual page to be loaded

Second Pass: Go through each page again and find free locations for any that are grouped.
If a page is the first one encountered in a group, find a block of free space for the group.
For instance, Odd/Even will require two contiguous spaces.  If a page is the second or subsequent
one encountered in a group, then we already have the block of space found and simply need to
stick it in the right place.  For instance, the lower page has already been loaded, then the upper
page goes right after it.

Third Pass: Find a free location for any non-grouped pages.  This includes system pages which have
their page number hardcoded.

=========================================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<memory.h>

#include <QFile>
#include <QDebug>

#include"hp41mod.h"
#include "hp41.h"


Chp41Mod::Chp41Mod(Chp41 * hp41)
{
    this->hp41 = hp41;
    pModule = new ModuleHeader;
}
Chp41Mod::Chp41Mod(Chp41 * hp41,QString pszFullPath)
{
    this->hp41 = hp41;
    pModule = new ModuleHeader;
    status = LoadMOD( pszFullPath);
}

/****************************/
// Returns 0 for success, 1 for open fail, 2 for read fail, 3 for invalid file, 4 for load conflict or no space
/****************************/
int Chp41Mod::LoadMOD( QString pszFullPath)
  {
#if 1
  uint page,hep_page=0,ww_page=0;

  // open file and read its contents into a buffer
  QFile file(pszFullPath);
  if (! file.open(QIODevice::ReadOnly)) return(1);

  qint64 FileSize=file.size();
  if ((FileSize-sizeof(ModuleFileHeader))%sizeof(ModuleFilePage))
    return(3);


  pBuff = file.readAll();

  if (FileSize!=pBuff.size()) {
      return(2);
  }

  // validate module header
  ModuleFileHeader *pMFH=(ModuleFileHeader*)pBuff.data();
  if (FileSize != sizeof(ModuleFileHeader)+pMFH->NumPages*sizeof(ModuleFilePage) ||
    0!=strcmp(pMFH->FileFormat,MOD_FORMAT) || pMFH->MemModules>4 || pMFH->XMemModules>3 ||
    pMFH->Original>1 || pMFH->AppAutoUpdate>1 || pMFH->Category>CATEGORY_MAX || pMFH->Hardware>HARDWARE_MAX)    /* out of range */
    {
    return(3);
    }


  // info strings
  strncpy(pModule->szFullFileName,pszFullPath.toLatin1().data(),sizeof(pModule->szFullFileName));
  strcpy(pModule->szFileFormat,pMFH->FileFormat);
  strcpy(pModule->szTitle,pMFH->Title);
  strcpy(pModule->szVersion,pMFH->Version);
  strcpy(pModule->szPartNumber,pMFH->PartNumber);
  strcpy(pModule->szAuthor,pMFH->Author);
  strcpy(pModule->szCopyright,pMFH->Copyright);
  strcpy(pModule->szLicense,pMFH->License);
  strcpy(pModule->szComments,pMFH->Comments);
  // special module characteristics
  pModule->MemModules=pMFH->MemModules;
  //MemModules+=pMFH->MemModules;
  pModule->XMemModules=pMFH->XMemModules;
  //XMemModules+=pMFH->XMemModules;
  pModule->Category=pMFH->Category;
  pModule->Hardware=pMFH->Hardware;
  pModule->Original=pMFH->Original;
  pModule->AppAutoUpdate=pMFH->AppAutoUpdate;
  pModule->NumPages=pMFH->NumPages;
//  ModuleList.append(pModuleNew);

#endif
  return(0);
  }


/******************************/
word * Chp41Mod::read_rom_file(QString FullFileName)
  {
  QFile *File;
  long FileSize,SizeRead;
  word *Rom;
  int i;

  File = new QFile(FullFileName);
  if (!File->open(QIODevice::ReadOnly))
    {
//    fprintf(stderr,"ERROR: File Open Failed: %s\n",FullFileName);
    return(NULL);
    }

  FileSize=File->size();
  if (FileSize!=8192)
    {
    File->close();
    fprintf(stderr,"ERROR: File Size Invalid: %s\n",FullFileName.toLatin1().data());
    fprintf(stderr,"  ROM file size is 8192 bytes\n");
    return(NULL);
    }
  Rom=(word*)malloc(sizeof(word)*0x1000);
  if (Rom==NULL)
    {
    File->close();
    fprintf(stderr,"ERROR: Memory Allocation\n");
    return(NULL);
    }
  SizeRead=File->read((char*)Rom,8192);
  File->close();
  if (SizeRead!=8192)
    {
    fprintf(stderr,"ERROR: File Read Failed: %s\n",FullFileName.toLatin1().data());
    free(Rom);
    return(NULL);
    }
  for (i=0;i<0x1000;i++)
    Rom[i]=(Rom[i]<<8)|(Rom[i]>>8);
  return(Rom);
  }



/*******************************/
int Chp41Mod::write_rom_file(char *FullFileName,word *Rom)
  {
  FILE *File;
  long SizeWritten;
  word *Rom2;
  int i;

  if (Rom==NULL)
    return(0);
  File=fopen(FullFileName,"wb");
  if (File==NULL)
    {
    fprintf(stderr,"ERROR: File Open Failed: %s\n",FullFileName);
    return(0);
    }
  Rom2=(word*)malloc(sizeof(word)*0x1000);
  if (Rom2==NULL)
    {
    fclose(File);
    fprintf(stderr,"ERROR: Memory Allocation\n");
    return(0);
    }
  for (i=0;i<0x1000;i++)
    Rom2[i]=(Rom[i]<<8)|(Rom[i]>>8);
  SizeWritten=fwrite(Rom2,1,8192,File);
  fclose(File);
  free(Rom2);
  if (SizeWritten!=8192)
    {
    fprintf(stderr,"ERROR: File Write Failed: %s\n",FullFileName);
    return(0);
    }
  return(1);
  }

/*******************************/
word * Chp41Mod::read_bin_file(char *FullFileName,int Page)
  {
  FILE *File;
  long FileSize,SizeRead;
  byte *BIN;
  word *Rom;

  File=fopen(FullFileName,"rb");
  if (File==NULL)
    {
    fprintf(stderr,"ERROR: File Open Failed: %s\n",FullFileName);
    return(NULL);
    }
  fseek(File,0,SEEK_END);
  FileSize=ftell(File);
  fseek(File,0,SEEK_SET);
  if (FileSize%5120)
    {
    fclose(File);
    fprintf(stderr,"ERROR: File Size Invalid: %s\n",FullFileName);
    return(NULL);
    }
  BIN=(byte*)malloc(FileSize);
  if (BIN==NULL)
    {
    fclose(File);
    fprintf(stderr,"ERROR: Memory Allocation\n");
    return(NULL);
    }
  SizeRead=fread(BIN,1,FileSize,File);
  fclose(File);
  if (SizeRead!=FileSize)
    {
    fprintf(stderr,"ERROR: File Read Failed: %s\n",FullFileName);
    free(BIN);
    return(NULL);
    }

  Rom=(word*)malloc(sizeof(word)*0x1000);
  if (Rom==NULL)
    {
    fprintf(stderr,"ERROR: Memory Allocation\n");
    return(NULL);
    }
  unpack_image(Rom,BIN+Page*5120);
  free(BIN);
  return(Rom);
  }

/*******************************/
int Chp41Mod::write_bin_file(char *FullFileName,word *Rom)
  {
  FILE *File;
  long SizeWritten;
  byte *BIN;

  if (Rom==NULL)
    return(0);
  File=fopen(FullFileName,"wb");
  if (File==NULL)
    {
    fprintf(stderr,"ERROR: File Open Failed: %s\n",FullFileName);
    return(0);
    }

  BIN=(byte*)malloc(5120);
  if (BIN==NULL)
    {
    fprintf(stderr,"ERROR: Memory Allocation\n");
    return(0);
    }
  pack_image(Rom,BIN);
  SizeWritten=fwrite(BIN,1,5120,File);
  fclose(File);
  free(BIN);
  if (SizeWritten!=5120)
    {
    fprintf(stderr,"ERROR: File Write Failed: %s\n",FullFileName);
    return(0);
    }
  return(1);
  }

/*******************************/
word *Chp41Mod::read_lst_file(char *FullFileName)
  {
  FILE *File;
  word *Rom;
  char LST[255];
  int i;
  word addr,mword;

  File=fopen(FullFileName,"rt");
  if (File==NULL)
    {
    fprintf(stderr,"ERROR: File Open Failed: %s\n",FullFileName);
    return(NULL);
    }
  Rom=(word*)malloc(sizeof(word)*0x1000);
  if (Rom==NULL)
    {
    fprintf(stderr,"ERROR: Memory Allocation\n");
    return(NULL);
    }
  i=0;
  while (fgets(LST,sizeof(LST),File)&&i<0x1000)
    {
    if (sscanf(LST,"%x %x",&addr,&mword)==2)
      Rom[i++]=mword;
    }
  fclose(File);
  if (i!=0x1000)
    {
    fprintf(stderr,"ERROR: File Size Invalid: %s\n",FullFileName);
    free(Rom);
    return(NULL);
    }
  return(Rom);
  }

/*******************************/
int Chp41Mod::write_lst_file(char *FullFileName,word *Rom,int Page)
  {
  FILE *File;
  int i;

  if (Rom==NULL)
    return(0);
  File=fopen(FullFileName,"wt");
  if (File==NULL)
    {
    fprintf(stderr,"ERROR: File Open Failed: %s\n",FullFileName);
    return(0);
    }

  for (i=0;i<0x1000;i++)
    fprintf(File,"%04X %03X\n",i+Page*0x1000,Rom[i]&0x03FF);
  fclose(File);
  return(1);
  }

/******************************/
int Chp41Mod::compare_rom_files(char *FullFileName1,char *FullFileName2)
  {
  word *ROM1,*ROM2;
  int res;
  ROM1=read_rom_file(FullFileName1);
  if (ROM1==NULL)
    return(0);
  ROM2=read_rom_file(FullFileName2);
  if (ROM2==NULL)
    {
    free(ROM1);
    return(0);
    }
  res=(0==memcmp(ROM1,ROM2,8192));
  free(ROM1);
  free(ROM2);
  return(res);
  }

/******************************/
void Chp41Mod::unpack_image(
  word *Rom,
  byte *Bin)
  {
  int i;
  word *ptr=Rom;
  if ((Rom==NULL)||(Bin==NULL))
    return;
  for (i=0;i<5120;i+=5)
    {
    *ptr++=((Bin[i+1]&0x03)<<8) | Bin[i];
    *ptr++=((Bin[i+2]&0x0F)<<6) | ((Bin[i+1]&0xFC)>>2);
    *ptr++=((Bin[i+3]&0x3F)<<4) | ((Bin[i+2]&0xF0)>>4);
    *ptr++=(Bin[i+4]<<2) | ((Bin[i+3]&0xC0)>>6);
    }
  }

/******************************/
void Chp41Mod::pack_image(
  word *rom,
  byte *BIN)
  {
  int i,j;
  if ((rom==NULL)||(BIN==NULL))
    return;
  for (i=0,j=0;i<0x1000;i+=4)
    {
    BIN[j++]=rom[i]&0x00FF;
    BIN[j++]=((rom[i+1]&0x003F)<<2) | ((rom[i]&0x0300)>>8);
    BIN[j++]=((rom[i+2]&0x000F)<<4) | ((rom[i+1]&0x03C0)>>6);
    BIN[j++]=((rom[i+3]&0x0003)<<6) | ((rom[i+2]&0x03F0)>>4);
    BIN[j++]=(rom[i+3]&0x03FC)>>2;
    }
  }

/******************************/
/* Returns 0 for success, 1 for open fail, 2 for read fail, 3 for invalid file, 4 for allocation error */
/******************************/
int Chp41Mod::output_mod_info(
  FILE *OutFile,         /* output file or set to stdout */
  int Verbose,           /* generate all info except FAT */
  int DecodeFat)         /* decode fat if it exists */
  {
  char drive[_MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
  QFile *MODFile;
  unsigned long FileSize;


  int i;
  word page_addr;

  if (DecodeFat)
    Verbose=1;

  /* check header */
//  ModuleFileHeader *pMFH=(ModuleFileHeader*)(pBuff.data());

  if (pModule->MemModules>4 || pModule->XMemModules>3 || pModule->Original>1 || pModule->AppAutoUpdate>1 ||
    pModule->Category>CATEGORY_MAX || pModule->Hardware>HARDWARE_MAX)    /* out of range */
    {
    if (Verbose)
      fprintf(OutFile,"ERROR: llegal value(s) in header: %s\n",pModule->szFullFileName);
    return(3);
    }

  if (!Verbose)
    {
    fprintf(OutFile,"%-20s %-30s %-20s\n",pModule->szFullFileName,pModule->szTitle,pModule->szAuthor);
    return(0);
    }

  /* output header info */
  fprintf(OutFile,"FILE NAME: %s\n",pModule->szFullFileName);
  fprintf(OutFile,"FILE FORMAT: %s\n",pModule->szFileFormat);
  fprintf(OutFile,"TITLE: %s\n",pModule->szTitle);
  fprintf(OutFile,"VERSION: %s\n",pModule->szVersion);
  fprintf(OutFile,"PART NUMBER: %pModule",pModule->szPartNumber);
  fprintf(OutFile,"AUTHOR: %s\n",pModule->szAuthor);
  fprintf(OutFile,"COPYRIGHT (c) %s\n",pModule->szCopyright);
  fprintf(OutFile,"LICENSE: %s\n",pModule->szLicense);
  fprintf(OutFile,"COMMENTS: %s\n",pModule->szComments);
  switch (pModule->Category)
    {
    case CATEGORY_UNDEF:
      fprintf(OutFile,"CATEGORY: Not categorized\n");
      break;
    case CATEGORY_OS:
      fprintf(OutFile,"CATEGORY: Operating system\n");
      break;
    case CATEGORY_APP_PAC:
      fprintf(OutFile,"CATEGORY: HP Application Pac\n");
      break;
    case CATEGORY_HPIL_PERPH:
      fprintf(OutFile,"CATEGORY: HP-IL related modules and devices\n");
      break;
    case CATEGORY_STD_PERPH:
      fprintf(OutFile,"CATEGORY: HP Standard peripheral\n");
      break;
    case CATEGORY_CUSTOM_PERPH:
      fprintf(OutFile,"CATEGORY: Custom peripheral\n");
      break;
    case CATEGORY_BETA:
      fprintf(OutFile,"CATEGORY: BETA releases not fully debugged and finished\n");
      break;
    case CATEGORY_EXPERIMENTAL:
      fprintf(OutFile,"CATEGORY: Test programs not meant for normal usage\n");
      break;
    }
  switch (pModule->Hardware)
    {
    case HARDWARE_NONE:
      fprintf(OutFile,"HARDWARE: None\n");
      break;
    case HARDWARE_PRINTER:
      fprintf(OutFile,"HARDWARE: 82143A Printer\n");
      break;
    case HARDWARE_CARDREADER:
      fprintf(OutFile,"HARDWARE: 82104A Card Reader\n");
      break;
    case HARDWARE_TIMER:
      fprintf(OutFile,"HARDWARE: 82182A Time Module or HP-41CX built in timer\n");
      break;
    case HARDWARE_WAND:
      fprintf(OutFile,"HARDWARE: 82153A Barcode Wand\n");
      break;
    case HARDWARE_HPIL:
      fprintf(OutFile,"HARDWARE: 82160A HP-IL Module\n");
      break;
    case HARDWARE_INFRARED:
      fprintf(OutFile,"HARDWARE: 82242A Infrared Printer Module\n");
      break;
    case HARDWARE_HEPAX:
      fprintf(OutFile,"HARDWARE: HEPAX Module\n");
      break;
    case HARDWARE_WWRAMBOX:
      fprintf(OutFile,"HARDWARE: W&W RAMBOX Device\n");
      break;
    case HARDWARE_MLDL2000:
      fprintf(OutFile,"HARDWARE: MLDL2000 Device\n");
      break;
    case HARDWARE_CLONIX:
      fprintf(OutFile,"HARDWARE: CLONIX-41 Module\n");
      break;
    }
  fprintf(OutFile,"MEMORY MODULES: %d\n",pModule->MemModules);
  fprintf(OutFile,"EXENDED MEMORY MODULES: %d\n",pModule->XMemModules);
  fprintf(OutFile,"ORIGINAL: %s\n",pModule->Original?"Yes - unaltered":"No - this file has been updated by a user application");
  fprintf(OutFile,"APPLICATION AUTO UPDATE: %s\n",pModule->AppAutoUpdate?"Yes - update this file when saving other data (for MLDL/RAM)":"No - do not update this file");
  fprintf(OutFile,"NUMBER OF PAGES: %d\n",pModule->NumPages);

  /* go through each page */
  for (i=0;i<pModule->NumPages;i++)
    {
    ModuleFilePage *pMFP;
    word Rom[0x1000];
    char ID[10];
    pMFP=(ModuleFilePage*)(pBuff.data()+sizeof(ModuleFileHeader)+sizeof(ModuleFilePage)*i);

    /* output page info */
    fprintf(OutFile,"\n");
    unpack_image(Rom,pMFP->Image);
    fprintf(OutFile,"ROM NAME: %s\n",pMFP->Name);
    get_rom_id(Rom,ID);
    if (0==strcmp(pMFP->ID,ID))
      fprintf(OutFile,"ROM ID: %s\n",pMFP->ID);
    else
      fprintf(OutFile,"ROM ID: \"%s\" (ACTUAL ID: \"%s\")\n",pMFP->ID,ID);
    if ((pMFP->Page>0x0f && pMFP->Page<POSITION_MIN) || pMFP->Page>POSITION_MAX || pMFP->PageGroup>8 ||
      pMFP->Bank==0 || pMFP->Bank>4 || pMFP->BankGroup>8 || pMFP->Ram>1 || pMFP->WriteProtect>1 || pMFP->FAT>1 ||  /* out of range values */
      (pMFP->PageGroup && pMFP->Page<=POSITION_ANY) ||    /* group pages cannot use non-grouped position codes */
      (!pMFP->PageGroup && pMFP->Page>POSITION_ANY))      /* non-grouped pages cannot use grouped position codes */
      fprintf(OutFile,"WARNING: Page info invalid: %s\n",pModule->szFullFileName);
    if (pMFP->Page<=0x0f)
      {
      fprintf(OutFile,"PAGE: %X - must be in this location\n",pMFP->Page);
      page_addr=pMFP->Page*0x1000;
      }
    else
      {
      fprintf(OutFile,"PAGE: May be in more than one location\n");
      switch (pMFP->Page)
        {
        case POSITION_ANY:
          fprintf(OutFile,"POSITION: Any page 8-F\n");
          break;
        case POSITION_LOWER:
          fprintf(OutFile,"POSITION: In lower relative to upper page\n");
          break;
        case POSITION_UPPER:
          fprintf(OutFile,"POSITION: In upper page relative to lower page\n");
          break;
        case POSITION_ODD:
          fprintf(OutFile,"POSITION: Any odd page (9,B,D,F)\n");
          break;
        case POSITION_EVEN:
          fprintf(OutFile,"POSITION: Any even page (8,A,C,E)\n");
          break;
        case POSITION_ORDERED:
          fprintf(OutFile,"POSITION: Sequentially in MOD file order\n");
          break;
        }
       }
    if (pMFP->PageGroup==0)
      fprintf(OutFile,"PAGE GROUP: 0 - not grouped\n");
    else
      fprintf(OutFile,"PAGE GROUP: %d\n",pMFP->PageGroup);
    fprintf(OutFile,"BANK: %d\n",pMFP->Bank);
    if (pMFP->BankGroup==0)
      fprintf(OutFile,"BANK GROUP: 0 - not grouped\n");
    else
      fprintf(OutFile,"BANK GROUP: %d\n",pMFP->BankGroup);
    fprintf(OutFile,"RAM: %s\n",pMFP->Ram?"Yes":"No");
    fprintf(OutFile,"WRITE PROTECTED: %s\n",pMFP->WriteProtect?"Yes":"No or Not Applicable");
    if (!pMFP->Ram && pMFP->WriteProtect)
      fprintf(OutFile,"WARNING: ROM pages should not have WriteProtect set\n");
    fprintf(OutFile,"FAT: %s\n",pMFP->FAT?"Yes":"No");

    /* output FAT */
    if (pMFP->FAT)
      {
      fprintf(OutFile,"XROM: %d\n",Rom[0]);
      fprintf(OutFile,"FCNS: %d\n",Rom[1]);
      if (!DecodeFat)
        fprintf(OutFile,"(FAT Not Decoded)\n");
      }
    if (pMFP->FAT && DecodeFat)
      {
      word entry_num=0;
      word page_start,addr,jmp_addr;

      if (pMFP->Page<=0xf)
        page_start=pMFP->Page*0x1000;
      else
        page_start=0x8000;

      fprintf(OutFile,"XROM  Addr Function    Type\n");
      addr=2;
      while (entry_num<=Rom[1] && !(Rom[addr]==0 && Rom[addr+1]==0))  /* while entry number is less then number of entries and fat terminator not found */
        {
        jmp_addr=((Rom[addr]&0x0ff)<<8) | (Rom[addr+1]&0x0ff);
        fprintf(OutFile,"%02d,%02d %04X ",Rom[0],entry_num,jmp_addr+page_addr);
        if (Rom[addr]&0x200)
          fprintf(OutFile,"            USER CODE");
        else if (jmp_addr<0x1000)                               /* 4K MCODE def */
          {
          int addr2=jmp_addr;
          char ch,punct;
          int end,prompt;
          do
            {
            addr2--;
            decode_fatchar(Rom[addr2],&ch,&punct,&end);
            fprintf(OutFile,"%c",ch);
            }
          while (!end && addr2>jmp_addr-11);
          while (addr2>jmp_addr-11)                             /* pad it out */
            {
            fprintf(OutFile," ");
            addr2--;
            }
          fprintf(OutFile," 4K MCODE");
          /* function type */
          if (Rom[jmp_addr]==0)
            {
            fprintf(OutFile," Nonprogrammable");
            if (Rom[jmp_addr+1]==0)
              fprintf(OutFile," Immediate");
            else
              fprintf(OutFile," NULLable");
            }
          else
            fprintf(OutFile," Programmable");
          /* prompt type -high two bits of first two chars */
          prompt=(Rom[jmp_addr-1]&0x300)>>8;
          if (prompt && !(Rom[jmp_addr-2]&0x0080))
            prompt|=(Rom[jmp_addr-2]&0x300)>>4;
          switch (prompt)
            {
            case 0:     /* no prompt */
              break;
            case 1:
              fprintf(OutFile," Prompt: Alpha (null input valid)");
              break;
            case 2:
              fprintf(OutFile," Prompt: 2 Digits, ST, INF, IND ST, +, -, * or /");
              break;
            case 3:
              fprintf(OutFile," Prompt: 2 Digits or non-null Alpha");
              break;
            case 11:
              fprintf(OutFile," Prompt: 3 Digits");
              break;
            case 12:
              fprintf(OutFile," Prompt: 2 Digits, ST, IND or IND ST");
              break;
            case 13:
              fprintf(OutFile," Prompt: 2 Digits, IND, IND ST or non-null Alpha");
              break;
            case 21:
              fprintf(OutFile," Prompt: non-null Alpha");
              break;
            case 22:
              fprintf(OutFile," Prompt: 2 Digits, IND or IND ST");
              break;
            case 23:
              fprintf(OutFile," Prompt: 2 digits or non-null Alpha");
              break;
            case 31:
              fprintf(OutFile," Prompt: 1 Digit, IND or IND ST");
              break;
            case 32:
              fprintf(OutFile," Prompt: 2 Digits, IND or IND ST");
              break;
            case 33:
              fprintf(OutFile," Prompt: 2 Digits, IND, IND ST, non-null Alpha . or ..");
              break;
            }
          }
        else
          fprintf(OutFile,"            8K MCODE (Not decoded)");
        fprintf(OutFile,"\n");
        entry_num++;
        addr+=2;
        }

      /* interrupt vectors */
      fprintf(OutFile,"INTERRUPT VECTORS:\n");
      fprintf(OutFile,"Pause loop:                      %03X\n",Rom[0x0ff4]);
      fprintf(OutFile,"Main running loop:               %03X\n",Rom[0x0ff5]);
      fprintf(OutFile,"Deep sleep wake up, no key down: %03X\n",Rom[0x0ff6]);
      fprintf(OutFile,"Off:                             %03X\n",Rom[0x0ff7]);
      fprintf(OutFile,"I/O service:                     %03X\n",Rom[0x0ff8]);
      fprintf(OutFile,"Deep sleep wake up:              %03X\n",Rom[0x0ff9]);
      fprintf(OutFile,"Cold start:                      %03X\n",Rom[0x0ffa]);
      }

    fprintf(OutFile,"CHECKSUM:                        %03X",Rom[0x0fff]);
    if (compute_checksum(Rom)==Rom[0x0fff])
      fprintf(OutFile," (Correct - Same as Computed Value)\n");
    else
      fprintf(OutFile," (Incorrect - Computed Value: %03X)\n",compute_checksum(Rom));

//  {
//  int i;
//  word *ROM2;
//  ROM2=read_rom_file("..\\rom\\41ZL.rom");
//  for (i=0;i<=0xfff;i++)
//    {
//    if (Rom[i]!=ROM2[i])
//      fprintf(OutFile," error on byte %03X)\n",i);
//    }
//  }

    }

  fprintf(OutFile,"\n");
  return(0);
  }

/******************************/
/* Returns 0 for success, 1 for open fail, 2 for read fail, 3 for invalid file, 4 for allocation error */
/******************************/
int Chp41Mod::extract_roms(
  char *FullFileName)
  {
  char drive[_MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
  FILE *MODFile;
  unsigned long FileSize,SizeRead;
  byte *pBuff;
  ModuleFileHeader *pMFH;
  int i;

  /* open and read MOD file into a buffer */
  _splitpath(FullFileName,drive,dir,fname,ext);
  MODFile=fopen(FullFileName,"rb");
  if (MODFile==NULL)
    return(1);
  fseek(MODFile,0,SEEK_END);
  FileSize=ftell(MODFile);
  fseek(MODFile,0,SEEK_SET);
  if ((FileSize-sizeof(ModuleFileHeader))%sizeof(ModuleFilePage))
    {
    fclose(MODFile);
    return(3);
    }
  pBuff=(byte*)malloc(FileSize);
  if (pBuff==NULL)
    {
    fclose(MODFile);
    return(4);
    }
  SizeRead=fread(pBuff,1,FileSize,MODFile);
  fclose(MODFile);
  if (SizeRead!=FileSize)
    {
    free(pBuff);
    return(2);
    }

  /* check header */
  pMFH=(ModuleFileHeader*)pBuff;
  if (FileSize!=sizeof(ModuleFileHeader)+pMFH->NumPages*sizeof(ModuleFilePage))
    {
    free(pBuff);
    return(3);
    }
  if (0!=strcmp(pMFH->FileFormat,MOD_FORMAT))
    {
    free(pBuff);
    return(3);
    }
  if (pMFH->MemModules>4 || pMFH->XMemModules>3 || pMFH->Original>1 || pMFH->AppAutoUpdate>1 ||
    pMFH->Category>CATEGORY_MAX || pMFH->Hardware>HARDWARE_MAX)    /* out of range */
    {
    free(pBuff);
    return(3);
    }

  /* go through each page */
  for (i=0;i<pMFH->NumPages;i++)
    {
    char ROMFileName[255];
    ModuleFilePage *pMFP;
    word Rom[0x1000];
    pMFP=(ModuleFilePage*)(pBuff+sizeof(ModuleFileHeader)+sizeof(ModuleFilePage)*i);
    /* write the ROM file */
    unpack_image(Rom,pMFP->Image);
    _makepath(ROMFileName,drive,dir,pMFP->Name,"ROM");
    write_rom_file(ROMFileName,Rom);
    }
  free(pBuff);
  return(0);
  }

/*******************************/
/* Sum all values and when bit sum overflows into 11th bit add 1. */
/* Then take 2's complement.  Source: Zenrom pg. 101 */
/*******************************/
word Chp41Mod::compute_checksum(word *Rom)
  {
  word checksum=0;
  int i;
  if (Rom==NULL)
    return(0);
  for (i=0;i<0xfff;i++)
    {
    checksum+=Rom[i]&0x3ff;
    if (checksum>0x3ff)
      checksum=(checksum&0x03ff)+1;
    }
  return((~checksum+1)&0x3ff);
  }

/*******************************/
/* gets the ROM ID at the end of the ROM.  This is valid for most ROMs except O/S which seems to only use 0x0ffe */
/* ROM ID is lcd coded and may have punct bits set but only uses chars 0-3f (no halfnut) */
/* the high two bits are apparently meaningless although some ROMs have them set */
/*******************************/
void Chp41Mod::get_rom_id(
  word *Rom,
  char *ID)           /* output: provide char[9] */
  {
  char *ptr=ID;
  char punct;
  int i;
  for (i=0x0ffe;i>=0x0ffb;i--)
    {
    decode_lcdchar((word)(Rom[i]&0x00ff),ptr++,&punct);
    if (punct)
      *(ptr++)=punct;
    }
  *ptr=0;
  }



/*********************/
/* Decodes a 9 bit LCD char into ASCII values where possible */
/* LDC char table: Mcode for Beginners, Emery, pg. 108 */
/*********************/
void Chp41Mod::decode_lcdchar(
  word lcdchar,                     /* LCD char in (bits 8-0) */
  char *ch,                         /* ASCII char out */
  char *punct)                      /* punctuation char out if any or ==0 */
  {
  char val=(char)(lcdchar&0x3f);    /* take off bits 6-8 */
  if (lcdchar&0x0100)               /* bit 8 set - second 4 rows */
    *ch=LCDtoASCII[val+0x040];
  else                              /* first 4 rows */
    *ch=LCDtoASCII[val];
  switch (lcdchar&0x00c0)           /* punct bits 6,7 */
    {
    case 0:
      *punct=0;
      break;
    case 0x0040:
      *punct='.';
      break;
    case 0x0080:
      *punct=':';
      break;
    case 0x00c0:
      *punct=',';
      break;
    }
  }

/*********************/
/* Decodes a 10 bit FAT char into ASCII values where possible */
/*********************/
void Chp41Mod::decode_fatchar(
  word tyte,                        /* tyte in */
  char *ch,                         /* PC char out */
  char *punct,                      /* punctuation char out if any or ==0 */
  int *end)
  {
  word lcdchar=tyte&0x003f;         /* ignore two high bits - used for prompting - Zenrom pg 100. */
  if (tyte&0x40)                    /* bit 6 is special chars (bit 8 in the LCD table) */
    lcdchar|=0x0100;
  decode_lcdchar(lcdchar,ch,punct);
  *end=(tyte&0x0080)?1:0;           /* bit 7 indicates end of name */
  }



