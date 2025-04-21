#include <windows.h>
#include "Draw.h"
#include "Define.h"

#define FADEMODE_OFF	0
#define FADEMODE_IN		1
#define FADEMODE_OUT	2

#define COLOR_FADE	0

typedef struct{
	long mode;//フェードスイッチ( 0:OFF / 1:IN / 2:OUT );
	BOOL bMask;
	long count;//フェードカウント
	char ani_no[15][20];//フェードパーツアニメーション
	char flag[15][20];
	char dir;
}FADEPARAMETER;

static FADEPARAMETER gFade;
static unsigned long _mask_color;
void InitFade( void )
{
	memset( &gFade, 0, sizeof(FADEPARAMETER) );
	_mask_color = GetCortBoxColor( RGB( 0, 0, 32 ) );
}

void SetFadeMask( void )
{
	gFade.bMask = TRUE;
}

void ClearFade( void )
{
	gFade.bMask = FALSE;
	gFade.mode  = FADEMODE_OFF;
}

void StartFadeOut( char dir )
{
	long x, y;

	gFade.mode  = FADEMODE_OUT;
	gFade.count = 0;
	gFade.dir   = dir;
	gFade.bMask = FALSE;

	for( y = 0; y < 15; y++ ){
		for( x = 0; x < 20; x++ ){
			gFade.ani_no[y][x] = 0;
			gFade.flag[y][x]   = 0;
		}
	}
}

void StartFadeIn( char dir )
{
	long x, y;

	gFade.mode  = FADEMODE_IN;
	gFade.count = 0;
	gFade.dir   = dir;
	gFade.bMask = TRUE;

	for( y = 0; y < 15; y++ ){
		for( x = 0; x < 20; x++ ){
			gFade.ani_no[y][x] = 15;
			gFade.flag[y][x]   = 0;
		}
	}
	x = x;
}


