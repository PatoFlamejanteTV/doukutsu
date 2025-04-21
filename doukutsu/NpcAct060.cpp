#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"

// トロ子
void ActNpc060( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{  0, 64, 16, 80},//停止
		{ 16, 64, 32, 80},

		{ 32, 64, 48, 80},
		{ 16, 64, 32, 80},
		{ 48, 64, 64, 80},
		{ 16, 64, 32, 80},


		{112, 64,128, 80},//6ふっとび
		{128, 64,144, 80},//7つぶれ

	};

	RECT rcRight[] = {
		{  0, 80, 16, 96},//停止
		{ 16, 80, 32, 96},

		{ 32, 80, 48, 96},
		{ 16, 80, 32, 96},
		{ 48, 80, 64, 96},
		{ 16, 80, 32, 96},


		{112, 80,128, 96},//6ふっとび
		{128, 80,144, 96},//7つぶれ

	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0;
	case 1://待機 =============
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		//近づくとふり向く…。
		if( npc->x - PARTSSIZE*VS < gMC.x &&
			npc->x + PARTSSIZE*VS > gMC.x &&
			npc->y - PARTSSIZE*VS < gMC.y &&
			npc->y + PARTSSIZE*VS > gMC.y ){

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

	case 3: //走る =============
		npc->act_no   = 4;
		npc->ani_no   = 1;
		npc->ani_wait = 0;
	case 4:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )
			npc->ani_no = 1;
		if( npc->flag & FLAG_HIT_LEFT  ){
			npc->direct = DIR_RIGHT;
			npc->xm     = VS;
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->direct = DIR_LEFT;
			npc->xm     = -VS;
		}
		if( npc->direct == DIR_LEFT ) npc->xm = -2*VS;
		else                          npc->xm =  2*VS;

		break;

	case 6: //逃げ =============
		npc->act_no   = 7;
		npc->act_wait = 0;
		npc->ani_no   = 1;
		npc->ani_wait = 0;
		npc->ym = -2*VS;
	case 7:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )
			npc->ani_no = 1;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;

		// 走る
		if( npc->act_wait++ &&
			npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 3;
		}

		break;
	case 8: //びっくり =============
		npc->ani_no   = 1;
		npc->act_wait = 0;
		npc->act_no   = 9;
		npc->ym       = -1*VS;
	case 9:

		if( npc->act_wait++ &&
			npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 0;
		}

		break;
	case 10:
		npc->act_no = 11;
		npc->ani_no = 6;
		npc->ym     = -2*VS;
		PlaySoundObject( WAVE_NPC_SMALL, 1 );
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;

		break;
	case 11:// こける

		// ばったり
		if( npc->act_wait++ &&
			npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 12;
			npc->ani_no =  7;
			npc->bits |= BITS_EVENT_CHECK;
		}

		break;
	case 12:
		npc->xm     = 0;
		break;

	}

	npc->ym += VS/8;

	if( npc->xm >  2*VS )npc->xm =  2*VS;
	if( npc->xm < -2*VS )npc->xm = -2*VS;
	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// キング
