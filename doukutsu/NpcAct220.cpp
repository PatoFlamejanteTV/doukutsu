#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"



// ミミガー=スコップ(停止)
void ActNpc220( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 64, 16, 80},
		{ 16, 64, 32, 80},
	};
	RECT rcRight[] = {
		{  0, 80, 16, 96},
		{ 16, 80, 32, 96},

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
	}
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ミミガー=スコップ(うろうろ)
void ActNpc221( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 64, 16, 80},
		{ 16, 64, 32, 80},

		{ 32, 64, 48, 80},
		{  0, 64, 16, 80},
		{ 48, 64, 64, 80},
		{  0, 64, 16, 80},
	};
	RECT rcRight[] = {
		{  0, 80, 16, 96},
		{ 16, 80, 32, 96},

		{ 32, 80, 48, 96},
		{  0, 80, 16, 96},
		{ 48, 80, 64, 96},
		{  0, 80, 16, 96},
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


// 檻16x32
void ActNpc222( NPCHAR *npc )
{
	RECT rc = {96,168,112,200};
	if( !npc->act_no ){
		npc->act_no++;
		npc->y -= 8*VS;
	}

	npc->rect = rc;
}

// 坂本 百鈴
void ActNpc223( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 80,192, 96,216},
		{ 96,192,112,216},
		{112,192,128,216},// びっくり
	};
	RECT rcRight[] = {
		{ 80,216, 96,240},
		{ 96,216,112,240},
		{112,216,128,240},
	};

	switch( npc->act_no ){
	case 0:
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
	case 3://びっくり
		npc->ani_no = 2;
		break;
	}

	if( npc->act_no < 2 ){
		if( gMC.y < npc->y + VS*16 && gMC.y > npc->y - VS*16 ){
			if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
	}
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// チエ
void ActNpc224( NPCHAR *npc )
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

	if( npc->act_no < 2 ){
		if( gMC.y < npc->y + VS*16 && gMC.y > npc->y - VS*16 ){
			if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
	}
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// メガネ・爺
void ActNpc225( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{64, 64,80, 80},
		{80, 64,96, 80},
	};
	RECT rcRight[] = {
		{64, 80,80, 96},
		{80, 80,96, 96},
	};

	switch( npc->act_no ){
	case 0:
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

	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// 釣竿ミミガー（イベント）
void ActNpc226( NPCHAR *npc )
{
	RECT rcRight[] = {
		{256, 56,272, 80},
		{272, 56,288, 80},

		{288, 56,304, 80},
		{256, 56,272, 80},
		{304, 56,320, 80},
		{256, 56,272, 80},

		{240, 56,256, 80},// 6
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
		npc->ani_no   = 2;
		npc->ani_wait = 0;
	case 11:
		
		npc->xm =  VS;
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;

		npc->act_wait++;
		break;

	case 20:// 向こう
		npc->xm     = 0;
		npc->ani_no = 6;
		break;
	}
	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;


	npc->rect = rcRight[npc->ani_no];

}

// バケツ
void ActNpc227( NPCHAR *npc )
{
	RECT rc = {208, 32,224, 48};

	npc->rect = rc;
}

// ドロール（イベント）
void ActNpc228( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  0, 32, 40},
		{ 32,  0, 64, 40},

		{ 64,  0, 96, 40},
		{ 96,  0,128, 40},
	};
	RECT rcRight[] = {
		{  0, 40, 32, 80},
		{ 32, 40, 64, 80},

		{ 64, 40, 96, 80},
		{ 96, 40,128, 80},
	};

	switch( npc->act_no ){
	case 0:// 待機
		npc->act_no = 1;
		npc->y -= 8*VS;
	case 1:
		npc->xm = 0;
		npc->act_no = 2;
		npc->ani_no = 0;
	case 2:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_wait++;
		if( npc->ani_wait > 50 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;

	case 10: //ジャンプ
		npc->act_no = 11;
		npc->ani_no =  2;
		npc->act_wait = 0;
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 12;
			npc->ani_no = 3;
			npc->ym = -VS*3;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS;
			else                          npc->xm =  VS;
		}
		break;

	case 12:// 空中
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_no   =  2;
			npc->act_no   = 13;
			npc->act_wait =  0;
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

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// 赤い花の芽
void ActNpc229( NPCHAR *npc )
{
	RECT rc[] = {
		{  0, 96, 48,112},
		{  0,112, 48,128},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y -= VS*16;
	case 1:
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rc[0];
	else                          npc->rect = rc[1];
}

// 赤い花の芽
void ActNpc230( NPCHAR *npc )
{
	RECT rc[] = {
		{ 48, 96, 96,128},
		{ 96, 96,144,128},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->x -= VS*16;
		npc->y -= VS*16;
	case 1:
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rc[0];
	else                          npc->rect = rc[1];
}

// ロケット
void ActNpc231( NPCHAR *npc )
{
	long i;
	RECT rc[] = {
		{176, 32, 208, 48},
		{176, 48, 208, 64},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
//		npc->x -= VS*8;
	case 1:
//		npc->damage = 0;
		npc->ani_no = 0;
		break;

	case 10:// 地面を確認
		npc->act_no   = 11;
		npc->act_wait =  0;
	case 11:
		npc->act_wait++;
		npc->ym += VS/64;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			if( npc->act_wait < 10 ) npc->act_no = 12;
			else                     npc->act_no =  1;
		}
		break;

	case 12:// 発射

		npc->bits &= ~BITS_EVENT_CHECK;

		npc->act_no   = 13;
		npc->act_wait =  0;
		npc->ani_no   =  1;
		for( i = 0; i < 10; i++ ){
			SetNpChar( 4,
				npc->x + Random( -16, 16 ) * VS,
				npc->y + Random(  -8,  8 ) * VS,
				0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_BREAK1, 1 );
		}

	case 13:
		
		npc->ym -= VS/64;
		npc->act_wait++;
		if( npc->act_wait%2 == 0 ) SetCaret( npc->x - 10*VS, npc->y+8*VS, CARET_MISSILE, DIR_DOWN );
		if( npc->act_wait%2 == 1 ) SetCaret( npc->x + 10*VS, npc->y+8*VS, CARET_MISSILE, DIR_DOWN );

		if( npc->act_wait%4 == 1 ) PlaySoundObject( WAVE_FIRE, 1 );

		if( npc->flag & FLAG_HIT_TOP ||
			gMC.flag  & FLAG_HIT_TOP || npc->act_wait > 450 ){

			if( npc->flag & FLAG_HIT_TOP ||
			gMC.flag  & FLAG_HIT_TOP ) npc->ym = 0;

			npc->act_no = 15;
			for( i = 0; i < 6; i++ ){
				SetNpChar( 4,
					npc->x + Random( -16, 16 ) * VS,
					npc->y + Random(  -8,  8 ) * VS,
					0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
				PlaySoundObject( WAVE_BREAK1
					, 1 );
			}

		}

		break;

	case 15:// 落下
		npc->ym += VS/64;
		npc->act_wait++;
		if( npc->ym < 0 ){
			if( npc->act_wait% 8 == 0 ) SetCaret( npc->x - 10*VS, npc->y+8*VS, CARET_MISSILE, DIR_DOWN );
			if( npc->act_wait% 8 == 4 ) SetCaret( npc->x + 10*VS, npc->y+8*VS, CARET_MISSILE, DIR_DOWN );
			if( npc->act_wait%16 == 1 ) PlaySoundObject( WAVE_FIRE, 1 );
		}

//		if( gMC.flag & FLAG_HIT_BOTTOM && gMC.y > npc->y + npc->hit.bottom ) npc->damage = 127;
//		else                                                                 npc->damage =   0;


		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->bits |= BITS_EVENT_CHECK;
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;
	}

	if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;
	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	npc->y += npc->ym;

//	if( npc->ym > 2*VS ) npc->bits &= ~BITS_BLOCK_MYCHAR2;
//	else                 npc->bits |=  BITS_BLOCK_MYCHAR2;

	npc->rect = rc[npc->ani_no];
}

// オレンジベル
void ActNpc232( NPCHAR *npc )
{
	long i;

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
		npc->ym     = VS;
		for( i = 0; i < 8; i++ ){
			SetNpChar( 233, npc->x, npc->y, 0, 0, npc->direct, npc, MAX_NPC/2 ); 
		}
	case 1:
	
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ) npc->direct = DIR_RIGHT;
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ) npc->direct = DIR_LEFT;
	
		// move
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;
		if( npc->y < npc->tgt_y ) npc->ym += VS/64;
		else                      npc->ym -= VS/64;
		if( npc->ym >  VS ) npc->ym =  VS;
		if( npc->ym < -VS ) npc->ym = -VS;

		// ani
		npc->ani_wait++;
		if( npc->ani_wait > 5 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;
		break;
	}
	npc->y += npc->ym;
	npc->x += npc->xm;

	RECT rcLeft[] = {
		{128,  0,160, 32},
		{160,  0,192, 32},
		{192,  0,224, 32},
	};
	RECT rcRight[] = {
		{128, 32,160, 64},
		{160, 32,192, 64},
		{192, 32,224, 64},
	};
	
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

//バット with オレンジベル
void ActNpc233( NPCHAR *npc )
{
	unsigned char deg;
	switch( npc->act_no ){
	case 0://初期化

		npc->act_no = 1;
		deg = (unsigned char)Random( 0, 255 );
		npc->xm = GetCos( deg ) * 1;
		deg = (unsigned char)Random( 0, 255 );
		npc->ym = GetSin( deg ) * 1;
		npc->count1 = 120;
		npc->count2 = Random( -32, 32 ) * VS;

	case 1://滞空
		if( npc->pNpc->code_char == 232 ){
			npc->tgt_x = npc->pNpc->x;
			npc->tgt_y = npc->pNpc->y;
			npc->direct = npc->pNpc->direct;
		}
		//振り向き
		if( npc->tgt_x              < npc->x ) npc->xm -= VS/64;
		if( npc->tgt_x              > npc->x ) npc->xm += VS/64;
		if( npc->tgt_y+ npc->count2 < npc->y ) npc->ym -= VS/16;
		if( npc->tgt_y+ npc->count2 > npc->y ) npc->ym += VS/16;

		if( npc->xm >  2*VS )npc->xm =  2*VS;
		if( npc->xm < -2*VS )npc->xm = -2*VS;
		if( npc->ym >  2*VS )npc->ym =  2*VS;
		if( npc->ym < -2*VS )npc->ym = -2*VS;


		if( npc->count1 < 120 ){
			npc->count1++;
			break;
		}
		if( npc->x-8*VS < gMC.x && npc->x+8*VS            > gMC.x &&
			npc->y      < gMC.y && npc->y+11*PARTSSIZE*VS > gMC.y ){
			npc->xm /= 4;
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
		{256,  0,272, 16},
		{272,  0,288, 16},
		{288,  0,304, 16},
		{304,  0,320, 16},
	};
	RECT rect_right[] = {
		{256, 16,272, 32},
		{272, 16,288, 32},
		{288, 16,304, 32},
		{304, 16,320, 32},
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
	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
}


// 刈られた赤い花
void ActNpc234( NPCHAR *npc )
{
	RECT rc[] = {
		{144, 96,192,112},
		{144,112,192,128},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y += VS*16;
	case 1:
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rc[0];
	else                          npc->rect = rc[1];
}

// ミドリン
void ActNpc235( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{192, 96,208,112},
		{208, 96,224,112},

		{224, 96,240,112},
		{192, 96,208,112},
	};
	RECT rcRight[] = {
		{192,112,208,128},
		{208,112,224,128},

		{224,112,240,128},
		{192,112,208,128},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0;
	case 1://待機
		if( Random( 0, 30 ) == 1 ){
			npc->act_no   =  2;
			npc->act_wait =  0;
			npc->ani_no   =  1;
		}
		if( Random( 0, 30 ) == 1 ){
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

		if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
		else                          npc->xm =  VS*2;
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;

		npc->act_wait++;
		if( npc->act_wait > 64 ){
			npc->act_no = 0;
		}
		break;
	}
	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;
	if( npc->ani_no == 2 )npc->hit.top = 5*VS;
	else                  npc->hit.top = 4*VS;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ガンフィッシュ
void ActNpc236( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{128, 64,152, 88},
		{152, 64,176, 88},
		{176, 64,200, 88},
		{200, 64,224, 88},
		{224, 64,248, 88},
		{248, 64,272, 88},
	};
	RECT rcRight[] = {
		{128, 88,152,112},
		{152, 88,176,112},
		{176, 88,200,112},
		{200, 88,224,112},
		{224, 88,248,112},
		{248, 88,272,112},
	};


	switch( npc->act_no ){
	case 0:

		npc->act_no = 1;
		npc->act_wait = Random( 0, 50 );
		npc->tgt_x = npc->x;
		npc->tgt_y = npc->y;
		npc->ym    = 0;
	case 1:
		if( npc->act_wait ) npc->act_wait--;
		else{
			npc->ym = VS;
			npc->act_no = 2;
		}
		break;

	case 2:// ぷかぷか
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;

		if( gMC.x < npc->x +  8*VS*PARTSSIZE &&
			gMC.x > npc->x -  8*VS*PARTSSIZE &&
			gMC.y < npc->y +  2*VS*PARTSSIZE &&
			gMC.y > npc->y - 10*VS*PARTSSIZE ){
			npc->act_wait++;
		}

		if( npc->act_wait > 80 ){
			npc->act_no   = 10;
			npc->act_wait =  0;
		}


		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		break;

	case 10: // ため

		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_wait = 0;
			npc->act_no   = 20;
		}

		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;

		break;

	case 20: // 発射

		npc->act_wait++;
		if( npc->act_wait > 60 ){
			npc->act_wait = 0;
			npc->act_no   = 2;
		}

		if( npc->act_wait%10 == 3 ){
			PlaySoundObject( WAVE_POP, 1 );
			if( npc->direct == DIR_LEFT ) SetNpChar( 237, npc->x - 8*VS, npc->y - 8*VS, -2*VS, -2*VS, DIR_LEFT, 0, MAX_NPC/2 );
			else                          SetNpChar( 237, npc->x + 8*VS, npc->y - 8*VS,  2*VS, -2*VS, DIR_LEFT, 0, MAX_NPC/2 );
		}

		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 4;
		break;
	}
	if( npc->y < npc->tgt_y ) npc->ym += VS/32;
	else                      npc->ym -= VS/32;
	if( npc->ym >  VS/2 ) npc->ym =  VS/2;
	if( npc->ym < -VS/2 ) npc->ym = -VS/2;

	npc->y += npc->ym;
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// ガンウォーター
void ActNpc237( NPCHAR *npc )
{
	RECT rc = {312, 32,320, 40};
	
	int i;
	BOOL bHit;

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;

	case 1:
		bHit = FALSE;
		npc->act_wait++;

		if( npc->flag & FLAG_HIT_X ){
			bHit = TRUE;
		}
		if( npc->act_wait > 10 && npc->flag & FLAG_HIT_WATER ){
			bHit = TRUE;
		}
		if( bHit ){
			for( i = 0; i < 5; i++ )
				SetCaret( npc->x, npc->y, CARET_SPLASH, 0 );
			PlaySoundObject( WAVE_SPLASH, 1 );
			npc->cond = 0;
			return;
		}
		break;
	}

	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;
	npc->rect = rc;

}

// プレスＨ
void ActNpc238( NPCHAR *npc )
{
	long i;
	RECT rc[] = {
		{184,200,208,216},
		{208,200,232,216},
		{232,200,256,216},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no     = 1;
		npc->tgt_x      = npc->x;
		npc->tgt_y      = npc->y;
		npc->view.front = 16*VS;
		npc->view.back  =  8*VS;
	case 1:
		if( npc->direct == DIR_LEFT  &&
			gMC.x < npc->x           &&
			gMC.x > npc->x -12*16*VS &&
			gMC.y > npc->y - 4*VS    &&
			gMC.y < npc->y + 8*VS    ){

			npc->act_no   = 10;
			npc->act_wait =  0;
			npc->ani_no   =  2;
		}
		if( npc->direct == DIR_RIGHT &&
			gMC.x > npc->x           &&
			gMC.x < npc->x +12*16*VS &&
			gMC.y > npc->y - 4*VS    &&
			gMC.y < npc->y + 8*VS    ){

			npc->act_no   = 10;
			npc->act_wait =  0;
			npc->ani_no   =  2;
		}
		break;

	case 10:
		npc->damage = 127;
		if( npc->direct == DIR_LEFT ) npc->x -= 6*VS;
		else                          npc->x += 6*VS;
		npc->act_wait++;
		if( npc->act_wait == 8 ){
			npc->act_no   = 20;
			npc->act_wait =  0;
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4,
					npc->x + Random( -16, 16 ) * VS,
					npc->y + Random(  -8,  8 ) * VS,
					0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
				PlaySoundObject( WAVE_BREAK1, 1 );
			}

		}

		break;

	case 20:
		npc->damage = 0;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_wait =  0;
			npc->act_no   = 30;
		}
		break;

	case 30:
		npc->damage = 0;
		npc->ani_no = 1;
		npc->act_wait++;
		if( npc->act_wait == 12 ){
			npc->act_no   =  1;
			npc->act_wait =  0;
			npc->ani_no = 0;
		}
		if( npc->direct == DIR_LEFT ) npc->x += 4*VS;
		else                          npc->x -= 4*VS;
		break;
	}

	if(      npc->direct == DIR_LEFT  && gMC.x < npc->x ) npc->hit.back =  16*VS;
	else if( npc->direct == DIR_RIGHT && gMC.x > npc->x ) npc->hit.back =  16*VS;
	else                                                  npc->hit.back   = 8*VS;

	npc->rect = rc[npc->ani_no];
}

// 檻大 //left:jail2 / right:ring2
void ActNpc239( NPCHAR *npc )
{
	RECT rcLeft  = {192, 48,256, 80};
	RECT rcRight = { 96,112,144,144};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ){
			npc->x +=  8*VS;
			npc->y += 16*VS;
		}else{
			npc->view.front = 24*VS;
			npc->view.back  = 24*VS;
			npc->view.top   =  8*VS;
			npc->view.back  = 24*VS;
		}
	case 1:
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft;
	else                          npc->rect = rcRight;
}