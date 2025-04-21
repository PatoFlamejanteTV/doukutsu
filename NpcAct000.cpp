#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"

#include "forNpc.h"

//ダミ（落下しない）
void ActNpc000( NPCHAR *npc )
{
	RECT rect = {0,0,16,16};

	if( !npc->act_no ){
		npc->act_no = 1;
		if( npc->direct == DIR_RIGHT ){
			npc->y += 16*VS;
		}
	}
	
	npc->rect = rect;
}

//経験値オブジェクト
void ActNpc001( NPCHAR *npc )
{



	if( gBack.type == BKACT_FLOW || gBack.type == BKACT_CROUD ){

		if( !npc->act_no ){
			npc->act_no = 1;
			npc->ym = Random( -VS/4, VS/4 );
			npc->xm = Random(   127, 256 );
		}
		npc->xm -= VS/64;
		if( npc->x < VS*PARTSSIZE*5     ) npc->cond = 0;
		if( npc->x < -3*VS ) npc->x = -3*VS;
		if( npc->flag & FLAG_HIT_LEFT   ) npc->xm =  VS/2;
		if( npc->flag & FLAG_HIT_TOP    ) npc->ym =  VS/8;
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS/8;

	}else{

	
	
		if( !npc->act_no ){
			npc->act_no = 1;
			npc->ani_no = Random( 0, 4 );
			npc->xm     = Random( -VS, VS );
			npc->ym     = Random( -VS*2, 0 );

			if( Random( 0, 1 ) ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;


		}
	
		//Gravity
		if( npc->flag & FLAG_HIT_WATER ) npc->ym += VS/24;
		else                             npc->ym += VS/12;

		if( npc->flag & FLAG_HIT_LEFT  && npc->xm < 0 ) npc->xm *= -1;
		if( npc->flag & FLAG_HIT_RIGHT && npc->xm > 0 ) npc->xm *= -1;
		if( npc->flag & FLAG_HIT_TOP   && npc->ym < 0 ) npc->ym *= -1;
		//地面に着地
		if( npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_EXPHOP, 1 );
			npc->ym = -(VS + VS/4);
			npc->xm = npc->xm*2/3;
		}
		if( npc->flag & (FLAG_HIT_BOTTOM|FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
			PlaySoundObject( WAVE_EXPHOP, 1 );
			npc->count2++;
			if( npc->count2 > 2 )npc->y -= VS;
		}else{
			npc->count2 = 0;
		}
		if( npc->xm < -MAX_MOVE ) npc->xm = -MAX_MOVE;
		if( npc->xm >  MAX_MOVE ) npc->xm =  MAX_MOVE;
		if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;
		if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	
	}

	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rect[] = {
		{  0, 16, 16, 32},
		{ 16, 16, 32, 32},
		{ 32, 16, 48, 32},
		{ 48, 16, 64, 32},
		{ 64, 16, 80, 32},
		{ 80, 16, 96, 32},
	};
	RECT rcNo = {0,0,0,0};

	npc->ani_wait++;
	if( npc->direct == DIR_LEFT ){
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 5)npc->ani_no = 0;
		}
	}else{
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no--;
			if( npc->ani_no < 0 )npc->ani_no = 5;
		}
	}

	npc->rect = rect[ npc->ani_no];

	if( npc->act_no ){
		switch( npc->exp ){
		case  1:
			break;
		case  5:
			npc->rect.top    += 16;
			npc->rect.bottom += 16;
			break;
		case 20:
			npc->rect.top    += 32;
			npc->rect.bottom += 32;
			break;
		}
		npc->act_no = 1;
	}
	
	//時間で消滅
	npc->count1++;
	if( npc->count1 > 10*50 && npc->ani_no == 5 && npc->ani_wait == 2 ) npc->cond = 0;
	if( npc->count1 >  8*50 && npc->count1/2%2                          ) npc->rect = rcNo;
}


