#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"


// カーリーついてくる
void ActNpc180( NPCHAR *npc )
{
	long xx, yy;
	RECT rcLeft[] = {
		{  0, 96, 16,112},// 静止

		{ 16, 96, 32,112},// 歩行
		{  0, 96, 16,112},
		{ 32, 96, 48,112},
		{  0, 96, 16,112},

		{ 48, 96, 64,112},//5 静止
 
		{ 64, 96, 80,112},// 歩行
		{ 48, 96, 64,112},
		{ 80, 96, 96,112},
		{ 48, 96, 64,112},

		{144, 96,160,112},//10 倒れ
	
	};
	RECT rcRight[] = {
		{  0,112, 16,128},// 静止

		{ 16,112, 32,128},// 歩行
		{  0,112, 16,128},
		{ 32,112, 48,128},
		{  0,112, 16,128},

		{ 48,112, 64,128},//5 静止
 
		{ 64,112, 80,128},// 歩行
		{ 48,112, 64,128},
		{ 80,112, 96,128},
		{ 48,112, 64,128},

		{144,112,160,128},//10 倒れ
	};

	// 目的変更 ============================
	if( npc->y < gMC.y -(VS*PARTSSIZE*10) ){
		if( npc->y < 16*VS*PARTSSIZE ){
			// 一段目左へ
			npc->tgt_x = PARTSSIZE*VS*320;
			npc->tgt_y = npc->y;
		}else{
			npc->tgt_x = PARTSSIZE*VS*0;
			npc->tgt_y = npc->y;
		}
	}else if( gCurlyShoot_wait ){
		// 敵を攻撃
		npc->tgt_x = gCurlyShoot_x;
		npc->tgt_y = gCurlyShoot_y;
	}else{
		// 自キャラを目指す
		npc->tgt_x = gMC.x;
		npc->tgt_y = gMC.y;
	}

	// 壁止め
	if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT  ) npc->xm = 0;
	if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ) npc->xm = 0;


	switch( npc->act_no ){
	case   0:
	case  10:// 自キャラの座標へ
//		npc->x      = gMC.x;
//		npc->y      = gMC.y;
//		npc->act_no = 100;
//		npc->ani_no = 0;
		break;
	case  11:
		break;

	case  20:// 自キャラの座標へ武器を装備
		npc->x      = gMC.x;
		npc->y      = gMC.y;
		npc->act_no = 100;
		npc->ani_no = 0;
		SetNpChar( 183, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
		if( GetNPCFlag( 563 ) ) SetNpChar( 182, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
		else                    SetNpChar( 181, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
		
		break;


	case 40: //武器を装備
		npc->act_no   = 41;
		npc->act_wait =  0;
		npc->ani_no   = 10;
	case 41:
		npc->act_wait++;
		if( npc->act_wait == 15*50 ){
			npc->bits    &= ~BITS_EVENT_CHECK;
			npc->ani_no = 0;
		}
		if( npc->act_wait > 20*50 ){
//			npc->x      = gMC.x;
//			npc->y      = gMC.y;
			npc->act_no = 100;
			npc->ani_no = 0;
			SetNpChar( 183, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
			if( GetNPCFlag( 563 ) ) SetNpChar( 182, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
			else                    SetNpChar( 181, 0, 0, 0, 0, DIR_LEFT, npc, MAX_NPC/2 );
		}
		break;

		
	case 100: // 停止
		npc->ani_no = 0;
		npc->xm     = npc->xm *7/8;
		npc->count1 =  0;

		if(       npc->x > npc->tgt_x + VS*PARTSSIZE*1 ){
			npc->act_no = 200;
			npc->ani_no = 1;
			npc->direct = DIR_LEFT;
			npc->act_wait = Random( 20, 60 );
		}else if( npc->x < npc->tgt_x - VS*PARTSSIZE*1 ){
			npc->act_no = 300;
			npc->ani_no = 1;
			npc->direct = DIR_RIGHT;
			npc->act_wait = Random( 20, 60 );
		}

		break;

	case 200: // 左歩行
		npc->xm -= VS/16;
		npc->direct = DIR_LEFT;
		if( npc->flag & FLAG_HIT_LEFT ) npc->count1++;
		else                            npc->count1 = 0;
		break;
	case 210: // ジャンプ
		npc->xm -= VS/16;
		npc->direct = DIR_LEFT;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 100;
		}
		break;

	case 300: // 右歩行
		npc->xm += VS/16;
		npc->direct = DIR_RIGHT;
		if( npc->flag & FLAG_HIT_RIGHT ) npc->count1++;
		else                             npc->count1 = 0;
		break;
	case 310: // ジャンプ
		npc->xm += VS/16;
		npc->direct = DIR_RIGHT;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 100;
		}
		break;
	}


	if( gCurlyShoot_wait       ) gCurlyShoot_wait--;
	if( gCurlyShoot_wait == 70 ) npc->count2 = 10;

	// ->jump
	if( gCurlyShoot_wait == 60 && npc->flag & FLAG_HIT_BOTTOM && Random( 0, 2 ) ){
		npc->count1 = 0;
		npc->ym     = -VS*3;
		npc->ani_no = 1;
		PlaySoundObject( WAVE_JUMP, 1 );
		if( npc->x > npc->tgt_x ) npc->act_no = 210;
		else                      npc->act_no = 310;
	}

	// 対象物との距離
	xx = npc->x - npc->tgt_x;
	yy = npc->y - npc->tgt_y;
	if( xx < 0 ) xx *= -1;


	if( npc->act_no == 100 ){
		if( xx+VS*2 < yy ) npc->ani_no = 5;
		else               npc->ani_no = 0;
	}
	if( npc->act_no == 210 || npc->act_no == 310 ){
		if( xx+VS*2 < yy ) npc->ani_no = 6;
		else               npc->ani_no = 1;
	}
	// 徒歩左右共通アクション
	if( npc->act_no == 200 || npc->act_no == 300 ){
		npc->ani_wait++;

		if( xx+VS*2 < yy ) npc->ani_no = 6 + (npc->ani_wait/4)%4;
		else               npc->ani_no = 1 + (npc->ani_wait/4)%4;

		if( npc->act_wait ){
			npc->act_wait--;
			// ->jump
			if( npc->flag && FLAG_HIT_BOTTOM && npc->count1 > 10 ){
				npc->count1  =  0;
				npc->ym      = -VS*3;
				npc->act_no += 10;
				npc->ani_no  =  1;
				PlaySoundObject( WAVE_JUMP, 1 );
			}
		}else{
			npc->act_no = 100;
			npc->ani_no = 0;
		}
	}


	// 離れすぎたらジャンプが高くなる
	if( npc->act_no >= 100 && npc->act_no < 500 ){
		if( npc->x < gMC.x - VS*PARTSSIZE*5 ||
			npc->x > gMC.x + VS*PARTSSIZE*5 ){
			if( npc->flag && FLAG_HIT_LEFT|FLAG_HIT_RIGHT ) npc->ym += VS/32;
			else                                            npc->ym += VS/10;
		}else{
			npc->ym += VS/10;
		}
	}

	if( npc->xm >  VS*3/2   ) npc->xm =  VS*3/2;
	if( npc->xm < -VS*3/2   ) npc->xm = -VS*3/2;
	if( npc->ym >  MAX_MOVE ) npc->ym =  MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->act_no >= 100 && !(npc->flag & FLAG_HIT_BOTTOM) ){
		switch( npc->ani_no ){
		case 1000:
			break;
		default:
			if( xx+VS*2 < yy ) npc->ani_no = 6;
			else               npc->ani_no = 1;
		}
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// カーリーのマシンガン
void ActNpc181( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{216,152,232,168},
		{232,152,248,168},
	};
	RECT rcRight[] = {
		{216,168,232,184},
		{232,168,248,184},
	};

	if( !npc->pNpc ) return;

	// 配置
	if( npc->pNpc->ani_no < 5 ){
		if( npc->pNpc->direct == DIR_LEFT ){
			npc->direct = DIR_LEFT;
			npc->x = npc->pNpc->x - VS*8;
		}else{
			npc->direct = DIR_RIGHT;
			npc->x = npc->pNpc->x + VS*8;
		}
		npc->y = npc->pNpc->y;
		npc->ani_no = 0;
	}else{
		if( npc->pNpc->direct == DIR_LEFT ){
			npc->direct = DIR_LEFT;
			npc->x = npc->pNpc->x - VS*0;
		}else{
			npc->direct = DIR_RIGHT;
			npc->x = npc->pNpc->x + VS*0;
		}
		npc->y = npc->pNpc->y - VS*10;
		npc->ani_no = 1;
	}

	if( npc->pNpc->ani_no == 1 ||
		npc->pNpc->ani_no == 3 ||
		npc->pNpc->ani_no == 6 ||
		npc->pNpc->ani_no == 8 ){
		npc->y -= VS;
	}


	switch( npc->act_no ){
	case 0:
		if( npc->pNpc->count2 == 10 ){
			npc->pNpc->count2 = 0;
			npc->act_no   = 10;
			npc->act_wait =  0;
		}
		break;

	case 10:
		npc->act_wait++;
		if( npc->act_wait%6 == 1 ){
			if( npc->ani_no == 0 ){
				if( npc->direct == DIR_LEFT ){
					SetBullet( BULLET_MACHINEGUN3, npc->x - 4*VS, npc->y + 3*VS, DIR_LEFT );
					SetCaret(                      npc->x - 4*VS, npc->y + 3*VS, CARET_FLASH, 0 );
				}else{
					SetBullet( BULLET_MACHINEGUN3, npc->x + 4*VS, npc->y + 3*VS, DIR_RIGHT );
					SetCaret(                      npc->x + 4*VS, npc->y + 3*VS, CARET_FLASH, 0 );
				}
			}else{
				if( npc->direct == DIR_LEFT ){
					SetBullet( BULLET_MACHINEGUN3, npc->x - 2*VS, npc->y - 4*VS, DIR_UP );
					SetCaret(                      npc->x - 2*VS, npc->y - 4*VS, CARET_FLASH, 0 );
				}else{
					SetBullet( BULLET_MACHINEGUN3, npc->x + 2*VS, npc->y - 4*VS, DIR_UP );
					SetCaret(                      npc->x + 2*VS, npc->y - 4*VS, CARET_FLASH, 0 );
				}
			}
		}
		if( npc->act_wait == 60 )
			npc->act_no = 0;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// カーリーのポーラスター
void ActNpc182( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{184,152,200,168},
		{200,152,216,168},
	};
	RECT rcRight[] = {
		{184,168,200,184},
		{200,168,216,184},
	};

	if( !npc->pNpc ) return;

	// 配置
	if( npc->pNpc->ani_no < 5 ){
		if( npc->pNpc->direct == DIR_LEFT ){
			npc->direct = DIR_LEFT;
			npc->x = npc->pNpc->x - VS*8;
		}else{
			npc->direct = DIR_RIGHT;
			npc->x = npc->pNpc->x + VS*8;
		}
		npc->y = npc->pNpc->y;
		npc->ani_no = 0;
	}else{
		if( npc->pNpc->direct == DIR_LEFT ){
			npc->direct = DIR_LEFT;
			npc->x = npc->pNpc->x - VS*0;
		}else{
			npc->direct = DIR_RIGHT;
			npc->x = npc->pNpc->x + VS*0;
		}
		npc->y = npc->pNpc->y - VS*10;
		npc->ani_no = 1;
	}

	if( npc->pNpc->ani_no == 1 ||
		npc->pNpc->ani_no == 3 ||
		npc->pNpc->ani_no == 6 ||
		npc->pNpc->ani_no == 8 ){
		npc->y -= VS;
	}


	switch( npc->act_no ){
	case 0:
		if( npc->pNpc->count2 == 10 ){
			npc->pNpc->count2 = 0;
			npc->act_no   = 10;
			npc->act_wait =  0;
		}
		break;

	case 10:
		npc->act_wait++;
		if( npc->act_wait%12 == 1 ){
			if( npc->ani_no == 0 ){
				if( npc->direct == DIR_LEFT ){
					SetBullet( BULLET_POLESTAR3, npc->x - 4*VS, npc->y + 3*VS, DIR_LEFT );
					SetCaret(                    npc->x - 4*VS, npc->y + 3*VS, CARET_FLASH, 0 );
				}else{
					SetBullet( BULLET_POLESTAR3, npc->x + 4*VS, npc->y + 3*VS, DIR_RIGHT );
					SetCaret(                    npc->x + 4*VS, npc->y + 3*VS, CARET_FLASH, 0 );
				}
			}else{
				if( npc->direct == DIR_LEFT ){
					SetBullet( BULLET_POLESTAR3, npc->x - 2*VS, npc->y - 4*VS, DIR_UP );
					SetCaret(                    npc->x - 2*VS, npc->y - 4*VS, CARET_FLASH, 0 );
				}else{
					SetBullet( BULLET_POLESTAR3, npc->x + 2*VS, npc->y - 4*VS, DIR_UP );
					SetCaret(                    npc->x + 2*VS, npc->y - 4*VS, CARET_FLASH, 0 );
				}
			}
		}
		if( npc->act_wait == 60 )
			npc->act_no = 0;
		break;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ウオーターバルーン
void ActNpc183( NPCHAR *npc )
{
	RECT rect[] = {
		{ 56, 96, 80,120},
		{ 80, 96,104,120},
	};

	if( !npc->pNpc ) return;

	switch( npc->act_no ){
	case 0:
		npc->x = npc->pNpc->x;
		npc->y = npc->pNpc->y;
		npc->act_no = 1;
	case 1:
		break;
	}

	npc->x += (npc->pNpc->x - npc->x )/2;
	npc->y += (npc->pNpc->y - npc->y )/2;

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )
		npc->ani_no = 0;


	if( npc->pNpc->flag & FLAG_HIT_WATER ){
		npc->rect = rect[npc->ani_no];
	}else{
		npc->rect.right = 0;
	}
}



// バリケード大
void ActNpc184( NPCHAR *npc )
{
	long i;
	RECT rc[] = {
		{  0, 64, 32, 96},
		{ 32, 64, 64, 96},
		{ 64, 64, 96, 96},
		{ 32, 64, 64, 96},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->x += 8*VS;
		npc->y += 8*VS;
	case 1:
		break;
	case 10:// 移動
		npc->act_no   = 11;
		npc->ani_no   = 1;
		npc->act_wait = 0;
		npc->bits |= BITS_THROW_BLOCK;
	case 11:
		switch( npc->direct ){
		case DIR_LEFT:  npc->x -= VS/4; break;
		case DIR_UP:    npc->y -= VS/4; break;
		case DIR_RIGHT: npc->x += VS/4; break;
		case DIR_DOWN:  npc->y += VS/4; break;
		}
		npc->act_wait++;
		if( npc->act_wait%8 == 0 ) PlaySoundObject( WAVE_QUAKE, 1 );

		SetQuake( 20 );
		break;

	case 20://停止(煙)
		for( i = 0; i < 4; i++ ){
			SetNpChar( 4, npc->x + Random( -12, 12 ) * VS, npc->y + 16*VS,
				Random(-VS*2/3,VS*2/3), Random(-3*VS,0), DIR_LEFT, NULL, MAX_NPC/2 );
		}
		npc->act_no = 1;
		break;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 10 ){
		npc->ani_wait =  0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3 )
		npc->ani_no = 0;

	npc->rect = rc[npc->ani_no];
}


// バリケード中
void ActNpc185( NPCHAR *npc )
{
	RECT rc[] = {
		{ 96, 64,112, 96},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y += 8*VS;
	case 1:
		break;
	case 10:// 移動
		npc->act_no   = 11;
		npc->ani_no   = 1;
		npc->act_wait = 0;
		npc->bits |= BITS_THROW_BLOCK;
	case 11:
		switch( npc->direct ){
		case DIR_LEFT:  npc->x -= VS/4; break;
		case DIR_UP:    npc->y -= VS/4; break;
		case DIR_RIGHT: npc->x += VS/4; break;
		case DIR_DOWN:  npc->y += VS/4; break;
		}
		npc->act_wait++;

		break;
	case 20:
		npc->y -= VS*24;
		npc->act_no = 1;
		break;
	}


	npc->rect = rc[0];
}

// バリケード小
void ActNpc186( NPCHAR *npc )
{
	RECT rc[] = {
		{ 48, 48, 64, 64},
		{ 64, 48, 80, 64},
		{ 80, 48, 96, 64},
		{ 64, 48, 80, 64},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		break;
	case 10:// 移動
		npc->act_no   = 11;
		npc->ani_no   = 1;
		npc->act_wait = 0;
		npc->bits |= BITS_THROW_BLOCK;
	case 11:
		switch( npc->direct ){
		case DIR_LEFT:  npc->x -= VS/4; break;
		case DIR_UP:    npc->y -= VS/4; break;
		case DIR_RIGHT: npc->x += VS/4; break;
		case DIR_DOWN:  npc->y += VS/4; break;
		}
		npc->act_wait++;

		break;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 10 ){
		npc->ani_wait =  0;
		npc->ani_no++;
	}
	if( npc->ani_no > 3 )
		npc->ani_no = 0;

	npc->rect = rc[npc->ani_no];
}




//ウィルオウィプス
void ActNpc187( NPCHAR *npc )
{
	long i;
	switch( npc->act_no ){
	case 0://初期化
		npc->act_no   = 1;
		npc->tgt_x    = npc->x;
		npc->tgt_y    = npc->y;
		npc->count1   = 120;
		npc->act_wait = Random( 0, 50 );
		for( i = 0; i < 5; i++ ){
			SetNpChar( 188, 0, 0, 0, 0, i*(256/5), npc, MAX_NPC/2 );
		}

	case 1://ランダム待機
		npc->act_wait++;
		if( npc->act_wait < 50 )break;
		npc->act_wait = 0;
		npc->act_no   = 2;
		npc->ym       = VS*3/2;
		break;
	case 2://滞空
		npc->count1+= 4;
		//振り向き
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->tgt_y < npc->y ) npc->ym -= VS/32;
		if( npc->tgt_y > npc->y ) npc->ym += VS/32;

		if( npc->ym >  VS*5/3 )npc->ym =  VS*5/3;
		if( npc->ym < -VS*5/3 )npc->ym = -VS*5/3;
		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{224,104,256,136},
		{256,104,288,136},
	};
	RECT rect_right[] = {
		{224,136,256,168},
		{256,136,288,168},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 ) npc->ani_no = 0;
	if( npc->direct == DIR_LEFT )
		npc->rect = rect_left[npc->ani_no];
	else 
		npc->rect = rect_right[npc->ani_no];
}


//ウィルオウィプスミニ
void ActNpc188( NPCHAR *npc )
{
	unsigned char deg;

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->count1 = npc->direct;
	case 1:
		if( npc->pNpc->code_char == 187 && npc->pNpc->cond & COND_ALIVE ){
			deg = (unsigned char)( ( npc->pNpc->count1 + npc->count1 )%256 );
			npc->x = npc->pNpc->x + GetSin( deg ) * 20;
			npc->y = npc->pNpc->y + GetCos( deg ) * 32;
		}else{
			npc->xm = Random( -VS, VS );
			npc->ym = Random( -VS, VS );
			npc->act_no = 10;
		}

		break;
	case 10:
		if( gMC.x < npc->x ) npc->xm -= VS/16;
		else                 npc->xm += VS/16;
		if( gMC.y < npc->y ) npc->ym -= VS/16;
		else                 npc->ym += VS/16;
		if( npc->xm >  VS*4   ) npc->xm =  VS*4;
		if( npc->xm < -VS*4   ) npc->xm = -VS*4;
		if( npc->ym >  VS*1   ) npc->ym =  VS*1;
		if( npc->ym < -VS*1   ) npc->ym = -VS*1;
		npc->x += npc->xm;
		npc->y += npc->ym;

		break;

	}


	//振り向き
	if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
	else                 npc->direct = DIR_RIGHT;
	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )
		npc->ani_no = 0;

	
	//アニメーション------------------
	RECT rect_left[] = {
		{288,104,304,120},
		{304,104,320,120},
	};
	RECT rect_right[] = {
		{288,120,304,136},
		{304,120,320,136},
	};

	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[ npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];

}




//魂
void ActNpc189( NPCHAR *npc )
{

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->xm = -VS/8;
	case 1:
		npc->y += npc->ym;
		npc->act_wait++;
		if( npc->act_wait > 8*32 ){
			npc->act_no = 10;
		}
		break;
	case 10:
		if( gMC.x < npc->x ) npc->xm -= VS/64;
		else                 npc->xm += VS/64;
		if( gMC.y < npc->y ) npc->ym -= VS/64;
		else                 npc->ym += VS/64;
		if( npc->xm >  VS*2   ) npc->xm =  VS*2;
		if( npc->xm < -VS*2   ) npc->xm = -VS*2;
		if( npc->ym >  VS*2   ) npc->ym =  VS*2;
		if( npc->ym < -VS*2   ) npc->ym = -VS*2;
		npc->x += npc->xm;
		npc->y += npc->ym;

		break;

	}

	//振り向き
	if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
	else                 npc->direct = DIR_RIGHT;
	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )
		npc->ani_no = 0;

	
	//アニメーション------------------
	RECT rect[] = {
		{224,184,232,200},
		{232,184,240,200},
		{240,184,248,200},
	};

	npc->rect = rect[npc->ani_no];
}

