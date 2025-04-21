#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"

//包丁ゾンビ
void ActNpc080( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{   0,  64,  24, 88},
		{  24,  64,  48, 88},
		{   0,  64,  24, 88},
		{  48,  64,  72, 88},

		{  72,  64,  96, 88},//かまえ
		{  96,  64, 120, 88},//きる
		{ 120,  64, 144, 88},
	};

	RECT rcRight[] = {
		{   0,  88,  24,112},
		{  24,  88,  48,112},
		{   0,  88,  24,112},
		{  48,  88,  72,112},

		{  72,  88,  96,112},//かまえ
		{  96,  88, 120,112},//きる
		{ 120,  88, 144,112},
	};

	switch( npc->act_no ){
	case 0:
		npc->bits     &= ~BITS_BANISH_DAMAGE;
		npc->act_no    = 1;
		npc->damage    = 0;
		npc->hit.front = 4*VS;
//		npc->hit.front = 4*VS;
	case 1:// 待機
		npc->ani_no = 0;
		//近づきあるき
		if( npc->x - 8 * PARTSSIZE*VS < gMC.x &&
			npc->x + 8 * PARTSSIZE*VS > gMC.x &&
			npc->y - 3 * PARTSSIZE*VS < gMC.y &&
			npc->y + 2 * PARTSSIZE*VS > gMC.y ){

			npc->ani_wait = 0;
			npc->act_no   = 2;
		}
		//ダメージあるき
		if( npc->shock ){
			npc->ani_no   = 1;
			npc->ani_wait = 0;
			npc->act_no   = 2;
			npc->bits &= ~BITS_BANISH_DAMAGE;// 防御
		}
		
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;

		break;
	case 2:// 歩く


		npc->ani_wait++;
		if( npc->ani_wait > 6 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 0;

		//切にかかかる
		if( npc->x - 1 * PARTSSIZE*VS < gMC.x &&
			npc->x + 1 * PARTSSIZE*VS > gMC.x ){

			npc->hit.front = 18*VS;
			npc->act_wait = 0;
			npc->act_no   = 3;
			npc->bits    |= BITS_BANISH_DAMAGE;

//			npc->ym = -VS;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
			else                          npc->xm =  VS*2;


		}

		if( gMC.x < npc->x ){
			npc->direct = DIR_LEFT;
			npc->xm = -VS/2;
		}else{
			npc->direct = DIR_RIGHT;
			npc->xm = VS/2;
		}
		
		break;

	case 3:// かまえ
		npc->xm = 0;
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_wait = 0;
			npc->act_no   = 4;
		}
		npc->ani_no = 4;
		break;

	case 4:// 切る
		npc->damage   = 10;
		npc->act_wait++;
		if( npc->act_wait > 2 ){
			npc->act_wait = 0;
			npc->act_no   = 5;
		}
		npc->ani_no = 5;
		break;

	case 5:// 切りポーズ
		npc->ani_no = 6;
		npc->act_wait++;
		if( npc->act_wait > 60 ){
			npc->act_no = 0;
		}
		break;
	}
	if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ) npc->xm = 0;
	if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ) npc->xm = 0;

	// 重力
	npc->ym += VS/16;
	if( npc->xm >  VS*2     ) npc->xm =  VS*2;
	if( npc->xm < -VS*2     ) npc->xm = -VS*2;
	if( npc->ym >  MAX_MOVE ) npc->xm =  MAX_MOVE;
	if( npc->ym < -MAX_MOVE ) npc->xm = -MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];


}
// ジャイアントピニョン
void ActNpc081( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{144, 64,168, 88},//停止
		{168, 64,192, 88},

		{192, 64,216, 88},//歩く
		{216, 64,240, 88},
		{144, 64,168, 88},

		{240, 64,264, 88},// shock
	};

	RECT rcRight[] = {

		{144, 88,168,112},//停止
		{168, 88,192,112},
				 
		{192, 88,216,112},//歩く
		{216, 88,240,112},
		{144, 88,168,112},
				 
		{240, 88,264,112},// shock
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
			if( npc->x < gMC.x ) npc->xm =  VS/2;
			else                 npc->xm = -VS/2;
		}
		break;
	}

	npc->ym += VS/8;

	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[  npc->ani_no ];
	else                          npc->rect = rcRight[ npc->ani_no ];
}




