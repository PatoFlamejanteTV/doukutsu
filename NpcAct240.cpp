#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"


// 囚ミミガー
void ActNpc240( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{160, 64,176, 80},
		{176, 64,192, 80},

		{192, 64,208, 80},
		{160, 64,176, 80},
		{208, 64,224, 80},
		{160, 64,176, 80},
	};
	RECT rcRight[] = {
		{160, 80,176, 96},
		{176, 80,192, 96},

		{192, 80,208, 96},
		{160, 80,176, 96},
		{208, 80,224, 96},
		{160, 80,176, 96},
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
		npc->ani_no   = 2;
		npc->ani_wait = 0;
		if( Random( 0, 9 )%2 ) npc->direct = DIR_LEFT;
		else                   npc->direct = DIR_RIGHT;
	case 11:
		
		if(      npc->direct == DIR_LEFT  && npc->flag & FLAG_HIT_LEFT  ) npc->direct = DIR_RIGHT;
		else if( npc->direct == DIR_RIGHT && npc->flag & FLAG_HIT_RIGHT ) npc->direct = DIR_LEFT;

		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;

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


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

//クリッター Red
void ActNpc241( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 16, 16},
		{ 16,  0, 32, 16},
		{ 32,  0, 48, 16},//ジャンプ
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
		if( npc->act_wait >= 8 &&
			npc->x - PARTSSIZE*9*VS < gMC.x &&
			npc->x + PARTSSIZE*9*VS > gMC.x &&
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
			npc->x - PARTSSIZE*6*VS < gMC.x &&
			npc->x + PARTSSIZE*6*VS > gMC.x &&
			npc->y - PARTSSIZE*5*VS < gMC.y &&
			npc->y + PARTSSIZE*6*VS > gMC.y ){

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
			if( npc->direct == DIR_LEFT ) npc->xm = -VS/1;
			else                          npc->xm =  VS/1;
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

	npc->ym += VS/6;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];

}

