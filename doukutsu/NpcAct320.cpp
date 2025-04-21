#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"
#include "KeyControl.h"

extern long g_GameFlags;

// 参戦おんぶカーリー
void ActNpc320( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{ 16, 96, 32,112},//0 横向き
		{ 48, 96, 64,112},//1 上向き
		{ 96, 96,112,112},//2 下向き
	};
	RECT rcRight[] = {
		{ 16,112, 32,128},//0 横向き
		{ 48,112, 64,128},//1 上向き
		{ 96,112,112,128},//2 下向き
	};
	
	if( !npc->act_no ){
		npc->act_no = 1;
		npc->x = gMC.x;
		npc->y = gMC.y;
		SetNpChar( 321, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
	}
	
	// 地面
	if( gMC.flag & FLAG_HIT_BOTTOM ){
		if( gMC.up ){
			npc->tgt_x      = gMC.x;
			npc->tgt_y      = gMC.y - 10*VS;
			npc->ani_no = 1;
		}else{
			npc->ani_no = 0;
			if( gMC.direct == DIR_LEFT ){
				npc->tgt_x = gMC.x +  7*VS;
				npc->tgt_y = gMC.y -  3*VS;
			}else{
				npc->tgt_x = gMC.x -  7*VS;
				npc->tgt_y = gMC.y -  3*VS;
			}
		}
	// 空中
	}else{
		if( gMC.up ){
			npc->tgt_x      = gMC.x;
			npc->tgt_y      = gMC.y +  8*VS;
			npc->ani_no = 2;
		}else if( gMC.down ){
			npc->tgt_x      = gMC.x;
			npc->tgt_y      = gMC.y -  8*VS;
			npc->ani_no = 1;

		}else{
			npc->ani_no = 0;
			if( gMC.direct == DIR_LEFT ){
				npc->tgt_x = gMC.x + 7*VS;
				npc->tgt_y = gMC.y -  3*VS;
			}else{
				npc->tgt_x = gMC.x - 7*VS;
				npc->tgt_y = gMC.y -  3*VS;
			}
		}
	}


	npc->x += (npc->tgt_x - npc->x )/2;
	npc->y += (npc->tgt_y - npc->y )/2;


	if( gMC.ani_no%2 ) npc->y -= VS;

	if( gMC.direct == DIR_LEFT ) npc->rect = rcRight[npc->ani_no];
	else                         npc->rect = rcLeft[ npc->ani_no];
	
}


// カーリーのネメシス
void ActNpc321( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{136,152,152,168},// 横
		{152,152,168,168},// 上
		{168,152,184,168},// 下
	};
	RECT rcRight[] = {
		{136,168,152,184},// 横
		{152,168,168,184},// 上
		{168,168,184,184},// 下
	};

	long direct;
	if( !npc->pNpc ) return;

	// 配置
	switch( npc->pNpc->ani_no  ){
	case 0:
		if( gMC.direct == DIR_LEFT ){
			npc->x = npc->pNpc->x + VS*8;
			direct = DIR_RIGHT;
		}else{
			npc->x = npc->pNpc->x - VS*8;
			direct = DIR_LEFT;
		}
		npc->y = npc->pNpc->y;
		break;
	case 1:
		if( gMC.direct == DIR_LEFT ) npc->x = npc->pNpc->x - VS*0;
		else                         npc->x = npc->pNpc->x + VS*0;
		direct = DIR_UP;
		npc->y = npc->pNpc->y - VS*10;
		break;
	case 2:
		if( gMC.direct == DIR_LEFT ) npc->x = npc->pNpc->x - VS*0;
		else                         npc->x = npc->pNpc->x + VS*0;
		direct = DIR_DOWN;
		npc->y = npc->pNpc->y + VS*10;
		break;
	}
	npc->ani_no = npc->pNpc->ani_no;
	if( g_GameFlags & GAMEFLAG_KEYCONTROL && CountBulletNum( BULLET_NEMESIS_C ) < 2 && gKeyTrg & gKeyShot ){
		
		SetBullet( BULLET_NEMESIS_C, npc->pNpc->x, npc->pNpc->y, direct    );
		SetCaret(                    npc->pNpc->x, npc->pNpc->y, CARET_FLASH, 0 );
		PlaySoundObject( WAVE_NEMESIS,  1 );
	}