// ミザリー（地面）
void ActNpc082( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = 2;
	case 1:
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 3;
		}
		break;
	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 2;
		}

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
		if( npc->act_wait == 50 ){
			npc->act_no = 0;
			npc->ani_no = 0;
		}
		break;

	case 30://杖振り上げ ----
		npc->act_no   = 31;
		npc->ani_no   =  3;
		npc->ani_wait =  0;
	case 31:
		npc->ani_wait++;
		if( npc->ani_wait > 10 ){
			npc->act_no   = 32;
			npc->ani_no   =  4;
			npc->ani_wait =  0;
		}
		break;
	case 32:
		npc->ani_wait++;
		if( npc->ani_wait > 100 ){
			npc->act_no   =  1;
			npc->ani_no   =  2;
		}
		break;

	case 40:// ドクターへ攻撃
		npc->act_no   = 41;
		npc->act_wait =  0;
	case 41:
		npc->ani_no   =  4;
		npc->act_wait++;
		switch( npc->act_wait ){
		case 30: case 40: case 50:
			SetNpChar( 11, npc->x+8*VS, npc->y-8*VS, VS*3, Random( -VS, 0 ), DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_FRONTIA, 1 );
			break;
		}
		if( npc->act_wait > 50 ){
			npc->act_no = 0;
		}
		break;

	case 50: //跪く
		npc->ani_no = 8;
		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rcLeft[] = {
		{ 80,  0, 96, 16},//浮遊
		{ 96,  0,112, 16},
		{112,  0,128, 16},//2地面
		{128,  0,144, 16},
		{144,  0,160, 16},//杖

		{160,  0,176, 16},// 5Flash
		{176,  0,192, 16},
		{144,  0,160, 16},

		{208, 64,224, 80},// 8跪く
	};
	RECT rcRight[] = {
		{ 80, 16, 96, 32},//浮遊
		{ 96, 16,112, 32},
		{112, 16,128, 32},//2地面
		{128, 16,144, 32},
		{144, 16,160, 32},//杖

		{160, 16,176, 32},// 5Flash
		{176, 16,192, 32},
		{144, 16,160, 32},

		{208, 80,224, 96},// 8跪く
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

/*	if( npc->act_no == 1 ){
		if( npc->ani_wait < 16*2 ){
			npc->ani_wait++;
			npc->rect.bottom += (-16) + npc->ani_wait/2;
		}
	}
	*/
}

// イゴール(イベント)
void ActNpc083( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 40, 40},//待機
		{ 40,  0, 80, 40},

		{ 80,  0,120, 40},//歩く
		{  0,  0, 40, 40},
		{120,  0,160, 40},
		{  0,  0, 40, 40},

		{160,  0,200, 40},//ためて 6
		{200,  0,240, 40},//パンチ
	};

	RECT rcRight[] = {
		{  0, 40, 40, 80},//待機
		{ 40, 40, 80, 80},

		{ 80, 40,120, 80},//歩く
		{  0, 40, 40, 80},
		{120, 40,160, 80},
		{  0, 40, 40, 80},

		{160, 40,200, 80},//ためて 6
		{200, 40,240, 80},//パンチ
	};

	switch( npc->act_no ){

	// 待機------------------
	case 0:
		npc->xm = 0;
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1:

		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		break;
	// 前進------------------
	case 2:
		npc->act_no   = 3;
		npc->ani_no   = 2;
		npc->ani_wait = 0;
	case 3:

		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;

		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;

		break;

	// ハンマーパンチ--------
	case 4:
		npc->xm = 0;
		npc->act_no   = 5;
		npc->act_wait = 0;
		npc->ani_no   = 6;
	case 5:// 構え
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_wait = 0;
			npc->act_no   = 6;
			npc->ani_no   = 7;
			PlaySoundObject( WAVE_DESTROY_SMALL, 1 );
		}
		break;
	case 6:// なぐる -> 待機
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no   = 0;
			npc->ani_no   = 0;
		}
		break;

	// スーを捕まえる--------
	case 7:
		npc->act_no = 1;
	case 8:
		break;
	
	}

	npc->ym += VS/8;

	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// 超音波
