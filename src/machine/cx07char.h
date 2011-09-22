#ifndef CX07CHAR_H
#define CX07CHAR_H

UINT8 X07_CarDef[256][8]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x00
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x01
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x02
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x03
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x04
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x05
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x06
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x07
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x08
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x09
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x0A
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x0B
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x0C
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x0D
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x0E
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x0F
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x10
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x11
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x12
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x13
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x14
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x15
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x16
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x17
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x18
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x19
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x1A
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x1B
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x1C
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x1D
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x1E
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x1F
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x20
{0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00},     // 0x21
{0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00},     // 0x22
{0x50,0x50,0xF8,0x50,0xF8,0x50,0x50,0x00},     // 0x23
{0x20,0x78,0xA0,0x70,0x28,0xF0,0x20,0x00},     // 0x24
{0xC0,0xC8,0x10,0x20,0x40,0x98,0x18,0x00},     // 0x25
{0x40,0xA0,0xA0,0x40,0xA8,0x90,0x68,0x00},     // 0x26
{0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x27
{0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00},     // 0x28
{0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00},     // 0x29
{0x00,0x20,0xA8,0x70,0xA8,0x20,0x00,0x00},     // 0x2A
{0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00},     // 0x2B
{0x00,0x00,0x00,0x00,0x60,0x20,0x40,0x00},     // 0x2C
{0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00},     // 0x2D
{0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00},     // 0x2E
{0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00},     // 0x2F
{0x70,0x88,0x98,0xA8,0xC8,0x88,0x70,0x00},     // 0x30
{0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00},     // 0x31
{0x70,0x88,0x08,0x30,0x40,0x80,0xF8,0x00},     // 0x32
{0x70,0x88,0x08,0x30,0x08,0x88,0x70,0x00},     // 0x33
{0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00},     // 0x34
{0xF8,0x80,0xF0,0x08,0x08,0x88,0x70,0x00},     // 0x35
{0x30,0x40,0x80,0xF0,0x88,0x88,0x70,0x00},     // 0x36
{0xF8,0x08,0x10,0x20,0x40,0x40,0x40,0x00},     // 0x37
{0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00},     // 0x38
{0x70,0x88,0x88,0x78,0x08,0x10,0x60,0x00},     // 0x39
{0x00,0x60,0x60,0x00,0x60,0x60,0x00,0x00},     // 0x3A
{0x00,0x60,0x60,0x00,0x60,0x20,0x40,0x00},     // 0x3B
{0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x00},     // 0x3C
{0x00,0x00,0xF8,0x00,0xF8,0x00,0x00,0x00},     // 0x3D
{0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00},     // 0x3E
{0x70,0x88,0x08,0x10,0x20,0x00,0x20,0x00},     // 0x3F
{0x70,0x88,0xB8,0xA8,0xB8,0x80,0x78,0x00},     // 0x40
{0x70,0x88,0x88,0xF8,0x88,0x88,0x88,0x00},     // 0x41
{0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00},     // 0x42
{0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00},     // 0x43
{0xE0,0x90,0x88,0x88,0x88,0x90,0xE0,0x00},     // 0x44
{0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8,0x00},     // 0x45
{0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x00},     // 0x46
{0x70,0x88,0x80,0xB8,0x88,0x98,0x68,0x00},     // 0x47
{0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00},     // 0x48
{0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00},     // 0x49
{0x38,0x10,0x10,0x10,0x10,0x90,0x60,0x00},     // 0x4A
{0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00},     // 0x4B
{0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00},     // 0x4C
{0x88,0xD8,0xA8,0xA8,0x88,0x88,0x88,0x00},     // 0x4D
{0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x00},     // 0x4E
{0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00},     // 0x4F
{0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00},     // 0x50
{0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00},     // 0x51
{0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00},     // 0x52
{0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00},     // 0x53
{0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x00},     // 0x54
{0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00},     // 0x55
{0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00},     // 0x56
{0x88,0x88,0x88,0x88,0xA8,0xA8,0x50,0x00},     // 0x57
{0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00},     // 0x58
{0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00},     // 0x59
{0xF8,0x08,0x10,0x20,0x40,0x80,0xF8,0x00},     // 0x5A
{0x70,0x40,0x40,0x40,0x40,0x40,0x70,0x00},     // 0x5B
{0x88,0x50,0x20,0xF8,0x20,0xF8,0x20,0x00},     // 0x5C
{0x70,0x10,0x10,0x10,0x10,0x10,0x70,0x00},     // 0x5D
{0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00},     // 0x5E
{0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00},     // 0x5F
{0x40,0x20,0x00,0x00,0x00,0x00,0x00,0x00},     // 0x60
{0x00,0x00,0x70,0x08,0x78,0x88,0x78,0x00},     // 0x61
{0x80,0x80,0xF0,0x88,0x88,0x88,0xF0,0x00},     // 0x62
{0x00,0x00,0x70,0x80,0x80,0x80,0x70,0x00},     // 0x63
{0x08,0x08,0x78,0x88,0x88,0x88,0x78,0x00},     // 0x64
{0x00,0x00,0x70,0x88,0xF8,0x80,0x78,0x00},     // 0x65
{0x18,0x20,0x78,0x20,0x20,0x20,0x20,0x00},     // 0x66
{0x00,0x00,0x78,0x88,0x78,0x08,0xF0,0x00},     // 0x67
{0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x00},     // 0x68
{0x00,0x20,0x00,0x60,0x20,0x20,0x70,0x00},     // 0x69
{0x10,0x00,0x30,0x10,0x10,0x10,0x60,0x00},     // 0x6A
{0x40,0x40,0x48,0x50,0x60,0x50,0x48,0x00},     // 0x6B
{0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00},     // 0x6C
{0x00,0x00,0x50,0xA8,0xA8,0xA8,0xA8,0x00},     // 0x6D
{0x00,0x00,0xB0,0x48,0x48,0x48,0x48,0x00},     // 0x6E
{0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00},     // 0x6F
{0x00,0x00,0xF0,0x88,0xF0,0x80,0x80,0x00},     // 0x70
{0x00,0x00,0x78,0x88,0x78,0x08,0x08,0x00},     // 0x71
{0x00,0x00,0x58,0x60,0x40,0x40,0x40,0x00},     // 0x72
{0x00,0x00,0x78,0x80,0x70,0x08,0xF0,0x00},     // 0x73
{0x20,0x20,0x78,0x20,0x20,0x20,0x18,0x00},     // 0x74
{0x00,0x00,0x90,0x90,0x90,0x90,0x68,0x00},     // 0x75
{0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00},     // 0x76
{0x00,0x00,0x88,0x88,0xA8,0xA8,0x50,0x00},     // 0x77
{0x00,0x00,0xC8,0x30,0x20,0x60,0x98,0x00},     // 0x78
{0x00,0x00,0x88,0x88,0x78,0x08,0x70,0x00},     // 0x79
{0x00,0x00,0xF8,0x10,0x20,0x40,0xF8,0x00},     // 0x7A
{0x10,0x20,0x20,0x40,0x20,0x20,0x10,0x00},     // 0x7B
{0x20,0x20,0x20,0x00,0x20,0x20,0x20,0x00},     // 0x7C
{0x40,0x20,0x20,0x10,0x20,0x20,0x40,0x00},     // 0x7D
{0x40,0xA8,0x10,0x00,0x00,0x00,0x00,0x00},     // 0x7E
{0x20,0x00,0x20,0x40,0x80,0x88,0x70,0x00},     // 0x7F
{0x00,0x20,0x70,0xF8,0xF8,0x20,0x70,0x00},     // 0x80
{0x00,0xD8,0xF8,0xF8,0x70,0x70,0x20,0x00},     // 0x81
{0x00,0x70,0x70,0xF8,0xF8,0x20,0x70,0x00},     // 0x82
{0x00,0x20,0x70,0xF8,0xF8,0x70,0x20,0x00},     // 0x83
{0x00,0x70,0x88,0x88,0x88,0x70,0x00,0x00},     // 0x84
{0x00,0x70,0xF8,0xF8,0xF8,0x70,0x00,0x00},     // 0x85
{0x50,0x00,0x70,0x88,0xF8,0x88,0x88,0x00},     // 0x86
{0x20,0x50,0x20,0x70,0x88,0xF8,0x88,0x00},     // 0x87
{0x50,0x00,0x70,0x08,0x78,0x88,0x78,0x00},     // 0x88
{0x40,0x20,0x70,0x08,0x78,0x88,0x78,0x00},     // 0x89
{0x20,0x88,0x70,0x08,0x78,0x88,0x78,0x00},     // 0x8A
{0x10,0x20,0x70,0x08,0x78,0x88,0x78,0x00},     // 0x8B
{0x20,0x20,0x70,0x08,0x78,0x88,0x78,0x00},     // 0x8C
{0x70,0x08,0x78,0x88,0x78,0x00,0xF8,0x00},     // 0x8D
{0x50,0x00,0x70,0x20,0x20,0x20,0x70,0x00},     // 0x8E
{0x50,0x00,0x00,0x60,0x20,0x20,0x70,0x00},     // 0x8F
{0x40,0x20,0x00,0x60,0x20,0x20,0x70,0x00},     // 0x90
{0x20,0x88,0x00,0x60,0x20,0x20,0x70,0x00},     // 0x91
{0x10,0x20,0x00,0x60,0x20,0x20,0x70,0x00},     // 0x92
{0x50,0x00,0x88,0x88,0x88,0x88,0x70,0x00},     // 0x93
{0x50,0x00,0x90,0x90,0x90,0x90,0x68,0x00},     // 0x94
{0x40,0x20,0x90,0x90,0x90,0x90,0x68,0x00},     // 0x95
{0x20,0x88,0x00,0x90,0x90,0x90,0x68,0x00},     // 0x96
{0x10,0x20,0x90,0x90,0x90,0x90,0x68,0x00},     // 0x97
{0x10,0x20,0xF8,0x80,0xF0,0x80,0xF8,0x00},     // 0x98
{0x50,0x00,0x70,0x88,0xF8,0x80,0x78,0x00},     // 0x99
{0x40,0x20,0x70,0x88,0xF8,0x80,0x78,0x00},     // 0x9A
{0x20,0x88,0x70,0x88,0xF8,0x80,0x78,0x00},     // 0x9B
{0x10,0x20,0x70,0x88,0xF8,0x80,0x78,0x00},     // 0x9C
{0x50,0x00,0x70,0x88,0x88,0x88,0x70,0x00},     // 0x9D
{0x50,0x00,0x00,0x70,0x88,0x88,0x70,0x00},     // 0x9E
{0x40,0x20,0x00,0x70,0x88,0x88,0x70,0x00},     // 0x9F
{0x38,0x20,0x20,0x20,0xA0,0x60,0x20,0x00},     // 0xA0
{0x00,0x00,0x00,0x00,0x20,0x50,0x20,0x00},     // 0xA1
{0x70,0x40,0x40,0x40,0x40,0x40,0x40,0x00},     // 0xA2
{0x10,0x10,0x10,0x10,0x10,0x10,0x70,0x00},     // 0xA3
{0x00,0x00,0x00,0x00,0x40,0x20,0x10,0x00},     // 0xA4
{0x00,0x00,0x00,0x00,0x20,0x70,0x20,0x00},     // 0xA5
{0xF8,0x08,0xF8,0x08,0x08,0x10,0x20,0x00},     // 0xA6
{0x00,0x00,0xF8,0x08,0x30,0x20,0x40,0x00},     // 0xA7
{0x00,0x00,0x18,0x60,0xA0,0x20,0x20,0x00},     // 0xA8
{0x00,0x00,0x20,0xF8,0x88,0x10,0x20,0x00},     // 0xA9
{0x00,0x00,0xF8,0x20,0x20,0x20,0xF8,0x00},     // 0xAA
{0x00,0x00,0x10,0xF8,0x30,0x50,0x90,0x00},     // 0xAB
{0x00,0x00,0x40,0xF8,0x48,0x50,0x40,0x00},     // 0xAC
{0x00,0x00,0x70,0x10,0x10,0x10,0xF8,0x00},     // 0xAD
{0x00,0x00,0xF0,0x10,0xF0,0x10,0xF0,0x00},     // 0xAE
{0x00,0x00,0xA8,0xA8,0x08,0x10,0x20,0x00},     // 0xAF
{0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00},     // 0xB0
{0xF8,0x08,0x28,0x30,0x20,0x20,0x40,0x00},     // 0xB1
{0x08,0x10,0x30,0x50,0x90,0x10,0x10,0x00},     // 0xB2
{0x20,0xF8,0x88,0x08,0x08,0x10,0x20,0x00},     // 0xB3
{0xF8,0x20,0x20,0x20,0x20,0x20,0xF8,0x00},     // 0xB4
{0x10,0xF8,0x10,0x30,0x50,0x90,0x10,0x00},     // 0xB5
{0x40,0xF8,0x48,0x48,0x48,0x48,0x98,0x00},     // 0xB6
{0x20,0xF8,0x20,0x20,0xF8,0x20,0x20,0x00},     // 0xB7
{0x78,0x48,0x88,0x08,0x08,0x10,0x20,0x00},     // 0xB8
{0x40,0x78,0x90,0x10,0x10,0x20,0x40,0x00},     // 0xB9
{0xF8,0x08,0x08,0x08,0x08,0x08,0xF8,0x00},     // 0xBA
{0x50,0xF8,0x50,0x50,0x10,0x20,0x40,0x00},     // 0xBB
{0xC0,0x00,0xC8,0x08,0x08,0x10,0xE0,0x00},     // 0xBC
{0xF8,0x08,0x08,0x10,0x20,0x50,0x88,0x00},     // 0xBD
{0x40,0xF8,0x48,0x40,0x40,0x40,0x38,0x00},     // 0xBE
{0x88,0x48,0x08,0x08,0x08,0x10,0x20,0x00},     // 0xBF
{0x78,0x48,0x88,0x38,0x08,0x10,0x20,0x00},     // 0xC0
{0xF8,0x20,0xF8,0x20,0x20,0x40,0x80,0x00},     // 0xC1
{0xA8,0xA8,0xA8,0x08,0x08,0x10,0x20,0x00},     // 0xC2
{0x70,0x00,0xF8,0x20,0x20,0x40,0x80,0x00},     // 0xC3
{0x40,0x40,0x40,0x60,0x50,0x40,0x40,0x00},     // 0xC4
{0x20,0xF8,0x20,0x20,0x20,0x40,0x80,0x00},     // 0xC5
{0x70,0x00,0x00,0x00,0x00,0x00,0xF8,0x00},     // 0xC6
{0xF8,0x08,0x08,0x50,0x20,0x50,0x88,0x00},     // 0xC7
{0x20,0xF8,0x10,0x20,0x70,0xA8,0x20,0x00},     // 0xC8
{0x08,0x08,0x08,0x08,0x10,0x20,0x40,0x00},     // 0xC9
{0x50,0x50,0x50,0x48,0x48,0x48,0x88,0x00},     // 0xCA
{0x80,0x80,0xF8,0x80,0x80,0x80,0x78,0x00},     // 0xCB
{0xF8,0x08,0x08,0x08,0x10,0x20,0x40,0x00},     // 0xCC
{0x40,0xA0,0xA0,0x90,0x10,0x08,0x08,0x00},     // 0xCD
{0x20,0xF8,0x20,0xA8,0xA8,0xA8,0x20,0x00},     // 0xCE
{0xF8,0x08,0x08,0x50,0x20,0x10,0x10,0x00},     // 0xCF
{0xF8,0x00,0x00,0xF8,0x00,0x00,0xF8,0x00},     // 0xD0
{0x20,0x20,0x40,0x40,0x88,0x88,0xF8,0x00},     // 0xD1
{0x08,0x08,0x50,0x20,0x30,0x48,0x80,0x00},     // 0xD2
{0xF8,0x40,0xF8,0x40,0x40,0x40,0x38,0x00},     // 0xD3
{0x40,0xF8,0x48,0x50,0x40,0x40,0x40,0x00},     // 0xD4
{0x70,0x10,0x10,0x10,0x10,0x10,0xF8,0x00},     // 0xD5
{0xF8,0x08,0x08,0xF8,0x08,0x08,0xF8,0x00},     // 0xD6
{0x70,0x00,0xF8,0x08,0x08,0x10,0x20,0x00},     // 0xD7
{0x88,0x88,0x88,0x88,0x08,0x10,0x20,0x00},     // 0xD8
{0x20,0xA0,0xA0,0xA0,0xA0,0xA8,0xB0,0x00},     // 0xD9
{0x80,0x80,0x80,0x88,0x90,0xA0,0xC0,0x00},     // 0xDA
{0xF8,0x88,0x88,0x88,0x88,0x88,0xF8,0x00},     // 0xDB
{0xF8,0x88,0x88,0x08,0x08,0x10,0x20,0x00},     // 0xDC
{0xC0,0x00,0x08,0x08,0x08,0x10,0xE0,0x00},     // 0xDD
{0x40,0x20,0x80,0x40,0x00,0x00,0x00,0x00},     // 0xDE
{0x40,0xA0,0x40,0x00,0x00,0x00,0x00,0x00},     // 0xDF
{0x20,0x88,0x00,0x70,0x88,0x88,0x70,0x00},     // 0xE0
{0x10,0x20,0x00,0x70,0x88,0x88,0x70,0x00},     // 0xE1
{0x00,0x70,0x88,0x88,0x70,0x00,0xF8,0x00},     // 0xE2
{0x50,0x00,0x88,0x50,0x20,0x40,0x80,0x00},     // 0xE3
{0x70,0x88,0x80,0x88,0x70,0x20,0x40,0x00},     // 0xE4
{0x00,0x70,0x80,0x80,0x70,0x20,0x40,0x00},     // 0xE5
{0x70,0x00,0x88,0xC8,0xA8,0x98,0x88,0x00},     // 0xE6
{0x70,0x00,0xB0,0x48,0x48,0x48,0x48,0x00},     // 0xE7
{0xF8,0x88,0x80,0x80,0x80,0x80,0x80,0x00},     // 0xE8
{0xF8,0x88,0x40,0x20,0x40,0x88,0xF8,0x00},     // 0xE9
{0xF8,0x88,0x88,0x88,0x88,0x88,0x88,0x00},     // 0xEA
{0x70,0x88,0x88,0x88,0x88,0x50,0xD8,0x00},     // 0xEB
{0x00,0x68,0x90,0x90,0x90,0x68,0x00,0x00},     // 0xEC
{0x60,0x90,0xB0,0x88,0x88,0xF0,0x80,0x00},     // 0xED
{0x00,0xC8,0x30,0x20,0x20,0x20,0x20,0x00},     // 0xEE
{0x30,0x48,0xA0,0x90,0x90,0x60,0x00,0x00},     // 0xEF
{0x00,0x70,0x80,0x70,0x80,0x70,0x00,0x00},     // 0xF0
{0x08,0x78,0xA0,0x90,0x90,0x60,0x00,0x00},     // 0xF1
{0x60,0x90,0xF8,0x90,0x90,0x60,0x00,0x00},     // 0xF2
{0x00,0x98,0xA0,0xC0,0xA0,0x98,0x00,0x00},     // 0xF3
{0x40,0x20,0x20,0x20,0x50,0x98,0x00,0x00},     // 0xF4
{0x00,0x90,0x90,0x90,0xE8,0x80,0x80,0x00},     // 0xF5
{0x00,0x70,0x88,0x88,0xB0,0x80,0x80,0x00},     // 0xF6
{0x00,0xF8,0x50,0x50,0x50,0x98,0x00,0x00},     // 0xF7
{0x00,0xF8,0x40,0x40,0x48,0x30,0x00,0x00},     // 0xF8
{0x20,0x70,0xA8,0xA8,0x70,0x20,0x20,0x00},     // 0xF9
{0x00,0xC8,0x30,0x20,0x20,0x60,0x98,0x00},     // 0xFA
{0x00,0x98,0x88,0xA8,0xA8,0xD8,0x00,0x00},     // 0xFB
{0x00,0x88,0x88,0x90,0xA0,0xC0,0x00,0x00},     // 0xFC
{0x18,0x20,0x70,0x20,0x60,0xA0,0x58,0x00},     // 0xFD
{0x20,0x70,0xA8,0xA0,0xA8,0x70,0x20,0x00},     // 0xFE
{0x00,0x20,0x00,0xF8,0x00,0x20,0x00,0x00}};    // 0xFF


