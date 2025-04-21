#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"


//ダークマスター
void ActNpc160( NPCHAR *npc )
{
	long i;
	switch( npc->act_no ){
	case 0://初期化
				npc->bits &= ~BITS_BLOCK_MYCHAR;
		npc->act_no = 1;
	case 1://登場落下
			if( npc->x < gMC.x )npc->direct = DIR_RIGHT;
			else                npc->direct = DIR_LEFT;
		npc->ym = VS*5;
		if( npc->y < VS*PARTSSIZE*8 ){
			npc->count1++;
		}else{
			npc->bits &= ~BITS_THROW_BLOCK;
//			npc->bits |=  BITS_BLOCK_MYCHAR;
			npc->act_no = 2;
		}
		break;
	case 2://通常落下
		npc->ym = VS*5;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			DeleteNpCharCode( 161, TRUE );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
								Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			npc->act_no = 3;
			npc->act_wait = 0;
			SetQuake( 30 );
			PlaySoundObject( WAVE_QUAKE, 1 );
			PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		}
		if( npc->y < gMC.y && gMC.flag &FLAG_HIT_BOTTOM ) npc->damage = 20;
		else                                              npc->damage =  0;

		break;
	case 3://ズシン
		npc->damage = 20;
		npc->damage =  0;
		npc->act_wait++;
		if( npc->act_wait > 24 ){
			npc->act_no = 4;
			npc->count1 = 0;
			npc->count2 = 0;
		}
		break;
	case 4://待機
		gSuperXpos = npc->x;
		gSuperYpos = npc->y;
		if( npc->shock%2 == 1 ){
			SetNpChar( 161, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
				Random(-3*VS,3*VS), Random(-3*VS,3*VS), DIR_LEFT, NULL, MAX_NPC/2 );
			npc->count1++;
			if( npc->count1 > 30 ){
				npc->count1 = 0;
				npc->act_no = 5;
				npc->ym = -VS*6;
//				npc->bits &= ~BITS_BLOCK_MYCHAR;
				npc->bits |= BITS_THROW_BLOCK;
			}
		}

		break;
	case 5:// ジャンプ

		gSuperXpos = npc->x;
		gSuperYpos = npc->y;
		npc->count1++;
		if( npc->count1 > 60 ){
			npc->count1 = 0;
			npc->act_no = 6;
		}
		break;
	case 6://くろい雨
		gSuperXpos = gMC.x;
		gSuperYpos = VS*16*400;
		npc->count1++;
		if( npc->count1 > 110 ){
			npc->count1 = 10;
			npc->x = gMC.x;
			npc->y = 0*VS;
			npc->ym = MAX_MOVE;
			npc->act_no = 1;
		}
		break;
		
	}

//	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->y += npc->ym;



	switch( npc->act_no ){
	case 0:
	case 1:npc->ani_no = 3;break;
	case 2:npc->ani_no = 3;break;
	case 3:npc->ani_no = 2;break;
	case 4:npc->ani_no = 0;break;
	case 5:npc->ani_no = 3;break;
	case 6:npc->ani_no = 3;break;
	}

	RECT rect_left[] = {
		{  0,  0,  40, 24},
		{ 40,  0,  80, 24},
		{ 80,  0, 120, 24},
		{120,  0, 160, 24},
	};
	RECT rect_right[] = {
		{  0,  24,  40, 48},
		{ 40,  24,  80, 48},
		{ 80,  24, 120, 48},
		{120,  24, 160, 48},
	};
	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[npc->ani_no];
	else                         npc->rect = rect_right[npc->ani_no];
}


