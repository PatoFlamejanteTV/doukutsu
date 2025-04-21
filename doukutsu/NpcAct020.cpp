#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"

//パソコン
void ActNpc020( NPCHAR *npc )
{

	RECT rcLeft  = {288, 16,320, 40};
	RECT rcRight[] = {
		{288, 40,320, 64},
		{288, 40,320, 64},
		{288, 64,320, 88},
	};


	npc->ani_wait++;
	if( npc->ani_wait > 3 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 ) npc->ani_no = 0;
	
	//アニメーション------------------
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft;
	else                          npc->rect = rcRight[npc->ani_no];
}

//空箱
void ActNpc021( NPCHAR *npc )
{

	if( !npc->act_no ){
		npc->act_no = 1;
		if( npc->direct == DIR_RIGHT ) npc->y += 16*VS;
	}
	RECT rect = {224, 40, 240, 48};
	npc->rect = rect;
}

//転送装置
void ActNpc022( NPCHAR *npc )
{
	RECT rect[] = {
		{240, 16,264, 48},
		{248,152,272,184},
	};

	switch( npc->act_no ){
	case 0:
		npc->ani_no = 0;
		break;
	case 1:
		npc->ani_no++;
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}

	npc->rect = rect[npc->ani_no];

}

//転送装置ランプ
void ActNpc023( NPCHAR *npc )
{
	RECT rect[] = {
		{264, 16, 288, 20},
		{264, 20, 288, 24},
		{264, 24, 288, 28},
		{264, 28, 288, 32},
		{264, 32, 288, 36},
		{264, 36, 288, 40},
		{264, 40, 288, 44},
		{264, 44, 288, 48},
	};
	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 7 ) npc->ani_no = 0;
	npc->rect = rect[npc->ani_no];
}


//パワークリッター
void ActNpc024( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 24, 24},
		{ 24,  0, 48, 24},
		{ 48,  0, 72, 24},//ジャンプ
		{ 72,  0, 96, 24},
		{ 96,  0,120, 24},
		{120,  0,144, 24},
	};
	RECT rcRight[] = {
		{  0, 24, 24, 48},
		{ 24, 24, 48, 48},
		{ 48, 24, 72, 48},//ジャンプ
		{ 72, 24, 96, 48},
		{ 96, 24,120, 48},
		{120, 24,144, 48},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->y += 3*VS;
		npc->act_no = 1;
	case 1://待機
		if( npc->act_wait >= 8 &&
			npc->x - PARTSSIZE*8*VS < gMC.x &&
			npc->x + PARTSSIZE*8*VS > gMC.x &&
			npc->y - PARTSSIZE*8*VS < gMC.y &&
			npc->y + PARTSSIZE*3*VS > gMC.y ){

			if( npc->x > gMC.x )npc->direct = DIR_LEFT;
			else                npc->direct = DIR_RIGHT;
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
			npc->x - PARTSSIZE*6*VS < gMC.x &&
			npc->x + PARTSSIZE*6*VS > gMC.x &&
			npc->y - PARTSSIZE*6*VS < gMC.y &&
			npc->y + PARTSSIZE*3*VS > gMC.y ){

			npc->act_no   = 2;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}		break;

	case 2://かがむ
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 3;
			npc->ani_no = 2;
			npc->ym     = -MAX_MOVE;
			PlaySoundObject( WAVE_BIGJUMP, 1 );
			if( npc->x > gMC.x )npc->direct = DIR_LEFT;
			else                npc->direct = DIR_RIGHT;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
			else                          npc->xm =  VS/2;
		}
		break;

	case 3://上昇
		if( npc->ym > VS ){
			npc->tgt_y    = npc->y;
			npc->act_no   = 4;
			npc->ani_no   = 3;
			npc->act_wait = 0;
			npc->act_wait = 0;

		}
		break;
	case 4://滞空
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
		//壁に衝突
		npc->act_wait++;
		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT|FLAG_HIT_TOP) ||
			npc->act_wait > 50*2 ){

			npc->damage   = 12;
			npc->act_no   = 5;
			npc->ani_no   = 2;
			npc->xm = npc->xm/2;
			break;
		}
		if( npc->act_wait%4 == 1 ) PlaySoundObject( WAVE_HERI2, 1 );

	
		npc->ani_wait++;
		if(npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 ) npc->ani_no = 3;
		break;
	case 5://落下
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->damage   = 2;//DANAGE 2
			npc->xm       = 0;
			npc->act_wait = 0;
			npc->ani_no   = 0;
			npc->act_no   = 1;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
		}
		break;
	}

	//移動量
	if( npc->act_no != 4 ){
		//重力
		npc->ym += VS/16;
		if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	}else{
		if( npc->x < gMC.x ) npc->xm += VS/16;
		else                 npc->xm -= VS/16;
		//浮遊
		if( npc->y > npc->tgt_y ) npc->ym -= VS/32;
		else                      npc->ym += VS/32;
		if( npc->ym >  VS )npc->ym =  VS;
		if( npc->ym < -VS )npc->ym = -VS;
		if( npc->xm >  VS )npc->xm =  VS;
		if( npc->xm < -VS )npc->xm = -VS;
	}
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];

}

