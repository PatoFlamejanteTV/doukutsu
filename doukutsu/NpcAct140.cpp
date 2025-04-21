#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"

#include "forNpc.h"

// トロ子（狂暴化）
void ActNpc140( NPCHAR *npc )
{
	long i;

	RECT rcLeft[] = {
		{  0,  0, 32, 32},
		{ 32,  0, 64, 32},
		{ 64,  0, 96, 32},// かがみ
		{ 96,  0,128, 32},// ジャンプ
		{128,  0,160, 32},// 構え
		{160,  0,192, 32},// 投げ
		{192,  0,224, 32},// 構え（空中）
		{224,  0,256, 32},// 投げ（空中）

		{  0, 64, 32, 96 },//トロ子
		{ 32, 64, 64, 96 },//変か
		{ 64, 64, 96, 96 },

		{ 96, 64,128, 96 },//ガク
		{128, 64,160, 96 },//バタ
		{  0,  0,  0,  0 },

	};
	RECT rcRight[] = {
		{  0, 32, 32, 64},
		{ 32, 32, 64, 64},
		{ 64, 32, 96, 64},//2 かがみ
		{ 96, 32,128, 64},//3 ジャンプ
		{128, 32,160, 64},//4 構え
		{160, 32,192, 64},//5 投げ
		{192, 32,224, 64},//6 構え（空中）
		{224, 32,256, 64},// 投げ（空中）

		{  0, 96, 32,128 },//8トロ子
		{ 32, 96, 64,128 },//9変か
		{ 64, 96, 96,128 },
	
		{ 96, 96,128,128 },//11ガク
		{128, 96,160,128 },//12バタ
		{  0,  0,  0,  0 },
	};


	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 9;
		npc->act_wait = 0;
		npc->bits &= ~BITS_EVENT_CHECK;
	case 1:
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 2;
			npc->act_wait = 0; 
			npc->ani_no   = 8;
		}
		break;
	case 2:// 変化
		npc->ani_no++;
		if( npc->ani_no > 10 )
			npc->ani_no = 9;

		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 3;
			npc->act_wait = 0;
			npc->ani_no   = 0;
		}
		break;
	case 3:
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 10;
			npc->bits |= BITS_BANISH_DAMAGE;
		}
		break;
	case 10://直立 ---
		npc->bits = npc->bits;
		npc->act_no   = 11;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->act_wait = Random( 20,130 );
		npc->xm       = 0;
	case 11:
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ) npc->ani_no = 0; 

		if( CountArmsBullet( 6 ) ||
			CountArmsBullet( 3 ) > 3 ){ npc->act_no = 20; }

		if( npc->act_wait ) npc->act_wait--;
		else{
			if( Random( 0, 99 )%2 ) npc->act_no = 20;
			else                    npc->act_no = 50;
		}
		break;

	case 20://かがみ → ジャンプ → 投げ → 着陸 ---
		npc->act_no   = 21;
		npc->ani_no   = 2;
		npc->act_wait = 0;
	case 21:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no   = 22;
			npc->act_wait = 0;
			npc->ani_no   = 3;
			npc->ym       = -MAX_MOVE;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS;
			else                          npc->xm =  VS;
		}
		break;
	case 22://滞空
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no   = 23;
			npc->act_wait = 0;
			npc->ani_no   = 6;
			SetNpChar( 141, 0,0,0,0,DIR_LEFT,npc,0 );
		}
		break;
	case 23://かまえ
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 24;
			npc->act_wait = 0;
			npc->ani_no   = 7;
		}
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		break;
	case 24://なげ
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->act_no   = 25;
			npc->ani_no   = 3;
		}
		break;

	case 25:
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 26;
			npc->act_wait = 0;
			npc->ani_no   = 2;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 20 );
		}
		break;
	case 26://着陸
		npc->xm = npc->xm *8/9;
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no = 10;
			npc->ani_no = 0;
		}
		break;

	
	case 50://かまえ
		npc->act_no   = 51;
		npc->act_wait = 0;
		npc->ani_no   = 4;
		SetNpChar( 141, 0,0,0,0,DIR_LEFT,npc,0 );
	case 51:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 52;
			npc->act_wait = 0;
			npc->ani_no   = 5;
		}
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		break;
	case 52://なげ
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->act_no   = 10;
			npc->ani_no   = 0;
		}
		break;

		
	case 100:// やられ -----------
		npc->ani_no = 3;
		npc->act_no = 101;
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->damage = 0;
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		break;
	case 101:
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 102;
			npc->act_wait = 0;
			npc->ani_no   = 2;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetQuake( 20 );
		}
		break;
	case 102://着陸
		npc->xm = npc->xm *8/9;
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no   = 103;
			npc->act_wait = 0;
			npc->ani_no   = 10;
		}
		break;
	case 103://うがあ･･
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->ani_no   = 9;
			npc->act_no = 104;
			npc->act_wait = 0;
		}
		break;
	case 104:// 変化
		npc->ani_no++;
		if( npc->ani_no > 10 )
			npc->ani_no =  9;

		npc->act_wait++;
		if( npc->act_wait > 100 ){
			npc->act_wait =   0;
			npc->ani_no   =   9;
			npc->act_no   = 105;
		}
		break;
	case 105:// ああ…
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->ani_wait =   0;
			npc->act_no   = 106;
			npc->ani_no   =  11;
		}
		break;
	case 106:// ガク…バタ
		npc->ani_wait++;
		if( npc->ani_wait > 50 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 12 )
			npc->ani_no = 12;
		break;

	
	case 140:// 消滅 ====
		npc->act_no   = 141;
		npc->act_wait =  0;
		npc->ani_no   = 12;
		PlaySoundObject( WAVE_WARP, 1 );
	case 141:
		npc->ani_no++;
		if( npc->ani_no > 13 )
			npc->ani_no = 12;
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



	if( npc->act_no > 100 && npc->act_no < 105 ){
		if( npc->act_wait%9 == 0 )
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + Random( -12, 12 ) * VS,
						Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
	}

	npc->ym += VS/16;

	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// トロ子岩
