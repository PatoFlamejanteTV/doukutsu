#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"

// スー（召還）
void ActNpc280( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{112, 32,128, 48},//フェードイン
		{144, 32,160, 48},
	};

	RECT rcRight[] = {
		{112, 48,128, 64},//フェードイン
		{144, 48,160, 64},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->x        += 6*VS;
		npc->tgt_x    = npc->x;
		PlaySoundObject( WAVE_WARP, 1 );
	case 1:// フェードイン
		npc->act_wait++;
		if( npc->act_wait == 64 ){
			npc->act_no   = 2;
			npc->act_wait = 0;
		}
		break;

	case 2:// 滞空
		npc->ani_no = 0;
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 4;
			npc->act_wait = 0;
			npc->ani_no   = 1;
			PlaySoundObject( WAVE_STEP, 1 );
		}
		break;
	case 4:// 着陸～
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

// ブラッディ=ドクター
void ActNpc281( NPCHAR *npc )
{
	RECT rc = {0,0,0,0};
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		break;
	case 10://増殖
		npc->act_no   = 11;
		npc->act_wait =  0;
	case 11:
		npc->act_wait++;
		SetNpChar( 270, npc->x, npc->y + 128*VS, 0, 0, DIR_RIGHT, npc, MAX_NPC/2 );
		if( npc->act_wait > 150 ){
			npc->act_no = 12;
		}
		break;
	case 12:
		break;

	// ５秒待って血を削除
	case 20:
		npc->act_no   = 21;
		npc->act_wait =  0;
	case 21:
		npc->act_wait++;
		if( npc->act_wait > 250 ){
			DeleteNpCharCode( 270, FALSE );
			npc->act_no = 22;
		}
		break;

	case 22:
		break;
	}

//	gSuperXpos = npc->x;
//	gSuperYpos = npc->y;

	npc->rect = rc;
}

