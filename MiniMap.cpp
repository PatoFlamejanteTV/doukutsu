#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Define.h"
#include "Tags.h"
#include "KeyControl.h"
#include "Escape.h"


extern MAPDATA gMap;

extern RECT   grcGame;

/*extern long   gKey;
extern long   gKeyTrg;
extern long   gKeyJump;
extern long   gKeyShot;
*/

extern MYCHAR gMC;
extern HWND   ghWnd;

void GetTrg( void );


/*
//Attribute
//＜非表示 0x00＞
#define ATRB_DISABLE	0x00
//＜奥0x01-0x3F＞
#define ATRB_BACK		0x01
#define ATRB_BACK_W		0x02
//＜前0x40-0x5F＞
#define ATRB_FRONT		0x40
#define ATRB_BLOCK		0x41
#define ATRB_DAMAGE		0x42
#define ATRB_SNACK		0x43

#define ATRB_TRI_A		0x50
#define ATRB_TRI_B		0x51
#define ATRB_TRI_C		0x52
#define ATRB_TRI_D		0x53
#define ATRB_TRI_E		0x54
#define ATRB_TRI_F		0x55
#define ATRB_TRI_G		0x56
#define ATRB_TRI_H		0x57
//＜水0x60-0x7F＞
#define ATRB_FRONT_W	0x60
#define ATRB_BLOCK_W	0x61
#define ATRB_DAMAGE_W	0x62
#define ATRB_SNACK_W	0x63

#define ATRB_TRI_A_W	0x70
#define ATRB_TRI_B_W	0x71
#define ATRB_TRI_C_W	0x72
#define ATRB_TRI_D_W	0x73
#define ATRB_TRI_E_W	0x74
#define ATRB_TRI_F_W	0x75
#define ATRB_TRI_G_W	0x76
#define ATRB_TRI_H_W	0x77
//＜流0x80-0xBF＞
#define ATRB_LEFT		0x80
#define ATRB_UP			0x81
#define ATRB_RIGHT		0x82
#define ATRB_DOWN		0x83

#define ATRB_LEFT_W		0xA0
#define ATRB_UP_W		0xA1
#define ATRB_RIGHT_W	0xA2
#define ATRB_DOWN_W		0xA3

*/
unsigned char GetAttribute( long x, long y );
void PutFramePerSecound( void );
void PutMapName( BOOL bMini );

void WriteMiniMapLine( long line )
{
	long x;
	unsigned char a;
	RECT rcLevel[] = {
		{240, 24, 241, 25},
		{241, 24, 242, 25},
		{242, 24, 243, 25},
		{243, 24, 244, 25},
	};

	
	for( x = 0; x < gMap.width; x++ ){
		a = GetAttribute( x, line );
		if( a == ATRB_DISABLE ) Surface2Surface( x, line, &rcLevel[0], SURF_MINIMAP, SURF_TEXTBOX );
		else if(//薄
			a == ATRB_EBLOCK  ||
			a == ATRB_BACK    ||
			a == ATRB_FRONT   ||
			a == ATRB_LEFT    ||
			a == ATRB_UP      ||
			a == ATRB_RIGHT   ||
			a == ATRB_DOWN    ||
			a == ATRB_TRI_B   ||
			a == ATRB_TRI_C   ||
			a == ATRB_TRI_F   ||
			a == ATRB_TRI_G   ||
			a == ATRB_BACK_W  ||
			a == ATRB_FRONT_W ||
			a == ATRB_TRI_B_W ||
			a == ATRB_TRI_C_W ||
			a == ATRB_TRI_F_W ||
			a == ATRB_TRI_G_W ||
			a == ATRB_LEFT_W  ||
			a == ATRB_UP_W    ||
			a == ATRB_RIGHT_W ||
			a == ATRB_DOWN_W  ){
			Surface2Surface( x, line, &rcLevel[1], SURF_MINIMAP, SURF_TEXTBOX );
		}else if( 
			a == ATRB_SNACK   ||
			a == ATRB_SNACK_W ||
			a == ATRB_TRI_A   ||
			a == ATRB_TRI_D   ||
			a == ATRB_TRI_E   ||
			a == ATRB_TRI_H   ||
			a == ATRB_FRONT_W ||
			a == ATRB_TRI_A_W ||
			a == ATRB_TRI_D_W ||
			a == ATRB_TRI_E_W ||
			a == ATRB_TRI_H_W ){
			Surface2Surface( x, line, &rcLevel[2], SURF_MINIMAP, SURF_TEXTBOX );
		}else{
			Surface2Surface( x, line, &rcLevel[3], SURF_MINIMAP, SURF_TEXTBOX );
		}
	}
}


