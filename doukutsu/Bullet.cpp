#include <windows.h>
#include "Draw.h"
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
//#include "Map.h"
#include "KeyControl.h"


extern MYCHAR gMC;
BULLET        gBul[MAX_BULLET];
extern RECT   grcGame;

void SetNpChar( long code_char, long x, long y, long xm, long ym,
				long dir, NPCHAR *npc, long start_index );
void SetCaret( long x, long y, long code, long dir );
long Random( int min, int max );




void InitBullet( void )
{
	long i;
	for( i = 0; i < MAX_BULLET; i++ )
		gBul[i].cond = 0;
}

//現在出力中の弾を数える
long CountArmsBullet( long arms_code )
{
	long i;
	long count = 0;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE &&
			(gBul[i].code_bullet+2)/3 == arms_code )
			count++;
	}
	return count;
}

//現在出力中の弾を数える
long CountBulletNum( long bullet_code )
{
	long i;
	long count = 0;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE &&
			gBul[i].code_bullet == bullet_code )
			count++;
	}
	return count;
}


//現在出力中の弾を数える
void DeleteBullet( long code )
{
	long i;
	long count = 0;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE &&
			(gBul[i].code_bullet+2)/3 == code )
			gBul[i].cond = 0;
	}
}


void ClearBullet( void )
{
	long i;
	for( i = 0; i < MAX_BULLET; i++ ) gBul[i].cond = 0;
}

void PutBullet( long fx, long fy )
{
	long i;
	long x, y;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE ){
			switch( gBul[i].direct ){
			case DIR_LEFT: x = gBul[i].x - gBul[i].view.front; y = gBul[i].y - gBul[i].view.top;   break;
			case DIR_RIGHT:x = gBul[i].x - gBul[i].view.back;  y = gBul[i].y - gBul[i].view.top;   break;
			case DIR_UP:   x = gBul[i].x - gBul[i].view.top;   y = gBul[i].y - gBul[i].view.front; break;
			case DIR_DOWN: x = gBul[i].x - gBul[i].view.top;   y = gBul[i].y - gBul[i].view.back;  break;
			}
			PutBitmap3( &grcGame, x/VS - fx/VS, y/VS - fy/VS, &gBul[i].rect, SURF_BULLET );
		}
	}
}

typedef struct{
	char damage;
	char life;
	long life_count;
	long bbits;
	long enemyXL;
	long enemyYL;
	long blockXL;
	long blockYL;
//	RANGE hit;//あたり判定
	RANGE view;//表示
}BULLETTABLE;

BULLETTABLE gBulTbl[] = {
	// damage,life,count,bits, hit_e[2], hit_b[2], view[4]
	//ダミー
	{0,0,0,0, 0,0,0,0, 0,0,0,0},
	//01スネーク ----------------------
	{  4,  1, 20, BBITS_BLOCKBUSTER|BBITS_THROWMAP,  4, 4, 2, 2,  8, 8, 8, 8},
	{  6,  1, 23, BBITS_BLOCKBUSTER|BBITS_THROWMAP,  4, 4, 2, 2,  8, 8, 8, 8},
	{  8,  1, 30, BBITS_BLOCKBUSTER|BBITS_THROWMAP,  4, 4, 2, 2,  8, 8, 8, 8},
	//02ポーラスター ----------------------
	{  1,  1,  8, BBITS_BLOCKBUSTER,                 6, 6, 2, 2,  8, 8, 8, 8},
	{  2,  1, 12, BBITS_BLOCKBUSTER,                 6, 6, 2, 2,  8, 8, 8, 8},
	{  4,  1, 16, BBITS_BLOCKBUSTER,                 6, 6, 2, 2,  8, 8, 8, 8},
	//03ファイアボール ---------------------
	{  2,  2,100, BBITS_REPLACE,                     8,16, 4, 2,  8, 8, 8, 8},
	{  3,  2,100, BBITS_REPLACE,                     4, 4, 4, 2,  8, 8, 8, 8},
	{  3,  2,100, BBITS_REPLACE,                     4, 4, 4, 2,  8, 8, 8, 8},
	//04マシンガン ---------------------
	{  2,  1, 20, BBITS_BLOCKBUSTER,                 2, 2, 2, 2,  8, 8, 8, 8},
	{  4,  1, 20, BBITS_BLOCKBUSTER,                 2, 2, 2, 2,  8, 8, 8, 8},
	{  6,  1, 20, BBITS_BLOCKBUSTER,                 2, 2, 2, 2,  8, 8, 8, 8},
	//05ミサイル ---------------------
	{  0, 10, 50, BBITS_REPLACE|BBITS_BLOCKBUSTER,   2, 2, 2, 2,  8, 8, 8, 8},
	{  0, 10, 70, BBITS_REPLACE|BBITS_BLOCKBUSTER,   4, 4, 4, 4,  8, 8, 8, 8},
	{  0, 10, 90, BBITS_REPLACE|BBITS_BLOCKBUSTER,   4, 4, 0, 0,  8, 8, 8, 8},
	//06ボム ---------------------
	{  1,100,100, BBITS_THROWMAP|BBITS_NOBREAK,     16,16, 0, 0,  0, 0, 0, 0},
	{  1,100,100, BBITS_THROWMAP|BBITS_NOBREAK,     16,16, 0, 0,  0, 0, 0, 0},
	{  1,100,100, BBITS_THROWMAP|BBITS_NOBREAK,     16,16, 0, 0,  0, 0, 0, 0},
	//07バブリン ---------------------
	{  1,  1, 20, BBITS_REPLACE,                     2, 2, 2, 2,  4, 4, 4, 4},
	{  2,  1, 20, BBITS_REPLACE,                     2, 2, 2, 2,  4, 4, 4, 4},
	{  2,  1, 20, BBITS_REPLACE,                     4, 4, 4, 4,  4, 4, 4, 4},
	//08スピン ---------------------
	{  3,  1, 32, BBITS_BLOCKBUSTER,                 2, 2, 2, 2,  4, 4, 4, 4},// スピン
	{  0,100,  0, BBITS_BLOCKBUSTER|BBITS_THROWMAP,  8, 8, 8, 8, 12,12,12,12},// エッジ 1
	{127,  1,  2, BBITS_THROWMAP,                    8, 4, 8, 4,  0, 0, 0, 0},// 落石
	//09ブレード ---------------------
	{ 15,  1, 30, BBITS_BLOCKBUSTER|BBITS_THROWMAP,  8, 8, 4, 2,  8, 8, 8, 8},
	{  6,  3, 18, BBITS_BLOCKBUSTER|BBITS_THROWMAP, 10,10, 4, 2, 12,12,12,12},
	{  1,100, 30, BBITS_BLOCKBUSTER|BBITS_THROWMAP,  6, 6, 4, 4, 12,12,12,12},
	//10スーパーミサイル ---------------------
	{  0, 10, 30, BBITS_REPLACE|BBITS_BLOCKBUSTER,   2, 2, 2, 2,  8, 8, 8, 8},
	{  0, 10, 40, BBITS_REPLACE|BBITS_BLOCKBUSTER,   4, 4, 4, 4,  8, 8, 8, 8},
	{  0, 10, 40, BBITS_REPLACE|BBITS_BLOCKBUSTER,   4, 4, 0, 0,  8, 8, 8, 8},
	//11スーパーボム ---------------------
	{  2,100,100, BBITS_THROWMAP|BBITS_NOBREAK,     12,12, 0, 0,  0, 0, 0, 0},
	{  2,100,100, BBITS_THROWMAP|BBITS_NOBREAK,     12,12, 0, 0,  0, 0, 0, 0},
	{  2,100,100, BBITS_THROWMAP|BBITS_NOBREAK,     12,12, 0, 0,  0, 0, 0, 0},
	//12ネメシス ---------------------
	{  4,  4, 20, BBITS_BLOCKBUSTER,                 4, 4, 3, 3,  8, 8,24, 8},
	{  4,  2, 20, BBITS_BLOCKBUSTER,                 2, 2, 2, 2,  8, 8,24, 8},
	{  1,  1, 20, BBITS_BLOCKBUSTER,                 2, 2, 2, 2,  8, 8,24, 8},
	//13シュプール ---------------------
	{  4,  4,  30, BBITS_BLOCKERASE,                 6, 6, 3, 3,  8, 8, 8, 8},
	{  8,  8,  30, BBITS_BLOCKERASE,                 6, 6, 3, 3,  8, 8, 8, 8},
	{ 12, 12,  30, BBITS_BLOCKERASE,                 6, 6, 3, 3,  8, 8, 8, 8},
	//14シュプール テイル --------------
	{  3,100, 30, BBITS_BLOCKBUSTER,                 6, 6, 3, 3,  4, 4, 4, 4},
	{  6,100, 30, BBITS_BLOCKBUSTER,                 6, 6, 3, 3,  4, 4, 4, 4},
	{ 11,100, 30, BBITS_BLOCKBUSTER,                 6, 6, 3, 3,  4, 4, 4, 4},
	//15カーリーネメシス ---------------------
	{  4,  4, 20, BBITS_BLOCKBUSTER,                 4, 4, 3, 3,  8, 8,24, 8},
	{  0,  4,  4, BBITS_THROWMAP,                    0, 0, 0, 0,  0, 0,0,  0},// 敵全滅
	{  1,  1,  1, BBITS_THROWMAP|BBITS_BLOCKBUSTER,  1, 1, 1, 1,  1, 1,1,  1},


};