// コアオプション（リフト）
void ActNpc282( NPCHAR *npc )
{
	RECT tc[] = {
		{256, 80,320,120},
		{256,  0,320, 40},// すり抜け
		{256,120,320,160},// 目を開く
	};

	switch( npc->act_no ){
	case 0:	// 待機
		npc->act_no = 20;
		npc->tgt_y = npc->y;
		if( Random( 0, 100 )%2 ) npc->ym = -VS/2;
		else                     npc->ym =  VS/2;
//		break;

//	case 10:// 流れスタート
		
	case 20:

		npc->xm = -VS;
		if( npc->x < -64*VS ) npc->cond = 0;
			// npc->x += gMap.width * VS * PARTSSIZE;
		if( npc->tgt_y < npc->y ) npc->ym -= VS/32;
		if( npc->tgt_y > npc->y ) npc->ym += VS/32;

		if( npc->ym >  VS/2 )npc->ym =  VS/2;
		if( npc->ym < -VS/2 )npc->ym = -VS/2;

		// 背ノリ
		if( gMC.flag & FLAG_HIT_BOTTOM &&
			gMC.y < npc->y -  4*VS &&
			gMC.x > npc->x - 24*VS &&
			gMC.x < npc->x + 24*VS ){

			npc->tgt_y = VS*PARTSSIZE*9;
			npc->ani_no = 2;
		}else if( npc->ani_no != 1 ){
			npc->ani_no = 0;
		}

		//押しつぶし
		if( gMC.flag & FLAG_HIT_LEFT                          &&
			gMC.x                  < npc->x - npc->hit.back   &&
			gMC.x                  > npc->x - npc->hit.back-8*VS   &&
			gMC.y + gMC.hit.bottom > npc->y - npc->hit.top    &&
			gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom ){
			npc->bits &= ~BITS_BLOCK_MYCHAR2;
			npc->ani_no = 1;
		}else if( gMC.flag & FLAG_HIT_RIGHT                   &&
			gMC.x                  > npc->x + npc->hit.back   &&
			gMC.x                  < npc->x + npc->hit.back + 8*VS   &&
			gMC.y + gMC.hit.bottom > npc->y - npc->hit.top    &&
			gMC.y - gMC.hit.top    < npc->y + npc->hit.bottom ){
			npc->bits &= ~BITS_BLOCK_MYCHAR2;
			npc->ani_no = 1;
		}else if( gMC.flag & FLAG_HIT_TOP                     &&
			gMC.y                  < npc->y - npc->hit.top    &&
			gMC.y                  > npc->y - npc->hit.top-8*VS    &&
			gMC.x + gMC.hit.front  > npc->x - npc->hit.back   &&
			gMC.x - gMC.hit.back   < npc->x + npc->hit.front  ){
			npc->bits &= ~BITS_BLOCK_MYCHAR2;
			npc->ani_no = 1;
		}else if( gMC.flag & FLAG_HIT_BOTTOM                  &&
			gMC.y                  > npc->y + npc->hit.bottom - 4*VS  &&
			gMC.y                  < npc->y + npc->hit.bottom + 12*VS &&
			gMC.x + gMC.hit.front  > npc->x - npc->hit.back   - 4*VS  &&
			gMC.x - gMC.hit.back   < npc->x + npc->hit.front  + 4*VS  ){
			npc->bits &= ~BITS_BLOCK_MYCHAR2;
			npc->ani_no = 1;
		}
		break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = tc[npc->ani_no];
}

// ミザリアン
void ActNpc283( NPCHAR *npc )
{
	long x, y, direct;
	RECT rcLeft[] = {
		{  0, 64, 32, 96},
		{ 32, 64, 64, 96},
		{ 64, 64, 96, 96},//2 浮遊
		{ 96, 64,128, 96},//3 
		{128, 64,160, 96},//4 魔法
		{160, 64,192, 96},//5 
		{192, 64,224, 96},//6 発射

		{224, 64,256, 96},//7

		{  0,  0,  0,  0},// 8

		{256, 64,288, 96},//9  やられ
		{288, 64,320, 96},//10 倒れ
	};
	RECT rcRight[] = {
		{  0, 96, 32,128},
		{ 32, 96, 64,128},
		{ 64, 96, 96,128},
		{ 96, 96,128,128},
		{128, 96,160,128},// 
		{160, 96,192,128},// 
		{192, 96,224,128},//6 

		{224, 96,256,128},//7

		{  0,  0,  0,  0},// 8
		{256, 96,288,128},//9  やられ
		{288, 96,320,128},//10 倒れ
	};

	// やられ
	if( npc->act_no < 100 ){
		if( !gBoss[0].cond || npc->life < 1000 - 600 ) npc->act_no = 100;
	}
	switch( npc->act_no ){
	case 0:// 出現
		npc->act_no =    1;
		npc->y     -= VS*8;PlaySoundObject( WAVE_WARP, 1 );

	case 1:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 9;
		else                    npc->ani_no = 0;
		break;

	case 10://イベント待機
		npc->act_no   = 11;
		npc->ani_no   =  9;
	case 11:
		break;

	case 20://待機
		gSuperXpos    =  0;
		npc->act_no   = 21;
		npc->act_wait =  0;
		npc->ani_no   =  0;
		npc->ani_wait =  0;
	case 21:
		npc->xm = npc->xm *7/8;
		npc->ym = npc->ym *7/8;
		// ani
		npc->ani_wait++;
		if( npc->ani_wait > 20 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no   >  1 ) npc->ani_no = 0;
		
		npc->act_wait++;
		if( npc->act_wait > 100 ) npc->act_no = 30;
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
		break;

	case 30:// コアの前でガード
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->ani_no   =  2;
		npc->count2   = npc->life;
	case 31:
		// ani
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym = -VS;
		}
		if( npc->x > gBoss[0].x  ) npc->xm -= VS/16;
		else                              npc->xm += VS/16;
		if( npc->y > gMC.y              ) npc->ym -= VS/32;
		else                              npc->ym += VS/32;

		if( npc->xm >  1*VS ) npc->xm =  1*VS;
		if( npc->xm < -1*VS ) npc->xm = -1*VS;
		if( npc->ym >  1*VS ) npc->ym =  1*VS;
		if( npc->ym < -1*VS ) npc->ym = -1*VS;


		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;

		npc->act_wait++;
		if( npc->act_wait > 150 ){
			if( npc->life < npc->count2 - 20 || gSuperXpos ){
				gSuperXpos  = 0;
				npc->act_no = 40;
			}
		}
		if( gBoss[0].ani_no && npc->act_wait > 250) npc->act_no = 50;
		break;


	case 40:// 魔法
		npc->act_no   = 41;
		npc->act_wait =  0;
		npc->xm       =  0;
		npc->ym       =  0;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		PlaySoundObject( WAVE_CHARGE, 1 );
		if( gMC.y < VS*PARTSSIZE*10 ) npc->count2 = 290;
		else                          npc->count2 = 289;
	case 41:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 4;
		else                    npc->ani_no = 5;

		// クリッター配置
		if( npc->act_wait % 6 == 1 ){
			if( npc->count2 == 289 ){
				x = npc->x + Random( -64, 64 ) *VS;
				y = npc->y + Random( -32, 32 ) *VS;
			}else{
				x = npc->x + Random( -32, 32 ) *VS;
				y = npc->y + Random( -64, 64 ) *VS;
			}
			if( x < VS*32                           ) x = VS*32;
			if( x > (gMap.width  - 2)*VS*PARTSSIZE ) x = (gMap.width  - 2)*VS*PARTSSIZE;
			if( y < VS*32                           ) y = VS*32;
			if( y > (gMap.length - 2)*VS*PARTSSIZE ) y = (gMap.length - 2)*VS*PARTSSIZE;

			PlaySoundObject( WAVE_POP, 1 );
			SetNpChar( npc->count2, x, y, 0, 0, DIR_LEFT, 0, MAX_NPC/2 );
		}

		if( npc->act_wait > 50 ){
			npc->act_no   = 42;
			npc->act_wait =  0;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 42:
		npc->act_wait++;
		npc->ani_no = 6;
		if( npc->act_wait > 50 ){
			npc->ym = -VS;
			if( npc->direct == DIR_LEFT ) npc->xm =  VS;
			else                          npc->xm = -VS;
			npc->act_no = 30;
		}
		break;

	case 50: // ミサイル攻撃
		npc->act_no   = 51;
		npc->act_wait =  0;
		npc->xm       =  0;
		npc->ym       =  0;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		PlaySoundObject( WAVE_CHARGE, 1 );
		
	case 51:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 4;
		else                    npc->ani_no = 5;

		if( gMC.equip & EQUIP_BOOST2 ){
			// ミサイル配置
			if( npc->act_wait % 10 == 1 ){
				if( npc->direct == DIR_LEFT ){
					x = npc->x + 10*VS;
					y = npc->y;
					switch( (npc->act_wait/6)%4 ){
					case 0: direct = 256 - 40; break;
					case 1: direct = 256 - 20; break;
					case 2: direct =   0 + 20; break;
					case 3: direct =   0 + 40; break;
					}
				}else{
					x = npc->x - 10*VS;
					y = npc->y;
					switch( (npc->act_wait/6)%4 ){
					case 0: direct = 128 - 40; break;
					case 1: direct = 128 - 20; break;
					case 2: direct = 128 + 20; break;
					case 3: direct = 128 + 40; break;
					}
				}

				PlaySoundObject( WAVE_POP, 1 );
				SetNpChar( 301, x, y, 0, 0, direct, 0, MAX_NPC/2 );
			}
		}else{
			// ミサイル配置
			if( npc->act_wait % 24 == 1 ){
				if( npc->direct == DIR_LEFT ){
					x = npc->x + 10*VS;
					y = npc->y;
					switch( (npc->act_wait/6)%4 ){
					case 0: direct = 256 - 40; break;
					case 1: direct = 256 - 20; break;
					case 2: direct =   0 + 20; break;
					case 3: direct =   0 + 40; break;
					}
				}else{
					x = npc->x - 10*VS;
					y = npc->y;
					switch( (npc->act_wait/6)%4 ){
					case 0: direct = 128 - 40; break;
					case 1: direct = 128 - 20; break;
					case 2: direct = 128 + 20; break;
					case 3: direct = 128 + 40; break;
					}
				}

				PlaySoundObject( WAVE_POP, 1 );
				SetNpChar( 301, x, y, 0, 0, direct, 0, MAX_NPC/2 );
			}
		}

		if( npc->act_wait > 50 ){
			npc->act_no   = 42;
			npc->act_wait =  0;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;

	case  99: // ストップ
		npc->xm = 0;
		npc->ym = 0;
		npc->ani_no = 9;
		npc->bits      &= ~BITS_BANISH_DAMAGE;
		break;

	case 100: // ノックダウン
		npc->act_no     = 101;
		npc->ani_no     =   9;
		npc->damage     =   0;
		npc->bits      &= ~BITS_BANISH_DAMAGE;
		npc->bits      |=  BITS_THROW_BLOCK;
		npc->ym         = -VS;
		npc->shock     +=  50;
		npc->hit.bottom = 12*VS;
		gBoss[0].ani_no++;
	case 101:
		npc->ym += VS/16;
		if( npc->y > 13*VS*PARTSSIZE + 8*VS - npc->hit.bottom ){
			npc->y = 13*VS*PARTSSIZE + 8*VS - npc->hit.bottom;
			npc->act_no = 102;
			npc->ani_no =  10;
			npc->xm     =   0;
			npc->ym     =   0;
		}
		break;
	case 102:
		break;
	}


	npc->y += npc->ym;
	if( npc->shock ){
		npc->x += npc->xm/2;
	}else{
		npc->x += npc->xm;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// 
// バトルスー
void ActNpc284( NPCHAR *npc )
{
	unsigned char deg;
	RECT rcLeft[] = {
		{  0, 128, 32, 160},
		{ 32, 128, 64, 160},

		{ 64, 128, 96, 160},//2 屈み
		{ 96, 128,128, 160},//3 ダッシュ

		{128, 128,160, 160},//4 // スクリュー
		{160, 128,192, 160},//5 
		{192, 128,224, 160},//6 
		{224, 128,256, 160},//7

		{  0,  0,  0,  0},// 8

		{256, 128,288, 160},//9  やられ
		{288, 128,320, 160},//10 倒れ
		{224,  64,256,  96},//11 変身
		{208,  32,224,  48},//
	};
	RECT rcRight[] = {
		{  0, 160, 32,192},
		{ 32, 160, 64,192},

		{ 64, 160, 96,192},
		{ 96, 160,128,192},

		{128, 160,160,192},// 
		{160, 160,192,192},// 
		{192, 160,224,192},//6 
		{224, 160,256,192},//7

		{  0,  0,  0,  0},// 6
		{256, 160,288, 192},//9 やられ
		{288, 160,320, 192},//10 倒れ

		{224,  96,256, 128},//11 変身
		{208,  48,224,  64},//
	};

	// やられ
	if( npc->act_no < 100 ){
		if( !gBoss[0].cond || npc->life < 1000 - 500 ) npc->act_no = 100;
	}

	switch( npc->act_no ){
	case 0:// 出現
		npc->act_no =    1;
		npc->y     -= VS*4;
//		npc->y     -= VS*4;
		PlaySoundObject( WAVE_WARP, 1 );
		npc->count2   = npc->life;

	case 1:
		npc->act_wait++;
		if( npc->act_wait/2%2 ){
			npc->view.top   = 16*VS;
			npc->view.back  = 16*VS;
			npc->view.front = 16*VS;
			npc->ani_no   = 11;
		}else{
			npc->view.top  =  3*VS;
			npc->view.back =  8*VS;
			npc->view.front=  8*VS;
			npc->ani_no =   12;
		}
		if( npc->act_wait > 50 )npc->act_no = 10;
		break;

	case 10://イベント待機
		npc->act_no   = 11;
		npc->ani_no   = 11;
		npc->view.top   = 16*VS;
		npc->view.back  = 16*VS;
		npc->view.front = 16*VS;
		DeleteNpCharCode( 257, TRUE );
	case 11:
		break;

	case 20://待機
		npc->act_no   = 21;
		npc->act_wait =  0;
		npc->ani_no   =  0;
		npc->ani_wait =  0;
		npc->damage   =  0;
		npc->bits    |= BITS_BANISH_DAMAGE;
		npc->bits    &= ~BITS_THROW_BLOCK;
	case 21:
		npc->xm = npc->xm *7/8;
		npc->ym = npc->ym *7/8;
		// ani
		npc->ani_wait++;
		if( npc->ani_wait > 20 ){
			npc->ani_wait =  0;
			npc->ani_no++;
		}
		if( npc->ani_no   >  1 ) npc->ani_no = 0;
		
		npc->act_wait++;
		if( npc->act_wait > 80 ) npc->act_no = 30;
		if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
		else                 npc->direct = DIR_LEFT;
		if( npc->life < npc->count2 - 50 ){
			npc->count2   = npc->life;
			gSuperXpos    = 10;
		}
		break;

	case 30:// ダッシュ
		npc->act_no   = 31;
		npc->act_wait =  0;
		npc->ani_no   =  2;
		npc->xm = 0;
		npc->ym = 0;
	case 31:
		// ani
		npc->act_wait++;
		if( npc->act_wait > 16 ){

			npc->count1++;
			npc->count1 = npc->count1%4;
	//		if( gBoss[0].direct == DIR_LEFT ){
				switch( npc->count1 ){
				case 1: case 3: npc->act_no = 34; break;
				case 0:         npc->act_no = 32; break;
				case 2:         npc->act_no = 32; break;
				}
//			}else{
//				npc->act_no   = 32;
//			}

		}
		break;

	case 32:// ダッシュ移動
		npc->act_no   = 33;
		npc->act_wait =  0;
		npc->bits  &= ~BITS_BANISH_DAMAGE;
		if( gMC.x < npc->x ) npc->tgt_x = gMC.x - 10*PARTSSIZE*VS;
		else                 npc->tgt_x = gMC.x + 10*PARTSSIZE*VS;
		npc->tgt_y = gMC.y;
		deg = GetArktan( npc->x - npc->tgt_x, npc->y - npc->tgt_y );
		npc->xm = GetCos( deg ) * 3;
		npc->ym = GetSin( deg ) * 3;
		// マップの内側へ向かうときは壁をすり抜ける
		npc->bits &= ~BITS_THROW_BLOCK;
		if( npc->x < gMap.width*VS*PARTSSIZE/2 && npc->xm > 0 ){
			if( npc->y < gMap.length*VS*PARTSSIZE/2 && npc->ym > 0 ) npc->bits |= BITS_THROW_BLOCK;
			if( npc->y > gMap.length*VS*PARTSSIZE/2 && npc->ym < 0 ) npc->bits |= BITS_THROW_BLOCK;
		}
		if( npc->x > gMap.width*VS*PARTSSIZE/2 && npc->xm < 0 ){
			if( npc->y < gMap.length*VS*PARTSSIZE/2 && npc->ym > 0 ) npc->bits |= BITS_THROW_BLOCK;
			if( npc->y > gMap.length*VS*PARTSSIZE/2 && npc->ym < 0 ) npc->bits |= BITS_THROW_BLOCK;
		}

		if( npc->xm > 0 ) npc->direct = DIR_RIGHT;
		else              npc->direct = DIR_LEFT;
	case 33:
		npc->act_wait++;
		if( npc->act_wait/2%2 ) npc->ani_no = 3;
		else                    npc->ani_no = 8;
		if( npc->act_wait > 50 || npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) )
			npc->act_no = 20;
		break;

	case 34:// スクリューアタック
		npc->act_no   = 35;
		npc->act_wait =  0;
		npc->damage   =  4;
		npc->tgt_x    = gMC.x;
		npc->tgt_y    = gMC.y;
		deg     = GetArktan( npc->x - npc->tgt_x, npc->y - npc->tgt_y );
		npc->ym = GetSin( deg ) * 3;
		npc->xm = GetCos( deg ) * 3;
		// マップの内側へ向かうときは壁をすり抜ける
		npc->bits &= ~BITS_THROW_BLOCK;
		if( npc->x < gMap.width*VS*PARTSSIZE/2 && npc->xm > 0 ){
			if( npc->y < gMap.length*VS*PARTSSIZE/2 && npc->ym > 0 ) npc->bits |= BITS_THROW_BLOCK;
			if( npc->y > gMap.length*VS*PARTSSIZE/2 && npc->ym < 0 ) npc->bits |= BITS_THROW_BLOCK;
		}
		if( npc->x > gMap.width*VS*PARTSSIZE/2 && npc->xm < 0 ){
			if( npc->y < gMap.length*VS*PARTSSIZE/2 && npc->ym > 0 ) npc->bits |= BITS_THROW_BLOCK;
			if( npc->y > gMap.length*VS*PARTSSIZE/2 && npc->ym < 0 ) npc->bits |= BITS_THROW_BLOCK;
		}

		if( npc->xm > 0 ) npc->direct = DIR_RIGHT;
		else              npc->direct = DIR_LEFT;

	case 35:
		npc->act_wait++;

		if(      npc->act_wait > 20 && npc->shock )
			npc->act_no = 40;
		else if( npc->act_wait > 50 || npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
			npc->act_no = 20;
		}

		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 7 )
			npc->ani_no = 4;
		if( npc->act_wait %5 == 1 )
			PlaySoundObject( WAVE_HERI, 1 );

		break;

	case 40:// ダメージ吹っ飛び
		npc->act_no   = 41;
		npc->act_wait =  0;
		npc->ani_no   =  2;
		npc->damage   =  0;
		npc->bits &= ~BITS_THROW_BLOCK;
	case 41:
		npc->xm = npc->xm *7/8;
		npc->ym = npc->ym *7/8;
		// ani
		npc->act_wait++;
		if( npc->act_wait > 6 ){
			npc->act_no   = 42;
			npc->act_wait =  0;
			npc->ym = -VS*1;
			if( npc->direct == DIR_LEFT ) npc->xm =  VS*1;
			else                          npc->xm = -VS*1;
		}
		break;
	case 42:
		npc->ani_no = 9;

		// 着陸
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 43;
			npc->act_wait =  0;
			npc->ani_no   =  2;
			if( npc->x < gMC.x ) npc->direct = DIR_RIGHT;
			else                 npc->direct = DIR_LEFT;
		}
		npc->ym += VS/16;
		if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
		break;
	case 43:
		npc->act_wait++;
		if( npc->act_wait > 16 ){
			npc->act_no = 20;
		}
		break;

	case  99: // ストップ
		npc->xm = 0;
		npc->ym = 0;
		npc->ani_no = 9;
		npc->bits      &= ~BITS_BANISH_DAMAGE;
		break;

	case 100: // ノックダウン
		npc->act_no   = 101;
		npc->ani_no   =   9;
		npc->damage   =   0;
		npc->bits    &= ~BITS_BANISH_DAMAGE;
		npc->bits    |=  BITS_THROW_BLOCK;
		npc->ym       = -VS;
		npc->shock   +=  50;
		gBoss[0].ani_no++;
	case 101:
		npc->ym += VS/16;
		if( npc->y > 13*VS*PARTSSIZE + 8*VS - npc->hit.bottom ){
			npc->y = 13*VS*PARTSSIZE + 8*VS - npc->hit.bottom;
			npc->act_no = 102;
			npc->ani_no =  10;
			npc->xm     =   0;
			npc->ym     =   0;
		}
		break;
	case 102:
		break;
	}


	npc->y += npc->ym;
	if( npc->shock ) npc->x += npc->xm/2;
	else             npc->x += npc->xm;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// レッドトルネード
void ActNpc285( NPCHAR *npc )
{
	RECT rc = {232,104,248,120};
	unsigned char deg;

	// 画面外で消える
	if( npc->x < 0 || npc->x > gMap.width*PARTSSIZE*VS ){
		VanishNpChar( npc );
		return;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->tgt_x  = npc->x;
		npc->tgt_y  = npc->y;
		npc->count1 = npc->direct/8;
		npc->direct = npc->direct%8;
	case 1:
		npc->count1 += 24;
		npc->count1 = npc->count1%256;
		deg         = (unsigned char)npc->count1;
		if( npc->act_wait < 64*2 ) npc->act_wait++;

		if( npc->direct == DIR_LEFT ) npc->xm -= VS/24;
		else                          npc->xm += VS/24;

		npc->tgt_x += npc->xm;

		npc->x = npc->tgt_x + GetCos( deg ) * 4;
		npc->y = npc->tgt_y + GetSin( deg ) * 6;


		SetNpChar( 286, npc->x, npc->y, 0,0,DIR_LEFT, NULL, MAX_NPC/2 );
		break;
	}
	
	npc->rect = rc;
}

// 
// レッドトルネードの尾
void ActNpc286( NPCHAR *npc )
{
	RECT rc[] = {
		{232,120,248,136},
		{232,136,248,152},
		{232,152,248,168},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 0 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 2 ){
		npc->cond = 0;
		return;
	}
	npc->rect = rc[npc->ani_no];

}

// レッドスモーク
void ActNpc287( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,  224, 16, 240},
		{ 16,  224, 32, 240},
		{ 32,  224, 48, 240},

		{ 48,  224, 64, 240},
		{ 64,  224, 80, 240},
		{ 80,  224, 96, 240},
		{ 96,  224,112, 240},
	};

	

	if( npc->act_no == 0 ){
		npc->xm = Random( -4, 4 )*VS;
		npc->act_no = 1;
	}else{
		npc->xm = npc->xm*20/21;
		npc->ym = npc->ym*20/21;
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 6 ){
		npc->cond = 0;
	}else{
		npc->rect = rcLeft[ npc->ani_no];
	}
}