// 壊れロボ
void ActNpc190( NPCHAR *npc )
{
	RECT rect[] = {
		{192, 32,208, 48},
		{208, 32,224, 48},
	};
	long i;


	switch( npc->act_no ){
	case 0:
		npc->ani_no = 0;
		break;
	case 10:
		PlaySoundObject( WAVE_DESTROY_LARGE, 1 );
		for( i = 0; i < 8; i++ ){
			SetNpChar( 4, npc->x, npc->y + Random( -8, 8 ) *VS,
				Random( -8, -2 )*VS, Random( -3, 3 )*VS, DIR_LEFT, NULL, MAX_NPC/2 );
		}
		npc->cond = 0;
		break;
	case 20:
		npc->ani_wait++;
		if( npc->ani_wait > 10 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		break;
	}

	npc->rect = rect[npc->ani_no];
}

// 水面操作
extern long gWaterY;
void ActNpc191( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->act_no = 10;
		npc->tgt_y  = npc->y;
		npc->ym     = VS;
	case 10:
		if( npc->y < npc->tgt_y ) npc->ym += VS/128;
		else                      npc->ym -= VS/128;
		if( npc->ym < -VS/2 ) npc->ym = -VS/2;
		if( npc->ym >  VS/2 ) npc->ym =  VS/2;
		npc->y += npc->ym;
		break;

	case 20:
		npc->act_no = 21;
		npc->act_wait = 0;
	case 21:
		if( npc->y < npc->tgt_y ) npc->ym += VS/128;
		else                      npc->ym -= VS/128;
		if( npc->ym < -VS/1 ) npc->ym = -VS/1;
		if( npc->ym >  VS/1 ) npc->ym =  VS/1;
		npc->y += npc->ym;
		npc->act_wait++;
		if( npc->act_wait > 20*50 ){
			npc->act_no = 22;
		}
		break;
	case 22:
		if( npc->y < 0 ) npc->ym += VS/128;
		else             npc->ym -= VS/128;
		if( npc->ym < -VS/1 ) npc->ym = -VS/1;
		if( npc->ym >  VS/1 ) npc->ym =  VS/1;
		npc->y += npc->ym;
		if( npc->y < 64*VS || gSuperYpos ){
			npc->act_no   = 21;
			npc->act_wait =  0;
		}
		break;

	case 30: //上昇っぱなし
		if( npc->y < 0 ) npc->ym += VS/128;
		else             npc->ym -= VS/128;
		if( npc->ym < -VS/1 ) npc->ym = -VS/1;
		if( npc->ym >  VS/2 ) npc->ym =  VS/2;
		npc->y += npc->ym;
		break;
	}


	gWaterY = npc->y;
	npc->rect.right  = 0;
	npc->rect.bottom = 0;
}