void ActNpc084( NPCHAR *npc )
{
	//地面に着地
	if( npc->flag & FLAG_HIT_X ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}

	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rect_left[] = {
		{48,48,64,64},
		{64,48,80,64},
		{48,64,64,80},
		{64,64,80,80},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3)npc->ani_no = 0;
	npc->rect = rect_left[npc->ani_no];

	//時間消滅
	npc->count1++;
	if( npc->count1 > 300 ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}
}

// タマゴモニター
void ActNpc085( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{256, 96, 272, 120},
		{256, 96, 272, 120},
		{272, 96, 288, 120},
	};
	RECT rcRight[] = {
		{256, 96, 272, 120},
		{288, 96, 304, 120},
		{304, 96, 320, 120},
	};

	switch( npc->act_no ){
	case 0:
		npc->ani_no = 0;
		//近づき点灯
		if( npc->x -  8*VS < gMC.x &&
			npc->x +  8*VS > gMC.x &&
			npc->y - 16*VS < gMC.y &&
			npc->y +  8*VS > gMC.y ){

			PlaySoundObject( WAVE_ON, 1 );

			npc->act_no   = 1;
		}
		break;
	case 1:
		npc->ani_no++;
		if( npc->ani_no > 2 )
			npc->ani_no = 1;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// 弾数アイテム
void ActNpc086( NPCHAR *npc )
{
	RECT rect1[] = {
		{  0, 80, 16, 96},
		{ 16, 80, 32, 96},
	};
	RECT rect3[] = {
		{  0,112, 16,128},
		{ 16,112, 32,128},
	};

	RECT rcLast = { 16,0, 32,16};

	if( npc->direct == DIR_LEFT ){
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1)npc->ani_no = 0;
	}

	if( gBack.type == BKACT_FLOW || gBack.type == BKACT_CROUD ){

		if( !npc->act_no ){
			npc->act_no = 1;
			npc->ym = Random( -VS/16, VS/16 );
			npc->xm = Random(   127, 256 );
		}
		npc->xm -= VS/64;
		if( npc->x < VS*PARTSSIZE*5     ) npc->cond = 0;
		if( npc->x < -3*VS ) npc->x = -3*VS;
		if( npc->flag & FLAG_HIT_LEFT   ) npc->xm =  VS/2;
		if( npc->flag & FLAG_HIT_TOP    ) npc->ym =  VS/8;
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS/8;
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	switch( npc->exp ){
	case 1: npc->rect = rect1[npc->ani_no]; break;
	case 3: npc->rect = rect3[npc->ani_no]; break;
	}

	//時間で消滅
	if( npc->direct == DIR_LEFT                ) npc->count1++;
	if( npc->count1 > 11*50                    ) npc->cond       = 0;
	if( npc->count1 > 10*50 && npc->count1/2%2 ) npc->rect.right = 0;
	if( npc->count1 > 11*50-3                  ) npc->rect       = rcLast; 
}

// 体力アイテム
void ActNpc087( NPCHAR *npc )
{
	RECT rect2[] = {
		{ 32, 80, 48, 96},
		{ 48, 80, 64, 96},
	};
	RECT rect6[] = {
		{ 64, 80, 80, 96},
		{ 80, 80, 96, 96},
	};

	RECT rcLast = { 16,0, 32,16};

	if( npc->direct == DIR_LEFT ){
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1)npc->ani_no = 0;
	}

	if( gBack.type == BKACT_FLOW || gBack.type == BKACT_CROUD ){

		if( !npc->act_no ){
			npc->act_no = 1;
			npc->ym = Random( -VS/16, VS/16 );
			npc->xm = Random(   127, 256 );
		}
		npc->xm -= VS/64;
		if( npc->x < VS*PARTSSIZE*5     ) npc->cond = 0;
		if( npc->x < -3*VS ) npc->x = -3*VS;
		if( npc->flag & FLAG_HIT_LEFT   ) npc->xm =  VS/2;
		if( npc->flag & FLAG_HIT_TOP    ) npc->ym =  VS/8;
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS/8;
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	switch( npc->exp ){
	case 2: npc->rect = rect2[npc->ani_no]; break;
	case 6: npc->rect = rect6[npc->ani_no]; break;
	}

	//時間で消滅
	if( npc->direct == DIR_LEFT                ) npc->count1++;
	if( npc->count1 > 11*50                    ) npc->cond       = 0;
	if( npc->count1 > 10*50 && npc->count1/2%2 ) npc->rect.right = 0;
	if( npc->count1 > 11*50-3                  ) npc->rect       = rcLast; 
}


// イゴール(戦闘)
void ActNpc088( NPCHAR *npc )
{
	long i;
	unsigned char deg;
	long xm, ym;

	RECT rcLeft[] = {
		{  0,  0, 40, 40},//待機
		{ 40,  0, 80, 40},

		{ 80,  0,120, 40},//歩く
		{  0,  0, 40, 40},
		{120,  0,160, 40},
		{  0,  0, 40, 40},

		{160,  0,200, 40},//ためて 6
		{200,  0,240, 40},//パンチ

		{  0, 80, 40,120},//ジャンプ 8
		{ 40, 80, 80,120},//かがみ

		{240,  0,280, 40},//波動砲
		{280,  0,320, 40},
	
	};

	RECT rcRight[] = {
		{  0, 40, 40, 80},//待機
		{ 40, 40, 80, 80},

		{ 80, 40,120, 80},//歩く
		{  0, 40, 40, 80},
		{120, 40,160, 80},
		{  0, 40, 40, 80},

		{160, 40,200, 80},//ためて 6
		{200, 40,240, 80},//パンチ

		{120, 80,160,120},
		{160, 80,200,120},
	
		{240, 40,280, 80},//波動砲
		{280, 40,320, 80},
	};

	switch( npc->act_no ){

	// 待機------------------
	case 0:
		npc->xm       = 0;
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;

//		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
//		else                 npc->direct = DIR_RIGHT;

	case 1:

		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		// 前進へ
		npc->act_wait++;
		if( npc->act_wait > 50*1 ){
			npc->act_no = 2;
		}

		break;
	// 前進------------------
	case 2:
		npc->act_no   = 3;
		npc->act_wait = 0;
		npc->ani_no   = 2;
		npc->ani_wait = 0;

		npc->count1++;

		// ライフがあるときは波動砲は使わない
		if( npc->count1 < 3 || npc->life > 150 ){
			npc->count2 = 0;
			if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}else{
			npc->count2 = 1;
			if( gMC.x < npc->x ) npc->direct = DIR_RIGHT;
			else                 npc->direct = DIR_LEFT;
		}
	
	case 3:

		npc->act_wait++;
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;

		if( npc->direct == DIR_LEFT ) npc->xm = -VS;//*3/2;
		else                          npc->xm =  VS;//*3/2;

		// 波動砲へ
		if( npc->count2 ){
			if( npc->act_wait > 16 ){
				npc->act_no = 9;
				npc->xm     = 0;
				npc->ani_no = 10;
				break;
			}
		}else{
			//ジャンプ
			if( npc->act_wait > 50*1 ){

				npc->ani_no   = 8;
				npc->ym       = -2*VS;
				npc->act_no   = 7;
				npc->act_wait = 0;
				npc->xm = npc->xm*3/2;
				npc->damage   = 2;
				break;
			}

			// ハンマーパンチへ
			if( npc->direct == DIR_LEFT ){
				if( npc->x -24*VS < gMC.x ) npc->act_no = 4;
			}else{
				if( npc->x +24*VS > gMC.x ) npc->act_no = 4;
			}
		}

		break;

	// ハンマーパンチ--------
	case 4:
		npc->xm = 0;
		npc->act_no   = 5;
		npc->act_wait = 0;
		npc->ani_no   = 6;
	case 5:// 構え
		npc->act_wait++;
		if( npc->act_wait > 12 ){
			npc->act_wait  = 0;
			npc->act_no    = 6;
			npc->ani_no    = 7;
			PlaySoundObject( WAVE_DESTROY_SMALL, 1 );
			npc->damage    = 5;
			npc->hit.front = 24*VS;
			npc->hit.top   = 1;
		}
		break;
	case 6:// なぐる -> 待機
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no    = 0;
			npc->ani_no    = 0;
			npc->damage    = 0;
			npc->hit.front = 8*VS;
			npc->hit.top   = 16*VS;
		}
		break;

	// ジャンプ--------
	case 7:
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no    = 8;
			npc->ani_no    = 9;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
			npc->damage = 0;
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
		break;
	case 8:// 着陸
		npc->xm = 0;
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no    = 0;
			npc->ani_no    = 0;
			npc->damage    = 0;
		}
		break;
	

	case 9:// 波動砲-----------------
		npc->act_no   = 10;
		npc->act_wait = 0;
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 10://エネルギー充填／弾吐き
		npc->act_wait++;
		if( npc->act_wait > 50*2 && npc->act_wait%6 == 1 ){

			if( npc->direct == DIR_LEFT ) deg = 128+8;
			else                          deg = 256-8;
			deg += (unsigned char)Random( -16, 16 );
			ym = GetSin( deg ) * 3;
			xm = GetCos( deg ) * 3;
			SetNpChar( 11, npc->x, npc->y+4*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_BREAK1, 1 );
		}
		//アニメーション
		if( npc->act_wait > 50 && npc->act_wait/2%2 ) npc->ani_no = 11;
		else                                          npc->ani_no = 10;

		if( npc->act_wait > 50*2 + 32 ){
			npc->act_no    = 0;
			npc->ani_no    = 0;
			npc->count1    = 0;// 波動砲カウント
		}
		break;
	}

	npc->ym += VS/8;

	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}