void SetBullet( long no, long x, long y, long dir )
{
	long i;

	i = 0;
	while( i < MAX_BULLET && gBul[i].cond & COND_ALIVE )i++;
	if( i >= MAX_BULLET )return;


	memset( &gBul[i], 0, sizeof(BULLET) );

	gBul[i].code_bullet = no;
	gBul[i].cond        = COND_ALIVE;
	gBul[i].direct      = (unsigned char)dir;
	gBul[i].damage      = gBulTbl[no].damage;
	gBul[i].life        = gBulTbl[no].life;
	gBul[i].life_count  = gBulTbl[no].life_count;
	gBul[i].bbits       = gBulTbl[no].bbits;

	gBul[i].enemyXL     = gBulTbl[no].enemyXL     * VS;
	gBul[i].enemyYL     = gBulTbl[no].enemyYL     * VS;
	gBul[i].blockXL     = gBulTbl[no].blockXL     * VS;
	gBul[i].blockYL     = gBulTbl[no].blockYL     * VS;

	gBul[i].view.back   = gBulTbl[no].view.back   * VS;
	gBul[i].view.front  = gBulTbl[no].view.front  * VS;
	gBul[i].view.top    = gBulTbl[no].view.top    * VS;
	gBul[i].view.bottom = gBulTbl[no].view.bottom * VS;

	gBul[i].x    = x;
	gBul[i].y    = y;

}

//スネーク
void ActBullet_Frontia1( BULLET *bul )
{
	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}

	//start
	if( !bul->act_no ){
		bul->ani_no = Random( 0,2 );
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -3*VS; break;
		case DIR_UP:    bul->ym = -3*VS; break;
		case DIR_RIGHT: bul->xm =  3*VS; break;
		case DIR_DOWN:  bul->ym =  3*VS; break;
		}
	}else{
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	//アニメーション
	bul->ani_wait++;
	if( bul->ani_wait > 0 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 3 )bul->ani_no = 0;

	RECT rcLeft[] = {
		{136, 80,152, 80},
		{120, 80,136, 96},
		{136, 64,152, 80},
		{120, 64,136, 80},
	};
	RECT rcRight[] = {
		{120, 64,136, 80},
		{136, 64,152, 80},
		{120, 80,136, 96},
		{136, 80,152, 80},
	};
	if( bul->direct == DIR_LEFT )bul->rect = rcLeft[ bul->ani_no];
	else                         bul->rect = rcRight[bul->ani_no];

//	SetNpChar( 129, bul->x, bul->y, 0, 0, bul->ani_no,   NULL, MAX_NPC/2 );

}


//スネーク
void ActBullet_Frontia2( BULLET *bul, long level )
{
	static unsigned long inc = 0;

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}

	//start
	if( !bul->act_no ){
		bul->ani_no = Random( 0,2 );
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -1*VS; break;
		case DIR_UP:    bul->ym = -1*VS; break;
		case DIR_RIGHT: bul->xm =  1*VS; break;
		case DIR_DOWN:  bul->ym =  1*VS; break;
		}

		inc++;
		switch( bul->direct ){
		case DIR_LEFT:
		case DIR_RIGHT:
			if( inc%2 ) bul->ym =  VS*2;
			else        bul->ym = -VS*2;
			break;

		case DIR_UP:			
		case DIR_DOWN:			
			if( inc%2 ) bul->xm =  VS*2;
			else        bul->xm = -VS*2;
			break;
		}

	}else{
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm += -VS/4; break;
		case DIR_UP:    bul->ym += -VS/4; break;
		case DIR_RIGHT: bul->xm +=  VS/4; break;
		case DIR_DOWN:  bul->ym +=  VS/4; break;
		}
		switch( bul->direct ){
		case DIR_LEFT:
		case DIR_RIGHT:
			if( bul->count1%5 == 2 ){
				if( bul->ym < 0 ) bul->ym =  VS*2;
				else              bul->ym = -VS*2;
			}
			break;

		case DIR_UP:			
		case DIR_DOWN:			
			if( bul->count1%5 == 2 ){
				if( bul->xm < 0 ) bul->xm =  VS*2;
				else              bul->xm = -VS*2;
			}
			break;
		}


		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	//アニメーション
	bul->ani_wait++;
	if( bul->ani_wait > 0 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 2 ) bul->ani_no = 0;

	RECT rect[] = {
		{192, 16, 208, 32},
		{208, 16, 224, 32},
		{224, 16, 240, 32},
	};
	bul->rect = rect[bul->ani_no];
	if( level == 2 ) SetNpChar( 129, bul->x, bul->y, 0, -VS, bul->ani_no,   NULL, MAX_NPC/2 );
	else             SetNpChar( 129, bul->x, bul->y, 0, -VS, bul->ani_no+3, NULL, MAX_NPC/2 );

}





//ポーラスター
void ActBullet_PoleStar( BULLET *bul, long level )
{

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}
	//start
	if( !bul->act_no ){
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -8*VS; break;
		case DIR_UP:    bul->ym = -8*VS; break;
		case DIR_RIGHT: bul->xm =  8*VS; break;
		case DIR_DOWN:  bul->ym =  8*VS; break;
		}

		switch( level ){
		case 1:
			switch( bul->direct ){
			case DIR_LEFT:  bul->enemyYL = 2*VS; break;
			case DIR_UP:    bul->enemyXL = 2*VS; break;
			case DIR_RIGHT: bul->enemyYL = 2*VS; break;
			case DIR_DOWN:  bul->enemyXL = 2*VS; break;
			}
			break;
		case 2:
			switch( bul->direct ){
			case DIR_LEFT:  bul->enemyYL = 4*VS; break;
			case DIR_UP:    bul->enemyXL = 4*VS; break;
			case DIR_RIGHT: bul->enemyYL = 4*VS; break;
			case DIR_DOWN:  bul->enemyXL = 4*VS; break;
			}
			break;
		}
	}else{
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	//アニメーション
	RECT rect1[] = {
		{128, 32, 144, 48},
		{144, 32, 160, 48},
	};
	RECT rect2[] = {
		{160, 32, 176, 48},
		{176, 32, 192, 48},
	};
	RECT rect3[] = {
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};
	switch( level ){
	case 1:
		if( bul->direct == DIR_UP || bul->direct == DIR_DOWN ) bul->rect = rect1[1];
		else                                                   bul->rect = rect1[0];
		break;
	case 2:
		if( bul->direct == DIR_UP || bul->direct == DIR_DOWN ) bul->rect = rect2[1];
		else                                                   bul->rect = rect2[0];
		break;
	case 3:
		if( bul->direct == DIR_UP || bul->direct == DIR_DOWN ) bul->rect = rect3[1];
		else                                                   bul->rect = rect3[0];
		break;
	}
}