void ActNpc141( NPCHAR *npc )
{
	long i;
	unsigned char deg;
	RECT rect[] = {
		{288, 32,304, 48},
		{304, 32,320, 48},
	};
	
	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->act_wait = 0;
	case 1:
		// くっつき
		if( npc->pNpc->direct == DIR_LEFT ) npc->x = npc->pNpc->x + 10*VS;
		else                                npc->x = npc->pNpc->x - 10*VS;
		npc->y = npc->pNpc->y -8*VS;

		if( npc->pNpc->act_no == 24 ||
			npc->pNpc->act_no == 52 ){
			npc->act_no = 10;
			if( npc->pNpc->direct == DIR_LEFT ) npc->x = npc->pNpc->x - 16*VS;
			else                                npc->x = npc->pNpc->x + 16*VS;
			npc->y = npc->pNpc->y;

			
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );

			npc->ym = GetSin( deg ) * 4;
			npc->xm = GetCos( deg ) * 4;
			PlaySoundObject( WAVE_POP, 1 );
		}


		break;

	
	case 10://なげられ
		if( npc->flag & (FLAG_HIT_BOTTOM|FLAG_HIT_LEFT|FLAG_HIT_RIGHT|FLAG_HIT_TOP) ){
			npc->act_no   = 20;
			npc->act_wait = 0;
			SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
			PlaySoundObject( WAVE_BREAK1, 1 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x, npc->y, Random(-VS,VS), Random(-VS,VS), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			break;
		}
		npc->x += npc->xm;
		npc->y += npc->ym;
		break;
	case 20://なげられ
		npc->x += npc->xm;
		npc->y += npc->ym;
		npc->act_wait++;
		if( npc->act_wait > 4 ){
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x, npc->y, Random(-VS,VS), Random(-VS,VS), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			//花になる
			npc->code_char = 142;
			npc->ani_no =  0;
			npc->act_no = 20;
			npc->xm = 0;
			npc->bits &= ~BITS_BLOCK_BULLET;
			npc->bits |=  BITS_BANISH_DAMAGE;
			npc->damage = 1;
		}
		break;
	}


	npc->ani_no++;
	if( npc->ani_no > 1 ) npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];
}


// プチローズ
void ActNpc142( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,128, 16,144},
		{ 16, 128, 32, 144},
		{ 32, 128, 48, 144},
		{ 48, 128, 64, 144},
		{ 64, 128, 80, 144}
		,
	};

	switch( npc->act_no ){
	case 0:
//		npc->ym = -VS;
//		if( gMC.x < npc->x ) npc->xm = -VS;
//		else                 npc->xm =  VS;
		break;
	case 10:// 地面
		npc->act_no   = 11;
		npc->ani_no   =  0;
		npc->act_wait =  0;
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no   = 12;
			npc->ani_no   =  1;
			npc->ani_wait =  0;
		}
		break;

	case 12://屈伸
		npc->ani_wait++;
		if( npc->ani_wait > 8 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no == 3 ){
			npc->act_no = 20;
			npc->ym = -VS;
			if( gMC.x < npc->x ) npc->xm = -VS;
			else                 npc->xm =  VS;
		}
		break;
	case 20:
		if( npc->ym > -VS/4 ) npc->ani_no = 4;
		else                  npc->ani_no = 3;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_no   =  2;
			npc->act_no   = 21;
			npc->act_wait =  0;
			npc->xm       =  0;
			PlaySoundObject( WAVE_STEP, 1 );
		}
		break;
	case 21:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 10;
			npc->ani_no =  0;
		}
		break;
	}

	npc->ym += VS/8;

	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;
	if( npc->ym < -MAX_MOVE ) npc->ym = -MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];

}

