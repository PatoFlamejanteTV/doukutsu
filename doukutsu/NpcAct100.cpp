#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"

// 通気口
void ActNpc100( NPCHAR *npc )
{

	RECT rc[] = {
		{272, 48, 288, 64},
		{272, 48, 288, 64},
	};
	switch( npc->act_no ){
	case 0:
		npc->y += 16*VS;
		npc->act_no = 1;
	case 1:
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rc[0];
	else                          npc->rect = rc[1];

}



//電源端末
void ActNpc101( NPCHAR *npc )
{

	RECT rect[] = {
		{240,136,256,152},
		{240,136,256,152},
		{256,136,272,152},
	};


	npc->ani_wait++;
	if( npc->ani_wait > 3 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 ) npc->ani_no = 0;
	
	//アニメーション------------------
	npc->rect = rect[npc->ani_no];
}

//電源ビリビリ
void ActNpc102( NPCHAR *npc )
{

	RECT rect[] = {
		{208,120,224,136},
		{224,120,240,136},
		{240,120,256,136},
		{256,120,272,136},
	};

	if( !npc->act_no ){
		npc->act_no = 1;
		npc->y += 8*VS;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 0 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3 ) npc->ani_no = 0;
	
	//アニメーション------------------
	npc->rect = rect[npc->ani_no];
}

//ソニック
void ActNpc103( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{192, 96,208,120},
		{208, 96,224,120},
		{224, 96,240,120},
	};
	RECT rcRight[] = {
		{192,120,208,144},
		{208,120,224,144},
		{224,120,240,144},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:

		if( npc->direct == DIR_LEFT ) npc->xm += -VS/16;
		else                          npc->xm +=  VS/16;

		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;
		break;
	}

	npc->x += npc->xm;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	npc->count1++;
	if( npc->count1 > 50*2 ) npc->cond = 0;
	if( npc->count1%4 == 1 ) PlaySoundObject( WAVE_SONIC, 1 );

}

// カエル
void ActNpc104( NPCHAR *npc )
{
	BOOL bJump;
	RECT rcLeft[] = {
		{  0,112, 32,144},
		{ 32,112, 64,144},
		{ 64,112, 96,144},
	};
	RECT rcRight[] = {
		{  0,144, 32,176},
		{ 32,144, 64,176},
		{ 64,144, 96,176},
	};


	switch( npc->act_no ){
	// 地面 ==========
	case 0:
		npc->act_no   = 1;
		npc->act_wait = 0;
		npc->xm       = 0;
		npc->ym       = 0;
		//落下スタート
		if( npc->direct == 4 ){
			if( Random(0,1) ) npc->direct = DIR_LEFT;
			else              npc->direct = DIR_RIGHT;
			npc->bits |= BITS_THROW_BLOCK;
			npc->ani_no =  2;
			npc->act_no =  3;
			break;
		}else{
			npc->bits &= ~BITS_THROW_BLOCK;
		}
	case 1:
		npc->act_wait++;

		if( Random( 0, 50 ) == 1 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 0;
			npc->ani_wait = 0;
			break;
		}
		

		break;
	case 2:// けろけろ
		npc->act_wait++;
		npc->ani_wait++;

		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ){
			npc->ani_no = 0;
		}
		if( npc->act_wait > 18 ){
			npc->act_no = 1;
			npc->act_no = 1;
		}

		break;
	// 落下スタート ===
	case 3:
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->bits &= ~BITS_THROW_BLOCK;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 0;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}
		break;
	// 空中 ==========
	case 10:
		npc->act_no   = 11;
	case 11:
		if( npc->flag & FLAG_HIT_LEFT  && npc->xm < 0 ){
			npc->xm *= -1;
			npc->direct = DIR_RIGHT;
		}
		if( npc->flag & FLAG_HIT_RIGHT && npc->xm > 0 ){
			npc->xm *= -1;
			npc->direct = DIR_LEFT;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 0;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}
		break;
	}


	bJump = FALSE;
	if( npc->act_no < 10 && npc->act_no != 3 && npc->act_wait > 10 ){		// ジャンプ
		if( npc->shock ) bJump = TRUE;
		if( npc->x < gMC.x -VS*PARTSSIZE* 10 ||
			npc->x > gMC.x +VS*PARTSSIZE* 10 ||
			npc->y < gMC.y -VS*PARTSSIZE*  4 ||
			npc->y > gMC.y +VS*PARTSSIZE*  4 ){
		}else if( Random( 0, 50 ) == 2 ){
			bJump = TRUE;
		}
	}

	if( bJump ){
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
		npc->act_no = 10;
		npc->ani_no = 2;
		npc->ym     = -MAX_MOVE;
		if( !(gMC.cond & COND_HIDE) )PlaySoundObject( WAVE_NPCJUMP1, 1 );
		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;
	}

	npc->ym += VS/4;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}



