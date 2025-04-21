#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"



// 囚ミミガー（停止）
void ActNpc260( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{128, 64,144, 80},
		{144, 64,160, 80},
		{224, 64,240, 80},
	};
	RECT rcRight[] = {
		{128, 80,144, 96},
		{144, 80,160, 96},
		{224, 80,240, 96},
	};

	switch( npc->act_no ){
	case 0:
		npc->x += VS*1;
		npc->y -= VS*2;
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機
		if( Random( 0, 160 ) == 1 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 12 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;
	case 10:
		npc->act_no = 11;
		npc->ani_no =  2;
		SetNpChar( 87, npc->x, npc->y - 16*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
	case 11:
		break;
	}

		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// 囚チエ
void ActNpc261( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{112, 32,128, 48},
		{128, 32,144, 48},
	};
	RECT rcRight[] = {
		{112, 48,128, 64},
		{128, 48,144, 64},
	};

	switch( npc->act_no ){
	case 0:
		npc->x -= VS*1;
		npc->y -= VS*2;
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機
		if( Random( 0, 160 ) == 1 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 12 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;
	}

		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// 囚チャコ
void ActNpc262( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{128,  0,144, 16},
		{144,  0,160, 16},
	};
	RECT rcRight[] = {
		{128, 16,144, 32},
		{144, 16,160, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->x -= VS*1;
		npc->y -= VS*2;
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機
		if( Random( 0, 160 ) == 1 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 12 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;
	}

		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ドクター（戦闘LV1）
void ActNpc263( NPCHAR *npc )
{
	long deg;
	long xm, ym;
	RECT rcLeft[] = {
		{  0,  0, 24, 32},
		{ 24,  0, 48, 32},
		{ 48,  0, 72, 32},// 滞空
		{  0,  0,  0,  0},
		{ 72,  0, 96, 32},//4 手をかまえ
		{ 96,  0,120, 32},//5 手をあげる
		{120,  0,144, 32},//6 空中溜め
		{144,  0,168, 32},//7 氾濫
		{264,  0,288, 32},//8 ぐああ
	};
	RECT rcRight[] = {
		{  0, 32, 24, 64},
		{ 24, 32, 48, 64},
		{ 48, 32, 72, 64},
		{  0,  0,  0,  0},
		{ 72, 32, 96, 64},
		{ 96, 32,120, 64},
		{120, 32,144, 64},
		{144, 32,168, 64},
		{264, 32,288, 64},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y += 8*VS;
		npc->ani_no = 3;
	case 1:
		break;
	case 2:// 点滅出現
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 0;
		else                    npc->ani_no = 3;
		if( npc->act_wait > 50 ){
			npc->act_no = 10;
		}
		break;

	case 10: // 落下着陸
		npc->ym    += VS/4;
		npc->bits  |= BITS_BANISH_DAMAGE;
		npc->damage = 3;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 20;
			npc->act_wait =  0;
			npc->ani_no   =  0;
			npc->count2   = npc->life;

			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 20: // 地面待機
		npc->act_wait++;

		if( npc->act_wait < 50 && npc->life < npc->count2 - 20 )
			npc->act_wait = 50;


		if( npc->act_wait ==  50 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
			npc->ani_no = 4;
		}
		// 赤い衝撃
		if( npc->act_wait ==  80 ){
			npc->ani_no = 5;
			PlaySoundObject( WAVE_BUNRET, 1 );
			if( npc->direct == DIR_LEFT ){
				SetNpChar( 264, npc->x - 16*VS, npc->y, 0, 0,   0*8+DIR_LEFT,  NULL, MAX_NPC/2 );
				SetNpChar( 264, npc->x - 16*VS, npc->y, 0, 0, 128*8+DIR_LEFT,  NULL, MAX_NPC/2 );
			}else{
				SetNpChar( 264, npc->x + 16*VS, npc->y, 0, 0,   0*8+DIR_RIGHT, NULL, MAX_NPC/2 );
				SetNpChar( 264, npc->x + 16*VS, npc->y, 0, 0, 128*8+DIR_RIGHT, NULL, MAX_NPC/2 );
			}
		}
		if( npc->act_wait == 120 ) npc->ani_no = 0;
		if( npc->act_wait > 130 && npc->life < npc->count2 - 50 ) npc->act_wait = 161;
		if( npc->act_wait > 160 ){
			npc->act_no   = 100;
			npc->ani_no   =   0;
		}
		break;

	case  30:// 赤い氾濫
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->ani_no   =  6;
		npc->tgt_x    = npc->x;
		npc->bits |= BITS_BANISH_DAMAGE;
	case  31:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->x = npc->tgt_x;
		else                    npc->x = npc->tgt_x + VS;

		if( npc->act_wait > 50 ){
			npc->act_no   = 32;
			npc->act_wait =  0;
			npc->ani_no   =  7;
			PlaySoundObject( WAVE_THUNDER, 1 );
			for( deg = 8; deg < 256; deg += 16 ){
				xm = GetCos( (unsigned char)deg ) * 2;
				ym = GetSin( (unsigned char)deg ) * 2;
				SetNpChar( 266, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}

		break;
	case  32:
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 100;
		}
		break;

	case 100:// 転送
		npc->act_no   = 101;
		npc->bits    &= ~BITS_BANISH_DAMAGE;
		npc->damage   =   0;
		npc->act_wait =   0;
		PlaySoundObject( WAVE_WARP, 1 );
	case 101:
		npc->act_wait += 2;
		if( npc->act_wait > 16 ){
			npc->act_no   = 102;
			npc->act_wait =   0;
			npc->ani_no   =   3;// 消え
			npc->tgt_x = Random( 5, 35 ) *VS*PARTSSIZE;
			npc->tgt_y = Random( 5,  7 ) *VS*PARTSSIZE; 
		}
		break;
	case 102: // 透明待機
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_no   = 103;
			npc->act_wait =  16;
			npc->ani_no   =   2;
			npc->ym       =   0;
			npc->x = npc->tgt_x;
			npc->y = npc->tgt_y;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 103: // 転送出現
		npc->act_wait -= 2;
		if( npc->act_wait <= 0 ){
			npc->bits    |=  BITS_BANISH_DAMAGE;
			npc->damage   =  3;
			if( npc->count1 < 3 ){
				npc->count1++;
				npc->act_no = 10;
			}else{
				npc->count1 =  0;
				npc->act_no = 30;
			}
		}
		break;
	case 500: // ぐああ…
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->ani_no  = 6;
		npc->ym     += VS/32;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 501;
			npc->act_wait =   0;
			npc->tgt_x    = npc->x;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 501:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_no = 8;
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->x = npc->tgt_x;
		else                    npc->x = npc->tgt_x + VS;
		break;
	}

	// 赤い石
	if( npc->act_no >= 10 ){
		if( npc->act_no == 102 ){
			gSuperXpos = npc->tgt_x;
			gSuperYpos = npc->tgt_y;
		}else{
			gSuperXpos = npc->x;
			gSuperYpos = npc->y;
		}
	}

	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	// ワープ
	if( npc->act_no == 101 || npc->act_no == 103 ){
		npc->rect.top    +=      npc->act_wait;
		npc->rect.bottom -=      npc->act_wait;
		npc->view.top    = (16 - npc->act_wait)*VS;
	}else{
		npc->view.top = 16*VS;
	}
}


// 赤い衝撃
void ActNpc264( NPCHAR *npc )
{
	RECT rc = {288,  0,304, 16};
	unsigned char deg;

	// 画面外で消える
	if( npc->x < 0 || npc->x > gMap.width*PARTSSIZE*VS ){
		VanishNpChar( npc );
		return;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
		npc->count1 = npc->direct/8;
		npc->direct = npc->direct%8;
	case 1:
		npc->count1 += 6;
		npc->count1 = npc->count1%256;
		deg         = (unsigned char)npc->count1;
		if( npc->act_wait < 64*2 ) npc->act_wait++;

		if( npc->direct == DIR_LEFT ) npc->xm -= VS/24;
		else                          npc->xm += VS/24;

		npc->tgt_x += npc->xm;

		npc->x = npc->tgt_x + (GetCos( deg ) * npc->act_wait /2) / 4;
		npc->y = npc->tgt_y + (GetSin( deg ) * npc->act_wait /2) / 1;

//		npc->count2++;
//		if( npc->count2%4 == 1 )
			SetNpChar( 265, npc->x, npc->y, 0,0,DIR_LEFT, NULL, MAX_NPC/2 );
		break;
	}
	
	npc->rect = rc;
}

// 赤い衝撃の尾
void ActNpc265( NPCHAR *npc )
{
	RECT rc[] = {
		{288, 16,304, 32},
		{288, 32,304, 48},
		{288, 48,304, 64},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 3 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 ){
		npc->cond = 0;
		return;
	}
	npc->rect = rc[npc->ani_no];

}

// 赤い氾濫
void ActNpc266( NPCHAR *npc )
{
	RECT rc[] = {
		{304, 16,320, 32},
		{304, 32,320, 48},
	};
	if(  npc->flag & FLAG_HIT_LEFT   ) npc->xm *= -1;
	if(  npc->flag & FLAG_HIT_RIGHT  ) npc->xm *= -1;
	if(  npc->flag & FLAG_HIT_TOP    ) npc->ym =  VS;
	if(  npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->ani_no++;
	if( npc->ani_no > 1 )
		npc->ani_no = 0;
	npc->rect = rc[npc->ani_no];

	npc->act_wait++;
	if( npc->act_wait%4 == 1 ){
		SetNpChar( 265, npc->x, npc->y, 0,0,DIR_LEFT, NULL, MAX_NPC/2 );
	}
	if( npc->act_wait > 250 ) VanishNpChar( npc );

}

// ドクター（戦闘LV2）
void ActNpc267( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0,  0,  0},
		{  0, 64, 40,112},
		{ 40, 64, 80,112},
		{ 80, 64,120,112},//3 屈み
		{120, 64,160,112},//4 ジャンプ
		{160, 64,200,112},// 
		{200, 64,240,112},//6 波
		{240, 64,280,112},//7 タックル
		{280, 64,320,112},//

		{  0,160, 40,208},// 9dead
	};
	RECT rcRight[] = {
		{  0,  0,  0,  0},
		{  0,112, 40,160},
		{ 40,112, 80,160},
		{ 80,112,120,160},
		{120,112,160,160},
		{160,112,200,160},
		{200,112,240,160},
		{240,112,280,160},
		{280,112,320,160},

		{ 40,160, 80,208},
	};

	long xm, ym;
	long i;

	switch( npc->act_no ){
	case 0:// 出現
		if( gSuperXpos > gMC.x ) npc->direct = DIR_LEFT;
		else                     npc->direct = DIR_RIGHT;
		if( npc->direct == DIR_LEFT ) npc->x = gSuperXpos - 6*VS;
		else                          npc->x = gSuperXpos + 6*VS;
		npc->y      = gSuperYpos;
//		break;
	case 1:
		npc->act_no   =  2;
	case 2:
		npc->ym += VS/4;
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 0;
		else                    npc->ani_no = 3;
		break;

	case 5:// 地面待機
		npc->act_no   =  6;
		npc->ani_no   =  1;
		npc->ani_wait =  0;
	case 6:
		npc->ym += VS/4;
		npc->ani_wait++;
		if( npc->ani_wait > 40 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 1;
		break;

	case 7:// 戦闘開始 --------------------
		npc->act_no   =  8;
		npc->act_wait =  0;
		npc->ani_no   =  3;
	case 8:
		npc->ym += VS/8;
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_no = 10;
		}
		break;

	case 10:// 地面 ----------------------------------
		npc->bits    |= BITS_BLOCK_BULLET;
		npc->xm       =  0;
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->ani_no   =  1;
		npc->ani_wait =  0;
		npc->count2   = npc->life;
	case 11:
		npc->ym += VS/4;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			// 吹き飛ばし
			if( npc->life < npc->count2 - 20 ){
				if( gMC.flag & FLAG_HIT_BOTTOM       &&
					gMC.x > npc->x - PARTSSIZE*VS* 3 &&
					gMC.x < npc->x + PARTSSIZE*VS* 3 &&
					npc->ani_no != 6 ){
					npc->ani_no  = 6;
					DamageMyChar( 5 );
					SetQuake( 10 );
					PlaySoundObject( WAVE_QUAKE, 1 );
					gMC.ym = -VS*2;
					if( npc->x > gMC.x ) gMC.xm = -MAX_MOVE;
					else                 gMC.xm =  MAX_MOVE;
					for( i = 0; i < 100; i++ )
						SetNpChar( 270, npc->x + Random( -16, 16 )*VS, npc->y+ Random( -16, 16 )*VS,
										Random( -VS, VS )*3, Random( -VS, VS )*3, DIR_DOWN, 0, MAX_NPC/3 );

				}
			}else{

				// アニメ
				npc->ani_wait++;
				if( npc->ani_wait > 10 ){
					npc->ani_wait = 0;
					npc->ani_no++;
					if( npc->ani_no > 2 ) npc->ani_no = 1;
				}
			}

		}else{
			npc->ani_no = 4;
		}

		npc->act_wait++;
		// ジャンプかブラスター
		if( npc->act_wait > 30 || npc->life < npc->count2 - 20 ){
			npc->count1++;
			if( npc->count1 > 10 ) npc->count1 = 0;
			switch( npc->count1 ){
			case 8:
				npc->act_no   = 20; // 発砲
				break;
			case 2: case 7:
				npc->act_no   = 100; // ワープ
				break;
			case 3: case 6:
				npc->act_no   = 30; // 体当たり
				break;
			case 1: case 9:
				npc->act_no   = 40; // タックルジャンプ
				break;
			default:                // ジャンプ
				npc->act_no   = 15;
				npc->act_wait =  0;
				break;
			}
		}
		break;

	case 15:// 屈み
		npc->ani_no = 3;
		npc->act_wait++;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->act_wait > 20 ){
			npc->act_no   = 16;
			npc->ani_no   =  4;
			npc->ani_wait =  0;
			npc->ym       = -3*VS;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
			else                          npc->xm =  VS*2;
		}
		break;
	case 16:// 滞空
		npc->ym += VS/8;
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 4;
		//向き
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		//着陸
		if( npc->ym > 0 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 17;
		}
		break;
	case 17:// 着陸
		npc->act_no   = 18;
		npc->act_wait =  0;
		SetQuake( 10 );
		PlaySoundObject( WAVE_QUAKE, 1 );
	case 18:// 踏ん張り
		npc->ani_no   =  3;
		npc->act_wait++;
		npc->xm = npc->xm *7/8;
		npc->ym += VS/4;
		if( npc->act_wait > 10 ){
			npc->act_no =  10;
		}
		break;

	case 20: // 発砲
		npc->act_no = 21;
		npc->act_wait = 0;
	case 21:
		npc->act_wait++;

		npc->ani_no = 6;

		if( npc->act_wait > 20 && npc->act_wait%3 == 1 ){
			ym = Random( -VS,    VS );
			xm = Random(  VS/2, VS ) * 4;
			if( npc->direct == DIR_LEFT ) SetNpChar( 269, npc->x - 8*VS, npc->y-4*VS, -xm, ym, DIR_LEFT,  NULL, MAX_NPC/2 );
			else                          SetNpChar( 269, npc->x + 8*VS, npc->y-4*VS,  xm, ym, DIR_RIGHT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
		}


		if( npc->act_wait > 90 ){
			npc->act_no   = 10;
		}
		break;

	case 30: // タックル構え
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->bits |= BITS_BLOCK_MYCHAR;
		npc->bits &= ~BITS_BANISH_DAMAGE;
	case 31:
		npc->ani_no   =  3;
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no   = 32;
			npc->act_wait =  0;
			npc->ani_no   =  7;
			npc->bits    |= BITS_DAMAGE_SIDE;
			npc->damage   = 10;
			PlaySoundObject( WAVE_BUNRET, 1 );
			if( npc->direct == DIR_LEFT ) npc->xm = -MAX_MOVE;
			else                          npc->xm =  MAX_MOVE;
		}
		break;
	case 32: // タックル
		npc->act_wait++;
		npc->ym = 0;
		if( npc->act_wait/2%2 ) npc->ani_no = 7;
		else                    npc->ani_no = 8;
		if( npc->act_wait > 30 ){
			npc->act_wait =  0;
			npc->act_no   = 18;
			npc->damage   =  5;
			npc->bits    &= ~(BITS_BLOCK_MYCHAR|BITS_DAMAGE_SIDE);
			npc->bits    |= BITS_BANISH_DAMAGE;
		}
		if( npc->flag & FLAG_HIT_LEFT || npc->flag & FLAG_HIT_RIGHT ){
			npc->act_no   = 15;
			npc->act_wait =  0;
			npc->damage   =  5;
			npc->bits    &= ~(BITS_BLOCK_MYCHAR|BITS_DAMAGE_SIDE);
			npc->bits    |= BITS_BANISH_DAMAGE;
		}
		break;

	case 40:// 屈み(スーパー)
		npc->ani_no = 3;
		npc->act_wait++;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->act_wait > 20 ){
			npc->act_no   = 41;
			npc->ani_no   =  4;
			npc->ani_wait =  0;
			npc->ym       = -4*VS;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
			else                          npc->xm =  VS*2;
		}
		break;
	case 41:// 滞空(スーパー)
		npc->ym += VS/8;
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 4;
		if( gMC.y > npc->y &&
			gMC.x > npc->x - 8*VS &&
			gMC.x < npc->x + 8*VS ){
			npc->act_no = 16;
			npc->ym = MAX_MOVE;
			npc->xm = 0;
		}

		//着陸
		if( npc->ym > 0 && npc->flag & FLAG_HIT_BOTTOM ) npc->act_no   = 17;
		break;



	case 100:
		npc->act_no   = 101;
		npc->act_wait =   0;
		npc->bits    &= ~(BITS_BANISH_DAMAGE|BITS_BLOCK_BULLET);
		npc->damage   =   0;
		PlaySoundObject( WAVE_WARP, 1 );
	case 101:// 転送 ----------------------
		npc->act_wait += 2;
		if( npc->act_wait > 28 ){
			npc->act_no   = 102;
			npc->act_wait =   0;
			npc->ani_no   =   0;// 消え
			npc->tgt_x = gMC.x;//Random( 5, 35 ) *VS*PARTSSIZE;
			npc->tgt_y = gMC.y - 32*VS;//Random( 5,  7 )
			if( npc->tgt_y < VS*PARTSSIZE* 4 ) npc->tgt_y = VS*PARTSSIZE*4;
			if( npc->tgt_x < VS*PARTSSIZE* 4 ) npc->tgt_x = VS*PARTSSIZE*4;
			if( npc->tgt_x > VS*PARTSSIZE*36 ) npc->tgt_x = VS*PARTSSIZE*36;
		}
		break;
	case 102: // 透明待機
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_no   = 103;
			npc->act_wait =  28;
			npc->ani_no   =   4;
			npc->ym       =   0;
			npc->x = npc->tgt_x;
			npc->y = npc->tgt_y;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 103: // 転送出現
		npc->act_wait -= 2;
		if( npc->act_wait <= 0 ){

			npc->bits   |= (BITS_BANISH_DAMAGE|BITS_BLOCK_BULLET);
			npc->damage  =   5;
			npc->act_no  =  16;
			npc->ym      = -VS;
			npc->xm      =   0;
		}
		break;

	case 500: // やられ -----------------
		DeleteNpCharCode( 269, TRUE );
		npc->bits &= ~BITS_BANISH_DAMAGE|BITS_BLOCK_BULLET;
		npc->ani_no  = 4;
		npc->ym     += VS/16;
		npc->xm      = 0;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 501;
			npc->act_wait =   0;
			npc->tgt_x    = npc->x;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 501:
		npc->ani_no = 9;
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->x = npc->tgt_x;
		else                    npc->x = npc->tgt_x + VS;
		break;

	case 510: // 消えていく -----------
		npc->act_no   = 511;
		npc->act_wait =   0;
		npc->ani_no   =   9;
		npc->tgt_x    = npc->x;
		npc->y       += 16*VS;
		npc->bits |= BITS_THROW_BLOCK;
	case 511:
		SetQuake( 2 );
		npc->act_wait++;
		if( npc->act_wait % 6 == 3 )PlaySoundObject( WAVE_BUNRET, 1 );
		if( npc->act_wait/2%2 ) npc->x = npc->tgt_x;
		else                    npc->x = npc->tgt_x + VS;
		if( npc->act_wait > 44*8 ){
			npc->ani_no = 0;
			npc->act_no = 512;
		}
		break;
	case 512:
		break;
	case 520: // 赤い石を上へ
		npc->damage = 0;
		gSuperYpos = -32*VS;
		break;
	}

	// 赤い石
	if( npc->act_no >= 11 && npc->act_no < 501 ){
		if( npc->act_no == 102 ){
			gSuperXpos = npc->tgt_x;
			gSuperYpos = npc->tgt_y;
		}else{
			gSuperXpos = npc->x;
			gSuperYpos = npc->y;
		}
	}

	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	// 血
	if( npc->act_no >= 512 ){
	}else if( npc->act_no < 510 ){
		if( npc->act_no != 102 &&
			npc->act_no != 103 &&
			Random( 0, 3 ) == 2 ) SetNpChar( 270, npc->x + Random( -16, 16 )*VS,
											npc->y + Random( - 8,  4 )*VS, npc->xm, 0, DIR_DOWN, 0, MAX_NPC/2 );
	}else{
			SetNpChar( 270, npc->x + Random( -16, 16 )*VS,
							npc->y - (42*8 - npc->act_wait)/8*VS, Random( -VS, VS ), Random( -VS, 0 )*2, DIR_DOWN, 0, MAX_NPC/3 );
			SetNpChar( 270, npc->x + Random( -16, 16 )*VS,
							npc->y - (42*8 - npc->act_wait)/8*VS, Random( -VS, VS ), Random( -VS, 0 )*2, DIR_DOWN, 0, MAX_NPC/3 );
			SetNpChar( 270, npc->x + Random( -16, 16 )*VS,
							npc->y - (42*8 - npc->act_wait)/8*VS,                 0, Random( -VS, 0 )*2, DIR_DOWN, 0, MAX_NPC/3 );
			SetNpChar( 270, npc->x + Random( -16, 16 )*VS,		 
							npc->y - (42*8 - npc->act_wait)/8*VS,                 0, Random( -VS, 0 )*2, DIR_DOWN, 0, MAX_NPC/3 );
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	// ワープ
	if( npc->act_no == 511 ){
		npc->rect.top    += npc->act_wait/8;
		npc->view.top    = (44 - npc->act_wait/8)*VS;
		npc->view.bottom = 4*VS;
	}else if( npc->act_no == 101 || npc->act_no == 103 ){
		npc->rect.top    +=      npc->act_wait;
		npc->rect.bottom -=      npc->act_wait;
		npc->view.top    = (28 - npc->act_wait)*VS;
	}else{
		npc->view.top = 28*VS;
	}
}


// バルコニーラビル-B
void ActNpc268( NPCHAR *npc )
{
	unsigned char deg;
	long xm, ym;
	RECT rcLeft[] = {
		{  0,  0, 40, 40},
		{ 40,  0, 80, 40},

		{ 80,  0,120, 40},//2 歩く
		{  0,  0, 40, 40},
		{120,  0,160, 40},
		{  0,  0, 40, 40},

		{ 40, 80, 80,120},//6 屈み
		{  0, 80, 40,120},//7 ジャンプ
		{240,  0,280, 40},//波動砲
		{280,  0,320, 40},
	};
	RECT rcRight[] = {
		{  0, 40, 40, 80},
		{ 40, 40, 80, 80},

		{ 80, 40,120, 80},
		{  0, 40, 40, 80},
		{120, 40,160, 80},
		{  0, 40, 40, 80},

		{160, 80,200,120},
		{120, 80,160,120},
		{240, 40,280, 80},//波動砲
		{280, 40,320, 80},
	};

	if( npc->x < gMC.x - 20 * VS * PARTSSIZE ||
		npc->x > gMC.x + 20 * VS * PARTSSIZE ||
		npc->y < gMC.y - 15 * VS * PARTSSIZE ||
		npc->y > gMC.y + 15 * VS * PARTSSIZE ){

		npc->act_no = 1;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y += VS*8;
	case 1:// 待機
		npc->ani_wait++;
		if( npc->ani_wait > 20 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		if( npc->x < gMC.x + PARTSSIZE*7*VS &&
			npc->x > gMC.x - PARTSSIZE*7*VS &&
			npc->x < gMC.x + PARTSSIZE*3*VS &&
			npc->x > gMC.x - PARTSSIZE*7*VS ){
			

			npc->act_no = 10;
		}
		if( npc->shock ){
			npc->act_no = 10;
		}
		break;
	case 10:// 歩く
		npc->act_no    = 11;
		npc->act_wait  =  0;
		npc->ani_no    =  0;
		npc->ani_wait  =  0;
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 11:

		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;

		if( npc->x < gMC.x + 4*VS*PARTSSIZE &&
			npc->x > gMC.x - 4*VS*PARTSSIZE ){
			npc->act_no   = 20;
			npc->act_wait =  0;
		}
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ){
			npc->act_no   = 20;
			npc->act_wait =  0;
		}
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ){
			npc->act_no   = 20;
			npc->act_wait =  0;
		}
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		break;

	case 20://かがむ
		npc->xm     = 0;
		npc->ani_no = 6;
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 30;
			npc->ym     = -MAX_MOVE;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS*1;
			else                          npc->xm =  VS*1;
			PlaySoundObject( WAVE_BIGJUMP, 1 );
		}
		break;

	case 30://滞空
		npc->ani_no = 7;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 40;
			npc->act_wait =  0;
			SetQuake( 20 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		break;

	case 40://着陸
		npc->xm     = 0;
		npc->ani_no = 6;
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 50;
		}
		break;

	case 50:// 波動砲-----------------
		npc->act_no   = 51;
		npc->act_wait = 0;
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 51://エネルギー充填／弾吐き
		npc->act_wait++;
		if( npc->act_wait > 30 && npc->act_wait%4 == 1 ){

			if( npc->direct == DIR_LEFT ) deg = 128+8;
			else                          deg = 256-8;
			deg += (unsigned char)Random( -16, 16 );
			ym = GetSin( deg ) * 5;
			xm = GetCos( deg ) * 5;
			SetNpChar( 11, npc->x, npc->y+4*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_BREAK1, 1 );
		}
		//アニメーション
		if( npc->act_wait < 50 && npc->act_wait/2%2 ) npc->ani_no = 9;
		else                                          npc->ani_no = 8;

		if( npc->act_wait > 50*1 + 32 ){
			npc->act_no    = 10;
			if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;

	}


	npc->ym += VS/10;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// コウモリブラスター
void ActNpc269( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{232,  0,248, 16},
		{248,  0,264, 16},
		{248, 16,264, 32},
	};
	RECT rcRight[] = {
		{232, 32,248, 48},
		{248, 32,264, 48},
		{248, 48,264, 64},
	};

	switch( npc->act_no ){
	case 0: 
		npc->act_no = 1;
		npc->xm2 = npc->xm;
		npc->ym2= npc->ym;
	case 1:
		if( npc->xm2 < 0 && npc->flag & FLAG_HIT_LEFT ){
			npc->direct = DIR_RIGHT;
			npc->xm2     *= -1;
		}else if( npc->xm2 > 0 && npc->flag & FLAG_HIT_RIGHT ){
			npc->direct = DIR_LEFT;
			npc->xm2     *= -1;
		}else if( npc->ym2 < 0 && npc->flag & FLAG_HIT_TOP    ) npc->ym2 *= -1;
		else if(  npc->ym2 > 0 && npc->flag & FLAG_HIT_BOTTOM ) npc->ym2 *= -1;


		npc->x += npc->xm2;
		npc->y += npc->ym2;
		// 壁消滅
/*		if( npc->flag & FLAG_HIT_X ){
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			VanishNpChar( npc );
			return;
		}
*/
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ブラッド
void ActNpc270( NPCHAR *npc )
{
	RECT rc[] = {
		{170,34,174,38},
		{170,42,174,46},
	};
	// 飛び散り
	if( npc->direct == DIR_DOWN || npc->direct == DIR_UP ){

		if( npc->direct == DIR_DOWN ) npc->ym += VS/8;
		if( npc->direct == DIR_UP   ) npc->ym -= VS/8;
		npc->act_wait++;

		if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
		npc->x += npc->xm;
		npc->y += npc->ym;

		if( npc->act_wait > 50     ) npc->cond = 0;
		if( npc->flag & FLAG_HIT_X ) npc->cond = 0;

	// わらわら
	}else if( npc->direct == DIR_RIGHT ){
		switch( npc->act_no ){
		case 0:
			npc->act_no = 1;
			npc->bits  |= BITS_THROW_BLOCK;
			npc->xm     = Random( -VS, VS )*3;
			npc->ym     = Random( -VS, VS )*3;
			npc->count1 = Random( VS/32, VS/10 );
			npc->count2 = Random( VS/4, VS/2 );
		case 1:
			break;
		}

		if( npc->x < npc->pNpc->x ) npc->xm += VS/npc->count1;
		if( npc->x > npc->pNpc->x ) npc->xm -= VS/npc->count1;
		if( npc->y < npc->pNpc->y ) npc->ym += VS/npc->count1;
		if( npc->y > npc->pNpc->y ) npc->ym -= VS/npc->count1;

		if( npc->xm >  npc->count2*2 ) npc->xm =  npc->count2*2;
		if( npc->xm < -npc->count2*2 ) npc->xm = -npc->count2*2;
		if( npc->ym >  npc->count2*3 ) npc->ym =  npc->count2*3;
		if( npc->ym < -npc->count2*3 ) npc->ym = -npc->count2*3;

		npc->x += npc->xm;
		npc->y += npc->ym;

	}
	npc->rect = rc[Random(0,1)];
}

// 流れブロック 小
void ActNpc271( NPCHAR *npc )
{
	long a;
	if( npc->xm < 0 && npc->x < -16*VS ){
		VanishNpChar( npc );
		return;
	}
	if( npc->xm > 0 && npc->x > gMap.width*VS*PARTSSIZE + 16*VS ){
		VanishNpChar( npc );
		return;
	}

	if( !npc->act_no ){
		npc->act_no = 1;
		a = Random( 0, 9 );
		if( a == 9 ){
			npc->rect.left   = 0;
			npc->rect.right  = 32;
			npc->rect.top    = 64;
			npc->rect.bottom = 96;
			npc->view.front  = 16*VS;
			npc->view.back   = 16*VS;
			npc->view.top    = 16*VS;
			npc->view.bottom = 16*VS;
			npc->hit.front   = 12*VS;
			npc->hit.back    = 12*VS;
			npc->hit.top     = 12*VS;
			npc->hit.bottom  = 12*VS;
		}else{
			npc->rect.left   = 112 + a%3 * 16;
			npc->rect.top    =   0 + a/3 * 16;
			npc->rect.right  = npc->rect.left + 16;
			npc->rect.bottom = npc->rect.top  + 16;
		}

		if( npc->direct == DIR_LEFT ) npc->xm = Random(  VS/2,   VS ) * -2;
		else                          npc->xm = Random(  VS/2,   VS ) *  2;
		npc->ym = Random( -VS, VS );
	}

	if( npc->ym < 0 && npc->y - npc->hit.top < 8*VS                 ){
		npc->ym *= -1;
		SetCaret( npc->x, npc->y -  8 * VS, CARET_SMALLSTAR, DIR_LEFT );
		SetCaret( npc->x, npc->y -  8 * VS, CARET_SMALLSTAR, DIR_LEFT );

	}
	if( npc->ym > 0 && npc->y + npc->hit.bottom > 15*PARTSSIZE*VS-8*VS ){
		npc->ym *= -1;
		SetCaret( npc->x, npc->y +  8 * VS, CARET_SMALLSTAR, DIR_LEFT );
		SetCaret( npc->x, npc->y +  8 * VS, CARET_SMALLSTAR, DIR_LEFT );
	}

/*	if( gMC.flag & FLAG_HIT_LEFT                          &&
		gMC.x                  < npc->x - npc->hit.back   &&
		gMC.y + gMC.hit.bottom > npc->y - npc->hit.top    &&
		gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom ){
		npc->damage = 127;
	}else if( gMC.flag & FLAG_HIT_RIGHT                   &&
		gMC.x                  > npc->x + npc->hit.back   &&
		gMC.y + gMC.hit.bottom > npc->y - npc->hit.top    &&
		gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom ){
		npc->damage = 127;
	}else if( gMC.flag & FLAG_HIT_TOP                     &&
		gMC.y                  < npc->y - npc->hit.top    &&
		gMC.x + gMC.hit.front  > npc->x - npc->hit.back   &&
		gMC.x - gMC.hit.back   < npc->x + npc->hit.front  ){
		npc->damage = 127;
	}else if( gMC.flag & FLAG_HIT_BOTTOM                  &&
		gMC.y                  > npc->y + npc->hit.bottom &&
		gMC.x + gMC.hit.front  > npc->x - npc->hit.back   &&
		gMC.x - gMC.hit.back   < npc->x + npc->hit.front  ){
		npc->damage = 127;
	}else{
		npc->damage = 0;
	}
*/
	npc->x += npc->xm;
	npc->y += npc->ym;
}



// 流れブロック配置
void ActNpc272( NPCHAR *npc )
{
//	long xm;
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->act_wait = Random( 0, 200 );
	case 1:
		if( npc->act_wait ){
			npc->act_wait--;
			break;
		}
		npc->act_no = 0;
		SetNpChar( 271, npc->x, npc->y + Random( -32, 32 )*VS, 0, 0, npc->direct, NULL, MAX_NPC/2 );
		break;
	}
}

// トロールハンマー
void ActNpc273( NPCHAR *npc )
{
	RECT rc[] = {
		{248, 40,272, 64},
		{272, 40,296, 64},
		{296, 40,320, 64},
	};

	switch( npc->act_no ){
	case 0: 
		npc->act_no = 1;
	case 1:
		npc->x += npc->xm;
		npc->y += npc->ym;
		// 壁消滅
		if( npc->flag & FLAG_HIT_X ){
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			VanishNpChar( npc );
			return;
		}
		npc->act_wait++;
		if( npc->act_wait%5 == 0 ) PlaySoundObject( WAVE_HERI2, 1 );
		npc->ani_no++;
		if( npc->ani_no > 2 )
			npc->ani_no = 0;
		break;
	}

	npc->rect = rc[ npc->ani_no];
}


// ドロール(大農園)
void ActNpc274( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 32, 40},
		{ 32,  0, 64, 40},

		{ 64,  0, 96, 40},
		{ 64, 80, 96,120},
		{ 96, 80,128,120},
		{ 96,  0,128, 40},
	};
	RECT rcRight[] = {
		{  0, 40, 32, 80},
		{ 32, 40, 64, 80},

		{ 64, 40, 96, 80},
		{ 64,120, 96,160},
		{ 96,120,128,160},
		{ 96, 40,128, 80},
	};

	unsigned char deg;
	long xm, ym;

	switch( npc->act_no ){
	case 0:// 待機
		npc->act_no = 1;
		npc->y     -= 8*VS;
		npc->tgt_x  = npc->x;
	case 1:
		npc->xm     = 0;
		npc->act_no = 2;
		npc->ani_no = 0;
	case 2:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_wait++;
		if( npc->ani_wait > 40 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ) npc->ani_no   =  0;
		if( npc->shock      ) npc->act_no   = 10;
		break;

	case 10: //ジャンプ
		npc->act_no   = 11;
		npc->ani_no   =  2;
		npc->act_wait =  0;
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 12;
			npc->ani_no =  3;
			npc->ym     = -VS*3;
			npc->count1 = 0;

			if( npc->tgt_x > npc->x ) npc->xm =  VS;
			else                      npc->xm = -VS;
		}
		break;

	case 12:// 空中
		if( npc->ym >  0 ){
			npc->ani_no = 4;
			if( !npc->count1 ){
				npc->count1++;
				deg = GetArktan( npc->x - gMC.x, npc->y -10*VS - gMC.y );
				ym  = GetSin( deg ) * 4;
				xm  = GetCos( deg ) * 4;
				SetNpChar( 273, npc->x, npc->y-10*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
				PlaySoundObject( WAVE_POP, 1 );
			}
		}
		if( npc->ym > VS ) npc->ani_no = 5;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_no   =  2;
			npc->act_no   = 13;
			npc->act_wait =  0;
			npc->xm       =  0;
		}
		break;

	case 13:// 着陸
		npc->xm = npc->xm / 2;
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 1;
		}
		break;
	}

	npc->ym += VS/6;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ゴン太(大農園へおとどけ)