//バット (壁抜け突進)Red
void ActNpc242( NPCHAR *npc )
{

	if( npc->x < 0 || npc->x > gMap.width*PARTSSIZE*VS ){
			VanishNpChar( npc );
			return;
	}

	switch( npc->act_no ){
	case 0://初期化
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
		npc->act_wait = Random( 0, 50 );
	case 1:
		if( npc->act_wait ){
			npc->act_wait--;
			break;
		}
		npc->act_no = 2;
		npc->ym = VS*2;

	case 2://滞空
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;
		
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
	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[ npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
}

// バットRED配置
void ActNpc243( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->act_wait = Random( 0, 500 );
	case 1:
		if( npc->act_wait ){
			npc->act_wait--;
			break;
		}
		npc->act_no = 0;
		SetNpChar( 242, npc->x, npc->y + Random( -32, 32 )*VS, 0, 0, npc->direct, NULL, MAX_NPC/2 );
		break;
	}
}

// 赤い水滴
void ActNpc244( NPCHAR *npc )
{
	RECT rc = { 96,  0, 104, 16};
	BOOL bHit;

	long i;
	npc->ym += VS/8;

	bHit = FALSE;
	if( npc->flag & FLAG_HIT_X ){
		bHit = TRUE;
	}
	if( npc->act_wait > 10 && npc->flag & FLAG_HIT_WATER ){
		bHit = TRUE;
	}
	if( bHit ){
		for( i = 0; i < 3; i++ ) SetCaret( npc->x, npc->y+4*VS, CARET_SPLASH, DIR_RIGHT );

		if( npc->x > gMC.x - PARTSSIZE*VS*16 &&
			npc->x < gMC.x + PARTSSIZE*VS*16 && 
			npc->y > gMC.y - PARTSSIZE*VS*10 && 
			npc->y < gMC.y + PARTSSIZE*VS*10 ) 
			PlaySoundObject( WAVE_SPLASH, 1 );
		npc->cond = 0;
		return;
	}

	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->y += npc->ym;

	npc->rect = rc;

}

// 赤い水滴配置
void ActNpc245( NPCHAR *npc )
{
	RECT rc[] = {
		{0,  0, 0, 0},
		{104,  0, 112, 16},
		{112,  0, 120, 16},
		{120,  0, 128, 16},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x    = npc->x;
		npc->act_wait = npc->code_event;
	case 1:
		npc->ani_no = 0;
		if( npc->act_wait ){
			npc->act_wait--;
			return;
		}
		npc->act_no   = 10;
		npc->ani_wait = 0;
		break;
	case 10:
		npc->ani_wait++;
		if( npc->ani_wait > 10 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 ){
			SetNpChar( 244, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			npc->act_no   = 1;
			npc->act_wait = npc->code_flag;
		}
		break;
	}
	if( npc->ani_wait/2%2 ) npc->x = npc->tgt_x;
	else                    npc->x = npc->tgt_x+VS;

	npc->rect = rc[npc->ani_no];
}

// 空中待機プレス
void ActNpc246( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{144,112,160,136},
		{160,112,176,136},
		{176,112,192,136},
	};

	long i;

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->y -= 4*VS;
	case 1://待機
		if( gMC.x < npc->x + 8 * VS &&
			gMC.x > npc->x - 8 * VS &&
			gMC.y > npc->y + 8 * VS &&
			gMC.y < npc->y + 8 * PARTSSIZE* VS ) npc->act_no = 5;
		break;
	case 5://待機
		if( npc->flag & FLAG_HIT_BOTTOM ){
		}else{
			npc->act_no   = 10;
			npc->ani_wait =  0;
			npc->ani_no   =  1;
		}
		break;
	case 10://落下
		// anime..
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 2;

		// damage
		if( gMC.y > npc->y ){
			npc->bits &= ~BITS_BLOCK_MYCHAR2;
			npc->damage = 127;
		}else{
			npc->bits |= BITS_BLOCK_MYCHAR2;
			npc->damage = 0;
		}

		if( npc->flag & FLAG_HIT_BOTTOM ){
			if( npc->ani_no>1 ){
				for( i = 0; i < 4; i++ )
					SetNpChar( 4, npc->x, npc->y, Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
				PlaySoundObject( WAVE_QUAKE, 1 );
				SetQuake( 10 );
			}
			npc->act_no   = 20;
			npc->ani_no   = 0;
			npc->ani_wait = 0;
			npc->bits    |= BITS_BLOCK_MYCHAR2;
			npc->damage   = 0;
		}
		break;
	case 20:
		break;
	}

	if( npc->act_no >= 5 ){
		npc->ym += VS/4;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		npc->y += npc->ym;
	}

	npc->rect = rcLeft[npc->ani_no];
}

// ミザリー（戦闘）
void ActNpc247( NPCHAR *npc )
{
	unsigned char deg;
	long xm, ym;
	RECT rcLeft[] = {
		{  0,  0, 16, 16},//座り
		{ 16,  0, 32, 16},
		{ 32,  0, 48, 16},//直立
		{ 48,  0, 64, 16},
		{ 64,  0, 80, 16},// 4 呪文
		{ 80,  0, 96, 16},// 5 点滅
		{ 96,  0,112, 16},

		{  0,  0,  0,  0},// 7
		{112,  0,128, 16},// 8非ザマ月
	};
	RECT rcRight[] = {
		{  0, 16, 16, 32},
		{ 16, 16, 32, 32},
		{ 32, 16, 48, 32},
		{ 48, 16, 64, 32},
		{ 64, 16, 80, 32},
		{ 80, 16, 96, 32},
		{ 96, 16,112, 32},

		{  0,  0,  0,  0},// 7
		{112, 16,128, 32},
	};
	
	switch( npc->act_no ){
	case   0:// 腰掛け ------------------------------
		npc->act_no = 1;
//		npc->x += 4*VS;
		npc->y += 6*VS;
		npc->tgt_y = VS*PARTSSIZE*4;
	case   1:
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

	case  20:// 落下着陸 ------------------------------
		npc->xm  = 0;
		npc->ym += VS/8;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 21;
			npc->ani_no =  2;
		}
		break;
	case  21:
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 22;
			npc->act_wait = 0;
			npc->ani_no   = 3;
		}
		break;
	case 22://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no   = 21;
			npc->ani_no   =  2;
		}
		break;

	case 100:// 浮遊（） ------------------------------
		npc->act_no   = 101;
		npc->act_wait =   0;
		npc->ani_no   =   0;
		npc->xm       =   0;
		npc->bits    |= BITS_BANISH_DAMAGE;
		npc->count2   = npc->life;
	case 101:
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->y < npc->tgt_y ) npc->ym += VS/16;
		else                      npc->ym -= VS/16;
		if( npc->ym < -VS*1 ) npc->ym = -VS*1;
		if( npc->ym >  VS*1 ) npc->ym =  VS*1;

		npc->act_wait++;
		if( npc->act_wait > 200 || npc->life <= npc->count2 - 80 ){
			npc->act_wait =   0;

			npc->act_no   = 110;
		//	npc->act_no   = 120;
		}

		break;

	case 110:// ショット ------------------------------
		npc->act_no   = 111;
		npc->act_wait =   0;
		npc->xm       =   0;
		npc->ym       =   0;
		npc->bits    &= ~BITS_BANISH_DAMAGE;
	case 111:// 点滅
		npc->act_wait++;
		if( npc->act_wait % 2 ) npc->ani_no = 5;
		else                    npc->ani_no = 6;
		if( npc->act_wait > 30 ){
			npc->act_wait =   0;
			npc->count1++;
			if( npc->count1%3 == 0 ) npc->act_no   = 113;
			else                     npc->act_no   = 112;
			npc->ani_no   =   4;
		}
		break;
	case 112:// 発射
		npc->act_wait++;
		if( npc->act_wait % 6 == 0 ){

			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -4, 4 );

			ym = GetSin( deg ) * 4;
			xm = GetCos( deg ) * 4;
			SetNpChar( 248, npc->x, npc->y+4*VS, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_FIRE, 1 );
		}
		
		if( npc->act_wait > 30 ){
			npc->act_wait =   0;
			npc->act_no   = 150;
		}
		break;
	case 113:// 落石配置
		npc->act_wait++;
		if( npc->act_wait == 10 ){
			ym = gMC.y - (VS * 64);
			SetNpChar( 279, gMC.x, ym, 0, 0, DIR_UP, NULL, MAX_NPC/2 );
		}
		
		if( npc->act_wait > 30 ){
			npc->act_wait =   0;
			npc->act_no   = 150;
		}
		break;

	case 120:// いなづま ------------------------------
