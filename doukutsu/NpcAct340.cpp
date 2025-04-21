#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"


// 神官ボロス
void ActNpc340( NPCHAR *npc )
{
	long i;
	long x;


	switch( npc->act_no ){
	case 0:// 初期設定
		npc->act_no      = 1;
		npc->cond        = COND_ALIVE;
		npc->exp         = 1;
		npc->direct      = DIR_LEFT;
		npc->y          -= 6*VS;
		npc->damage      = 0;
		SetNpChar( 341, npc->x, npc->y - 16 *VS, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
	case 1:// 待機
		break;

	case 10: // 開眼
		npc->act_no      = 11;
		npc->act_wait    =  0;
	case 11:
		npc->act_wait++;
//		if( npc->act_wait == 50 ) 
		if( npc->act_wait > 100 ) npc->act_no = 100;
		break;
		
	// 戦闘開始 ///////////////////////////////////////
	case 100:
		npc->act_no   = 110;
		npc->act_wait =   0;
		npc->ani_no   =   1;
		npc->ani_wait =   0;
		npc->damage   =   4;
		npc->bits    |= BITS_BANISH_DAMAGE;
	case 110:
		npc->act_no = 111;
		npc->damage =   3;
		npc->tgt_x  = npc->life;
	case 111:
		npc->ani_wait++;
		if( npc->ani_wait > 10 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 1;

		npc->act_wait++;
		if( npc->life < npc->tgt_x - 50 || npc->act_wait > 150 ){
			switch( npc->count2 % 5 ){
			case 0: case 1: case 2: case 3:// 体
				npc->act_no = 200;
				break;
			case 4:// 雷
				npc->act_no = 300;
				break;
			}
			npc->count2++;
		}
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		break;

	case 200:// 体当たり ----------------------------------
		npc->act_no   = 201;
		npc->count1   =   0;
	case 201:
		if( !npc->xm ) npc->act_no   = 202;
		else           npc->act_no   = 203;
		npc->act_wait =   0;
		npc->ani_no   =   3;
		npc->damage   =   3;
		npc->count1++;
	case 202:
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->xm       =   npc->xm * 8 /9;
		npc->ym       =   npc->ym * 8 /9;
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no = 210;// 横
		}
		break;
	case 203:
		npc->xm       =   npc->xm * 8 /9;
		npc->ym       =   npc->ym * 8 /9;
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			if( gMC.y < npc->y+12*VS ) npc->act_no = 220;// 上
			else                       npc->act_no = 230;// 下
		}
		break;

	case 210:// 横へ
		npc->act_no   = 211;
		npc->act_wait = 0;
		npc->ani_no   = 6;
		npc->ani_wait = 0;
		npc->ym       = 0;
		npc->damage   = 10;
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		PlaySoundObject( WAVE_BUNRET, 1 );
	case 211:
		if( npc->direct == DIR_LEFT ) npc->xm = -4*VS;
		else                          npc->xm =  4*VS;
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 6;
		else                    npc->ani_no = 7;

		if( npc->direct == DIR_LEFT  && npc->flag & FLAG_HIT_LEFT  ){
			npc->act_no   = 212;
			npc->act_wait =   0;
			npc->damage   =   3;
			SetQuake2( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		if( npc->direct == DIR_RIGHT && npc->flag & FLAG_HIT_RIGHT ){
			npc->act_no   = 212;
			npc->act_wait =   0;
			npc->damage   =   3;
			SetQuake2( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		if( npc->count1 < 4 && gMC.x > npc->x - 16*VS && gMC.x < npc->x + 16*VS ){
			npc->act_no = 201;
		}
		break;
	case 212:// quake
		npc->act_wait++;
		npc->xm     = 0;
		npc->ani_no = 6;
		if( npc->act_wait > 30 ){
			if( npc->count1 > 3 ) npc->act_no = 240;
			else                  npc->act_no = 201;
		}
		break;

	case 220:// 上へ
		npc->act_no   = 221;
		npc->act_wait = 0;
		npc->ani_no   = 8;
		npc->ani_wait = 0;
		npc->xm       = 0;
		npc->damage   = 10;
		npc->direct   = DIR_LEFT;
		PlaySoundObject( WAVE_BUNRET, 1 );
	case 221:
		npc->ym = -4*VS;
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 8;
		else                    npc->ani_no = 9;
		if( npc->y < VS*PARTSSIZE*2 + 16*VS ){
			npc->y = VS*PARTSSIZE*2 + 16*VS;
			npc->ym       =   0;
			npc->act_no   = 222;
			npc->act_wait =   0;
			npc->damage   =   3;
			for( i = 0; i < 8; i++ ){
				x = npc->x + Random(-16, 16)*VS;
				SetNpChar( 4, x, npc->y - 10*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			}
			SetNpChar( 332, npc->x -12*VS, npc->y-12*VS, 0, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
			SetNpChar( 332, npc->x +12*VS, npc->y-12*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
			SetQuake2( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		if( npc->count1   <   4    &&
			gMC.y > npc->y - 16*VS &&
			gMC.y < npc->y + 16*VS ){
			npc->act_no = 201;
		}
		break;
	case 222:// quake
		npc->act_wait++;
		npc->xm     = 0;
		npc->ani_no = 8;
		if( npc->act_wait > 30 ){
			if( npc->count1 > 3 ) npc->act_no = 240;
			else                  npc->act_no = 201;
		}
		break;

	case 230:// 下へ
		npc->act_no   = 231;
		npc->act_wait = 0;
		npc->ani_no   = 8;
		npc->ani_wait = 0;
		npc->xm       = 0;
		npc->damage   = 10;
		npc->direct   = DIR_RIGHT;
		PlaySoundObject( WAVE_BUNRET, 1 );
	case 231:
		npc->ym       =  4*VS;
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 8;
		else                    npc->ani_no = 9;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 232;
			npc->act_wait =   0;
			npc->damage   =   3;
			if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
			for( i = 0; i < 8; i++ ){
				x = npc->x + Random(-16, 16)*VS;
				SetNpChar( 4, x, npc->y + 10*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			}
			SetNpChar( 332, npc->x -12*VS, npc->y+12*VS, 0, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
			SetNpChar( 332, npc->x +12*VS, npc->y+12*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
			SetQuake2( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		if( npc->count1 < 4 && gMC.y > npc->y - 16*VS && gMC.y < npc->y + 16*VS ){
			npc->act_no = 201;
		}
		break;

	case 232:// quake
		npc->act_wait++;
		npc->xm     = 0;
		npc->ani_no = 3;
		if( npc->act_wait > 30 ){
			if( npc->count1 > 3 ) npc->act_no = 242;
			else                  npc->act_no = 201;
		}
		break;
		
	case 240:// 落下
		npc->act_no = 241;
		npc->direct = DIR_LEFT;
	case 241:
		npc->ym += VS/4;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		npc->ani_wait++;
		if( npc->ani_wait/2%2 ) npc->ani_no = 4;
		else                    npc->ani_no = 5;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 242;
			npc->act_wait = 0;
			npc->ani_no   = 3;
			if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 242:
		npc->xm = npc->xm * 3 / 4;
		npc->ani_no   = 3;
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 110;
		}
		break;

		
	case 300:// 雷撃 ----------------------------
		//map 40x24 (10, 7) (29, 7)
		npc->act_no   = 310;
		npc->act_wait =  0;
		npc->ym       = -3*VS;
		if( npc->x > 20*VS*PARTSSIZE ){
			npc->direct = DIR_RIGHT;
			npc->tgt_x  = gMC.x;//10*VS*PARTSSIZE;
			npc->tgt_y  =  11*VS*PARTSSIZE;
		}else{
			npc->direct = DIR_LEFT;
			npc->tgt_x  = gMC.x;//29*VS*PARTSSIZE;
			npc->tgt_y  =  11*VS*PARTSSIZE;
		}
		npc->ani_wait = 0;
	case 310:
		npc->ani_wait++;
		npc->act_wait++;
		if( npc->act_wait > 200 && npc->ani_wait < 20 ) npc->direct = DIR_RIGHT;
		else                                            npc->direct = DIR_LEFT;
		if( npc->ani_wait/2%2 ) npc->ani_no = 4;
		else                    npc->ani_no = 5;

		if( npc->x < npc->tgt_x ) npc->xm += VS/8;
		else                      npc->xm -= VS/8;
		if( npc->y < npc->tgt_y ) npc->ym += VS/8;
		else                      npc->ym -= VS/8;
		if( npc->xm >  VS*2     ) npc->xm =  VS*2;
		if( npc->xm < -VS*2     ) npc->xm = -VS*2;
		if( npc->ym >  VS*2     ) npc->ym =  VS*2;
		if( npc->ym < -VS*2     ) npc->ym = -VS*2;

		if( npc->act_wait > 200 && npc->act_wait%40 == 1 ){
			npc->ani_wait = 0;
			SetNpChar( 333, gMC.x, 19*VS*PARTSSIZE, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		}
		if( npc->act_wait > 200+ 40*7 ){
			npc->act_no   = 320;
			npc->act_wait =   0;
		}
		break;

	case 320:// 連雷
		npc->xm     = 0;
		npc->ym     = 0;
		npc->direct = DIR_RIGHT;
		npc->act_wait++;
		if( npc->act_wait == 40 ) SetFlash( 0, 0, FLASHMODE_FLASH );
		if( npc->act_wait > 50 && npc->act_wait%10 == 1 ){
			x = (2 + 4*(npc->act_wait-50)/10)*VS*PARTSSIZE;
			SetNpChar( 333, x, 19*VS*PARTSSIZE, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		}
		if( npc->act_wait > 50+ 10*9 ){
			npc->act_no = 240;
		}
		npc->ani_wait++;
		if( npc->ani_wait/2%2 ) npc->ani_no = 4;
		else                    npc->ani_no = 5;
		break;




	case 1000:// やられ落下
		npc->act_no   =   1001;
		npc->act_wait =      0;
		npc->ani_no   =     10;
		npc->tgt_x    = npc->x;
		npc->xm       =      0;
		npc->bits    &= ~BITS_BANISH_DAMAGE;
		SetDestroyNpChar( npc->x, npc->y, 16, 16 );
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
	case 1001:
		npc->ym += VS/16;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		npc->act_wait++;
		if( npc->act_wait / 2 % 2 )npc->x = npc->tgt_x + VS;
		else                       npc->x = npc->tgt_x - VS;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 1002;
			npc->act_wait = 0;
		}
		break;
	case 1002:
		npc->act_wait++;
		if( npc->act_wait >  150 ){
			npc->act_wait =    0;
			npc->act_no   = 1003;
			npc->ani_no   =    3;
		}
		if( npc->act_wait / 2 % 2 )npc->x = npc->tgt_x + VS;
		else                       npc->x = npc->tgt_x - VS;
		break;
	case 1003:// 屈み
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_wait =    0;
			npc->act_no   = 1004;
			npc->ani_no   =    3;
			npc->ym      -= VS*5;
			npc->direct   = DIR_LEFT;
			npc->bits  |= BITS_THROW_BLOCK;
		}
		break;
	case 1004:// 発射
		if( npc->y < 0 ){
			npc->xm       =    0;
			npc->ym       =    0;
			npc->act_no   = 1005;
			npc->act_wait =    0;
			SetFlash( 0, 0, FLASHMODE_FLASH );
			PlaySoundObject( WAVE_WARP, 1 );
		}
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 8;
		else                    npc->ani_no = 9;
		break;
	case 1005:
//		if( npc->act_wait < 41 ) npc->act_wait++;
		// ターゲットサイト
//		if( npc->act_wait == 30 )
		break;
	}
	
	npc->x += npc->xm;
	npc->y += npc->ym;


	RECT rcLeft[] = {
		{  0,  0, 48, 40},//0 立ち
		{ 48,  0, 96, 40},//1 構え
		{ 96,  0,144, 40},
		{144,  0,192, 40},//3 屈み
		{192,  0,240, 40},//4 空中
		{240,  0,288, 40},
		
		{  0, 80, 48,120},//6 ダッシュ
		{ 48, 80, 96,120},
		{ 96, 80,144,120},//8 ロケット
		{144, 80,192,120},
		{192, 80,240,120},//10 ギャー
	};

	RECT rcRight[] = {
		{  0, 40, 48, 80},// 立ち
		{ 48, 40, 96, 80},// 構え
		{ 96, 40,144, 80},
		{144, 40,192, 80},// 屈み
		{192, 40,240, 80},// 4 空中
		{240, 40,288, 80},

		{  0,120, 48,160},// ダッシュ
		{ 48,120, 96,160},
		{ 96,120,144,160},// プレス
		{144,120,192,160},
		{192,120,240,160},//10 ギャー
	};

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[  npc->ani_no ];
	else                          npc->rect = rcRight[ npc->ani_no ];
}

// ボロスの表情
void ActNpc341( NPCHAR *npc )
{
	RECT rc[] = {
		{288, 32,320, 48},
		{288, 48,320, 64},
		{288, 64,320, 80},
	};

	if( npc->pNpc->act_no == 11 &&
		npc->pNpc->act_wait > 50 ){
		npc->ani_wait++;
	}
	if( npc->ani_wait > 4 ){
		npc->ani_wait = 0;
		if( npc->ani_no < 2 ) npc->ani_no++;
	}
	if( npc->pNpc->ani_no ){
		npc->cond = 0;
	}
	npc->rect = rc[npc->ani_no];
}

// ボロスサークル
#define NPC342_LIFE 100
void ActNpc342( NPCHAR *npc )
{
	static long _flash;
	RECT rc[] = {
		{240, 48,280, 88},// 開き
		{240, 88,280,128},// フラッシュ
		{280, 48,320, 88},// 閉じ
	};
	unsigned char deg;

	if( npc->act_no < 1000 && npc->pNpc->act_no >= 1000 ){
		npc->act_no = 1000;
	}

	switch( npc->act_no ){
	case 0: // 登場
		npc->act_no   = 10;
		npc->count1   = (npc->direct & 0xff) * 2;
		npc->direct   >>= 8;
		npc->count2   =  48*4;
		npc->damage   =  14;
	case 10:
		if( npc->count2 < 80*4 ) npc->count2 += 2*4;
		else                     npc->act_no  = 11;
		break;
	case 11:
		if( npc->count2 > 76*4 ) npc->count2 -= 1*4;
		else                     npc->act_no  = 12;
		break;
	case 12:
		if( npc->pNpc->act_no == 311 ) npc->act_no = 20;
		break;

	case 20: //歩行回転 ---------------------------
		npc->act_no = 21;
		npc->bits  |= BITS_BANISH_DAMAGE;
		npc->life   = 1000;
	case 21:
		npc->count1 -= 2;
		if( npc->count1 < 0 ) npc->count1 += 256*2;
		if( npc->shock ){
			if( (++_flash)/2%2 ) npc->ani_no = 1;
			else                 npc->ani_no = 0;
		}else                    npc->ani_no = 0;

		if( npc->life < 1000 - NPC342_LIFE ){
			npc->act_no = 22;
			npc->bits  &= ~BITS_BANISH_DAMAGE;
			SetDestroyNpChar( npc->x, npc->y, 16*VS, 32 );
			PlaySoundObject( WAVE_DESTROY_MIDDLE, 1 );
		}
		npc->pNpc->count1 = 4;//生きてる証
		if( npc->pNpc->act_no == 401 ) npc->act_no = 23;
		break;
	case 22:
		npc->ani_no  = 2;
		npc->count1 -= 2;
		if( npc->count1 < 0 ) npc->count1 += 256*2;
		if( npc->pNpc->act_no == 401 ) npc->act_no = 23;
		break;
	case 23:
		npc->ani_no  = 2;
		npc->count1 -= 4;
		if( npc->count1 < 0 ) npc->count1 += 256*2;
		if( npc->pNpc->act_no == 420 ) npc->act_no = 30;
		break;

	case 30: // 回転 (96) --------------------------
		npc->act_no  =   31;
		npc->life    = 1000;
		npc->damage  =   10;
		if( npc->direct == DIR_LEFT ) npc->bits |= BITS_BANISH_DAMAGE;
		npc->ym      =    0;
	case 31:
		npc->count1 += 1;
		npc->count1 = npc->count1%(256*2);
		if( npc->count2 > 64*4 ) npc->count2--;

		if( npc->bits & BITS_BANISH_DAMAGE ){
			if( npc->shock ){
				if( (++_flash)/2%2 ) npc->ani_no = 1;
				else                 npc->ani_no = 0;
			}else                    npc->ani_no = 0;
		}else{
			npc->ani_no = 2;
		}

		if( npc->life < 1000 - NPC342_LIFE ) npc->act_no = 40;
		break;
	case 40: // バウンド
		npc->act_no = 41;
		npc->xm     =  0;
		npc->ym     =  0;
		npc->ani_no =  2;
		npc->damage =  5;
		npc->bits  &= ~(BITS_BANISH_DAMAGE|BITS_THROW_BLOCK);
		SetDestroyNpChar( npc->x, npc->y, 16*VS, 32 );
		PlaySoundObject( WAVE_DESTROY_MIDDLE, 1 );
	case 41:
		if( npc->flag & FLAG_HIT_LEFT  ) npc->xm =  VS/2;
		if( npc->flag & FLAG_HIT_RIGHT ) npc->xm = -VS/2;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			if( !npc->xm ){
				if( gMC.x < npc->x ) npc->xm =  VS/2;
				else                 npc->xm = -VS/2;
			}
			npc->ym = -VS * 4;
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		npc->ym += VS / 16;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		break;

	case 1000:
		npc->act_no = 1001;
		npc->xm     =    0;
		npc->ym     =    0;
		npc->ani_no =    2;
		npc->bits  &= ~(BITS_BANISH_DAMAGE|BITS_THROW_BLOCK);
		npc->damage =    0;
		npc->count1 = npc->count1/4;
		npc->exp    = 0;
	case 1001:
		if( npc->count1 > 0 ){
			npc->count1--;
			if( (npc->count1)/2%2 ) npc->ani_no = 1;
			else                    npc->ani_no = 0;
		}else{
			SetDestroyNpChar( npc->x, npc->y, 16*VS, 32 );
			PlaySoundObject( WAVE_DESTROY_MIDDLE, 1 );
			VanishNpChar( npc );
			return;
		}
		break;
	}

	// 歩き埃
	if( npc->act_no == 21 || npc->act_no == 22 ){
		switch( npc->pNpc->direct ){
		case DIR_LEFT:
			if( npc->count1 == 64*2+6*2 ){
				SetNpChar( 4, npc->x+8*VS, npc->y+12*VS, 0,0,0,NULL,MAX_NPC/2 );
				SetNpChar( 4, npc->x-8*VS, npc->y+12*VS, 0,0,0,NULL,MAX_NPC/2 );
				PlaySoundObject( WAVE_QUAKE, 1 );	
			}
			break;
		case DIR_UP:
			if( npc->count1 == 128*2+6*2 ){
				SetNpChar( 4, npc->x-12*VS, npc->y+8*VS, 0,0,0,NULL,MAX_NPC/2 );
				SetNpChar( 4, npc->x-12*VS, npc->y-8*VS, 0,0,0,NULL,MAX_NPC/2 );
				PlaySoundObject( WAVE_QUAKE, 1 );	
			}
			break;
		case DIR_RIGHT:
			if( npc->count1 == 192*2+6*2 ){
				SetNpChar(   4, npc->x+8*VS, npc->y-12*VS, 0,0,0,NULL,MAX_NPC/2 );
				SetNpChar(   4, npc->x-8*VS, npc->y-12*VS, 0,0,0,NULL,MAX_NPC/2 );
				SetNpChar( 345, npc->x-8*VS, npc->y-12*VS, 0,0,0,NULL,MAX_NPC/2 );
				PlaySoundObject( WAVE_QUAKE, 1 );	
			}
			break;
		case DIR_DOWN:
			if( npc->count1 ==  0*2+6*2 ){
				SetNpChar( 4, npc->x+12*VS, npc->y+8*VS, 0,0,0,NULL,MAX_NPC/2 );
				SetNpChar( 4, npc->x+12*VS, npc->y-8*VS, 0,0,0,NULL,MAX_NPC/2 );
				PlaySoundObject( WAVE_QUAKE, 1 );	
			}
			break;
		}
	}

	// 回転座標設定
	if( npc->act_no < 40 ){
		deg         = (unsigned char)( npc->count1 / 2 );
		npc->tgt_x = npc->pNpc->x + GetCos( deg ) * npc->count2 / 4 ;
		npc->tgt_y = npc->pNpc->y + GetSin( deg ) * npc->count2 / 4 ;
		npc->xm = npc->tgt_x - npc->x;
		npc->ym = npc->tgt_y - npc->y;
	}
	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];

}


// ボロスダミーボディ
void ActNpc343( NPCHAR *npc )
{
	RECT rc = {0,0,120,120};
	npc->rect = rc;
	npc->act_wait++;
	if( npc->act_wait > 100 ) npc->cond = 0;
	npc->x = npc->pNpc->x;
	npc->y = npc->pNpc->y;
}

// ボロスダミーアイ
void ActNpc344( NPCHAR *npc )
{
	RECT rc[] = {
		{272,0,296,16},
		{296,0,320,16},
	};
	if( npc->direct == DIR_LEFT ){
		npc->rect = rc[0];
		npc->x = npc->pNpc->x - 24*VS;
	}else{
		npc->rect = rc[1];
		npc->x = npc->pNpc->x + 24*VS;
	}
	npc->act_wait++;
	if( npc->act_wait > 100 ) npc->cond = 0;
	npc->y = npc->pNpc->y-36*VS;
}

//ボンヘッド
void ActNpc345( NPCHAR *npc )
{
	RECT rc[] = {
		{128,176,144,192},
		{144,176,160,192},
		{160,176,176,192},
		{176,176,192,192},
	};
	long i;

	switch( npc->act_no ){
	case 0:
		npc->act_no = 100;
		npc->ani_no = Random( 0, 16 ) % 4;

	case 100:// 落下
		npc->ym += VS/8;
		if( npc->ym > VS*3+VS/2 )
			npc->ym = VS*3+VS/2;

		if( npc->y > VS*PARTSSIZE*8 )
			npc->bits &= ~BITS_THROW_BLOCK;

		if( (npc->act_wait++)/2%2 )
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );


		if( npc->flag & FLAG_HIT_BOTTOM ){

			npc->ym     = -VS;
			npc->act_no = 110;
			npc->bits |= BITS_THROW_BLOCK;
			PlaySoundObject( WAVE_BREAK1, 1 );
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
	npc->ani_wait++;
	if( npc->ani_wait > 8 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3 )
		npc->ani_no = 0;

	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];

}


// ボロスリフト
void ActNpc346( NPCHAR *npc )
{
	static long _flash;
	RECT rc[] = { {240, 0,272, 16}, };
	unsigned char deg;

	if( npc->act_no < 1000 && npc->pNpc->act_no >= 1000 ){
		npc->act_no = 1000;
	}
	switch( npc->act_no ){
	case 0: // 登場
		npc->act_no   = 10;
		npc->count1   = npc->direct*4;
		npc->count2   = 48*4;
		npc->ani_no   = 0;//y移動量
	case 10:
		if( npc->count2 < 112*4 ) npc->count2 += 2*4;
		else                      npc->act_no  = 11;
		break;
	case 11:
		if( npc->pNpc->act_no == 411 ) npc->act_no = 20;
		break;

	case 20: //右回転１
		npc->count1 -= 1;
		if( npc->count1 < 0 ) npc->count1 += 256*4;
		if( npc->pNpc->act_no == 421 ) npc->act_no =  40;
		if( npc->pNpc->act_no == 423 ) npc->act_no = 100;
		break;
	case 30: //左回転１
		npc->count1 += 1;
		npc->count1 = npc->count1 % ( 256*4 );
		if( npc->pNpc->act_no == 425 ) npc->act_no =  50;
		if( npc->pNpc->act_no == 427 ) npc->act_no = 100;
		break;
	case 40: //右回転２
		npc->count1 -= 2;
		if( npc->count1 < 0 ) npc->count1 += 256*4;
		if( npc->pNpc->act_no == 422 ) npc->act_no =  20;
		break;
	case 50: //左回転２
		npc->count1 += 2;
		npc->count1 = npc->count1 % ( 256*4 );
		if( npc->pNpc->act_no == 426 ) npc->act_no =  30;
		break;
	case 100:// 停止
		npc->ani_no = 0;
		if( npc->pNpc->act_no == 424 ) npc->act_no =  30;
		if( npc->pNpc->act_no == 428 ) npc->act_no =  20;
		break;
		
	case 1000: // 落下 -----------------
		npc->act_no = 1001;
		npc->xm = 0;
		npc->ym = 0;
		npc->bits &= ~BITS_BLOCK_MYCHAR2;
	case 1001:
		npc->ym += VS/8;
		if( npc->y > gMap.length*PARTSSIZE*VS ){
			npc->cond = 0;
		}
		break;
	}

	if( npc->act_no < 1000 ){
		if( gMC.y > npc->y - 8*VS && gMC.ym < 0 ) npc->bits &= ~BITS_BLOCK_MYCHAR2;
		else                                      npc->bits |=  BITS_BLOCK_MYCHAR2;

		// 座標設定
		deg        = (unsigned char)(npc->count1/4);
		npc->tgt_x = npc->pNpc->x +         GetCos( deg ) * npc->count2 / 4 ;
		npc->tgt_y = npc->pNpc->y + 16*VS + GetSin( deg ) * npc->count2 / 4 ;
		npc->xm    = npc->tgt_x - npc->x;

		if( npc->act_no == 20 || npc->act_no == 30 ){
			if( !(npc->count1%4) ){
				npc->ani_no = (npc->tgt_y - npc->y)/4;
			}
		}else if( npc->act_no == 40 || npc->act_no == 50 ){
			if( !(npc->count1/2%2) ){
				npc->ani_no = (npc->tgt_y - npc->y)/2;
			}
		}else{
			npc->ani_no = (npc->tgt_y - npc->y);
		}
		npc->ym = npc->ani_no;
	}

	npc->x    += npc->xm;
	npc->y    += npc->ym;

	npc->rect = rc[0];
}



// 月光虫
void ActNpc347( NPCHAR *npc )
{
	switch( npc->act_no ){
	case  0:
		npc->act_no = 1;
	case  1://待機
		npc->ani_no = 0;
		if( gMC.y < npc->y + 8*VS*PARTSSIZE &&
			gMC.y > npc->y - 8*VS*PARTSSIZE ){
			npc->act_no   = 10;
			npc->act_wait =  0;
			npc->ani_no   =  1;
		}
		break;
	case 10://屈み
		npc->act_wait++;
		if( npc->act_wait == 4 ) npc->ani_no = 2;
		if( npc->act_wait > 12 ){
			npc->act_no = 12;
			npc->xm     = VS * 7 / 2;
			PlaySoundObject( WAVE_NPCJUMP2, 1 );
			npc->ani_no = 3;
		}
		break;
	case 12:// 空中
		if( gMC.y < npc->y ) npc->ym = -VS/3;
		else                 npc->ym =  VS/3;
		if( npc->flag & FLAG_HIT_LEFT ){
			npc->act_no   = 13;
			npc->act_wait =  0;
			npc->ani_no   =  2;
			npc->xm       =  0;
			npc->ym       =  0;
			break;
		}
		npc->xm -= VS/12;
		if( npc->xm < -MAX_MOVE )
			npc->xm = -MAX_MOVE;
		npc->x  += npc->xm;
		npc->y  += npc->ym;
		break;

	case 13:// 着陸
		npc->act_wait++;
		if( npc->act_wait == 2 ) npc->ani_no = 1;
		if( npc->act_wait == 6 ) npc->ani_no = 0;
		if( npc->act_wait > 16 ) npc->act_no = 1;
		break;
	}
	
	RECT rc[] = {
		{256, 48,272, 64},
		{272, 48,288, 64},// 屈む
		{288, 48,304, 64},
		{304, 48,320, 64},// ジャンプ
	};
	npc->rect = rc[npc->ani_no];
}

// 地獄の槍
void ActNpc348( NPCHAR *npc )
{
	RECT rc[] = {
		{128,152,160,176},
		{160,152,192,176},
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		npc->act_wait++;
		if( npc->act_wait < 4 * 32 ){
			npc->y -= VS / 4;
			if( npc->act_wait/2%2 ) npc->ani_no = 1;
			else                    npc->ani_no = 0;
		}else{
			npc->act_no = 10;
			npc->ani_no =  0;
			npc->damage =  2;
		}
		break;
	case 10:
		break;
	}
	npc->rect = rc[npc->ani_no];
}

// ダミー２
void ActNpc349( NPCHAR *npc )
{
	RECT rect = {0,0,16,16};

	if( !npc->act_no ){
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT  ) npc->x +=  8*VS;
		if( npc->direct == DIR_RIGHT ) npc->y += 16*VS;
	}
	
	npc->rect = rect;
}


// 翼アローエンジェル
void ActNpc350( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,160, 24,184},// 移動
		{ 24,160, 48,184},
		{ 48,160, 72,184},// 2構え
		{ 72,160, 96,184},
		{ 96,160,120,184},// 4flash
		{120,160,144,184},// 5撃った
		{144,160,168,184},
	};
	RECT rcRight[] = {
		{  0,184, 24,208},// 移動
		{ 24,184, 48,208},
		{ 48,184, 72,208},// 2構え
		{ 72,184, 96,208},
		{ 96,184,120,208},// 4flash
		{120,184,144,208},// 5撃った
		{144,184,168,208},
	};
	switch( npc->act_no ){
	case 0:// 登場
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ) npc->tgt_x = npc->x - 8*PARTSSIZE*VS;
		else                          npc->tgt_x = npc->x + 8*PARTSSIZE*VS;
		npc->tgt_y = npc->y;
		npc->ym = Random( -VS, VS )*2;
		npc->xm = Random( -VS, VS )*2;
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 1 )
				npc->ani_no = 0;
		}
		if( npc->direct == DIR_LEFT ){
			if( npc->x < npc->tgt_x ) npc->act_no  = 20;
		}else{
			if( npc->x > npc->tgt_x ) npc->act_no  = 20;
		}
		break;
	case 20:// 構え
		npc->act_no   = 21;
		npc->act_wait =  Random( 0, 150 );
		npc->ani_no   =  2;
		npc->ani_wait =  0;
	case 21:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 3 )
				npc->ani_no = 2;
		}
		npc->act_wait++;
		if( npc->act_wait > 300 ){
			npc->act_no = 30;
		}
		if( gMC.x < npc->x + 7*VS*PARTSSIZE &&
			gMC.x > npc->x - 7*VS*PARTSSIZE &&
			gMC.y < npc->y + 1*VS*PARTSSIZE &&
			gMC.y > npc->y - 1*VS*PARTSSIZE )
		{
			npc->act_no = 30;
		}
		break;
	case 30:// 点滅発射
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->ani_wait =  0;
	case 31:
		npc->ani_wait++;
		if( npc->ani_wait/2%2 ) npc->ani_no = 3;
		else                    npc->ani_no = 4;
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 40;
			npc->ani_no   =  5;
			if( npc->direct == DIR_LEFT ) SetNpChar( 312, npc->x, npc->y, -4*VS, 0, DIR_LEFT,  NULL, MAX_NPC*4/5 );
			else                          SetNpChar( 312, npc->x, npc->y,  4*VS, 0, DIR_RIGHT, NULL, MAX_NPC*4/5 );
		}

		break;
	case 40:// 余韻
		npc->act_no   = 41;
		npc->act_wait =  0;
		npc->ani_wait =  0;
	case 41:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 6 )
				npc->ani_no = 5;
		}
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_no = 50;
			npc->ani_no = 0;
			npc->xm = 0;
			npc->ym = 0;
		}
		break;
	case 50:// さらば
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 1 )
				npc->ani_no = 0;
		}
		if( npc->direct == DIR_LEFT ) npc->xm -= VS/16;
		else                          npc->xm += VS/16;

		if( npc->x < 0 || npc->x > gMap.width *VS*PARTSSIZE ){
			VanishNpChar( npc );
			return;
		}
		break;
	}
	if( npc->act_no < 50 ){
		if( npc->x < npc->tgt_x ) npc->xm += VS/12;
		if( npc->x > npc->tgt_x ) npc->xm -= VS/12;
		if( npc->y < npc->tgt_y ) npc->ym += VS/12;
		if( npc->y > npc->tgt_y ) npc->ym -= VS/12;
		if( npc->xm >  VS*2 ) npc->xm =  VS*2;
		if( npc->xm < -VS*2 ) npc->xm = -VS*2;
		if( npc->ym >  VS*2 ) npc->ym =  VS*2;
		if( npc->ym < -VS*2 ) npc->ym = -VS*2;
	}
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[  npc->ani_no ];
	else                          npc->rect = rcRight[ npc->ani_no ];
}

