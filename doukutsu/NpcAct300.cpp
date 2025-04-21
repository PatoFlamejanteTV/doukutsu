#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"

// 悪魔の冠
void ActNpc300( NPCHAR *npc )
{
	RECT rc = {192,80,208,96};

	if( !npc->act_no ){
		npc->act_no = 1;
		npc->y += 6*VS;
	}

	npc->ani_wait++;
	if( npc->ani_wait%8 == 1 ) SetCaret( npc->x + Random( -8, 8 )*VS, npc->y + 8*VS, CARET_SMALLSTAR, DIR_UP );
	npc->rect = rc;


}

// 誘導ミサイル
void ActNpc301( NPCHAR *npc )
{
	long dir;
	RECT rect[] = {
		{144,  0,160, 16},
		{160,  0,176, 16},
		{176,  0,192, 16},
		{192,  0,208, 16},
		{144, 16,160, 32},
		{160, 16,176, 32},
		{176, 16,192, 32},
		{192, 16,208, 32},
	};


	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->count1 = npc->direct;
	case 1:
		//移動
		npc->xm = GetCos((unsigned char)npc->count1)*2;
		npc->ym = GetSin((unsigned char)npc->count1)*2;
		npc->y += npc->ym;
		npc->x += npc->xm;

		//MyCharを追いかける
		dir = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
		if( dir < npc->count1 ){
			if( npc->count1 - dir < 128 ) npc->count1 -= 1;
			else                          npc->count1 += 1;
		}else{
			if( dir - npc->count1 < 128 ) npc->count1 += 1;
			else                          npc->count1 -= 1;

		}
		if( npc->count1 > 255 ) npc->count1 -= 256;
		if( npc->count1 <   0 ) npc->count1 += 256;
		break;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		SetCaret( npc->x, npc->y, CARET_MISSILE, 4 );
	}

	//アニメーション
	npc->ani_no = (npc->count1+16)/32;
	if(npc->ani_no > 7)npc->ani_no = 7;

	npc->rect = rect[npc->ani_no];
}

// フォーカスマン
void ActNpc302( NPCHAR *npc )
{
	long n;
	switch( npc->act_no ){
	case  0:
		break;
	case 10:// 落石よけステージ用
		npc->x = gMC.x;
		npc->y = gMC.y - VS*PARTSSIZE*(2);
		break;
	case 20:
		switch( npc->direct ){
		case 0: npc->x -= VS*2; break;
		case 1: npc->y -= VS*2; break;
		case 2: npc->x += VS*2; break;
		case 3: npc->y += VS*2; break;
		}
		gMC.x = npc->x;
		gMC.y = npc->y;
		break;
	case 30://
		npc->x = gMC.x;
		npc->y = gMC.y + VS*PARTSSIZE*(5);
		break;

	case 100: // 指定のNPCとマイキャラの間に位置する
		npc->act_no = 101;
		if( npc->direct ){
			for( n = MAX_NPC/3; n < MAX_NPC; n++ ){
				if( gNPC[ n ].cond & COND_ALIVE && gNPC[ n ].code_event == npc->direct ){
					npc->pNpc = &gNPC[ n ];
					break;
				}
			}
			if( n == MAX_NPC ){
				npc->cond = 0;
				break;
			}
		}else{
			npc->pNpc = &gBoss[0];
		}
	case 101:
		npc->x = ( gMC.x + npc->pNpc->x ) / 2;
		npc->y = ( gMC.y + npc->pNpc->y ) / 2;
		break;
	}
}