// お～い
void ActNpc105( NPCHAR *npc )
{

	RECT rect[] = {
		{128, 32,144, 48},
		{128, 32,128, 32},
	};


	npc->act_wait++;
	if( npc->act_wait > 30 ){
		npc->cond = 0;
	}
	if( npc->act_wait < 5 ){
		npc->y -= VS;
	}
	
	//アニメーション------------------
	npc->rect = rect[npc->ani_no];
}

// お～いCall
void ActNpc106( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		SetNpChar( 105, npc->x, npc->y - VS* 8, 0, 0, DIR_LEFT, NULL, MAX_NPC*3/4 );
		npc->act_no = 1;
	case 1:
		break;
	}
}

// マルコ
void ActNpc107( NPCHAR *npc )
{
	long i;
	switch( npc->act_no ){
	// 座り
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_RIGHT )
			npc->ani_no = 5;
	case 1:
		break;
	// 起動 -----------------
	case 10:// 点滅
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->ani_wait =  0;
		for( i = 0; i < 4; i++ ){
			SetNpChar( 4, npc->x, npc->y, Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			PlaySoundObject( WAVE_ON, 1 );
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		npc->act_wait++;
		if( npc->act_wait > 50*2 ){
			npc->act_no = 12;
		}
		break;
	case 12:// 点灯
		npc->act_no   = 13;
		npc->act_wait =  0;
		npc->ani_no   =  1;
	case 13:
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no = 14;
		}
		break;
	case 14://がたがた
		npc->act_no   = 15;
		npc->act_wait =  0;
	case 15:
		if(npc->act_wait/2%2){
			npc->x += VS;
			PlaySoundObject( WAVE_DOOR, 1 );
		}else
			npc->x -= VS;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no = 16;
		}
		break;
	case 16:// 起立
		npc->act_no   = 17;
		npc->act_wait =  0;
		npc->ani_no   =  2;
		PlaySoundObject( WAVE_BREAK1, 1 );
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, npc->x, npc->y, Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
	case 17:
		npc->act_wait++;
		if( npc->act_wait > 50*3 ){
			npc->act_no = 18;
		}
		break;
	case 18:// 屈伸
		npc->act_no   = 19;
		npc->act_wait =  0;
		npc->ani_no   =  3;
		npc->ani_wait =  0;
	case 19:
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 ){
			PlaySoundObject( WAVE_DOOR, 1 );
			npc->ani_no = 3;
		}

		npc->act_wait++;
		if( npc->act_wait > 50*2 ){
			npc->act_no = 20;
			PlaySoundObject( WAVE_BREAK1, 1 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x, npc->y, Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
		}
		break;
	case 20:// ポーズ
		npc->ani_no   =  4;
		break;
	case 21:// クシャ
		npc->act_no = 22;
		npc->ani_no =  5;
		PlaySoundObject( WAVE_NPC_MIDDLE, 1 );
	case 22:
		break;
	case 100:// ダンス
		npc->act_no   = 101;
		npc->ani_no   = 6;
		npc->ani_wait = 0;
	case 101:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no > 9 )
			npc->ani_no = 6;
		break;
	case 110:// 爆破
		SetDestroyNpChar( npc->x, npc->y, 16*VS, 16 );
		npc->cond = 0;
		break;
	}

	RECT rcPoweron[] = {
		{144,  0,160, 24},
		{160,  0,176, 24},
		{176,  0,192, 24},
		{192,  0,208, 24},
		{208,  0,224, 24},
		{224,  0,240, 24},// クシャ

		{176,  0,192, 24},// 6 ダンス
		{192,  0,208, 24},
		{208,  0,224, 24},
		{192,  0,208, 24},


	};