// 石像
void ActNpc351( NPCHAR *npc )
{
	RECT rc[] = {
		{  0, 96, 32,136},
		{ 32, 96, 64,136},
		{ 64, 96, 96,136},
		{ 96, 96,128,136},
		{128, 96,160,136},

		{  0,176, 32,216},
		{ 32,176, 64,216},
		{ 64,176, 96,216},
		{ 96,176,128,216},
	};
	
	switch( npc->act_no ){
	case  0:
		npc->act_no  =  1;
		npc->ani_no  = npc->direct/10;
		npc->x      +=  8*VS;
		npc->y      += 12*VS;
	case  1:
		break;
	case 10:// 破壊可 or 最彫刻
		if( GetNPCFlag( npc->code_flag ) ){
			npc->act_no = 20;
		}else{
			npc->act_no = 11;
			npc->bits |= BITS_BANISH_DAMAGE;
		}
	case 11:
		if( npc->life <= 1000 - 100 ){
			SetNpChar( 351, npc->x - 8*VS, npc->y - 12*VS, 0, 0, (npc->ani_no-1+5)*10, NULL, 0 );
			npc->cond |= COND_LOSE;
		}
		break;
	case 20:
		npc->ani_no = npc->ani_no-1+5;
		npc->act_no = 1;
		break;
	}
	npc->rect = rc[  npc->ani_no ];
}