//ベヒモス
void ActNpc002( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 32,  0, 64, 24},
		{  0,  0, 32, 24},
		{ 32,  0, 64, 24},
		{ 64,  0, 96, 24},
		{ 96,  0,128, 24},//構え
		{128,  0,160, 24},//怒り
		{160,  0,192, 24},//
	};
	RECT rcRight[] = {
		{ 32, 24, 64, 48},
		{  0, 24, 32, 48},
		{ 32, 24, 64, 48},
		{ 64, 24, 96, 48},
		{ 96, 24,128, 48},//構え
		{128, 24,160, 48},//怒り
		{160, 24,192, 48},//
	};

	if(      npc->flag & FLAG_HIT_LEFT  ) npc->direct = DIR_RIGHT;
	else if( npc->flag & FLAG_HIT_RIGHT ) npc->direct = DIR_LEFT;


	switch( npc->act_no ){
	case 0://ノーマル
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;
		//アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 8 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 0;
		if( npc->shock ){
			npc->count1 = 0;
			npc->act_no = 1;
			npc->ani_no = 4;
		}
		break;
	case 1://怒り？
		npc->xm = npc->xm*7/8;
		npc->count1++;
		if( npc->count1 > 40 ){
			if( npc->shock ){
				npc->count1   = 0;
				npc->act_no   = 2;
				npc->ani_no   = 6;
				npc->ani_wait = 0;
				npc->damage   = 5;
			}else{
				npc->act_no = 0;
				npc->ani_wait = 0;
			}
		}
		break;
	case 2://怒りの突撃
		if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
		else                          npc->xm =  VS*2;
		npc->count1++;
		if( npc->count1 > 200 ){
			npc->act_no = 0;
			npc->damage = 1;
		}
		//アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 )
			npc->ani_no = 5;
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

//消滅待機オブジェクト
void ActNpc003( NPCHAR *npc )
{

	npc->count1++;
	if( npc->count1 > 100 ){
		npc->cond = 0;
	}
//	npc->x = npc->y;

	//アニメーション------------------
	RECT rect = {0,0,0,0};
	npc->rect = rect;
}

//爆風
void ActNpc004( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 16,  0, 17,  1},
		{ 16,  0, 32, 16},
		{ 32,  0, 48, 16},

		{ 48,  0, 64, 16},
		{ 64,  0, 80, 16},
		{ 80,  0, 96, 16},
		{ 96,  0,112, 16},
		{112,  0,128, 16},
	};

	RECT rcUp[] = {
		{ 16,  0, 17,  1},
		{ 80, 48, 96, 64},
		{  0,128, 16,144},

		{ 16,128, 32,144},
		{ 32,128, 48,144},
		{ 48,128, 64,144},
		{ 64,128, 80,144},
		{ 80,128, 96,144},
	};
	
	unsigned char deg;

	if( npc->act_no == 0 ){
		if( npc->direct == DIR_LEFT || npc->direct == DIR_UP ){
			deg = (unsigned char)Random( 0, 255 );
			npc->xm = GetCos( deg ) * Random(VS,MAX_MOVE)/VS;
			npc->ym = GetSin( deg ) * Random(VS,MAX_MOVE)/VS;
		}
		npc->ani_no   = Random( 0, 4 );
		npc->ani_wait = Random( 0, 3 );
		npc->act_no = 1;
	}else{
		npc->xm = npc->xm*20/21;
		npc->ym = npc->ym*20/21;
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 4 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 7 ){
		npc->cond = 0;
	}else{
		if( npc->direct == DIR_UP    ) npc->rect = rcUp[   npc->ani_no];
		if( npc->direct == DIR_LEFT  ) npc->rect = rcLeft[ npc->ani_no];
		if( npc->direct == DIR_RIGHT ) npc->rect = rcLeft[ npc->ani_no];
	}
}