// イゴール(敗北)
void ActNpc089( NPCHAR *npc )
{
	long i;
	RECT rcLeft[] = {
		{ 80, 80,120,120},
		{240, 80,264,104},
		{264, 80,288,104},
		{288, 80,312,104},
	};
	RECT rcRight[] = {
		{200, 80,240,120},
		{240,104,264,128},
		{264,104,288,128},
		{288,104,312,128},
	};

	switch( npc->act_no ){
	case 0:
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		npc->act_no = 1;
	case 1://ぶるぶる
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_wait = 0;
			npc->act_no   = 2;
		}
		if( npc->act_wait%5 == 0 )
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );

		if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ 0];
		else                          npc->rect = rcRight[0];
		if( npc->act_wait/2%2 ){
			npc->rect.left -= 1;
		}

		break;
	case 2://ちいさく
		npc->act_wait++;
		if( npc->act_wait/2%2 && npc->act_wait < 100 ){
			npc->ani_no = 0;
			npc->view.back  = 20*VS;
			npc->view.front = 20*VS;
			npc->view.top   = 20*VS;
		}else{
			npc->ani_no = 1;
			npc->view.back  = 12*VS;
			npc->view.front = 12*VS;
			npc->view.top   =  8*VS;
		}

		if( npc->act_wait > 150 ){
			npc->act_wait = 0;
			npc->act_no   = 3;
			npc->ani_no   = 1;
		}
		if( npc->act_wait%9 == 0 )
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
		else                          npc->rect = rcRight[npc->ani_no];
		break;
	case 3://倒れアニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 50 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no == 3 ){
			npc->act_no = 4;
		}
		if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
		else                          npc->rect = rcRight[npc->ani_no];
		break;
	case 4:
		break;
	}
	npc->ym += VS/8;

	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

}