void ActNpc061( NPCHAR *npc )
{
	long i;

	RECT rcLeft[] = {

		{224, 32,240, 48},// 停止
		{240, 32,256, 48},// 
		{256, 32,272, 48},// ふっとび
		{272, 32,288, 48},// 寝込み

		{288, 32,304, 48},//4 歩き
		{224, 32,240, 48},// 
		{304, 32,320, 48},// 
		{224, 32,240, 48},// 

		{272, 32,288, 48},//8 寝込み
		{  0,  0,  0,  0},// 寝込み
		{112, 32,128, 48},// ブレード
	};

	RECT rcRight[] = {

		{224, 48,240, 64},// 停止
		{240, 48,256, 64},// 
		{256, 48,272, 64},// ふっとび
		{272, 48,288, 64},// 寝込み

		{288, 48,304, 64},//4 歩き
		{224, 48,240, 64},// 
		{304, 48,320, 64},// 
		{224, 48,240, 64},// 

		{272, 48,288, 64},//8 寝込み
		{  0,  0,  0,  0},// 寝込み
		{112, 32,128, 48},// ブレード
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0;
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
	case 5:// 死んだふり
		npc->ani_no = 3;
		npc->xm     = 0;
		break;
	case 6: //ふっとび =============
		npc->act_no   = 7;
		npc->act_wait = 0;
		npc->ani_wait = 0;
		npc->ym = -2*VS;
	case 7:
		npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;

		// 死んだふり
		if( npc->act_wait++ &&
			npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 5;
		}

		break;

	case 8: //歩行
		npc->act_no   = 9;
		npc->ani_no   = 4;
		npc->ani_wait = 0;
	case 9:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 7 )
			npc->ani_no = 4;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;

		break;

	case 10: //走る
		npc->act_no   = 11;
		npc->ani_no   = 4;
		npc->ani_wait = 0;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 7 )
			npc->ani_no = 4;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
		else                          npc->xm =  VS*2;

		break;

	case 20://剣装備
		SetNpChar( 145, 0, 0, 0, 0, DIR_RIGHT, npc, MAX_NPC/2 );
		npc->ani_no = 0;
		npc->act_no = 0;
		break;
	case 30://吹っ飛び２
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->ani_wait =  0;
		npc->ym       =  0;
	case 31:
		npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->xm = -3*VS;
		else                          npc->xm =  3*VS;

		// 死んだふり
		if(	npc->flag & FLAG_HIT_LEFT ){
			npc->direct = DIR_RIGHT;
			npc->act_no   = 7;
			npc->act_wait = 0;
			npc->ani_wait = 0;
			npc->ym       = -2*VS;
			npc->xm       =  VS;
			PlaySoundObject( WAVE_DESTROY_MIDDLE, 1 );
			SetDestroyNpChar( npc->x, npc->y, 4*VS,  4 );
		}
		break;

	case 40:// 消滅 ====
		npc->act_no   = 42;
		npc->act_wait =  0;
		npc->ani_no   =  8;
		PlaySoundObject( WAVE_WARP, 1 );
	case 42:
		npc->ani_no++;
		if( npc->ani_no > 9 )
			npc->ani_no = 8;
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
							Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			npc->act_no = 50;
			npc->surf   = SURF_NPC_SYMBOL;
			npc->ani_no = 10;
		}
		break;
	case 50://つるぎの姿 ====
		break;

	case 60://飛び込み ====
		npc->ani_no = 6;
		npc->act_no = 61;
		npc->ym = -MAX_MOVE;
		npc->xm = VS*2;
		npc->count2 = 1; //ふりあげ
		break;
	case 61:
		npc->ym += VS/8;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 0;
			npc->count2 = 0;
			npc->xm     = 0;
		}
		break;
	}

	if( npc->act_no < 30 ||
		npc->act_no >= 40 ){
		npc->ym += VS/8;

		if( npc->xm >  2*VS )npc->xm =  2*VS;
		if( npc->xm < -2*VS )npc->xm = -2*VS;
		if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// PCカズマ
void ActNpc062( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{272,192,288,216},
		{288,192,304,216},
		{304,192,320,216},
	};


	switch( npc->act_no ){
	case 0:
		npc->x +=  -4*VS;//-= VS*12;
		npc->y += 16*VS;
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://キーボードカタカタ

		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )npc->ani_no = 0;


		//停止へ
		if( Random( 0, 80 ) == 1 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		//前かがみへ
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 3;
			npc->act_wait = 0;
			npc->ani_no   = 2;
		}
		break;

	case 2://停止
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_no = 3;
			npc->act_wait = 0;
			npc->ani_no = 2;
		}

		break;

	case 3://前かがみ
		npc->act_wait++;
		if( npc->act_wait > 80 ){
			npc->act_no   = 1;
			npc->ani_no   = 0;
		}

		break;

	}

	npc->rect = rcLeft[npc->ani_no];
}