//クリッター
void ActNpc005( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 48, 16, 64},
		{ 16, 48, 32, 64},
		{ 32, 48, 48, 64},//ジャンプ
	};
	RECT rcRight[] = {
		{  0, 64, 16, 80},
		{ 16, 64, 32, 80},
		{ 32, 64, 48, 80},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->y += 3*VS;
		npc->act_no = 1;
	case 1://待機
		if( npc->x > gMC.x )npc->direct = DIR_LEFT;
		else                npc->direct = DIR_RIGHT;
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
			npc->x - PARTSSIZE*3*VS < gMC.x &&
			npc->x + PARTSSIZE*3*VS > gMC.x &&
			npc->y - PARTSSIZE*5*VS < gMC.y &&
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


// ビートル
void ActNpc006( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 80, 16, 96},//停止
		{ 16, 80, 32, 96},
		{ 32, 80, 48, 96},
		{ 48, 80, 64, 96},
		{ 64, 80, 80, 96},
	};
	RECT rcRight[] = {
		{  0, 96, 16,112},//停止
		{ 16, 96, 32,112},
		{ 32, 96, 48,112},
		{ 48, 96, 64,112},
		{ 64, 96, 80,112},
	};


	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ) npc->act_no = 1;
		else                          npc->act_no = 3;
		break;
	case 1://←移動
		npc->xm -= VS/32;
		if( npc->xm < -VS*2 ) npc->xm = -VS*2;
		if( npc->shock ) npc->x += npc->xm/2;
		else             npc->x += npc->xm;
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 ) npc->ani_no = 1;
		if( npc->flag & FLAG_HIT_LEFT ){
			npc->act_no = 2;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->xm = 0;
			npc->direct = DIR_RIGHT;
		}
		break;
	case 2://→停止
		npc->act_wait++;
		if( npc->act_wait > 60 ){
			npc->act_no = 3;
			npc->ani_wait = 0;
			npc->ani_no = 1;
		}
		break;
	case 3://→移動
		npc->xm += VS/32;
		if( npc->xm > VS*2 ) npc->xm = VS*2;
		if( npc->shock ) npc->x += npc->xm/2;
		else             npc->x += npc->xm;
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 ) npc->ani_no = 1;
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->act_no = 4;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->xm = 0;
			npc->direct = DIR_LEFT;
		}
		break;
	case 4://←停止
		npc->act_wait++;
		if( npc->act_wait > 60 ){
			npc->act_no = 1;
			npc->ani_wait = 0;
			npc->ani_no = 1;
		}
		break;
		
	}


	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];
}


// バジリスク
void ActNpc007( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{256, 64,288, 80},
		{256, 80,288, 96},
		{256, 96,288,112},
	};
	RECT rcRight[] = {
		{288, 64,320, 80},
		{288, 80,320, 96},
		{288, 96,320,112},
	};


	switch( npc->act_no ){
	case 0:
		npc->x = gMC.x;
		if( npc->direct == DIR_LEFT ) npc->act_no = 1;
		else                          npc->act_no = 2;
		break;
	case 1://左向き
		npc->xm -= VS/8;
		if( npc->x < gMC.x - 12*VS*PARTSSIZE )
			npc->act_no = 2;
		if( npc->flag & FLAG_HIT_LEFT ){
			npc->xm = 0;
			npc->act_no = 2;
		}
		break;
	case 2://右向き
		npc->xm += VS/8;
		if( npc->x > gMC.x + 12*VS*PARTSSIZE )
			npc->act_no = 1;
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->xm = 0;
			npc->act_no = 1;
		}
		break;
	}

	if( npc->xm < 0 ) npc->direct = DIR_LEFT;
	else              npc->direct = DIR_RIGHT;

	if( npc->xm >  MAX_MOVE ) npc->xm =  MAX_MOVE;
	if( npc->xm < -MAX_MOVE ) npc->xm = -MAX_MOVE;
	npc->x += npc->xm;

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )npc->ani_no = 0;

	if( npc->direct == DIR_LEFT)npc->rect = rcLeft[npc->ani_no];
	else                        npc->rect = rcRight[npc->ani_no]; 


}