// イゴール(うなだれ)
void ActNpc090( NPCHAR *npc )
{
	RECT rect = {280, 80,296,104};
	npc->rect = rect;
}

// 檻
void ActNpc091( NPCHAR *npc )
{
	RECT rect = { 96, 88,128,112};

	if( !npc->act_no ){
		npc->act_no++;
		npc->y += 16*VS;
	}
	npc->rect = rect;
}

// PCスー
void ActNpc092( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{272,216,288,240},
		{288,216,304,240},
		{304,216,320,240},
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


// チャコ
void ActNpc093( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{128,  0,144, 16},//停止
		{144,  0,160, 16},

		{160,  0,176, 16},//歩く
		{128,  0,144, 16},
		{176,  0,192, 16},
		{128,  0,144, 16},

		{ 32, 32, 48, 48},// 6 寝る

	};

	RECT rcRight[] = {

		{128, 16,144, 32},//停止
		{144, 16,160, 32},

		{160, 16,176, 32},//歩く
		{128, 16,144, 32},
		{176, 16,192, 32},
		{128, 16,144, 32},

		{ 32, 32, 48, 48},// 6 寝る
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
		//近づくとふり向く…。
		if( npc->x - PARTSSIZE*2*VS < gMC.x &&
			npc->x + PARTSSIZE*2*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y &&
			npc->y + PARTSSIZE*1*VS > gMC.y ){

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

	case 3: //歩行
		npc->act_no   = 4;
		npc->ani_no   = 2;
		npc->ani_wait = 0;
	case 4:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->x -= VS;
		else                          npc->x += VS;

		break;
	case 10:// 寝る
		npc->ani_no = 6;
		npc->act_wait++;
		if( npc->act_wait > 200 ){
			npc->act_wait = 0;
			SetCaret( npc->x, npc->y, CARET_ZZZ, DIR_LEFT );
		}
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// クララ
void ActNpc094( NPCHAR *npc )
{
	RECT rect[] = {
		{272,  0,320 , 24},
		{272, 24,320 , 48},
		{272, 48,320 , 72},
		{272, 72,320 , 96},
		{272, 96,320 ,120},
	};

	switch( npc->act_no ){
	case 0:
		npc->ani_no = 4;
		if( npc->shock ){
			npc->ani_no   =  0;
			npc->act_no   = 10;
			npc->act_wait =  0;
		}
		break;
	case 10:
		npc->bits |=  BITS_BANISH_DAMAGE; 
		npc->bits &= ~BITS_BLOCK_BULLET;
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_wait =  0;
			npc->ani_wait =  0;
			npc->act_no   = 11;
		}
		break;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 ){
			npc->act_no = 12;
			npc->ani_no =  3;
		}
		break;
	case 12://上昇
		npc->ym = -VS*2/3;
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_wait =  0;
			npc->act_no   = 10;
			npc->ani_no   =  0;
		}
		break;
	case 20:
		npc->xm = npc->xm/2;
//		if( npc->ym < 0 )
			npc->ym += VS/16;
		if( !npc->shock ){
			npc->act_wait =  30;
			npc->act_no   =  10;
			npc->ani_no   =  0;
		}
		break;
	}

	if( npc->shock ){
		npc->count2++;
		if( npc->count2 > WAIT_NPC_DAMAGE-4 ){
			npc->act_no = 20;
			npc->ani_no = 4;
			npc->bits &= ~BITS_BANISH_DAMAGE; 
			npc->bits |=  BITS_BLOCK_BULLET; 
		}
	}else{
		npc->count2 = 0;
	}



	if( npc->act_no >= 10 ){

		if( npc->flag & FLAG_HIT_LEFT  ){
			npc->count1 = 50;
			npc->direct = DIR_RIGHT;
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->count1 = 50;
			npc->direct = DIR_LEFT;
		}

		// x 移動量
		if( npc->count1 ){
			npc->count1--;
			if( npc->direct == DIR_LEFT ) npc->xm -= VS/4;
			else                          npc->xm += VS/4;
		}else{
				npc->count1 = 50;
			if( npc->x > gMC.x ){
				npc->direct = DIR_LEFT;
			}else{
				npc->direct = DIR_RIGHT;
			}
		}
		
		
		npc->ym += VS/32;
		if( npc->flag & FLAG_HIT_BOTTOM )
			npc->ym = -VS*2;
	}

	if( npc->xm >  VS/2     )npc->xm =  VS/2;
	if( npc->xm < -VS/2     )npc->xm = -VS/2;
	if( npc->ym >  VS*3/2 )npc->ym =  VS*3/2;
	if( npc->ym < -VS*3/2 )npc->ym = -VS*3/2;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
}