const UINT8 CT6834::key_tbl[256] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x0d,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x20,0x21,0x22,0x23, 0x24,0x25,0x26,0x27, 0x28,0x29,0x2a,0x2b, 0x2c,0x2d,0x2e,0x2f,
    0x30,0x31,0x32,0x33, 0x34,0x35,0x36,0x37, 0x38,0x39,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x41,0x42,0x43, 0x44,0x45,0x46,0x47, 0x48,0x49,0x4a,0x4b, 0x4c,0x4d,0x4e,0x4f,
    0x50,0x51,0x52,0x53, 0x54,0x55,0x56,0x57, 0x58,0x59,0x5a,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x3a,0x3b, 0x2c,0x2d,0x2e,0x2f,
    0x40,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x5b, 0x5c,0x5d,0x5e,0x00,
    0x00,0x00,0x5c,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
};

const UINT8 CT6834::key_tbl_s[256] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x0d,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x20,0x00,0x00,0x00, 0x0c,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x12,0x16,0x00,
    0x00,0x21,0x22,0x23, 0x24,0x25,0x26,0x27, 0x28,0x29,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x61,0x62,0x63, 0x64,0x65,0x66,0x67, 0x68,0x69,0x6a,0x6b, 0x6c,0x6d,0x6e,0x6f,
    0x70,0x71,0x72,0x73, 0x74,0x75,0x76,0x77, 0x78,0x79,0x7a,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x2a,0x2b, 0x3c,0x3d,0x3e,0x3f,
    0x60,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x7b, 0x7c,0x7d,0x7e,0x00,
    0x00,0x00,0x5f,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
};