// 転びジェンカ
void ActNpc143( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{208, 32,224, 48},
	};
	RECT rcRight[] = {
		{208, 48,224, 64},
	};

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}





// トロ子（召還）
void ActNpc144( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{  0, 64, 16, 80},//停止
		{ 16, 64, 32, 80},

		{ 32, 64, 48, 80},//2落下
		{ 16, 64, 32, 80},

		{128, 64,144, 80},//7つぶれ

	};

	RECT rcRight[] = {
		{  0, 80, 16, 96},//停止
		{ 16, 80, 32, 96},

		{ 32, 80, 48, 96},//2落下
		{ 16, 80, 32, 96},

		{128, 80,144, 96},//7つぶれ
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->tgt_x = npc->x;
		PlaySoundObject( WAVE_WARP, 1 );
	case 1:// フェードイン
		npc->act_wait++;
		if( npc->act_wait == 64 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
		}
		break;

	case 2:// 滞空
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 ) npc->ani_no = 2;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 4;
			npc->act_wait = 0;
			npc->ani_no   = 4;
			PlaySoundObject( WAVE_STEP, 1 );
		}
		break;
	case 4:// 着陸～
		break;

	case 10:
		npc->act_no   = 11;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 11://待機 =============
		if( Random( 0, 120 ) == 10 ){
			npc->act_no   = 12;
			npc->act_wait = 0;
			npc->ani_no   = 1;
		}
		break;
	case 12://瞬く
		npc->act_wait++;
		if( npc->act_wait > 8 ){
			npc->act_no = 11;
			npc->ani_no = 0;
		}

		break;


	}

	if( npc->act_no > 1 ){
		npc->ym += VS/16;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		npc->y += npc->ym;
	}


	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	if( npc->act_no == 1 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait/4;
		if( npc->act_wait/2%2 ) npc->x = npc->tgt_x;
		else                    npc->x = npc->tgt_x +VS;
	}
}


// つるぎ
void ActNpc145( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 96,32,112,48},
	};
	RECT rcRight[] = {
		{112,32,128,48},
	};
	switch( npc->act_no ){
	case 0:
		if( npc->pNpc->count2 == 0 ){
			if( npc->pNpc->direct == DIR_LEFT ) npc->direct = DIR_LEFT;
			else                                npc->direct = DIR_RIGHT;
		}else{
			if( npc->pNpc->direct == DIR_LEFT ) npc->direct = DIR_RIGHT;
			else                                npc->direct = DIR_LEFT;
		}

		if( npc->direct == DIR_LEFT ) npc->x = npc->pNpc->x - 10*VS;
		else                          npc->x = npc->pNpc->x + 10*VS;
		npc->y = npc->pNpc->y;

		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// 稲妻
void ActNpc146( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,  0,   0,   0},
		{256,  0, 272, 240},
		{272,  0, 288, 240},
		{288,  0, 304, 240},
		{304,  0, 320, 240},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		if( npc->direct == DIR_RIGHT ) SetFlash( 0, 0, FLASHMODE_FLASH );
	case 1:
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 2;
			PlaySoundObject( WAVE_THUNDER, 1 );
		}
		break;
	case 2:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no == 2 ) npc->damage = 10;
		if( npc->ani_no > 4 ){
			SetDestroyNpChar( npc->x, npc->y, 8*VS,  8 );
			npc->cond = 0;
		}
		break;
	}

	npc->rect = rect[npc->ani_no];
}