// パゴット
void ActNpc008( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 80, 80, 96, 96},
		{ 96, 80,112, 96},
	};

	RECT rcRight[] = {
		{ 80, 96, 96,112},
		{ 96, 96,112,112},
	};


	switch( npc->act_no ){
	case 0:
		if(	gMC.x < npc->x + 1*PARTSSIZE*VS &&
			gMC.x > npc->x - 1*PARTSSIZE*VS ){

			npc->bits   |= BITS_BANISH_DAMAGE;
			npc->ym     = -VS/2;
			npc->tgt_y  = npc->y;
			npc->act_no = 1;
			npc->damage = 2;
			if( npc->direct == DIR_LEFT ){
				npc->x  = gMC.x + 16*PARTSSIZE*VS;
				npc->xm = -MAX_MOVE/2; 
			}else{
				npc->x  = gMC.x - 16*PARTSSIZE*VS;
				npc->xm = MAX_MOVE/2; 
			}
		}else{
			npc->bits       &= ~BITS_BANISH_DAMAGE;
			npc->rect.right = 0;
			npc->damage     = 0;
			npc->xm         = 0;
			npc->ym         = 0;
			return;
		}
		break;
	case 1://左向き
		if( npc->x > gMC.x ){
			npc->direct = DIR_LEFT;
			npc->xm -= 16;
		}else{
			npc->direct =   DIR_RIGHT;
			npc->xm += 16;
		}
		if( npc->xm >  MAX_MOVE/2 ) npc->xm =  MAX_MOVE/2;
		if( npc->xm < -MAX_MOVE/2 ) npc->xm = -MAX_MOVE/2;

		if( npc->y < npc->tgt_y ) npc->ym += 8;
		else                      npc->ym -= 8;
		if( npc->ym >  VS/2 ) npc->ym =  VS/2;
		if( npc->ym < -VS/2 ) npc->ym = -VS/2;

		if( npc->shock ){
			npc->x += npc->xm/2;
			npc->y += npc->ym/2;
		}else{
			npc->x += npc->xm;
			npc->y += npc->ym;
		}
		break;
	}



	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )npc->ani_no = 0;

	if( npc->direct == DIR_LEFT)npc->rect = rcLeft[npc->ani_no];
	else                        npc->rect = rcRight[npc->ani_no]; 


}

// バルログ 登場
void ActNpc009( NPCHAR *npc )
{
	long i;
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = 2;
	case 1://通常落下
		npc->ym += VS/16;
		if( npc->count1 < 40 ){
			npc->count1++;
		}else{
			npc->bits &= ~BITS_THROW_BLOCK;
			npc->bits |=  BITS_BLOCK_MYCHAR;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
								Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			npc->act_no   = 2;
			npc->ani_no   = 1;
			npc->act_wait = 0;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
		}
		break;
	case 2://着陸
		npc->act_wait++;
		if( npc->act_wait > 16 ){
			npc->act_no   = 3;
			npc->ani_no   = 0;
			npc->ani_wait = 0;
		}
		break;
	case 3:
		break;
	}

	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;
	//アニメーション------------------
	RECT rect_left[] = {
		{  0,  0, 40, 24},//ノーマル
		{ 80,  0,120, 24},//着地
		{120,  0,160, 24},//ジャンプ
	};
	RECT rect_right[] = {
		{  0, 24, 40, 48},//ノーマル
		{ 80, 24,120, 48},//着地
		{120, 24,160, 48},//ジャンプ
	};
	if(npc->direct == DIR_LEFT ) npc->rect = rect_left[ npc->ani_no];
	else                         npc->rect = rect_right[npc->ani_no];
}

// バルログ戦闘
void ActNpc010( NPCHAR *npc )
{
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
			npc->act_no = 4;
			npc->act_wait = 0;
			npc->xm = (gMC.x - npc->x)/100;
			npc->ym = -3*VS;
			npc->ani_no = 3;
		}
		break;
	//滞空
	case 4:
		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
			npc->xm = 0;
		}
		if( npc->y+16*VS < gMC.y ) npc->damage = 5;
		else                 npc->damage = 0;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 5;
			npc->act_wait = 0;
			npc->ani_no = 2;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
			npc->damage = 0;
		}
		break;
	//着地
	case 5:
		npc->xm = 0;
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->act_no = 1;
			npc->act_wait = 0;
		}
		break;


	}
	
	

	//Gravity
	npc->ym += VS/16;

	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{  0,  0, 40, 24},//ノーマル
		{ 40,  0, 80, 24},//吐く
		{ 80,  0,120, 24},//かがむ
		{120,  0,160, 24},//ジャンプ
	};
	RECT rect_right[] = {
		{  0, 24, 40, 48},
		{ 40, 24, 80, 48},
		{ 80, 24,120, 48},
		{120, 24,160, 48},
	};


	if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
	else                 npc->direct = DIR_LEFT;

	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
}


//バブル
void ActNpc011( NPCHAR *npc )
{
	//地面に着地
	if( npc->flag & FLAG_HIT_X ){
		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}

	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rect_left[] = {
		{208,104,224,120},
		{224,104,240,120},
		{240,104,256,120},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
		if( npc->ani_no > 2)npc->ani_no = 0;
	}
	npc->rect = rect_left[npc->ani_no];
	//時間消滅
	npc->count1++;
	if( npc->count1 > 150 ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}
}