const UINT8 CT6834::key_tbl_k[256] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x0d,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x20,0x00,0x00,0x00, 0x0b,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x12,0x16,0x00,
    0xdc,0xc7,0xcc,0xb1, 0xb3,0xb4,0xb5,0xd4, 0xd5,0xd6,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0xc1,0xba,0xbf, 0xbc,0xb2,0xca,0xb7, 0xb8,0xc6,0xcf,0xc9, 0xd8,0xd3,0xd0,0xd7,
    0xbe,0xc0,0xbd,0xc4, 0xb6,0xc5,0xcb,0xc3, 0xbb,0xdd,0xc2,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0xb9,0xda, 0xc8,0xce,0xd9,0xd2,
    0xde,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0xdf, 0xb0,0xd1,0xcd,0x00,
    0x00,0x00,0xdb,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
};

const UINT8 CT6834::key_tbl_ks[256] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x0d,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x20,0x00,0x00,0x00, 0x0c,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x12,0x16,0x00,
    0xa6,0x00,0x00,0xa7, 0xa9,0xaa,0xab,0xac, 0xad,0xae,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0xa8,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0xaf,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0xa4,0x00,0xa1,0xa5,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0xa2, 0x00,0xa3,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
};

const UINT8 CT6834::key_tbl_g[256] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x0d,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x20,0x00,0x00,0x00, 0x0c,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x12,0x16,0x00,
    0x8a,0xe9,0x90,0x91, 0x92,0x93,0xec,0xe0, 0xf2,0xf1,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x88,0xed,0xe4, 0xef,0x99,0xfd,0x9d, 0xfe,0xf9,0xe5,0x9b, 0xf4,0xf5,0x89,0x9e,
    0xf7,0x8b,0xf6,0x9f, 0x97,0x94,0x95,0xfb, 0x98,0x96,0xe1,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x81,0x82, 0x9c,0xf0,0x9a,0x80,
    0xe7,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x84, 0x00,0x85,0xfc,0x00,
    0x00,0x00,0x83,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
};

const UINT8 CT6834::key_tbl_c[256] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x0d,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x20,0x00,0x00,0x00, 0x0b,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x12,0x16,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x01,0x02,0x03, 0x04,0x05,0x06,0x07, 0x08,0x09,0x0a,0x0b, 0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13, 0x14,0x15,0x16,0x17, 0x18,0x19,0x1a,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
};


#endif // CX07CHAR_H
