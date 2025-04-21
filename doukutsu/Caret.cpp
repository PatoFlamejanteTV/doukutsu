#include <windows.h>
#include <stdio.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "Map.h"



#define MAX_CARET	64

CARET gCrt[MAX_CARET];
extern RECT grcGame;
long Random( int min, int max );
int GetSin( unsigned char deg );
int GetCos( unsigned char deg );

typedef void (* CARETFUNCTION)( CARET *crt );


typedef struct{
	long view_left;
	long view_top;
}CARETTABLE;

CARETTABLE gCaretTable[] = {
	//Caret00 ダミー
	{0,0},
	//Caret01 水飛沫
	{4*VS, 4*VS},
	//Caret02 流れ弾
	{8*VS, 8*VS},
	//Caret03 薬きょう
	{8*VS, 8*VS},
	//Caret04 スネーク
	{8*VS, 8*VS},
	//Caret05 zzz
	{4*VS, 4*VS},
	//Caret06 ボタン残像
	{8*VS, 8*VS},
	//Caret07 ミサイルスモーク
	{4*VS, 4*VS},
	//Caret08 溺死
	{8*VS, 8*VS},
	//Caret09 ?
	{8*VS, 8*VS},
	//Caret10 LEVEL UP!
	{28*VS, 8*VS},
	//Caret11 ダメージスモーク
	{4*VS, 4*VS},
	//Caret12 big smoke
	{16*VS, 16*VS},
	//Caret13 スター
	{4*VS, 4*VS},
	//Caret14 ボム
	{20*VS, 20*VS},
	//Caret15 バブリン破片
	{4*VS, 4*VS},
	//Caret16 EMPTY!
	{20*VS, 4*VS},
	//Caret17 PUSH KEY JUMP!
	{52*VS, 4*VS},
};


void InitCaret( void )
{
	memset( gCrt, 0, sizeof(CARET)*MAX_CARET );
}

void ActCaret00( CARET *crt )
{
}

//水飛沫
void ActCaret01( CARET *crt )
{
	RECT rcLeft[] = {
		{  0, 64,  8, 72},
		{  8, 64, 16, 72},
		{ 16, 64, 24, 72},
		{ 24, 64, 32, 72},
	};
	RECT rcRight[] = {
		{ 64, 24, 72, 32},
		{ 72, 24, 80, 32},
		{ 80, 24, 88, 32},
		{ 88, 24, 96, 32},
	};


	if( crt->act_no == 0 ){
		crt->act_no = 1;
		crt->xm = Random( -VS*2, VS*2 );
		crt->ym = Random( -VS*2,  0 );
	}

	//Act
	crt->ym += VS/8;
	crt->x += crt->xm;
	crt->y += crt->ym;

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 5 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 3 )
			crt->cond = 0;
	}

	//Rect
	if( crt->direct == DIR_LEFT ) crt->rect = rcLeft[  crt->ani_no];
	else                          crt->rect = rcRight[ crt->ani_no];
}

//流れ弾
void ActCaret02( CARET *crt )
{
	RECT rect_left[] = {// 煙
		{  0, 32, 16, 48},
		{ 16, 32, 32, 48},
		{ 32, 32, 48, 48},
		{ 48, 32, 64, 48},
	};
	RECT rect_right[] = {// ノーダメージ
		{176,  0,192, 16},
		{192,  0,208, 16},
		{208,  0,224, 16},
		{224,  0,240, 16},
	};
	RECT rect_up[] = {// 煙
		{  0, 32, 16, 48},
		{ 32, 32, 48, 48},
		{ 16, 32, 32, 48},
	};

	//Act

	//Animation
	switch( crt->direct ){
	case DIR_LEFT:
		crt->ym -= VS/32;
		crt->y += crt->ym;
		crt->ani_wait++;
		if( crt->ani_wait > 5 ){
			crt->ani_wait = 0;
			crt->ani_no++;
		}
		if( crt->ani_no > 3 )
			crt->cond = 0;
		crt->rect = rect_left[  crt->ani_no];
		break;
	case DIR_RIGHT:
		crt->ani_wait++;
		if( crt->ani_wait > 2 ){
			crt->ani_wait = 0;
			crt->ani_no++;
		}
		if( crt->ani_no > 3 )
			crt->cond = 0;
		crt->rect = rect_right[ crt->ani_no];
		break;
	case DIR_UP:
		crt->ani_wait++;
		crt->rect = rect_up[crt->ani_wait/2%3];
		if( crt->ani_wait > 24 ) crt->cond = 0;
		break;
	}


}

//薬莢
void ActCaret03( CARET *crt )
{

	RECT rect[] = {
		{  0, 48, 16, 64},
		{ 16, 48, 32, 64},
		{ 32, 48, 48, 64},
		{ 48, 48, 64, 64},
	};

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 2 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 3 )
			crt->cond = 0;
	}

	//Rect
	crt->rect = rect[crt->ani_no];
}

