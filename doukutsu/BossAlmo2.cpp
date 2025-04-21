#include <windows.h>
#include "Sound.h"
#include "Draw.h"
#include "Define.h"

#include "Tags.h"

extern MYCHAR gMC;

extern MAPDATA gMap;

extern NPCHAR gBoss[];

long Random( int min, int max );
void SetNpChar( long code_char, long x, long y, long xm, long ym,
				long dir, NPCHAR *npc, long start_index );
void SetCaret( long x, long y, long code, long dir );
void SetQuake( long time );
void ChangeAllNpCharByCode( long code1, long code2 );

int GetSin( unsigned char deg );
int GetCos( unsigned char deg );
unsigned char GetArktan( long x,long y );

void SetDestroyNpChar( long x, long y, long w, long num );
void DeleteNpCharCode( long code, BOOL bSmoke );

void SetFlash( long x, long y, long mode );

void SetNoise( long freq );
void CutNoise( void );

extern long gCurlyShoot_wait;
extern long gCurlyShoot_x;
extern long gCurlyShoot_y;

extern long gSuperYpos;


// gBoss[0].ani_no:1以上なら左移動でも開閉。
// 0 メイン
// 1 サブ
// 2 サブ

// 3 顔
// 4 頭
// 5 尻

// 6 サブ
// 7 サブ

//  8 甲羅
//  9 甲羅
// 10 甲羅
// 11 コア

// 顔
static void _ActBossCharA_Head( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,  0, 72,112},
		{  0,112, 72,224},// ダメージ
		{160,  0,232,112},// 閉じ
		{  0,  0,  0,  0},
	};

	switch( npc->act_no ){
	case 10:
		npc->act_no     = 11;
		npc->ani_no     =  2;
		npc->bits       = BITS_THROW_BLOCK;
		npc->view.front = 36*VS;
		npc->view.top   = 56*VS;
	case 11:
		npc->x          = gBoss[0].x - VS*36;
		npc->y          = gBoss[0].y;
		break;

	// フェードアウト
	case 50:
		npc->act_no   =  51;
		npc->act_wait = 112;
	case 51:
		npc->act_wait--;
		if( !npc->act_wait ){
			npc->act_no = 100;
			npc->ani_no = 3;
		}
		break;
	case 100:
		npc->ani_no = 3;
		break;
	}

	
	npc->rect       = rect[npc->ani_no];


	if( npc->act_no == 51 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait;
	}

}

// 尻
static void _ActBossCharA_Tail( NPCHAR *npc )
{
	RECT rect[] = {
		{  72,  0, 160,112},
		{  72,112, 160,224},
		{   0,  0,   0,  0},
	};

	switch( npc->act_no ){
	case 10:
		npc->act_no     = 11;
		npc->ani_no     =  0;
		npc->bits       = BITS_THROW_BLOCK;

		npc->view.front = 44*VS;
		npc->view.top   = 56*VS;
	case 11:
		npc->x          = gBoss[0].x + VS*44;
		npc->y          = gBoss[0].y;
		break;

		// フェードアウト
	case 50:
		npc->act_no   =  51;
		npc->act_wait = 112;
	case 51:
		npc->act_wait--;
		if( !npc->act_wait ){
			npc->act_no = 100;
			npc->ani_no = 2;
		}
		break;
	case 100:
		npc->ani_no = 2;
		break;
	}

	npc->rect       = rect[npc->ani_no];
	if( npc->act_no == 51 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait;
	}

}