// スモークボール
void ActNpc288( NPCHAR *npc )
{
	RECT rc[] = {
		{232, 72,248, 88},
		{232, 88,248,104},

		{232,  0,256, 24},
		{232, 24,256, 48},
		{232, 48,256, 72},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
//		if( npc->x > gMC.x )
			npc->xm = -VS;
//		else                 npc->xm =  VS;
	case 1://落下
		// 上昇
		if( npc->direct == DIR_UP ){
			npc->ym -= VS/16;
			if( npc->ym < -MAX_MOVE         ) npc->ym = -MAX_MOVE;
			if( npc->flag & FLAG_HIT_TOP    ) npc->act_no = 2;
		// 下降
		}else if( npc->direct == DIR_DOWN ){
			npc->ym += VS/16;
			if( npc->ym >  MAX_MOVE         ) npc->ym =  MAX_MOVE;
			if( npc->flag & FLAG_HIT_BOTTOM ) npc->act_no = 2;
		}

		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ) npc->ani_no = 0;
		break;

	case 2://爆裂
		PlaySoundObject( WAVE_BOM, 1 );
		npc->act_no   = 3;
		npc->act_wait = 0;
		npc->bits |= BITS_THROW_BLOCK;
		npc->ym       = 0;
		if( npc->x > gMC.x ) npc->xm = -VS*2;
		else                 npc->xm =  VS*2;
		npc->view.back = VS*12;
		npc->view.front = VS*12;
		npc->view.top = VS*12;
		npc->view.bottom = VS*12;
	case 3:

		npc->ani_no++;
		if( npc->ani_no > 4 )
			npc->ani_no = 2;

		npc->act_wait++;
		if( npc->act_wait % 4 == 1 ){
			if( npc->direct == DIR_UP ) SetNpChar( 287, npc->x, npc->y, 0,  VS*2, DIR_LEFT, NULL, MAX_NPC/2 );
			else                        SetNpChar( 287, npc->x, npc->y, 0, -VS*2, DIR_LEFT, NULL, MAX_NPC/2 );	
		}
		if( npc->x < VS*16 || npc->x > gMap.width*VS*PARTSSIZE - 16*VS ) npc->cond = 0;
		break;
	}
	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}


