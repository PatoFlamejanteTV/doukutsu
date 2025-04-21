#include <windows.h>
#include "Sound.h"
#include "Draw.h"
#include "Define.h"

#include "Tags.h"

extern MYCHAR gMC;


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


static void ActBossChar02_01( void )
{
	NPCHAR *boss;
	long minus;

	if( gBoss[0].direct == DIR_LEFT ) minus =  1;
	else                              minus = -1;

	boss = &gBoss[1];

	switch( gBoss[0].ani_no ){
	// 無機能
	case 0: 
		boss->hit_voice   = WAVE_NPC_LARGE;
		boss->hit.front   = 16*VS;
		boss->hit.top     = 16*VS;
		boss->hit.back    = 16*VS;
		boss->hit.bottom  = 16*VS;
		boss->size = NPCSIZE_LARGE;
		boss->bits = BITS_BLOCK_BULLET;
		break;
	// ノーマル
	case 1:
		boss->x      = minus* (-24*VS) + gBoss[0].x;
		boss->y      =        (-24*VS) + gBoss[0].y;
		break;
	// 伏せ
	case 2:
		boss->x      = minus* (-24*VS) + gBoss[0].x;
		boss->y      =        (-20*VS) + gBoss[0].y;
		break;
	// 開口
	case 3: case 4:
		boss->x      = minus* (-24*VS) + gBoss[0].x;
		boss->y      =        (-16*VS) + gBoss[0].y;
		break;
	// ジャンプ
	case 5:
		boss->x      = minus* (-24*VS) + gBoss[0].x;
		boss->y      =        (-43*VS) + gBoss[0].y;
		break;
	}
}
static void ActBossChar02_02( void )
{
	NPCHAR *boss;

	boss = &gBoss[2];

	switch( gBoss[0].ani_no ){
	// 無機能
	case 0:
		boss->hit_voice   = WAVE_NPC_LARGE;
		boss->hit.front   = 24*VS;
		boss->hit.top     = 16*VS;
		boss->hit.back    = 24*VS;
		boss->hit.bottom  = 16*VS;
		boss->size = NPCSIZE_LARGE;
		boss->bits = BITS_BLOCK_BULLET;
		break;
	// ノーマル
	case 1:
	// 伏せ
	case 2:
	// 開口
	case 3: case 4:
	// ジャンプ
	case 5:
		boss->x      = gBoss[0].x;
		boss->y      = gBoss[0].y;
		break;
	}

}