// キャスト
void ActNpc352( NPCHAR *npc )
{
// x100で指定
// 0  r キング、
// 1  r トロ子、
// 2 *r 数馬、
// 3  r スー、
// 4 *r 百鈴、
// 5  r ブースター、
// 6  r ジェンカ

// 7  g 蓮美
// 8  g Drゲロ
// 9 *g バルログ
//10  r カーリー
//11  r ミザリー
//12 *g マルコ
//13  g はぐれ銃鍛冶
	switch( npc->act_no ){
	case   0:
		npc->act_no = 1;
		npc->ani_no = 0;
		npc->count1 = npc->direct/100;
		npc->direct = npc->direct%100;
		switch( npc->count1 ){
		case 7: case 8: case 9: case 12: case 13:
			npc->surf = SURF_NPC_ENEMY;
			break;
		}
		switch( npc->count1 ){
		case 2: case 4: case 9: case 12:
			npc->view.top = 16*VS;
			break;
		}
		if( npc->count1 == 9 ){
			npc->view.back  = 20*VS;
			npc->view.front = 20*VS;
			npc->x -= VS*1;
		}
		if( npc->count1 == 0 ){
			SetNpChar( 145, 0, 0, 0, 0, DIR_RIGHT, npc, MAX_NPC/2 );
		}
	case   1:
		npc->ym += VS/8;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym     = 0;
			npc->act_no = 2;
			npc->ani_no = 1;
		}
		npc->y += npc->ym;
		break;
	case 2:
		break;
	}
	
	RECT rc[] = {
		{304, 48,320, 64},//king
		{224, 48,240, 64},
		{ 32, 80, 48, 96},//toro
		{  0, 80, 16, 96},
		{224,216,240,240},//kazuma
		{192,216,208,240},
		{ 48, 16, 64, 32},//su-
		{  0, 16, 16, 32},
		{112,192,128,216},//momo
		{ 80,192, 96,216},
		{304,  0,320, 16},//booster
		{224,  0,240, 16},
		{176, 32,192, 48},//jenka
		{176, 32,192, 48},

		{240, 16,256, 32},//hasumi
		{224, 16,240, 32},
		{208, 16,224, 32},//gero
		{192, 16,208, 32},
		{280,128,320,152},//bllg
		{280,152,320,176},
		{ 32,112, 48,128},//curly
		{  0,112, 16,128},
		{ 80,  0, 96, 16},//miza
		{112,  0,128, 16},
		{ 16,152, 32,176},//malco
		{  0,152, 16,176},
		{ 48, 16, 64, 32},//kaji
		{ 48,  0, 64, 16},
	};
	
	npc->rect = rc[npc->count1*2 + npc->ani_no];
}

