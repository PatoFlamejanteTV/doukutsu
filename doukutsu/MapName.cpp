#include <windows.h>
#include <stdio.h>

#include "Draw.h"
#include "Sound.h"
#include "Define.h"

struct{
	long flag;
	long wait;
//	RECT rect;
	char name[32];
}gMapName;

extern RECT grcGame;
static RECT _rc     = {0,0,160,12};

void ReadyMapName( unsigned char *str )
{
	long a;
	unsigned char name[24] = {0x89,0x49,0x93,0xac,0x8d,0xb9,0x4f,0x68,
							  0x77,0x64,0x6b,0x1f,0x6f,0x71,0x64,0x72,
							  0x64,0x6d,0x73,0x72,0xff,0x00,0x00,0x00};
//	gMapName.rect   = _rc;
	gMapName.flag   = 0;
	gMapName.wait   = 0;

	if( !strcmp( (char *)str, "u" ) ){
		for( a = 0; a < 24; a++ ){
			name[a] += 1;
		}
		str = name;
	}
	strcpy( gMapName.name, (char *)str );

	a = strlen( gMapName.name );
	CortBox2( &_rc, 0, SURF_MAPNAME );
	PutText2( (160-a*6)/2+6, 0+1, (char *)gMapName.name, COLOR_TEXTSHADOW, SURF_MAPNAME ); 
	PutText2( (160-a*6)/2+6, 0,   (char *)gMapName.name, COLOR_TEXT,       SURF_MAPNAME ); 
}

void PutMapName( BOOL bMini )
{
	RECT rect = {0,0,160,16};

	
	if( bMini ){
		RECT rcBack;
		rcBack.left   = 0;
		rcBack.right  = 320;
		rcBack.top    = 7;
		rcBack.bottom = 8 + 16;
			CortBox( &rcBack, 0 );
			PutBitmap3( &grcGame,  74,10, &_rc, SURF_MAPNAME );
	}else{
		if( gMapName.flag ){
			PutBitmap3( &grcGame,  74,80, &_rc, SURF_MAPNAME );
			gMapName.wait++;
			if( gMapName.wait > 160 )gMapName.flag = 0;
		}
	}
}

void StartMapName( void )
{
	gMapName.flag = 1;
	gMapName.wait = 0;

}

// çƒç\íz
void RestoreMapName( void )
{
	long a;
	a = strlen( gMapName.name );
	CortBox2( &_rc, 0, SURF_MAPNAME );
	PutText2( (160-a*6)/2+6, 0+1, (char *)gMapName.name, COLOR_TEXTSHADOW, SURF_MAPNAME ); 
	PutText2( (160-a*6)/2+6, 0,   (char *)gMapName.name, COLOR_TEXT,       SURF_MAPNAME ); 
}
