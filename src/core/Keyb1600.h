#ifndef KEYB1600_H
#define KEYB1600_H

#include "common.h"
#include "Keyb.h"

BYTE scandef_pc1600[] = {
//+0		+1			+2			+3			+4			+5			+6			+7
')',		':',		';',		',',		K_SML,		K_DEF,		K_SHT,		NUL,
'(',		'/',		'*',		'-',		'z',		'a',		'q',		NUL,
'9',		'6',		'3',		'+',		'x',		's',		'w',		NUL,
'8',		'5',		'2',		'.',		'c',		'd',		'e',		NUL,
'7',		'4',		'1',		'0',		'v',		'f',		'r',		NUL,
K_UA,		K_DA,		K_LA,		K_RA,		'b',		'g',		't',		NUL,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,

NUL,		NUL,		K_INS,		K_DEL,		'n',		'h',		'y',		NUL,
NUL,		NUL,		NUL,		K_MOD,		'm',		'j',		'u',		NUL,
NUL,		NUL,		NUL,		K_CLR,		' ',		'k',		'i',		NUL,
NUL,		NUL,		NUL,		NUL,		K_RET,		'l',		'o',		NUL,
NUL,		NUL,		NUL,		NUL,		NUL,		'=',		'p',		NUL,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL
};


TransMap KeyMap1600[]={
{1,	"  -         ",	'-',	486,	190,	1},		//OK
{2,	"  (         ",	'(',	300,	190,	1},		//OK
{3,	"  )         ",	')',	335,	190,    1},     	//OK
{4,	"  *         ",	'*',	485,	160,	1},		//OK
{5,	"  ,         ",	',',	984,	218,	2},		//
{6,	"  .         ",	'.',	415,	218,	1},		//OK
{7,	"  /         ",	'/',	485,	133,	1},		//OK
{8,	"  :         ",	':',	984,	144,	2},		//
{9,	"  ;         ",	';',	984,	181,	2},		//
{11,	"  +         ",	'+',	486,	218,	1},		//OK
{12,	" CLS        ",	K_CLR,	521,	133,	1},		//OK
{13,	" DEF        ",	K_DEF,	25,		103,	5},		//OK
{14,	" RCL        ",	K_RCL,	123,	218,	1},		//OK
{15,	" Down Arrow ",	K_DA,	229,	218,	1},		//OK
{16,	" ENTER      ",	K_RET,	300,	218,	3},		//OK
{17,	" Equal      ",	'=',	450,	218,	1},		//OK
{18,	" RSV        ",	K_RSV,	87,		218,	1},		//OK
{19,	" Left Arrow ",	K_LA ,	521,	189,	1},		//OK
{20,	" MODE       ",	K_MOD,	521,	160,	1},		//OK
{21,	" On/Brk     ",	K_BRK,	523,	103,	5},		//OK
{22,	" Off        ",	K_OF ,	486,	103,	5},		//OK
{23,	" Rigth Arrow",	K_RA ,	521,	217,	1},		//OK
{24,	" SHIFT1     ",	K_SHT,	415,	103,	5},		//OK
 {25,	" SHIFT2     ",	K_SHT,	922,	157,	1},
{26,	" SML        ",	K_SML,	52,		218,	1},		//OK
 {27,	" Space      ",	' ',	158,	218,	3},		//OK
{28,	" Up Arrow   ",	K_UA ,	264,	218,	1},		//OK
{29,	"0           ",	'0',	380,	218,	1},		//OK
{30,	"1           ",	'1',	380,	190,	1},		//OK
{31,	"2           ",	'2',	415,	190,	1},		//OK
{32,	"3           ",	'3',	450,	190,	1},		//OK
{33,	"4           ",	'4',	380,	160,	1},		//OK
{34,	"5           ",	'5',	415,	160,	1},		//OK
{35,	"6           ",	'6',	450,	160,	1},		//OK
{36,	"7           ",	'7',	380,	133,	1},		//OK
{37,	"8           ",	'8',	415,	133,	1},		//OK
{38,	"9           ",	'9',	450,	133,	1},		//OK
{39,	"A           ",	'a',	34,		160,	1},		//OK
{40,	"B           ",	'b',	193,	190,	1},		//OK
{41,	"C           ",	'c',	123,	190,	1},		//OK
{42,	"D           ",	'd',	104,	160,	1},		//OK
{43,	"E           ",	'e',	96,		133,	1},		//OK
{44,	"F           ",	'f',	140,	160,	1},		//OK
{45,	"G           ",	'g',	176,	160,	1},		//OK
{46,	"H           ",	'h',	211,	160,	1},		//OK
{47,	"I           ",	'i',	273,	133,	1},		//OK
{48,	"J           ",	'j',	246,	160,	1},		//OK
{49,	"K           ",	'k',	282,	160,	1},		//OK
{50,	"L           ",	'l',	317,	160,	1},		//OK
{51,	"M           ",	'm',	264,	190,	1},		//OK
{52,	"N           ",	'n',	229,	190,	1},		//OK
{53,	"O           ",	'o',	303,	133,	1},		//OK
{54,	"P           ",	'p',	344,	133,	1},		//OK
{55,	"Q           ",	'q',	24,		133,	1},		//OK
{56,	"R           ",	'r',	131,	133,	1},		//OK
{57,	"S           ",	's',	69,		160,	1},		//OK
{58,	"T           ",	't',	167,	133,	1},		//OK
{59,	"U           ",	'u',	238,	133,	1},		//OK
{60,	"V           ",	'v',	158,	190,	1},		//OK
{61,	"W           ",	'w',	60,		133,	1},		//OK
{62,	"X           ",	'x',	87,		190,	1},		//OK
{63,	"Y           ",	'y',	203,	133,	1},		//OK
{64,	"Z           ",	'z',	52,		190,	1},		//OK
{65,	"F1          ",	K_F1,	99,		103,	5},		//OK
{66,	"F2          ",	K_F2,	146,	103,	5},		//OK
{67,	"F3          ",	K_F3,	193,	103,	5},		//OK
{68,	"F4          ",	K_F4,	241,	103,	5},		//OK
{69,	"F5          ",	K_F5,	288,	103,	5},		//OK
{70,	"F6          ",	K_F6,	335,	103,	5},		//OK

{71,	"CTRL       ",	K_CTRL, 552-388,278,	1},		//OK
{72,	"BS         ",	K_BS,   304-388,44,		9},		//OK
{73,	"KBII        ",	K_KBII, 304-388,44,		9}		//OK
};

int KeyMap1600Lenght = 72;



#endif // KEYB1600_H