// ヤブクラゲ
void ActNpc095( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{208, 64,224, 80},
		{224, 64,240, 80},
		{240, 64,256, 80},
		{256, 64,272, 80},
	};

	RECT rcRight[] = {
		{208, 80,224, 96},
		{224, 80,240, 96},
		{240, 80,256, 96},
		{256, 80,272, 96},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->act_wait = Random( 0, 50 );
		npc->tgt_y  = npc->y;
		npc->tgt_x  = npc->x;
		if( npc->direct == DIR_LEFT ) npc->xm = VS;
		else                          npc->xm = -VS;
	case 1:
		npc->act_wait--;
		if( npc->act_wait > 0 ) break;
		npc->act_no = 10;
	case 10:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_wait =  0;
			npc->ani_wait =  0;
			npc->act_no   = 11;
		}
		break;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no == 2 ){
			if( npc->direct == DIR_LEFT ) npc->xm -= VS/2;
			else                          npc->xm += VS/2;
			npc->ym -= VS;
		}
		if( npc->ani_no > 2 ){
			npc->act_no = 12;
			npc->ani_no =  3;
		}
		break;
	case 12:
		npc->act_wait++;
		if( npc->y > npc->tgt_y && npc->act_wait > 10 ){
			npc->act_wait =  0;
			npc->act_no   = 10;
			npc->ani_no   =  0;
		}
		break;
	}


			if( npc->x > npc->tgt_x ) npc->direct = DIR_LEFT;
			else                      npc->direct = DIR_RIGHT;


	if( npc->flag & FLAG_HIT_LEFT  ){
		npc->count1 = 50;
		npc->direct = DIR_RIGHT;
	}
	if( npc->flag & FLAG_HIT_RIGHT ){
		npc->count1 = 50;
		npc->direct = DIR_LEFT;
	}