// バイク
void ActNpc192( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		npc->act_no      =  1;
		npc->view.back   = 16*VS;
		npc->view.front  = 16*VS;
		npc->view.top    =  8*VS;
		npc->view.bottom =  8*VS;
	case 1://停止
		break;

	case 10: //乗り
		npc->act_no = 11;
		npc->ani_no = 1;
		npc->view.top    =  16*VS;
		npc->view.bottom =  16*VS;
		npc->y -= VS*5;
	case 11: //乗り
		break;

	case 20: // 振動
		npc->act_no   = 21;
		npc->act_wait = 1;
		npc->tgt_x = npc->x;
		npc->tgt_y = npc->y;
	case 21:
		npc->x = npc->tgt_x + Random( -1, 1 )*VS;
		npc->y = npc->tgt_y + Random( -1, 1 )*VS;
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no = 30;
		}
		break;

	case 30:// ターン
		npc->act_no   = 31;
		npc->act_wait = 1;
		npc->xm       = -VS*4;
		npc->x        = npc->tgt_x;
		npc->y        = npc->tgt_y;
		PlaySoundObject( WAVE_BOM, 1 );

	case 31:
		npc->xm += VS/16;
		npc->x += npc->xm;
		npc->act_wait++;
		npc->y = npc->tgt_y + Random( -1, 1 )*VS;
		if( npc->act_wait > 10 ){
			npc->direct = DIR_RIGHT;
		}
		if( npc->act_wait > 50*4 ){
			npc->act_no   = 40;
		}
		break;
	case 40:// GO!
		npc->act_no   = 41;
		npc->act_wait =  2;
		npc->direct   = DIR_LEFT;
		npc->y -= VS*16*3;
		npc->xm = -VS*8;
	case 41:
		npc->x += npc->xm;
		npc->y += npc->ym;
		npc->act_wait+=2;
		if( npc->act_wait > 50*12*2 ){
			npc->cond = 0;
		}
		break;
	}

	if( !(npc->act_wait%4) && npc->act_no >= 20 ){
		PlaySoundObject( WAVE_FIRE, 1 );
		if( npc->direct == DIR_LEFT ) SetCaret( npc->x+10*VS, npc->y+10*VS, CARET_MISSILE, DIR_RIGHT );
		else                          SetCaret( npc->x-10*VS, npc->y+10*VS, CARET_MISSILE, DIR_LEFT  );
	}

	RECT rcLeft[] = {
		{224, 64,256, 80},
		{256, 64,288, 96},
	};
	RECT rcRight[] = {
		{224, 80,256, 96},
		{288, 64,320, 96},
	};

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}