//	if( npc->act_no < 30 )
		npc->rect = rcPoweron[npc->ani_no];
}

//フロッギーバブル
void ActNpc108( NPCHAR *npc )
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
		{ 96, 48,112, 64},
		{112, 48,128, 64},
		{128, 48,144, 64},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2)
		npc->ani_no = 0;

	npc->rect = rect_left[npc->ani_no];

	//時間消滅
	npc->count1++;
	if( npc->count1 > 300 ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}
}

// ひしゃげマルコ
void ActNpc109( NPCHAR *npc )
{
	long i;
	RECT rcLeft[] = {
		{240,  0,256, 24},// 停止
		{256,  0,272, 24},
	};

	RECT rcRight[] = {
		{240, 24,256, 48},// 停止
		{256, 24,272, 48},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_wait--;
		if( npc->act_wait ) 
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

	case 10:// 起立
		npc->act_no   =  0;
		PlaySoundObject( WAVE_BREAK1, 1 );
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, npc->x, npc->y, Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		break;
	}

	npc->ym += VS/8;

	if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;

	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// 小カエル
void ActNpc110( NPCHAR *npc )
{
	BOOL bJump;
	RECT rcLeft[] = {
		{ 96,128,112,144},
		{112,128,128,144},
		{128,128,144,144},
	};
	RECT rcRight[] = {
		{ 96,144,112,160},
		{112,144,128,160},
		{128,144,144,160},
	};


	switch( npc->act_no ){
	// 地面 ==========
	case 0:
		npc->act_no   = 1;
		npc->act_wait = 0;
		npc->xm       = 0;
		npc->ym       = 0;
		//落下スタート
		if( npc->direct == 4 ){
			if( Random(0,1) ) npc->direct = DIR_LEFT;
			else              npc->direct = DIR_RIGHT;
			npc->bits |= BITS_THROW_BLOCK;
			npc->ani_no =  2;
			npc->act_no =  3;
			break;
		}else{
			npc->bits &= ~BITS_THROW_BLOCK;
		}
	case 1:
		npc->act_wait++;

		if( Random( 0, 50 ) == 1 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			npc->ani_no   = 0;
			npc->ani_wait = 0;
			break;
		}
		

		break;
	case 2:// けろけろ
		npc->act_wait++;
		npc->ani_wait++;

		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ){
			npc->ani_no = 0;
		}
		if( npc->act_wait > 18 ){
			npc->act_no = 1;
			npc->act_no = 1;
		}

		break;
	// 落下スタート ===
	case 3:
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->bits &= ~BITS_THROW_BLOCK;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 0;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}
		break;
	// 空中 ==========
	case 10:
		npc->act_no   = 11;
	case 11:
		if( npc->flag & FLAG_HIT_LEFT  && npc->xm < 0 ){
			npc->xm *= -1;
			npc->direct = DIR_RIGHT;
		}
		if( npc->flag & FLAG_HIT_RIGHT && npc->xm > 0 ){
			npc->xm *= -1;
			npc->direct = DIR_LEFT;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 0;
			npc->ani_no   = 0;
			npc->act_wait = 0;
		}
		break;
	}


	bJump = FALSE;
	if( npc->act_no < 10 && npc->act_no != 3 && npc->act_wait > 10 ){		// ジャンプ
		if( npc->shock ) bJump = TRUE;
		if( npc->x < gMC.x -VS*PARTSSIZE* 10 ||
			npc->x > gMC.x +VS*PARTSSIZE* 10 ||
			npc->y < gMC.y -VS*PARTSSIZE*  4 ||
			npc->y > gMC.y +VS*PARTSSIZE*  4 ){
		}else if( Random( 0, 50 ) == 2 ){
			bJump = TRUE;
		}
	}

	if( bJump ){
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
		npc->act_no = 10;
		npc->ani_no = 2;
		npc->ym     = -MAX_MOVE/2;
		PlaySoundObject( WAVE_NPCJUMP2, 1 );
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;
	}

	npc->ym += VS/4;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// 転送マイキャラ