void ActNpc275( NPCHAR *npc )
{
	RECT rcRight[] = {
		{272, 80,288, 96},
		{288, 80,304, 96},// まばたき
		{272, 80,288, 96},// シッポフリフリ
		{304, 80,320, 96},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1:// 待機 ---
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		// 近づくとフリフリ
		if( npc->x - PARTSSIZE*4*VS < gMC.x && npc->x + PARTSSIZE*4*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y && npc->y + PARTSSIZE*1*VS > gMC.y ){

			npc->ani_wait++;
			if( npc->ani_wait > 3 ){
				npc->ani_wait = 0;
				npc->ani_no++;
			}
			if( npc->ani_no > 3 ){
				npc->ani_no = 2;
			}
		}
		break;

	case 2:// 瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;
	}

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;
	npc->rect = rcRight[npc->ani_no];

}

// エンペラードロール(最後の洞窟２)
void ActNpc276( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 64, 32,104},
		{ 32, 64, 64,104},
		{ 64, 64, 96,104},//屈む

		{ 96, 64,128,104},// 3
		{128, 64,160,104},

		{160, 64,192,104},// 5
		{192, 64,224,104},
		{224, 64,256,104},
		{256, 64,288,104},// 8石化
	};
	RECT rcRight[] = {
		{  0,104, 32,144},
		{ 32,104, 64,144},
		{ 64,104, 96,144},//屈む
			 
		{ 96,104,128,144},// 3
		{128,104,160,144},
			 
		{160,104,192,144},// 5
		{192,104,224,144},
		{224,104,256,144},
		{256,104,288,144},
	};

	unsigned char deg;
	long xm, ym;

	switch( npc->act_no ){
	case 0:// 待機
		npc->act_no = 1;
		npc->y     -= 8*VS;
	case 1:
		npc->xm     = 0;
		npc->act_no = 2;
		npc->ani_no = 0;
	case 2:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_wait++;
		if( npc->ani_wait > 20 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no   =  0;
		if( npc->shock )
			npc->act_no   = 10;
		break;


	case 10: // 戦闘体勢
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->ani_no   =  3;
		npc->bits |= BITS_BANISH_DAMAGE;
	case 11:
		npc->act_wait++;
		switch( npc->act_wait ){
		case 30: case 40: case 50:
			npc->ani_no   =  4;
			deg = GetArktan( npc->x - gMC.x, npc->y  - gMC.y );
			ym  = GetSin( deg ) * 4;
			xm  = GetCos( deg ) * 4;
			SetNpChar( 277, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			break;
		case 34: case 44: case 54:
			npc->ani_no   =  3;
			break;
		}
		if( npc->act_wait > 60 ){
			npc->act_no   = 20;
			npc->act_wait =  0;
			npc->ani_no   =  2; 
		}
		break;


	case 20:// 屈み
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no   = 21;
			npc->act_wait =  0;
			npc->ani_no   =  5;
			npc->ym = -MAX_MOVE;
			if( npc->x < gMC.x ) npc->xm =  VS/2;
			else                 npc->xm = -VS/2;
		}
		break;

	case 21:// ジャンプ
		npc->act_wait++;
		switch( npc->act_wait ){
		case 30: case 40: case 50:
			npc->ani_no   =  6;
			deg = GetArktan( npc->x - gMC.x, npc->y -10*VS - gMC.y );
			ym  = GetSin( deg ) * 4;
			xm  = GetCos( deg ) * 4;
			SetNpChar( 277, npc->x, npc->y-10*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			break;
		case 34: case 44: 
			npc->ani_no   =  5;
			break;
		}

		if( npc->act_wait > 53 ) npc->ani_no = 7;
		
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 22;
			npc->act_wait =  0;
			npc->ani_no   =  2;
			SetQuake( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		break;

	case 22:// 着陸
		npc->xm = npc->xm / 2;
		npc->act_wait++;
		if( npc->act_wait > 22 ){
			npc->act_no = 10;
		}
		break;

	case 50:// 石化
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->damage = 0;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 51;
			npc->ani_no   =  2;
			SetQuake( 10 );
			SetExpObjects( npc->x, npc->y, 19 );
			SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );
			PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		}
		break;
	case 51:// 石化
		npc->xm = npc->xm *7/8;
		npc->ani_no = 8;
		break;
	}

	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->act_no < 50 ){
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
	}


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// トロールハンマー 赤
void ActNpc277( NPCHAR *npc )
{
	RECT rc[] = {
		{128,  0,152, 24},
		{152,  0,176, 24},
		{176,  0,200, 24},
	};

	switch( npc->act_no ){
	case 0: 
		npc->act_no = 1;
	case 1:
		npc->x += npc->xm;
		npc->y += npc->ym;
		// 壁消滅
		if( npc->flag & FLAG_HIT_X ){
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			VanishNpChar( npc );
			return;
		}

		npc->act_wait++;
		if( npc->act_wait%5 == 0 ) PlaySoundObject( WAVE_HERI2, 1 );
		npc->ani_no++;
		if( npc->ani_no > 2 )
			npc->ani_no = 0;
		break;
	}

	npc->rect = rc[ npc->ani_no];
}