/*		npc->act_no   = 121;
		npc->act_wait =   0;
		npc->xm       =   0;
		npc->ym       =   0;
		npc->bits    &= ~BITS_BANISH_DAMAGE;
	case 121:// 点滅
		npc->act_wait++;
		if( npc->act_wait % 2 ) npc->ani_no = 5;
		else                    npc->ani_no = 6;
		if( npc->act_wait > 30 ){
			npc->act_wait =   0;
			npc->act_no   = 122;
			npc->ani_no   =   4;
		}
		break;
	case 122:// 発射
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_wait =   0;
			npc->act_no   = 150;
		}
*/		break;
		
	case 150:// テレポート ----------------------------
		npc->act_no   = 151;
		npc->act_wait =   0;
		npc->ani_no   =   7;
		SetNpChar( 249, npc->x, npc->y, 0, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
		SetNpChar( 249, npc->x, npc->y, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
		npc->tgt_x = Random( 9, 31 ) *VS*PARTSSIZE;
		npc->tgt_y = Random( 5,  7 ) *VS*PARTSSIZE; 
		PlaySoundObject( WAVE_WARP, 1 );
	
	case 151: // 出現
		npc->act_wait++;
		if( npc->act_wait == 42 ){
			SetNpChar( 249, npc->tgt_x + 2*VS*8, npc->tgt_y, 0, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
			SetNpChar( 249, npc->tgt_x - 2*VS*8, npc->tgt_y, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
		}

		if( npc->act_wait > 50 ){
			npc->act_wait =   0;
			npc->ym       = -VS*1;
			npc->bits    |= BITS_BANISH_DAMAGE;
			npc->x        = npc->tgt_x;
			npc->y        = npc->tgt_y;

			if( npc->life < 340){
				SetNpChar( 252, 0, 0, 0, 0,   0,  npc, MAX_NPC/2 );
				SetNpChar( 252, 0, 0, 0, 0, 128,  npc, MAX_NPC/2 );
			}
			if( npc->life < 180){
				SetNpChar( 252, 0, 0, 0, 0,  64,  npc, MAX_NPC/2 );
				SetNpChar( 252, 0, 0, 0, 0, 192,  npc, MAX_NPC/2 );
			}


			if( gMC.x < npc->x - PARTSSIZE*VS*7 ||
				gMC.x > npc->x + PARTSSIZE*VS*7 ) npc->act_no   = 160;
			else                                  npc->act_no   = 100;


		}
		break;
	case 160: // ブラックボール
		npc->act_no   = 161;
		npc->act_wait =   0;
		npc->ani_no   =   4;
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 161:
		if( npc->y < npc->tgt_y ) npc->ym += VS/16;
		else                      npc->ym -= VS/16;
		if( npc->ym < -VS*1 ) npc->ym = -VS*1;
		if( npc->ym >  VS*1 ) npc->ym =  VS*1;
		npc->act_wait++;
		if( npc->act_wait % 24 == 0 ){
			SetNpChar( 250, npc->x, npc->y+4*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_FIRE, 1 );
		}
		
		if( npc->act_wait > 72 ){
			npc->act_wait =   0;
			npc->act_no   = 100;
		}
		break;

	case 1000://やられ --------------------------------
		npc->bits     &= ~BITS_BANISH_DAMAGE;
		npc->act_no   = 1001;
		npc->act_wait = 0;
		npc->ani_no   =    4;
		npc->tgt_x    = npc->x;
		npc->tgt_y    = npc->y;
		npc->xm       = 0;
		npc->ym       = 0;
		DeleteNpCharCode( 252, TRUE );
		SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
	case 1001:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->x = npc->tgt_x + VS;
		else                  npc->x = npc->tgt_x;
		break;

	case 1010:// 落下
		npc->ym += VS/32;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 1020;
			npc->ani_no =    8;
		}
		break;
	}

	if( npc->xm < -VS*1 ) npc->xm = -VS*1;
	if( npc->xm >  VS*1 ) npc->xm =  VS*1;
	if( npc->ym < -VS*2 ) npc->ym = -VS*2;
	if( npc->ym >  VS*2 ) npc->ym =  VS*2;

	npc->x += npc->xm;
	npc->y += npc->ym;

	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// ブラックバブル
void ActNpc248( NPCHAR *npc )
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
		{  0, 48, 16, 64},
		{ 16, 48, 32, 64},
		{ 32, 48, 48, 64},
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
	if( npc->count1 > 300 ){
		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}
}

// ミザリー残像
void ActNpc249( NPCHAR *npc )
{
	RECT rc[] = {
		{ 48, 48, 64, 64},
		{ 64, 48, 80, 64},
	};

	npc->act_wait++;
	if( npc->act_wait > 8 ){
		npc->cond = 0;
	}

	if( npc->direct == DIR_LEFT ){
		npc->rect = rc[0];
		npc->x   -= VS*2;
	}else{
		npc->rect = rc[1];
		npc->x   += VS*2;
	}
}

// ブラックボール
void ActNpc250( NPCHAR *npc )
{
	RECT rc[] = {
		{  0, 32, 16, 48},
		{ 16, 32, 32, 48},
		{ 32, 32, 48, 48},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_y  = npc->y;
		npc->xm     = 0;
		npc->ym     = -VS;
	case 1: // 漂う
		if( npc->x < gMC.x      ) npc->xm += VS/32;
		else                      npc->xm -= VS/32;
		if( npc->y < npc->tgt_y ) npc->ym += VS/16;
		else                      npc->ym -= VS/16;

		if( npc->xm >  VS ) npc->xm =  VS;
		if( npc->xm < -VS ) npc->xm = -VS;
		if( npc->ym >  VS ) npc->ym =  VS;
		if( npc->ym < -VS ) npc->ym = -VS;

		npc->x += npc->xm;
		npc->y += npc->ym;

		// ani
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		
		if( gMC.x > npc->x - 8*VS &&
			gMC.x < npc->x + 8*VS &&
			gMC.y > npc->y       ){
			
			npc->act_no = 10;
		}
		break;

	case 10:// 点滅
		npc->act_no   = 11;
		npc->act_wait =  0;
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			SetNpChar( 251, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_THUNDER, 1 );
			npc->cond = 0;
			return;
		}
		if( npc->act_wait/2%2 ) npc->ani_no = 2;
		else                    npc->ani_no = 1;
		break;
	}

	npc->rect = rc[npc->ani_no];
}