//ダークバブル
void ActNpc161( NPCHAR *npc )
{
	npc->exp = 0;
	if( npc->x < gSuperXpos ) npc->xm += VS/8;
	else                      npc->xm -= VS/8;
	if( npc->y < gSuperYpos ) npc->ym += VS/8;
	else                      npc->ym -= VS/8;

	if( npc->xm < -MAX_MOVE*3 )npc->xm = -MAX_MOVE*3;
	if( npc->xm >  MAX_MOVE*3 )npc->xm =  MAX_MOVE*3;
	if( npc->ym < -MAX_MOVE*3 )npc->ym = -MAX_MOVE*3;
	if( npc->ym >  MAX_MOVE*3 )npc->ym =  MAX_MOVE*3;

	if( npc->life < 100 ){
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits &= ~BITS_BLOCK_BULLET;
		npc->damage = 0;
		npc->ani_no = 2;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;
	if( npc->ani_no < 2 ){
		if( Random( 0, 10 ) == 2 )npc->ani_no = 0;
		else                      npc->ani_no = 1;
	}

	RECT rect[] = {
		{  0, 48, 16, 64},
		{ 16, 48, 32, 64},
		{ 32, 48, 48, 64},
	};
	npc->rect = rect[npc->ani_no];
}

//ブラックプーエンド
void ActNpc162( NPCHAR *npc )
{
	long i;
	RECT rect_left  = { 40,  0,  80, 24};
	RECT rect_right = { 40, 24,  80, 48};
	RECT rect_end = {0, 0, 0, 0};
	switch( npc->act_no ){
	case 0:
		DeleteNpCharCode( 161, TRUE );
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		for( i = 0; i < 10; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
				Random(-VS*3,VS*3), Random(-3*VS,VS*3), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		if( npc->x > gMC.x )npc->direct = DIR_LEFT;
		else                npc->direct = DIR_RIGHT;
		if( npc->direct == DIR_LEFT )npc->rect = rect_left;
		else                         npc->rect = rect_right;
		npc->count1 = 0;
		npc->act_no = 1;
	case 1://ブルブル
		npc->count1++;
		if( !(npc->count1%4) ) SetNpChar( 161, npc->x + Random( -12, 12 ) * VS,
								               npc->y + Random( -12, 12 ) * VS,
															0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		if( npc->count1 > 160 ){
			npc->count1 = 0;
			npc->act_no = 2;
			npc->tgt_y = npc->y;
		}
		break;
	case 2:
		SetQuake( 2 );
		npc->count1++;
		if( npc->count1 <= 24*10 ){
			if( npc->direct == DIR_LEFT )npc->rect = rect_left;
			else                         npc->rect = rect_right;
			npc->rect.top = npc->rect.top +  npc->count1/8;
			npc->y        = npc->tgt_y    + (npc->count1/8) * VS;
			npc->rect.left -= npc->count1/2%2;
		}else{
			npc->rect = rect_end;
			npc->count1 = 0;
			npc->act_no = 3;
		}
		if( npc->count1%3 == 2)
			SetNpChar( 161, npc->x + Random( -12, 12 ) * VS, npc->y-12*VS,
									 Random( -VS, VS ), VS/2, DIR_LEFT, NULL, MAX_NPC/2 );
		if( npc->count1%4 == 2) PlaySoundObject( WAVE_SPLASH, 1 );
		break;
	case 3:
		npc->count1++;
		if( npc->count1 < 60 ){
		}else{
			DeleteNpCharCode( 161, TRUE );
			npc->cond = 0;
		}
		break;
	}
	gSuperXpos = npc->x;
	gSuperYpos = -1000*VS;
}







//ドクター毛炉
void ActNpc163( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{192,  0,208, 16}, // 普通
		{208,  0,224, 16}, // 瞬き
	};
	RECT rcRight[] = {
		{192, 16,208, 32}, // 普通
		{208, 16,224, 32}, // 瞬き
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

//ナース斜河
void ActNpc164( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{224,  0,240, 16}, // 普通
		{240,  0,256, 16}, // 瞬き
	};
	RECT rcRight[] = {
		{224, 16,240, 32}, // 普通
		{240, 16,256, 32}, // 瞬き
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

//ベッドカーリー
void ActNpc165( NPCHAR *npc )
{
	RECT rcRight[] = {
		{192, 96,208,112}, // 普通
		{208, 96,224,112}, // 瞬き
	};

	RECT rcLeft = {144, 96,160,112};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->y += VS*10;
	case 1://待機
		if( npc->direct == DIR_RIGHT  &&
			gMC.x > npc->x - VS * 32 &&
			gMC.x < npc->x + VS * 32 &&
			gMC.y > npc->y - VS * 16 &&
			gMC.y < npc->y + VS * 16 ){

			npc->ani_no = 1;
		}else
			npc->ani_no = 0;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft;
	else                          npc->rect = rcRight[npc->ani_no];
}


// ガウディレジ
void ActNpc166( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{144,104,184,128}, // 普通
		{184,104,224,128}, // 瞬き
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

	npc->rect = rcLeft[ npc->ani_no];
}

// ブースター（座り／落下）
void ActNpc167( NPCHAR *npc )
{
	long i;
	RECT rect[] = {
		{304,  0,320, 16},//落下
		{304, 16,320, 32},//座り
		{0,0,0,0},
	};


	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = 1;
	case 1:
		break;
	case 10://落下
		npc->ani_no = 0;
		npc->ym += VS/8;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		npc->y += npc->ym;
		break;
	case 20:// 消滅 ====
		npc->act_no   = 21;
		npc->act_wait =  0;
		npc->ani_no   =  0;
		PlaySoundObject( WAVE_WARP, 1 );
	case 21:
		npc->ani_no++;
		if( npc->ani_no > 2 )
			npc->ani_no = 1;
		npc->act_wait++;
		if( npc->act_wait > 100 ){
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
							Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			npc->cond = 0;
		}
		break;
	}

	npc->rect = rect[npc->ani_no];
}


// 大石
void ActNpc168( NPCHAR *npc )
{
	RECT rect = {264,56,320,96};



	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		break;

	case 10://がたがた
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->tgt_x    = npc->x;
	case 11:
		npc->act_wait++;
		npc->x = npc->tgt_x;
		if( (npc->act_wait/3)%2 ) npc->x += VS;
		break;

	case 20:// ぽ～い
		npc->act_no = 21;
		npc->act_wait = 0;
		npc->ym  = -2*VS;
		npc->xm  =  VS/2;
		PlaySoundObject( WAVE_BUNRET, 1 );
	case 21:
		npc->ym += VS/32;
		npc->x += npc->xm;
		npc->y += npc->ym;
		if( npc->act_wait && npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_EXPLOSION, 1 );
			SetQuake( 40 );
			npc->act_no = 0;
		}
		if( !npc->act_wait )npc->act_wait++;
		break;
	}

	npc->rect = rect;
}



// バルログ フル装備
void ActNpc169( NPCHAR *npc )
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

		if( npc->direct == DIR_LEFT ) npc->xm -= VS/16;
		else                          npc->xm += VS/16;


		if( npc->act_wait >= 8 &&
			npc->x - 12*VS < gMC.x &&
			npc->x + 12*VS > gMC.x &&
			npc->y - 12*VS < gMC.y &&
			npc->y +  8*VS > gMC.y ){

			npc->act_no = 10;
			npc->ani_no = 5;
			gMC.cond   |= COND_HIDE;//hide!!
			DamageMyChar( 5 );
			break;
		}

		// タイムアウト
		npc->act_wait++;
		if( npc->act_wait > 25*3 ){
			npc->act_no = 9;
			npc->ani_no = 0;
			break;
		}
		
		// 壁に当たる
		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
			if( npc->count2 < 5 ) npc->count2++;
			else{

				npc->act_no = 4;
				npc->act_wait = 0;
				npc->ani_no = 7;
				npc->ym     = -MAX_MOVE;
			}
		}else npc->count2 = 0;

		// 時にはジャンプ
		if( !(npc->count1%2) && npc->act_wait > 25*1 ){
			npc->act_no = 4;
			npc->act_wait = 0;
			npc->ani_no = 7;
			npc->ym     = -MAX_MOVE;

			break;
		}

		break;

	case 4:// ジャンプ
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;


		npc->act_wait++;
		if( npc->act_wait < 30 && 1 == npc->act_wait%6 ){
			PlaySoundObject( WAVE_POP, 1 );
			SetNpChar( 170, npc->x, npc->y, 0, 0, npc->direct, NULL, MAX_NPC/2 );
		}

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 9;
			npc->ani_no = 8;
			SetQuake( 30 );
			PlaySoundObject( WAVE_QUAKE, 1 );
		}
		// 空中キャッチ
		if( npc->act_wait >= 8 &&
			npc->x - 12*VS < gMC.x &&
			npc->x + 12*VS > gMC.x &&
			npc->y - 12*VS < gMC.y &&
			npc->y +  8*VS > gMC.y ){

			npc->act_no = 10;
			npc->ani_no = 5;
			gMC.cond   |= COND_HIDE;//hide!!
			DamageMyChar( 10 );
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

	if( npc->xm < -VS*3/2    )npc->xm = -VS*3/2;
	if( npc->xm >  VS*3/2    )npc->xm =  VS*3/2;
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


	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[ npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
}

// ミサイル赤
void ActNpc170( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{112, 96,128,104},
		{128, 96,144,104},
	};
	RECT rcRight[] = {
		{112,104,128,112},
		{128,104,144,112},
	};

	BOOL bHit;

	bHit = FALSE;

	if( npc->direct == DIR_LEFT  && npc->flag & FLAG_HIT_LEFT   ) bHit = TRUE;
	if( npc->direct == DIR_RIGHT && npc->flag & FLAG_HIT_RIGHT  ) bHit = TRUE;
	if( bHit ){
		PlaySoundObject( WAVE_BOM, 1 );
		SetDestroyNpChar( npc->x, npc->y, 0, 3 );
		VanishNpChar( npc );
		return;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ) npc->xm = Random(  1,  2 )*VS;
		else                          npc->xm = Random( -2, -1 )*VS;
		npc->ym = Random( -2, 0 )*VS;
	case 1:


		npc->count1++;
		if( npc->direct == DIR_LEFT ){
			npc->xm -= VS/16;
			if( npc->count1%3 == 1 ) SetCaret( npc->x+8*VS, npc->y, CARET_MISSILE, DIR_RIGHT );
		}else{
			npc->xm += VS/16;
			if( npc->count1%3 == 1 ) SetCaret( npc->x-8*VS, npc->y, CARET_MISSILE, DIR_LEFT  );
		}

		if( npc->count1 < 50 ){
			if( npc->y < gMC.y ) npc->ym += VS/16;
			else                 npc->ym -= VS/16;
		}else{
			npc->ym = 0;
		}

		npc->ani_no++;
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}

	if( npc->xm < -VS*2 ) npc->xm = -VS*3;
	if( npc->xm >  VS*2 ) npc->xm =  VS*3;

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}