// リトルおやじ
void ActNpc278( NPCHAR *npc )
{
	RECT rcPapa[] = {
		{  0, 120,  8, 128},
		{  8, 120, 16, 128},
	};
	RECT rcMama[] = {
		{ 16, 120, 24, 128},
		{ 24, 120, 32, 128},
	};
	RECT rcKodomo[] = {
		{ 32, 120, 40, 128},
		{ 40, 120, 48, 128},
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0;
	case 1://待機
		if( Random( 0, 60 ) == 1 ){
			npc->act_no   =  2;
			npc->act_wait =  0;
			npc->ani_no   =  1;
		}
		if( Random( 0, 60 ) == 1 ){
			npc->act_no   = 10;
			npc->act_wait =  0;
			npc->ani_no   =  1;
		}
		break;
	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;

	case 10: // 歩く
		npc->act_no   = 11;
		npc->act_wait = Random( 0, 16 );
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		if( Random( 0, 9 )%2 ) npc->direct = DIR_LEFT;
		else                   npc->direct = DIR_RIGHT;
	case 11:
		
		if(      npc->direct == DIR_LEFT  && npc->flag & FLAG_HIT_LEFT  ) npc->direct = DIR_RIGHT;
		else if( npc->direct == DIR_RIGHT && npc->flag & FLAG_HIT_RIGHT ) npc->direct = DIR_LEFT;

		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		npc->act_wait++;
		if( npc->act_wait > 32 ){
			npc->act_no = 0;
		}
		break;
	}
	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;


	switch( npc->code_event ){
	case 200: npc->rect = rcPapa[   npc->ani_no]; break;
	case 210: npc->rect = rcMama[   npc->ani_no]; break;
	default:  npc->rect = rcKodomo[ npc->ani_no]; break;
	}

}