// バルログ待機/撤退
void ActNpc012( NPCHAR *npc )
{
	long i;
	long x, y;

	switch( npc->act_no ){
	case 0:// 滞在 ------------
		if( npc->direct == 4 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		npc->act_no = 1;
		npc->ani_no = 0;
	case 1:// 時々瞬き 
		if( Random(0,100) == 0 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 2:
		npc->act_wait++;
		if( npc->act_wait > 16 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;
		
	case 10:// 普通撤退 -------
		if( npc->direct == 4 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		npc->act_no   = 11;
		npc->ani_no   =  2;
		npc->act_wait =  0;
		npc->tgt_x    =  0;
	//かがむ
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 12;
			npc->act_wait = 0;
			npc->ani_no   = 3;
			npc->ym       = -4*VS;
			npc->bits    |= BITS_THROW_BLOCK;
		}
		break;
	//撤退
	case 12:
		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
			npc->xm = 0;
		}
		if( npc->y < 0 ){
			npc->code_char = 0;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
		}
		break;

	case 20:// やられ撤退 ----------
		if( npc->direct == 4 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		npc->act_no   = 21;
		npc->ani_no   =  5;
		npc->act_wait =  0;
		npc->count1   =  0;
		for( i = 0; i < 4; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
				Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
	//ガタガタ
	case 21:
		npc->tgt_x    =  1;// 煙吹きフラグ
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->act_wait++;
		npc->count1++;
		if( npc->count1/2%2 ) npc->x += VS;
		else                  npc->x -= VS;
		if( npc->act_wait > 100 ){
			npc->act_no   = 11;
			npc->act_wait =  0;
			npc->ani_no   =  2;
		}
		// 重力
		npc->ym += VS/16;
		if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
		break;

	case 30: // ニヤri -------
		npc->ani_no = 4;
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_no = 0;
			npc->ani_no = 0;
		}
		break;

	case 40:// ピカピカ -----
		if( npc->direct == 4 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		npc->act_no   = 41;
		npc->act_wait =  0;
		npc->ani_no   =  5;
	case 41:
		npc->ani_wait++;
		if( npc->ani_wait/2%2 ) npc->ani_no = 5;
		else                    npc->ani_no = 6;
		break;

	case 42:// ピカピカ2 -----
		if( npc->direct == 4 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		npc->act_no   = 43;
		npc->act_wait =  0;
		npc->ani_no   =  6;
	case 43:
		npc->ani_wait++;
		if( npc->ani_wait/2%2 ) npc->ani_no = 7;
		else                    npc->ani_no = 6;
		break;

	case 50:// きゃっち
		npc->ani_no = 8;
		npc->xm = 0;
		break;
	case 60:// 走る
		npc->act_no   = 61;
		npc->ani_no   = 9;
		npc->ani_wait = 0;
	case 61:
		//アニメ
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no == 10 || npc->ani_no == 11 ){
				PlaySoundObject( WAVE_STEP, 1 );
			}
		}
		if( npc->ani_no > 12 ) npc->ani_no = 9;

		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;
		break;

	case 70: //フェードアウト
		npc->act_no   = 71;
		npc->act_wait = 64;
		PlaySoundObject( WAVE_WARP, 1 );
		npc->ani_no = 13;
	case 71:
		npc->act_wait--;
		if( !npc->act_wait ){
			npc->cond = 0;
		}
		break;
	case 80:// ガーン
		npc->count1 =  0;
		npc->act_no = 81;
	case 81:
		npc->count1++;
		if( npc->count1/2%2 ) npc->x += VS;
		else                npc->x -= VS;
		npc->ani_no = 5;
		npc->xm = 0;
		// 重力
		npc->ym += VS/16;
		break;
		
	case 100: // 最終退避
		npc->act_no   = 101;
		npc->act_wait =   0;
		npc->ani_no   =   2;
	case 101:
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no   = 102;
			npc->act_wait = 0;
			npc->ani_no   = 3;
			npc->ym       = -4*VS;
			npc->bits    |= BITS_THROW_BLOCK;
			DeleteNpCharCode( 150, FALSE );
			DeleteNpCharCode( 117, FALSE );
			SetNpChar( 355, 0, 0, 0, 0, 0, npc, MAX_NPC/2 );
			SetNpChar( 355, 0, 0, 0, 0, 1, npc, MAX_NPC/2 );
		}
		break;
	case 102:
		x = npc->x/VS/PARTSSIZE;
		y = npc->y/VS/PARTSSIZE;
		if( y >= 0 && y < 35 ){
			if( ChangeMapParts( x,   y, 0 ) ){
				ChangeMapParts( x-1, y, 0 );
				ChangeMapParts( x+1, y, 0 );
				PlaySoundObject( WAVE_BOM, 1 );
				SetQuake2( 10 );
			}
		}
		
		if( npc->y < -32*VS ){
			npc->code_char = 0;
//			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
		}
		break;
	}
	
	if( npc->tgt_x && !Random(0,10) )
		SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
				Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );

	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{  0,  0, 40, 24},//ノーマル
		{160,  0,200, 24},//まばたき
		{ 80,  0,120, 24},//着地
		{120,  0,160, 24},//ジャンプ
		{240,  0,280, 24},//ニヤリ

		{200,  0,240, 24},//5ガーン
		{280,  0,320, 24},//白
		{  0,  0,  0,  0},
		{ 80, 48,120, 72},//8きゃち

		{  0, 48, 40, 72},//9走る
		{  0,  0, 40, 24},
		{ 40, 48, 80, 72},
		{  0,  0, 40, 24},

		{280,  0,320, 24},//13え？！
	};
	RECT rect_right[] = {
		{  0, 24, 40, 48},//ノーマル
		{160, 24,200, 48},//まばたき
		{ 80, 24,120, 48},//着地
		{120, 24,160, 48},//ジャンプ
		{240, 24,280, 48},//ニヤリ

		{200, 24,240, 48},//5ガーン
		{280, 24,320, 48},//白
		{  0,  0,  0,  0},
		{ 80, 72,120, 96},//8きゃち

		{  0, 72, 40, 96},//走る
		{  0, 24, 40, 48},
		{ 40, 72, 80, 96},
		{  0, 24, 40, 48},
		{280, 24,320, 48},//13え？！
	};



	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
	if( npc->act_no == 71 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait/2;
		if( npc->act_wait%2 ) npc->rect.left++;
	}
}