// エレベーター
void ActNpc025( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{256, 64,288, 80},
		{256, 80,288, 96},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->x += VS*8;
	case 1://F1待機
		npc->act_wait++;
		if( npc->act_wait > 50*3 ){
			npc->act_wait = 0;
			npc->act_no++;
		}
		break;
	case 2://F1F2移動
		npc->act_wait++;
		if( npc->act_wait <= PARTSSIZE*4 ){
			npc->y -= VS;
		}else{
			npc->act_wait = 0;
			npc->act_no++;
		}
		break;
	case 3://F2待機
		npc->act_wait++;
		if( npc->act_wait > 50*3 ){
			npc->act_wait = 0;
			npc->act_no++;
		}
		break;
	case 4://F2F3移動
		npc->act_wait++;
		if( npc->act_wait <= PARTSSIZE*4 ){
			npc->y -= VS;
		}else{
			npc->act_wait = 0;
			npc->act_no++;
		}
		break;
	case 5://F3待機
		npc->act_wait++;
		if( npc->act_wait > 50*3 ){
			npc->act_wait = 0;
			npc->act_no++;
		}
		break;
	case 6://F3F2移動
		npc->act_wait++;
		if( npc->act_wait <= PARTSSIZE*4 ){
			npc->y += VS;
		}else{
			npc->act_wait = 0;
			npc->act_no++;
		}
		break;
	case 7://F2待機
		npc->act_wait++;
		if( npc->act_wait > 50*3 ){
			npc->act_wait = 0;
			npc->act_no++;
		}
		break;
	case 8://F2F1移動
		npc->act_wait++;
		if( npc->act_wait <= PARTSSIZE*4 ){
			npc->y += VS;
		}else{
			npc->act_wait = 0;
			npc->act_no   = 1;
		}
		break;
	}

	switch( npc->act_no ){
	case 2:
	case 4:
	case 6:
	case 8:
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}

	npc->rect = rcLeft[npc->ani_no];
}