//ファイアボール
void ActBullet_FireBall( BULLET *bul, long level )
{
	BOOL bBreak;



	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}

	bBreak = FALSE;
	//壁消滅
	if(	bul->flag & FLAG_HIT_TOP  && bul->flag & FLAG_HIT_BOTTOM ) bBreak = TRUE;
	if(	bul->flag & FLAG_HIT_LEFT && bul->flag & FLAG_HIT_RIGHT  ) bBreak = TRUE;
	if( bul->direct == DIR_LEFT   && bul->flag & FLAG_HIT_LEFT   ) bul->direct = DIR_RIGHT;//bBreak = TRUE;
	if( bul->direct == DIR_RIGHT  && bul->flag & FLAG_HIT_RIGHT  ) bul->direct = DIR_LEFT; //bBreak = TRUE;

	if( bBreak ){	
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_SMOKE, DIR_LEFT );
		PlaySoundObject( WAVE_SMOKE, 1 );
		return;
	}
	
	
	//start
	if( !bul->act_no ){
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:
			bul->xm = -2*VS;
			break;
		case DIR_UP:
			bul->xm = gMC.xm;
			if( gMC.xm < 0 )bul->direct = DIR_LEFT;
			else            bul->direct = DIR_RIGHT;
			if( gMC.direct == DIR_LEFT ) bul->xm -= VS/4;
			else                         bul->xm +=  VS/4;
			bul->ym = -MAX_MOVE; 
			break;
		case DIR_RIGHT:
			bul->xm =  2*VS;
			break;
		case DIR_DOWN:
			bul->xm = gMC.xm;
			if( gMC.xm < 0 )bul->direct = DIR_LEFT;
			else            bul->direct = DIR_RIGHT;
			bul->ym = +MAX_MOVE; 
			break;
		}
	}else{
		//移動
		if( bul->flag & FLAG_HIT_BOTTOM )bul->ym = -VS*2;
		else if( bul->flag & FLAG_HIT_LEFT  )bul->xm =  2*VS;
		else if( bul->flag & FLAG_HIT_RIGHT )bul->xm = -2*VS;
		bul->ym += VS/6;
		if( bul->ym > 2*VS-1 ) bul->ym = VS*2-1;
		bul->x += bul->xm;
		bul->y += bul->ym;
		//効果音
		if( bul->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT|FLAG_HIT_BOTTOM) ){
			PlaySoundObject( WAVE_FIRE, 1 );
		}
	}

	RECT rect_left1[] = {
		{128,  0,144, 16},
		{144,  0,160, 16},
		{160,  0,176, 16},
		{176,  0,192, 16},
	};
	RECT rect_right1[] = {
		{128, 16,144, 32},
		{144, 16,160, 32},
		{160, 16,176, 32},
		{176, 16,192, 32},
	};

	RECT rect_left2[] = {
		{192, 16,208, 32},
		{208, 16,224, 32},
		{224, 16,240, 32},
	};
	RECT rect_right2[] = {
		{224, 16,240, 32},
		{208, 16,224, 32},
		{192, 16,208, 32},
	};


	bul->ani_no++;
	if( level == 1 ){
		if( bul->ani_no > 3 )bul->ani_no = 0;
		if( bul->direct == DIR_LEFT ) bul->rect = rect_left1[bul->ani_no];
		else                          bul->rect = rect_right1[bul->ani_no];
	}else{
		if( bul->ani_no > 2 )bul->ani_no = 0;
		if( bul->direct == DIR_LEFT ) bul->rect = rect_left2[bul->ani_no];
		else                          bul->rect = rect_right2[bul->ani_no];
		if( level == 2 ) SetNpChar( 129, bul->x, bul->y, 0, -VS, bul->ani_no,   NULL, MAX_NPC/2 );
		else             SetNpChar( 129, bul->x, bul->y, 0, -VS, bul->ani_no+3, NULL, MAX_NPC/2 );
	}



}




//マシンガン
void ActBullet_MachineGun( BULLET *bul, long level )
{
	long move;
	RECT rect1[] = {
		{ 64,  0, 80, 16},//level1
		{ 80,  0, 96, 16},
		{ 96,  0,112, 16},
		{112,  0,128, 16},
	};
	RECT rect2[] = {
		{ 64, 16, 80, 32},//level2
		{ 80, 16, 96, 32},
		{ 96, 16,112, 32},
		{112, 16,128, 32},
	};
	RECT rect3[] = {
		{ 64, 32, 80, 48},//level3
		{ 80, 32, 96, 48},
		{ 96, 32,112, 48},
		{112, 32,128, 48},
	};

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}
	//start
	if( !bul->act_no ){
		switch( level ){
		case 1: move = 8*VS; break;
		case 2: move = 8*VS; break;
		case 3: move = 8*VS; break;
		}
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:
			bul->xm = -move;
			bul->ym = Random( -VS/3, VS/3 );
			break;
		case DIR_UP:
			bul->ym = -move; 
			bul->xm = Random( -VS/3, VS/3 );
			break;
		case DIR_RIGHT:
			bul->xm =  move;
			bul->ym = Random( -VS/3, VS/3 );
			break;
		case DIR_DOWN:
			bul->ym =  move; 
			bul->xm = Random( -VS/3, VS/3 );
			break;
		}
	}else{
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
		switch( level ){
		case 1:
			bul->rect = rect1[bul->direct];
			break;
		case 2:
			bul->rect = rect2[bul->direct];
			if( bul->direct == DIR_UP || bul->direct == DIR_DOWN )
				SetNpChar( 127, bul->x, bul->y, 0, 0, DIR_UP,   NULL, MAX_NPC/2 );
			else
				SetNpChar( 127, bul->x, bul->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			break;
		case 3:
			bul->rect = rect3[bul->direct];
				SetNpChar( 128, bul->x, bul->y, 0, 0, bul->direct, NULL, MAX_NPC/2 );

			break;
		}

	}


}


