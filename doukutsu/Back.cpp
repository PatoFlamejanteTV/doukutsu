#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Define.h"

#include "Tags.h"

BACKDATA gBack;

extern RECT grcGame;
extern char gDataPath[];

long gWaterY;


static unsigned long _color_black;

BOOL InitBack( char *fName, long type )
{
	FILE *fp;
	char path[MAX_PATH];

	// 色を作る
	_color_black = GetCortBoxColor( RGB( 0x00, 0x00, 0x10 ) );

	sprintf( path, "%s\\%s.pbm", gDataPath, fName );

	BITMAPFILEHEADER bfh;//Bitmapファイルヘッダ
	BITMAPINFOHEADER bih;//Bitmap情報ヘッダ

	//ファイルを開く
	fp = fopen( path, "rb" );
	if( !fp ) return FALSE;

	//ファイルヘッダ読み込み
	fread( &bfh, sizeof(BITMAPFILEHEADER), 1, fp );
	if( bfh.bfType != 0x4d42 ) return FALSE;
	//情報ヘッダ読み込み
	fread( &bih, sizeof(BITMAPINFOHEADER), 1, fp );
	fclose( fp );

	gBack.partsW = bih.biWidth;
	gBack.partsH = bih.biHeight;
	gBack.flag = 1;
	//ワーク領域にロード
	if( !ReloadBitmap_File( fName, SURF_BACK ) ){
		return FALSE;
	}

	gBack.type = type;

	gWaterY = 16*VS*240;

	return TRUE;
}


void ActBack( void ){
	switch( gBack.type ){
	case BKACT_FLOW: // 左へスクロール
		gBack._fx += VS*6;
		break;
	case BKACT_CROUD: // 雲海
	case BKACT_CROU2: // 雲海
		gBack._fx++;
		gBack._fx = gBack._fx%640;
		break;
	}
}

void PutBack( long fx, long fy )
{
//	static _fx;
	long x, y;
	RECT rect;
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = gBack.partsW;
	rect.bottom = gBack.partsH;



	switch( gBack.type ){
	case BKACT_FIXED:
		y = 0;
		for( ; y < SURFACE_HEIGHT;  y+= gBack.partsH ){
			x = 0;
			for( ; x < SURFACE_WIDTH; x += gBack.partsW ){
				PutBitmap4( &grcGame, x, y, &rect, SURF_BACK);
			}
		}
		break;
	case BKACT_SCRHF: // 1 / 2 スクロール 
		y = -( (fy/2/VS) % gBack.partsH );
		for( ; y < SURFACE_HEIGHT;  y+= gBack.partsH ){
			x = -( (fx/2/VS) % gBack.partsW );
			for( ; x < SURFACE_WIDTH; x += gBack.partsW ){
				PutBitmap4( &grcGame, x, y, &rect, SURF_BACK);
			}
		}
		break;
	case BKACT_SCROL:
		y = -( (fy/VS) % gBack.partsH );
		for( ; y < SURFACE_HEIGHT;  y+= gBack.partsH ){
			x = -( (fx/VS) % gBack.partsW );
			for( ; x < SURFACE_WIDTH; x += gBack.partsW ){
				PutBitmap4( &grcGame, x, y, &rect, SURF_BACK);
			}
		}
		break;

//	case BKACT_WATER: CortBox( &grcGame, _color_black ); break;
//	case BKACT_BLACK: CortBox( &grcGame, _color_black ); break;
	case BKACT_FLOW: // 左へスクロール
//		gBack._fx += VS*6;
		y = -( gBack.partsH );
		for( ; y < SURFACE_HEIGHT;  y+= gBack.partsH ){
			x = -( (gBack._fx/VS) % gBack.partsW );
			for( ; x < SURFACE_WIDTH; x += gBack.partsW ){
				PutBitmap4( &grcGame, x, y, &rect, SURF_BACK);
			}
		}
		break;
	case BKACT_CROUD:  // 雲海
	case BKACT_CROU2: // 雲海(アイテムは下に落下)
//		gBack._fx++;
//		gBack._fx = _fx%640;
		
		// 一列目
		rect.top    =  0; rect.bottom = 88; rect.left   = 0; rect.right  = 320;
		PutBitmap4( &grcGame, 0, 0, &rect, SURF_BACK);

		// ２列目
		rect.top    =          88; rect.bottom   = 123;
		rect.left   = gBack._fx/2; rect.right  = 320;
		PutBitmap4( &grcGame,   0,               88, &rect, SURF_BACK);
		rect.left   =  0;
		PutBitmap4( &grcGame, 320 - (gBack._fx/2)%320,         88, &rect, SURF_BACK);

		// ３列目
		rect.top    =               123; rect.bottom = 146;
		rect.left   = (gBack._fx*1)%320; rect.right  = 320;
		PutBitmap4( &grcGame,   0,              123, &rect, SURF_BACK);
		rect.left   =  0;
		PutBitmap4( &grcGame, 320 - (gBack._fx)%320,123, &rect, SURF_BACK);

		// 4列目
		rect.top    =               146; rect.bottom = 176;
		rect.left   = (gBack._fx*2)%320; rect.right  = 320;
		PutBitmap4( &grcGame,   0,              146, &rect, SURF_BACK);
		rect.left   =  0;
		PutBitmap4( &grcGame, 320 - (gBack._fx*2)%320,146, &rect, SURF_BACK);

		// 5列目
		rect.top    =               176; rect.bottom = 240;
		rect.left   = (gBack._fx*4)%320; rect.right  = 320;
		PutBitmap4( &grcGame,   0,              176, &rect, SURF_BACK);
		rect.left   =  0;
		PutBitmap4( &grcGame, 320 - (gBack._fx*4)%320,176, &rect, SURF_BACK);
		break;
	}
}

void PutFront( long fx, long fy )
{
	long xpos, ypos;
	RECT rcWater[] = {
		{ 0, 0,32,16},
		{ 0,16,32,48},
	};

	long x, y;

	long x_1, x_2;
	long y_1, y_2;

	switch( gBack.type ){

	case BKACT_WATER: // 水
//		gWaterY += VS;

		x_1 = fx / (VS*32);
		x_2 = x_1 + 11;

		y_1 = 0;
		y_2 = y_1 + 32;

		for( y = y_1; y < y_2; y++ ){
			ypos = y*32*VS/VS - fy/VS + gWaterY/VS;
			if( ypos < -32 ) continue;
			if( ypos > 240 ) break;
			for( x = x_1; x < x_2; x++ ){
				xpos = x*32*VS/VS - fx/VS;
				PutBitmap3( &grcGame, xpos, ypos, &rcWater[1], SURF_BACK);
				// 水面
				if( !y ) PutBitmap3( &grcGame, xpos, ypos, &rcWater[0], SURF_BACK);
			}
		}

		break;
	}

}