void ActNpc111( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 16, 16},
		{ 16,  0, 32, 16},
	};
	RECT rcRight[] = {
		{  0, 16, 16, 32},
		{ 16, 16, 32, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = 0;
		npc->y -= VS*16;
		break;
	case 1:
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_wait = 0;
			npc->act_no   = 2;
			npc->ani_no   = 1;
			npc->ym       = -MAX_MOVE/2;
		}
		break;
	case 2:// 滞空
		if( npc->ym > 0 ) npc->hit.bottom = 16*VS;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 3;
			npc->act_wait = 0;
			npc->ani_no   = 0;
		}
		break;
	case 3:// 着陸～
		npc->act_wait++;
		if( npc->act_wait > 40 ){
			npc->act_no   = 4;
			npc->act_wait = 64;
			PlaySoundObject( WAVE_WARP, 1 );
		}
		break;
	case 4:// フェードアウト
		npc->act_wait--;
		npc->ani_no   = 0;
		if( !npc->act_wait ){
			npc->cond = 0;
		}
		break;
	}

	npc->ym += VS/8;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	if( gMC.equip & EQUIP_MIMIGAH ){
		npc->rect.top    += 32;
		npc->rect.bottom += 32;
	}

	if( npc->act_no == 4 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait/4;
		if( npc->act_wait/2%2 ) npc->rect.left++;
	}
}

// 転送マイキャラ２
void ActNpc112( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 16, 16},
		{ 16,  0, 32, 16},
	};
	RECT rcRight[] = {
		{  0, 16, 16, 32},
		{ 16, 16, 32, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->x += VS*16;
		npc->y += VS*8;
		PlaySoundObject( WAVE_WARP, 1 );
	case 1:// フェードイン
		npc->act_wait++;
		if( npc->act_wait == 64 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
		}
		break;
	case 2:
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no     = 3;
			npc->ani_no     = 1;
			npc->hit.bottom = 8*VS;
		}
		break;
	case 3:// 滞空
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 4;
			npc->act_wait = 0;
			npc->ani_no   = 0;
		}
		break;
	case 4:// 着陸～
		break;
	}

	npc->ym += VS/8;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	if( gMC.equip & EQUIP_MIMIGAH ){
		npc->rect.top    += 32;
		npc->rect.bottom += 32;
	}

	if( npc->act_no == 1 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait/4;
		if( npc->act_wait/2%2 ) npc->rect.left++;
	}
}

// ブースター博士
void ActNpc113( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{224,  0,240, 16},//停止
		{240,  0,256, 16},

		{256,  0,272, 16},//歩く
		{224,  0,240, 16},
		{272,  0,288, 16},
		{224,  0,240, 16},

		{288,  0,304, 16},//6向こう

	};

	RECT rcRight[] = {

		{224, 16,240, 32},//停止
		{240, 16,256, 32},

		{256, 16,272, 32},//歩く
		{224, 16,240, 32},
		{272, 16,288, 32},
		{224, 16,240, 32},

		{288, 16,304, 32},
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

	case 5:// 向こう
		npc->ani_no = 6;
		break;


	case 30://フェードイン --------------------
		npc->act_no     = 31;
		npc->ani_no     = 0;
		npc->ani_wait   = 0;
		npc->hit.bottom = 16*VS;
		npc->x -= VS*16;
		npc->y += VS*8;
		PlaySoundObject( WAVE_WARP, 1 );
	case 31:// フェードイン
		npc->act_wait++;
		if( npc->act_wait == 64 ){
			npc->act_no   = 32;
			npc->act_wait = 0;
		}
		break;
	case 32:
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no     = 33;
			npc->ani_no     = 1;
			npc->hit.bottom = 8*VS;
		}
		break;
	case 33:// 滞空
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 34;
			npc->act_wait = 0;
			npc->ani_no   = 0;
		}
		break;
	case 34:// 着陸～
		break;
	}

	npc->ym += VS/8;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	if( npc->act_no == 31 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait/4;
		if( npc->act_wait/2%2 ) npc->rect.left++;
	}
}



