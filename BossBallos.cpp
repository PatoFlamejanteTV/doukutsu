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
void SetQuake(  long time );
void SetQuake2( long time );
void ChangeAllNpCharByCode( long code1, long code2 );

int GetSin( unsigned char deg );
int GetCos( unsigned char deg );
unsigned char GetArktan( long x,long y );

void SetDestroyNpChar( long x, long y, long w, long num );
void DeleteNpCharCode( long code, BOOL bSmoke );

void SetFlash( long x, long y, long mode );
BOOL ChangeMapParts( long x, long y, unsigned char no );
long CountArmsBullet( long arms_code );
//BOOL IsActiveEdgeBullet( void );

void DamageMyChar( long damage );

// 0 dum
// 1 eye
// 2 eye
// 3 body
// 4 Šz@-44
// 5 •   

#define BOSSLIFE 800

static void _ActBossChar_Eye( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{272, 0,296, 16},
		{272,16,296, 32},
		{272,32,296, 48},
		{  0, 0,  0,  0},
		{240,16,264, 32},//•
	};
	RECT rcRight[] = {
		{296, 0,320, 16},
		{296,16,320, 32},
		{296,32,320, 48},
		{  0, 0,  0,  0},
		{240,32,264, 48},
	};



	switch( npc->act_no ){
	case 0:
		break;
	case 100:// ŠJŠá
		npc->act_no   = 101;
		npc->ani_no   =  0;
		npc->ani_wait =  0;
	case 101:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 ){
			npc->act_no = 102;
		}
		break;
	case 102:
		npc->ani_no = 3;
		break;
	case 200:// •Â
		npc->act_no   = 201;
		npc->ani_no   =   3;
		npc->ani_wait =   0;
	case 201:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no--;
		}
		if( npc->ani_no <= 0 ){
			npc->act_no = 202;
		}
		break;
	case 202:
		break;

	case 300:
		npc->act_no = 301;
		npc->ani_no =   4;
		if( npc->direct == DIR_LEFT ) SetDestroyNpChar( npc->x - 4*VS, npc->y, 4*VS, 10 );
		else                          SetDestroyNpChar( npc->x + 4*VS, npc->y, 4*VS, 10 );
	case 301:
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->x = gBoss[0].x - 24 * VS;
	else                          npc->x = gBoss[0].x + 24 * VS;
	npc->y = gBoss[0].y-36*VS;

	if( npc->act_no >= 0 && npc->act_no < 300 ){
		if( 3 != npc->ani_no ){
			npc->bits &= ~BITS_BANISH_DAMAGE;
		}else{
//			if( IsActiveEdgeBullet() ) npc->bits  &= ~BITS_BANISH_DAMAGE;
//			else
				npc->bits  |=  BITS_BANISH_DAMAGE;
		}
		
	}
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[  npc->ani_no ];
	else                          npc->rect = rcRight[ npc->ani_no ];
}

static void _ActBossChar_Body( NPCHAR *npc )
{
	RECT rc[] = {
		{  0,  0,120,120},
		{120,  0,240,120},
		{  0,120,120,240},
		{120,120,240,240},
	};
	npc->x = gBoss[0].x;
	npc->y = gBoss[0].y;
	
	npc->rect = rc[ npc->ani_no ];
}

static void _ActBossChar_HITAI( NPCHAR *npc )
{
	npc->x = gBoss[0].x;
	npc->y = gBoss[0].y-44*VS;
}
static void _ActBossChar_HARA( NPCHAR *npc )
{
	npc->x = gBoss[0].x;
	npc->y = gBoss[0].y;
}