// 魔旋風
void ActNpc171( NPCHAR *npc )
{

	RECT rcLeft[] = {
		{120, 48,152, 80},
		{152, 48,184, 80},
	};
	RECT rcRight[] = {
		{184, 48,216, 80},
		{216, 48,248, 80},
	};


	if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
	else                 npc->direct = DIR_RIGHT;

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->act_wait = Random( 0, 50 );
		npc->tgt_y    = npc->y;
	case 1:
		if( npc->act_wait )npc->act_wait--;
		else{
			npc->act_no = 10;
			npc->ym     = VS;
		}
	case 10:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		if( npc->y < npc->tgt_y ) npc->ym += VS/32;
		else                      npc->ym -= VS/32;
		if( npc->ym >  VS ) npc->ym =  VS;
		if( npc->ym < -VS ) npc->ym = -VS;
		npc->y += npc->ym;

		if( npc->direct == DIR_LEFT ){
			if( gMC.y < npc->y + VS*PARTSSIZE* 5 &&
				gMC.y > npc->y - VS*PARTSSIZE* 5 &&
				gMC.x < npc->x                   &&
				gMC.x > npc->x - VS*PARTSSIZE*10 ){
				npc->count1++;
			}
		}else{
			if( gMC.y < npc->y + VS*PARTSSIZE* 5 &&
				gMC.y > npc->y - VS*PARTSSIZE* 5 &&
				gMC.x < npc->x + VS*PARTSSIZE*10 &&
				gMC.x > npc->x                   ){
				npc->count1++;
			}
		}

		if( npc->count1 > 20 ){
			SetNpChar( 172, npc->x, npc->y, 0, 0, npc->direct, NULL, MAX_NPC/2 );
			npc->count1 = -100;
			gCurlyShoot_wait = Random( 80, 100 );
			gCurlyShoot_x    = npc->x;
			gCurlyShoot_y    = npc->y;
		}
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// ファイアーリング
void ActNpc172( NPCHAR *npc )
{
	RECT rect[] = {
		{248, 48,264, 80},
		{264, 48,280, 80},
		{280, 48,296, 80},
	};


	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;
		if( npc->direct == DIR_LEFT ) npc->x -= VS;
		else                          npc->x += VS;

		if( npc->flag & FLAG_HIT_LEFT || npc->flag & FLAG_HIT_RIGHT ){
			SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
			VanishNpChar( npc );
			return;
		}


		break;
	}

	npc->rect = rect[npc->ani_no];

}

