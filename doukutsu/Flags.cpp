#include <windows.h>
#include <stdio.h>
#include "Define.h"
#define MAX_SKIPFLAG  (8*8)


unsigned char gFlagNPC[  MAX_FLAG     / 8 ];
unsigned char gSkipFlag[ MAX_SKIPFLAG / 8 ];

void InitFlags( void )
{
	memset( gFlagNPC, 0x00, MAX_FLAG/8 );
}
void InitSkipFlags( void )
{
	memset( gSkipFlag, 0x00, MAX_SKIPFLAG/8 );
}

void SetNPCFlag( long a )
{
	gFlagNPC[a/8] |= 0x01<<(a%8);
}

void CutNPCFlag( long a)
{
	gFlagNPC[a/8] &= ~( 0x01<<(a%8) );
}

BOOL GetNPCFlag( long a )
{
	if( gFlagNPC[a/8] & ( 0x01<<(a%8) ) )
		return TRUE;
	return FALSE;
}


void SetSkipFlag( long a ){     gSkipFlag[a/8] |= 0x01<<(a%8); }
void CutSkipFlag( long a ){     gSkipFlag[a/8] &= ~( 0x01<<(a%8) ); }
BOOL GetSkipFlag( long a ){ if( gSkipFlag[a/8] & ( 0x01<<(a%8) ) ) return TRUE; return FALSE; }