// マイキャラのマシンガン
void ActNpc303( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{216,152,232,168},
		{232,152,248,168},
	};
	RECT rcRight[] = {
		{216,168,232,184},
		{232,168,248,184},
	};

	if( !npc->pNpc ) return;

	// 配置
	if( npc->pNpc->direct == DIR_LEFT ){
		npc->direct = DIR_LEFT;
		npc->x = npc->pNpc->x - VS*8;
	}else{
		npc->direct = DIR_RIGHT;
		npc->x = npc->pNpc->x + VS*8;
	}
	npc->y = npc->pNpc->y;
	npc->ani_no = 0;

	if( npc->pNpc->ani_no == 3 ||
		npc->pNpc->ani_no == 5 ){
		npc->y -= VS;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// ガウディ（credit）
void ActNpc304( NPCHAR *npc )
{
	RECT rc[] = {
		{ 0,176,24,192},
		{24,176,48,192},
		{48,176,72,192},
		{72,176,96,192},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y += 10*VS;
	case 1:
		npc->ani_no = 0;
		break;
	case 10:// 寝
		npc->ani_no = 1;
		break;
	case 20:// じたばた
		npc->act_no = 21;
		npc->ani_no = 2;
	case 21:
		npc->ani_wait++;
		if( npc->ani_wait > 10 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;
		break;
	}
	npc->rect = rc[npc->ani_no];
}

// 仔犬コロ
void ActNpc305( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{160,144,176,160},
		{176,144,192,160},
	};
	RECT rcRight[] = {
		{160,160,176,176},
		{176,160,192,176},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y -= 16*VS;
		npc->ani_wait = Random( 0,6);
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 6 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ナースバルログ
void ActNpc306( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{240, 96,280,128},
		{280, 96,320,128},
	};
	RECT rcRight[] = {
		{160,152,200,184},
		{200,152,240,184},
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->y += 4*VS;
	case 1://待機
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;

	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;
	}
	
	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[ npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// 囚サンタ
void ActNpc307( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 32, 16, 48},
		{ 16, 32, 32, 48},
	};
	RECT rcRight[] = {
		{  0, 48, 16, 64},
		{ 16, 48, 32, 64},
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
	}

	if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
	else                 npc->direct = DIR_RIGHT;
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// スタンピー
void ActNpc308( NPCHAR *npc )
{
	unsigned char deg;
	RECT rcLeft[ ] = { {128,112,144,128}, {144,112,160,128} };
	RECT rcRight[] = { {128,128,144,144}, {144,128,160,144} };
	

	switch( npc->act_no ){
	case 0: // 待機
		npc->act_no = 1;
	case 1:
		if( gMC.x < npc->x +15*VS*PARTSSIZE &&
			gMC.x > npc->x -15*VS*PARTSSIZE &&
			gMC.y < npc->y +12*VS*PARTSSIZE &&
			gMC.y > npc->y -12*VS*PARTSSIZE ){
			npc->act_no = 10;
		}
		break;
	case 10: // 追跡(停止)
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->xm2      =  0;
		npc->ym2      =  0;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 50 ) npc->act_no = 20;

		npc->ani_wait++;
		if( npc->act_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 1 )
				npc->ani_no = 0;
		}
		// 画面外停止
		if( gMC.x > npc->x +20*VS*PARTSSIZE ||
			gMC.x < npc->x -20*VS*PARTSSIZE ||
			gMC.y > npc->y +15*VS*PARTSSIZE ||
			gMC.y < npc->y -15*VS*PARTSSIZE ){
			npc->act_no = 0;
		}
		break;
	case 20: // 追跡(移動)
		npc->act_no   = 21;
		npc->act_wait =  0;
		deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
		deg += (unsigned char)Random( -3, 3 );

		npc->ym2 = GetSin( deg ) * 2;
		npc->xm2 = GetCos( deg ) * 2;
		if( npc->xm2 < 0 ) npc->direct = DIR_LEFT;
		else               npc->direct = DIR_RIGHT;
	case 21:
		// はねっかえり
		if( npc->xm2 < 0 && npc->flag & FLAG_HIT_LEFT   ){
			npc->direct = DIR_RIGHT;
			npc->xm2 *= -1;
		}
		if( npc->xm2 > 0 && npc->flag & FLAG_HIT_RIGHT  ){
			npc->direct = DIR_LEFT;
			npc->xm2 *= -1;
		}
		if( npc->ym2 < 0 && npc->flag & FLAG_HIT_TOP    ){
			npc->ym2 *= -1;
		}
		if( npc->ym2 > 0 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym2 *= -1;
		}
		// 水面
		if( npc->flag & FLAG_HIT_WATER ) npc->ym2 = -1*VS;

		npc->x += npc->xm2;
		npc->y += npc->ym2;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no = 10;
		}


		npc->ani_no++;
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// エンジェル（飛行）
void ActNpc309( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 0, 0,16,16},
		{16, 0,32,16},
	};
	RECT rcRight[] = {
		{ 0,16,16,32},
		{16,16,32,32},
	};

	switch( npc->act_no ){
	case 0: // 待機
		npc->act_no = 1;
	case 1:
		if( npc->direct == DIR_LEFT ){
			if( gMC.x > npc->x -18*VS*PARTSSIZE &&
				gMC.x < npc->x -17*VS*PARTSSIZE ){
				npc->act_no = 10;
				break;
			}
		}else{
			if( gMC.x < npc->x +18*VS*PARTSSIZE &&
				gMC.x > npc->x +17*VS*PARTSSIZE ){
				npc->act_no = 10;
				break;
			}
		}
		return;
	// 追跡
	case 10:
		npc->act_no = 11;
		npc->bits  |= BITS_BANISH_DAMAGE;
		npc->damage = 5;
	case 11:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->direct == DIR_LEFT ) npc->xm2 -= VS/32;
		else                          npc->xm2 += VS/32;
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
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
	// →やられ
	if( npc->life <= 1000 -4 ){
		npc->code_char = 316;
		npc->act_no    =   0;
	}
}