//	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
//	else                          npc->rect = rcRight[npc->ani_no];
	if( gMC.direct == DIR_LEFT ) npc->rect = rcRight[npc->ani_no];
	else                         npc->rect = rcLeft[ npc->ani_no];

}


// バリケード
void DeleteMapParts( long x, long y );
void ActNpc322( NPCHAR *npc )
{
	RECT rc[] = {
		{272,216,296,240},
		{296,216,320,240},
		{160,216,184,240},
	};
	
	// ブロックを巻き添えに消える
	if( npc->act_no < 2 && npc->life <= 1000 - 32 ){
		npc->act_no   = 2;
		npc->act_wait = 0;
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits |=  BITS_BLOCK_BULLET;
		PlaySoundObject( WAVE_KEYBREAK, 1 );
	}
	switch( npc->act_no ){
	case 0:
		npc->act_no  = 1;
		if( npc->direct == DIR_LEFT ) npc->y += VS * 8;
		else                          npc->x += VS * 8;
	case 1:
		if( npc->shock ) npc->count1++;
		else             npc->count1 = 0;
		npc->rect = rc[ npc->count1/2%2 ];
		break;
	case 2:
		npc->ani_no = 2;
		switch( npc->act_wait ){
		case 50*0:SetNpChar( 207, npc->x + VS*4, npc->y, 0, 0, 0, NULL, MAX_NPC*3/4 );break;
		case 50*1:SetNpChar( 207, npc->x + VS*4, npc->y, 0, 0, 1, NULL, MAX_NPC*3/4 );break;
		case 50*2:SetNpChar( 207, npc->x + VS*4, npc->y, 0, 0, 2, NULL, MAX_NPC*3/4 );break;
		case 50*3:SetNpChar( 207, npc->x + VS*4, npc->y, 0, 0, 3, NULL, MAX_NPC*3/4 );break;
		case 50*4:SetNpChar( 207, npc->x + VS*4, npc->y, 0, 0, 4, NULL, MAX_NPC*3/4 );break;
		case 50*5:
			npc->hit.back   = VS*48;
			npc->hit.front  = VS*48;
			npc->hit.top    = VS*48;
			npc->hit.bottom = VS*48;
			npc->damage = 12;
			PlaySoundObject( WAVE_QUAKE, 1 );
			SetDestroyNpChar( npc->x, npc->y, 48*VS, 40 );
			SetQuake( 10 );
			if( npc->direct == DIR_LEFT ){
				DeleteMapParts( npc->x/VS/PARTSSIZE, (npc->y - 8*VS)/VS/PARTSSIZE );
				DeleteMapParts( npc->x/VS/PARTSSIZE, (npc->y + 8*VS)/VS/PARTSSIZE );
			}else{
				DeleteMapParts( (npc->x - 8*VS)/VS/PARTSSIZE, npc->y/VS/PARTSSIZE );
				DeleteMapParts( (npc->x + 8*VS)/VS/PARTSSIZE, npc->y/VS/PARTSSIZE );
			}
			npc->cond |= COND_LOSE;
			break;
		}
		npc->act_wait++;
		npc->rect = rc[ 2 ];
		break;
	}


}