// プレス
void ActNpc114( NPCHAR *npc )
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
		npc->y       -= 4*VS;
	case 1://待機
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
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->damage = 0;
			npc->bits   |= BITS_BLOCK_MYCHAR2;
		}
		break;
	}

	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->y += npc->ym;

	npc->rect = rcLeft[npc->ani_no];
}

//Ravil-A
void ActNpc115( NPCHAR *npc )
{
	long i;
	RECT rcLeft[] = {
		{  0,120, 24,144},//  待機
		{ 24,120, 48,144},//1 構え
		{ 48,120, 72,144},//2 屈伸
		{ 72,120, 96,144},//3 ジャンプ
		{ 96,120,120,144},//4 アタック
		{120,120,144,144},//5 眠り
	};
	
	RECT rcRight[] = {
		{  0,144, 24,168},//  待機
		{ 24,144, 48,168},//1 構え
		{ 48,144, 72,168},//2 屈伸
		{ 72,144, 96,168},//3 ジャンプ
		{ 96,144,120,168},//4 アタック
		{120,144,144,168},//5 眠り
	};

	switch( npc->act_no ){
	case 0:// 待機
		npc->xm       = 0;
		npc->act_no   = 1;
		npc->act_wait = 0;
		npc->count1   = 0;
	case 1:
		if( gMC.x < npc->x + PARTSSIZE*VS*6 &&
			gMC.x > npc->x - PARTSSIZE*VS*6 &&
			gMC.y < npc->y + PARTSSIZE*VS*2 &&
			gMC.y > npc->y - PARTSSIZE*VS*6 ){
			npc->act_no = 10;
		}
		if( npc->shock ){
			npc->act_no = 10;
		}
		break;


	case 10:
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_no = 1;
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_wait = 0;
			npc->act_no   = 20;
		}
		break;

	// ステップ
	case 20:
		npc->damage = 0;
		npc->xm     = 0;
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}

		if( npc->ani_no > 2 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS;
			else                          npc->xm =  VS;
			npc->count1++;
			if( npc->count1 > 2 ){
				npc->count1   = 0;
				npc->ani_no   = 4; 
				npc->act_no   = 21;
				npc->ym       = -VS*2;
				npc->xm       *= 2;
				npc->damage   = 5;// ダメージ
				PlaySoundObject( WAVE_BAIT, 1 );
			}else{
				npc->act_no   = 21;
				npc->ym       = -VS*2;
				PlaySoundObject( WAVE_NPCJUMP1, 1 );
			}
		}
		break;
	case 21:// 着陸
		if( npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_STEP, 1 );
			npc->act_no   = 20;
			npc->ani_no   =  1;
			npc->ani_wait =  0;
			npc->damage   =  0;
			if( gMC.x > npc->x + PARTSSIZE*VS*9 ||
				gMC.x < npc->x - PARTSSIZE*VS*9 ||
				gMC.y > npc->y + PARTSSIZE*VS*3 ||
				gMC.y < npc->y - PARTSSIZE*VS*9 ){
				npc->act_no = 0;
			}

		}
		break;
	case 30: //出現 ---
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
				Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		npc->ani_no = 0;
		npc->act_no = 0;
		break;
	case 50:// やられ　---
		npc->act_no = 51;
		npc->ani_no = 4;
		npc->damage = 0;
		npc->ym       = -VS*1;
		npc->bits &= ~(BITS_BLOCK_MYCHAR|BITS_BANISH_DAMAGE); 
		PlaySoundObject( WAVE_NPC_MIDDLE, 1 );
	case 51:
		if( npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_STEP, 1 );
			npc->act_no   = 52;
			npc->ani_no   =  5;
			npc->xm       =  0;
		}
		break;
	case 52:
		break;

	}

	if( npc->act_no > 50 ) npc->ym += VS/16;
	else                   npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// 赤い花びら
void ActNpc116( NPCHAR *npc )
{
	RECT rc = {272,184,320,200};
	npc->rect = rc;
}