// エンジェル（歩行）
void ActNpc310( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 32, 0, 56,16},
		{ 56, 0, 80,16},
		{ 80, 0,104,16},// 2 振りかぶり
		{104, 0,128,16},
		{128, 0,152,16},
	};
	RECT rcRight[] = {
		{ 32, 16, 56,32},
		{ 56, 16, 80,32},
		{ 80, 16,104,32},
		{104, 16,128,32},
		{128, 16,152,32},
	};

	switch( npc->act_no ){
	case 0: // 待機
		npc->act_no = 1;
		npc->bits  &= ~BITS_BANISH_DAMAGE;
		npc->bits  |= BITS_BLOCK_BULLET;
		npc->damage = 0;
	case 1:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_no = 0;
		if( gMC.x > npc->x -8*VS*PARTSSIZE &&
			gMC.x < npc->x +8*VS*PARTSSIZE &&
			gMC.y > npc->y -8*VS*PARTSSIZE &&
			gMC.y < npc->y +1*VS*PARTSSIZE ){
			npc->act_no = 10;
		}
		break;
	case 10:
		npc->xm = 0;
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->bits    &= ~BITS_BANISH_DAMAGE;
		npc->bits    |= BITS_BLOCK_BULLET;
		npc->damage   =  0;
		npc->ani_no   =  0;
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no = 20;
		}
		break;

	// 追跡
	case 20:
		npc->act_no   = 21;
		npc->act_wait =  0;
		npc->bits    &= ~BITS_BLOCK_BULLET;
		npc->bits    |= BITS_BANISH_DAMAGE;
		npc->damage   =  0;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 21:
		if( npc->direct == DIR_LEFT ) npc->xm = -VS * 2;
		else                          npc->xm =  VS * 2;

		// ani
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		npc->act_wait++;
		if( npc->act_wait > 50*1 ){
			npc->act_no = 10;
		}
		if( npc->x < gMC.x + 40*VS &&
			npc->x > gMC.x - 40*VS ){
			npc->ym     = -3 * VS/2;
			npc->xm = npc->xm/2;
			npc->ani_no =  2;
			npc->act_no = 30;
			PlaySoundObject( WAVE_NPCJUMP1, 1 );
		}
		break;

	case 30:// ジャンプ切り
		if( npc->ym > -VS/4 ){
			npc->act_no   = 31;
			npc->ani_wait =  0;
			npc->ani_no   =  3;
			npc->damage   =  9;
		}
		break;
	case 31:
		npc->ani_wait++;
		if( npc->ani_wait >  2 ){
			npc->ani_wait =  0;
			npc->ani_no   =  4;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 32;
			npc->act_wait =  0;
			npc->xm       =  0;
			npc->damage   =  3;
		}
		break;
	case 32:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 10;
			npc->damage   =  0;
		}
		break;
	}

	npc->ym += VS / 16;
	npc->x  += npc->xm;
	npc->y  += npc->ym;
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
	// →やられ
	if( npc->life <= 1000 -4 ){
		npc->code_char = 316;
		npc->act_no    =   0;
	}
}

