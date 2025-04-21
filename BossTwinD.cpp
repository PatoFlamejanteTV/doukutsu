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

long CountArmsBullet( long arms_code );



static void ActBossCharT_DragonBody( NPCHAR *npc )
{
	unsigned char deg;
	RECT rcLeft[] = {
		{  0, 0, 40,40},
		{ 40, 0, 80,40},
		{ 80, 0,120,40},
	};
	RECT rcRight[] = {
		{  0,40, 40,80},
		{ 40,40, 80,80},
		{ 80,40,120,80},
	};

	switch( npc->act_no ){
	case 0:
		deg = (unsigned char)(( (npc->pNpc->count1/4 + npc->count1) )%256);
		npc->act_no = 10;
		npc->x += npc->pNpc->x + GetCos( deg ) * npc->pNpc->tgt_x;
		npc->y += npc->pNpc->y + GetSin( deg ) * npc->pNpc->tgt_y;
	case 10:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		break;

	case 100:
		deg = (unsigned char)(( (npc->pNpc->count1/4 + npc->count1) )%256);
		npc->tgt_x = npc->pNpc->x + GetCos( deg ) * npc->pNpc->tgt_x;
		npc->tgt_y = npc->pNpc->y + GetSin( deg ) * npc->pNpc->tgt_y;
		npc->x += (npc->tgt_x - npc->x )*1/8;
		npc->y += (npc->tgt_y - npc->y )*1/8;


		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		break;

	case 401:
		break;

	case 1000://やられ
		npc->act_no = 1001;
		npc->bits &= ~BITS_BANISH_DAMAGE;
	case 1001:
		deg = (unsigned char)(( (npc->pNpc->count1/4 + npc->count1) )%256);
		npc->tgt_x = npc->pNpc->x + GetCos( deg ) * npc->pNpc->tgt_x;
		npc->tgt_y = npc->pNpc->y + GetSin( deg ) * npc->pNpc->tgt_y;
		npc->x += (npc->tgt_x - npc->x )*1/8;
		npc->y += (npc->tgt_y - npc->y )*1/8;
		if( npc->x > npc->pNpc->x ) npc->direct = DIR_LEFT;
		else                        npc->direct = DIR_RIGHT;
		break;
	}
	
	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )
		npc->ani_no = 0;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// head
static void ActBossCharT_DragonHead( NPCHAR *npc )
{
	unsigned char deg;
	long xm, ym;
	RECT rcLeft[] = {
		{  0,80, 40,112},
		{ 40,80, 80,112},
		{ 80,80,120,112},
		{120,80,160,112},
	};
	RECT rcRight[] = {
		{  0,112, 40,144},
		{ 40,112, 80,144},
		{ 80,112,120,144},
		{120,112,160,144},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		break;

	case 100:
		npc->act_no = 200;

	case 200:// 素顔待機
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->ani_no = 0;
		npc->hit.front = 16*VS; 
		npc->act_no = 201;
		npc->count1 = Random( 100, 200 );
	case 201:
		if( npc->count1 ) npc->count1--;
		else{
			npc->act_no   = 210;
			npc->act_wait = 0;
			npc->count2   = 0;
		}
		break;

	case 210://口を開く
		npc->act_wait++;
		if( npc->act_wait == 3 ){
			npc->ani_no = 1;
		}
		if( npc->act_wait == 6 ){
			npc->ani_no = 2;
			npc->hit.front = 8*VS; 
			npc->bits |= BITS_BANISH_DAMAGE;
			npc->count2   = 0;
		}
		if( npc->act_wait > 150 ){
			npc->act_no = 220;
			npc->act_wait = 0;
		}
		if( npc->shock ) npc->count2++;
		if( npc->count2 > 10 ){// ギャフン
			PlaySoundObject( WAVE_NPC_MIDDLE, 1 );
			SetDestroyNpChar( npc->x, npc->y, npc->view.back,  4 );
			npc->act_no   = 300;
			npc->act_wait = 0;
			npc->ani_no   = 3;
			npc->hit.front = 16*VS; 
		}
		break;
	// ファイア
	case 220:
		npc->act_wait++;
		if( npc->act_wait%8==1 ){
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -6, 6 );

			ym = GetSin( deg ) * 1;
			xm = GetCos( deg ) * 1;
			if( npc->direct == DIR_LEFT ) SetNpChar( 202, npc->x-VS*8, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			else                          SetNpChar( 202, npc->x+VS*8, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_FRONTIA, 1 );
		}

		if( npc->act_wait > 50 ){
			npc->act_no   = 200;
		}
		break;

	// sleep
	case 300:
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_no = 200;
		}
		break;

	// 突進
	case 400:
		npc->act_no = 401;
		npc->act_wait = 0;
		npc->ani_no = 0;
		npc->hit.front = 16*VS; 
		npc->bits &= ~BITS_BANISH_DAMAGE;
	case 401:

		npc->act_wait++;
		if( npc->act_wait == 3 ){
			npc->ani_no = 1;
		}
		if( npc->act_wait == 6 ){
			npc->ani_no = 2;
			npc->hit.front = 8*VS; 
			npc->bits |= BITS_BANISH_DAMAGE;
			npc->count2   = 0;
		}
		if( npc->act_wait > 20 && npc->act_wait%32 == 1 ){
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -6, 6 );

			ym = GetSin( deg ) * 1;
			xm = GetCos( deg ) * 1;
			if( npc->direct == DIR_LEFT ) SetNpChar( 202, npc->x-VS*8, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			else                          SetNpChar( 202, npc->x+VS*8, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_FRONTIA, 1 );
		}
		break;

	case 1000://やられ
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->ani_no = 3;
		break;
	}
	
	npc->direct = npc->pNpc->direct;
	if( npc->direct == DIR_LEFT ) npc->x = npc->pNpc->x - 4*VS;
	else                          npc->x = npc->pNpc->x + 4*VS;
	npc->y = npc->pNpc->y - 8*VS;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