// エンジェル（きりもみ→飛行）
void ActNpc323( NPCHAR *npc )
{
	RECT rc[] = {
		{216, 32,232, 56},
		{232, 32,248, 56},
		{216, 56,232, 80},
		{232, 56,248, 80},
	};
	
	npc->ani_wait++;
	if( npc->ani_wait > 3 ){
		npc->ani_wait = 0;
		npc->ani_no++;
		if( npc->ani_no > 3 )
			npc->ani_no = 0;
	}
	
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		switch( npc->direct ){
		case DIR_LEFT:  npc->xm = -3*VS; break;
		case DIR_RIGHT: npc->xm = +3*VS; break;
		case DIR_UP:    npc->ym = -3*VS; break;
		case DIR_DOWN:  npc->ym = +3*VS; break;
		}
	case 1:
		npc->act_wait++;
		if( npc->act_wait == 16 ) npc->bits &= ~BITS_THROW_BLOCK; 
		npc->x += npc->xm;
		npc->y += npc->ym;
		
		if( npc->flag & FLAG_HIT_X ){
			npc->act_no = 10;
		}
		if( npc->act_wait > 20 ){
			switch( npc->direct ){
			case DIR_LEFT:  if( npc->x <= gMC.x + 32*VS ) npc->act_no = 10; break;
			case DIR_RIGHT: if( npc->x >= gMC.x - 32*VS ) npc->act_no = 10; break;
			case DIR_UP:    if( npc->y <= gMC.y + 32*VS ) npc->act_no = 10; break;
			case DIR_DOWN:  if( npc->y >= gMC.y - 32*VS ) npc->act_no = 10; break;
			}
		}
		break;
	}
	
	if( npc->act_no == 10 ){
		npc->code_char = 309;
		npc->ani_no    = 0;
		npc->act_no    = 11;
		npc->bits     |=  BITS_BANISH_DAMAGE;
		npc->bits     &= ~BITS_THROW_BLOCK; 
		npc->damage    = 5;
		npc->view.top  = 8*VS;
		
	}
	
	npc->rect = rc[npc->ani_no];
}

// きりもみ配置
void ActNpc324( NPCHAR *npc )
{
	switch( npc->act_no ){
	case  0:
		break;
	case 10:// 5体配置
		npc->act_no   = 11;
		npc->act_wait =  0;
	case 11:// 
		npc->act_wait++;
		if( (npc->act_wait%50) == 1 ){
			SetNpChar( 323, npc->x, npc->y, 0, 0, npc->direct, NULL, MAX_NPC/2 );
		}
		if( npc->act_wait > 351 ) npc->act_no = 0;
		break;
	}

}

// プラズマ（ヘヴィプレス）
void ActNpc325( NPCHAR *npc )
{
	RECT rc[] = {
		{240, 96,272,128},//フラッシュ
		{272, 96,304,128},
		{240,128,272,160},

		{240,  0,256, 96},//プラズマ
		{256,  0,272, 96},
		{272,  0,288, 96},
		{288,  0,304, 96},
	};

	switch( npc->act_no ){
	case 0:// フラッシュ
		npc->act_no = 1;
		PlaySoundObject( WAVE_WARP, 1 );
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;

		npc->act_wait++;
		if( npc->act_wait > 50 ){
			npc->act_no     = 10;
			npc->ani_wait   =  0;
			npc->ani_no     =  3;
			npc->damage     = 10;
			npc->view.front =  8*VS;
			npc->view.top   = 12*VS;
			PlaySoundObject( WAVE_THUNDER, 1 );
			SetDestroyNpChar( npc->x, npc->y+84*VS, 0,  3 );
		}
		break;
	case 10:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 6 ){
			npc->cond = 0;
			return;
		}
		break;
	}
	npc->rect = rc[npc->ani_no];
}