// ミサイル
void ActBullet_Missile( BULLET *bul, long level )
{
	BOOL bHit;

	static unsigned long inc;

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}

	bHit = FALSE;

	if( bul->life != 10                                         ) bHit = TRUE;
	if( bul->direct == DIR_LEFT  && bul->flag & FLAG_HIT_LEFT   ) bHit = TRUE;
	if( bul->direct == DIR_RIGHT && bul->flag & FLAG_HIT_RIGHT  ) bHit = TRUE;
	if( bul->direct == DIR_UP    && bul->flag & FLAG_HIT_TOP    ) bHit = TRUE;
	if( bul->direct == DIR_DOWN  && bul->flag & FLAG_HIT_BOTTOM ) bHit = TRUE;

	if( bul->direct == DIR_LEFT  && bul->flag & FLAG_HIT_RUP    ) bHit = TRUE;
	if( bul->direct == DIR_LEFT  && bul->flag & FLAG_HIT_RDOWN  ) bHit = TRUE;
	if( bul->direct == DIR_RIGHT && bul->flag & FLAG_HIT_LUP    ) bHit = TRUE;
	if( bul->direct == DIR_RIGHT && bul->flag & FLAG_HIT_LDOWN  ) bHit = TRUE;

	if( bHit ){
		SetBullet( BULLET_BOM1 + level-1, bul->x, bul->y, DIR_LEFT );
		bul->cond = 0;
	}

	//start
	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:
		case DIR_RIGHT:
			bul->tgt_y = bul->y;
			break;
		case DIR_UP:
		case DIR_DOWN:
			bul->tgt_x = bul->x;
			break;
		}
		if( level == 3 ){
			switch( bul->direct ){
			case DIR_LEFT:
			case DIR_RIGHT:
				if( bul->y > gMC.y ) bul->ym = +VS/2;
				else                 bul->ym = -VS/2;
				bul->xm =  Random(-VS, VS );
				break;
			case DIR_UP:
			case DIR_DOWN:
				if( bul->x > gMC.x ) bul->xm = +VS/2;
				else                 bul->xm = -VS/2;
				bul->ym =  Random(-VS, VS );
				break;
			}
			inc++;
			switch( inc%3 ){
			case 0: bul->ani_no = VS/4;  break;
			case 1: bul->ani_no = VS/8;  break;
			case 2: bul->ani_no = VS/10; break;
			}

		}else{
			bul->ani_no = VS/4;
		}
	case 1:
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm += -bul->ani_no; break;
		case DIR_UP:    bul->ym += -bul->ani_no; break;
		case DIR_RIGHT: bul->xm +=  bul->ani_no; break;
		case DIR_DOWN:  bul->ym +=  bul->ani_no; break;
		}

		if( level == 3 ){
			switch( bul->direct ){
			case DIR_LEFT:
			case DIR_RIGHT:
				if( bul->y < bul->tgt_y ) bul->ym += VS/16;
				else                      bul->ym -= VS/16;
				break;
			case DIR_UP:
			case DIR_DOWN:
				if( bul->x < bul->tgt_x ) bul->xm += VS/16;
				else                      bul->xm -= VS/16;
				break;
			}
		}

		if( bul->xm < -VS*5 ) bul->xm = -VS*5;
		if( bul->xm >  VS*5 ) bul->xm =  VS*5;
		if( bul->ym < -VS*5 ) bul->ym = -VS*5;
		if( bul->ym >  VS*5 ) bul->ym =  VS*5;
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
		break;
	}
	bul->count2++;
	if(bul->count2 > 2 ){
		bul->count2 = 0;
		switch( bul->direct ){
		case 0: SetCaret( bul->x+8*VS, bul->y, CARET_MISSILE, DIR_RIGHT ); break;
		case 1: SetCaret( bul->x, bul->y+8*VS, CARET_MISSILE, DIR_DOWN  ); break;
		case 2: SetCaret( bul->x-8*VS, bul->y, CARET_MISSILE, DIR_LEFT  ); break;
		case 3: SetCaret( bul->x, bul->y-8*VS, CARET_MISSILE, DIR_UP    ); break;
		}
	}

	//アニメーション
	RECT rect1[] = {
		{  0,  0, 16, 16},// ←
		{ 16,  0, 32, 16},// ↑
		{ 32,  0, 48, 16},// →
		{ 48,  0, 64, 16},// ↓
	};
	RECT rect2[] = {
		{  0, 16, 16, 32},// ←
		{ 16, 16, 32, 32},// ↑
		{ 32, 16, 48, 32},// →
		{ 48, 16, 64, 32},// ↓
	};
	RECT rect3[] = {
		{  0, 32, 16, 48},// ←
		{ 16, 32, 32, 48},// ↑
		{ 32, 32, 48, 48},// →
		{ 48, 32, 64, 48},// ↓
	};
	switch( level ){
	case 1:bul->rect = rect1[bul->direct]; break;
	case 2:bul->rect = rect2[bul->direct]; break;
	case 3:bul->rect = rect3[bul->direct]; break;
	}
}


// ボム
void SetDestroyNpCharUp( long x, long y, long w, long num );
void ActBullet_Bom( BULLET *bul, long level )
{
	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		switch( level ){
		case 1:
			bul->act_wait = 10;
			break;
		case 2:
			bul->act_wait = 15;
			break;
		case 3:
			bul->act_wait =  5;
		}
		PlaySoundObject( WAVE_BOM, 1 );
	case 1:
		switch( level ){
		case 1:
			if( !(bul->act_wait%3) )
				SetDestroyNpCharUp( bul->x + Random( -16, 16 )*VS,
					              bul->y + Random( -16, 16 )*VS, bul->enemyXL, 2 );
			break;
		case 2:
			if( !(bul->act_wait%3) )
				SetDestroyNpCharUp( bul->x + Random( -32, 32 )*VS,
					              bul->y + Random( -32, 32 )*VS, bul->enemyXL, 2 );
			break;
		case 3:
			if( !(bul->act_wait%3) )
				SetDestroyNpCharUp( bul->x + Random( -40, 40 )*VS,
					              bul->y + Random( -40, 40 )*VS, bul->enemyXL, 2 );
			break;
		}
//		if( !(bul->act_wait%5) ) PlaySoundObject( WAVE_BOM, 1 );
		bul->act_wait--;
		if( bul->act_wait < 0 )bul->cond = 0;
		break;
	}
}


//バブリン１号
void ActBullet_Bubblin1( BULLET *bul )
{
	if( bul->flag & (FLAG_HIT_BLOCK | FLAG_HIT_X) ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_SMOKE, DIR_LEFT );
		return;
	}

	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -3*VS; break;
		case DIR_RIGHT: bul->xm =  3*VS; break;
		case DIR_UP:    bul->ym = -3*VS; break;
		case DIR_DOWN:  bul->ym =  3*VS; break;
		}
	case 1:
		break;
	}

	switch( bul->direct ){
	case DIR_LEFT:  bul->xm += VS/12; break;
	case DIR_RIGHT: bul->xm -= VS/12; break;
	case DIR_UP:    bul->ym += VS/12; break;
	case DIR_DOWN:  bul->ym -= VS/12; break;
	}




	bul->x += bul->xm;
	bul->y += bul->ym;


	bul->act_wait++;
	if( bul->act_wait > 40 ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_BUBBLIN, DIR_LEFT );
	}

	// アニメ
	RECT rect[] = {
		{192,  0, 200,  8},
		{200,  0, 208,  8},
		{208,  0, 216,  8},
		{216,  0, 224,  8},
	};
	bul->ani_wait++;
	if( bul->ani_wait > 3 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 3 )bul->ani_no = 3;
	bul->rect = rect[bul->ani_no];

}

//バブリン２号
void ActBullet_Bubblin2( BULLET *bul )
{
	BOOL bDelete;

	bDelete = FALSE;
	if( bul->direct == DIR_LEFT  && bul->flag & FLAG_HIT_LEFT   ) bDelete = TRUE;
	if( bul->direct == DIR_RIGHT && bul->flag & FLAG_HIT_RIGHT  ) bDelete = TRUE;
	if( bul->direct == DIR_UP    && bul->flag & FLAG_HIT_TOP    ) bDelete = TRUE;
	if( bul->direct == DIR_DOWN  && bul->flag & FLAG_HIT_BOTTOM ) bDelete = TRUE;

	if( bDelete ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_SMOKE, DIR_LEFT );
		return;
	}

	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -3*VS; bul->ym = Random( -VS/2, VS/2 ); break;
		case DIR_RIGHT: bul->xm =  3*VS; bul->ym = Random( -VS/2, VS/2 ); break;
		case DIR_UP:    bul->ym = -3*VS; bul->xm = Random( -VS/2, VS/2 ); break;
		case DIR_DOWN:  bul->ym =  3*VS; bul->xm = Random( -VS/2, VS/2 ); break;
		}
	case 1:
		break;
	}

	switch( bul->direct ){
	case DIR_LEFT:  bul->xm += VS/32; break;
	case DIR_RIGHT: bul->xm -= VS/32; break;
	case DIR_UP:    bul->ym += VS/32; break;
	case DIR_DOWN:  bul->ym -= VS/32; break;
	}

	bul->x += bul->xm;
	bul->y += bul->ym;


	bul->act_wait++;
	if( bul->act_wait > 60 ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_BUBBLIN, DIR_LEFT );
	}

	// アニメ
	RECT rect[] = {
		{192,  8, 200, 16},
		{200,  8, 208, 16},
		{208,  8, 216, 16},
		{216,  8, 224, 16},
	};
	bul->ani_wait++;
	if( bul->ani_wait > 3 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 3 )bul->ani_no = 3;
	bul->rect = rect[bul->ani_no];

}

