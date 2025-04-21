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
BOOL ChangeMapParts( long x, long y, unsigned char no );
long CountArmsBullet( long arms_code );

//
void ActBossChar_Press( void )
{
	NPCHAR *npc;
	static unsigned char _flash;
	long i;
	long x;

	npc = &gBoss[0];

	switch( npc->act_no ){
	case 0:// 初期設定
		npc->act_no      = 10;
		npc->cond        = COND_ALIVE;
		npc->exp         = 1;
		npc->direct      = DIR_RIGHT;
//		npc->act_no      = 100;
		npc->x           = 0*VS*PARTSSIZE;
		npc->y           = 0*VS*PARTSSIZE;
		npc->view.front  = 40*VS;
		npc->view.top    = 60*VS;
		npc->view.back   = 40*VS;
		npc->view.bottom = 60*VS;
		npc->hit_voice   = WAVE_NPC_GOHST;
		npc->hit.front   = 49*VS;
		npc->hit.top     = 60*VS;
		npc->hit.back    = 40*VS;
		npc->hit.bottom  = 48*VS;
		npc->bits        = BITS_BLOCK_MYCHAR2|BITS_THROW_BLOCK|BITS_EVENT_BREAK|BITS_VIEWDAMAGE;//|BITS_BANISH_DAMAGE;
		npc->size        = NPCSIZE_LARGE;
		npc->damage      = 10;
		

		npc->code_event  = 1000;
		npc->life        = 70*10;
	case 1:// 待機
		break;
	case 5:// HELL3配置
		npc->act_no   = 6;
		npc->x        = 0;
		npc->y        = 0;
		gBoss[1].cond = 0;
		gBoss[2].cond = 0;
	case   6:
		break;

	case  10:// HELL3配置
		npc->act_no = 11;
		npc->x      = 10*VS*PARTSSIZE;
		npc->y      = 74*VS;
	case  11:
		break;
		
	case  20:// HELL4配置
		npc->damage =  0;
		npc->act_no = 21;
		npc->x      = 10*VS*PARTSSIZE;
		npc->y      = 25*VS*PARTSSIZE + 13*VS;
		npc->bits &= ~BITS_BLOCK_MYCHAR2;
		gBoss[1].cond = 0;
		gBoss[2].cond = 0;
	case  21:
		npc->act_wait++;
		if( npc->act_wait%16 == 0 ){
			SetDestroyNpChar( npc->x + Random(-40,40)*VS,
							  npc->y + Random(-60,60)*VS, 1,  1 );
		}
		break;

	case  30:// HELL4、落下配置
		npc->act_no = 31;
		npc->ani_no =  2;
		npc->x      = 10*VS*PARTSSIZE;
		npc->y      =  4*VS*PARTSSIZE;
	case  31:
		npc->y     += VS*4;
		if( npc->y >= 25*VS*PARTSSIZE + 13*VS ){
			npc->y      = 25*VS*PARTSSIZE + 13*VS;
			npc->ani_no =  0;
			npc->act_no = 20;
			PlaySoundObject( WAVE_BOM, 1 );
			for( i = 0; i < 5; i++ ){
				x =npc->x +  Random( -40, 40 ) * VS;
				SetNpChar( 4, x, npc->y + 60*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
		break;

	case 100: //// 戦闘開始
		npc->act_no         = 101;
		npc->count2         =  9;
		npc->act_wait       = -100;
//		npc->bits |= BITS_BANISH_DAMAGE;
		gBoss[1].cond       = COND_ALIVE;
		gBoss[1].hit.front  = 14*VS;
		gBoss[1].hit.back   = 14*VS;
		gBoss[1].hit.top    =  8*VS;
		gBoss[1].hit.bottom =  8*VS;
		gBoss[1].bits       = BITS_BLOCK_BULLET|BITS_THROW_BLOCK;
		gBoss[2]            = gBoss[1];
		gBoss[3].cond       = COND_ALIVE|COND_ZEROINDEXDAMAGE;
		gBoss[3].bits       |= BITS_BANISH_DAMAGE;
		gBoss[3].hit.front  =  6*VS;
		gBoss[3].hit.back   =  6*VS;
		gBoss[3].hit.top    =  8*VS;
		gBoss[3].hit.bottom =  8*VS;
		SetNpChar( 325, npc->x, npc->y + 60 * VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
	case 101:
		// ブロック破壊
		if( npc->count2 >1 && npc->life < npc->count2 * 70 ){
			npc->count2--;
			for( i = 0; i < 5; i++ ){
				ChangeMapParts( 8 + i, npc->count2, 0 );
				SetDestroyNpChar( ( 8 + i )*VS*PARTSSIZE, npc->count2*VS*PARTSSIZE,  0, 4 );
				PlaySoundObject( WAVE_BREAK1, 1 );
			}
		}

		npc->act_wait++;
		if( npc->act_wait == 81 || npc->act_wait == 241 ){
			SetNpChar( 323,  3*PARTSSIZE*VS, 15*PARTSSIZE*VS, 0, 0, DIR_UP, NULL, MAX_NPC/2 );
		}
		if( npc->act_wait ==  1 || npc->act_wait == 161 ){
			SetNpChar( 323, 17*PARTSSIZE*VS, 15*PARTSSIZE*VS, 0, 0, DIR_UP, NULL, MAX_NPC/2 );
		}
		// プラズマ
		if( npc->act_wait >= 300 ){
			npc->act_wait  = 0;
			SetNpChar( 325, npc->x, npc->y + 60 * VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		}
		break;

	case 500: //// 倒した
		gBoss[3].bits &= ~BITS_BANISH_DAMAGE;
		npc->act_no    = 501;
		npc->act_wait  =   0;
		npc->count1    =   0;
		DeleteNpCharCode( 325, TRUE );
		DeleteNpCharCode( 330, TRUE );
	case 501:
		npc->act_wait++;
		if( npc->act_wait%16 == 0 ){
			PlaySoundObject( WAVE_BREAK1, 1 );
			SetDestroyNpChar( npc->x + Random(-40,40)*VS,
							  npc->y + Random(-60,60)*VS, 1,  1 );
		}
		if( npc->act_wait == 95 ) npc->ani_no = 1;
		if( npc->act_wait == 98 ) npc->ani_no = 2;
		if( npc->act_wait > 100 ){
			npc->act_no   = 510;
		}
		break;
	case 510:// 落下
		npc->ym    += VS/8;
		npc->damage = 127;
		npc->y     += npc->ym;
		if( !npc->count1 && npc->y > 10*VS*PARTSSIZE ){
			npc->count1 =   1;
			npc->ym     = -VS;
			npc->damage =   0;
			for( i = 0; i < 7; i++ ){
				ChangeMapParts( 7 + i, 14, 0 );
				SetDestroyNpChar( ( 7 + i )*VS*PARTSSIZE, 14*VS*PARTSSIZE,  0, 0 );
				PlaySoundObject( WAVE_BREAK1, 1 );
			}
		}
		if( npc->y > 30*VS*PARTSSIZE ){
			npc->act_no = 520;
		}
		break;
	case 520:// 画面外
		break;
	}
	
	
	gBoss[1].x = npc->x - 24*VS;
	gBoss[1].y = npc->y + 52*VS;
	gBoss[2].x = npc->x + 24*VS;
	gBoss[2].y = npc->y + 52*VS;
	gBoss[3].x = npc->x;
	gBoss[3].y = npc->y + 40*VS;


	RECT rc[] = {
		{  0,  0, 80,120},
		{ 80,  0,160,120},
		{160,  0,240,120},
	};

	RECT rcDamage[] = {
		{  0,120, 80,240},
		{ 80,120,160,240},
		{160,120,240,240},
	};

	if( npc->shock ){
		_flash++;
		if( _flash/2%2 ) npc->rect = rc[      npc->ani_no];
		else             npc->rect = rcDamage[npc->ani_no];
	}else{
		npc->rect = rc[npc->ani_no];
	}
}


