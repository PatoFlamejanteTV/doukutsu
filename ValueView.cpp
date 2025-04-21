#include <windows.h>
#include <stdio.h>
#include "Define.h"
#include "Draw.h"

#define MAX_VALUEVIEW	16


void PutBitmap3( RECT *rcView, long x, long y, RECT *rect, long a );

typedef struct{
	long flag;
	long *px;
	long *py;
	long offset_y;
	long value;
	long count;
	RECT rect;
}VALUEVIEW;

VALUEVIEW gVV[MAX_VALUEVIEW];
static long gVVIndex;
extern RECT grcGame;

void ClearValueView( void )
{

	memset( gVV, 0, sizeof(VALUEVIEW)*MAX_VALUEVIEW );
	gVVIndex = 0;
}


void SetValueView( long *px, long *py, long value )
{
	long minus;
	long v;

	long index;
	int i;


	//使用中のポインタを調べてみる
	for( i = 0; i < MAX_VALUEVIEW; i++ ){
		if( gVV[i].flag && gVV[i].px == px ){
			if( gVV[i].value < 0 && value < 0 )break;
			if( gVV[i].value > 0 && value > 0 )break;
		}
	}
	// 無ければ新規
	if( i == MAX_VALUEVIEW ){
		index = gVVIndex;
		gVVIndex++;
		if( gVVIndex == MAX_VALUEVIEW )gVVIndex = 0;
		gVV[index].count    = 0;
		gVV[index].offset_y = 0;
		gVV[index].value    = value;
	}else{
		index = i;
		gVV[index].count    = 32;
		gVV[index].value    += value;
		value = gVV[index].value;
	}



	if( value < 0 ){
		value *= -1;
		minus = 1;
	}else minus = 0;
	v = value;
	
	
	//右寄せ
	long width;
	if(       value > 999 ){
		width = 8 + 32;
	}else if( value >  99 ){
		width = 8 + 24;
	}else if( value >   9 ){
		width = 8 + 16;
	}else{
		width = 8 +  8;
	}



	gVV[index].flag     = 1;
	gVV[index].px       = px;
	gVV[index].py       = py;

	gVV[index].rect.left   = 40 - width;
	gVV[index].rect.top    = 8 * index;
	gVV[index].rect.right  = 40;
	gVV[index].rect.bottom = 8 * index + 8;


	//ここで数字を指定RECTに書き込み。
	RECT rect[20] = {
		{  0, 56,  8, 64},
		{  8, 56, 16, 64},
		{ 16, 56, 24, 64},
		{ 24, 56, 32, 64},
		{ 32, 56, 40, 64},
		{ 40, 56, 48, 64},
		{ 48, 56, 56, 64},
		{ 56, 56, 64, 64},
		{ 64, 56, 72, 64},
		{ 72, 56, 80, 64},

		{  0, 64,  8, 72},
		{  8, 64, 16, 72},
		{ 16, 64, 24, 72},
		{ 24, 64, 32, 72},
		{ 32, 64, 40, 72},
		{ 40, 64, 48, 72},
		{ 48, 64, 56, 72},
		{ 56, 64, 64, 72},
		{ 64, 64, 72, 72},
		{ 72, 64, 80, 72},
	};

	long dig[4] = {1,10,100,1000};
	long fig[4];//桁毎の数値


	for( i = 3; i >= 0; i-- ){
		fig[i] = 0;
		while( v >= dig[i] ){
			v -= dig[i];
			fig[i]++;
		}
	}

	long sw;
	sw = 0;
	RECT rcPlus  = {32,48,40,56};
	RECT rcMinus = {40,48,48,56};
	CortBox2( &gVV[index].rect, 0, SURF_VALUEVIEW );
	if( minus )
		Surface2Surface( gVV[index].rect.left,
						 gVV[index].rect.top, &rcMinus, SURF_VALUEVIEW, SURF_TEXTBOX );
	else
		Surface2Surface( gVV[index].rect.left,
						 gVV[index].rect.top, &rcPlus,  SURF_VALUEVIEW, SURF_TEXTBOX );
	for( i = 3; i >= 0; i-- ){
		//最大桁までを表示
		if( sw == 0 && i != 0 && !fig[i] )continue;
		sw = 1;
		if( minus )fig[i] += 10;
		Surface2Surface( 8 + (3 - i) * 8,
						 gVV[index].rect.top, &rect[fig[i]], SURF_VALUEVIEW, SURF_TEXTBOX );
	}

	

	
	

}

//
void ActValueView( void )
{
	long v;

	for( v = 0; v < MAX_VALUEVIEW; v++ ){
		if( !gVV[v].flag )continue;
		gVV[v].count++;
		if( gVV[v].count < 32 )
			gVV[v].offset_y -= VS/2;
		if( gVV[v].count > 72 ){//&& gVV[v].count%2 ){
			gVV[v].rect.top++;
//			gVV[v].rect.bottom--;
		}
		if( gVV[v].count > 72 + 8 ){
			gVV[v].flag = 0;
		}
	}
}

//表示
void PutValueView( long flx, long fly )
{
	long offset_x;
	long v;

	for( v = 0; v < MAX_VALUEVIEW; v++ ){
		if( !gVV[v].flag )continue;
//		if( gVV[v].count%2 )continue;
		offset_x = (gVV[v].rect.right - gVV[v].rect.left) / 2;
		PutBitmap3( &grcGame, *gVV[v].px/VS - offset_x - flx/VS,
			                  *gVV[v].py/VS + gVV[v].offset_y/VS - 4 - fly/VS, &gVV[v].rect, SURF_VALUEVIEW );
	}
}