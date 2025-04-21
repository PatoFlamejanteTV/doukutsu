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


void ActBossChar_Ironhead( void )
{
	long i;
	NPCHAR *npc;
	static unsigned char _flash;

	npc = &gBoss[0];

	switch( npc->act_no ){
	case 0:// 初期設定
		npc->cond        = COND_ALIVE;
		npc->exp         = 1;
		npc->direct      = DIR_RIGHT;
		npc->act_no      = 100;
		npc->x           = 10*VS*PARTSSIZE;
		npc->y           =  8*VS*PARTSSIZE;
		npc->view.front  = 40*VS;
		npc->view.top    = 12*VS;
		npc->view.back   = 24*VS;
		npc->view.bottom = 12*VS;
		npc->hit_voice   = WAVE_NPC_GOHST;
		npc->hit.front   = 16*VS;
		npc->hit.top     = 10*VS;
		npc->hit.back    = 16*VS;
		npc->hit.bottom  = 10*VS;
		npc->bits = BITS_THROW_BLOCK|BITS_EVENT_BREAK|BITS_BANISH_DAMAGE|BITS_VIEWDAMAGE;
		npc->size = NPCSIZE_LARGE;
		npc->damage      = 10;

		npc->code_event  = 1000;
		npc->life        =  400;


	
	case 1:// 待機
		break;

	case 100: // 画面外待機
		npc->act_no   = 101;
		npc->bits    &= ~BITS_BANISH_DAMAGE;
		npc->act_wait = 0;

	case 101:
		npc->act_wait++;
		if( npc->act_wait > 1*50 ){
			npc->act_no = 250;
			npc->act_wait = 0;
		}
		if( !(npc->act_wait%4) )
			SetNpChar( 197, VS*PARTSSIZE*Random( 15, 18 ),
							VS*PARTSSIZE*Random(  2, 13 ), 0, 0, 0, 0, MAX_NPC/2 );
		break;

	case 250: // 突進
		npc->act_no = 251;
		if( npc->direct == DIR_RIGHT ){
			npc->x = VS*PARTSSIZE*15;
			npc->y = gMC.y;
		}else{
			npc->x = VS*PARTSSIZE*45;
			npc->y = VS*PARTSSIZE*Random( 2, 13 );
		}
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
		npc->ym = Random( -VS, VS );
		npc->xm = Random( -VS, VS );
		npc->bits  |= BITS_BANISH_DAMAGE;



	case 251:

		if( npc->direct == DIR_RIGHT ) npc->tgt_x += 2*VS;
		else{
			npc->tgt_x -= 1*VS;
			if( npc->tgt_y < gMC.y ) npc->tgt_y += VS;
			else                     npc->tgt_y -= VS;
		}

		if( npc->x < npc->tgt_x ) npc->xm += VS/64;
		else                      npc->xm -= VS/64;
		if( npc->y < npc->tgt_y ) npc->ym += VS/64;
		else                      npc->ym -= VS/64;

		if( npc->ym >  VS ) npc->ym =  VS;
		if( npc->ym < -VS ) npc->ym = -VS;

		npc->x += npc->xm;
		npc->y += npc->ym;

		if( npc->direct == DIR_RIGHT ){
			if( npc->x > VS*PARTSSIZE*45 ){
				npc->direct = DIR_LEFT;
				npc->act_no = 100;
			}
		}else{
			if( npc->x < VS*PARTSSIZE*17 ){
				npc->direct = DIR_RIGHT;
				npc->act_no = 100;
			}
		}
		// レーザー
		if( npc->direct == DIR_LEFT ){
			npc->act_wait++;
			if( npc->act_wait == 50*6 ||
				npc->act_wait == 50*6+10 ||
				npc->act_wait == 50*6+20 ){
				PlaySoundObject( WAVE_POP, 1 );
				SetNpChar( 198, npc->x+10*VS, npc->y+VS, Random(-3,0)*VS, Random(-3,3)*VS, DIR_RIGHT, 0, MAX_NPC/2 );
			}
		}

		// アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 7 )
			npc->ani_no = 0;
		
		break;

	case 1000:// 退去
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->ani_no = 8;
		npc->damage = 0;
		npc->act_no = 1001;
		npc->tgt_x = npc->x;
		npc->tgt_y = npc->y;
		SetQuake( 20 );
		for( i = 0; i < 32; i++ ){
			SetNpChar( 4,
				npc->x + Random( -128, 128 )*VS,
				npc->y + Random(  -64,  64 )*VS,
				Random( -VS/4, VS/4 ) *VS,
				Random( -VS/4, VS/4 ) *VS,
				DIR_LEFT, NULL, MAX_NPC/2 ); 
		}
		DeleteNpCharCode( 197, TRUE );
		DeleteNpCharCode( 271, TRUE );
		DeleteNpCharCode( 272, TRUE );
	case 1001:
		npc->tgt_x -= VS;
		npc->x = npc->tgt_x + Random( -1, 1 )*VS;
		npc->y = npc->tgt_y + Random( -1, 1 )*VS;
		npc->act_wait++;
		if( !(npc->act_wait%4) )
			SetNpChar( 4,
				npc->x + Random( -128, 128 )*VS,
				npc->y + Random(  -64,  64 )*VS,
				Random( -VS/4, VS/4 ) *VS,
				Random( -VS/4, VS/4 ) *VS,
				DIR_LEFT, NULL, MAX_NPC/2 ); 
		break;

	}

	RECT rc[] = {
		{  0,  0, 64, 24},
		{ 64,  0,128, 24},
		{128,  0,192, 24},
		{ 64,  0,128, 24},

		{  0,  0, 64, 24},
		{192,  0,256, 24},
		{256,  0,320, 24},
		{192,  0,256, 24},

		{256, 48,320, 72},
	};

	RECT rcDamage[] = {
		{  0, 24, 64, 48},
		{ 64, 24,128, 48},
		{128, 24,192, 48},
		{ 64, 24,128, 48},

		{  0, 24, 64, 48},
		{192, 24,256, 48},
		{256, 24,320, 48},
		{192, 24,256, 48},

		{256, 48,320, 72},
	};

	if( npc->shock ){
		_flash++;
		if( _flash/2%2 ) npc->rect = rc[      npc->ani_no];
		else             npc->rect = rcDamage[npc->ani_no];
	}else{
		npc->rect = rc[npc->ani_no];
	}
}