//バット滞空型
void ActNpc026( NPCHAR *npc )
{
	unsigned char deg;
	switch( npc->act_no ){
	case 0://初期化
		deg = (unsigned char)Random( 0, 255 );
		npc->xm = GetCos( deg ) * 1;
		deg += 64;
		npc->tgt_x = npc->x + GetCos( deg )*8;
		deg = (unsigned char)Random( 0, 255 );
		npc->ym = GetSin( deg ) * 1;
		deg += 64;
		npc->tgt_y = npc->y + GetSin( deg )*8;
		npc->act_no = 1;
		npc->count1 = 120;
	case 1://滞空
		//振り向き
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->tgt_x < npc->x ) npc->xm -= VS/32;
		if( npc->tgt_x > npc->x ) npc->xm += VS/32;
		if( npc->tgt_y < npc->y ) npc->ym -= VS/32;
		if( npc->tgt_y > npc->y ) npc->ym += VS/32;

		if( npc->xm >  1*VS )npc->xm =  1*VS;
		if( npc->xm < -1*VS )npc->xm = -1*VS;
		if( npc->ym >  1*VS )npc->ym =  1*VS;
		if( npc->ym < -1*VS )npc->ym = -1*VS;


		if( npc->count1 < 120 ){
			npc->count1++;
			break;
		}
		if( npc->x-8*VS < gMC.x && npc->x+8*VS            > gMC.x &&
			npc->y      < gMC.y && npc->y+6*PARTSSIZE*VS > gMC.y ){
			npc->xm /= 2;
			npc->ym = 0;
			npc->act_no = 3;
			npc->bits &= ~BITS_THROW_BLOCK;
		}
		break;
	case 3:
		//落下
		npc->ym += VS/8;
		if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym = 0;
			npc->xm *= 2;
			npc->count1 = 0;
			npc->act_no = 1;
			npc->bits |= BITS_THROW_BLOCK;
		}
		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{ 32, 80, 48, 96},
		{ 48, 80, 64, 96},
		{ 64, 80, 80, 96},
		{ 80, 80, 96, 96},
	};
	RECT rect_right[] = {
		{ 32, 96, 48,112},
		{ 48, 96, 64,112},
		{ 64, 96, 80,112},
		{ 80, 96, 96,112},
	};

	if( npc->act_no == 3 ){
		npc->ani_no = 3;
	}else{
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )npc->ani_no = 0;
	}
	if( npc->direct == DIR_LEFT )
		npc->rect = rect_left[npc->ani_no];
	else 
		npc->rect = rect_right[npc->ani_no];
}

//デストラップ
void ActNpc027( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 96, 64,128, 88},
	};
	npc->rect = rcLeft[npc->ani_no];
}

//クリッター LV2
void ActNpc028( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 48, 16, 64},
		{ 16, 48, 32, 64},
		{ 32, 48, 48, 64},//ジャンプ
		{ 48, 48, 64, 64},
		{ 64, 48, 80, 64},
		{ 80, 48, 96, 64},
	};
	RECT rcRight[] = {
		{  0, 64, 16, 80},
		{ 16, 64, 32, 80},
		{ 32, 64, 48, 80},//ジャンプ
		{ 48, 64, 64, 80},
		{ 64, 64, 80, 80},
		{ 80, 64, 96, 80},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->y += 3*VS;
		npc->act_no = 1;
	case 1://待機
		if( npc->act_wait >= 8 &&
			npc->x - PARTSSIZE*8*VS < gMC.x &&
			npc->x + PARTSSIZE*8*VS > gMC.x &&
			npc->y - PARTSSIZE*8*VS < gMC.y &&
			npc->y + PARTSSIZE*3*VS > gMC.y ){

			if( npc->x > gMC.x )npc->direct = DIR_LEFT;
			else                npc->direct = DIR_RIGHT;
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
			npc->x - PARTSSIZE*6*VS < gMC.x &&
			npc->x + PARTSSIZE*6*VS > gMC.x &&
			npc->y - PARTSSIZE*6*VS < gMC.y &&
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
			npc->ym     = -MAX_MOVE*4/5;
			PlaySoundObject( WAVE_NPCJUMP1, 1 );
			if( npc->x > gMC.x )npc->direct = DIR_LEFT;
			else                npc->direct = DIR_RIGHT;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
			else                          npc->xm =  VS/2;
		}
		break;

	case 3://上昇
		if( npc->ym > VS/2 ){
			npc->tgt_y    = npc->y;
			npc->act_no   = 4;
			npc->ani_no   = 3;
			npc->act_wait = 0;
			npc->act_wait = 0;

		}
		break;
	case 4://滞空
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
		//壁に衝突
		npc->act_wait++;
		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT|FLAG_HIT_TOP) ||
			npc->act_wait > 50*2 ){

			npc->damage   = 3;//DANAGE 4
			npc->act_no   = 5;
			npc->ani_no   = 2;
			npc->xm = npc->xm/2;
			break;
		}

		if( npc->act_wait%4 == 1 ) PlaySoundObject( WAVE_HERI, 1 );
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym = -VS;
		}
	
		npc->ani_wait++;
		if(npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 ) npc->ani_no = 3;
		break;
	case 5://落下
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->damage   = 2;//DANAGE 2
			npc->xm       = 0;
			npc->act_wait = 0;
			npc->ani_no   = 0;
			npc->act_no   = 1;
			PlaySoundObject( WAVE_STEP, 1 );
		}
		break;
	}

	//移動量
	if( npc->act_no != 4 ){
		//重力
		npc->ym += VS/8;
		if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	}else{
		if( npc->x < gMC.x ) npc->xm += VS/16;
		else                 npc->xm -= VS/16;
		//浮遊
		if( npc->y > npc->tgt_y ) npc->ym -= VS/32;
		else                      npc->ym += VS/32;
		if( npc->ym >  VS )npc->ym =  VS;
		if( npc->ym < -VS )npc->ym = -VS;
		if( npc->xm >  VS )npc->xm =  VS;
		if( npc->xm < -VS )npc->xm = -VS;
	}
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];

}