void ActBossChar_Frog( void )
{
	unsigned char deg;
	long xm, ym;
	long i;

	RECT rcLeft[] = {
		{  0,  0,  0,  0},
		{  0, 48, 80,112},
		{  0,112, 80,176},
		{  0,176, 80,240},
		{160, 48,240,112},// Shock!
		{160,112,240,200},// ジャンプ
		{200,  0,240, 24},//6 白目

		{ 80,  0,120, 24},//7 銀かがみ
		{120,  0,160, 24},//8 銀ジャンプ
	};

	RECT rcRight[] = {
		{  0,  0,  0,  0},
		{ 80, 48,160,112},
		{ 80,112,160,176},
		{ 80,176,160,240},
		{240, 48,320,112},// Shock!
		{240,112,320,200},// ジャンプ
		{200, 24,240, 48},//6 白目

		{ 80, 24,120, 48},//7 銀かがみ
		{120, 24,160, 48},//8 銀ジャンプ
	};

	NPCHAR *boss;

	boss = &gBoss[0];

	switch( boss->act_no ){
	case 0:// 初期設定
		boss->x =  6*VS*PARTSSIZE;
		boss->y = 12*VS*PARTSSIZE + VS*8;
		boss->direct = DIR_RIGHT;
		boss->view.front  = 48*VS;
		boss->view.top    = 48*VS;
		boss->view.back   = 32*VS;
		boss->view.bottom = 16*VS;
		boss->hit_voice   = WAVE_NPC_LARGE;
		boss->hit.front   = 24*VS;
		boss->hit.top     = 16*VS;
		boss->hit.back    = 24*VS;
		boss->hit.bottom  = 16*VS;
		boss->size        = NPCSIZE_LARGE;
		boss->exp         = 1;

		boss->code_event = 1000;
		boss->bits      |= BITS_EVENT_BREAK|BITS_VIEWDAMAGE;

		boss->life = 300;


	case 1:// 待機
		break;
	case 10:// 待機
		boss->act_no = 11;
		boss->ani_no = 3;
		boss->cond = COND_ALIVE;
		boss->rect = rcRight[0];
//		boss->act_no = 100;

		gBoss[1].cond = COND_ALIVE|COND_ZEROINDEXDAMAGE;
		gBoss[1].code_event = 1000;
		gBoss[2].cond = COND_ALIVE;
		gBoss[1].damage = 5;
		gBoss[2].damage = 5;
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, boss->x + Random( -12, 12 ) * VS, boss->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}

	case 11:
		break;

	case 20:// 登場点滅
		boss->act_no   = 21;
		boss->act_wait = 0;
	case 21:
		boss->act_wait++;
		if( boss->act_wait/2%2 ) boss->ani_no = 3;
		else                     boss->ani_no = 0;
//		if( boss->act_wait > 50 )
//			boss->act_no = 10;
		break;


	// ピョンピョン ===================
	case 100:// 待機
		boss->act_no   = 101;
		boss->act_wait =   0;
		boss->ani_no   =   1;
		boss->xm       =   0;
//		boss->bits = BITS_BLOCK_BULLET;
	case 101:// 停止
		boss->act_wait++;
		if( boss->act_wait > 50 ){
			boss->act_no   = 102;
			boss->ani_wait =  0;
			boss->ani_no   =  2;
		}
		break;
	case 102:// かがみ
		boss->ani_wait++;
		if( boss->ani_wait > 10 ){
			boss->act_no   = 103;
			boss->ani_wait =  0;
			boss->ani_no   =  1;
		}
		break;
	case 103:// to Jump
		boss->ani_wait++;
		if( boss->ani_wait > 4 ){
			boss->act_no = 104;
			boss->ani_no =   5;
			boss->ym     = -VS*2;
			PlaySoundObject( WAVE_BUNRET, 1 );
			if( boss->direct == DIR_LEFT ) boss->xm = -VS;
			else                              boss->xm =  VS;
			boss->view.top    = 64*VS;
			boss->view.bottom = 24*VS;
		}
		break;
	case 104:// 滞空
		// ■←
		if( boss->direct == DIR_LEFT &&
			boss->flag & FLAG_HIT_LEFT ){
			boss->direct = DIR_RIGHT;
			boss->xm = VS;
		}
		// ■←
		if( boss->direct == DIR_RIGHT &&
			boss->flag & FLAG_HIT_RIGHT ){
			boss->direct = DIR_LEFT;
			boss->xm = -VS;
		}
		// 着陸
		if( boss->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
			boss->act_no      = 100;
			boss->ani_no      = 1;
			boss->view.top    = 48*VS;
			boss->view.bottom = 16*VS;
			if( boss->direct == DIR_LEFT &&
				boss->x < gMC.x ){
				boss->direct = DIR_RIGHT;
				boss->act_no = 110;
			}
			if( boss->direct == DIR_RIGHT &&
				boss->x > gMC.x ){
				boss->direct = DIR_LEFT;
				boss->act_no = 110;
			}
			SetNpChar( 110, PARTSSIZE*VS*Random(4, 16),
				PARTSSIZE*VS*Random( 0, 4 ), 0, 0, 4, NULL, MAX_NPC/4 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, boss->x + Random( -12, 12 ) * VS, boss->y + boss->hit.bottom,
							Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
		break;

	// 泡吐き ==============================
	case 110:// 口開く
		boss->ani_no   = 1;
		boss->act_wait = 0;
		boss->act_no   = 111;
	case 111:
		boss->act_wait++;
		boss->xm = boss->xm * 8 / 9 ;
		if( boss->act_wait > 50 ){
			boss->ani_no   = 2;
			boss->ani_wait = 0;
			boss->act_no   = 112;
		}
		break;
	case 112:
		boss->ani_wait++;
		if( boss->ani_wait > 4 ){
			boss->act_no   = 113;
			boss->act_wait =  0;
			boss->ani_no   =  3;
			boss->count1   = 16;
			gBoss[1].bits |= BITS_BANISH_DAMAGE;
			boss->tgt_x    = boss->life;
		}
		break;
	case 113:
		// ダメージ点滅
		if( boss->shock ){
			if( (boss->count2++)/2%2 ) boss->ani_no = 4;
			else                       boss->ani_no = 3;
		}else{
			boss->count2 = 0;
			boss->ani_no = 3;
		}

		boss->xm = boss->xm*10/11;
		boss->act_wait++;
		if( boss->act_wait > 16 ){
			boss->act_wait = 0;
			boss->count1--;
			if( boss->direct == DIR_LEFT )
				deg = GetArktan( boss->x-32*VS - gMC.x, boss->y-8*VS - gMC.y );
			else
				deg = GetArktan( boss->x+32*VS - gMC.x, boss->y-8*VS - gMC.y );
			deg += (unsigned char)Random( -16, 16 );

			ym = GetSin( deg ) * 1;
			xm = GetCos( deg ) * 1;
			if( boss->direct == DIR_LEFT )
				SetNpChar( 108, boss->x-32*VS, boss->y-8*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			else
				SetNpChar( 108, boss->x+32*VS, boss->y-8*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );

			PlaySoundObject( WAVE_POP, 1 );
			if( !boss->count1 || boss->life < boss->tgt_x - 90 ){
				boss->act_no   = 114;
				boss->act_wait =   0;
				boss->ani_no   =   2;
				boss->ani_wait =   0;
				gBoss[1].bits &= ~BITS_BANISH_DAMAGE;
			}
		}
		break;
	case 114:
		boss->ani_wait++;
		if( boss->ani_wait > 10 ){
			gBoss[1].count1++;
			if( gBoss[1].count1 > 2 ){
				gBoss[1].count1 = 0;
				boss->act_no = 120;
			}else boss->act_no   = 100;
			boss->ani_wait =   0;
			boss->ani_no   =   1;
		}
		break;

	// カエルの雨 ========================
	case 120:// 待機
		boss->act_no   = 121;
		boss->act_wait =   0;
		boss->ani_no   =   1;
		boss->xm       =   0;
	case 121:// 停止
		boss->act_wait++;
		if( boss->act_wait > 50 ){
			boss->act_no   = 122;
			boss->ani_wait =  0;
			boss->ani_no   =  2;
		}
		break;
	case 122:// かがみ
		boss->ani_wait++;
		if( boss->ani_wait > 20 ){
			boss->act_no   = 123;
			boss->ani_wait =  0;
			boss->ani_no   =  1;
		}
		break;
	case 123:// to Jump
		boss->ani_wait++;
		if( boss->ani_wait > 4 ){
			boss->act_no = 124;
			boss->ani_no =   5;
			boss->ym     = -VS*5;
			boss->view.top    = 64*VS;
			boss->view.bottom = 24*VS;
			PlaySoundObject( WAVE_BUNRET, 1 );
		}
		break;

	case 124:
		// 着陸
		if( boss->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 60 );
			boss->act_no      = 100;
			boss->ani_no      = 1;
			boss->view.top    = 48*VS;
			boss->view.bottom = 16*VS;

			for( i = 0; i < 2; i++ ){
				SetNpChar( 104, PARTSSIZE*VS*Random(4, 16),
					PARTSSIZE*VS*Random( 0, 4 ), 0, 0, 4, NULL, MAX_NPC/4 );
			}
			for( i = 0; i < 6; i++ ){
				SetNpChar( 110, PARTSSIZE*VS*Random(4, 16),
					PARTSSIZE*VS*Random( 0, 4 ), 0, 0, 4, NULL, MAX_NPC/4 );
			}
			for( i = 0; i < 8; i++ ){
				SetNpChar( 4, boss->x + Random( -12, 12 ) * VS, boss->y + boss->hit.bottom,
							Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			if( boss->direct == DIR_LEFT &&
				boss->x < gMC.x ){
				boss->direct = DIR_RIGHT;
				boss->act_no = 110;
			}
			if( boss->direct == DIR_RIGHT &&
				boss->x > gMC.x ){
				boss->direct = DIR_LEFT;
				boss->act_no = 110;
			}
		}

		break;
	// がたがた ========================
	case 130:
		
		boss->act_no   = 131;
		boss->ani_no   = 3;
		boss->act_wait = 0;
		boss->xm       = 0;
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, boss->x + Random( -12, 12 ) * VS, boss->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		gBoss[1].cond = 0;
		gBoss[2].cond = 0;
	case 131://ぶるぶる
		boss->act_wait++;
		if( boss->act_wait%5 == 0 )
			SetNpChar( 4, boss->x + Random( -12, 12 ) * VS, boss->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );

		if( boss->act_wait/2%2 ) boss->x -= VS;
		else                     boss->x += VS;

		if( boss->act_wait > 100 ){
			boss->act_wait = 0;
			boss->act_no   = 132;
		}

		break;
	case 132://ちいさく
		boss->act_wait++;
		if( boss->act_wait/2%2 ){
			boss->view.front  = 20*VS;
			boss->view.top    = 12*VS;
			boss->view.back   = 20*VS;
			boss->view.bottom = 12*VS;
			boss->ani_no = 6;
		}else{
			boss->view.front  = 48*VS;
			boss->view.top    = 48*VS;
			boss->view.back   = 32*VS;
			boss->view.bottom = 16*VS;
			boss->ani_no = 3;
		}

		if( boss->act_wait%9 == 0 )
			SetNpChar( 4, boss->x + Random( -12, 12 ) * VS, boss->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );

		if( boss->act_wait > 150 ){
			boss->act_no     = 140;
			boss->hit.bottom = 12*VS;
		}
		break;
	// 落下/退散 ===================
	case 140:
		boss->act_no = 141;
	case 141:
		if( boss->flag & FLAG_HIT_BOTTOM ){
			boss->act_no = 142;
			boss->act_wait = 0;
			boss->ani_no = 7;
		}
		break;
	case 142:
		boss->act_wait++;
		if( boss->act_wait > 30 ){
			boss->ani_no = 8;
			boss->ym     = -5*VS;
			boss->bits   |= BITS_THROW_BLOCK;
			boss->act_no = 143;
		}
		break;
	case 143:
		boss->ym     = -5*VS;
		if( boss->y < 0 ){
			boss->cond = 0;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
		}
		break;
	}	


	// 重力
	boss->ym += VS/8;
	if( boss->ym > MAX_MOVE ) boss->ym = MAX_MOVE;

	boss->x += boss->xm;
	boss->y += boss->ym;


	// RECT

	if( boss->direct == DIR_LEFT ) boss->rect = rcLeft[ boss->ani_no];
	else                           boss->rect = rcRight[boss->ani_no];


	ActBossChar02_01();
	ActBossChar02_02();

}