// ガウディ (アーマー)
void ActNpc173( NPCHAR *npc )
{
	unsigned char deg;
	long xm, ym;
	RECT rcLeft[] = {
		{  0,128, 24, 152},
		{ 24,128, 48, 152},// かがみ
		{ 48,128, 72, 152},// ジャンプ
		{ 72,128, 96, 152},// 発射
	};

	RECT rcRight[] = {
		{  0,152, 24, 176},
		{ 24,152, 48, 176},// かがみ
		{ 48,152, 72, 176},// ジャンプ
		{ 72,152, 96, 176},// 発射
	};

	if( npc->x > gMC.x + (16*VS*20) ||
		npc->x < gMC.x - (16*VS*20) ||
		npc->y > gMC.y + (16*VS*15) ||
		npc->y < gMC.y - (16*VS*15) ){

		return;
	}
	


	switch( npc->act_no ){
	case 0://初期化
		npc->tgt_x  = npc->x;
		npc->act_no = 1;
		
	case 1:// 待機
		npc->ani_no = 0;
		npc->xm     = 0;
		if( npc->act_wait < 5 ) npc->act_wait++;
		else if(
			npc->x - VS*PARTSSIZE*12 < gMC.x &&
			npc->x + VS*PARTSSIZE*12 > gMC.x &&
			npc->y - VS*PARTSSIZE*10 < gMC.y &&
			npc->y + VS*PARTSSIZE*10 > gMC.y ){

			npc->act_no   = 10;
			npc->act_wait =  0;
			npc->ani_no   =  1;
		}
		break;

	case 10: // かがみ
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->count1++;
			if( npc->count1 == 3 ){
				PlaySoundObject( WAVE_NPCJUMP1, 1 );
				npc->count1 = 0;
				npc->act_no   = 25;
				npc->act_wait =  0;
				npc->ani_no   =  2;
				npc->ym       = -VS*3;
				if( npc->x < npc->tgt_x ) npc->xm =  VS/4;
				else                      npc->xm = -VS/4;
			}else{
				PlaySoundObject( WAVE_NPCJUMP1, 1 );
				npc->act_no   = 20;
				npc->ani_no   =  2;
				npc->ym       = -VS*1;
				if( npc->x < npc->tgt_x ) npc->xm =  VS/1;
				else                      npc->xm = -VS/1;
			}
		}
		break;

	case 20: // ジャンプ
		npc->act_wait++;

		if( npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_STEP, 1 );
			npc->ani_no   =  1;
			npc->act_no   = 30;
			npc->act_wait =  0;
		}
		break;
	case 25: // ジャンプ
		npc->act_wait++;
		if( npc->act_wait == 30 || npc->act_wait == 40 ){
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -6, 6 );

			ym = GetSin( deg ) * 3;
			xm = GetCos( deg ) * 3;
			SetNpChar( 174, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			npc->ani_no   =  3;

			gCurlyShoot_wait = Random( 80, 100 );
			gCurlyShoot_x    = npc->x;
			gCurlyShoot_y    = npc->y;
		}

		if( npc->act_wait == 35 || npc->act_wait == 45){
			npc->ani_no   = 2;
		}

		if( npc->flag & FLAG_HIT_BOTTOM ){
			PlaySoundObject( WAVE_STEP, 1 );
			npc->ani_no   =  1;
			npc->act_no   = 30;
			npc->act_wait =  0;
		}
		break;

	case 30:
		npc->xm = npc->xm*7/8;
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->ani_no   = 0;
			npc->act_no   = 1;
			npc->act_wait = 0;
		}
		break;
	}

	npc->ym += VS/10;


	//振り向き
	if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
	else                 npc->direct = DIR_RIGHT;

	if( npc->ym >  MAX_MOVE )npc->ym = MAX_MOVE;
	if( npc->ym < -MAX_MOVE )npc->ym = MAX_MOVE;
	
	npc->x += npc->xm;
	npc->y += npc->ym;




	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	// →やられ
	if( npc->life <= 1000 - 15 ){
		SetDestroyNpChar( npc->x, npc->y, 0, 2 );
		npc->code_char = 154;
		npc->act_no    =   0;
	}

}