//バブリン３号
void ActBullet_Bubblin3( BULLET *bul )
{

	bul->act_wait++;
	if( bul->act_wait > 100 ||
		!(gKey & gKeyShot) ){

		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_SMOKE, DIR_LEFT );
		PlaySoundObject( WAVE_SPINE, 1 );
		if(      gMC.up   ) SetBullet( BULLET_SPINE, bul->x, bul->y, DIR_UP );
		else if( gMC.down ) SetBullet( BULLET_SPINE, bul->x, bul->y, DIR_DOWN );
		else                SetBullet( BULLET_SPINE, bul->x, bul->y, gMC.direct );
		return;
	}

	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = Random( -VS*2, -VS*1 ); bul->ym = Random(   -4, 4   )*VS/2; break;
		case DIR_RIGHT: bul->xm = Random(  VS*1,  VS*2 ); bul->ym = Random(   -4, 4   )*VS/2; break;
		case DIR_UP:    bul->ym = Random( -VS*2, -VS*1 ); bul->xm = Random(   -4, 4   )*VS/2; break;
		case DIR_DOWN:  bul->ym = Random(  VS/4,  VS/2 ); bul->xm = Random(   -4, 4   )*VS/2; break;
		}
	case 1:
		break;
	}


	if( bul->x < gMC.x ) bul->xm += VS/16;
	if( bul->x > gMC.x ) bul->xm -= VS/16;
	if( bul->y < gMC.y ) bul->ym += VS/16;
	if( bul->y > gMC.y ) bul->ym -= VS/16;

	if( bul->xm < 0 && bul->flag & FLAG_HIT_LEFT   ) bul->xm =  VS*2;
	if( bul->xm > 0 && bul->flag & FLAG_HIT_RIGHT  ) bul->xm = -VS*2;
	if( bul->ym < 0 && bul->flag & FLAG_HIT_TOP    ) bul->ym =  VS*2;
	if( bul->ym > 0 && bul->flag & FLAG_HIT_BOTTOM ) bul->ym = -VS*2;

	bul->x += bul->xm;
	bul->y += bul->ym;



	// アニメ
	RECT rect[] = {
		{240, 16, 248, 24},
		{248, 16, 256, 24},
		{240, 24, 248, 32},
		{248, 24, 256, 32},
	};
	bul->ani_wait++;
	if( bul->ani_wait > 3 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 3 )bul->ani_no = 3;
	bul->rect = rect[bul->ani_no];

}



//スピン
void ActBullet_Spine( BULLET *bul )
{

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ||
		bul->flag & FLAG_HIT_BOTTOM ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}
	//start
	if( !bul->act_no ){
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:
			bul->xm = -Random(5*2,8*2)*VS/2;
			break;
		case DIR_UP:
			bul->ym = -Random(5*2,8*2)*VS/2;
			break;
		case DIR_RIGHT:
			bul->xm =  Random(5*2,8*2)*VS/2;
			break;
		case DIR_DOWN:
			bul->ym =  Random(5*2,8*2)*VS/2;
			break;
		}
	}else{
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	// アニメーション
	bul->ani_wait++;
	if( bul->ani_wait > 1 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 1 )
		bul->ani_no = 0;


	RECT rcLeft[] = {
		{224,  0,232,  8},
		{232,  0,240,  8},
	};
	RECT rcRight[] = {
		{224,  0,232,  8},
		{232,  0,240,  8},
	};
	RECT rcDown[] = {
		{224,  8,232, 16},
		{232,  8,240, 16},
	};

	switch( bul->direct ){
	case DIR_LEFT:  bul->rect = rcLeft[ bul->ani_no]; break;
	case DIR_UP:    bul->rect = rcDown[ bul->ani_no]; break;
	case DIR_RIGHT: bul->rect = rcRight[bul->ani_no]; break;
	case DIR_DOWN:  bul->rect = rcDown[ bul->ani_no]; break;
	}

}



//ブレード 1
void ActBullet_Sword1( BULLET *bul )
{

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}
	if( bul->count1 == 3 ) bul->bbits &= ~BBITS_THROWMAP;
	if( bul->count1%5 == 1 ) PlaySoundObject( WAVE_FIRE, 1 );
	//start
	if( !bul->act_no ){
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:
			bul->xm = -4*VS;
			break;
		case DIR_UP:
			bul->ym = -4*VS; 
			break;
		case DIR_RIGHT:
			bul->xm =  4*VS;
			break;
		case DIR_DOWN:
			bul->ym =  4*VS; 
			break;
		}
	}else{
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	//アニメーション
	RECT rcLeft[] = {
		{  0, 48, 16, 64},
		{ 16, 48, 32, 64},
		{ 32, 48, 48, 64},
		{ 48, 48, 64, 64},
	};
	RECT rcRight[] = {
		{ 64, 48, 80, 64},
		{ 80, 48, 96, 64},
		{ 96, 48,112, 64},
		{112, 48,128, 64},
	};

	bul->ani_wait++;
	if( bul->ani_wait > 1 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 3 )
		bul->ani_no = 0;

	if( bul->direct == DIR_LEFT ) bul->rect = rcLeft[bul->ani_no];
	else                          bul->rect = rcRight[bul->ani_no];

}

//ブレード 2
void ActBullet_Sword2( BULLET *bul )
{

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}
	if( bul->count1 == 3 ) bul->bbits &= ~BBITS_THROWMAP;
	if( bul->count1%7 == 1 ) PlaySoundObject( WAVE_SWORD, 1 );
	//start
	if( !bul->act_no ){
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:
			bul->xm = -4*VS;
			break;
		case DIR_UP:
			bul->ym = -4*VS; 
			break;
		case DIR_RIGHT:
			bul->xm =  4*VS;
			break;
		case DIR_DOWN:
			bul->ym =  4*VS; 
			break;
		}
	}else{
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	//アニメーション
	RECT rcLeft[] = {
		{160, 48,184, 72},
		{184, 48,208, 72},
		{208, 48,232, 72},
		{232, 48,256, 72},
	};
	RECT rcRight[] = {
		{160, 72,184, 96},
		{184, 72,208, 96},
		{208, 72,232, 96},
		{232, 72,256, 96},
	};

	bul->ani_wait++;
	if( bul->ani_wait > 1 ){
		bul->ani_wait = 0;
		bul->ani_no++;
	}
	if( bul->ani_no > 3 )
		bul->ani_no = 0;


	if( bul->direct == DIR_LEFT ) bul->rect = rcLeft[bul->ani_no];
	else                          bul->rect = rcRight[bul->ani_no];

}