// スネーク
void ActCaret04( CARET *crt )
{
	RECT rect[] = {
		{ 64, 32, 80, 48},//front
		{ 80, 32, 96, 48},
		{ 96, 32,112, 48},

		{ 64, 48, 80, 64},
		{ 80, 48, 96, 64},
		{ 96, 48,112, 64},

		{ 64, 64, 80, 80},
		{ 80, 64, 96, 80},
		{ 96, 64,112, 80},
	};

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 1 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 2 )
			crt->cond = 0;
	}

	//Rect
	crt->rect = rect[ crt->direct*3 + crt->ani_no ];
}


//.zZ
void ActCaret05( CARET *crt )
{

	RECT rect[] = {

		{ 32, 64, 40, 72},
		{ 32, 72, 40, 80},
		{ 40, 64, 48, 72},
		{ 40, 72, 48, 80},

		{ 40, 64, 48, 72},
		{ 40, 72, 48, 80},
		{ 40, 64, 48, 72},

	};

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 4 ){
		crt->ani_wait = 0;
		crt->ani_no++;
	}
	if( crt->ani_no > 6 )
		crt->cond = 0;
	crt->x += VS/4;
	crt->y -= VS/4;

	//Rect
	crt->rect = rect[crt->ani_no];
}



//small smoke
void ActCaret07( CARET *crt )
{
	RECT rcLeft[] = {// white
		{ 56,  0, 64,  8},
		{ 64,  0, 72,  8},
		{ 72,  0, 80,  8},
		{ 80,  0, 88,  8},
		{ 88,  0, 96,  8},
		{ 96,  0,104,  8},
		{104,  0,112,  8},
	};

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 1 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 6 )
			crt->cond = 0;
	}


	//Rect
	crt->rect = rcLeft[ crt->ani_no];


	switch( crt->direct ){
	case DIR_LEFT:  crt->x -= VS*2; break;
	case DIR_UP:    crt->y -= VS*2; break;
	case DIR_RIGHT: crt->x += VS*2; break;
	case DIR_DOWN:  crt->y += VS*2; break;
	}
//	if( crt->direct == DIR_DOWN )crt->y += VS*3;

}




//溺死
void ActCaret08( CARET *crt )
{
	RECT rcLeft  = { 16, 80, 32, 96};
	RECT rcRight = { 32, 80, 48, 96};
	//Rect
	if( crt->direct == DIR_LEFT ) crt->rect = rcLeft;
	else                          crt->rect = rcRight;
}

// ?
void ActCaret09( CARET *crt )
{
	RECT rcLeft  = {  0, 80, 16, 96};
	RECT rcRight = { 48, 64, 64, 80};
	//Animation?
	crt->ani_wait++;
	if( crt->ani_wait < 5 ) crt->y += -4*VS;
	if( crt->ani_wait == 32 ) crt->cond = 0;

	//Rect
	if( crt->direct == DIR_LEFT ) crt->rect = rcLeft;
	else                          crt->rect = rcRight;
}

//レベルアップ
void ActCaret10( CARET *crt )
{
	RECT rcLeft[]  = {
		{  0,  0, 56, 16},
		{  0, 16, 56, 32},
	};
	RECT rcRight[] = {
		{  0, 96, 56,112},
		{  0,112, 56,128},
	};
	//Animation?
	crt->ani_wait++;
	if( crt->direct == DIR_LEFT ){
		if( crt->ani_wait <  20 ) crt->y += -2*VS;
		if( crt->ani_wait == 80 ) crt->cond = 0;
	}else{
		if( crt->ani_wait <  20 )crt->y += -VS;
		if( crt->ani_wait == 80 ) crt->cond = 0;
	}

	//Rect
	if( crt->direct == DIR_LEFT ) crt->rect = rcLeft[ crt->ani_wait/2%2];
	else                          crt->rect = rcRight[crt->ani_wait/2%2];
}

//ダメージシンボル
void ActCaret11( CARET *crt )
{
	unsigned char deg;

	if( crt->act_no == 0 ){
		crt->act_no = 1;
		deg = (unsigned char)Random( 0, 255 );
		crt->xm = GetCos( deg )*2;
		crt->ym = GetSin( deg )*2;
	}
	crt->x += crt->xm;
	crt->y += crt->ym;
	
	RECT rcRight[] = {// red
		{ 56,  8, 64, 16},
		{ 64,  8, 72, 16},
		{ 72,  8, 80, 16},
		{ 80,  8, 88, 16},
		{ 88,  8, 96, 16},
		{ 96,  8,104, 16},
		{104,  8,112, 16},
	};
	
	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 2 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 6 )
			crt->cond = 0;
	}


	//Rect
	crt->rect = rcRight[crt->ani_no];

}