//シールド
void ActNpc013( NPCHAR *npc )
{

	RECT rect[] = {
		{128,  0,144, 16},
		{144,  0,160, 16},
		{160,  0,176, 16},
		{176,  0,192, 16},
	};
	npc->ani_wait++;
	if( npc->ani_wait > 0 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3 ) npc->ani_no = 0;
	//アニメーション------------------
	npc->rect = rect[npc->ani_no];
}


//カギ
void ActNpc014( NPCHAR *npc )
{
	RECT rect[] = {
		{192,  0,208, 16},
		{208,  0,224, 16},
		{224,  0,240, 16},
	};
	long i;

	switch( npc->act_no ){
	case 0://初期配置
		npc->act_no = 1;
		if( npc->direct == DIR_RIGHT ){
			npc->ym = -VS;
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
	case 1://待機
		break;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )npc->ani_no = 0;

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->y += npc->ym;


	npc->rect = rect[npc->ani_no];
}

//宝箱
void ActNpc015( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{240,  0,256, 16},
		{256,  0,272, 16},
		{272,  0,288, 16},
	};
	long i;

	switch( npc->act_no ){
	case 0://初期配置
		npc->act_no = 1;
		npc->bits |= BITS_EVENT_CHECK;
		if( npc->direct == DIR_RIGHT ){
			npc->ym = -VS;
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
	case 1://待機
		npc->ani_no = 0;
		if( !Random( 0,30 ) )npc->act_no = 2;
		break;
	case 2://キラリ
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 ){
			npc->act_no = 1;
		}
		break;
	}


	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->y += npc->ym;


	npc->rect = rcLeft[npc->ani_no];
}