void ActBullet_Sword3( BULLET *bul )
{
	RECT rcLeft[] = {
		{272,  0,296, 24},
		{296,  0,320, 24},
	};
	RECT rcUp[] = {
		{272, 48,296, 72},
		{296,  0,320, 24},
	};
	RECT rcRight[] = {
		{272, 24,296, 48},
		{296, 24,320, 48},
	};
	RECT rcDown[] = {
		{296, 48,320, 72},
		{296, 24,320, 48},
	};


	//start
	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		bul->xm = 0;
		bul->ym = 0;
	case 1:
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -VS*4; break;
		case DIR_UP:    bul->ym = -VS*4; break;
		case DIR_RIGHT:	bul->xm = +VS*4; break;
		case DIR_DOWN:  bul->ym = +VS*4; break;
		}
		if( bul->life != 100 ){
			bul->act_no  = 2;
			bul->ani_no  = 1;
			bul->damage  = -1;
			bul->act_wait = 0;
		}
		bul->act_wait++;
		if( bul->act_wait%4 == 1 ){

			PlaySoundObject( WAVE_SWORD, 1 );
			bul->count1++;
			if( bul->count1%2 ) SetBullet( BULLET_EDGE, bul->x , bul->y, DIR_LEFT );
			else                SetBullet( BULLET_EDGE, bul->x , bul->y, DIR_RIGHT );
		}
		//時間消滅
		bul->count1++;
		if( bul->count1 == 5 ) bul->bbits &= ~BBITS_THROWMAP;
		if( bul->count1 > bul->life_count ){
			bul->cond = 0;
			SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
			return;
		}
		break;
	case 2:
		bul->xm = 0;
		bul->ym = 0;
		bul->act_wait++;

		if( 0 == Random( -1, 1 )  ){
			PlaySoundObject( WAVE_SWORD, 1 );
			if( Random( 0, 1 )%2 ) SetBullet( BULLET_EDGE, bul->x + Random(-64,64)*VS, bul->y+ Random(-64,64)*VS, DIR_LEFT );
			else                   SetBullet( BULLET_EDGE, bul->x + Random(-64,64)*VS, bul->y+ Random(-64,64)*VS, DIR_RIGHT );
		}
		if( bul->act_wait > 50 ){
			bul->cond = 0;
		}

		break;
	}
	//移動
	bul->x += bul->xm;
	bul->y += bul->ym;
	switch( bul->direct ){
	case DIR_LEFT:  bul->rect = rcLeft[bul->ani_no];  break;
	case DIR_UP:    bul->rect = rcUp[bul->ani_no];    break;
	case DIR_RIGHT: bul->rect = rcRight[bul->ani_no]; break;
	case DIR_DOWN:  bul->rect = rcDown[bul->ani_no]; break;
	}

	if( bul->act_wait%2 ){
		bul->rect.right = 0;
	}

}

// エッジ
void ActBullet_Edge( BULLET *bul )
{
	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		bul->y -= 12*VS;
		if( bul->direct == DIR_LEFT ) bul->x += 16*VS;
		else                          bul->x -= 16*VS;
	case 1:
		bul->ani_wait++;
		if( bul->ani_wait > 2 ){
			bul->ani_wait = 0;
			bul->ani_no++;
		}
		if( bul->direct == DIR_LEFT ) bul->x -= 2*VS;
		else                          bul->x += 2*VS;
		bul->y += 2*VS;

		if( bul->ani_no == 1 ) bul->damage = 2;
		else                   bul->damage = 1;

		if( bul->ani_no > 4 ){
			bul->cond = 0;
		}
		break;

	}


	RECT rcLeft[] = {
		{  0, 64, 24, 88},
		{ 24, 64, 48, 88},
		{ 48, 64, 72, 88},
		{ 72, 64, 96, 88},
		{ 96, 64,120, 88},
	};
	RECT rcRight[] = {
		{  0, 88, 24, 112},
		{ 24, 88, 48, 112},
		{ 48, 88, 72, 112},
		{ 72, 88, 96, 112},
		{ 96, 88,120, 112},
	};

	if( bul->direct == DIR_LEFT ) bul->rect = rcLeft[ bul->ani_no];
	else                          bul->rect = rcRight[bul->ani_no];
}

// 落石
void ActBullet_Drop( BULLET *bul )
{
	RECT rc = {0,0,0,0};
	bul->act_wait++;
	if( bul->act_wait > 2 ){
		bul->cond = 0;
	}
	bul->rect = rc;
}







// スーパーミサイル
void ActBullet_SuperMissile( BULLET *bul, long level )
{
	BOOL bHit;

	static unsigned long inc;

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}

	bHit = FALSE;

	if( bul->life != 10                                         ) bHit = TRUE;
	if( bul->direct == DIR_LEFT  && bul->flag & FLAG_HIT_LEFT   ) bHit = TRUE;
	if( bul->direct == DIR_RIGHT && bul->flag & FLAG_HIT_RIGHT  ) bHit = TRUE;
	if( bul->direct == DIR_UP    && bul->flag & FLAG_HIT_TOP    ) bHit = TRUE;
	if( bul->direct == DIR_DOWN  && bul->flag & FLAG_HIT_BOTTOM ) bHit = TRUE;

	if( bul->direct == DIR_LEFT  && bul->flag & FLAG_HIT_RUP    ) bHit = TRUE;
	if( bul->direct == DIR_LEFT  && bul->flag & FLAG_HIT_RDOWN  ) bHit = TRUE;
	if( bul->direct == DIR_RIGHT && bul->flag & FLAG_HIT_LUP    ) bHit = TRUE;
	if( bul->direct == DIR_RIGHT && bul->flag & FLAG_HIT_LDOWN  ) bHit = TRUE;

	if( bHit ){
		SetBullet( BULLET_SUPERBOM1 + level-1, bul->x, bul->y, DIR_LEFT );
		bul->cond = 0;
	}

	//start
	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:
		case DIR_RIGHT:
			bul->tgt_y = bul->y;
			bul->enemyXL = 8*VS;
			bul->blockXL = 8*VS;
			break;
		case DIR_UP:
		case DIR_DOWN:
			bul->tgt_x = bul->x;
			bul->enemyYL = 8*VS;
			bul->blockYL = 8*VS;
			break;
		}
		if( level == 3 ){
			switch( bul->direct ){
			case DIR_LEFT:
			case DIR_RIGHT:
				if( bul->y > gMC.y ) bul->ym = +VS/2;
				else                 bul->ym = -VS/2;
				bul->xm =  Random(-VS, VS );
				break;
			case DIR_UP:
			case DIR_DOWN:
				if( bul->x > gMC.x ) bul->xm = +VS/2;
				else                 bul->xm = -VS/2;
				bul->ym =  Random(-VS, VS );
				break;
			}
			inc++;
			switch( inc%3 ){
			case 0: bul->ani_no = VS;  break;
			case 1: bul->ani_no = VS/2;  break;
			case 2: bul->ani_no = VS/3; break;
			}

		}else{
			bul->ani_no = VS/1;
		}
	case 1:
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm += -bul->ani_no; break;
		case DIR_UP:    bul->ym += -bul->ani_no; break;
		case DIR_RIGHT: bul->xm +=  bul->ani_no; break;
		case DIR_DOWN:  bul->ym +=  bul->ani_no; break;
		}

		if( level == 3 ){
			switch( bul->direct ){
			case DIR_LEFT:
			case DIR_RIGHT:
				if( bul->y < bul->tgt_y ) bul->ym += VS/8;
				else                      bul->ym -= VS/8;
				break;
			case DIR_UP:
			case DIR_DOWN:
				if( bul->x < bul->tgt_x ) bul->xm += VS/8;
				else                      bul->xm -= VS/8;
				break;
			}
		}

		if( bul->xm < -VS*10 ) bul->xm = -VS*10;
		if( bul->xm >  VS*10 ) bul->xm =  VS*10;
		if( bul->ym < -VS*10 ) bul->ym = -VS*10;
		if( bul->ym >  VS*10 ) bul->ym =  VS*10;
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
		break;
	}
	bul->count2++;
	if(bul->count2 > 2 ){
		bul->count2 = 0;
		switch( bul->direct ){
		case 0: SetCaret( bul->x+8*VS, bul->y, CARET_MISSILE, DIR_RIGHT ); break;
		case 1: SetCaret( bul->x, bul->y+8*VS, CARET_MISSILE, DIR_DOWN  ); break;
		case 2: SetCaret( bul->x-8*VS, bul->y, CARET_MISSILE, DIR_LEFT  ); break;
		case 3: SetCaret( bul->x, bul->y-8*VS, CARET_MISSILE, DIR_UP    ); break;
		}
	}

	//アニメーション
	RECT rect1[] = {
		{120, 96,136,112},// ←
		{136, 96,152,112},// ↑
		{152, 96,168,112},// →
		{168, 96,184,112},// ↓
	};
	RECT rect2[] = {
		{184, 96,200,112},// ←
		{200, 96,216,112},// ↑
		{216, 96,232,112},// →
		{232, 96,248,112},// ↓
	};
	switch( level ){
	case 1: bul->rect = rect1[bul->direct]; break;
	case 2: bul->rect = rect2[bul->direct]; break;
	case 3: bul->rect = rect1[bul->direct]; break;
	}
}