//顔
static void _ActBossCharA_Face( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,  0,  0,  0},
		{160,112,232,152},
		{160,152,232,192},

		{160,192,232,232},
		{248,160,320,200},
	};

	switch( npc->act_no ){
	case  0:
		npc->ani_no = 0;
		break;
	case 10:// 顔
		npc->ani_no = 1;
		break;
	case 20:// 歯
		npc->ani_no = 2;
		break;
	case 30:// 唇
		npc->act_no   =31;
		npc->ani_no   = 3;
		npc->act_wait = 50*2;
	case 31:
		npc->act_wait++;
		if( npc->act_wait > 50*6 ){
			npc->act_wait = 0;
		}
		if( npc->act_wait > 50*5 && npc->act_wait % 16 == 1 )
			PlaySoundObject( WAVE_QUAKE, 1 );
		if( npc->act_wait > 50*5 && npc->act_wait % 16 == 7 ){
			SetNpChar( 293, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/4 );
			PlaySoundObject( WAVE_THUNDER, 1 );
		}
		if( npc->act_wait == 50*4 ) PlaySoundObject( WAVE_COREPOW, 1 );
		if( npc->act_wait > 50*4 && npc->act_wait % 2 ) npc->ani_no = 4;
		else                                            npc->ani_no = 3;
		break;
	}

	npc->view.back  = 36*VS;
	npc->view.front = 36*VS;
	npc->view.top   = 20*VS;

	npc->x = gBoss[0].x - 36*VS;
	npc->y = gBoss[0].y +  4*VS;
	npc->bits       = BITS_THROW_BLOCK;


	npc->rect = rect[npc->ani_no];
}

// サブ
static void _ActBossCharA_Mini( NPCHAR *npc )
{
	RECT rect[] = {
		{256,  0,320, 40},
		{256, 40,320, 80},
		{256, 80,320,120}, //閉じ
	};
	if( !npc->cond )return;

//	long xm, ym;
//	unsigned char deg;
	long deg;

	npc->life = 1000;

	switch( npc->act_no ){
	case   0:
		npc->bits &= ~BITS_BANISH_DAMAGE;
		break;

	case  5:// 閉じ(スロー回転)
		npc->ani_no =  0;
		npc->bits   &= ~BITS_BANISH_DAMAGE;
		npc->count2 += 1;
		npc->count2 = npc->count2 % 256;
		break;

	case  10:// 閉じ
		npc->ani_no =  0;
		npc->bits   &= ~BITS_BANISH_DAMAGE;
		npc->count2 += 2;
		npc->count2 = npc->count2 % 256;
		break;

	case  20:// 開き
		npc->ani_no =  1;
		npc->bits   &= ~BITS_BANISH_DAMAGE;
		npc->count2 += 2;
		npc->count2 = npc->count2 % 256;
		break;

	case  30:// 閉じ(高速)
		npc->ani_no =  0;
		npc->bits   &= ~BITS_BANISH_DAMAGE;
		npc->count2 += 4;
		npc->count2 = npc->count2 % 256;
		break;

	case 200:// 撤退
		npc->act_no = 201;
		npc->ani_no = 2;
		npc->xm     = 0;
		npc->ym     = 0;
	case 201:
		npc->xm += VS/16;
		npc->x  += npc->xm;
		if( npc->x > gMap.width * VS* PARTSSIZE + 32*VS ){
			npc->cond = 0;
		}
		break;
	}

	if( npc->act_no  < 50 ){
			if( npc->count1 ) deg =  128 + npc->count2;
			else              deg =  384 + npc->count2;
			npc->x = npc->pNpc->x - 8*VS + GetCos( (unsigned char)(deg/2) )*48;
			npc->y = npc->pNpc->y        + GetSin( (unsigned char)(deg/2) )*80;
	}

	npc->rect = rect[npc->ani_no];

}

// 当たり判定
static void _ActBossCharA_Hit( NPCHAR *npc )
{
	switch( npc->count1 ){
	case 0:
		npc->x = gBoss[0].x +  0*VS;
		npc->y = gBoss[0].y - 32*VS;
		break;
	case 1:
		npc->x = gBoss[0].x + 28*VS;
		npc->y = gBoss[0].y +  0*VS;
		break;
	case 2:
		npc->x = gBoss[0].x + 4*VS;
		npc->y = gBoss[0].y + 32*VS;
		break;
	case 3:
		npc->x = gBoss[0].x - 28*VS;
		npc->y = gBoss[0].y +  4*VS;
		break;
	}
}