// 壊れバイク
void ActNpc193( NPCHAR *npc )
{
	RECT rc = {256, 96,320,112};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y +=  0*VS;
		npc->x += 24*VS;
	case 1:
		break;
	}
	npc->rect = rc;
}

// バラバラロボ
void ActNpc194( NPCHAR *npc )
{
	RECT rc = {192,120,224,128};

	if( !npc->act_no ){
		npc->act_no = 1;
		npc->y += VS*4;
	}
	npc->rect = rc;
}

// 排水檻
void ActNpc195( NPCHAR *npc )
{
	RECT rc = {112, 64,128, 80};

	npc->rect = rc;
}


// Stream の 上下
void ActNpc196( NPCHAR *npc )
{
	RECT rcLeft  = {112, 64,144, 80};
	RECT rcRight = {112, 80,144, 96};

	npc->x -= VS*6;
	if( npc->x <= VS*PARTSSIZE*19 ) npc->x += VS*PARTSSIZE*22;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft;
	else                          npc->rect = rcRight;
}


// はりせんぼん
void ActNpc197( NPCHAR *npc )
{

	RECT rc[] = {
		{  0,  0, 16, 16},
		{ 16,  0, 32, 16},
		{ 32,  0, 48, 16},
		{ 48,  0, 64, 16},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 10;
		npc->ani_wait =  0;
		npc->ym       = Random( -VS, VS );
		npc->xm       = VS*4;

	case 10:// 追いつき
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;

		if( npc->xm < 0 ){
			npc->damage =  3;
			npc->act_no = 20;
		}
		break;

	case 20:// ペカペカ突進
		npc->damage = 3;
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;

		if( npc->x < VS*PARTSSIZE*3 ){
			npc->destroy_voice = 0;
			LoseNpChar( npc, TRUE );
		}
		break;
	}

	if( npc->flag & FLAG_HIT_TOP    ) npc->ym = +VS;
	if( npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS;
	npc->xm -= VS/40;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];

}