// 戻り伊藤／スー
void ActNpc326( NPCHAR *npc )
{
	switch( npc->act_no ){

	case  0:// マシン待機
		npc->act_no =  1;
		npc->y     -=  8 * VS;
		npc->x     += 16 * VS;
		npc->ani_no =  0;
	case 1:
		if( ++npc->act_wait > 80 ){
			npc->act_no   = 10;
			npc->act_wait =  0;
			break;
		}
		if( npc->direct == DIR_LEFT ){
			if( npc->act_wait == 30 ) npc->ani_no = 1;
			if( npc->act_wait == 40 ) npc->ani_no = 0;
		}else{
			if( npc->act_wait == 50 ) npc->ani_no = 1;
			if( npc->act_wait == 60 ) npc->ani_no = 0;
		}
		break;
		
	case 10:// もどる
		if( ++npc->act_wait > 50 ){
			npc->act_no   = 15;
			npc->ani_no   =  4;
			if( npc->direct == DIR_LEFT ) npc->act_wait =   0;
			else                          npc->act_wait = -20;
			break;
		}
		if( npc->act_wait/2%2 ) npc->ani_no = 2;
		else                    npc->ani_no = 3;
		break;

	case 15:
		if( ++npc->act_wait > 40 ){
			npc->act_wait = 0;
			npc->act_no = 20;
		}

		break;

	case 20:// 地面待機
		npc->ym += VS/8;
		if( npc->ym > MAX_MOVE )
			npc->ym = MAX_MOVE;
		npc->y += npc->ym;
		
		if( ++npc->act_wait > 50 ){
			npc->act_no   = 30;
			npc->act_wait =  0;
			npc->ani_no   =  6;
			if( npc->direct == DIR_LEFT ){
				SetNpChar( 327, npc->x, npc->y - 16*VS, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
			}else{
				SetNpChar( 327, npc->x, npc->y -  8*VS, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
			}
			break;
		}
		break;

	case 30:// くしゃみ
		npc->act_wait++;
		if( npc->act_wait == 30 ) npc->ani_no =  7;
		if( npc->act_wait == 40 ) npc->act_no = 40;
		break;
	case 40:// 地面待機
		npc->act_no   = 41;
		npc->act_wait =  0;
		npc->ani_no   =  0;
	case 41:
		npc->act_wait++;
		if( npc->act_wait == 30 ) npc->ani_no =  1;
		if( npc->act_wait == 40 ) npc->ani_no =  0;
		break;
	}

	RECT rcItoh[] = {
		{  0,128, 16,152},// 待機
		{ 16,128, 32,152},
		{ 32,128, 48,152},//2 フラッシュ
		{ 48,128, 64,152},
		{ 64,128, 80,152},//4 人間
		{ 80,128, 96,152},
		{ 96,128,112,152},//6 へっ
		{112,128,128,152},// くし！
	};
	RECT rcSu[] = {
		{128,128,144,152},// 待機
		{144,128,160,152},
		{160,128,176,152},//2 フラッシュ
		{176,128,192,152},
		{192,128,208,152},//4 人間
		{208,128,224,152},
		{224,128,240,152},//6 へっ
		{ 32,152, 48,176},// くし！
	};
	if( npc->direct == DIR_LEFT ) npc->rect = rcItoh[ npc->ani_no ];
	else                          npc->rect = rcSu[   npc->ani_no ];
}

// クシャミ
void ActNpc327( NPCHAR *npc )
{
	RECT rc[] = { {240, 80,256, 96}, {256, 80,272, 96} };
	npc->act_wait++;
	switch( npc->act_no ){
	case 0:
		if( npc->act_wait < 4      ) npc->y     -= VS*2;
		if( npc->pNpc->ani_no == 7 ){
			npc->ani_no = 1;
			npc->act_no = 1;
			npc->tgt_x  = npc->x;
			npc->tgt_y  = npc->y;
		}
		break;
	case 1:
		if( npc->act_wait < 48 ){
			npc->x = npc->tgt_x + Random( -1, 1 )*VS;
			npc->y = npc->tgt_y + Random( -1, 1 )*VS;
		}else{
			npc->x = npc->tgt_x;
			npc->y = npc->tgt_y;
		}
		break;
	}
	if( npc->act_wait > 70     ) npc->cond   = 0;
	npc->rect = rc[ npc->ani_no ];
}

// 変身装置
void ActNpc328( NPCHAR *npc )
{
	RECT rc = {96,  0,128, 48};
	npc->rect = rc;
}

// 換気扇
void ActNpc329( NPCHAR *npc )
{
	RECT rc[] = {
		{ 48,  0, 64, 16},
		{ 64,  0, 80, 16},
	};
	npc->ani_wait++;
	if( npc->ani_wait/2%2 ) npc->rect = rc[0];
	else                    npc->rect = rc[1];
}

// コロコロ
void ActNpc330( NPCHAR *npc )
{
	RECT rc[] = {
		{144,136,160,152},
		{160,136,176,152},
		{176,136,192,152},
	};
	switch( npc->act_no ){
	case 0:
		ChangeMapParts( npc->x/VS/PARTSSIZE, npc->y/VS/PARTSSIZE, 0 );
		if( npc->direct == DIR_LEFT ) npc->act_no = 10;
		else                          npc->act_no = 30;
		break;
	case 10:// 左へ
		npc->xm += -VS/8;
		npc->ym =   0;
		if( npc->flag & FLAG_HIT_LEFT   ) npc->act_no = 20;
		break;
	case 20:// 上へ
		npc->xm =   0;
		npc->ym += -VS/8;
		if( npc->flag & FLAG_HIT_TOP    ) npc->act_no = 30;
		break;
	case 30:// 右へ
		npc->xm +=  VS/8;
		npc->ym =   0;
		if( npc->flag & FLAG_HIT_RIGHT  ) npc->act_no = 40;
		break;
	case 40:// 下へ
		npc->xm =   0;
		npc->ym +=  VS/8;
		if( npc->flag & FLAG_HIT_BOTTOM ) npc->act_no = 10;
		break;
	}
	if( npc->xm <-2*VS ) npc->xm = -2*VS;
	if( npc->xm > 2*VS ) npc->xm =  2*VS;
	if( npc->ym <-2*VS ) npc->ym = -2*VS;
	if( npc->ym > 2*VS ) npc->ym =  2*VS;
	npc->x += npc->xm;
	npc->y += npc->ym;
	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )
		npc->ani_no = 0;
	npc->rect = rc[npc->ani_no];
}

// スモークボーン
void ActNpc331( NPCHAR *npc )
{
	RECT rc[] = {
		{288, 80,304, 96},
		{304, 80,320, 96},
		{288, 96,304,112},
		{304, 96,320,112},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym     = -VS;
			npc->act_no =  10;
		}
		break;
	case 10:
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->cond   =   0;
			SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		}
		break;
	}
	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )
		npc->ym = MAX_MOVE;
	npc->y  += npc->ym;
	npc->x  += npc->xm;

	// アニメーション
	npc->ani_wait++;
	if( npc->ani_wait > 3 ){
		npc->ani_wait = 0;
		if( npc->direct == DIR_LEFT ) npc->ani_no++;
		else                          npc->ani_no--;
		if( npc->ani_no < 0 ) npc->ani_no += 4;
		if( npc->ani_no > 3 ) npc->ani_no -= 4;
	}

	npc->rect = rc[npc->ani_no];
}