void ActBossChar_Undead( void )
{
	NPCHAR *npc;
	npc = &gBoss[0];
	static unsigned char _flash = 0;
	BOOL bShock;
	long i;
	long x,  y;

	static long _life;

	bShock = FALSE;

	switch( npc->act_no ){
	case 0:
		break;
	case 1:
		npc->act_no          = 10;
		npc->exp             = 1;
		npc->cond            = COND_ALIVE;
		npc->bits            = BITS_THROW_BLOCK|BITS_BLOCK_BULLET|BITS_VIEWDAMAGE;
//		npc->life            = 10;
		npc->life            = 700;
		npc->hit_voice       = WAVE_ALMONDDMG;
					  
		npc->x               = VS*PARTSSIZE * 37;
		npc->y               = VS*PARTSSIZE *  7 + VS*PARTSSIZE/2;
		npc->xm              = 0;
		npc->ym              = 0;

		npc->code_event      = 1000;
		npc->bits           |= BITS_EVENT_BREAK;

		// 顔と尻
		gBoss[ 3].cond       = COND_ALIVE;
		gBoss[ 3].act_no     =  0;
		gBoss[ 4].cond       = COND_ALIVE;
		gBoss[ 4].act_no     = 10;
		gBoss[ 5].cond       = COND_ALIVE;
		gBoss[ 5].act_no     = 10;

		// HIT
		gBoss[ 8].cond       = COND_ALIVE;
		gBoss[ 8].bits       = BITS_THROW_BLOCK;
		gBoss[ 8].view.front =  0*VS;
		gBoss[ 8].view.top   =  0*VS;
		gBoss[ 8].hit.back   = 40*VS;
		gBoss[ 8].hit.top    = 16*VS;
		gBoss[ 8].hit.bottom = 16*VS;
		gBoss[ 8].count1     =  0;

		gBoss[ 9]            = gBoss[8];
		gBoss[ 9].hit.back   = 36*VS;
		gBoss[ 9].hit.top    = 24*VS;
		gBoss[ 9].hit.bottom = 24*VS;
		gBoss[ 9].count1     =  1;

		gBoss[10]            = gBoss[8];
		gBoss[10].hit.back   = 44*VS;
		gBoss[10].hit.top    =  8*VS;
		gBoss[10].hit.bottom =  8*VS;
		gBoss[10].count1     =  2;

		gBoss[11]            = gBoss[8];
		gBoss[11].cond      |= COND_ZEROINDEXDAMAGE;
		gBoss[11].hit.back   = 20*VS;
		gBoss[11].hit.top    = 20*VS;
		gBoss[11].hit.bottom = 20*VS;
		gBoss[11].count1     =  3;

		// Mini
		gBoss[1].cond        = COND_ALIVE;
		gBoss[1].act_no      = 0;
		gBoss[1].bits        = BITS_THROW_BLOCK|BITS_BANISH_DAMAGE;//|BITS_BLOCK_BULLET;
		gBoss[1].life        = 1000;
		gBoss[1].hit_voice   = WAVE_NPC_GOHST;
		gBoss[1].hit.back    = 24*VS;
		gBoss[1].hit.top     = 16*VS;
		gBoss[1].hit.bottom  = 16*VS;
		gBoss[1].view.front  = 32*VS;
		gBoss[1].view.top    = 20*VS;
		gBoss[1].pNpc        = npc;

		gBoss[2]             = gBoss[1];
		gBoss[2].count2      = 128;

		gBoss[6]             = gBoss[1];
		gBoss[6].count1      =   1;

		gBoss[7]             = gBoss[1];
		gBoss[7].count1      =   1;
		gBoss[7].count2      = 128;

		_life = npc->life;

	case 10:
		break;

	case 15:// イベント開き
		npc->act_no = 16;
//		npc->act_no     = 210;
		bShock          = TRUE;
		npc->direct     = DIR_LEFT;
		gBoss[ 3].act_no =  10;
		gBoss[ 4].ani_no =   0;
	case 16:
		break;


	case 20:
		npc->act_no     = 210;
		bShock          = TRUE;
		npc->direct     = DIR_LEFT;
		gBoss[1].act_no = 5;
		gBoss[2].act_no = 5;
		gBoss[6].act_no = 5;
		gBoss[7].act_no = 5;

		break;

	case 200: // 閉じている ----------
		npc->act_no      = 201;
		npc->act_wait    =   0;
		gBoss[ 3].act_no =   0;
		gBoss[ 4].ani_no =   2;
		gBoss[ 5].ani_no =   0;
		gBoss[ 8].bits &= ~(BITS_BLOCK_BULLET);
		gBoss[ 9].bits &= ~(BITS_BLOCK_BULLET);
		gBoss[10].bits &= ~(BITS_BLOCK_BULLET);
		gBoss[11].bits &= ~(BITS_BANISH_DAMAGE);
		gSuperYpos      =    0;
		CutNoise();
			bShock      = TRUE;
	case 201:
//		npc->tgt_x = gMC.x;
//		npc->tgt_y = gMC.y;
		npc->act_wait++;
		if( npc->direct == DIR_RIGHT || npc->ani_no > 0 || npc->life < 200){
			if( npc->act_wait > 50*4 ){
				npc->count1++;
				PlaySoundObject( WAVE_COREOPEN, 1 );
				if(      npc->life < 200 ){ npc->act_no = 230; }
				else if( npc->count1 > 2 ){ npc->act_no = 220; }
				else{                       npc->act_no = 210; }
			}
		}
		break;

	case 210: // 開いている 普通----------
		npc->act_no      = 211;
		npc->act_wait    =   0;
		gBoss[ 3].act_no =  10;
		gBoss[ 8].bits  |= (BITS_BLOCK_BULLET);
		gBoss[ 9].bits  |= (BITS_BLOCK_BULLET);
		gBoss[10].bits  |= (BITS_BLOCK_BULLET);
		gBoss[11].bits  |= (BITS_BANISH_DAMAGE);
		_life = npc->life;
		bShock = TRUE;
	case 211:
		_flash++;
		if( npc->shock && _flash/2%2 ){
			gBoss[4].ani_no = 1;
			gBoss[5].ani_no = 1;
		}else{
			gBoss[4].ani_no = 0;
			gBoss[5].ani_no = 0;
		}
			
		npc->act_wait++;

		if( npc->act_wait%100 == 1 ){
			gCurlyShoot_wait = Random( 80, 100 );
			gCurlyShoot_x    = gBoss[11].x;
			gCurlyShoot_y    = gBoss[11].y;
		}


		// スモークボール
		if( npc->act_wait < 50*6  ){
			if( npc->act_wait%120 ==  1 )
				SetNpChar( 288,	npc->x -32 * VS, npc->y - 16 * VS, 0, 0, DIR_UP,   NULL, 32 );
			if( npc->act_wait%120 == 61 )
				SetNpChar( 288,	npc->x -32 * VS, npc->y + 16 * VS, 0, 0, DIR_DOWN, NULL, 32 );
		}
		// 閉じる
		if( npc->life < _life - 50 || npc->act_wait > 50*8 ) npc->act_no = 200;

		break;

	case 220: // 歯 ----------
		npc->act_no      = 221;
		npc->act_wait    =   0;
		npc->count1      =   0;
		gSuperYpos       =   1;
		gBoss[ 3].act_no =  20;
		gBoss[ 8].bits  |= (BITS_BLOCK_BULLET );
		gBoss[ 9].bits  |= (BITS_BLOCK_BULLET );
		gBoss[10].bits  |= (BITS_BLOCK_BULLET );
		gBoss[11].bits  |= (BITS_BANISH_DAMAGE);
		SetQuake( 50 * 2 );
		_life = npc->life;
//		SetNoise( 500 );
				bShock = TRUE;
	case 221:
		npc->act_wait++;

		// レッドトルネード
		if( npc->act_wait%40 == 1 ){
			switch( Random( 0, 3 ) ){
			case 0: x = gBoss[1].x; y = gBoss[1].y; break;
			case 1: x = gBoss[2].x; y = gBoss[2].y; break;
			case 2: x = gBoss[6].x; y = gBoss[6].y; break;
			case 3: x = gBoss[7].x; y = gBoss[7].y; break;
			}
			PlaySoundObject( WAVE_BUNRET, 1 );
			SetNpChar( 285, x - 16*VS, y, 0, 0,   0*8+DIR_LEFT,  NULL, MAX_NPC/2 );
			SetNpChar( 285, x - 16*VS, y, 0, 0, 128*8+DIR_LEFT,  NULL, MAX_NPC/2 );
		}

		// ダメージ点滅
		_flash++;
		if( npc->shock && _flash/2%2 ){
			gBoss[4].ani_no = 1;
			gBoss[5].ani_no = 1;
		}else{
			gBoss[4].ani_no = 0;
			gBoss[5].ani_no = 0;
		}

		// 閉じる
		if( npc->life     < _life - 150 ||
			npc->act_wait > 50*8        ||
			npc->life     < 200          ) npc->act_no = 200;
		
		break;

	case 230: // 波動砲----------
		npc->act_no      = 231;
		npc->act_wait    =   0;
		gBoss[ 3].act_no =  30;
		gBoss[ 8].bits  |= (BITS_BLOCK_BULLET);
		gBoss[ 9].bits  |= (BITS_BLOCK_BULLET);
		gBoss[10].bits  |= (BITS_BLOCK_BULLET);
		gBoss[11].bits  |= (BITS_BANISH_DAMAGE);
		PlaySoundObject( WAVE_BUNRET, 1 );
		SetNpChar( 285, gBoss[3].x-16*VS, gBoss[3].y, 0, 0,   0*8+DIR_LEFT,  NULL, MAX_NPC/2 );
		SetNpChar( 285, gBoss[3].x-16*VS, gBoss[3].y, 0, 0, 128*8+DIR_LEFT,  NULL, MAX_NPC/2 );
		SetNpChar( 285, gBoss[3].x, gBoss[3].y-16*VS, 0, 0,   0*8+DIR_LEFT,  NULL, MAX_NPC/2 );
		SetNpChar( 285, gBoss[3].x, gBoss[3].y-16*VS, 0, 0, 128*8+DIR_LEFT,  NULL, MAX_NPC/2 );
		SetNpChar( 285, gBoss[3].x, gBoss[3].y+16*VS, 0, 0,   0*8+DIR_LEFT,  NULL, MAX_NPC/2 );
		SetNpChar( 285, gBoss[3].x, gBoss[3].y+16*VS, 0, 0, 128*8+DIR_LEFT,  NULL, MAX_NPC/2 );
		_life  = npc->life;
		bShock = TRUE;
	case 231:
		_flash++;
		if( npc->shock && _flash/2%2 ){
			gBoss[4].ani_no = 1;
			gBoss[5].ani_no = 1;
		}else{
			gBoss[4].ani_no = 0;
			gBoss[5].ani_no = 0;
		}
			
		npc->act_wait++;

		if( npc->act_wait%100 == 1 ){
			gCurlyShoot_wait = Random( 80, 100 );
			gCurlyShoot_x    = gBoss[11].x;
			gCurlyShoot_y    = gBoss[11].y;
		}


		// スモークボール
//		if( npc->act_wait < 50*6  ){
			if( npc->act_wait%120 ==  1 )
				SetNpChar( 288,	npc->x -32 * VS, npc->y - 16 * VS, 0, 0, DIR_UP,   NULL, 32 );
			if( npc->act_wait%120 == 61 )
				SetNpChar( 288,	npc->x -32 * VS, npc->y + 16 * VS, 0, 0, DIR_DOWN, NULL, 32 );
//		}
		// 閉じる
//		if( npc->life < _life - 50 || npc->act_wait > 50*8 ) npc->act_no = 200;

		break;

	case 500:// 煙を吹く
		CutNoise( );
		npc->act_no     = 501;
		npc->act_wait   =   0;
		npc->xm         =   0;
		npc->ym         =   0;
		gBoss[3].act_no =   0;
		gBoss[4].ani_no =   2;
		gBoss[5].ani_no =   0;
		gBoss[1].act_no =   5;
		gBoss[2].act_no =   5;
		gBoss[6].act_no =   5;
		gBoss[7].act_no =   5;

		SetQuake( 20 );
		for( i = 0; i < 100; i++ ){
			SetNpChar( 4,
				npc->x + Random( -128, 128 )*VS,
				npc->y + Random(  -64,  64 )*VS,
				Random( -VS/4, VS/4 ) *VS,
				Random( -VS/4, VS/4 ) *VS,
				DIR_LEFT, NULL, 0 ); 
		}
		DeleteNpCharCode( 282, TRUE );

		gBoss[11].bits &= ~BITS_BANISH_DAMAGE;
		for( i = 0; i < 12; i++ ) gBoss[i].bits &= ~(BITS_BLOCK_BULLET);
	case 501:
		npc->act_wait++;
		if( npc->act_wait%16 ){
			SetNpChar( 4,
				npc->x + Random( -64, 64 )*VS,
				npc->y + Random( -32, 32 )*VS,
				Random( -VS/4, VS/4 ) *VS,
				Random( -VS/4, VS/4 ) *VS,
				DIR_LEFT, NULL, MAX_NPC/2 ); 
		}

		npc->x += VS / 8;
		npc->y += VS / 4;

		if( npc->act_wait > 200 ){
			npc->act_wait =    0;
			npc->act_no   = 1000;
		}

		break;

	// やられ-------
	case 1000:
		SetQuake( 100 );
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait%8 == 0 )
			PlaySoundObject( WAVE_BOM, 1 );
			SetDestroyNpChar( gBoss[0].x + Random(-72,72)*VS,
							  gBoss[0].y + Random(-64,64)*VS, 1,  1 );
		if( gBoss[0].act_wait > 50*2 ){
			gBoss[0].act_wait =    0;
			gBoss[0].act_no   = 1001;
			SetFlash( gBoss[0].x, gBoss[0].y, FLASHMODE_EXPLOSION );
			PlaySoundObject( WAVE_EXPLOSION, 1 );
		}
		break;

	case 1001:
		SetQuake( 40 );
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait > 50 ){
			for( i = 0; i < MAX_BOSS_PARTS; i++ ) gBoss[i].cond = 0;
			DeleteNpCharCode( 158, TRUE );
			DeleteNpCharCode( 301, TRUE );
		}
		break;
	}

	if( bShock ){
		SetQuake( 20 );
		if( npc->act_no == 201 ){ gBoss[1].act_no = gBoss[2].act_no = gBoss[6].act_no = gBoss[7].act_no = 10; }
		if( npc->act_no == 221 ){ gBoss[1].act_no = gBoss[2].act_no = gBoss[6].act_no = gBoss[7].act_no = 20; }
		if( npc->act_no == 231 ){ gBoss[1].act_no = gBoss[2].act_no = gBoss[6].act_no = gBoss[7].act_no = 30; }
		PlaySoundObject( WAVE_QUAKE, 1 );
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, gBoss[4].x + Random( -32, 16 ) * VS, gBoss[4].y,
				Random( -VS, VS ), Random( -VS/2,VS/2), DIR_LEFT, NULL, MAX_NPC/2 );
		}
	}

	// 移動
	if( npc->act_no >= 200 && npc->act_no < 300 ){
		if( npc->x < VS*PARTSSIZE*12             ) npc->direct = DIR_RIGHT;
		if( npc->x > VS*PARTSSIZE*(gMap.width-4) ) npc->direct = DIR_LEFT;
		if( npc->direct == DIR_LEFT              ) npc->xm -= VS/128;
		else                                       npc->xm += VS/128;
	}

	// コアリフト配置
	switch( npc->act_no ){
	case 201: case 211: case 221: case 231:
		npc->count2++;
		if( npc->count2 == 150 ){
			npc->count2 = 0;
			SetNpChar( 282, gMap.width * VS*PARTSSIZE+64, VS*PARTSSIZE * (10 + Random( -1, 3 ) ),
																		0, 0, DIR_LEFT, NULL, 48 );
		}else if( npc->count2 == 75 ){
			SetNpChar( 282, gMap.width * VS*PARTSSIZE+64, VS*PARTSSIZE * ( 3 + Random( -3, 0 ) ),
																		0, 0, DIR_LEFT, NULL, 48 );
		}
		break;
	}

	if( npc->xm >  VS/4 ) npc->xm =  VS/4;
	if( npc->xm < -VS/4 ) npc->xm = -VS/4;
	if( npc->ym >  VS/4 ) npc->ym =  VS/4;
	if( npc->ym < -VS/4 ) npc->ym = -VS/4;

	npc->x += npc->xm;
	npc->y += npc->ym;

	_ActBossCharA_Face( &gBoss[ 3] );
	_ActBossCharA_Head( &gBoss[ 4] );
	_ActBossCharA_Tail( &gBoss[ 5] );
	_ActBossCharA_Mini( &gBoss[ 1] );
	_ActBossCharA_Mini( &gBoss[ 2] );
	_ActBossCharA_Mini( &gBoss[ 6] );
	_ActBossCharA_Mini( &gBoss[ 7] );
	_ActBossCharA_Hit(  &gBoss[ 8] );
	_ActBossCharA_Hit(  &gBoss[ 9] );
	_ActBossCharA_Hit(  &gBoss[10] );
	_ActBossCharA_Hit(  &gBoss[11] );
}