// エンジェル（弓）
void ActNpc311( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 32, 24, 56},// 待機
		{ 24, 32, 48, 56},// 1 構え
		{ 48, 32, 72, 56},// 2 構えF
		{ 72, 32, 96, 56},// 3 放つ
		{ 96, 32,120, 56},// 4 構え斜め
		{120, 32,144, 56},// 5 構え斜めF
		{144, 32,168, 56},// 6 放つ斜め
		};

	RECT rcRight[] = {
		{  0, 56, 24, 80},// 待機
		{ 24, 56, 48, 80},// 1 構え
		{ 48, 56, 72, 80},// 2 構えF
		{ 72, 56, 96, 80},// 3 放つ
		{ 96, 56,120, 80},// 4 構え斜め
		{120, 56,144, 80},// 5 構え斜めF
		{144, 56,168, 80},// 6 放つ斜め
	};

	switch( npc->act_no ){
	case 0:// 待機
		npc->act_no = 1;
	case 1:
		if( npc->direct == DIR_LEFT ){
			if( gMC.x > npc->x - 20*VS*PARTSSIZE &&
				gMC.x < npc->x                   &&
				gMC.y > npc->y - 10*VS*PARTSSIZE &&
				gMC.y < npc->y + 10*VS*PARTSSIZE ){
				npc->act_no = 10;
			}
		}else{
			if( gMC.x > npc->x                   &&
				gMC.x < npc->x + 20*VS*PARTSSIZE &&
				gMC.y > npc->y - 10*VS*PARTSSIZE &&
				gMC.y < npc->y + 10*VS*PARTSSIZE ){
				npc->act_no = 10;
			}
		}
			
		break;

	case 10:// 構え
		npc->act_no   = 11;
	case 11:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( gMC.x > npc->x - 14*VS*PARTSSIZE &&
		    gMC.x < npc->x + 14*VS*PARTSSIZE &&
		    gMC.y > npc->y -  1*VS*8 ){
		    npc->ani_no = 1;
		    npc->count1 = 0;
	    }else{
	    	npc->ani_no = 4;
		    npc->count1 = 1;
	    }
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 20;
		}
		break;
		
	case 20:// フラッシュ
		npc->act_no   = 21;
		npc->act_wait =  0;
	case 21:
		if( npc->count1 == 0 ){
			npc->ani_no++;
			if( npc->ani_no > 2 ) npc->ani_no = 1;
		}else{
			npc->ani_no++;
			if( npc->ani_no > 5 ) npc->ani_no = 4;
		}
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 30;
		}
		break;

	case 30: // 放つ
		npc->act_no   = 31;
		npc->act_wait =  0;
		if( npc->count1 == 0 ){
			if( npc->direct == DIR_LEFT ) SetNpChar( 312, npc->x, npc->y, -3*VS, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
			else                          SetNpChar( 312, npc->x, npc->y,  3*VS, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
			npc->ani_no = 3;
		}else{
			if( npc->direct == DIR_LEFT ) SetNpChar( 312, npc->x, npc->y, -3*VS, -3*VS, DIR_LEFT,  NULL, MAX_NPC/2 );
			else                          SetNpChar( 312, npc->x, npc->y,  3*VS, -3*VS, DIR_RIGHT, NULL, MAX_NPC/2 );
			npc->ani_no = 6;
			
		}
	case 31:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no = 40;
			npc->act_wait = Random( 0, 100 );
		}
		break;
	case 40:// 休み
		npc->ani_no = 0;
		npc->act_wait++;
		if( npc->act_wait > 150 ){
			npc->act_no = 10;
		}
		if( gMC.x < npc->x - 22*VS*PARTSSIZE ||
			gMC.x > npc->x + 22*VS*PARTSSIZE ||
			gMC.y < npc->y - 15*VS*PARTSSIZE ||
			gMC.y > npc->y + 15*VS*PARTSSIZE ){
			npc->act_no   =  40;
			npc->act_wait = 0;
		}
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	// →やられ
	if( npc->life <= 1000 - 8 ){
		npc->code_char = 316;
		npc->act_no    =   0;
	}
}



// エンジェルアロー
void ActNpc312( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,160, 16,176},
		{ 16,160, 32,176},
		{ 32,160, 48,176},
		{ 48,160, 64,176},
		{ 64,160, 80,176},
	};
	RECT rcRight[] = {
		{  0,176, 16,192},
		{ 16,176, 32,192},
		{ 32,176, 48,192},
		{ 48,176, 64,192},
		{ 64,176, 80,192},
	};
	
	if( npc->act_no >  0 &&
		npc->act_no < 20 &&
		npc->flag & FLAG_HIT_X ){

		npc->act_no = 20;
	}
	
	
	switch( npc->act_no ){
	case 0:// 飛ぶ
		npc->act_no   = 1;
		npc->act_wait = 0;
		if( npc->xm < 0 ) npc->direct = DIR_LEFT;
		else              npc->direct = DIR_RIGHT;
		if( npc->ym < 0 ) npc->ani_no = 0;
		else              npc->ani_no = 2;
	case 1:
		npc->act_wait++;
		if( npc->act_wait == 4 )
			npc->bits &= ~BITS_THROW_BLOCK;
		if( npc->act_wait > 10 ){
			npc->act_no = 10;
		}
		break;

	case 10:// 失速
		npc->act_no   = 11;
		npc->ani_wait =  0;
		npc->xm       = npc->xm * 3 / 4;
		npc->ym       = npc->ym * 3 / 4;
	case 11:
		npc->ym += VS/16;
		npc->ani_wait++;
		if( npc->ani_wait > 10 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 ) npc->ani_no = 4;
		break;

	case 20:// 刺さる
		npc->act_no   = 21;
		npc->act_wait =  0;
		npc->xm       =  0;
		npc->ym       =  0;
		npc->damage   =  0;
	case 21:
		npc->act_wait++;
		if( npc->act_wait > 30 ) npc->act_no = 30;
		break;
		
	case 30:// 点滅消える
		npc->act_no   = 31;
		npc->act_wait =  0;
	case 31:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->cond = 0;
			return;
		}
		break;
	}

	
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	// 点滅
	if( npc->act_no == 31 && npc->act_wait/2%2 ){
		npc->rect.left  = 0;
		npc->rect.right = 0;
	}
}