// ブラックサンダー
void ActNpc251( NPCHAR *npc )
{
	RECT rc[] = {
		{ 80, 32, 96, 64},
		{ 96, 32,112, 64},
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1: // 落下
		npc->ani_no++;
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		npc->y += VS*8;
		// 壁消滅
		if( npc->flag & FLAG_HIT_X ){
			SetDestroyNpChar( npc->x, npc->y, npc->view.back,  3 );
			npc->cond = 0;
		}
		break;
	}

	npc->rect = rc[npc->ani_no];
}

// ミザリーサークル→コウモリ
void ActNpc252( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 48, 32, 64, 48},

		{112, 32,128, 48},
		{128, 32,144, 48},
		{144, 32,160, 48},
	};
	RECT rcRight[] = {
		{ 48, 32, 64, 48},

		{112, 48,128, 64},
		{128, 48,144, 64},
		{144, 48,160, 64},
	};
	unsigned char deg;

	switch( npc->act_no ){
	case 0: // グルグル
		npc->act_no = 1;
		npc->act_wait = 0;
		npc->count1 = npc->direct;
	case 1:
		npc->count1 += 2;
		npc->count1 = npc->count1%256;
		deg         = (unsigned char)npc->count1;
		if( npc->act_wait < 48*4 ) npc->act_wait++;
		npc->x = npc->pNpc->x + GetCos( deg ) * npc->act_wait /4 ;
		npc->y = npc->pNpc->y + GetSin( deg ) * npc->act_wait /4 ;
		
		if( npc->pNpc->act_no == 151 ){
			npc->act_no = 10;
			npc->ani_no =  0;
		}
		break;
		
	case 10: //コウモリ化
		npc->act_no = 11;
		npc->bits   |= BITS_BANISH_DAMAGE;
		npc->bits   &= ~BITS_BLOCK_BULLET;
		npc->bits   &= ~BITS_THROW_BLOCK;
		deg         = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
		deg        += (unsigned char)Random( -3, 3 );
		npc->xm     = GetCos( deg ) * 1;
		npc->ym     = GetSin( deg ) * 1;
		npc->ani_no   = 1;
		npc->ani_wait = 0;

		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 11:
		npc->x += npc->xm;
		npc->y += npc->ym;
		// 壁消滅
		if( npc->flag & FLAG_HIT_X ){
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			SetNpChar( 4, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			npc->cond = 0;
		}
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 1;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// EXPカプセル
void ActNpc253( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->act_no     = 1;
//		npc->exp        = npc->code_event;
//		npc->code_event = 0;
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}
	if( npc->life <= 100 ){
		SetExpObjects( npc->x, npc->y, npc->code_flag );
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );
		PlaySoundObject( WAVE_BUNRET, 1 );
		npc->cond = 0;
	}
	RECT rc[] = {
		{  0, 64, 16, 80},
		{ 16, 64, 32, 80},
	};

	npc->rect = rc[npc->ani_no];
}