// スーパーボム
void ActBullet_SuperBom( BULLET *bul, long level )
{
	switch( bul->act_no ){
	case 0:
		bul->act_no = 1;
		switch( level ){
		case 1:
			bul->act_wait = 10;
			break;
		case 2:
			bul->act_wait = 14;
			break;
		case 3:
			bul->act_wait =  6;
		}
		PlaySoundObject( WAVE_BOM, 1 );
	case 1:
		switch( level ){
		case 1:
			if( !(bul->act_wait%3) )
				SetDestroyNpCharUp( bul->x + Random( -16, 16 )*VS,
					              bul->y + Random( -16, 16 )*VS, bul->enemyXL, 2 );
			break;
		case 2:
			if( !(bul->act_wait%3) )
				SetDestroyNpCharUp( bul->x + Random( -32, 32 )*VS,
					              bul->y + Random( -32, 32 )*VS, bul->enemyXL, 2 );
			break;
		case 3:
			if( !(bul->act_wait%3) )
				SetDestroyNpCharUp( bul->x + Random( -40, 40 )*VS,
					              bul->y + Random( -40, 40 )*VS, bul->enemyXL, 2 );
			break;
		}
//		if( !(bul->act_wait%5) ) PlaySoundObject( WAVE_BOM, 1 );
		bul->act_wait--;
		if( bul->act_wait < 0 )bul->cond = 0;
		break;
	}
}


// ネメシス
void ActBullet_Nemesis( BULLET *bul, long level )
{

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}
	//start
	if( !bul->act_no ){
		bul->act_no = 1;
		bul->count1 = 0;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -8*VS; break;
		case DIR_UP:    bul->ym = -8*VS; break;
		case DIR_RIGHT: bul->xm =  8*VS; break;
		case DIR_DOWN:  bul->ym =  8*VS; break;
		}
		switch( level ){
		case 1: break;
		case 2: break;
		case 3:
			bul->xm = bul->xm / 3;
			bul->ym = bul->ym / 3;
			break;
		}
	}else{
		
		if( level == 1 && bul->count1 % 4 == 1 ){
			switch( bul->direct ){
			case DIR_LEFT:  SetNpChar( 4, bul->x, bul->y, -VS, Random( -VS, VS ), DIR_RIGHT, NULL, MAX_NPC/2 ); break;
			case DIR_UP:    SetNpChar( 4, bul->x, bul->y, Random( -VS, VS ), -VS, DIR_RIGHT, NULL, MAX_NPC/2 ); break;
			case DIR_RIGHT: SetNpChar( 4, bul->x, bul->y, +VS, Random( -VS, VS ), DIR_RIGHT, NULL, MAX_NPC/2 ); break;
			case DIR_DOWN:  SetNpChar( 4, bul->x, bul->y,  Random( -VS, VS ), VS, DIR_RIGHT, NULL, MAX_NPC/2 ); break;
			}
		}
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}
	
	
	bul->ani_no++;
	if( bul->ani_no > 1 )
		bul->ani_no = 0;

	//アニメーション
	RECT rcL[] = { {  0,112, 32,128}, {  0,128, 32,144}, };
	RECT rcU[] = { { 32,112, 48,144}, { 48,112, 64,144}, };
	RECT rcR[] = { { 64,112, 96,128}, { 64,128, 96,144}, };
	RECT rcD[] = { { 96,112,112,144}, {112,112,128,144}, };

	switch( bul->direct ){
	case DIR_LEFT:  bul->rect = rcL[bul->ani_no]; break;
	case DIR_UP:    bul->rect = rcU[bul->ani_no]; break;
	case DIR_RIGHT: bul->rect = rcR[bul->ani_no]; break;
	case DIR_DOWN:  bul->rect = rcD[bul->ani_no]; break;
	}
	bul->rect.top    +=  32 * ((level-1)/2);
	bul->rect.bottom +=  32 * ((level-1)/2);
	bul->rect.left   += 128 * ((level-1)%2);
	bul->rect.right  += 128 * ((level-1)%2);
}




//シュプール
void ActBullet_Spur( BULLET *bul, long level )
{

	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		SetCaret( bul->x, bul->y, CARET_FLASH, 0 );
		return;
	}
	
	if( bul->damage && bul->life != 100 ) bul->damage = 0;
	//start
	if( !bul->act_no ){
		bul->act_no = 1;
		switch( bul->direct ){
		case DIR_LEFT:  bul->xm = -8*VS; break;
		case DIR_UP:    bul->ym = -8*VS; break;
		case DIR_RIGHT: bul->xm =  8*VS; break;
		case DIR_DOWN:  bul->ym =  8*VS; break;
		}

		switch( level ){
		case 1:
			switch( bul->direct ){
			case DIR_LEFT:  bul->enemyYL = 2*VS; break;
			case DIR_UP:    bul->enemyXL = 2*VS; break;
			case DIR_RIGHT: bul->enemyYL = 2*VS; break;
			case DIR_DOWN:  bul->enemyXL = 2*VS; break;
			}
			break;
		case 2:
			switch( bul->direct ){
			case DIR_LEFT:  bul->enemyYL = 4*VS; break;
			case DIR_UP:    bul->enemyXL = 4*VS; break;
			case DIR_RIGHT: bul->enemyYL = 4*VS; break;
			case DIR_DOWN:  bul->enemyXL = 4*VS; break;
			}
			break;
		}
	}else{
		//移動
		bul->x += bul->xm;
		bul->y += bul->ym;
	}

	//アニメーション
	RECT rect1[] = {
		{128, 32, 144, 48},
		{144, 32, 160, 48},
	};
	RECT rect2[] = {
		{160, 32, 176, 48},
		{176, 32, 192, 48},
	};
	RECT rect3[] = {
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};
	bul->damage = bul->life;
	
	switch( level ){
	case 1:
		if( bul->direct == DIR_UP || bul->direct == DIR_DOWN ) bul->rect = rect1[1];
		else                                                   bul->rect = rect1[0];
		break;
	case 2:
		if( bul->direct == DIR_UP || bul->direct == DIR_DOWN ) bul->rect = rect2[1];
		else                                                   bul->rect = rect2[0];
		break;
	case 3:
		if( bul->direct == DIR_UP || bul->direct == DIR_DOWN ) bul->rect = rect3[1];
		else                                                   bul->rect = rect3[0];
		break;
	}
	SetBullet( BULLET_SPURTAIL1 + level-1, bul->x, bul->y, bul->direct );
}

void ActBullet_SpurTail( BULLET *bul, long level )
{
	bul->count1++;
	if( bul->count1 > 20 ){
		bul->ani_no = bul->count1 - 20;
	}
	if( bul->ani_no > 2 ) bul->cond = 0;
	
	if( bul->damage && bul->life != 100 ) bul->damage = 0;
	
	RECT rc_h_lv1[] = { {192, 32,200, 40}, {200, 32,208, 40}, {208, 32,216, 40}, };
	RECT rc_v_lv1[] = { {192, 40,200, 48}, {200, 40,208, 48}, {208, 40,216, 48}, };
	RECT rc_h_lv2[] = { {216, 32,224, 40}, {224, 32,232, 40}, {232, 32,240, 40}, };
	RECT rc_v_lv2[] = { {216, 40,224, 48}, {224, 40,232, 48}, {232, 40,240, 48}, };
	RECT rc_h_lv3[] = { {240, 32,248, 40}, {248, 32,256, 40}, {256, 32,264, 40}, };
	RECT rc_v_lv3[] = { {240, 32,248, 40}, {248, 32,256, 40}, {256, 32,264, 40}, };
	switch( level ){
	case 1:
		if( bul->direct == DIR_LEFT || bul->direct == DIR_RIGHT ) bul->rect = rc_h_lv1[bul->ani_no];
		else                                                      bul->rect = rc_v_lv1[bul->ani_no];
		break;
	case 2:
		if( bul->direct == DIR_LEFT || bul->direct == DIR_RIGHT ) bul->rect = rc_h_lv2[bul->ani_no];
		else                                                      bul->rect = rc_v_lv2[bul->ani_no];
		break;
	case 3:
		if( bul->direct == DIR_LEFT || bul->direct == DIR_RIGHT ) bul->rect = rc_h_lv3[bul->ani_no];
		else                                                      bul->rect = rc_v_lv3[bul->ani_no];
		break;
	}
}