// きりもみ飛行赤天使
void ActNpc353( NPCHAR *npc )
{
	RECT rc[] = {
		{168,160,184,184},// きりもみ
		{184,160,200,184},
		{168,184,184,208},
		{184,184,200,208},
	};
	
	RECT rcLeft[] = {
		{200,160,216,176},
		{216,160,232,176},
	};
	RECT rcRight[] = {
		{200,176,216,192},
		{216,176,232,192},
	};
	
	switch( npc->act_no ){
	case 0: // きりもみ登場
		npc->act_no = 1;
		switch( npc->direct ){
		case DIR_LEFT:  npc->xm = -3*VS; break;
		case DIR_RIGHT: npc->xm = +3*VS; break;
		case DIR_UP:    npc->ym = -3*VS; break;
		case DIR_DOWN:  npc->ym = +3*VS; break;
		}
	case 1:
		npc->act_wait++;
		if( npc->act_wait == 8 ) npc->bits &= ~BITS_THROW_BLOCK; 
		npc->x += npc->xm;
		npc->y += npc->ym;
		
		if( npc->act_wait == 16 ){
			npc->act_no = 10;
		}
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 3 )
				npc->ani_no = 0;
		}
		npc->rect = rc[npc->ani_no];
		break;

	case 10: // 飛行
		npc->act_no    = 11;
		npc->ani_no    = 0;
		npc->bits     |=  BITS_BANISH_DAMAGE;
		npc->bits     &= ~BITS_THROW_BLOCK; 
		npc->damage    = 5;
		npc->view.top  = 8*VS;
	case 11:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( gMC.y -24*VS > npc->y ){
			if( npc->direct == DIR_LEFT ) npc->xm2 += VS/32;
			else                          npc->xm2 -= VS/32;
		}else{
			if( npc->direct == DIR_LEFT ) npc->xm2 -= VS/32;
			else                          npc->xm2 += VS/32;
		}
		if( npc->y > gMC.y          ) npc->ym2 -= VS/32;
		else                          npc->ym2 += VS/32; 

		// はねっかえり
		if( npc->xm2 < 0 && npc->flag & FLAG_HIT_LEFT   ){
			npc->xm2 = npc->xm2 * -1;
		}
		if( npc->xm2 > 0 && npc->flag & FLAG_HIT_RIGHT  ){
			npc->xm2 = npc->xm2 * -1;
		}
		if( npc->ym2 < 0 && npc->flag & FLAG_HIT_TOP    ){
			npc->ym2 = npc->ym2 * -1;
		}
		if( npc->ym2 > 0 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym2 = npc->ym2 * -1;
		}
		if( npc->xm2 < -MAX_MOVE ) npc->xm2 = -MAX_MOVE;
		if( npc->xm2 >  MAX_MOVE ) npc->xm2 =  MAX_MOVE;
		if( npc->ym2 < -MAX_MOVE ) npc->ym2 = -MAX_MOVE;
		if( npc->ym2 >  MAX_MOVE ) npc->ym2 =  MAX_MOVE;

		npc->x += npc->xm2;
		npc->y += npc->ym2;

		// ani
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 1 )
				npc->ani_no = 0;
		}
		if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[  npc->ani_no ];
		else                          npc->rect = rcRight[ npc->ani_no ];
		break;
	}
}