enum_ESCRETURN MiniMapLoop( void )
{
	long f, line;
	RECT rcView;
	RECT rcMiniMap;

	long my_x;
	long my_y;
	unsigned char my_wait;
	RECT my_rect = {0,57, 1,58};

	my_x = (gMC.x/VS+8)/PARTSSIZE;
	my_y = (gMC.y/VS+8)/PARTSSIZE;

	//フェードIN
	for( f = 0; f <= 8; f++ ){
		GetTrg();
		if( gKey & KEY_ESC ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
			case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
			case enum_ESCRETURN_continue: break;
			}
		}
		PutBitmap4( &grcGame,  0, 0, &grcGame, SURF_DISPLAY );
		rcView.left   = SURFACE_WIDTH/2  - gMap.width  *f / 8 / 2;
		rcView.right  = SURFACE_WIDTH/2  + gMap.width  *f / 8 / 2;
		rcView.top    = SURFACE_HEIGHT/2 - gMap.length *f / 8 / 2;
		rcView.bottom = SURFACE_HEIGHT/2 + gMap.length *f / 8 / 2;
		PutMapName( TRUE );
		CortBox( &rcView, 0x00 );
		PutFramePerSecound();
		if( !Flip_SystemTask( ghWnd ) ) return enum_ESCRETURN_exit;//強制終了
	}


	rcMiniMap.left   = 0;
	rcMiniMap.right  = gMap.width; //rcView.right - rcView.left;
	rcMiniMap.top    = 0;
	rcMiniMap.bottom = gMap.length; //rcView.bottom - rcView.top;

	rcView.left   --;
	rcView.right  = rcView.left + gMap.width + 2; //++;
	rcView.top    --;
	rcView.bottom = rcView.top + gMap.length + 2; //++;
	//ミニマップをクリア。
	CortBox2( &rcMiniMap, 0x00, SURF_MINIMAP );

	line = 0;
	my_wait = 0;
	//ミニマップの表示ループ
	while( 1 ){
		GetTrg();
		if( gKeyTrg & (gKeyOk|gKeyCancel) )break;
		if( gKey & KEY_ESC ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
			case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
			case enum_ESCRETURN_continue: break;
			}
		}
		PutBitmap4( &grcGame,  0, 0, &grcGame, SURF_DISPLAY );
		CortBox( &rcView, 0x00 );

		if( line < gMap.length ){
			WriteMiniMapLine( line );
			line++;
		}
		if( line < gMap.length ){
			WriteMiniMapLine( line );
			line++;
		}
		PutBitmap3( &grcGame,  rcView.left+1, rcView.top+1, &rcMiniMap, SURF_MINIMAP );
		PutMapName( TRUE );
		my_wait++;
		if( my_wait/8%2 )PutBitmap3( &grcGame,  my_x + rcView.left+1, my_y + rcView.top+1, &my_rect, SURF_TEXTBOX );
		PutFramePerSecound();

		if( !Flip_SystemTask( ghWnd ) ) return enum_ESCRETURN_exit;//強制終了
	}


	//パレットを戻す

	//フェードOUT
	for( f = 8; f >= -1; f-- ){
		GetTrg();
		if( gKey & KEY_ESC ){
			switch( Call_Escape( ghWnd ) ){
			case enum_ESCRETURN_exit:     return enum_ESCRETURN_exit;
			case enum_ESCRETURN_restart:  return enum_ESCRETURN_restart;
			case enum_ESCRETURN_continue: break;
			}
		}
		PutBitmap4( &grcGame,  0, 0, &grcGame, SURF_DISPLAY );
//		rcView.left   = SURFACE_WIDTH/2 - (gMap.width /2)*f/16;
//		rcView.right  = SURFACE_WIDTH/2 + (gMap.width /2)*f/16;
//		rcView.top    = SURFACE_HEIGHT/2 - (gMap.length/2)*f/16;
//		rcView.bottom = SURFACE_HEIGHT/2 + (gMap.length/2)*f/16;
		rcView.left   = SURFACE_WIDTH/2  - gMap.width  *f / 8 / 2;
		rcView.right  = SURFACE_WIDTH/2  + gMap.width  *f / 8 / 2;
		rcView.top    = SURFACE_HEIGHT/2 - gMap.length *f / 8 / 2;
		rcView.bottom = SURFACE_HEIGHT/2 + gMap.length *f / 8 / 2;
		PutMapName( TRUE );
		CortBox( &rcView, 0x00 );
		PutFramePerSecound();
		if( !Flip_SystemTask( ghWnd ) ) return enum_ESCRETURN_exit;//強制終了
	}
	return enum_ESCRETURN_continue;
}




char gMapping[MAX_MAPPING];
extern long gStageNo;
BOOL IsMapping( void )
{
	if( !gMapping[gStageNo] )return FALSE;
	return TRUE;
}

void StartMapping( void )
{
	memset( gMapping, 0, sizeof(char) * MAX_MAPPING );
}

void SetMapping( long a )
{
	gMapping[a] = 1;

}