//クリッター LV3(垂直上昇)
void ActNpc147( NPCHAR *npc )
{
	long xm, ym;

	unsigned char deg;

	RECT rcLeft[] = {
		{  0, 96, 16,112},
		{ 16, 96, 32, 112},
		{ 32, 96, 48, 112},//ジャンプ
		{ 48, 96, 64, 112},
		{ 64, 96, 80, 112},
		{ 80, 96, 96, 112},
	};
	RECT rcRight[] = {
		{  0, 112, 16, 128},
		{ 16, 112, 32, 128},
		{ 32, 112, 48, 128},//ジャンプ
		{ 48, 112, 64, 128},
		{ 64, 112, 80, 128},
		{ 80, 112, 96, 128},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->y += 3*VS;
		npc->act_no = 1;
	case 1://待機
		if( npc->act_wait >= 8 &&
			npc->x - PARTSSIZE*6*VS < gMC.x &&
			npc->x + PARTSSIZE*6*VS > gMC.x &&
			npc->y - PARTSSIZE*6*VS < gMC.y &&
			npc->y + PARTSSIZE*2*VS > gMC.y ){

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
			npc->x - PARTSSIZE*3*VS < gMC.x &&
			npc->x + PARTSSIZE*3*VS > gMC.x &&
			npc->y - PARTSSIZE*6*VS < gMC.y &&
			npc->y + PARTSSIZE*2*VS > gMC.y ){

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
			if( npc->x > gMC.x )npc->direct = DIR_LEFT;
			else                npc->direct = DIR_RIGHT;
//			if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
//			else                          npc->xm =  VS/2;
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
			npc->act_wait > 60 ){

			npc->damage   = 3;//DANAGE 4
			npc->act_no   = 5;
			npc->ani_no   = 2;

			break;
		}
		if( npc->act_wait%4 == 1 ) PlaySoundObject( WAVE_HERI, 1 );

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym = -VS;
		}
		// バブル発射
		if( npc->act_wait%30 == 6 ){
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -6, 6 );
			ym = GetSin( deg ) * 3;
			xm = GetCos( deg ) * 3;
			SetNpChar( 148, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
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
		npc->ym += VS/16;
		if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	}else{
//		if( npc->x < gMC.x ) npc->xm += VS/16;
//		else                 npc->xm -= VS/16;
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


// クリッターバブル
void ActNpc148( NPCHAR *npc )
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
		{ 96,96,104,104},
		{104,96,112,104},
	};
		npc->ani_no++;
	if( npc->ani_no > 1)npc->ani_no = 0;
	npc->rect = rect_left[npc->ani_no];

	//時間消滅
	npc->count1++;
	if( npc->count1 > 300 ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}
}