// 迫る壁
void ActNpc354( NPCHAR *npc )
{
	long i, x, y;

	switch( npc->act_no ){
	case  0:
		npc->hit.bottom = 280*VS;
	case  1:
		break;
	case 10:
		npc->act_no   = 11;
		npc->act_wait =  0;
		if( npc->direct == DIR_LEFT ) npc->x += 16 * VS;
		else                          npc->x -= 16 * VS;
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 50*2 ){
			npc->act_wait =  0;
			SetQuake( 20 );
			PlaySoundObject( WAVE_QUAKE,  1 );
			PlaySoundObject( WAVE_BREAK1, 1 );
			if( npc->direct == DIR_LEFT ) npc->x -= 16 * VS;
			else                          npc->x += 16 * VS;
			for( i = 0; i < 20; i++ ){
				x = npc->x / VS / PARTSSIZE;
				y = npc->y / VS / PARTSSIZE + i;
				ChangeMapParts( x, y, 109 );
			}
		}
		break;
	}
}

// くっつきクォート／カーリー
void ActNpc355( NPCHAR *npc )
{
	RECT rc[] = {
		{ 80, 16, 96, 32},//クォート
		{ 80, 96, 96,112},// カーリー
		{128, 16,144, 32},//クォート
		{208, 96,224,112},// カーリー
	};
	switch( npc->act_no ){
	case 0:
		switch( npc->direct ){
		case 0:
			npc->surf = SURF_MYCHAR;
			npc->ani_no = 0;
			npc->x = npc->pNpc->x - 14*VS;
			npc->y = npc->pNpc->y +  10*VS;
			break;
		case 1:
			npc->surf   = SURF_NPC_REGULAR;
			npc->ani_no = 1;
			npc->x = npc->pNpc->x + 14*VS;
			npc->y = npc->pNpc->y +  10*VS;
			break;
		case 2:
			npc->surf = SURF_MYCHAR;
			npc->ani_no = 2;
			npc->x = npc->pNpc->x - 7*VS;
			npc->y = npc->pNpc->y -  19*VS;
			break;
		case 3:
			npc->surf   = SURF_NPC_REGULAR;
			npc->ani_no = 3;
			npc->x = npc->pNpc->x + 4*VS;
			npc->y = npc->pNpc->y -  19*VS;
			break;
		}
		break;
	
	}
	npc->rect = rc[npc->ani_no];	
}