// マピニョン
BOOL IsActiveSomeBullet( void );
void ActNpc313( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{128,  0,144, 16},
		{144,  0,160, 16},// 1 瞬き
		{160,  0,176, 16},// 2 屈み
		{176,  0,192, 16},// 3 ジャンプ！
		{192,  0,208, 16},// 4 降下
		{208,  0,224, 16},// 5 構え
		{224,  0,240, 16},// 6 ロケット横
		{240,  0,256, 16},// 7 ロケット横F
		{256,  0,272, 16},// 8 ダメージ

		{272,  0,288, 16}, // 9 走る
		{288,  0,304, 16}, //10
		{128,  0,144, 16}, //11
		
		{176,  0,192, 16}, //12 ロケット縦
		{304,  0,320, 16}, //13 ロケット縦F
		
	};
	RECT rcRight[] = {
		{128, 16,144, 32},
		{144, 16,160, 32},// 1 瞬き
		{160, 16,176, 32},// 2 屈み
		{176, 16,192, 32},// 3 ジャンプ！
		{192, 16,208, 32},// 4 降下
		{208, 16,224, 32},// 5 構え
		{224, 16,240, 32},// 6 ロケット横
		{240, 16,256, 32},// 7 ロケット横F
		{256, 16,272, 32},// 8 ダメージ

		{272, 16,288, 32}, // 9 走る
		{288, 16,304, 32}, //10
		{128, 16,144, 32}, //11
		
		{176, 16,192, 32}, //12 ロケット縦
		{304, 16,320, 32}, //13 ロケット縦F
	};