// トロ子戦闘
void ActNpc063( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{ 64, 64, 80, 80},
		{ 80, 64, 96, 80},
		{ 64, 64, 80, 80},
		{ 96, 64,112, 80},

		{112, 64,128, 80},//4ふっとび
		{128, 64,144, 80},//5つぶれ
	};

	RECT rcRight[] = {

		{ 64, 80, 80, 96},
		{ 80, 80, 96, 96},
		{ 64, 80, 80, 96},
		{ 96, 80,112, 96},

		{112, 80,128, 96},//4ふっとび
		{128, 80,144, 96},//5つぶれ
	};

	switch( npc->act_no ){
	case 0:// 飛びこみ =============
		npc->act_no   = 1;
		npc->act_wait = 0;
		npc->ani_wait = 0;
		npc->ym = -2*VS;
	case 1:

		//ブロックとの当たり判定を発生
		if( npc->ym > 0 )npc->bits &= ~BITS_THROW_BLOCK;

		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 0;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;

		// 走る
		if( npc->act_wait++ &&
			npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 2;
		}

		break;
	case 2:// 走る =============
		npc->act_no   = 3;
		npc->act_wait = 0;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 3:
		// アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 0;

		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_wait = 40;
			npc->xm *= -1;
			if( npc->direct == DIR_LEFT ) npc->direct = DIR_RIGHT;
			else                          npc->direct = DIR_LEFT;
		}

		if( npc->act_wait > 35 )npc->bits |= BITS_BANISH_DAMAGE;

		if( npc->direct == DIR_LEFT ) npc->xm -= VS/8;
		else                          npc->xm += VS/8;

		//こけ！
		if( npc->shock ){
			npc->act_no = 4;
			npc->ani_no = 4;
			npc->ym     = -2*VS;
			npc->bits &= ~BITS_BANISH_DAMAGE;
			npc->damage = 0;
		}

		break;
	case 4:// こける
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;

		// ばったり
		if( npc->act_wait++ &&
			npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 5;
			npc->bits |= BITS_EVENT_CHECK;
		}

		break;
	case 5:
		npc->xm     = 0;
		npc->ani_no = 5;
		break;
	}

	npc->ym += VS/8;

	if( npc->xm >  2*VS )npc->xm =  2*VS;
	if( npc->xm < -2*VS )npc->xm = -2*VS;
	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

//	if( npc->shock ) npc->x += (npc->xm)/4;
//	else

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}





//クリッター LV0
void ActNpc064( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 0, 16, 16},
		{ 16, 0, 32, 16},
		{ 32, 0, 48, 16},//ジャンプ
	};
	RECT rcRight[] = {
		{  0, 16, 16, 32},
		{ 16, 16, 32, 32},
		{ 32, 16, 48, 32},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->y += 3*VS;
		npc->act_no = 1;
	case 1://待機
		if( npc->x > gMC.x )npc->direct = DIR_LEFT;
		else                npc->direct = DIR_RIGHT;
		if( npc->tgt_x < 100 ){
			npc->tgt_x++;
		}
		if( npc->act_wait >= 8 &&
			npc->x - PARTSSIZE*7*VS < gMC.x &&
			npc->x + PARTSSIZE*7*VS > gMC.x &&
			npc->y - PARTSSIZE*5*VS < gMC.y &&
			npc->y + PARTSSIZE*5*VS > gMC.y ){

			npc->ani_no = 1;
		}else{
			if( npc->act_wait < 8 )
				npc->act_wait++;
			npc->ani_no = 0;
		}

		if( npc->shock ){//ダメージかがむ
			npc->act_no   = 2;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}
		//近づきかがむ
		if( npc->act_wait >= 8 &&
			npc->tgt_x >= 100  &&
			npc->x - PARTSSIZE*4*VS < gMC.x &&
			npc->x + PARTSSIZE*4*VS > gMC.x &&
			npc->y - PARTSSIZE*5*VS < gMC.y &&
			npc->y + PARTSSIZE*3*VS > gMC.y ){

			npc->act_no   = 2;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}
		break;

	case 2://かがむ
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 3;
			npc->ani_no = 2;
			npc->ym     = -MAX_MOVE;
			PlaySoundObject( WAVE_NPCJUMP1, 1 );
			if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
			else                          npc->xm =  VS/2;
		}
		break;

	case 3://滞空
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->xm       = 0;
			npc->act_wait = 0;
			npc->ani_no   = 0;
			npc->act_no   = 1;
			PlaySoundObject( WAVE_STEP, 1 );
		}
		break;
	}

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];

}