// ヘリ
void ActNpc254( NPCHAR *npc )
{
	RECT rc[] = {
		{  0,  0,128, 64},
		{  0, 64,128,128},
	};
	
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		SetNpChar( 255, npc->x + 18*VS, npc->y - 57*VS, 0, 0, DIR_LEFT,  npc, MAX_NPC/2 );
		SetNpChar( 255, npc->x - 32*VS, npc->y - 52*VS, 0, 0, DIR_RIGHT, npc, MAX_NPC/2 );
	case 1:
		break;
	case 20://プロペラを回す
		npc->act_wait=  0;
		npc->count1  = 60;
		npc->act_no  = 21;
	case 21:
		break;
	case 30:// 百鈴
		npc->act_no = 21;
		SetNpChar( 223, npc->x - 11*VS, npc->y- 14*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		break;

	case 40:// いろいろ搭載
		npc->act_no = 21;
		SetNpChar( 223, npc->x -  9*VS, npc->y- 14*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 ); // 百鈴
		SetNpChar(  40, npc->x - 22*VS, npc->y- 14*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 ); // 百鈴
		SetNpChar(  93, npc->x - 35*VS, npc->y- 14*VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 ); // 百鈴

		break;

	}

	if( npc->direct == DIR_LEFT ) npc->rect = rc[0];
	else                          npc->rect = rc[1];
}