// パタパタバルログ
void ActNpc356( NPCHAR *npc )
{
	RECT rcRight[] = {
		{240,128,280,152},
		{240,152,280,176},
	};

	switch( npc->act_no ){
	case  0://
		npc->act_no   = 11;
		npc->ani_wait = 0;
		npc->tgt_y    = npc->y - VS*16;
		npc->tgt_x    = npc->x - VS* 6;
		npc->ym       = 0;
		SetNpChar( 355, 0, 0, 0, 0, 3, npc, MAX_NPC/3 );
		SetNpChar( 355, 0, 0, 0, 0, 2, npc, MAX_NPC/3 );
	case 11:// 滞空
		if( npc->x < npc->tgt_x ) npc->xm += VS/64;
		else                      npc->xm -= VS/64;
		if( npc->y < npc->tgt_y ) npc->ym += VS/64;
		else                      npc->ym -= VS/64;
		npc->x += npc->xm;
		npc->y += npc->ym;
		break;
	case 20:// 直行！
		npc->act_no = 21;
		npc->xm = -2*VS;
		npc->ym = +VS;
	case 21:
		npc->ani_wait++;
		npc->xm += VS/32;
		npc->ym -= VS/64;
		npc->x += npc->xm;
		npc->y += npc->ym;
		if( npc->x > 60*VS*PARTSSIZE ){
			npc->act_no = 22;
		}
		break;
	case 22:
		npc->xm = 0;
		npc->ym = 0;
		break;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 4 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	//	if( npc->ani_no == 1 ) PlaySoundObject( WAVE_SONIC, 1 );
	}
	if( npc->ani_no > 1 )
		npc->ani_no = 0;

	npc->rect = rcRight[npc->ani_no]; 
}