//バット滞空型 LV0
void ActNpc065( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0://初期化
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
		npc->count1 = 120;
		npc->act_no = 1;
		npc->act_wait = Random( 0, 50 );
	case 1://ランダム待機
		npc->act_wait++;
		if( npc->act_wait < 50 )break;
		npc->act_wait = 0;
		npc->act_no   = 2;
		npc->ym       = VS*3/2;
		break;
	case 2://滞空
		//振り向き
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->tgt_y < npc->y ) npc->ym -= VS/32;
		if( npc->tgt_y > npc->y ) npc->ym += VS/32;

		if( npc->ym >  VS*3/2 )npc->ym =  VS*3/2;
		if( npc->ym < -VS*3/2 )npc->ym = -VS*3/2;
		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{ 32, 32, 48, 48},
		{ 48, 32, 64, 48},
		{ 64, 32, 80, 48},
		{ 80, 32, 96, 48},
	};
	RECT rect_right[] = {
		{ 32, 48, 48, 64},
		{ 48, 48, 64, 64},
		{ 64, 48, 80, 64},
		{ 80, 48, 96, 64},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )npc->ani_no = 0;
	if( npc->direct == DIR_LEFT )
		npc->rect = rect_left[npc->ani_no];
	else 
		npc->rect = rect_right[npc->ani_no];
}

extern NPCHAR gNPC[];
// トロ子カプセル
void ActNpc066( NPCHAR *npc )
{
	RECT rect[] = {
		{ 32,192,  56,216},
		{ 56,192,  80,216},

		{ 32,216,  56,240},
		{ 56,216,  80,240},
	};

	switch( npc->act_no )
	{
	case 0:
		{
			// イベントNO 1000を選択
			long a;
			for( a = 0; a < MAX_NPC; a++ ){
				if( gNPC[a].code_event == 1000 )break;
			}
			if( a == MAX_NPC )break;

			npc->tgt_x = gNPC[a].x;
			npc->tgt_y = gNPC[a].y;
			npc->count1 = a;

			// 移動量を計算
			unsigned char deg;
			deg         = GetArktan( npc->x - npc->tgt_x, npc->y - npc->tgt_y );
			npc->xm     = GetCos( deg ) * 2;
			npc->ym     = GetSin( deg ) * 2;
			npc->act_no = 1;
		}

	case 1:
		//アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )npc->ani_no = 0;

		//接触
		if( 
			npc->x - 3*VS < npc->tgt_x &&
			npc->x + 3*VS > npc->tgt_x &&
			npc->y - 3*VS < npc->tgt_y &&
			npc->y + 3*VS > npc->tgt_y ){

			npc->act_no = 2;
			npc->ani_no = 2;
//			npc->xm = 0;
//			npc->ym = 0;
			gNPC[npc->count1].cond = 0;
			PlaySoundObject( WAVE_SPLASH, 1 );
		}
		break;
	case 2: // 撤退
		npc->xm -= VS/16;
		npc->ym -= VS/16;
		if( npc->xm < -MAX_MOVE ) npc->xm = -MAX_MOVE;
		if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;
		if( npc->y < -8*VS )npc->cond = 0;

		//アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )npc->ani_no = 2;
		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];

}