// クトゥルー
void ActNpc029( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,192, 16,216},
		{ 16,192, 32,216},
	};
	RECT rcRight[] = {
		{  0,216, 16,240},
		{ 16,216, 32,240},
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

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// モンク
void ActNpc030( NPCHAR *npc )
{
	RECT rc[] = {
		{ 48,  0, 64, 16},
		{ 48, 16, 64, 32},
		{  0, 32, 16, 48},
	};
	
	if( npc->direct == DIR_LEFT ){
		switch( npc->act_no ){
		case 0:
			npc->act_no   = 1;
			npc->ani_no   = 0;
			npc->ani_wait = 0;
		case 1://待機
//			npc->act_wait++;
//			if( npc->act_wait/2%2 ) npc->ani_no = 0;
//			else                    npc->ani_no = 1;
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
	}else{
		if( !npc->act_no ){
			npc->act_no = 1;
			npc->y     += VS*16;
			npc->ani_no = 2;
		}
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_wait = 0;
			SetCaret( npc->x, npc->y-2*VS, CARET_ZZZ, DIR_LEFT );
		}
	}
	npc->rect = rc[npc->ani_no];


}


//バット天井
void ActNpc031( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 80, 16, 96},
		{ 16, 80, 32, 96},
		{ 32, 80, 48, 96},
		{ 48, 80, 64, 96},
		{ 64, 80, 80, 96},
	};
	RECT rcRight[] = {
		{  0, 96, 16,112},
		{ 16, 96, 32,112},
		{ 32, 96, 48,112},
		{ 48, 96, 64,112},
		{ 64, 96, 80,112},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1://目下通過待ち
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}

		//真下に来た…。
		if( npc->x - 8*VS < gMC.x &&
			npc->x + 8*VS > gMC.x &&
			npc->y - 8*VS < gMC.y &&
			npc->y + 6*PARTSSIZE*VS > gMC.y ){

			npc->ani_no   = 0;
			npc->act_no   = 3;
		}
		break;

	case 2://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;

	case 3://様子見（ある程度はなれたら落下）
		npc->ani_no = 0;


		if( npc->shock ||
			npc->x - 20*VS > gMC.x ||
			npc->x + 20*VS < gMC.x ){

			npc->ani_no   = 1;
			npc->ani_wait = 0;
			npc->act_no   = 4;
			npc->act_wait = 0;
		}
		break;
	case 4://落下

		npc->ym += VS/16;// 重力
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		npc->act_wait++;
		if( npc->act_wait < 20 && !(npc->flag & FLAG_HIT_BOTTOM) ) break;

		if( npc->flag & FLAG_HIT_BOTTOM || npc->y > gMC.y -VS*16 ){
			npc->ani_wait = 0;
			npc->ani_no   = 2;
			npc->act_no   = 5;
			npc->tgt_y    = npc->y;
			if( npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS;
		}else{
		}
		break;
	case 5://ぱたぱた。
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )npc->ani_no = 2;

		//移動
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( gMC.x      < npc->x ) npc->xm -= VS/32;
		if( gMC.x      > npc->x ) npc->xm += VS/32;
		if( npc->tgt_y < npc->y ) npc->ym -= VS/32;
		if( npc->tgt_y > npc->y ) npc->ym += VS/32;

		if( npc->xm >  1*VS )npc->xm =  1*VS;
		if( npc->xm < -1*VS )npc->xm = -1*VS;
		if( npc->ym >  1*VS )npc->ym =  1*VS;
		if( npc->ym < -1*VS )npc->ym = -1*VS;

		if( npc->flag & FLAG_HIT_BOTTOM )npc->ym = -VS;
		if( npc->flag & FLAG_HIT_TOP    )npc->ym =  VS;

		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// 最大体力増加