void ProcFade( void )
{
	long x, y;

	switch( gFade.mode ){

	case FADEMODE_OFF://処理ナシ
		break;

	case FADEMODE_OUT://フェードアウト

		//フラグ立て処理-----------------------------
		switch( gFade.dir ){
		case DIR_LEFT:// ←
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if( 19 - gFade.count == x ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_RIGHT: // →
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if(gFade.count == x ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_UP: // ↑
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if( 14 - gFade.count == y ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_DOWN: // ↓
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if( gFade.count == y ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_CENTER: // ◎
			for( y = 0; y < 7; y++ )
				for( x = 0; x < 10; x++ )
					if( gFade.count == x + y )           gFade.flag[y][x] = 1;
			for( y = 0; y < 7; y++ )
				for( x = 10; x < 20; x++ )
					if( gFade.count == (19-x) + y )      gFade.flag[y][x] = 1;
			for( y = 7; y < 15; y++ )
				for( x = 0; x < 10; x++ )
					if( gFade.count == x + (14-y) )      gFade.flag[y][x] = 1;
			for( y = 7; y < 15; y++ )
				for( x = 10; x < 20; x++ )
					if( gFade.count == (19-x) + (14-y) ) gFade.flag[y][x] = 1;
			break;
		}


		for( y = 0; y < 15; y++ ){
			for( x = 0; x < 20; x++ ){
				if( gFade.ani_no[y][x] < 15 && gFade.flag[y][x] ){
					gFade.ani_no[y][x]++;
				}
			}
		}

		gFade.count++;
		if( gFade.count > 36 ){
			gFade.bMask = TRUE;
			gFade.mode = FADEMODE_OFF;
		}
		break;
	case FADEMODE_IN://フェードイン

		gFade.bMask = FALSE;//フェードインの時は真っ先にマスクを解除
		//フラグ立て処理-----------------------------
		switch( gFade.dir ){
		case DIR_LEFT:// ←
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if( 19 - gFade.count == x ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_RIGHT: // →
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if( gFade.count == x ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_UP: // ↑
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if( 14 - gFade.count == y ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_DOWN: // ↓
			for( y = 0; y < 15; y++ ){
				for( x = 0; x < 20; x++ ){

					if( gFade.count == y ) gFade.flag[y][x] = 1;

				}
			}
			break;
		case DIR_CENTER: // ◎
			for( y = 0; y < 7; y++ )
				for( x = 0; x < 10; x++ )
					if( 19 - gFade.count == x + y )           gFade.flag[y][x] = 1;
			for( y = 0; y < 7; y++ )
				for( x = 10; x < 20; x++ )
					if( 19 - gFade.count == (19-x) + y )      gFade.flag[y][x] = 1;
			for( y = 7; y < 15; y++ )
				for( x = 0; x < 10; x++ )
					if( 19 - gFade.count == x + (14-y) )      gFade.flag[y][x] = 1;
			for( y = 7; y < 15; y++ )
				for( x = 10; x < 20; x++ )
					if( 19 - gFade.count == (19-x) + (14-y) ) gFade.flag[y][x] = 1;
			break;
		}


		for( y = 0; y < 15; y++ ){
			for( x = 0; x < 20; x++ ){
				if( gFade.ani_no[y][x] > 0 && gFade.flag[y][x] ){
					gFade.ani_no[y][x]--;
				}
			}
		}
				
		gFade.count++;
		if( gFade.count > 36 ){
			gFade.mode = FADEMODE_OFF;
		}
		break;

	}
}
/*
void ProcFade_( void )
{
	long i,j;

	switch( gFS.sw_fade ){
	case SW_FADE_OFF://フェード無し-----------------------
		break;
	case SW_FADE_IN://フェードイン-----------------------
		switch( gFS.dir ){
		case DIR_LEFT://←
			for( j = 0; j < 15; j++ ){
				for( i = 19; i >= 19 - gFS.count; i-- ){
					if( i < 0 )break;
					gFS.ani_no[j][i]--;
					if( gFS.ani_no[j][i] < 0 )
						gFS.ani_no[j][i] = 0;
				}
			}
			break;
		case DIR_RIGHT://→
			for( j = 0; j < 15; j++ ){
				for( i = 0; i <= gFS.count; i++ ){
					if( i > 19 )break;
					gFS.ani_no[j][i]--;
					if( gFS.ani_no[j][i] < 0 )
						gFS.ani_no[j][i] = 0;
				}
			}
			break;
		case DIR_UP://↑
			for( j = 14; j >= 14 - gFS.count; j-- ){
				if( j < 0 )break;
				for( i = 0; i < 20; i++ ){
					gFS.ani_no[j][i]--;
					if( gFS.ani_no[j][i] < 0 )
						gFS.ani_no[j][i] = 0;
				}
			}
			break;
		case DIR_DOWN://↓
			for( j = 0; j <= gFS.count; j++ ){
				if( j > 14 )break;
				for( i = 0; i < 20; i++ ){
					gFS.ani_no[j][i]--;
					if( gFS.ani_no[j][i] < 0 )
						gFS.ani_no[j][i] = 0;
				}
			}
			break;
		}
		gFS.count++;
		if( gFS.count > 20 +16 ){
			gFS.sw_fade = SW_FADE_OFF;
		}
		break;
	case SW_FADE_OUT://フェードアウト-----------------------
		switch( gFS.dir ){
		case DIR_LEFT://←
			for( j = 0; j < 15; j++ ){
				for( i = 19; i >= 19 - gFS.count; i-- ){
					if( i < 0 )break;
					gFS.ani_no[j][i]++;
					if( gFS.ani_no[j][i] > 15 )
						gFS.ani_no[j][i] = 15;
				}
			}
			break;
		case DIR_RIGHT://→
			for( j = 0; j < 15; j++ ){
				for( i = 0; i <= gFS.count; i++ ){
					if( i > 19 )break;
					gFS.ani_no[j][i]++;
					if( gFS.ani_no[j][i] > 15 )
						gFS.ani_no[j][i] = 15;
				}
			}
			break;
		case DIR_UP://↑
			for( j = 14; j >= 14 - gFS.count; j-- ){
				if( j < 0 )break;
				for( i = 0; i < 20; i++ ){
					gFS.ani_no[j][i]++;
					if( gFS.ani_no[j][i] > 15 )
						gFS.ani_no[j][i] = 15;
				}
			}
			break;
		case DIR_DOWN://↓
			for( j = 0; j <= gFS.count; j++ ){
				if( j > 14 )break;
				for( i = 0; i < 20; i++ ){
					gFS.ani_no[j][i]++;
					if( gFS.ani_no[j][i] > 15 )
						gFS.ani_no[j][i] = 15;
				}
			}
			break;
		}
		gFS.count++;
		if( gFS.count > 20 +16 ){
			gFS.sw_fade = SW_FADE_OFF;
			gFS.sw_mask = 1;
		}
		break;
	}
}
*/

extern RECT grcGame;
void PutFade( void )
{
	long x, y;
	RECT rect;

	rect.top = 0;
	rect.bottom = 16;

	if( gFade.bMask ){
		CortBox( &grcGame, _mask_color );
	}else{

		if( gFade.mode == FADEMODE_OFF )return;
		for( y = 0; y < 15; y++ ){
			for( x = 0; x < 20; x++ ){

				rect.left  = gFade.ani_no[y][x] * 16;
				rect.right = rect.left + 16;

				PutBitmap3( &grcGame,  x * 16, y * 16, &rect, SURF_FADE );
			}
		}
	}
}

BOOL GetFadeActive( void )
{
	if( gFade.mode == FADEMODE_OFF) return FALSE;
	return TRUE;
}