// ミザリー（出現）
void ActNpc067( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
		npc->ani_no = 0;
		PlaySoundObject( WAVE_WARP, 1 );
	case 1:
		npc->x = npc->tgt_x + Random( -1, 1 )*VS;
		npc->act_wait++;
		if( npc->act_wait == 32 ){
			npc->act_no = 10;
		}
		break;
	
	case 10://初期化
		npc->act_no   = 11;
		npc->act_wait = 0;
		npc->ani_no   = 0;
		npc->ym = VS;
	case 11://滞空
		if( npc->tgt_y < npc->y ) npc->ym -= VS/32;
		if( npc->tgt_y > npc->y ) npc->ym += VS/32;

		if( npc->ym >  VS/2 )npc->ym =  VS/2;
		if( npc->ym < -VS/2 )npc->ym = -VS/2;
		break;

	case 13://落下
		npc->ani_no = 1;
		npc->ym += VS/8;
		if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_STEP, 1 );
			npc->ym     = 0;
			npc->act_no = 14;
			npc->bits  |= BITS_THROW_BLOCK;
			npc->ani_no = 2;
		}
		break;
	case 14://地面
		break;

	case 15://トロ子バブル発射
		npc->act_no   = 16;
		npc->act_wait = 0;
		npc->ani_no   = 4;
	case 16:
		npc->act_wait++;
		if( npc->act_wait == 30 ){
			PlaySoundObject( WAVE_SPLASH, 1 );
			SetNpChar( 66, npc->x, npc->y - 16*VS, 0, 0, DIR_LEFT, npc, 0 ); 
		}
		if( npc->act_wait == 50 )npc->act_no = 14;
		break;

	case 20://退却
		npc->act_no = 21;
		npc->ani_no = 0;
		npc->ym     = 0;
		npc->bits   |= BITS_THROW_BLOCK;
	case  21:
		npc->ym -= VS/16;
		if( npc->y < -8*VS )npc->cond = 0;

		break;

	case 25://フラッシュ ------
		npc->act_no   = 26;
		npc->act_wait = 0;
		npc->ani_no   = 5;
		npc->ani_wait = 0;
	case 26:
		npc->ani_no++;
		if( npc->ani_no > 7 )
			npc->ani_no = 5;

		npc->act_wait++;
		if( npc->act_wait == 30 ){
			PlaySoundObject( WAVE_THUNDER, 1 );
			SetFlash( 0, 0, FLASHMODE_FLASH );
			npc->act_no = 27;
			npc->ani_no = 7;
		}
		break;
	case 27:
		npc->act_wait++;
		if( npc->act_wait == 50 )
			npc->act_no = 14;
		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rcLeft[] = {
		{ 80,  0, 96, 16},
		{ 96,  0,112, 16},
		{112,  0,128, 16},
		{128,  0,144, 16},
		{144,  0,160, 16},

		{160,  0,176, 16},// 5Flash
		{176,  0,192, 16},
		{144,  0,160, 16},
	};
	RECT rcRight[] = {
		{ 80, 16, 96, 32},
		{ 96, 16,112, 32},
		{112, 16,128, 32},
		{128, 16,144, 32},
		{144, 16,160, 32},

		{160, 16,176, 32},// 5Flash
		{176, 16,192, 32},
		{144, 16,160, 32},
	};

	// 滞空
	if( npc->act_no == 11 ){
		if( npc->ani_wait ){
			npc->ani_wait--;
			npc->ani_no = 1;
		}else{
			if( Random( 0, 100 ) == 1 )npc->ani_wait = 30;
			npc->ani_no = 0;
		}
	}
	// 地面
	if( npc->act_no == 14 ){
		if( npc->ani_wait ){
			npc->ani_wait--;
			npc->ani_no = 3;
		}else{
			if( Random( 0, 100 ) == 1 )npc->ani_wait = 30;
			npc->ani_no = 2;
		}
	}
	
	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];

	if( npc->act_no == 1 ){
		if( npc->ani_wait < 16*2 ){
			npc->ani_wait++;
			npc->rect.bottom += (-16) + npc->ani_wait/2;
		}
	}
}