// 悪魔の岩
void ActNpc149( NPCHAR *npc )
{
	long i;
	switch( npc->act_no ){
	case 0:
		npc->x += VS*8;
		npc->y += VS*8;
		if( npc->direct == DIR_LEFT ) npc->act_no = 10;
		else                          npc->act_no = 20;
		npc->xm = 0;
		npc->ym = 0;
		npc->bits |=  BITS_BLOCK_MYCHAR2;
		break;

	case 10:// 左待機
		npc->bits   &= ~BITS_DAMAGE_SIDE;
		npc->damage  =   0;
		if( gMC.x < npc->x +  25*VS &&
			gMC.x > npc->x - 400*VS &&
			gMC.y < npc->y +  25*VS &&
			gMC.y > npc->y -  25*VS ){
			npc->act_no   = 11;
			npc->act_wait =  0;
		}
		break;
	case 11:// 左移動
		npc->act_wait++;
		if( npc->act_wait%10 == 6 ){
			PlaySoundObject( WAVE_STONE, 1 );
		}
		if( npc->flag & FLAG_HIT_LEFT ){
			npc->xm     = 0;
			npc->direct = DIR_RIGHT;
			npc->act_no = 20;
			SetQuake( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x - 16*VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			break;
		}
		// 押しつぶし
		if( gMC.flag & FLAG_HIT_LEFT ){
			npc->bits   |= BITS_DAMAGE_SIDE;
			npc->damage  = 100;
		}else{
			npc->bits   &= ~BITS_DAMAGE_SIDE;
			npc->damage  = 0;
		}
		npc->xm -= VS/16;
		break;

	
	case 20:// 右待機
		npc->bits   &= ~BITS_DAMAGE_SIDE;
		npc->damage  =   0;
		if( gMC.x > npc->x - 25*VS &&
			gMC.x < npc->x + 400*VS &&
			gMC.y < npc->y + 25*VS &&
			gMC.y > npc->y - 25*VS ){
			npc->act_no   = 21;
			npc->act_wait =  0;
		}
		break;
	case 21:// 右移動
		npc->act_wait++;
		if( npc->act_wait%10 == 6 ){
			PlaySoundObject( WAVE_STONE, 1 );
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->xm     = 0;
			npc->direct = DIR_LEFT;
			npc->act_no = 10;
			SetQuake( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + 16*VS, npc->y + Random( -12, 12 ) * VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			break;
		}
		// 押しつぶし
		if( gMC.flag & FLAG_HIT_RIGHT ){
			npc->bits   |= BITS_DAMAGE_SIDE;
			npc->damage  = 100;
		}else{
			npc->bits   &= ~BITS_DAMAGE_SIDE;
			npc->damage  =   0;
		}
		npc->xm += VS/16;
		break;
	}

	if( npc->xm >  VS*1 ) npc->xm =  VS*1;
	if( npc->xm < -VS*1 ) npc->xm = -VS*1;

	npc->x += npc->xm;


	RECT rect = {16,0,48,32};
	npc->rect = rect;
}

// 主人公
void ActNpc150( NPCHAR *npc )
{
	long i;
	RECT rcLeft[] = {
		{  0, 0, 16, 16},
		{ 48, 0, 64, 16},//見上げ
		{144, 0,160, 16},

		{ 16, 0, 32, 16},//3
		{  0, 0, 16, 16},
		{ 32, 0, 48, 16},
		{  0, 0, 16, 16},

		{160, 0,176, 16},// 7
		
		{112, 0,128, 16},// 8向こう

	};

	RECT rcRight[] = {
		{  0,16, 16, 32},
		{ 48,16, 64, 32},//見上げ
		{144,16,160, 32},

		{ 16,16, 32, 32},//3
		{  0,16, 16, 32},
		{ 32,16, 48, 32},
		{  0,16, 16, 32},

		{160,16,176, 32},// 7

		{112,16,128, 32},// 8向こう
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = 0;
		if( npc->direct > 10 ){
			npc->x = gMC.x;
			npc->y = gMC.y;
			npc->direct -= 10;
		}
	case 1:
		break;
	case 2: // 見上げ
		npc->ani_no = 1;
		break;
	case 10: //倒れ
		npc->act_no = 11;
		for( i = 0; i < 4; i++ ){
			SetNpChar( 4, npc->x, npc->y, Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		PlaySoundObject( WAVE_DESTROY_MIDDLE, 1 );
	case 11:
		npc->ani_no =  2;
		break;

	case 20:
		npc->act_no   = 21;
		npc->act_wait = 64;
		PlaySoundObject( WAVE_WARP, 1 );

	case 21:// フェードアウト
		npc->act_wait--;
		if( !npc->act_wait ){
			npc->cond = 0;
		}
		break;

	case 50:// 歩く
		npc->act_no   = 51;
		npc->ani_no   =  3;
		npc->ani_wait =  0;
	case 51:
		npc->ani_wait++;
		if( npc->ani_wait > 4 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 )
			npc->ani_no = 3;

		if( npc->direct == DIR_LEFT ) npc->x -= VS;
		else                          npc->x += VS;
		break;

	case 60: // 落下
		npc->act_no = 61;
		npc->ani_no = 7;
		npc->tgt_x = npc->x;
		npc->tgt_y = npc->y;
	case 61:
		npc->tgt_y += VS/2;
		npc->x = npc->tgt_x + Random( -1, 1 )*VS;
		npc->y = npc->tgt_y + Random( -1, 1 )*VS;
		break;

	case 70: // あとずさり
		npc->act_no   = 71;
		npc->act_wait =  0;
		npc->ani_no   =  3;
		npc->ani_wait =  0;
	case 71:
		if( npc->direct == DIR_LEFT ) npc->x += VS/2;
		else                          npc->x -= VS/2;
		npc->ani_wait++;
		if( npc->ani_wait > 8 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 )
			npc->ani_no = 3;
		break;
	case 80:// 向こう
		npc->ani_no = 8;
		break;

	case  99:
//		SetNpChar( 303, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
	case 100:// ムンヲーク
		npc->act_no = 101;
		npc->ani_no =   3;
		npc->ani_wait = 0;
	case 101:
		npc->ym += VS/8;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym     =   0;
			npc->act_no = 102;
		}
		npc->y += npc->ym;
		break;
	case 102:
		npc->ani_wait++;
		if( npc->ani_wait > 8 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 )
			npc->ani_no = 3;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
	if( npc->act_no == 21 ){
		npc->rect.bottom = npc->rect.top + npc->act_wait/4;
		if( npc->act_wait/2%2 ) npc->rect.left++;
	}

	if( gMC.equip&EQUIP_MIMIGAH ){
		npc->rect.top    += 32;
		npc->rect.bottom += 32;
	}

}

// 黒ロボ
void ActNpc151( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{192,  0,208, 16},
		{208,  0,224, 16},
	};
	RECT rcRight[] = {
		{192, 16,208, 32},
		{208, 16,224, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機
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
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

//　ダミー破壊
void ActNpc152( NPCHAR *npc )
{
	RECT rc = {0,0,0,0};

	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_RIGHT ) npc->y += VS*16;
		npc->act_no   = 1;
		break;
	case 1:
		break;
	}
	npc->rect = rc;

}

static RECT grcKitL[] = {
		{  0,  0, 24, 24},
		{ 24,  0, 48, 24},//1  瞬き

		{ 48,  0, 72, 24},//2  ランニング
		{  0,  0, 24, 24},
		{ 72,  0, 96, 24},
		{  0,  0, 24, 24},

		{ 96,  0,120, 24},//6  タメ
		{120,  0,144, 24},//
		{144,  0,168, 24},//8  ファイア

		{168,  0,192, 24},//9  受け

		{192,  0,216, 24},//10 じた
		{216,  0,240, 24},//   ばた

		{240,  0,264, 24},//12 ウイング
		{264,  0,288, 24},//

		{  0, 48, 24, 72},//14 滞空
		{ 24, 48, 48, 72},
		{ 48, 48, 72, 72},//16 突撃
		{ 72, 48, 96, 72},

		{288,  0,312, 24},//18 空中ぺかぺか
		{ 24, 48, 48, 72},

		{ 96, 48,120, 72},//20 踏ん張り

};
static RECT grcKitR[] = {
		{  0, 24, 24, 48},
		{ 24, 24, 48, 48},//1  瞬き

		{ 48, 24, 72, 48},//2  ランニング
		{  0, 24, 24, 48},
		{ 72, 24, 96, 48},
		{  0, 24, 24, 48},

		{ 96, 24,120, 48},//6  タメ
		{120, 24,144, 48},//
		{144, 24,168, 48},//8  ファイア

		{168, 24,192, 48},//9  受け

		{192, 24,216, 48},//10 じた
		{216, 24,240, 48},//   ばた

		{240, 24,264, 48},//12 ウイング
		{264, 24,288, 48},//

		{  0, 72, 24, 96},//14 滞空
		{ 24, 72, 48, 96},
		{ 48, 72, 72, 96},//16 突撃
		{ 72, 72, 96, 96},

		{288, 24,312, 48},//18 空中ぺかぺか
		{ 24, 72, 48, 96},

		{ 96, 72,120, 96},//20 踏ん張り
	};

// ガウディ（徒歩）
void ActNpc153( NPCHAR *npc )
{

	if( npc->x > gMC.x + (PARTSSIZE*VS*20) ||
		npc->x < gMC.x - (PARTSSIZE*VS*20) ||
		npc->y > gMC.y + (PARTSSIZE*VS*15) ||
		npc->y < gMC.y - (PARTSSIZE*VS*15) ){
		return;
	}

	
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->xm     = 0;
		npc->ani_no = 0;
		npc->y     += 3*VS;
	case 1:
		if( Random( 0, 100 ) == 1 ){
			npc->act_no   = 2;
			npc->ani_no   = 1;
			npc->act_wait = 0;
		}
		if( Random( 0, 100 ) == 1 ){
			if( npc->direct == DIR_LEFT ) npc->direct = DIR_RIGHT;
			else                          npc->direct = DIR_LEFT;
		}
		if( Random( 0, 100 ) == 1 ){
			npc->act_no = 10;
		}
		break;
	case 2:// 瞬き
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;
		
	case 10:// 移動
		npc->act_no   = 11;
		npc->act_wait = Random( 25, 100 );
		npc->ani_no   =  2;
		npc->ani_wait =  0;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;
		// カウント
		if( npc->act_wait )npc->act_wait--;
		else{
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->xm     = 0;
		}
		// 横壁
		if(       npc->direct == DIR_LEFT && npc->flag  & FLAG_HIT_LEFT ){
			npc->ani_no = 2;
			npc->ym = -MAX_MOVE;
			npc->act_no = 20;
			if( !(gMC.cond & COND_HIDE) ) PlaySoundObject( WAVE_NPCJUMP1, 1 );
		}else if( npc->direct == DIR_RIGHT && npc->flag & FLAG_HIT_RIGHT ){
			npc->ani_no = 2;
			npc->ym = -MAX_MOVE;
			npc->act_no = 20;
			if( !(gMC.cond & COND_HIDE) ) PlaySoundObject( WAVE_NPCJUMP1, 1 );
		}



		break;

	case 20://ジャンプ滞空

		// 横壁
		if(       npc->direct == DIR_LEFT  && npc->flag & FLAG_HIT_LEFT  ){
			npc->count1++;
		}else if( npc->direct == DIR_RIGHT && npc->flag & FLAG_HIT_RIGHT ){
			npc->count1++;
		}else npc->count1 = 0;

		if( npc->count1 > 10 ){
			if( npc->direct == DIR_LEFT ) npc->direct = DIR_RIGHT;
			else                          npc->direct = DIR_LEFT;
		}
		if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
		else                          npc->xm =  VS/2;

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 21;
			npc->ani_no   = 20;
			npc->act_wait = 0;
			npc->xm       = 0;
			if( !(gMC.cond & COND_HIDE) ) PlaySoundObject( WAVE_STEP, 1 );

		}
		break;

	case 21://踏ん張り
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 1;
			npc->ani_no = 0;
		}
		break;
	}
	
	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;


	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = grcKitL[npc->ani_no];
	else                          npc->rect = grcKitR[npc->ani_no];

	// →やられ
	if( npc->life <= 1000 -15 ){
		npc->code_char = 154;
		npc->act_no    =   0;
	}
}
// ガウディ やられ
void ActNpc154( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits &= ~BITS_THROW_BLOCK;
		npc->damage = 0;
		npc->act_no = 1;
		npc->ani_no = 9;
		npc->ym     = -VS;
		if( npc->direct == DIR_LEFT ) npc->xm = VS/2;
		else                          npc->xm = -VS/2;
		PlaySoundObject( WAVE_NPC_INSECT, 1 );
		break;
	case 1://ふきとび
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ani_no   = 10;
			npc->ani_wait =  0;
			npc->act_no   =  2;
			npc->act_wait =  0;
		}
		break;
	case 2:// じたばた
		npc->xm = npc->xm *8/9;
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 11 )
			npc->ani_no = 10;
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

	if( npc->direct == DIR_LEFT ) npc->rect = grcKitL[npc->ani_no];
	else                          npc->rect = grcKitR[npc->ani_no];

}