// ブルードッグ
void ActNpc357( NPCHAR *npc )
{
	RECT rc = {224, 136, 240, 152};
	switch( npc->act_no ){
	case 0:
		npc->rect = rc;
		npc->act_wait++;
		break;
	case 10:
		npc->act_wait = 0;
		npc->act_no = 11;
		PlaySoundObject( WAVE_WARP, 1 );
	case 11:
		npc->act_wait++;
		npc->rect = rc;
		if( npc->act_wait/2%2 ) npc->rect.right = npc->rect.left;
		if( npc->act_wait > 50 ){
//			for( long i = 0; i < 3; i++ ){
//				SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, 0 );
//			}
			npc->cond = 0;
		}
		break;
	}
	if( npc->act_wait%8 == 1 )
		SetCaret( npc->x + Random( -8, 8 )*VS, npc->y + 8*VS, CARET_SMALLSTAR, DIR_UP );
}

//長身ミザリー
void ActNpc358( NPCHAR *npc )
{
	RECT rc[] = {
		{208,  8, 224, 32},// 横
		{224,  8, 240, 32},
		{240,  8, 256, 32},
		{256,  8, 272, 32},// こっち
		{272,  8, 288, 32},
	};
	
	switch( npc->act_no ){
	case  0:
		npc->ani_wait++;
		if( npc->ani_wait > 6 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	case 10:// 振り向き
		npc->ani_wait++;
		if( npc->ani_wait > 6 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )
			npc->ani_no = 3;
		break;
	}
	
	npc->rect = rc[ npc->ani_no ];
	
}

// 無害水滴配置
void ActNpc359( NPCHAR *npc )
{
	long x;
	if( gMC.x < npc->x +20*VS*PARTSSIZE &&
		gMC.x > npc->x -20*VS*PARTSSIZE &&
		gMC.y < npc->y +20*VS*PARTSSIZE &&
		gMC.y > npc->y -10*VS*PARTSSIZE )
	{
		if( Random( 0, 100 ) == 2 ){
			x = Random( -6, 6 ) *VS + npc->x;
			SetNpChar( 73, x, npc->y - 7*VS, 0, 0, DIR_LEFT, NULL, 0 );
		}
	}
}

// thank you
void ActNpc360( NPCHAR *npc )
{
	RECT rc = {0,176,48,184};
	if( !npc->act_no ){
		npc->act_no++;
		npc->x -= 8*VS;
		npc->y -= 8*VS;
	}
	npc->rect = rc;
	if( gMC.equip & EQUIP_STAR ){
		rc.right += gMC.star * 8;
	}
}