void ActNpc032( NPCHAR *npc )
{
	RECT rect[] = {
		{ 32, 96, 48,112},
		{ 48, 96, 64,112},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1)npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];
}

//バブルホッパー
void ActNpc033( NPCHAR *npc )
{
	//地面に着地
	if( npc->flag & ( FLAG_HIT_LEFT|FLAG_HIT_RIGHT ) ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}else if( npc->flag & FLAG_HIT_BOTTOM ){
		npc->ym = -2*VS;
	}

	npc->ym += VS/12;

	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rect_left[] = {
		{240, 64,256, 80},
		{240, 80,256, 96},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
		if( npc->ani_no > 1)npc->ani_no = 0;
	}
	npc->rect = rect_left[npc->ani_no];

	npc->act_wait++;
	if( npc->act_wait > 5*50 ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}
}

//ベッド
void ActNpc034( NPCHAR *npc )
{

	RECT rcLeft  = {192, 48,224, 64};
	RECT rcRight = {192,184,224,200};


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft;
	else                          npc->rect = rcRight;
}


// マンナン
//void SetExpObjects( long x, long y, long exp );

void ActNpc035( NPCHAR *npc )
{

	if( npc->act_no < 3 && npc->life < 100-10 ){
		PlaySoundObject( WAVE_DESTROY_MIDDLE, 1 );
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );
		SetExpObjects( npc->x, npc->y, npc->exp );
		npc->act_no   = 3;
		npc->act_wait = 0;
		npc->ani_no   = 2;
		npc->bits     &= ~BITS_BANISH_DAMAGE;
		npc->damage   = 0;
	}

	switch( npc->act_no ){
	case 0:
	case 1://起立
		if( npc->shock ){
//			PlaySoundObject( WAVE_SONIC, 1 );
			if( npc->direct == DIR_LEFT )
				SetNpChar( 103, npc->x-8*VS, npc->y+8*VS, 0, 0, npc->direct, NULL, MAX_NPC/2 );
			else
				SetNpChar( 103, npc->x+8*VS, npc->y+8*VS, 0, 0, npc->direct, NULL, MAX_NPC/2 );
			npc->ani_no   = 1;
			npc->act_no   = 2;
			npc->act_wait = 0;
		}
		break;

	case 2://ダメージ
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_wait =  0;
			npc->act_no   =  1;
			npc->ani_no   =  0;
		}
		break;

	case 3://目、ぱちくり
		npc->act_wait++;
		if( npc->act_wait == 50 || npc->act_wait == 60 ) npc->ani_no = 3;
		if( npc->act_wait == 53 || npc->act_wait == 63 ) npc->ani_no = 2;
		if( npc->act_wait > 100 ) npc->act_no = 4;
		break;
	case 4://立ちぼうけ
		break;
	}



	RECT rcLeft[] = {
		{ 96, 64,120, 96},
		{120, 64,144, 96},
		{144, 64,168, 96},
		{168, 64,192, 96},
	};
	RECT rcRight[] = {
		{ 96, 96,120,128},
		{120, 96,144,128},
		{144, 96,168,128},
		{168, 96,192,128},
	};

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}
// バルログ戦闘LV2
void ActNpc036( NPCHAR *npc )
{
	long i;
	unsigned char deg;
	long xm, ym;

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	//見つめる
	case 1:
		npc->act_wait++;
		if( npc->act_wait > 12 ){
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->count1 = 3;
				npc->ani_no = 1;
		}
		break;
	//泡を吐く
	case 2:
		npc->act_wait++;
		if( npc->act_wait > 16 ){
			npc->count1--;
			npc->act_wait = 0;
			deg = GetArktan( npc->x - gMC.x, npc->y+4*VS - gMC.y );
			deg += (unsigned char)Random( -16, 16 );

			ym = GetSin( deg ) * 1;
			xm = GetCos( deg ) * 1;
			SetNpChar( 11, npc->x, npc->y+4*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			if( !npc->count1 ){
				npc->act_no = 3;
				npc->act_wait = 0;
			}
		}
		break;
	//かがむ
	case 3:
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->act_no   = 4;
			npc->act_wait = 0;
			npc->xm = (gMC.x - npc->x)/100;
			npc->ym       = -3*VS;
			npc->ani_no   = 3;
		}
		break;
	//滞空
	case 4:
//		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
//			npc->xm = 0;
//		}
		if( npc->ym > -VS ){
			if( npc->life > 60 ){
				npc->act_no   = 5;
				npc->ani_no   = 4;
				npc->ani_wait = 0;
				npc->act_wait = 0;
				npc->tgt_y    = npc->y;
			}else{
				npc->act_no = 6;
			}
		}

		break;
	//パタパタ
	case 5:

		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 ){
			npc->ani_no = 4;
			PlaySoundObject( WAVE_PATA, 1 );
		}

		npc->act_wait++;
		if( npc->act_wait > 2*50 ){
			npc->act_no = 6;
			npc->ani_no = 3;
		}

		if( npc->y < npc->tgt_y ) npc->ym += VS/8;
		else                      npc->ym -= VS/8;
		if( npc->ym < -VS ) npc->ym = -VS;
		if( npc->ym >  VS ) npc->ym =  VS;
		break;
	//滞空
	case 6:
//		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
//			npc->xm = 0;
//		}
		if( npc->y+16*VS < gMC.y ) npc->damage = 10; //踏み潰し
		else                       npc->damage = 0;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 7;
			npc->act_wait = 0;
			npc->ani_no   = 2;
			PlaySoundObject( WAVE_QUAKE, 1 );
			PlaySoundObject( WAVE_BUNRET, 1 );
			SetQuake( 30 );
			npc->damage   = 0;
			for( i = 0; i < 8; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			for( i = 0; i < 8; i++ ){
				SetNpChar( 33, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
					Random( -VS*2, VS*2 ), Random(-2*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
		break;
	//着地
	case 7:
		npc->xm = 0;
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->act_no = 1;
			npc->act_wait = 0;
		}
		break;
	}
	
	

	//Gravity
	if( npc->act_no != 5 ){
		npc->ym += VS/10;
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
	}

	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{  0,  0, 40, 24},//ノーマル
		{ 40,  0, 80, 24},//吐く
		{ 80,  0,120, 24},//かがむ
		{120,  0,160, 24},//ジャンプ
		{160, 48,200, 72},//パタパタ
		{200, 48,240, 72},
	};
	RECT rect_right[] = {
		{  0, 24, 40, 48},
		{ 40, 24, 80, 48},
		{ 80, 24,120, 48},
		{120, 24,160, 48},
		{160, 72,200, 96},//パタパタ
		{200, 72,240, 96},
	};



	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
}

// 看板
void ActNpc037( NPCHAR *npc )
{
	RECT rect[] = {
		{192, 64,208, 80},
		{208, 64,224, 80},
	};


	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 ) npc->ani_no = 0;
	
	//アニメーション------------------
	npc->rect = rect[npc->ani_no];
}

// 炎
void ActNpc038( NPCHAR *npc )
{
	RECT rect[] = {
		{128, 64,144, 80},
		{144, 64,160, 80},
		{160, 64,176, 80},
		{176, 64,192, 80},
	};


	switch( npc->act_no ){
	case 0:
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 0;
		npc->rect = rect[npc->ani_no];
		break;
	case 10:
		npc->act_no = 11;
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );
	case 11:
		npc->rect.left  = 0;
		npc->rect.right = 0;
		break;
	}

}

// 案内
void ActNpc039( NPCHAR *npc )
{
	RECT rect[] = {
		{224, 64,240, 80},// セーブ
		{240, 64,256, 80},// パブ
	};
	if( npc->direct == DIR_LEFT ) npc->ani_no = 0;
	else                          npc->ani_no = 1;
	npc->rect = rect[npc->ani_no];
}