//ガウディカッター
void ActNpc174( NPCHAR *npc )
{
	BOOL bHit;

	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_RIGHT ){
			npc->act_no = 2;
		}

	case 1:// 横直進
		npc->x += npc->xm;
		npc->y += npc->ym;
		bHit = FALSE;
		if( npc->flag & FLAG_HIT_LEFT ){
			bHit = TRUE;
			npc->xm     =  VS;
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			bHit = TRUE;
			npc->xm     =  -VS;
		}
		if( npc->flag & FLAG_HIT_TOP ){
			bHit = TRUE;
			npc->ym     =  VS;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			bHit = TRUE;
			npc->ym     =  -VS;
		}

		if( bHit ){
			npc->act_no =   2;
			npc->count1++;
			PlaySoundObject( WAVE_NODAMAGE, 1 );
		}
		break;
	case 2:// 落下
		npc->ym += VS/8;
		npc->x += npc->xm;
		npc->y += npc->ym;
		//消滅
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->count1++;
			if( npc->count1 > 1 ){
				SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
				npc->cond = 0;
			}
		}
		break;

	}

	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;

	//アニメーション------------------
	RECT rect_left[] = {
		{120, 80,136, 96},
		{136, 80,152, 96},
		{152, 80,168, 96},
	};

		npc->ani_no++;
	if( npc->ani_no > 2)
		npc->ani_no = 0;

	npc->rect = rect_left[npc->ani_no];

}