//
void ActBossChar_Ballos( void )
{
	NPCHAR *npc;
	static unsigned char _flash;
	long i;
	long x, y;

	npc = &gBoss[0];

	switch( npc->act_no ){
	case 0:// ‰ŠúÝ’è
		npc->act_no          = 1;
		npc->cond            = COND_ALIVE;
		npc->exp             = 1;
		npc->direct          = DIR_LEFT;
		npc->x               = 20*VS*PARTSSIZE;
		npc->y               = -4*VS*PARTSSIZE;
		npc->hit_voice       = WAVE_NPC_GOHST;
		npc->hit.front       = 32*VS;
		npc->hit.top         = 48*VS;
		npc->hit.back        = 32*VS;
		npc->hit.bottom      = 48*VS;
		npc->bits            = BITS_THROW_BLOCK|BITS_EVENT_BREAK|BITS_BLOCK_MYCHAR2|BITS_VIEWDAMAGE;
		npc->size            = NPCSIZE_LARGE;
		npc->damage          =    0;
		npc->code_event      = 1000;
		npc->life            =  BOSSLIFE;//800;

		// –Ú
		gBoss[1].cond        = COND_ALIVE|COND_ZEROINDEXDAMAGE;
		gBoss[1].direct      = DIR_LEFT;
		gBoss[1].bits        = BITS_THROW_BLOCK;
		gBoss[1].life        = 10000;
		gBoss[1].view.front  = 12*VS;
		gBoss[1].view.top    =  0*VS;
		gBoss[1].view.back   = 12*VS;
		gBoss[1].view.bottom = 16*VS;
		gBoss[1].hit.front   = 12*VS;
		gBoss[1].hit.top     =  0*VS;
		gBoss[1].hit.back    = 12*VS;
		gBoss[1].hit.bottom  = 16*VS;
		
		gBoss[2]             = gBoss[1];
		gBoss[2].direct      = DIR_RIGHT;

		// ‘Ì
		gBoss[3].cond        = COND_ALIVE|COND_ZEROINDEXDAMAGE;;
		gBoss[3].bits        = BITS_THROW_BLOCK|BITS_BLOCK_MYCHAR|BITS_BLOCK_BULLET;
		gBoss[3].view.front  = 60*VS;
		gBoss[3].view.top    = 60*VS;
		gBoss[3].view.back   = 60*VS;
		gBoss[3].view.bottom = 60*VS;
		gBoss[3].hit.front   = 48*VS;
		gBoss[3].hit.top     = 24*VS;
		gBoss[3].hit.back    = 48*VS;
		gBoss[3].hit.bottom  = 32*VS;

		gBoss[4].cond        = COND_ALIVE|COND_ZEROINDEXDAMAGE;;
		gBoss[4].bits        = BITS_THROW_BLOCK|BITS_BLOCK_MYCHAR|BITS_BLOCK_BULLET;
		gBoss[4].hit.front   = 32*VS;
		gBoss[4].hit.top     =  8*VS;
		gBoss[4].hit.back    = 32*VS;
		gBoss[4].hit.bottom  =  8*VS;

		gBoss[5].cond        = COND_ALIVE|COND_ZEROINDEXDAMAGE;;
		gBoss[5].bits        = BITS_THROW_BLOCK|BITS_BLOCK_MYCHAR2|BITS_BLOCK_BULLET;
		gBoss[5].hit.front   = 32*VS;
		gBoss[5].hit.top     =  0*VS;
		gBoss[5].hit.back    = 32*VS;
		gBoss[5].hit.bottom  = 48*VS;


	case 1:// ‘Ò‹@
		break;
		
	case 100:// —Ž‰º“oê --------------------------------
		npc->act_no   = 101;
		npc->ani_no   =   0;
		npc->x        = gMC.x;
		SetNpChar( 333, gMC.x, 19*VS*PARTSSIZE, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
		npc->act_wait =   0;
	case 101:
		npc->act_wait++;
		if( npc->act_wait > 30 ) npc->act_no = 102;
		break;
	case 102:
		npc->ym += VS/8;
		if( npc->ym > 6*VS ) npc->ym = 6*VS;
		npc->y  += npc->ym;
		if( npc->y > 19*VS*PARTSSIZE - npc->hit.bottom ){
			npc->y = 19*VS*PARTSSIZE - npc->hit.bottom;
			npc->ym       =   0;
			npc->act_no   = 103;
			npc->act_wait =   0;
			SetQuake2( 30 );
			PlaySoundObject( WAVE_BOM, 1 );
			// “¥‚Ý’×‚µƒ_ƒ[ƒW
			if( gMC.y > npc->y + 48*VS &&
				gMC.x < npc->x + 24*VS &&
				gMC.x > npc->x - 24*VS ) DamageMyChar( 16 );
			for( i = 0; i < 16; i++ ){
				x =npc->x +  Random( -40, 40 ) * VS;
				SetNpChar( 4, x, npc->y + 40*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			}
			if( gMC.flag & FLAG_HIT_BOTTOM ) gMC.ym = - VS;
		}
		break;
	case 103:
		npc->act_wait++;
		if( npc->act_wait ==  50 ){
			npc->act_no     = 104;
			gBoss[1].act_no = 100;
			gBoss[2].act_no = 100;
		}
		break;
	case 104:
		break;
		
	case 200:// ˜A‘±“¥‚Ý’×‚µ ------------------------
		npc->act_no = 201;
		npc->count1 =   0;
	case 201:
		npc->act_no     = 203;
		npc->xm         =   0;
		npc->count1++;
		npc->hit.bottom = 48*VS;
		npc->damage     =  0;
		if( npc->count1%3 == 0 ) npc->act_wait = 150;
		else                     npc->act_wait =  50;
	case 203:
		npc->act_wait--;
		if( npc->act_wait <= 0 ){
			npc->act_no = 204;
			npc->ym     = -VS * 6;
			if( npc->x < gMC.x ) npc->xm = VS *  1;
			else                 npc->xm = VS * -1;
		}
		break;
	case 204:
	
		if( npc->x <  2*VS*PARTSSIZE + 48*VS ) npc->xm =   1 * VS;
		if( npc->x > 37*VS*PARTSSIZE - 48*VS ) npc->xm = - 1 * VS;
		npc->ym += VS/6;
		if( npc->ym > 6*VS ) npc->ym = 6*VS;
		npc->x  += npc->xm;
		npc->y  += npc->ym;
		if( npc->y > 19*VS*PARTSSIZE - npc->hit.bottom ){
			npc->y = 19*VS*PARTSSIZE - npc->hit.bottom;
			npc->ym       =   0;
			npc->act_no   = 201;
			npc->act_wait =   0;

			// “¥‚Ý’×‚µƒ_ƒ[ƒW
			if( gMC.y > npc->y + 56*VS     ) DamageMyChar( 16 );
			if( gMC.flag & FLAG_HIT_BOTTOM ) gMC.ym = - VS;

			SetQuake2( 30 );
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetNpChar( 332, npc->x -12*VS, npc->y+52*VS, 0, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
			SetNpChar( 332, npc->x +12*VS, npc->y+52*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_BOM, 1 );
			for( i = 0; i < 16; i++ ){
				x =npc->x +  Random( -40, 40 ) * VS;
				SetNpChar( 4, x, npc->y + 40*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
		break;

	case 220://‚â‚ç‚ê—Ž‰º
		npc->act_no     =  221;
		npc->life       = 1200;
		gBoss[1].act_no =  200;//close
		gBoss[2].act_no =  200;//close
//		npc->ym         =    0;
		npc->xm         =    0;
		npc->ani_no     =    0;
		npc->shock      =    0;
		_flash          =    0;
	case 221:
		npc->ym += VS/8;
		if( npc->ym > 6*VS ) npc->ym = 6*VS;
		npc->y  += npc->ym;
		if( npc->y > 19*VS*PARTSSIZE - npc->hit.bottom ){
			npc->y = 19*VS*PARTSSIZE - npc->hit.bottom;
			npc->ym       =   0;
			npc->act_no   = 222;
			npc->act_wait =   0;
			SetQuake2( 30 );
			PlaySoundObject( WAVE_QUAKE, 1 );
			for( i = 0; i < 16; i++ ){
				x =npc->x +  Random( -40, 40 ) * VS;
				SetNpChar( 4, x, npc->y + 40*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			}
			if( gMC.flag & FLAG_HIT_BOTTOM ) gMC.ym = - VS;
		}
		break;
	case 222:
		break;


	case 300:// ‚±‚ë‚ª‚è -------------------------------------------
		npc->act_no   = 301;
		npc->act_wait =   0;
		for( i = 0; i < 256; i += 64 ){
			SetNpChar( 342, npc->x, npc->y, 0, 0, i    + 0x100 * DIR_LEFT,  npc, 90 );
			SetNpChar( 342, npc->x, npc->y, 0, 0, i+32 + 0x100 * DIR_RIGHT, npc, 90 );
		}
		SetNpChar( 343, npc->x,       npc->y,       0, 0, DIR_LEFT,  npc, 24 );
		SetNpChar( 344, npc->x-24*VS, npc->y-36*VS, 0, 0, DIR_LEFT,  npc, 32 );
		SetNpChar( 344, npc->x+24*VS, npc->y-36*VS, 0, 0, DIR_RIGHT, npc, 32 );

	case 301:// ƒT[ƒNƒ‹‘•’…/•‚—V
		npc->y += ( (19*VS*PARTSSIZE - 79*VS) - npc->y) / 8;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 310;
			npc->act_wait =   0;
		}
		break;
	case 310:
		break;
	case 311:// ¶
		npc->direct = DIR_LEFT;
		npc->xm     = -(VS + VS*5/6);
		npc->ym     = 0;
		npc->x      += npc->xm;
		if( npc->x <  2*VS*PARTSSIZE + 79*VS ){
			npc->x =  2*VS*PARTSSIZE + 79*VS;
			npc->act_no = 312;
		}
		break;
	case 312:// ã
		npc->direct = DIR_UP;
		npc->ym     = -(VS + VS*5/6);
		npc->xm     = 0;
		npc->y     += npc->ym;
		if( npc->y <  2*VS*PARTSSIZE + 79*VS ){
			npc->y =  2*VS*PARTSSIZE + 79*VS;
			npc->act_no = 313;
		}
		break;
	case 313:// ‰E
		npc->direct = DIR_RIGHT;
		npc->xm     =  (VS + VS*5/6);
		npc->ym     = 0;
		npc->x     += npc->xm;
		if( npc->x > 37*VS*PARTSSIZE - 79*VS ){
			npc->x = 37*VS*PARTSSIZE - 79*VS;
			npc->act_no = 314;
		}
		if( npc->count1 ) npc->count1--;
		if( !npc->count1 &&
			npc->x > 19*VS*PARTSSIZE &&
			npc->x < 21*VS*PARTSSIZE ){
			npc->act_no = 400;
		}
		break;
	case 314:// ‰º
		npc->direct = DIR_DOWN;
		npc->ym     =  (VS + VS*5/6);
		npc->xm     = 0;
		npc->y     += npc->ym;
		if( npc->y > 19*VS*PARTSSIZE - 79*VS ){
			npc->y = 19*VS*PARTSSIZE - 79*VS;
			npc->act_no = 311;
		}
		break;

	case 400:// •‚—V -------------------------------------------
		npc->act_no   = 401;
		npc->act_wait =   0;
		npc->xm       =   0;
		npc->ym       =   0;
//		npc->life       = BOSSLIFE;
		DeleteNpCharCode( 339, FALSE );
	case 401:// ^’†‚Ö
		npc->y += ( ( 5*VS*PARTSSIZE + 79*VS) - npc->y) / 8;
		npc->act_wait++;
		if( npc->act_wait >  50 ){
			npc->act_wait =   0;
			npc->act_no   = 410;
//			for( i = 0; i < 256; i += 43 ){ SetNpChar( 346, npc->x, npc->y, 0, 0, i, npc, 80 ); }
//			for( i = 0; i < 256; i += 37 ){ SetNpChar( 346, npc->x, npc->y, 0, 0, i, npc, 80 ); }
			for( i = 0; i < 256; i += 32 ){ SetNpChar( 346, npc->x, npc->y, 0, 0, i, npc, 80 ); }
//			for( i = 0; i < 256; i += 29 ){ SetNpChar( 346, npc->x, npc->y, 0, 0, i, npc, 80 ); }
			SetNpChar( 343, npc->x,       npc->y,       0, 0, DIR_LEFT,  npc, 24 );
			SetNpChar( 344, npc->x-24*VS, npc->y-36*VS, 0, 0, DIR_LEFT,  npc, 32 );
			SetNpChar( 344, npc->x+24*VS, npc->y-36*VS, 0, 0, DIR_RIGHT, npc, 32 );
		}
		break;
	case 410:// ƒuƒƒbƒN€”õ
		npc->act_wait++;
		if( npc->act_wait >  50 ){
			npc->act_wait   =   0;
			npc->act_no     = 411;
		}
		break;
	case 411:// ƒT[ƒNƒ‹‹z’…^
		npc->act_wait++;
		if( npc->act_wait%30 == 1 ){
			x = (2 + (npc->act_wait/30)*2)*PARTSSIZE*VS;
			SetNpChar( 348, x, 21*VS*PARTSSIZE, 0, 0, 0, NULL, MAX_NPC*3/4 );
		}
		if( npc->act_wait/3%2 ) PlaySoundObject( WAVE_QUAKE, 1 );
		if( npc->act_wait > 18*30 ){
			npc->act_no = 420;
		}
		break;

	case 420:// ŠJŠáŠJŽn ===================================
		npc->act_no   = 421;
		npc->act_wait =   0;
		npc->ani_wait =   0;
		SetQuake2( 30 );
		PlaySoundObject( WAVE_EXPLOSION, 1 );
		gBoss[1].act_no = 102;//open
		gBoss[2].act_no = 102;//open
		for( i = 0; i < 256; i++ ){
			x = npc->x + Random( -60, 60 )*VS;
			y = npc->y + Random( -60, 60 )*VS;
			SetNpChar( 4, x, y, 0, 0, DIR_LEFT, NULL, 0 );
		}

	case 421:// ‰E‰ñ“]‚Q
		if( ++npc->ani_wait > 10*50 ){ npc->ani_wait = 0; npc->act_no = 422; }
		break;
	case 422:// ‰E‰ñ“]‚P
		if( ++npc->ani_wait >  4*50 ){ npc->ani_wait = 0; npc->act_no = 423; }
		break;
	case 423:// ’âŽ~
		if( ++npc->ani_wait >    20 ){ npc->ani_wait = 0; npc->act_no = 424; }
		break;
	case 424:// ¶‰ñ“]‚P
		if( ++npc->ani_wait >  4*50 ){ npc->ani_wait = 0; npc->act_no = 425; }
		break;
	case 425:// ¶‰ñ“]‚Q
		if( ++npc->ani_wait > 10*50 ){ npc->ani_wait = 0; npc->act_no = 426; }
		break;
	case 426:// ¶‰ñ“]‚P
		if( ++npc->ani_wait >  4*50 ){ npc->ani_wait = 0; npc->act_no = 427; }
		break;
	case 427:// ’âŽ~
		if( ++npc->ani_wait >    20 ){ npc->ani_wait = 0; npc->act_no = 428; }
		break;
	case 428:// ‰E‰ñ“]‚P
		if( ++npc->ani_wait >  4*50 ){ npc->ani_wait = 0; npc->act_no = 421; }
		break;
		
	// ‚â‚ç‚ê =========================================
	case 1000:
		npc->act_no     = 1001;
		npc->act_wait   =    0;
		gBoss[1].act_no =  300;//break
		gBoss[2].act_no =  300;//break
		
		gBoss[1].act_no &=  ~(BITS_BLOCK_MYCHAR|BITS_BLOCK_MYCHAR2);
		gBoss[2].act_no &=  ~(BITS_BLOCK_MYCHAR|BITS_BLOCK_MYCHAR2);
		gBoss[0].bits   &=  ~(BITS_BLOCK_MYCHAR|BITS_BLOCK_MYCHAR2);
		gBoss[3].bits   &=  ~(BITS_BLOCK_MYCHAR|BITS_BLOCK_MYCHAR2);
		gBoss[4].bits   &=  ~(BITS_BLOCK_MYCHAR|BITS_BLOCK_MYCHAR2);
		gBoss[5].bits   &=  ~(BITS_BLOCK_MYCHAR|BITS_BLOCK_MYCHAR2);
		
	case 1001:
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait%12 == 0 )
			PlaySoundObject( WAVE_BOM, 1 );
			SetDestroyNpChar( gBoss[0].x + Random(-60,60)*VS,
							  gBoss[0].y + Random(-60,60)*VS, 1,  1 );
		if( gBoss[0].act_wait > 50*3 ){
			gBoss[0].act_wait =  0;
			gBoss[0].act_no   = 1002;
			SetFlash( gBoss[0].x, gBoss[0].y, FLASHMODE_EXPLOSION );
			PlaySoundObject( WAVE_EXPLOSION, 1 );
		}
		break;
	case 1002:
		SetQuake2( 40 );
		gBoss[0].act_wait++;
		if( gBoss[0].act_wait == 50 ){
			gBoss[0].cond = 0;
			gBoss[1].cond = 0;
			gBoss[2].cond = 0;
			gBoss[3].cond = 0;
			gBoss[4].cond = 0;
			gBoss[5].cond = 0;
			DeleteNpCharCode( 350, TRUE );
			DeleteNpCharCode( 348, TRUE );
		}
		break;
	}
	
	// ÅIŒ`‘Ô‚ÌƒAƒNƒVƒ‡ƒ“
	if( npc->act_no > 420 && npc->act_no < 500 ){
//		if( IsActiveEdgeBullet() ){
//			gBoss[3].bits  &= ~BITS_BANISH_DAMAGE;
//			gBoss[4].bits  &= ~BITS_BANISH_DAMAGE;
//			gBoss[5].bits  &= ~BITS_BANISH_DAMAGE;
//		}else{
			gBoss[3].bits  |=  BITS_BANISH_DAMAGE;
			gBoss[4].bits  |=  BITS_BANISH_DAMAGE;
			gBoss[5].bits  |=  BITS_BANISH_DAMAGE;
//		}
		npc->act_wait++;
		if( npc->act_wait > 300 ){
			npc->act_wait = 0;
			if( gMC.x > npc->x ){
				for( i = 0; i < 8; i++ ){
					x = ( Random( -1*4, 1*4 ) + 39*4 ) * VS * PARTSSIZE / 4;
					y =   Random( 2*4, 17*4 )          * VS * PARTSSIZE / 4;
					SetNpChar( 350, x, y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
				}
			}else{
				for( i = 0; i < 8; i++ ){
					x = ( Random( -1*4, 1*4 ) +  0*4 ) * VS * PARTSSIZE /4;
					y =   Random( 2*4, 17*4 )          * VS * PARTSSIZE /4;
					SetNpChar( 350, x, y, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
				}
			}
		}
		if( npc->act_wait == 270 || npc->act_wait == 280 || npc->act_wait == 290 ){
			SetNpChar( 353, npc->x, npc->y - 52*VS, 0, 0, DIR_UP, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x, npc->y - 52*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
		if( npc->life > 500 ){
			if( Random( 0, 10 ) == 2 ){
				x = npc->x + Random( -40, 40 ) * VS;
				y = npc->y + Random(   0, 40 ) * VS;
				SetNpChar( 270, x, y, 0, 0, DIR_DOWN, NULL, 0 );
			}
		}else{
			if( Random( 0, 4 ) == 2 ){
				x = npc->x + Random( -40, 40 ) * VS;
				y = npc->y + Random(   0, 40 ) * VS;
				SetNpChar( 270, x, y, 0, 0, DIR_DOWN, NULL, 0 );
			}
		}
	}

	// ƒtƒ‰ƒbƒVƒ…
	if( npc->shock ){
		_flash++;
		if( _flash/2%2 ) gBoss[3].ani_no = 1;
		else             gBoss[3].ani_no = 0;
	}else{
		gBoss[3].ani_no = 0;
	}
	if( npc->act_no > 420 ) gBoss[3].ani_no += 2;

	_ActBossChar_Eye(    &gBoss[1] );
	_ActBossChar_Eye(    &gBoss[2] );
	_ActBossChar_Body(   &gBoss[3] );
	_ActBossChar_HITAI(  &gBoss[4] );
	_ActBossChar_HARA(   &gBoss[5] );

}