// バルログ LV0 つかみ投げ
void ActNpc068( NPCHAR *npc )
{

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->act_wait = 30;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	//見つめる
	case 1:
		npc->act_wait--;
		if( npc->act_wait )break;

		npc->act_no   = 2;
		npc->count1++;
		break;
	//走る ===================
	case 2:
		npc->act_no   = 3;
		npc->act_wait = 0;
		npc->ani_no   = 1;
		npc->ani_wait = 0;
	case 3:
		//アニメ
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no == 2 || npc->ani_no == 4 ){
				PlaySoundObject( WAVE_STEP, 1 );
			}
		}
		if( npc->ani_no > 4 ) npc->ani_no = 1;

		if( npc->direct == DIR_LEFT ) npc->xm -= VS/32;
		else                          npc->xm += VS/32;


		if( npc->act_wait >= 8 &&
			npc->x - 12*VS < gMC.x &&
			npc->x + 12*VS > gMC.x &&
			npc->y - 12*VS < gMC.y &&
			npc->y +  8*VS > gMC.y ){

			npc->act_no = 10;
			npc->ani_no = 5;
			gMC.cond   |= COND_HIDE;//hide!!
			DamageMyChar( 2 );
			break;
		}

		// タイムアウト
		npc->act_wait++;
		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ||
			npc->act_wait > 25*3 ){
			npc->act_no = 9;
			npc->ani_no = 0;
			break;
		}

		// 時にはジャンプ
		if( !(npc->count1%3) && npc->act_wait > 25*1 ){
			npc->act_no = 4;
			npc->ani_no = 7;
			npc->ym     = -VS*2;
			break;
		}

		break;

	case 4:// ジャンプ
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 9;
			npc->ani_no = 8;
			SetQuake( 30 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		if( npc->act_wait >= 8 &&
			npc->x - 12*VS < gMC.x &&
			npc->x + 12*VS > gMC.x &&
			npc->y - 12*VS < gMC.y &&
			npc->y +  8*VS > gMC.y ){

			npc->act_no = 10;
			npc->ani_no = 5;
			gMC.cond   |= COND_HIDE;//hide!!
			DamageMyChar( 2 );
		}

		break;
		
	case 9://摩擦停止
		npc->xm = npc->xm*4/5;
		if( npc->xm ) break;

		npc->act_no   = 0;

		break;
	//フリフリ ===================
	case 10:
		gMC.x = npc->x;
		gMC.y = npc->y;

		npc->xm = npc->xm*4/5;
		if( npc->xm ) break;

		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->ani_no   =  5;
		npc->ani_wait =  0;

			
		break;
	case 11:

		gMC.x = npc->x;
		gMC.y = npc->y;

		//アニメ
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 ) npc->ani_no = 5;


		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_no = 20;
		}
		break;
	//投げ… ===================
	case 20:
		PlaySoundObject( WAVE_BUNRET, 1 );
		gMC.cond   &= ~COND_HIDE;
		if( npc->direct == DIR_LEFT ){
			gMC.x       += 4*VS;
			gMC.y       -= 8*VS;
			gMC.xm       = MAX_MOVE;
			gMC.ym       = -VS;
			gMC.direct   = DIR_RIGHT;
			npc->direct  = DIR_RIGHT;
		}else{
			gMC.x       -= 4*VS;
			gMC.y       -= 8*VS;
			gMC.xm       = -MAX_MOVE;
			gMC.ym       = -VS;
			gMC.direct   = DIR_LEFT;
			npc->direct  = DIR_LEFT;
		}
		npc->act_no   = 21;
		npc->act_wait = 0;
		npc->ani_no   = 7;
	case 21:
		npc->act_wait++;
		if( npc->act_wait < 50 )break;

		npc->act_no = 0;
		break;
	}
	
	

	//Gravity
	npc->ym += VS/16;

	if( npc->xm < -VS*2    )npc->xm = -VS*2;
	if( npc->xm >  VS*2    )npc->xm =  VS*2;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{  0,  0, 40, 24},//ノーマル

		{  0, 48, 40, 72},//走る
		{  0,  0, 40, 24},
		{ 40, 48, 80, 72},
		{  0,  0, 40, 24},

		{ 80, 48,120, 72},//フリフリ
		{120, 48,160, 72},

		{120,  0,160, 24},//投げ…
		{ 80,  0,120, 24},//着陸
	};
	RECT rect_right[] = {
		{  0, 24, 40, 48},//ノーマル

		{  0, 72, 40, 96},//走る
		{  0, 24, 40, 48},
		{ 40, 72, 80, 96},
		{  0, 24, 40, 48},

		{ 80, 72,120, 96},//フリフリ
		{120, 72,160, 96},

		{120, 24,160, 48},//投げ…
		{ 80, 24,120, 48},//着陸
	};



	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
}

// ピニョン
void ActNpc069( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{ 48,  0, 64, 16},//停止
		{ 64,  0, 80, 16},

		{ 80,  0, 96, 16},//歩く
		{ 96,  0,112, 16},
		{ 48,  0, 64, 16},

		{112,  0,128, 16},// shock
	};

	RECT rcRight[] = {

		{ 48, 16, 64, 32},//停止
		{ 64, 16, 80, 32},
				 
		{ 80, 16, 96, 32},//歩く
		{ 96, 16,112, 32},
		{ 48, 16, 64, 32},
				 
		{112, 16,128, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0; 
	case 1://待機 =============
		if( Random( 0, 100 ) == 1 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
			break;
		}
		if( Random( 0, 150 ) == 1 ){
			if( npc->direct == DIR_LEFT ) npc->direct = DIR_RIGHT;
			else                          npc->direct = DIR_LEFT;
		}

		if( Random( 0, 150 ) == 1 ){
			npc->act_no   = 3;
			npc->act_wait = 50;
			npc->ani_no   = 0;
			break;
		}
		break;

	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;

	case 3: //歩く =============
		npc->act_no   = 4;
		npc->ani_no   = 2;
		npc->ani_wait = 0;
	case 4:
		npc->act_wait--;
		if( !npc->act_wait )npc->act_no = 0;
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )
			npc->ani_no = 2;
		if( npc->flag & FLAG_HIT_LEFT  ){
			npc->direct = DIR_RIGHT;
			npc->xm     = VS;
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->direct = DIR_LEFT;
			npc->xm     = -VS;
		}
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;

		break;

	case 5: // shock
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 0;
		}
		break;
	}


	// ショックでジャンプ
	switch( npc->act_no ){
	case 1:
	case 2:
	case 4:
		if( npc->shock ){
			npc->ym     = -VS;
			npc->ani_no = 5;
			npc->act_no = 5;
		}
		break;
	}

	npc->ym += VS/8;