//	SwordDamage1();
	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->y       += 4*VS;
	case 1://待機
		npc->ym += VS / 8;
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		//近づくとふり向く…。
		if( npc->x - PARTSSIZE*2*VS < gMC.x &&
			npc->x + PARTSSIZE*2*VS > gMC.x ){

			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;

	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;

	// 以下戦闘 ---------------------
	case 100:
		npc->act_no   = 110;
		npc->act_wait =   0;
		npc->count1   =   0;
		npc->bits |= BITS_BANISH_DAMAGE;
	case 110:
		npc->damage   =   1;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_no   =   0;
		npc->act_wait++;
		if( npc->act_wait >  4 ){
			npc->act_wait =   0;
			npc->act_no   = 120;

			npc->count2++;
			if( npc->count2 > 12 ){
				npc->count2 = 0;
				npc->act_no = 300;
			}
		}
		break;
	case 120:// 屈み
		npc->ani_no   =   2;
		npc->act_wait++;
		if( npc->act_wait >  4 ){
			npc->act_no   = 130;
			npc->ani_no   =   3;
			npc->xm       = Random( -VS, VS ) * 2;
			npc->ym       = -4 * VS;
			PlaySoundObject( WAVE_NPCJUMP1, 1 );

			npc->count1++;
		}
		break;

	case 130:// 空中
		npc->ym += VS/4;
		if( npc->y  > 8*VS*PARTSSIZE ) npc->bits &= ~BITS_THROW_BLOCK;
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ) npc->xm *= -1;
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ) npc->xm *= -1;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	
		if( npc->ym      < -1*VS ) npc->ani_no = 3;
		else if( npc->ym >  1*VS ) npc->ani_no = 4;
		else                       npc->ani_no = 0;

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 140;
			npc->act_wait =   0;
			npc->ani_no   =   2;
			npc->xm       =   0;
		}
		// to rocket
		if( npc->count1 > 4 && gMC.y < npc->y + 4*VS ){
			npc->act_no   = 200;
			npc->act_wait =   0;
			npc->xm       =   0;
			npc->ym       =   0;
		}
		
		break;
		
	case 140:// 着陸
		npc->ani_no   =   2;
		npc->act_wait++;
		if( npc->act_wait >  4 ){
			npc->act_no   = 110;
		}
		break;
		
	case 200:// 構え！ （ロケット横）-------------
		npc->ani_no   =   5;
		npc->act_wait++;
		if( npc->act_wait >  10 ){
			npc->act_no   = 210;
			npc->ani_no   =   6;
			if( npc->direct == DIR_LEFT ) npc->xm = -MAX_MOVE;
			else                          npc->xm =  MAX_MOVE;
			PlaySoundObject( WAVE_BUNRET, 1 );
			npc->bits &= ~BITS_BANISH_DAMAGE;
			npc->bits |=  BITS_BLOCK_BULLET;
			npc->damage   =   10;
		}
		break;
	case 210:// ロケット横
		if( ++npc->ani_no > 7 ) npc->ani_no = 6;
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ) npc->act_no = 220;
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ) npc->act_no = 220;
		break;
		
	case 220:// 衝突
		npc->act_no   = 221;
		npc->act_wait =   0;
		SetQuake( 16 );
		PlaySoundObject( WAVE_QUAKE, 1 );
		npc->damage   =   4;
	case 221:
		if( ++npc->ani_no > 7 ) npc->ani_no = 6;
		npc->act_wait++;

		if( !(npc->act_wait % 6) ){
			SetNpChar( 314, Random( 4, 16 ) *VS*PARTSSIZE,
						16*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		}

		if( npc->act_wait > 30 ){
			npc->count1 =   0;
			npc->act_no = 130;
			npc->bits  |=  BITS_BANISH_DAMAGE;
			npc->bits  &= ~BITS_BLOCK_BULLET;
			npc->damage = 3;
		}
		break;
	case 300:// 構え！（ロケット縦）-------------
		npc->act_no = 301;
		npc->ani_no =   9;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 301:// ダッシュ
		if( ++npc->ani_no > 11 ) npc->ani_no =  9;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS * 2;
		else                          npc->xm =  VS * 2;
		
		if( gMC.x > npc->x - 4*VS && gMC.x < npc->x + 4*VS ){
			npc->act_no   = 310;
			npc->act_wait =   0;
			npc->ani_no   =   2;
			npc->xm       =   0;
		}
		break;
	case 310:// 屈んで…
		npc->ani_no   =   2;
		npc->act_wait++;
		if( npc->act_wait > 4 ){
			npc->act_no = 320;
			npc->ani_no =  12;
			npc->ym     = -4 * VS;
			PlaySoundObject( WAVE_BUNRET, 1 );
			npc->bits  |= BITS_THROW_BLOCK;
			npc->bits &= ~BITS_BANISH_DAMAGE;
			npc->bits |=  BITS_BLOCK_BULLET;
			npc->damage   =   10;
		}
		break;
	case 320:// ロケット縦！
		if( ++npc->ani_no > 13 ) npc->ani_no = 12;
//		if( npc->flag & FLAG_HIT_TOP ) npc->act_no = 330;
		if( npc->y < 16*VS     ) npc->act_no = 330;
		break;
	case 330:// 衝突
		npc->ym       = 0;
		npc->act_no   = 331;
		npc->act_wait =   0;
		SetQuake( 16 );
		PlaySoundObject( WAVE_QUAKE, 1 );
	case 331:
		if( ++npc->ani_no > 13 ) npc->ani_no = 12;
		npc->act_wait++;
		if( !(npc->act_wait % 6) ){
			SetNpChar( 315, Random( 4, 16 ) *VS*PARTSSIZE,
						0*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		}
		if( npc->act_wait > 30 ){
			npc->count1 =   0;
			npc->act_no = 130;
			npc->bits  |=  BITS_BANISH_DAMAGE;
			npc->bits  &= ~BITS_BLOCK_BULLET;
			npc->damage = 3;
		}
		break;
		
	case 500:// やられ
		npc->bits  &=  ~BITS_BANISH_DAMAGE;

		npc->act_no   = 501;
		npc->act_wait =   0;
		npc->ani_no   =   8;
		npc->tgt_x    = npc->x;
		npc->damage   =   0;
		DeleteNpCharCode( 315, TRUE );
	case 501:
		npc->ym += VS/16;
		npc->act_wait++;
		if( npc->act_wait % 2 ) npc->x = npc->tgt_x;
		else                    npc->x = npc->tgt_x + VS;
		break;
	}
	
	// ミサイル/ブレード/Sミサイル防御
	if( npc->act_no > 100 && npc->act_no < 500 && 
		npc->act_no != 210 && npc->act_no != 320 )
	{
		
		if( IsActiveSomeBullet() ){
			npc->bits  &= ~BITS_BANISH_DAMAGE;
			npc->bits  |=  BITS_BLOCK_BULLET;
		
		}else{
			npc->bits  |=  BITS_BANISH_DAMAGE;
			npc->bits  &= ~BITS_BLOCK_BULLET;
		}
	}

	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no ];
	else                          npc->rect = rcRight[npc->ani_no ];
}