//クリッター 召還
void ActNpc289( NPCHAR *npc )
{
	RECT rcLeft[ ] = {
		{160, 32,176, 48},// ねてる
		{176, 32,192, 48},
		{192, 32,208, 48},//ジャンプ
	};
	RECT rcRight[] = {
		{160, 48,176, 64},
		{176, 48,192, 64},
		{192, 48,208, 64},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->act_no      = 1;
		npc->ani_no      = 2;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	//
	case 1:// フェードイン
		npc->act_wait++;
		if( npc->act_wait    > 16 ){
			npc->act_no      = 10;
			npc->view.top    =  8*VS;
			npc->view.bottom =  8*VS;
			npc->damage      =  2;
			npc->bits |= BITS_BANISH_DAMAGE;
		}
		break;
	case 10:// 着陸
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 11;
			npc->ani_no   =  0;
			npc->act_wait =  0;
			npc->xm       = 0;
			if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
			else                 npc->direct = DIR_RIGHT;
		}
		break;
	case 11:// ジャンプ
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->count1++;
			if( npc->count1 > 4 ) npc->act_no = 12;
			else                  npc->act_no = 10;
			PlaySoundObject( WAVE_NPCJUMP1, 1 );
			npc->ym     = -VS*3;
			if( npc->direct == DIR_LEFT ) npc->xm = -VS;
			else                          npc->xm =  VS;
			npc->ani_no = 2;
		}
		break;
	case 12:
		npc->bits |= BITS_THROW_BLOCK;
		if( npc->y > gMap.length *VS*PARTSSIZE ){
			VanishNpChar( npc );
			return;
		}
		break;
	}

	if( npc->act_no >= 10 ) npc->ym += VS/8;
	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;
	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

	if( npc->act_no == 1 ){
		npc->rect.top    += 8 - (npc->act_wait   /2);
		npc->rect.bottom -= 8 + (npc->act_wait   /2);
		npc->view.top    =  npc->act_wait*VS/2;
		npc->view.bottom =  npc->act_wait*VS/2;
	}
}