//	if( npc->xm >  2*VS )npc->xm =  2*VS;
//	if( npc->xm < -2*VS )npc->xm = -2*VS;
	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[  npc->ani_no ];
	else                          npc->rect = rcRight[ npc->ani_no ];
}

// ブリンク
void ActNpc070( NPCHAR *npc )
{
	RECT rect[] = {

		{ 96, 48,112, 64},
		{112, 48,128, 64},
		{128, 48,144, 64},
		{144, 48,160, 64},

	};

	npc->ani_wait++;
	if( npc->ani_wait > 3 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3 ) npc->ani_no = 0;
	
	//アニメーション------------------
	npc->rect = rect[npc->ani_no];
}



// チン魚
void ActNpc071( NPCHAR *npc )
{
//	unsigned char deg;

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;

		npc->ym = VS/4;

	case 1://滞空

		if( npc->tgt_y < npc->y ) npc->ym -= VS/64;
		if( npc->tgt_y > npc->y ) npc->ym += VS/64;

		if( npc->ym >  VS/2 ) npc->ym =  VS/2;
		if( npc->ym < -VS/2 ) npc->ym = -VS/2;
		break;

		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rcLeft[] = {
		{ 64, 32, 80, 48},
		{ 80, 32, 96, 48},

		{ 96, 32,112, 48},
	};
	RECT rcRight[] = {
		{ 64, 48, 80, 64},
		{ 80, 48, 96, 64},

		{ 96, 48,112, 64},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 4 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 ) npc->ani_no = 0;
	if( npc->shock      ) npc->ani_no = 2;


	
	if( npc->direct == DIR_LEFT )npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];


}

//水撒き機
void ActNpc072( NPCHAR *npc )
{
	if( npc->direct == DIR_LEFT ){
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if ( npc->ani_no > 1 ){
			npc->ani_no = 0;
			return;
		}
		if( gMC.x < npc->x +20*VS*PARTSSIZE &&
			gMC.x > npc->x -20*VS*PARTSSIZE &&
			gMC.y < npc->y +15*VS*PARTSSIZE &&
			gMC.y > npc->y -15*VS*PARTSSIZE ){

			npc->act_no++;
			if( npc->act_no%2 )
				SetNpChar( 73, npc->x, npc->y, Random( -VS, VS )*2, Random(-VS, VS/4)*3, DIR_LEFT, NULL, MAX_NPC/2 );
				SetNpChar( 73, npc->x, npc->y, Random( -VS, VS )*2, Random(-VS, VS/4)*3, DIR_LEFT, NULL, MAX_NPC/2 );
		}


	}

	RECT rect[] = {
		{224, 48,240, 64},
		{240, 48,256, 64},
	};
	npc->rect = rect[npc->ani_no];
}

// 水飛沫
void ActNpc073( NPCHAR *npc )
{
	RECT rect[] = {
		{ 72, 16, 74, 18},
		{ 74, 16, 76, 18},
		{ 76, 16, 78, 18},
		{ 78, 16, 80, 18},
		{ 80, 16, 82, 18},
	};

	// 重力
	npc->ym += VS/16;

	npc->ani_no = Random( 0, 4 );


	
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
	if( npc->direct == DIR_RIGHT ){
		npc->rect.top    += 2;
		npc->rect.bottom += 2;
	}
	
	npc->act_wait++;
	if( npc->act_wait > 10 ){

		if( npc->flag & FLAG_HIT_LEFT  )		npc->cond = 0;
		if( npc->flag & FLAG_HIT_RIGHT )		npc->cond = 0;
		//地面消滅
		if( npc->flag & FLAG_HIT_BOTTOM )		npc->cond = 0;
		if( npc->flag & FLAG_HIT_WATER  )		npc->cond = 0;
	}
	if( npc->y > gMap.length*VS*PARTSSIZE )		npc->cond = 0;
}