// カーリー（イベント）
void ActNpc117( NPCHAR *npc )
{
//	long i;
	RECT rcLeft[] = {

		{  0, 96, 16, 112},//停止

		{ 16, 96, 32, 112},
		{  0, 96, 16, 112},
		{ 32, 96, 48, 112},
		{  0, 96, 16, 112},

		{176, 96,192, 112},// しゃがみ 5
		{112, 96,128, 112},// 向こう 6
		{160, 96,176, 112},// ふっとび 7
		{144, 96,160, 112},// 倒れ     8
		{ 48, 96, 64, 112},// 上       9
	};

	RECT rcRight[] = {

		{  0, 112, 16, 128},//停止

		{ 16, 112, 32, 128},
		{  0, 112, 16, 128},
		{ 32, 112, 48, 128},
		{  0, 112, 16, 128},

		{176, 112,192, 128},// しゃがみ 5
		{112, 112,128, 128},// 向こう 6
		{160, 112,176, 128},// ふっとび 7
		{144, 112,160, 128},// 倒れ     8
		{ 48, 112, 64, 128},// 上       9
	};

	switch( npc->act_no ){
	case 0:
		if( npc->direct == 4 ){
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機
		npc->xm  = 0;

		npc->ym += VS/8;
		
		//近づくとふり向く…。
		if( npc->x - PARTSSIZE*2*VS < gMC.x &&
			npc->x + PARTSSIZE*2*VS > gMC.x &&
			npc->y - PARTSSIZE*2*VS < gMC.y &&
			npc->y + PARTSSIZE*1*VS > gMC.y ){

//			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
//			else                 npc->direct = DIR_RIGHT;
		}

		break;


	case 3: //歩行
		npc->act_no   = 4;
		npc->ani_no   = 1;
		npc->ani_wait = 0;
	case 4:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )
			npc->ani_no = 1;

		npc->ym += VS/8;

		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm = +VS;

		break;
	case 5:// しゃがみ
		npc->act_no = 6;
		npc->ani_no = 5;
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );break;
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		npc->xm  = 0;
		break;
	case 6:
		npc->ani_no = 5;
		break;
	case 10:// プレイヤーの目の前まで歩行
		npc->act_no   = 11;
		npc->ani_no   = 1;
		npc->ani_wait = 0;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )
			npc->ani_no = 1;
		if( npc->direct == DIR_LEFT ) npc->x -= VS;
		else                          npc->x += VS;

		if( gMC.x < npc->x + VS*20 &&
			gMC.x > npc->x - VS*20 ){
			npc->act_no = 0;
		}
		break;

	case 20:// 向こう
		npc->xm  = 0;
		npc->ani_no = 6;
		break;
	case 21:// 上向き
		npc->xm  = 0;
		npc->ani_no = 9;
		break;

	case 30: //ふっとび =============
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->ym       = -2*VS;
//		if( npc->direct == DIR_LEFT ) npc->ym =  2*VS;
//		else                          
	case 31:
		npc->ani_no = 7;
		if( npc->direct == DIR_LEFT ) npc->xm =  VS;
		else                          npc->xm = -VS;

		npc->ym += VS/8;
		if( npc->act_wait++ &&
			npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 32;
		}
		break;
	case 32:// 倒れ
		npc->ym += VS/8;
		npc->ani_no = 8;
		npc->xm     = 0;
		break;
		
	case 70: // あとずさり
		npc->act_no   = 71;
		npc->act_wait =  0;
		npc->ani_no   =  1;
		npc->ani_wait =  0;
	case 71:
		if( npc->direct == DIR_LEFT ) npc->x += VS/2;
		else                          npc->x -= VS/2;
		npc->ani_wait++;
		if( npc->ani_wait > 8 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 4 )
			npc->ani_no = 1;
		break;

	}

	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// カーリー（戦闘）