//Big smoke
void ActCaret12( CARET *crt )
{
	RECT rcLeft[] = {// white
		{112,  0,144, 32},
		{144,  0,176, 32},
	};

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 2 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 1 )
			crt->cond = 0;
	}


	//Rect
	crt->rect = rcLeft[ crt->ani_no];

}



//small star
void ActCaret13( CARET *crt )
{
	RECT rcLeft[] = {// white
		{ 56, 24, 64, 32},
		{ 0, 0,0,0  },
	};

	if( crt->act_no == 0 ){
		crt->act_no = 1;
		switch( crt->direct ){
		case DIR_LEFT:
			crt->xm = Random( -VS*3, VS*3 );
			crt->ym = Random( -VS*1, VS   );
			break;
		case DIR_UP:
			crt->ym = Random( 1, 3 ) * (-VS);
			break;
		}
	}
	switch( crt->direct ){
	case DIR_LEFT:
		crt->xm = crt->xm * 4 / 5;
		crt->ym = crt->ym * 4 / 5;
		break;
	case DIR_UP:
		break;
	}

	crt->x += crt->xm;
	crt->y += crt->ym;

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 20 ){
		crt->cond = 0;
	}


	//Rect
	crt->rect = rcLeft[ crt->ani_wait/2%2];

	if( crt->direct == 5 ){
		crt->x -= VS*4;
	}

}


//ボム
void ActCaret14( CARET *crt )
{

	RECT rect[] = {
		{  0, 96, 40,136},
		{ 40, 96, 80,136},
		{ 80, 96,120,136},
		{120, 96,160,136},
		{160, 96,200,136},
	};

	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 1 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 4 )
			crt->cond = 0;
	}

	//Rect
	crt->rect = rect[crt->ani_no];
}

//Bubblin smoke
void ActCaret15( CARET *crt )
{
	RECT rcLeft[] = {// white
		{  0, 72,  8, 80},
		{  8, 72, 16, 80},
		{ 16, 72, 24, 80},
		{ 24, 72, 32, 80},
	};
	//Animation
	crt->ani_wait++;
	if( crt->ani_wait > 2 ){
		crt->ani_wait = 0;
		crt->ani_no++;
		if( crt->ani_no > 3)
			crt->cond = 0;
	}


	//Rect
	crt->rect = rcLeft[ crt->ani_no];

}

//EMPTY!
void ActCaret16( CARET *crt )
{
	RECT rcLeft[]  = {
		{104,  96,144,104},
		{104, 104,144,112},
	};
	//Animation?
	crt->ani_wait++;
	if( crt->ani_wait <  10 ) crt->y += -VS*2;
	if( crt->ani_wait == 40 ) crt->cond = 0;

	//Rect
//	if( crt->direct == DIR_LEFT )
		crt->rect = rcLeft[ crt->ani_wait/2%2];
}

//PUSH JUMP
void ActCaret17( CARET *crt )
{
	RECT rcLeft[]  = {
		{  0, 144,144,152},
		{  0, 0,0,0},
	};
	crt->ani_wait++;
	if( crt->ani_wait >= 40 ) crt->ani_wait = 0;
	if( crt->ani_wait <  30 ) crt->rect = rcLeft[0];
	else                      crt->rect = rcLeft[1];
}

CARETFUNCTION gpCaretFuncTbl[] = {
	ActCaret00,ActCaret01,ActCaret02,ActCaret03,ActCaret04,ActCaret05,ActCaret04,ActCaret07,ActCaret08,ActCaret09,
	ActCaret10,ActCaret11,ActCaret12,ActCaret13,ActCaret14,ActCaret15,ActCaret16,ActCaret17,
};

void ActCaret( void )
{
	long i;
	long code;

	for( i = 0; i < MAX_CARET; i++ ){
		if( gCrt[i].cond & COND_ALIVE ){

			code = gCrt[i].code;
			gpCaretFuncTbl[code]( &gCrt[i] );

		}
	}
}

void PutCaret( long fx, long fy )
{
	long i;
	for( i = 0; i < MAX_CARET; i++){
		if( gCrt[i].cond & COND_ALIVE ){
			PutBitmap3( &grcGame, ( gCrt[i].x - gCrt[i].view_left ) /VS - fx/VS,
				( gCrt[i].y - gCrt[i].view_top ) /VS - fy/VS,
				&gCrt[i].rect, SURF_CARET );
		}
	}
}


void SetCaret( long x, long y, long code, long dir )
{
	long c;

	for( c = 0; c < MAX_CARET; c++){
		if( !gCrt[c].cond )break;
	}
	if( c == MAX_CARET )return;


	memset( &gCrt[c], 0, sizeof(CARET) );
	gCrt[c].cond      = COND_ALIVE;
	gCrt[c].code      = code;
	gCrt[c].x         = x;
	gCrt[c].y         = y;
	gCrt[c].view_left = gCaretTable[code].view_left;
	gCrt[c].view_top  = gCaretTable[code].view_top;
	gCrt[c].direct    = dir;

}