// ガウディ（飛行）
void ActNpc155( NPCHAR *npc )
{

	unsigned char deg;
	long xm, ym;
	if( npc->x > gMC.x + (PARTSSIZE*VS*20) ||
		npc->x < gMC.x - (PARTSSIZE*VS*20) ||
		npc->y > gMC.y + (PARTSSIZE*VS*15) ||
		npc->y < gMC.y - (PARTSSIZE*VS*15) ){
		return;
	}

	
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
		npc->act_wait = Random( 70, 150 );
		npc->ani_no   = 14;

	case 1://滞空
		// あにめ
		npc->ani_no++;
		if( npc->ani_no > 15 )
			npc->ani_no = 14;


		if( npc->act_wait ) npc->act_wait--;
		else{
			npc->act_no =  2;
			npc->ani_no = 18;
		}

		break;

	case 2://攻撃
		npc->ani_no++;
		if( npc->ani_no > 19 )
			npc->ani_no = 18;

		npc->act_wait++;
		if( npc->act_wait > 30 ){ 
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -6, 6 );

			ym = GetSin( deg ) * 3;
			xm = GetCos( deg ) * 3;
			SetNpChar( 156, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			if( !(gMC.cond & COND_HIDE) ) PlaySoundObject( WAVE_POP, 1 );
			npc->act_no   = 1;
			npc->act_wait = Random( 70, 150 );
			npc->ani_no   = 14;
			npc->ani_wait = 0;
		}

		break;
	}

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
	
	npc->x += npc->xm;
	npc->y += npc->ym;




	if( npc->direct == DIR_LEFT ) npc->rect = grcKitL[npc->ani_no];
	else                          npc->rect = grcKitR[npc->ani_no];

	// →やられ
	if( npc->life <= 985 ){
		npc->code_char = 154;
		npc->act_no    =   0;
	}

}