// ジャック
void ActNpc074( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{ 64,  0, 80, 16},// 停止
		{ 80,  0, 96, 16},

		{ 96,  0,112, 16},// 歩き
		{ 64,  0, 80, 16},// 
		{112,  0,128, 16},// 
		{ 64,  0, 80, 16},// 

	};

	RECT rcRight[] = {

		{ 64, 16, 80, 32},// 停止
		{ 80, 16, 96, 32},

		{ 96, 16,112, 32},// 歩き
		{ 64, 16, 80, 32},// 
		{112, 16,128, 32},// 
		{ 64, 16, 80, 32},// 

	};
	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0;
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

	case 8: //歩行
		npc->act_no   = 9;
		npc->ani_no   = 2;
		npc->ani_wait = 0;
	case 9:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;

		break;
	}

	npc->ym += VS/8;

	if( npc->xm >  2*VS )npc->xm =  2*VS;
	if( npc->xm < -2*VS )npc->xm = -2*VS;
	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// ルアー
void ActNpc075( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{272, 32,296, 56},// 停止
		{296, 32,320, 56},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機
		//近づくとふり目を開ける…。
		if( npc->x - PARTSSIZE*VS*3 < gMC.x &&
			npc->x + PARTSSIZE*VS*3 > gMC.x &&
			npc->y - PARTSSIZE*VS*3 < gMC.y &&
			npc->y + PARTSSIZE*VS*1 > gMC.y ){

			npc->ani_no = 1;

		}else{
			npc->ani_no = 0;
		}
		break;
	}

	npc->rect = rcLeft[npc->ani_no];
}

// プランツ
void ActNpc076( NPCHAR *npc )
{
	npc->rect.left   = npc->code_event*16;
	npc->rect.top    = 0;
	npc->rect.right  = npc->rect.left+16;
	npc->rect.bottom = 16;
}

// アンブレラ
void ActNpc077( NPCHAR *npc )
{
	RECT rc[] = {
		{  0, 16,  48, 48},
		{ 48, 16,  96, 48},
		{ 96, 16, 144, 48},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
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
		break;	}

	if( npc->direct == DIR_LEFT ) npc->rect = rc[npc->ani_no];
	else                          npc->rect = rc[     2     ];
}


// つぼ
void ActNpc078( NPCHAR *npc )
{

	RECT rc[] = {
		{160, 48, 176, 64},
		{176, 48, 192, 64},
	};

	if( npc->direct == DIR_LEFT ) npc->rect = rc[0];
	else                          npc->rect = rc[1];

}

// ハム
void ActNpc079( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 16, 16},// 停止
		{ 16,  0, 32, 16},
		{ 32,  0, 48, 16},
	};

	RECT rcRight[] = {
		{  0, 16, 16, 32},// 停止
		{ 16, 16, 32, 32},
		{ 32, 16, 48, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 2;
		npc->ani_wait = 0;
	case 1:
		break;
	case 2://待機
		npc->ani_no = 0;
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 3;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		//離れると後ろ向きふり向く…。
		if( npc->x - PARTSSIZE*2*VS < gMC.x &&
			npc->x + PARTSSIZE*2*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y &&
			npc->y + PARTSSIZE*1*VS > gMC.y ){

			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}else{
//			npc->act_no   = 0;
		}
		break;

	case 3://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 2;
			npc->ani_no = 0;
		}
		break;
	}

	npc->ym += VS/8;

	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}
/*
//トロッコ
void ActNpc079( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{256, 64,288,80},
		{256, 80,288,96},

	};

	if(       npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ){
		npc->xm = 0;
		npc->direct = DIR_RIGHT;
	}else if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ){
		npc->xm = 0;
		npc->direct = DIR_LEFT;
	}

	switch( npc->act_no ){
	case 0://停止準備
		npc->act_no = 1;
	case 1://停止
		break;
	case 2://ランニング準備
		npc->act_no = 3;
	case 3://ランニング
		if( npc->direct == DIR_LEFT ) npc->xm += -VS/16;
		else                          npc->xm +=  VS/16;
		//アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}


	npc->ym += VS/8;
	if( npc->xm >  VS*2 )npc->xm =  VS*2 ;
	if( npc->xm < -VS*2 )npc->xm = -VS*2 ;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;


		npc->rect = rcLeft[npc->ani_no];
}
*/