// 白い落石(テレポートエフェクト付き)
void ActNpc279( NPCHAR *npc )
{
	RECT rc[] = {
		{0, 16, 32, 48},
		{16, 0, 32, 16},
	};
	long i;

	switch( npc->act_no ){
	case 0:
		switch( npc->direct ){ 
		case DIR_LEFT:
			npc->act_no = 100;
			npc->bits  |= BITS_BLOCK_BULLET;
			npc->ani_no = 0;
			break;
		case DIR_RIGHT:
			npc->act_no      = 100;
			npc->bits       |= BITS_BLOCK_BULLET;
			npc->ani_no      = 1;
			npc->view.back   = 8*VS;
			npc->view.front  = 8*VS;
			npc->view.top    = 8*VS;
			npc->view.bottom = 8*VS;
			npc->hit.back    = 8*VS;
			npc->hit.front   = 8*VS;
			npc->hit.top     = 8*VS;
			npc->hit.bottom  = 8*VS;
			break;
		case DIR_UP:
			npc->ani_no =  0;
			npc->act_no = 10;
			break;
		}
		if( npc->direct != DIR_UP ) break;
	case 10:// テレポート
		npc->act_no   = 11;
		npc->act_wait = 16;
	case 11:
		npc->act_wait -= 2;
		if( npc->act_wait <= 0 ){
			npc->act_no = 100;
			npc->bits |= BITS_BLOCK_BULLET;
		}
		break;
	case 100:// 落下
		npc->ym += VS/8;
		if( npc->ym > VS*3+VS/2 ) npc->ym = VS*3+VS/2;

		if( npc->y > VS*PARTSSIZE*8 ) npc->bits &= ~BITS_THROW_BLOCK;


		if( npc->flag & FLAG_HIT_BOTTOM ){

			npc->ym     = -VS;
			npc->act_no = 110;
			npc->bits |= BITS_THROW_BLOCK;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 10 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + 16*VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}

		}
		break;
	case 110: // 去る
		npc->ym += VS/8;
		if( npc->y > gMap.length * VS*PARTSSIZE + 32*VS ){
			npc->cond = 0;
			return;
		}
		break;
	}

	if( gMC.y                  > npc->y ){

		npc->damage =  10;
	}else{
		npc->damage =   0;
	}

	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];

	if( npc->act_no == 11 ){
		npc->rect.top    +=      npc->act_wait;
		npc->rect.bottom -=      npc->act_wait;
		npc->view.top    = (16 - npc->act_wait)*VS;
	}

}