// こうもり召還
void ActNpc290( NPCHAR *npc )
{
	RECT rcLeft[ ] = {
		{112, 32,128, 48},
		{128, 32,144, 48},
		{144, 32,160, 48},
	};
	RECT rcRight[] = {
		{112, 48,128, 64},
		{128, 48,144, 64},
		{144, 48,160, 64},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->act_no      = 1;
		npc->ani_no      = 2;
		if( npc->x > gMC.x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
	//
	case 1:// フェードイン
		npc->act_wait++;
		if( npc->act_wait    > 16 ){
			npc->act_no      = 10;
			npc->view.top    =  8*VS;
			npc->view.bottom =  8*VS;
			npc->damage      =  2;
			npc->bits |= BITS_BANISH_DAMAGE;
			npc->tgt_y = npc->y;
			npc->ym    = VS*2;
		}
		break;
	case 10:// 羽ばたく
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 2 )
			npc->ani_no = 0;
		if( npc->y < npc->tgt_y ) npc->ym += VS/8;
		else                      npc->ym -= VS/8;
		if( npc->direct == DIR_LEFT ) npc->xm -= VS/32;
		else                          npc->xm += VS/32;

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

	if( npc->act_no == 1 ){
		npc->rect.top    += 8 - (npc->act_wait   /2);
		npc->rect.bottom -= 8 + (npc->act_wait   /2);
		npc->view.top    =  npc->act_wait*VS/2;
		npc->view.bottom =  npc->act_wait*VS/2;
	}
}

// コアオプション（フェイク）
void ActNpc291( NPCHAR *npc )
{
	RECT tc[] = {
		{256, 80,320,120},
		{256,  0,320, 40},// すり抜け
	};

	switch( npc->act_no ){
	case 0:	// 待機
		npc->act_no = 20;
		if( npc->direct == DIR_RIGHT ){
			npc->bits &= ~BITS_BLOCK_MYCHAR2;
			npc->ani_no = 1;
		}
	case 20:

		break;
	}

	npc->rect = tc[npc->ani_no];
}


// 地震発生装置
void ActNpc292( NPCHAR *npc )
{
	SetQuake( 10 );
}

// 波動砲（アンデッドコア）
void ActNpc293( NPCHAR *npc )
{
	RECT rect[] = {
		{240, 200, 280, 240},
		{280, 200, 320, 240},
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:
		npc->ani_no++;
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		SetNpChar( 4, npc->x + Random( 0, 16 ) * VS, npc->y + Random( -16, 16 ) * VS, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		npc->x -= VS*8;
		if( npc->x < -32*VS ) npc->cond = 0;
		break;
	}

	npc->rect = rect[npc->ani_no];
}

// 落石配置
void ActNpc294( NPCHAR *npc )
{
	long x, y, dir;

	switch( npc->act_no ){
	case 0:
		if( gMC.x < (gMap.width-6)*VS*PARTSSIZE ){
			npc->act_no = 1;
			npc->act_wait = 0;
		}
		break;
	case 1:
		npc->act_wait++;
		if( gMC.equip & EQUIP_BOOST2 ){
			npc->x = gMC.x + 4*PARTSSIZE*VS; 
			if( npc->x < (12+14) *VS*PARTSSIZE )
				npc->x = (12+14) *VS*PARTSSIZE;
		}else{
			npc->x = gMC.x + 6*PARTSSIZE*VS;
			if( npc->x < (12+11) *VS*PARTSSIZE )
				npc->x = (12+11) *VS*PARTSSIZE;
		}
		if( npc->x > (gMap.width- 10)*VS*PARTSSIZE )
			npc->x = (gMap.width- 10)*VS*PARTSSIZE;
		if( npc->act_wait > 24 ){
			if( gMC.equip & EQUIP_BOOST2 ) x = npc->x + Random( -14, 14 ) *VS*PARTSSIZE;//Random( -7, 16 ) *VS*PARTSSIZE;
			else                           x = npc->x + Random( -11, 11 ) *VS*PARTSSIZE;//Random( -4, 16 ) *VS*PARTSSIZE;
			y = gMC.y - 14               *VS*PARTSSIZE;
			if( Random( 0, 10 )%2 ) dir = DIR_LEFT;
			else                    dir = DIR_RIGHT;
			SetNpChar( 279, x, y, 0, 0, dir, NULL, MAX_NPC/2 );
			npc->act_wait = Random( 0, 15 );
		}
		break;
	}
}

// 雲(Fall)
void ActNpc295( NPCHAR *npc )
{
	RECT rc[] = {
		{  0,  0,208, 64},//208
		{ 32, 64,144, 96},//112
		{ 32, 96,104,128},// 72
		{104, 96,144,128},// 40
	};
	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->ani_no = npc->direct%4;
		switch( npc->direct ){
		// 上へ
		case 0: npc->ym = -8*VS; npc->view.back = 104*VS; npc->view.front = 104*VS; break;
		case 1: npc->ym = -4*VS; npc->view.back =  56*VS; npc->view.front =  56*VS; break;
		case 2: npc->ym = -2*VS; npc->view.back =  32*VS; npc->view.front =  32*VS; break;
		case 3: npc->ym = -1*VS; npc->view.back =  20*VS; npc->view.front =  20*VS; break;
		// 左へ
		case 4: npc->xm = -2*VS; npc->view.back = 104*VS; npc->view.front = 104*VS; break;
		case 5: npc->xm = -1*VS; npc->view.back =  56*VS; npc->view.front =  56*VS; break;
		case 6: npc->xm = -VS/2; npc->view.back =  32*VS; npc->view.front =  32*VS; break;
		case 7: npc->xm = -VS/4; npc->view.back =  20*VS; npc->view.front =  20*VS; break;
		}
		break;
	case 1:
		npc->x += npc->xm;
		npc->y += npc->ym;
		if( npc->x < -64*VS ) npc->cond = 0;
		if( npc->y < -32*VS ) npc->cond = 0;
		break;
	}
	npc->rect = rc[npc->ani_no];
}

// 雲配置
void ActNpc296( NPCHAR *npc )
{
	long x, y, dir, pri;

	npc->act_wait++;
	if( npc->act_wait > 16 ){
		npc->act_wait = Random( 0, 16 );
		dir = Random( 0, 100 ) % 4;

		if( npc->direct == DIR_LEFT ){
			switch( dir ){
				case 0: pri = MAX_NPC/ 4 * 3; break;
				case 1: pri = MAX_NPC/ 4;     break;
				case 2: pri = MAX_NPC/ 8;     break;
				case 3: pri =          0;     break;
			}
			x = npc->x + Random( -10, 10 ) *VS*PARTSSIZE;
			y = npc->y;
			SetNpChar( 295, x, y, 0, 0, dir,     NULL, pri );
		}else{
			switch( dir ){
				case 0: pri = MAX_NPC/ 4;     break;
				case 1: pri = MAX_NPC/ 6;     break;
				case 2: pri = MAX_NPC/ 8;     break;
				case 3: pri =          0;     break;
			}
			x = npc->x;
			y = npc->y + Random(  -7,  7 ) *VS*PARTSSIZE;
			SetNpChar( 295, x, y, 0, 0, dir + 4, NULL, pri );
		}
	}
}


// くわえられスー
void ActNpc297( NPCHAR *npc )
{
	RECT rc = {112, 48,128, 64};

	npc->x = npc->pNpc->x + 16*VS;
	npc->y = npc->pNpc->y +  8*VS;
	npc->rect = rc;

}

// すっぴんドクター
void ActNpc298( NPCHAR *npc )
{
	RECT rc[] = {
		{ 72,128, 88,160},// 0
		{ 88,128,104,160},// 1

		{104,128,120,160},// 2 歩く
		{ 72,128, 88,160},
		{120,128,136,160},
		{ 72,128, 88,160},

		{104,160,120,192},// 6 背中
		{120,160,136,192},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->y -= 8*VS;
	case 1:// 待機
		npc->ani_no = 0;
		break;
	case 10: // 笑う
		npc->act_no   = 11;
		npc->ani_no   =  0;
		npc->ani_wait =  0;
		npc->count1   =  0;
	case 11:
		npc->ani_wait++;
		if( npc->ani_wait > 6 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 ){
			npc->ani_no = 0;
			npc->count1++;
			if( npc->count1 > 7 ){
				npc->ani_no = 0;
				npc->act_no = 1;
			}
		}
		break;

	case 20: // 歩く
		npc->act_no   = 21;
		npc->ani_no   =  2;
		npc->ani_wait =  0;
	case 21:
		npc->ani_wait++;
		if( npc->ani_wait > 10 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		npc->x += VS/2;
		break;
	case 30:
		npc->ani_no = 6;
		break;
	case 40: // 背中笑い
		npc->act_no   = 41;
		npc->ani_no   =  6;
		npc->ani_wait =  0;
		npc->count1   =  0;
	case 41:
		npc->ani_wait++;
		if( npc->ani_wait > 6 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 7 ){
			npc->ani_no = 6;
			npc->count1++;
			if( npc->count1 > 7 ){
				npc->ani_no = 6;
				npc->act_no = 30;
			}
		}

		break;
	}

	npc->rect = rc[npc->ani_no];
}

// ミザリー・バルログ
void ActNpc299( NPCHAR *npc )
{
	RECT rc[] = {
		{0,0,48,48},
		{48,0,96,48},
	};

	if( !npc->act_no ){
		npc->act_no = 1;
		if( npc->direct == DIR_LEFT ){
			npc->ani_no = 1;
			npc->act_wait = 25;
			npc->y -= VS*25/8;
		}else{
			npc->ani_no = 0;
			npc->act_wait = 0;
		}
	}
	npc->act_wait++;
	if( (npc->act_wait/50)%2 ){
		npc->y += VS/8;
	}else{
		npc->y -= VS/8;
	}
	npc->rect = rc[npc->ani_no];

}