/*	// x 移動量
	if( npc->count1 ){
		npc->count1--;
		if( npc->direct == DIR_LEFT ) npc->xm -= VS/4;
		else                          npc->xm += VS/4;
	}else{
		if( npc->x < gMC.x ) npc->xm -= VS/4;
		else                 npc->xm += VS/4;
	}
*/	
	
	npc->ym += VS/16;
	if( npc->flag & FLAG_HIT_BOTTOM )
		npc->ym = -VS*2;

	if( npc->xm >  VS/2     )npc->xm =  VS/2;
	if( npc->xm < -VS/2     )npc->xm = -VS/2;
	if( npc->ym >  VS )npc->ym =  VS;
	if( npc->ym < -VS )npc->ym = -VS;

	if( npc->shock ){
		npc->x += npc->xm/2;
		npc->y += npc->ym/2;
	}else{
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}




// ホバー←
void ActNpc096( NPCHAR *npc )
{

	RECT rc[] = {
		{272,120,288,136},
		{288,120,304,136},
		{304,120,320,136},
	};
	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_RIGHT ) npc->act_no = 2;
		else                           npc->ani_no = 1;
	case 1:
		npc->ani_no = 0;
		break;
	// 旋風
	case 2:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;

		if( gMC.x > npc->x-VS*320 && gMC.x < npc->x + VS*320 &&
			gMC.y > npc->y-VS*240 && gMC.y < npc->y + VS*240 &&
			1 == Random( 0, 5 ) )
			SetNpChar( 199, npc->x, npc->y + Random( -8, 8 )*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 ); 

		if( gMC.y < npc->y +VS*8 &&
			gMC.y > npc->y -VS*8 &&
			gMC.x < npc->x        &&
			gMC.x > npc->x - VS*6*PARTSSIZE ){
			gMC.xm -= (VS/4 + VS/64);
			gMC.cond |= COND_FLOW;
		}
		break;
	}

	npc->rect = rc[npc->ani_no];
}