//セーブポイント
void ActNpc016( NPCHAR *npc )
{
	RECT rect[] = {
		{ 96, 16,112, 32},
		{112, 16,128, 32},
		{128, 16,144, 32},
		{144, 16,160, 32},
		{160, 16,176, 32},

		{176, 16,192, 32},
		{192, 16,208, 32},
		{208, 16,224, 32},

	};
	long i;

	switch( npc->act_no ){
	case 0://初期配置
		npc->bits |= BITS_EVENT_CHECK;
		npc->act_no = 1;
		if( npc->direct == DIR_RIGHT ){
			npc->bits &= ~BITS_EVENT_CHECK;
			npc->ym = -VS;
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
	case 1://待機
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->bits |= BITS_EVENT_CHECK;
		break;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 7 )npc->ani_no = 0;

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->y += npc->ym;


	npc->rect = rect[npc->ani_no];
}

//エネルギーポイント
void ActNpc017( NPCHAR *npc )
{
	RECT rect[] = {
		{288,  0,304, 16},
		{304,  0,320, 16},
	};
	long a;

	switch( npc->act_no ){
	case 0://初期配置
		npc->act_no = 1;
		if( npc->direct == DIR_RIGHT ){
			npc->ym = -VS;
			for( a = 0; a < 4; a++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
	case 1:
		a = Random( 0, 30 );
		if( a < 10 )      npc->act_no = 2;
		else if( a < 25 ) npc->act_no = 3;
		else              npc->act_no = 4;
		npc->act_wait = Random( 16,64 );
		npc->ani_wait = 0;
		break;
	case 2://点灯
		npc->rect = rect[0];

		npc->act_wait--;
		if( !npc->act_wait ){
			npc->act_no = 1;
		}
		break;
	case 3://点滅
		npc->ani_wait++;
		if( npc->ani_wait/1%2 ) npc->rect = rect[0];
		else                    npc->rect = rect[1];
		npc->act_wait--;
		if( !npc->act_wait ){
			npc->act_no = 1;
		}
		break;
	case 4://消滅
		npc->rect = rect[1];

		npc->act_wait--;
		if( !npc->act_wait ){
			npc->act_no = 1;
		}
		break;
	}


	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->y += npc->ym;


//	npc->rect = rect[npc->ani_no];
}



//扉（落下しない）
void ActNpc018( NPCHAR *npc )
{

	long i;

	RECT rect[] = {
		{224, 16,240, 40},
		{192,112,208,136},//消え。
	};

	//アニメーション------------------
	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_LEFT ) npc->rect = rect[0];
		else                          npc->rect = rect[1];
		break;

	case 1:
		for( i = 0; i < 4; i++ ){
			SetNpChar( 4, npc->x, npc->y, Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		npc->act_no = 0;
		npc->rect = rect[0];
		break;
	}
}

// バルログ 突き破り待機
void ActNpc019( NPCHAR *npc )
{
	long i;
	switch( npc->act_no ){
	case 0:
		for( i = 0; i < 16; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
							Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		npc->y     += VS*10;
		npc->act_no = 1;
		npc->ani_no = 3;
		npc->ym     = -VS/2;
		PlaySoundObject( WAVE_BREAK1, 1 );
		PlaySoundObject( WAVE_QUAKE,  1 );
		SetQuake( 30 );
	case 1://通常落下
		npc->ym += VS/16/2;
		if( npc->ym > 0 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 2;
			npc->ani_no   = 2;
			npc->act_wait = 0;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 30 );
		}
		break;
	case 2://着陸
		npc->act_wait++;
		if( npc->act_wait > 16 ){
			npc->act_no   = 3;
			npc->ani_no   = 0;
			npc->ani_wait = 0;
		}
		break;
	case 3://瞬き
		if( Random(0,100) == 0 ){
			npc->act_no = 4;
			npc->act_wait = 0;
			npc->ani_no = 1;
		}
		break;
	case 4:
		npc->act_wait++;
		if( npc->act_wait > 16 ){
			npc->act_no = 3;
			npc->ani_no = 0;
		}
		break;
	}

	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{  0,  0, 40, 24},//ノーマル
		{160,  0,200, 24},//まばたき
		{ 80,  0,120, 24},//着地
		{120,  0,160, 24},//ジャンプ
	};
	RECT rect_right[] = {
		{  0, 24, 40, 48},//ノーマル
		{160, 24,200, 48},//まばたき
		{ 80, 24,120, 48},//着地
		{120, 24,160, 48},//ジャンプ
	};

	if(npc->direct == DIR_LEFT ) npc->rect = rect_left[ npc->ani_no];
	else                         npc->rect = rect_right[npc->ani_no];
}