// Cave落石
void ActNpc314( NPCHAR *npc )
{
	RECT rc[] = {
		{ 64, 64, 80, 80},
		{ 80, 64, 96, 80},
		{ 96, 64,112, 80},
	};
	long i;

	switch( npc->act_no ){
	case 0:
		npc->count2 = 0;
		npc->act_no = 100;
		npc->bits  |= BITS_BLOCK_BULLET;
		npc->ani_no = Random( 0, 2 );
	case 100:// 落下
		npc->ym += VS/8;
		if( npc->ym > VS*3+VS/2 ) npc->ym = VS*3+VS/2;

		if( npc->y > VS*PARTSSIZE*8 ) npc->bits &= ~BITS_THROW_BLOCK;


		if( npc->flag & FLAG_HIT_BOTTOM ){

			npc->ym     = -VS;
			npc->act_no = 110;
			npc->bits |= BITS_THROW_BLOCK;
			PlaySoundObject( WAVE_BREAK1, 1 );
			SetQuake( 10 );
			for( i = 0; i < 2; i++ ){
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
	if( npc->ani_wait > 6 ){
		npc->ani_wait++;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )
		npc->ani_no = 0;

	if( gMC.y > npc->y ) npc->damage =  10;
	else                 npc->damage =   0;



	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];

}

// ダミマピニョン
void ActNpc315( NPCHAR *npc )
{
//	long i;
	RECT rcLeft[] = {
		{128,  0,144, 16},
		{160,  0,176, 16},// 1 屈み
		{176,  0,192, 16},// 2 ジャンプ！
		{192,  0,208, 16},// 3 降下
		
	};
	RECT rcRight[] = {
		{128, 16,144, 32},
		{160, 16,176, 32},// 1 屈み
		{176, 16,192, 32},// 2 ジャンプ！
		{192, 16,208, 32},// 3 降下
	};
	
	switch( npc->act_no ){
	case 0:
		npc->ani_no  = 3;
		npc->ym     += VS/4;
		if( npc->y > 8*VS*PARTSSIZE ){
			npc->act_no = 130;
			npc->bits &= ~BITS_THROW_BLOCK;
		}
		break;

	case 100:
		npc->act_no   = 110;
		npc->act_wait =   0;
		npc->count1   =   0;
		npc->bits |= BITS_BANISH_DAMAGE;
	case 110:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_no   =   0;
		npc->act_wait++;
		if( npc->act_wait >  4 ){
			npc->act_wait =   0;
			npc->act_no   = 120;
		}
		break;
	case 120:// 屈み
		npc->ani_no   =   1;
		npc->act_wait++;
		if( npc->act_wait >  4 ){
			npc->act_no   = 130;
			npc->ani_no   =   3;
			npc->xm       = Random( -VS, VS ) * 2;
			npc->ym       = -4 * VS;
			PlaySoundObject( WAVE_NPCJUMP1, 1 );

		}
		break;
	case 130:// 空中
		npc->ym += VS/4;
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ) npc->xm *= -1;
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ) npc->xm *= -1;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		
		if( npc->ym      < -1*VS ) npc->ani_no = 2;
		else if( npc->ym >  1*VS ) npc->ani_no = 0;
		else                       npc->ani_no = 3;

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 140;
			npc->act_wait =   0;
			npc->ani_no   =   1;
			npc->xm       =   0;
		}
		break;

	case 140:// 着陸
		npc->ani_no = 1;
		npc->act_wait++;
		if( npc->act_wait >  4 ){
			npc->act_no   = 110;
			npc->bits |= BITS_BANISH_DAMAGE;
		}
		break;
		

	}

	// ミサイル/ブレード/Sミサイル防御
	if( npc->act_no > 100 ){
		if( IsActiveSomeBullet() ){
			npc->bits  &= ~BITS_BANISH_DAMAGE;
			npc->bits  |=  BITS_BLOCK_BULLET;
		}else{
			npc->bits  |=  BITS_BANISH_DAMAGE;
			npc->bits  &= ~BITS_BLOCK_BULLET;
		}
	}
	npc->count2++;
	if( npc->count2 > 300 ){
		VanishNpChar( npc );
/*		for( i = 0; i < 2; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + 16*VS,
				Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
*/		return;
	}

	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no ];
	else                          npc->rect = rcRight[npc->ani_no ];

	
}


// エンジェル やられ
void ActNpc316( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{248, 32,272, 56},// ふっとび
		{272, 32,296, 56},
		{296, 32,320, 56},
	};
	RECT rcRight[] = {
		{248, 56,272, 80},// ふっとび
		{272, 56,296, 80},
		{296, 56,320, 80},
	};
	switch( npc->act_no ){
	case 0:
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits &= ~BITS_THROW_BLOCK;
		npc->damage = 0;
		npc->act_no = 1;
		npc->ani_no = 0;
		npc->view.front = 12*VS;
		npc->view.back  = 12*VS;
		npc->view.top   = 12*VS;
		npc->ym     = -VS;
		if( npc->direct == DIR_LEFT ) npc->xm = VS/2;
		else                          npc->xm = -VS/2;
		PlaySoundObject( WAVE_NPC_SMALL, 1 );
		break;
	case 1://ふきとび
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_no   = 1;
			npc->ani_wait = 0;
			npc->act_no   = 2;
			npc->act_wait = 0;
		}
		break;
	case 2:// じたばた
		npc->xm = npc->xm *8/9;
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 1;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->cond |= COND_LOSE;
		}
		break;
	}

	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;


	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// エンジェル(大男)