// プロペラ
void ActNpc255( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{128,  0,240, 16},
		{128, 16,240, 32},
		{128, 32,240, 48},
		{128, 16,240, 32},
	};

	RECT rcRight[] = {
		{240,  0,320, 16},
		{240, 16,320, 32},
		{240, 32,320, 48},
		{240, 16,320, 32},
	};
	
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ){
			npc->view.front = 56*VS;
			npc->view.back  = 56*VS;
		}else{
			npc->view.front = 40*VS;
			npc->view.back  = 40*VS;
		}
//		npc->act_no = 10;
	case 1:
		if( npc->pNpc->act_no >= 20 ){
			npc->act_no = 10;
		}
		break;
	case 10:
		npc->act_no = 11;
	case 11:
		npc->ani_no++;
		if( npc->ani_no > 3 )
			npc->ani_no = 0;
		break;
	}

	if( npc->direct == DIR_LEFT ){
		npc->x = npc->pNpc->x + 18*VS;
		npc->y = npc->pNpc->y - 57*VS;
	}else{
		npc->x = npc->pNpc->x - 32*VS;
		npc->y = npc->pNpc->y - 52*VS;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// ドクター（背中）
void ActNpc256( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 48,160, 72,192},//0 背中
		{ 72,160, 96,192},
		{  0,128, 24,160},//2 静止
		{ 24,128, 48,160},//3 笑う
		{  0,160, 24,192},//4 キラーン
		{ 24,160, 48,192},//5 キラーン
	};

	switch( npc->act_no ){
	case 0:
		gSuperXpos = 0;
		npc->act_no = 1;
		npc->y -= VS*8;
	case 1:
		npc->ani_no = 0;
		break;
	case 10: //背中笑い
		npc->act_no   = 11;
		npc->ani_wait =  0;
		npc->ani_no   =  0;
		npc->count1   =  0;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ){
			npc->ani_no = 0;
			npc->count1++;
		}
		if( npc->count1 > 5 ){
			npc->act_no = 1;
		}
		break;
	case 20: // こっち向き
		npc->act_no = 21;
	case 21:
		npc->ani_no =  2;
		break;
	case 40: // キラーン
		npc->act_no = 41;
		SetNpChar( 257, npc->x - 14*VS, npc->y -16*VS, 0, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
		SetNpChar( 257, npc->x - 14*VS, npc->y -16*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/3 );
	case 41:
		npc->ani_no =  4;
		break;
	case 50: //笑い
		npc->act_no   = 51;
		npc->ani_wait =  0;
		npc->ani_no   =  4;
		npc->count1   =  0;
	case 51:
		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 ){
			npc->ani_no = 4;
			npc->count1++;
		}
		if( npc->count1 > 5 ){
			npc->act_no = 41;
		}
		break;
	}

	npc->rect = rcLeft[ npc->ani_no];
	
}