//ソニック
void ActNpc198( NPCHAR *npc )
{
	RECT rcRight[] = {
		{208, 48,224, 72},
		{224, 48,240, 72},
		{240, 48,256, 72},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_wait++;
		if( npc->act_wait > 20 ){
			npc->act_no = 1;
			npc->xm     = 0;
			npc->ym     = 0;
			npc->count1 = 0;
		}
		break;
	case 1:
		npc->xm +=  VS/16;
		break;
	}
	npc->ani_wait++;
	if( npc->ani_wait > 0 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 )
		npc->ani_no = 0;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rcRight[npc->ani_no];

	npc->count1++;
	if( npc->count1 > 50*2 ) npc->cond = 0;
	if( npc->count1%4 == 1 ) PlaySoundObject( WAVE_SONIC, 1 );

}


// 旋風飛沫
void ActNpc199( NPCHAR *npc )
{
	RECT rect[] = {
		{ 72, 16, 74, 18},
		{ 74, 16, 76, 18},
		{ 76, 16, 78, 18},
		{ 78, 16, 80, 18},
		{ 80, 16, 82, 18},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = Random( 0, 2 );
		switch( npc->direct ){
		case 0: npc->xm = -1; break;
		case 1: npc->ym = -1; break;
		case 2: npc->xm =  1; break;
		case 3: npc->ym =  1; break;
		}
		npc->xm *= Random(4,8)*VS/2;
		npc->ym *= Random(4,8)*VS/2;
	case 1: break;
	}

	// アニメーション
	npc->ani_wait++;
	if( npc->ani_wait > 6 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 4 )
		npc->cond = 0;
	

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
//	npc->rect = rect[0];

//	if( npc->flag & FLAG_HIT_LEFT   ) npc->cond = 0;
//	if( npc->flag & FLAG_HIT_RIGHT  ) npc->cond = 0;
//	if( npc->flag & FLAG_HIT_TOP    ) npc->cond = 0;
//	if( npc->flag & FLAG_HIT_BOTTOM ) npc->cond = 0;
}