void ActNpc118( NPCHAR *npc )
{
	BOOL bUpper;
	RECT rcLeft[] = {

		{  0, 32, 32, 56}, // 普通
		{ 32, 32, 64, 56}, // 点滅
		{ 64, 32, 96, 56}, // 上向き

		{ 96, 32,128, 56},//走る
		{  0, 32, 32, 56},
		{128, 32,160, 56},
		{  0, 32, 32, 56},

		{  0, 32, 32, 56}, // バリア 7
		{160, 32,192, 56},  
	};

	RECT rcRight[] = {
		{  0, 56, 32, 80}, // 普通
		{ 32, 56, 64, 80}, // 点滅
		{ 64, 56, 96, 80}, // 上向き

		{ 96, 56,128, 80},//走る
		{  0, 56, 32, 80},
		{128, 56,160, 80},
		{  0, 56, 32, 80},

		{  0, 56, 32, 80}, // バリア 7
		{160, 56,192, 80},  
	};

	bUpper = FALSE;

	if( npc->direct == DIR_LEFT &&
		npc->x < gMC.x ){
		bUpper = TRUE;
	}
	if( npc->direct == DIR_RIGHT &&
		npc->x > gMC.x ){
		bUpper = TRUE;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機

		break;

	case 10:// 戦闘
		npc->act_no   = 11;
		npc->act_wait =  Random( 50, 100 );
		npc->ani_no   =  0;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->bits |=  BITS_BANISH_DAMAGE;
		npc->bits &= ~BITS_BLOCK_BULLET;
	case 11:
		if( npc->act_wait ) npc->act_wait--;
		else                npc->act_no   = 13;

		break;

	case 13: //走る
		npc->act_no   = 14;
		npc->ani_no   =  3;
		npc->act_wait = Random( 50, 100 );
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	case 14:

		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 )
			npc->ani_no = 3;
		if( npc->direct == DIR_LEFT ) npc->xm -= VS/8;
		else                          npc->xm += VS/8;

		if( npc->act_wait ) npc->act_wait--;
		else{
			npc->bits |= BITS_BANISH_DAMAGE;
			npc->act_no   = 20;// 発砲へ
			npc->act_wait = 0;
			PlaySoundObject( WAVE_CHARGE, 1 );
		}
		break;


	case 20:// 点滅→発砲
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->xm = npc->xm*8/9;
		npc->ani_no++;
		if( npc->ani_no > 1 ) npc->ani_no = 0;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 21;
			npc->act_wait =  0;
		}
		break;
	case 21:
		npc->act_wait++;
		if( npc->act_wait%4 == 1 ){
			if( npc->direct == DIR_LEFT ){
				if( bUpper ){
					npc->ani_no = 2;
					SetNpChar( 123, npc->x -0*VS, npc->y - 8*VS, 0, 0, DIR_UP,    NULL, MAX_NPC/2 );
				}else{
					npc->ani_no = 0;
					SetNpChar( 123, npc->x -8*VS, npc->y + 4*VS, 0, 0, DIR_LEFT,  NULL, MAX_NPC/2 );
					npc->x += VS;
				}
			}else{
				if( bUpper ){
					npc->ani_no = 2;
					SetNpChar( 123, npc->x -0*VS, npc->y - 8*VS, 0, 0, DIR_UP,    NULL, MAX_NPC/2 );
				}else{
					npc->ani_no = 0;
					SetNpChar( 123, npc->x +8*VS, npc->y + 4*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/2 );
					npc->x -= VS;
				}
			}
		}
		if( npc->act_wait > 30 ){
			npc->act_no = 10;
		}
		break;
	case 30:
		npc->ani_no++;
		if( npc->ani_no > 8 )
			npc->ani_no = 7;
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no = 10;
			npc->ani_no = 0;
		}
		break;

	}


	// ミサイルバリアー
	if( npc->act_no > 10 && npc->act_no < 30 &&
		CountArmsBullet( 6 ) ){
		npc->act_wait = 0;
		npc->act_no   = 30;
		npc->ani_no   = 7;
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits |=  BITS_BLOCK_BULLET;
		npc->xm = 0;

	}


	npc->ym += VS/16;
	if( npc->xm >  MAX_MOVE/3 ) npc->xm =  MAX_MOVE/3;
	if( npc->xm < -MAX_MOVE/3 ) npc->xm = -MAX_MOVE/3;
	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	npc->y += npc->ym;
	npc->x += npc->xm;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// 丸テーブル
void ActNpc119( NPCHAR *npc )
{
	RECT rc = {248,184,272,200};
	npc->rect = rc;
}