// ガウディエッグ
void ActNpc175( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{168, 80,192,104},
		{192, 80,216,104},
	};
	RECT rcRight[] = {
		{216, 80,240,104},
		{240, 80,264,104},
	};

	if( npc->act_no < 3 && npc->life < 100-10 ){
		LoseNpChar( npc, FALSE );
		npc->act_no   = 10;
		npc->ani_no   = 1;
		npc->bits     &= ~BITS_BANISH_DAMAGE;
		npc->damage   = 0;
	}

	switch( npc->act_no ){
	case 0:
		npc->ani_no = 0;
		npc->act_no = 1;
	case 1:
		break;
	case 10:
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->ym += VS/16;
	else                          npc->ym -= VS/16;

	if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;
	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;

	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// コケ壷
void ActNpc176( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 96,128,128,144},
		{128,128,160,144},
		{160,128,192,144},
	};
	RECT rcRight[] = {
		{ 96,144,128,160},
		{128,144,160,160},
		{160,144,192,160},
	};

	if( npc->act_no < 3 && npc->life < 1000-60 ){
		LoseNpChar( npc, FALSE );
		npc->act_no   = 10;
		npc->ani_no   = 2;
		npc->bits     &= ~BITS_BANISH_DAMAGE;
		npc->damage   = 0;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1: // 待機
		if( npc->direct == DIR_LEFT ){
			if( npc->x < gMC.x + VS*PARTSSIZE*10 &&
				npc->x > gMC.x - VS*PARTSSIZE*10 &&
				npc->y < gMC.y + VS*PARTSSIZE*10 &&
				npc->y > gMC.y - VS*PARTSSIZE* 1 ){
				npc->count1++;
			}
		}else{
			if( npc->x < gMC.x + VS*PARTSSIZE*10 &&
				npc->x > gMC.x - VS*PARTSSIZE*10 &&
				npc->y < gMC.y + VS*PARTSSIZE* 1 &&
				npc->y > gMC.y - VS*PARTSSIZE*10 ){
				npc->count1++;
			}
		}

		if( npc->count1 > 10 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
		}
		break;

	case 2: // 吐く
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->count2++;
			if( npc->count2 > 2 ){
				npc->count2 =   0;
				npc->count1 = -90;
			}else
				npc->count1 = -10;


			if( npc->direct == DIR_LEFT ) SetNpChar( 177, npc->x, npc->y - 8*VS, 0, 0, DIR_LEFT,  0, MAX_NPC/2 );
			else                          SetNpChar( 177, npc->x, npc->y + 8*VS, 0, 0, DIR_RIGHT, 0, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			npc->act_no =   0;
			npc->ani_no =   0;

			gCurlyShoot_wait = Random( 80, 100 );
			gCurlyShoot_x    = npc->x;
			gCurlyShoot_y    = npc->y;
		}
		break;

	case 10:
		break;
	}


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// コケ
void ActNpc177( NPCHAR *npc )
{
	RECT rc[] = {
		{192,128,208,144},
		{208,128,224,144},
	};


	if( npc->flag & FLAG_HIT_X ){
		SetCaret( npc->x, npc->y, CARET_FLASH, DIR_LEFT );
		npc->cond = 0;
		return;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ) npc->ym = -VS*3;
		else                          npc->ym =  VS*3;
	case 1:
		if( npc->y < gMC.y + VS*16 && npc->y > gMC.y - VS*16 ){
			npc->act_no = 10;
			npc->tgt_x  = npc->x;
			npc->tgt_y  = npc->y;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
			npc->xm = Random(0,1)*VS*2-VS;
			npc->ym = Random(0,1)*VS*2-VS;
		}
		break;

	case 10:
		if( npc->x < npc->tgt_x ) npc->xm += VS/16;
		else                      npc->xm -= VS/16;
		if( npc->y < npc->tgt_y ) npc->ym += VS/16;
		else                      npc->ym -= VS/16;

		npc->act_wait++;
		if( npc->act_wait > 6*50 ){
			SetCaret( npc->x, npc->y, CARET_FLASH, DIR_LEFT );
			npc->cond = 0;
			return;
		}

		if( npc->direct == DIR_LEFT ) npc->tgt_x -= VS/1;
		else                          npc->tgt_x += VS/1;

		break;
	}


	if( npc->xm >  VS*2 ) npc->xm =  VS*2;
	if( npc->xm < -VS*2 ) npc->xm = -VS*2;
	if( npc->ym >  VS*2 ) npc->ym =  VS*2;
	if( npc->ym < -VS*2 ) npc->ym = -VS*2;
	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->ani_wait++;
	if( npc->ani_wait > 6 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )
		npc->ani_no = 0;

	npc->rect = rc[ npc->ani_no];

}

