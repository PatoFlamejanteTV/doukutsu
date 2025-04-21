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

void SetNoise( long no, long freq );
void CutNoise( void );

extern long gCurlyShoot_wait;
extern long gCurlyShoot_x;
extern long gCurlyShoot_y;

extern long gSuperYpos;
// 0 メイン
// 1 サブ
// 2 サブ
// 3 サブ

// 4 顔
// 5 尻


// 6 サブ
// 7 サブ

//  8 甲羅
//  9 甲羅
// 10 甲羅
// 11 コア


// 顔
void ActBossChar_Core_Face( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,  0, 72,112},
		{  0,112, 72,224},
		{160,  0,232,112},
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
		npc->x          = gBoss[0].x - VS*36;
		npc->y          = gBoss[0].y;
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

void ActBossChar_Core_Tail( NPCHAR *npc )
{
	RECT rect[] = {
		{  72,  0, 160,112},
		{  72,112, 160,224},
		{  0,  0,  0,  0},
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
		npc->x          = gBoss[0].x + VS*44;
		npc->y          = gBoss[0].y;
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


void ActBossChar_Core_Mini( NPCHAR *npc )
{
	RECT rect[] = {
		{256,  0,320, 40},
		{256, 40,320, 80},
		{256, 80,320,120}, //閉じ
	};

	long xm, ym;
	unsigned char deg;

	npc->life = 1000;

	switch( npc->act_no ){
	case   0:
		break;
	case  10:// 閉じ待機
		npc->ani_no = 2;
		npc->bits &= ~BITS_BANISH_DAMAGE;
		break;

	case 100:// ショック閉じ
		npc->act_no   = 101;
		npc->ani_no   = 2;
		npc->act_wait = 0;
		npc->tgt_x  = gBoss[0].x + Random( -128, 32 )*VS; 
		npc->tgt_y  = gBoss[0].y + Random(  -64, 64 )*VS; 
		npc->bits |= BITS_BANISH_DAMAGE;
	case 101:
		npc->x += ( npc->tgt_x - npc->x )/16;
		npc->y += ( npc->tgt_y - npc->y )/16;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->ani_no = 0;
		}
		break;

	case 120:// 点滅
		npc->act_no   = 121;
		npc->act_wait =   0;
	case 121:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 0;
		else                    npc->ani_no = 1;
		if( npc->act_wait > 20 ){
			npc->act_no = 130;
		}
		break;
	
	case 130:// 閉じダッシュ
		npc->act_no   = 131;
		npc->ani_no   =   2;
		npc->act_wait =   0;
		npc->tgt_x  = npc->x + Random( 24, 48 )*VS; 
		npc->tgt_y  = npc->y + Random( -4,  4 )*VS; 
	case 131:
		npc->x += ( npc->tgt_x - npc->x )/16;
		npc->y += ( npc->tgt_y - npc->y )/16;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no = 140;
			npc->ani_no = 0;
		}

		if( npc->act_wait == 1 || npc->act_wait == 3 ){
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -2, 2 );

			ym = GetSin( deg ) * 2;
			xm = GetCos( deg ) * 2;
			SetNpChar( 178, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
		}		
		break;

	case 140:// 開き…
		npc->x += ( npc->tgt_x - npc->x )/16;
		npc->y += ( npc->tgt_y - npc->y )/16;
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

	if( npc->shock ){
		npc->tgt_x += VS*2;
	}

	npc->rect = rect[npc->ani_no];

}


void ActBossChar_Core_Hit( NPCHAR *npc )
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

void ActBossChar_Core( void )
{
	NPCHAR *npc;
	npc = &gBoss[0];
	static unsigned char _flash = 0;
	BOOL bShock;
	long i;
	unsigned char deg;
	long xm, ym;

	bShock = FALSE;

	switch( npc->act_no ){
	case 0:
		npc->act_no    = 10;
		npc->exp       = 1;
		npc->cond      = COND_ALIVE;
		npc->bits      = BITS_THROW_BLOCK|BITS_BLOCK_BULLET|BITS_VIEWDAMAGE;
		npc->life      = 650;
//		npc->life      = 1;
		npc->hit_voice = WAVE_ALMONDDMG;

		npc->x         = VS*PARTSSIZE*77;
		npc->y         = VS*PARTSSIZE*14;
		npc->xm        = 0;
		npc->ym        = 0;

		npc->code_event      = 1000;
		npc->bits           |= BITS_EVENT_BREAK;

		// 顔と尻
		gBoss[ 4].cond       = COND_ALIVE;
		gBoss[ 4].act_no     = 10;
		gBoss[ 5].cond       = COND_ALIVE;
		gBoss[ 5].act_no     = 10;

		// HIT
		gBoss[ 8].cond       = COND_ALIVE;
		gBoss[ 8].bits       = BITS_THROW_BLOCK|BITS_BLOCK_BULLET;
		gBoss[ 8].view.front =  0*VS;
		gBoss[ 8].view.top   =  0*VS;
		gBoss[ 8].hit.back   = 40*VS;
		gBoss[ 8].hit.top    = 16*VS;
		gBoss[ 8].hit.bottom = 16*VS;
		gBoss[ 8].count1     = 0;

		gBoss[ 9]            = gBoss[8];
		gBoss[ 9].hit.back   = 36*VS;
		gBoss[ 9].hit.top    = 24*VS;
		gBoss[ 9].hit.bottom = 24*VS;
		gBoss[ 9].count1     = 1;

		gBoss[10]            = gBoss[8];
		gBoss[10].hit.back   = 44*VS;
		gBoss[10].hit.top    = 8*VS;
		gBoss[10].hit.bottom = 8*VS;
		gBoss[10].count1     = 2;

		gBoss[11]            = gBoss[8];
		gBoss[11].cond      |= COND_ZEROINDEXDAMAGE;
		gBoss[11].hit.back   = 20*VS;
		gBoss[11].hit.top    = 20*VS;
		gBoss[11].hit.bottom = 20*VS;
		gBoss[11].count1     = 3;

		// Mini
		gBoss[1].cond        = COND_ALIVE;
		gBoss[1].act_no      = 10;
		gBoss[1].bits        = BITS_THROW_BLOCK|BITS_BLOCK_BULLET|BITS_BANISH_DAMAGE;
		gBoss[1].life        = 1000;
		gBoss[1].hit_voice   = WAVE_NPC_GOHST;
		gBoss[1].hit.back    = 24*VS;
		gBoss[1].hit.top     = 16*VS;
		gBoss[1].hit.bottom  = 16*VS;
		gBoss[1].view.front  = 32*VS;
		gBoss[1].view.top    = 20*VS;
		gBoss[1].x           = npc->x - 8*VS;
		gBoss[1].y           = npc->y -64*VS;

		gBoss[2]        = gBoss[1];
		gBoss[2].x      = npc->x +16*VS;
		gBoss[2].y      = npc->y - 0*VS;
		gBoss[3]        = gBoss[1];
		gBoss[3].x      = npc->x - 8*VS;
		gBoss[3].y      = npc->y +64*VS;
		gBoss[6]        = gBoss[1];
		gBoss[6].x      = npc->x -48*VS;
		gBoss[6].y      = npc->y -32*VS;
		gBoss[7]        = gBoss[1];
		gBoss[7].x      = npc->x -48*VS;
		gBoss[7].y      = npc->y +32*VS;


	case 10:
		break;

	case 200: // 閉じている ----------
		npc->act_no   = 201;
		npc->act_wait = 0;
		gBoss[11].bits &= ~BITS_BANISH_DAMAGE;
		gSuperYpos    = 0;
		CutNoise( );
	case 201:
		npc->tgt_x = gMC.x;
		npc->tgt_y = gMC.y;
		npc->act_wait++;
		if( npc->act_wait > 50*8 ){
			npc->count1++;
			PlaySoundObject( WAVE_COREOPEN, 1 );
			if( npc->count1 > 3 ){
				npc->count1 = 0;
				// 閉じる
				npc->act_no = 220;
				gBoss[4].ani_no = 0;
				gBoss[5].ani_no = 0;
				bShock = TRUE;

			}else{
				// 開く
				npc->act_no = 210;
				gBoss[4].ani_no = 0;
				gBoss[5].ani_no = 0;
				bShock = TRUE;
			}

		}
		break;

	case 210: // 開いている ----------
		npc->act_no   = 211;
		npc->act_wait =   0;
		npc->count2   = npc->life;
		gBoss[11].bits |= BITS_BANISH_DAMAGE;
	case 211:
		npc->tgt_x = gMC.x;
		npc->tgt_y = gMC.y;
		if( npc->shock ){
			_flash++;
			if( _flash/2%2 ){
				gBoss[4].ani_no = 0;
				gBoss[5].ani_no = 0;
			}else{
				gBoss[4].ani_no = 1;
				gBoss[5].ani_no = 1;
			}
			
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



		if( npc->act_wait < 50*4 && npc->act_wait%20 == 1 ){
			SetNpChar( 179,
				npc->x + Random( -48, -16 ) * VS,
				npc->y + Random( -64,  64 ) * VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		}
		if( npc->act_wait > 50*8 || npc->life < npc->count2 - 200 ){

			// 閉じる
			npc->act_no = 200;
			gBoss[4].ani_no = 2;
			gBoss[5].ani_no = 0;
			bShock = TRUE;
		}

		break;

	case 220: // 水流  開いている ----------
		npc->act_no   = 221;
		npc->act_wait =   0;
		gSuperYpos    = 1;
		gBoss[11].bits |= BITS_BANISH_DAMAGE;
		SetQuake( 50*2 );
		SetNoise( NOISE_RIVER, 1000 );
	case 221:
		npc->act_wait++;
		SetNpChar( 199, gMC.x + Random( -50, 150 )*VS*2, gMC.y + Random( -160, 160 )*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 ); 
		gMC.xm -= (VS/16);
		gMC.cond |= COND_FLOW;

		if( npc->shock ){
			_flash++;
			if( _flash/2%2 ){
				gBoss[4].ani_no = 0;
				gBoss[5].ani_no = 0;
			}else{
				gBoss[4].ani_no = 1;
				gBoss[5].ani_no = 1;
			}
			
		}else{
				gBoss[4].ani_no = 0;
				gBoss[5].ani_no = 0;
		}

		if( npc->act_wait == 50*6 || npc->act_wait == 50*7|| npc->act_wait == 50*8 ){

			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			ym = GetSin( deg ) * 3;
			xm = GetCos( deg ) * 3;
			SetNpChar( 218, npc->x - 40*VS, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_THUNDER, 1 );
		}
		
		if( npc->act_wait > 50*8 ){

			// 閉じる
			npc->act_no = 200;
			gBoss[4].ani_no = 2;
			gBoss[5].ani_no = 0;
			bShock = TRUE;
		}
		
		break;

	case 500:// 煙を吹く
		CutNoise( );
		npc->act_no     = 501;
		npc->act_wait   =   0;
		npc->xm         =   0;
		npc->ym         =   0;
		gBoss[4].ani_no =   2;
		gBoss[5].ani_no =   0;
		gBoss[1].act_no = 200;
		gBoss[2].act_no = 200;
		gBoss[3].act_no = 200;
		gBoss[6].act_no = 200;
		gBoss[7].act_no = 200;
		SetQuake( 20 );
		for( i = 0; i < 32; i++ ){
			SetNpChar( 4,
				npc->x + Random( -128, 128 )*VS,
				npc->y + Random(  -64,  64 )*VS,
				Random( -VS/4, VS/4 ) *VS,
				Random( -VS/4, VS/4 ) *VS,
				DIR_LEFT, NULL, MAX_NPC/2 ); 
		}
		for( i = 0; i < 12; i++ ){
			gBoss[i].bits &= ~(BITS_BLOCK_BULLET|BITS_BANISH_DAMAGE);
		}
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
		if( npc->act_wait/2%2 ) npc->x -= VS;
		else                    npc->x += VS;

		if( npc->x < 63*VS*PARTSSIZE ) npc->x += VS/4;
		else                           npc->x -= VS/4;
		if( npc->y < 11*VS*PARTSSIZE ) npc->y += VS/4;
		else                           npc->y -= VS/4;

		break;

	case 600:// フェードアウト
		npc->act_no = 601;
		gBoss[4].act_no =  50;
		gBoss[5].act_no =  50;

		gBoss[ 8].bits &= ~BITS_BLOCK_BULLET;
		gBoss[ 9].bits &= ~BITS_BLOCK_BULLET;
		gBoss[10].bits &= ~BITS_BLOCK_BULLET;
		gBoss[11].bits &= ~BITS_BLOCK_BULLET;

	case 601:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->x -= VS*4;
		else                    npc->x += VS*4;
		break;
	}

	if( bShock ){
		SetQuake( 20 );
		gBoss[1].act_no = 100;
		gBoss[2].act_no = 100;
		gBoss[3].act_no = 100;
		gBoss[6].act_no = 100;
		gBoss[7].act_no = 100;
		PlaySoundObject( WAVE_QUAKE, 1 );
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4,
				gBoss[4].x + Random( -32, 16 ) * VS, gBoss[4].y,
				Random(-VS,VS),
				Random(-VS/2,VS/2), DIR_LEFT, NULL, MAX_NPC/2 );
		}
	}

	if( npc->act_no >= 200 && npc->act_no < 300 ){
		switch( npc->act_wait ){
		case 30*1+50: gBoss[1].act_no = 120; break;
		case 30*2+50: gBoss[2].act_no = 120; break;
		case 30*3+50: gBoss[3].act_no = 120; break;
		case 30*4+50: gBoss[6].act_no = 120; break;
		case 30*5+50: gBoss[7].act_no = 120; break;
		}

		if( npc->x < npc->tgt_x + VS*PARTSSIZE*10 ) npc->xm += VS/128;
		if( npc->x > npc->tgt_x + VS*PARTSSIZE*10 ) npc->xm -= VS/128;
		if( npc->y < npc->tgt_y + VS*PARTSSIZE* 0 ) npc->ym += VS/128;
		if( npc->y > npc->tgt_y + VS*PARTSSIZE* 0 ) npc->ym -= VS/128;
	}

	if( npc->xm >  VS/4 ) npc->xm =  VS/4;
	if( npc->xm < -VS/4 ) npc->xm = -VS/4;
	if( npc->ym >  VS/4 ) npc->ym =  VS/4;
	if( npc->ym < -VS/4 ) npc->ym = -VS/4;
	npc->x += npc->xm;
	npc->y += npc->ym;

	ActBossChar_Core_Face( &gBoss[4] );
	ActBossChar_Core_Tail( &gBoss[5] );

	ActBossChar_Core_Mini( &gBoss[1] );
	ActBossChar_Core_Mini( &gBoss[2] );
	ActBossChar_Core_Mini( &gBoss[3] );
	ActBossChar_Core_Mini( &gBoss[6] );
	ActBossChar_Core_Mini( &gBoss[7] );

	ActBossChar_Core_Hit( &gBoss[8] );
	ActBossChar_Core_Hit( &gBoss[9] );
	ActBossChar_Core_Hit( &gBoss[10] );
	ActBossChar_Core_Hit( &gBoss[11] );
}