// 
// ボーンスモーク
void ActNpc332( NPCHAR *npc )
{
	RECT rc[] = {
		{144, 96,168,120},
		{168, 96,192,120},
		{192, 96,216,120},
	};
	long xm;

	switch( npc->act_no ){
	case 0://爆裂
		PlaySoundObject( WAVE_BOM, 1 );
		npc->act_no   = 1;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
		else                          npc->xm =  VS*2;
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
			if( npc->ani_no > 2 )
				npc->ani_no = 0;
		}

		npc->act_wait++;
		if( npc->act_wait % 6 == 1 ){
			if( npc->direct == DIR_LEFT ) xm = Random( -16, -4 ) *VS/8;
			else                          xm = Random(   4, 16 ) *VS/8;
			SetNpChar( 331, npc->x, npc->y, xm,  -VS*2, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_BREAK1, 1 );
		}
		break;
	}

	if( npc->flag & FLAG_HIT_LEFT  ) npc->cond = 0;
	if( npc->flag & FLAG_HIT_RIGHT ) npc->cond = 0;
	npc->x   += npc->xm;
	npc->rect = rc[npc->ani_no];
}
// サンダーシンボル
void ActNpc333( NPCHAR *npc )
{
	RECT rc[] = {
		{ 80,120, 104,144},
		{104,120, 128,144},
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x = npc->x;
		npc->tgt_y = npc->y;
		PlaySoundObject( WAVE_CHARGE, 1 );
		npc->y = gMC.y;
	case 1:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 0;
		else                    npc->ani_no = 1;
		if( npc->direct == DIR_LEFT && npc->act_wait == 20 )
			SetNpChar( 146, npc->tgt_x, npc->tgt_y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		if( npc->act_wait > 40 ){
			npc->cond = 0;
		}
		break;
	}
	npc->rect = rc[ npc->ani_no ];
}


// 汗
void ActNpc334( NPCHAR *npc )
{
	RECT rcLeft[ ] = {
		{160,184,168,200},
		{168,184,176,200},
	};
	RECT rcRight[] = {
		{176,184,184,200},
		{184,184,192,200},
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no = 10;
		if( npc->direct == DIR_LEFT ){
			npc->x +=       10*VS;
			npc->y -=       18*VS;
		}else{
			npc->x = gMC.x -10*VS;
			npc->y = gMC.y - 2*VS;
		}
	case 10:
		npc->act_wait++;
		if( npc->act_wait /  8 % 2 ) npc->ani_no = 0;
		else                         npc->ani_no = 1;
		if( npc->act_wait >= 8 * 8 ) npc->cond   = 0;
		break;
	}
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}



// 右泳ぎいかちゃん
void ActNpc335( NPCHAR *npc )
{
	RECT rc[3] = {
		{ 0,16,16,32},//構え
		{16,16,32,32},//ダッシュ
		{32,16,48,32},//ニュートラル
	};
	
	switch( npc->act_no ){
	case 0://構え
		npc->act_no   = 1;
		npc->act_wait = Random( 3, 20 );
	case 1://構え
		npc->act_wait--;
		if( npc->act_wait <= 0 ){
			npc->act_no   = 2;
			npc->act_wait = Random( 10, 50 );
			npc->ani_no   = 1;
			npc->xm       = 3*VS;
		}
		break;
	case 2://ダッシュ
		npc->act_wait--;
		if( npc->act_wait <= 0 ){
			npc->act_no   = 3;
			npc->act_wait = Random( 40, 50 );
			npc->ani_no   = 2;
			npc->ym  = Random( -VS/2, VS/2 );
		}
		break;
	case 3://ニュートラル
		npc->act_wait--;
		if( npc->act_wait <= 0 ){
			npc->act_no   = 1;
			npc->act_wait = 0;
			npc->ani_no   = 0;
		}
		break;
	}
	npc->xm -= VS/32;

	npc->x += npc->xm;
	npc->y += npc->ym;

	
	npc->rect = rc[npc->ani_no];
}

// いかちゃん配置
void ActNpc336( NPCHAR *npc )
{
	long y;
	switch( npc->act_no ){
	case 0:
		if( gMC.shock ) npc->cond = 0;
		break;
	case 10:
		npc->act_wait++;
		if( npc->act_wait%4 == 1 ){
			y = npc->y + Random( 0, 13 ) * VS * PARTSSIZE;
			SetNpChar( 335, npc->x, y, 0, 0, DIR_LEFT, NULL, 0 );
		}
		break;
	}
}

// ドロール（彫刻)
void ActNpc337( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{256,112,288,152},
		{288,112,320,152},
	};

	switch( npc->act_no ){
	case 0:// 待機
		npc->act_no = 1;
		npc->y -= 8*VS;
	case 1:
		npc->xm     = 0;
		npc->act_no = 2;
		npc->ani_no = 0;
	case 2:
		npc->ani_wait++;
		if( npc->ani_wait > 50 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;

	}

	npc->ym += VS/8;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rcLeft[ npc->ani_no];
}


// グリーンデビル
void ActNpc338( NPCHAR *npc )
{
	RECT rcLeft[ ] = {
		{288,  0,304, 16},
		{304,  0,320, 16},
	};
	RECT rcRight[] = {
		{288, 16,304, 32},
		{304, 16,320, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no      =  1;
		npc->view.top    =  8*VS;
		npc->view.bottom =  8*VS;
		npc->damage      =  3;
		npc->bits |= BITS_BANISH_DAMAGE;
		npc->tgt_y = npc->y;
		npc->ym    = Random( -5*2, 5*2 )*VS/2;
	case 1:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		if( npc->y < npc->tgt_y ) npc->ym += VS/4;
		else                      npc->ym -= VS/4;
		if( npc->direct == DIR_LEFT ) npc->xm -= VS/16;
		else                          npc->xm += VS/16;
		if( npc->xm >  2*VS ) npc->xm =  2*VS;
		if( npc->xm < -2*VS ) npc->xm = -2*VS;

		if( npc->x < 0 || npc->y < 0 ||
			npc->x > gMap.width  *VS*PARTSSIZE ||
			npc->y > gMap.length *VS*PARTSSIZE ){
			VanishNpChar( npc );
			return;
		}
		break;
	}
	npc->x += npc->xm;
	npc->y += npc->ym;
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// グリーンデビル配置
void ActNpc339( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->act_wait = Random( 0, 40 );
	case 1:
		if( npc->act_wait ){
			npc->act_wait--;
			break;
		}
		npc->act_no = 0;
		SetNpChar( 338, npc->x, npc->y + Random( -16, 16 )*VS, 0, 0, npc->direct, NULL, MAX_NPC/2 );
		break;
	case 10:
		break;
	}
}