void ActBullet_EnemyClear( BULLET *bul )
{
	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		return;
	}
	bul->damage = 10000;
	bul->enemyXL = VS*PARTSSIZE*100;
	bul->enemyYL = VS*PARTSSIZE*100;
}


void ActBullet_Star( BULLET *bul )
{
	//時間消滅
	bul->count1++;
	if( bul->count1 > bul->life_count ){
		bul->cond = 0;
		return;
	}
}

void ActBullet( void )
{
	long i;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE ){
			if( gBul[i].life < 1 ){
				gBul[i].cond = 0;
				continue;
			}
			switch( gBul[i].code_bullet ){
			// スネーク
			case BULLET_FRONTIA1:       ActBullet_Frontia1( &gBul[i]    );     break;
			case BULLET_FRONTIA2:       ActBullet_Frontia2( &gBul[i], 2 );     break;
			case BULLET_FRONTIA3:       ActBullet_Frontia2( &gBul[i], 3 );     break;
			// ポーラスター
			case BULLET_POLESTAR1:      ActBullet_PoleStar( &gBul[i], 1 );     break;
			case BULLET_POLESTAR2:      ActBullet_PoleStar( &gBul[i], 2 );     break;
			case BULLET_POLESTAR3:      ActBullet_PoleStar( &gBul[i], 3 );     break;
			// ファイアボール
			case BULLET_FIREBALL1:      ActBullet_FireBall( &gBul[i], 1 );     break;
			case BULLET_FIREBALL2:      ActBullet_FireBall( &gBul[i], 2 );     break;
			case BULLET_FIREBALL3:      ActBullet_FireBall( &gBul[i], 3 );     break;
			// マシンガン
			case BULLET_MACHINEGUN1:    ActBullet_MachineGun( &gBul[i], 1 );   break;
			case BULLET_MACHINEGUN2:    ActBullet_MachineGun( &gBul[i], 2 );   break;
			case BULLET_MACHINEGUN3:    ActBullet_MachineGun( &gBul[i], 3 );   break;
			// ミサイル
			case BULLET_MISSILE1:       ActBullet_Missile( &gBul[i], 1 );      break;
			case BULLET_MISSILE2:       ActBullet_Missile( &gBul[i], 2 );      break;
			case BULLET_MISSILE3:       ActBullet_Missile( &gBul[i], 3 );      break;
			// ボム
			case BULLET_BOM1:           ActBullet_Bom( &gBul[i], 1 );          break;
			case BULLET_BOM2:           ActBullet_Bom( &gBul[i], 2 );          break;
			case BULLET_BOM3:           ActBullet_Bom( &gBul[i], 3 );          break;
			// バブリン
			case BULLET_BUBBLIN1:       ActBullet_Bubblin1( &gBul[i] );        break;
			case BULLET_BUBBLIN2:       ActBullet_Bubblin2( &gBul[i] );        break;
			case BULLET_BUBBLIN3:       ActBullet_Bubblin3( &gBul[i] );        break;
			// スピン etc
			case BULLET_SPINE:          ActBullet_Spine(    &gBul[i] );        break;
			case BULLET_EDGE:           ActBullet_Edge(     &gBul[i] );        break;
			case BULLET_DROP:           ActBullet_Drop(     &gBul[i] );        break;
			// ブレード
			case BULLET_SWORD1:	        ActBullet_Sword1(   &gBul[i] );        break;
			case BULLET_SWORD2:	        ActBullet_Sword2(   &gBul[i] );        break;
			case BULLET_SWORD3:	        ActBullet_Sword3(   &gBul[i] );        break;
			// スーパーミサイル
			case  BULLET_SUPERMISSILE1: ActBullet_SuperMissile( &gBul[i], 1 ); break;
			case  BULLET_SUPERMISSILE2: ActBullet_SuperMissile( &gBul[i], 2 ); break;
			case  BULLET_SUPERMISSILE3: ActBullet_SuperMissile( &gBul[i], 3 ); break;
			// スーパーボム
			case  BULLET_SUPERBOM1:     ActBullet_SuperBom( &gBul[i], 1 );     break;
			case  BULLET_SUPERBOM2:     ActBullet_SuperBom( &gBul[i], 2 );     break;
			case  BULLET_SUPERBOM3:     ActBullet_SuperBom( &gBul[i], 3 );     break;
			// ネメシス
			case  BULLET_NEMESIS1:      ActBullet_Nemesis(  &gBul[i], 1 );     break;
			case  BULLET_NEMESIS2:      ActBullet_Nemesis(  &gBul[i], 2 );     break;
			case  BULLET_NEMESIS3:      ActBullet_Nemesis(  &gBul[i], 3 );     break;
			// シュプール
			case BULLET_SPUR1:          ActBullet_Spur(     &gBul[i], 1 );     break;
			case BULLET_SPUR2:          ActBullet_Spur(     &gBul[i], 2 );     break;
			case BULLET_SPUR3:          ActBullet_Spur(     &gBul[i], 3 );     break;
			// シュプールテイル
			case BULLET_SPURTAIL1:      ActBullet_SpurTail( &gBul[i], 1 );     break;
			case BULLET_SPURTAIL2:      ActBullet_SpurTail( &gBul[i], 2 );     break;
			case BULLET_SPURTAIL3:      ActBullet_SpurTail( &gBul[i], 3 );     break;
			// カーリーネメシス etc
			case BULLET_NEMESIS_C:  ActBullet_Nemesis(    &gBul[i], 1   );     break;
			case BULLET_ENEMYCLEAR: ActBullet_EnemyClear( &gBul[i]      );     break;
			case BULLET_STAR:       ActBullet_Star(       &gBul[i]      );     break;

			}
		}
	}
}


BOOL IsActiveSomeBullet( void )
{
	long i;

	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE ){
			switch( gBul[i].code_bullet ){
			case BULLET_MISSILE1:
			case BULLET_MISSILE2:
			case BULLET_MISSILE3:
			case BULLET_BOM1:
			case BULLET_BOM2:
			case BULLET_BOM3:
			case BULLET_SUPERMISSILE1:
			case BULLET_SUPERMISSILE2:
			case BULLET_SUPERMISSILE3:
			case  BULLET_SUPERBOM1:
			case  BULLET_SUPERBOM2:
			case  BULLET_SUPERBOM3:
			case BULLET_EDGE:
			case BULLET_SWORD1:
			case BULLET_SWORD2:
			case BULLET_SWORD3:
				return TRUE;
			
			}
		}
	}
	return FALSE;
}
/*
BOOL IsActiveEdgeBullet( void )
{
	long i;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE && gBul[i].code_bullet == BULLET_EDGE )
			return TRUE;
	}
	return FALSE;
}
*/

/*
void SwordDamage1( void )
{
	long i;
	for( i = 0; i < MAX_BULLET; i++ ){
		if( gBul[i].cond & COND_ALIVE ){
			switch( gBul[i].code_bullet ){
			case BULLET_SWORD1:
			case BULLET_SWORD2:
			case BULLET_SWORD3:
			case BULLET_EDGE:
			case BULLET_SUPERBOM1:
			case BULLET_SUPERBOM2:
			case BULLET_SUPERBOM3:
				gBul[i].damage = 0;
				break;
			}
		}
	}
}
*/