//ガウディバブル
void ActNpc156( NPCHAR *npc )
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
		{ 96,112,112,128},
		{112,112,128,128},
		{128,112,144,128},
	};

		npc->ani_no++;
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

// 悪魔の岩
void ActNpc157( NPCHAR *npc )
{
	long i;
	switch( npc->act_no ){
	case 0:
		npc->x += VS*8;
		npc->y += VS*8;
		if( npc->direct == DIR_LEFT ) npc->act_no = 10;
		else                          npc->act_no = 20;
		npc->xm = 0;
		npc->ym = 0;
		npc->bits |=  BITS_BLOCK_MYCHAR2;
		break;

	case 10:// 上待機
		npc->bits   &= ~BITS_DAMAGE_SIDE;
		npc->damage  =   0;
		if( gMC.y < npc->y +  25*VS &&
			gMC.y > npc->y - 400*VS &&
			gMC.x < npc->x +  25*VS &&
			gMC.x > npc->x -  25*VS ){
			npc->act_no   = 11;
			npc->act_wait =  0;
		}
		break;
	case 11:// 上移動
		npc->act_wait++;
		if( npc->act_wait%10 == 6 ){
			PlaySoundObject( WAVE_STONE, 1 );
		}
		if( npc->flag & FLAG_HIT_TOP ){
			npc->ym     = 0;
			npc->direct = DIR_RIGHT;
			npc->act_no = 20;
			SetQuake( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y - 16*VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			break;
		}
		// 押しつぶし
		if( gMC.flag & FLAG_HIT_TOP ){
			npc->bits   |= BITS_DAMAGE_SIDE;
			npc->damage  = 100;
		}else{
			npc->bits   &= ~BITS_DAMAGE_SIDE;
			npc->damage  = 0;
		}
		npc->ym -= VS/16;
		break;

	
	case 20:// 下待機
		npc->bits   &= ~BITS_DAMAGE_SIDE;
		npc->damage  =   0;
		if( gMC.y > npc->y - 25*VS &&
			gMC.y < npc->y + 400*VS &&
			gMC.x < npc->x + 25*VS &&
			gMC.x > npc->x - 25*VS ){
			npc->act_no   = 21;
			npc->act_wait =  0;
		}
		break;
	case 21:// 下移動
		npc->act_wait++;
		if( npc->act_wait%10 == 6 ){
			PlaySoundObject( WAVE_STONE, 1 );
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym     = 0;
			npc->direct = DIR_LEFT;
			npc->act_no = 10;
			SetQuake( 10 );
			PlaySoundObject( WAVE_QUAKE, 1 );
			for( i = 0; i < 4; i++ ){
				SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + 16*VS,
					Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
			}
			break;
		}
		// 押しつぶし
		if( gMC.flag & FLAG_HIT_BOTTOM ){
			npc->bits   |= BITS_DAMAGE_SIDE;
			npc->damage  = 100;
		}else{
			npc->bits   &= ~BITS_DAMAGE_SIDE;
			npc->damage  =   0;
		}
		npc->ym += VS/16;
		break;
	}

	if( npc->ym >  VS*1 ) npc->ym =  VS*1;
	if( npc->ym < -VS*1 ) npc->ym = -VS*1;

	npc->y += npc->ym;


	RECT rect = {16,0,48,32};
	npc->rect = rect;
}
// 誘導ミサイル
void ActNpc158( NPCHAR *npc )
{
	long dir;
	RECT rect[] = {
		{  0,224, 16,240},
		{ 16,224, 32,240},
		{ 32,224, 48,240},
		{ 48,224, 64,240},
		{ 64,224, 80,240},
		{ 80,224, 96,240},
		{ 96,224,112,240},
		{112,224,128,240},
	};


	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		switch( npc->direct ){
		case 0: npc->count1 = 32*5; break;
		case 1: npc->count1 = 32*7; break;
		case 2: npc->count1 = 32*1; break;
		case 3: npc->count1 = 32*3; break;
		}
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

// バーストキャット
void ActNpc159( NPCHAR *npc )
{
	RECT rect = {144,128,192,200};
	long i;

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4,
				npc->x + Random( -16, 16 ) * VS,
				npc->y + Random( -16, 16 ) * VS,
				Random(-VS*2/3,VS*2/3),
				Random(-VS*2/3,VS*2/3), DIR_LEFT, NULL, MAX_NPC/2 );
		}
	case 1:
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no = 2;
			npc->xm = -VS/2;
		}
		if( npc->act_wait/2%2 ) npc->x += VS;
		else                  npc->x -= VS;
		
		break;
	case 2:// 落下
		npc->act_wait++;
		npc->ym += VS/8;
		if( npc->y > 40*VS*PARTSSIZE ){
			npc->cond = 0;
		}
		break;
	}
	npc->y += npc->ym;
	npc->x += npc->xm;
	npc->rect = rect;

	if( npc->act_wait%8 == 1 )
		SetNpChar( 4,
				npc->x + Random( -16, 16 ) * VS,
				npc->y + Random( -16, 16 ) * VS,
				Random(-VS*2/3,VS*2/3),
				Random(-VS*2/3,VS*2/3), DIR_LEFT, NULL, MAX_NPC/2 );

}