void ActBossChar_Twin( void )
{
	NPCHAR *npc;
	static unsigned char _flash;

	npc = &gBoss[0];

	switch( npc->act_no ){
	case 0:// 初期設定
		npc->cond            = COND_ALIVE;
		npc->direct          = DIR_LEFT;
		npc->act_no          = 10;
		npc->exp             = 0;
		npc->x               = VS*PARTSSIZE*10;
		npc->y               = VS*PARTSSIZE*16/2;
		npc->view.front      = 8*VS;
		npc->view.top        = 8*VS;
		npc->view.back       = 128*VS;
		npc->view.bottom     = 8*VS;
		npc->hit_voice       = WAVE_NPC_GOHST;
		npc->hit.front       = 8*VS;
		npc->hit.top         = 8*VS;
		npc->hit.back        = 8*VS;
		npc->hit.bottom      = 8*VS;
		npc->bits            = BITS_THROW_BLOCK;
		npc->bits           |= BITS_EVENT_BREAK;
		npc->size            = NPCSIZE_LARGE;
		npc->damage          = 0;

		npc->code_event      = 1000;
		npc->life            = 500;
		npc->count2          = Random( 700, 1200 );
		npc->tgt_x           = 180;
		npc->tgt_y           =  61;


		// head
		gBoss[2].cond        = COND_ALIVE;
		gBoss[2].view.back   = 20*VS;
		gBoss[2].view.front  = 20*VS;
		gBoss[2].view.top    = 16*VS;
		gBoss[2].view.bottom = 16*VS;
		gBoss[2].hit.back    = 12*VS;
		gBoss[2].hit.front   = 12*VS;
		gBoss[2].hit.top     = 10*VS;
		gBoss[2].hit.bottom  = 10*VS;
		gBoss[2].bits        = BITS_THROW_BLOCK|BITS_BLOCK_BULLET;
		gBoss[2].pNpc        = &gBoss[3];
		gBoss[2].cond       |= COND_ZEROINDEXDAMAGE;
		gBoss[2].damage      = 10;

		// body
		gBoss[3].cond        = COND_ALIVE;
		gBoss[3].view.back   = 20*VS;
		gBoss[3].view.front  = 20*VS;
		gBoss[3].view.top    = 20*VS;
		gBoss[3].view.bottom = 20*VS;
		gBoss[3].hit.back    = 12*VS;
		gBoss[3].hit.front   = 12*VS;
		gBoss[3].hit.top     =  2*VS;
		gBoss[3].hit.bottom  = 16*VS;
		gBoss[3].bits        = BITS_THROW_BLOCK;//|BITS_BLOCK_BULLET;;
		gBoss[3].pNpc        = npc;
		gBoss[3].damage      = 10;

		gBoss[4]             =  gBoss[2];
		gBoss[4].pNpc        = &gBoss[5];
		gBoss[5]             =  gBoss[3];
		gBoss[5].count1      = 128;


	
	case 10:// 待機
		break;

	case 20:// 出現
		npc->tgt_x--;
		if( npc->tgt_x <= 112 ){
			npc->act_no   = 100;
			npc->act_wait = 0;
			gBoss[2].act_no = 100;
			gBoss[4].act_no = 100;
			gBoss[3].act_no = 100;
			gBoss[5].act_no = 100;
		}
		break;

	case 100:
		npc->act_wait++;
		if(      npc->act_wait <             100 ) npc->count1 += 1;
		else if( npc->act_wait <             120 ) npc->count1 += 2;
		else if( npc->act_wait < npc->count2     ) npc->count1 += 4;
		else if( npc->act_wait < npc->count2 +40 ) npc->count1 += 2;
		else if( npc->act_wait < npc->count2 +60 ) npc->count1 += 1;
		else{
			npc->act_wait = 0;
			npc->act_no = 110;
			npc->count2 = Random( 400, 700 );
			break;
		}
		if( npc->count1 > 1023 ){
			npc->count1 -= 1024;
		}
		break;

	case 110:
		npc->act_wait++;
		if(      npc->act_wait <              20 ) npc->count1 -= 1;
		else if( npc->act_wait <              60 ) npc->count1 -= 2;
		else if( npc->act_wait < npc->count2     ) npc->count1 -= 4;
		else if( npc->act_wait < npc->count2 +40 ) npc->count1 -= 2;
		else if( npc->act_wait < npc->count2 +60 ) npc->count1 -= 1;
		else{
			if( npc->life < 300 ){
				npc->act_wait   = 0;
				npc->act_no     = 400;
				gBoss[2].act_no = 400;
				gBoss[4].act_no = 400;
			}else{
				npc->act_wait = 0;
				npc->act_no = 100;
				npc->count2 = Random( 400, 700 );
			}
			break;
		}
		if( npc->count1 <= 0 ){
			npc->count1 += 1024;
		}
		break;

	case 400:// スクリュー
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_wait = 0;
			npc->act_no = 401;
		}
		break;
	case 401:
		npc->act_wait++;
		if(      npc->act_wait <     100 ) npc->count1 += 1;
		else if( npc->act_wait <     120 ) npc->count1 += 2;
		else if( npc->act_wait < 500     ) npc->count1 += 4;
		else if( npc->act_wait < 500 +40 ) npc->count1 += 2;
		else if( npc->act_wait < 500 +60 ) npc->count1 += 1;
		else{
			npc->act_no   = 100;
			npc->act_wait = 0;
			gBoss[2].act_no = 100;
			gBoss[4].act_no = 100;
			break;
		}
		if( npc->count1 > 1023 ){
			npc->count1 -= 1024;
		}
		break;

	case 1000: // やられ
		npc->act_no   = 1001;
		npc->act_wait =    0;
		gBoss[2].act_no      = 1000;
		gBoss[3].act_no      = 1000;
		gBoss[4].act_no      = 1000;
		gBoss[5].act_no      = 1000;
		SetDestroyNpChar( npc->x, npc->y, npc->view.back, 40 );

	case 1001:// 停止
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_no = 1010;
		}
		SetNpChar( 4, npc->x+Random( -128, 128 )*VS,
					  npc->y+Random( -70,  70 )*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		break;

	case 1010:// 接近
		npc->count1 += 4;
		if( npc->count1 > 1023 ){
			npc->count1 -= 1024;
		}
		if( npc->tgt_x >  8 ) npc->tgt_x--;
		if( npc->tgt_y >  0 ) npc->tgt_y--;
		if( npc->tgt_x < -8 ) npc->tgt_x++;
		if( npc->tgt_y <  0 ) npc->tgt_y++;
		if( !npc->tgt_y ){
			npc->act_no = 1020;
			npc->act_wait = 0;
			SetFlash( gBoss[0].x, gBoss[0].y, FLASHMODE_EXPLOSION );
			PlaySoundObject( WAVE_EXPLOSION, 1 );
		}
		break;

	case 1020:
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait > 50 ){
			DeleteNpCharCode( 211, TRUE );
			gBoss[0].cond = 0;
			gBoss[1].cond = 0;
			gBoss[2].cond = 0;
			gBoss[3].cond = 0;
			gBoss[4].cond = 0;
			gBoss[5].cond = 0;
			gBoss[0].act_no = 0;
		}
		break;
	case 1030:
		break;
	
	}


	ActBossCharT_DragonHead( &gBoss[2] );
	ActBossCharT_DragonBody( &gBoss[3] );
	ActBossCharT_DragonHead( &gBoss[4] );
	ActBossCharT_DragonBody( &gBoss[5] );



	RECT rc = {0,0,0,0};
	npc->rect = rc;

}