// 赤い結晶
void ActNpc257( NPCHAR *npc )
{
	RECT rc[] = {
		{176, 32,184, 48},
		{184, 32,192, 48},
		{  0,  0,  0,  0},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;

	case 1:
		if( gSuperXpos ){
			npc->act_no = 10;
		}
		break;

	case 10:
		if( npc->x < gSuperXpos ) npc->xm += VS/6;
		if( npc->x > gSuperXpos ) npc->xm -= VS/6;
		if( npc->y < gSuperYpos ) npc->ym += VS/6;
		if( npc->y > gSuperYpos ) npc->ym -= VS/6;

		if( npc->xm >  VS*2 ) npc->xm =  VS*2;
		if( npc->xm < -VS*2 ) npc->xm = -VS*2;
		if( npc->ym >  VS*2 ) npc->ym =  VS*2;
		if( npc->ym < -VS*2 ) npc->ym = -VS*2;

		npc->x += npc->xm;
		npc->y += npc->ym;
		break;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 3 ){
		npc->ani_wait =  0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 ){
		npc->ani_no = 0;
	}
	if( npc->direct == DIR_LEFT  && npc->xm > 0 ) npc->ani_no = 2;
	if( npc->direct == DIR_RIGHT && npc->xm < 0 ) npc->ani_no = 2;
	npc->rect = rc[npc->ani_no];
}

// ベッドミミガー（存在意義低い…）
void ActNpc258( NPCHAR *npc )
{
	RECT rc = { 48,32,64,48};
	npc->rect = rc;
}


// カーリー(おんぶ)
void ActNpc259( NPCHAR *npc )
{
	RECT rcLeft  = {224, 96,240,112};
	RECT rcRight = {224,112,240,128};

	switch( npc->act_no ){
	case 0:
		npc->bits &= ~BITS_EVENT_CHECK;
		npc->act_no   = 1;
	case 1:// 待機 ---
		if( gMC.direct == DIR_LEFT ) npc->direct = DIR_LEFT;
		else                         npc->direct = DIR_RIGHT;

		npc->y = gMC.y -VS*4;

		if( npc->direct == DIR_LEFT ){
			npc->x = gMC.x + 3*VS;
			npc->rect = rcLeft;
		}else{
			npc->x = gMC.x - 3*VS;
			npc->rect = rcRight;
		}
		if( gMC.ani_no%2 )npc->rect.top += 1;
		break;

	case 10:
		npc->act_no = 11;
		npc->xm =  VS/ 8;
		npc->ym = -VS/16;
		npc->rect = rcLeft;
		break;
	case 11:
		if( npc->y < VS*4*PARTSSIZE ) npc->ym = VS/16;
		npc->x += npc->xm;
		npc->y += npc->ym;
		break;
	case 20: // 消滅
		VanishNpChar( npc );
		SetDestroyNpCharUp( npc->x, npc->y, 16*VS, 64 );
		break;
	}


}