void ActNpc317( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 80, 32,120},
		{ 32, 80, 64,120},
		{ 64, 80, 96,120},
		{ 96, 80,128,120},
	};
	RECT rcRight[] = {
		{  0,120, 32,160},
		{ 32,120, 64,160},
		{ 64,120, 96,160},
		{ 96,120,128,160},
	};

	switch( npc->act_no ){
	case 0:// 待機
		npc->act_no = 1;
		npc->y     -= 8*VS;
		npc->tgt_x  = npc->x;
	case 1:
		npc->xm     = 0;
		npc->act_no = 2;
		npc->ani_no = 0;
		npc->count1 = 0;
	case 2:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_wait++;
		if( npc->ani_wait > 40 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ) npc->ani_no   =  0;

		if( gMC.x > npc->x - 20*VS*PARTSSIZE &&
			gMC.x < npc->x + 20*VS*PARTSSIZE &&
			gMC.y > npc->y - 10*VS*PARTSSIZE &&
			gMC.y < npc->y + 10*VS*PARTSSIZE ){
			npc->count1++;
			if( npc->count1 > 50 )npc->act_no = 10;
		}
		break;

	case 10: //構え
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->ani_no   =  2;
		SetNpChar( 319, npc->x, npc->y, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_wait =  0;
			npc->act_no   = 12;
			npc->ani_no   =  3;
			PlaySoundObject( WAVE_POP, 1 );
		}
		break;
	case 12:// 投げ
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no   = 1;
		}
		break;
	}

	npc->ym += VS/6;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	// →やられ
	if( npc->life <= 1000 - 64 ){
		npc->code_char = 318;
		npc->act_no    =   0;
	}

}

// 大男 やられ
void ActNpc318( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{224, 80,256,120},// ふっとび
		{256, 80,288,120},
		{288, 80,320,120},
	};
	RECT rcRight[] = {
		{224,120,256,160},// ふっとび
		{256,120,288,160},
		{288,120,320,160},
	};
	switch( npc->act_no ){
	case 0:
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits &= ~BITS_THROW_BLOCK;
		npc->bits &= ~BITS_BLOCK_MYCHAR;
		npc->damage = 0;
		npc->act_no = 1;
		npc->ani_no = 0;
//		npc->view.front = 12*VS;
//		npc->view.back  = 12*VS;
//		npc->view.top   = 12*VS;
		npc->ym     = -VS;
		if( npc->direct == DIR_LEFT ) npc->xm =  VS/8;
		else                          npc->xm = -VS/8;
		PlaySoundObject( WAVE_NPC_GOHST, 1 );
		break;
	case 1://ふきとび
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_no   = 1;
			npc->ani_wait = 0;
			npc->act_no   = 2;
			npc->act_wait = 0;
		}
		break;
	case 2:// じたばた
		npc->xm = npc->xm *8/9;
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 1;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->cond |= COND_LOSE;
		}
		break;
	}

	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;


	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// エンジェルブロック
void ActNpc319( NPCHAR *npc )
{
	RECT rc[] = {
		{16, 0, 32, 16},
		{16, 0, 32, 16},
		{96, 80,112,96},
	};

	switch( npc->act_no ){
	case 0:// 待機
		npc->y = npc->pNpc->y + 10*VS;
		if( npc->pNpc->direct == DIR_LEFT ) npc->x = npc->pNpc->x + 7*VS;
		else                                npc->x = npc->pNpc->x - 7*VS;
		// 消滅
		if( npc->pNpc->code_char == 318 ){
			SetDestroyNpChar( npc->x, npc->y, 0, 3 );
			npc->cond = 0;
			return;
		}
		// なげ
		if( npc->pNpc->ani_no    !=  2  ){
			npc->act_no = 2;
			npc->act_wait = 0;
			npc->ym = -VS*2;
			npc->y = npc->pNpc->y-4*VS;
			if( npc->pNpc->direct == DIR_LEFT ) npc->xm = -VS*2;
			else                                npc->xm =  VS*2;

		}
		break;
	case 1:

		break;
	case 2:// 飛びます
		npc->act_wait++;
		if( npc->act_wait == 4 ) npc->bits &= ~BITS_THROW_BLOCK;
		npc->ym += VS/12;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		npc->x += npc->xm;
		npc->y += npc->ym;

		if( npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_BREAK1, 1 );
			SetDestroyNpChar( npc->x, npc->y, 0, 3 );
			npc->cond = 0;
		}
		break;
	}
	npc->ani_no++;
	if( npc->ani_no > 2 )
		npc->ani_no = 0;
	npc->rect = rc[npc->ani_no];
}