// ホバー↑
void ActNpc097( NPCHAR *npc )
{

	RECT rc[] = {
		{272,136,288,152},
		{288,136,304,152},
		{304,136,320,152},
	};
	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_RIGHT ) npc->act_no = 2;
		else                           npc->ani_no = 1;
	case 1:
		npc->ani_no = 0;
		break;
	// 旋風
	case 2:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;

		if( gMC.x > npc->x-VS*320 && gMC.x < npc->x + VS*320 &&
			gMC.y > npc->y-VS*240 && gMC.y < npc->y + VS*240 &&
			1 == Random( 0, 5 ) )
			SetNpChar( 199, npc->x + Random( -8, 8 )*VS, npc->y, 0, 0, DIR_UP, NULL, MAX_NPC/2 ); 

		if( gMC.x < npc->x +VS*8 &&
			gMC.x > npc->x -VS*8 &&
			gMC.y < npc->y        &&
			gMC.y > npc->y - VS*6*PARTSSIZE ){
			gMC.ym -= (VS/4 + VS/64);
		}
		break;
	}

	npc->rect = rc[npc->ani_no];
}
// ホバー→
void ActNpc098( NPCHAR *npc )
{

	RECT rc[] = {
		{272,152,288,168},
		{288,152,304,168},
		{304,152,320,168},
	};
	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_RIGHT ) npc->act_no = 2;
		else                           npc->ani_no = 1;
	case 1:
		npc->ani_no = 0;
		break;
	// 旋風
	case 2:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;

		if( gMC.x > npc->x-VS*320 && gMC.x < npc->x + VS*320 &&
			gMC.y > npc->y-VS*240 && gMC.y < npc->y + VS*240 &&
			1 == Random( 0, 5 ) )
			SetNpChar( 199, npc->x, npc->y + Random( -8, 8 )*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 ); 

		if( gMC.y < npc->y +VS*8 &&
			gMC.y > npc->y -VS*8 &&
			gMC.x < npc->x + VS*6*PARTSSIZE &&
			gMC.x > npc->x  ){
			gMC.xm += (VS/4 + VS/64);
			gMC.cond |= COND_FLOW;
		}
		break;
	}

	npc->rect = rc[npc->ani_no];
}
// ホバー↓
void ActNpc099( NPCHAR *npc )
{

	RECT rc[] = {
		{272,168,288,184},
		{288,168,304,184},
		{304,168,320,184},
	};
	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_RIGHT ) npc->act_no = 2;
		else                           npc->ani_no = 1;
	case 1:
		npc->ani_no = 0;
		break;
	// 旋風
	case 2:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;

		if( gMC.x > npc->x-VS*320 && gMC.x < npc->x + VS*320 &&
			gMC.y > npc->y-VS*240 && gMC.y < npc->y + VS*240 &&
			1 == Random( 0, 5 ) )
			SetNpChar( 199, npc->x + Random( -8, 8 )*VS, npc->y, 0, 0, DIR_DOWN, NULL, MAX_NPC/2 ); 

		if( gMC.x < npc->x +VS*8 &&
			gMC.x > npc->x -VS*8 &&
			gMC.y < npc->y + VS*6*PARTSSIZE       &&
			gMC.y > npc->y  ){
			gMC.ym += (VS/4 + VS/64);
		}
		break;
	}

	npc->rect = rc[npc->ani_no];
}