//アーモンドソニック
void ActNpc178( NPCHAR *npc )
{
	//地面に着地
	if( npc->flag & FLAG_HIT_X ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}

	if( npc->flag & FLAG_HIT_WATER ){

		npc->y += npc->ym/2;
		npc->x += npc->xm/2;
	}else{
		npc->y += npc->ym;
		npc->x += npc->xm;
	}
	//アニメーション------------------
	RECT rect_left[] = {
		{  0, 224, 16,240},
		{ 16, 224, 32,240},
		{ 32, 224, 48,240},
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
	if( npc->count1 > 150 ){
//		PlaySoundObject( WAVE_BOM, 1 );
//		SetDestroyNpChar( npc->x, npc->y, 0, 3 );
		VanishNpChar( npc );
//		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}
}

// アーモンドオレンジ
void ActNpc179( NPCHAR *npc )
{
	//地面に着地
	if( npc->flag & FLAG_HIT_X ){
		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}

	npc->xm += -VS/16;
	npc->ym = 0;
	if( npc->xm < -VS*2 ) npc->xm = -VS*2;
	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rect_left[] = {
		{ 48, 224, 72,240},
		{ 72, 224, 96,240},
		{ 96, 224,120,240},
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
//		PlaySoundObject( WAVE_BOM, 1 );
//		SetDestroyNpChar( npc->x, npc->y, 0, 3 );
		VanishNpChar( npc );
//		npc->cond = 0;
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
	}
}
