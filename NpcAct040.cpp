#include <windows.h>
#include "Sound.h"
#include "Define.h"

#include "Tags.h"
#include "forNpc.h"

// サンタ
void ActNpc040( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{  0, 32, 16, 48},//停止
		{ 16, 32, 32, 48},

		{ 32, 32, 48, 48},//歩く
		{  0, 32, 16, 48},
		{ 48, 32, 64, 48},
		{  0, 32, 16, 48},

		{ 64, 32, 80, 48},//向こう
	};

	RECT rcRight[] = {

		{  0, 48, 16, 64},//停止
		{ 16, 48, 32, 64},

		{ 32, 48, 48, 64},
		{  0, 48, 16, 64},
		{ 48, 48, 64, 64},
		{  0, 48, 16, 64},

		{ 64, 48, 80, 64},
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
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// ミミガーの村の壊れた入り口
void ActNpc041( NPCHAR *npc )
{
	RECT rect = {  0, 80, 48,112};

	if( !npc->act_no ){
		npc->act_no++;
		npc->y -= VS*16;
	}
	npc->rect = rect;
}



// スー
void ActNpc042( NPCHAR *npc )
{
	long n;
	RECT rcLeft[] = {

		{  0,  0, 16, 16},//停止
		{ 16,  0, 32, 16},

		{ 32,  0, 48, 16},//歩く
		{  0,  0, 16, 16},
		{ 48,  0, 64, 16},
		{  0,  0, 16, 16},

		{ 64,  0, 80, 16},//向こう 6

		{ 80, 32, 96, 48},//イテ！
		{ 96, 32,112, 48},//突っ伏し
		
		{128, 32,144, 48},//訴え   9
		{  0,  0, 16, 16},
	
		{112, 32,128, 48},//つかまりました… 11
		{160, 32,176, 48},//あれ…12
	};

	RECT rcRight[] = {

		{  0, 16, 16, 32},//停止
		{ 16, 16, 32, 32},

		{ 32, 16, 48, 32},
		{  0, 16, 16, 32},
		{ 48, 16, 64, 32},
		{  0, 16, 16, 32},

		{ 64, 16, 80, 32},

		{ 80, 48, 96, 64},//イテ！
		{ 96, 48,112, 64},//突っ伏し

		{128, 48,144, 64},//訴え
		{  0, 16, 16, 32},

		{112, 48,128, 64},//つかまりました… 11
		{160, 48,176, 64},//あれ…
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
		npc->xm       = 0;
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
		if( npc->direct == DIR_LEFT ) npc->xm = -VS;
		else                          npc->xm =  VS;

		break;
	case 5:// 向こう
		npc->ani_no = 6;
		npc->xm     = 0;
		break;

	
	
	case 6: //イテ！
			PlaySoundObject( WAVE_NPC_SMALL, 1 );
		npc->act_wait = 0;
		npc->act_no   = 7;
		npc->ani_no   = 7;
	case 7: 
		npc->act_wait++;
		if( npc->act_wait > 10 ){
			npc->act_no = 0;
		}
		break;
	
	case 8: //吹っ飛び(後ろ)
			PlaySoundObject( WAVE_NPC_SMALL, 1 );
		npc->act_wait = 0;
		npc->act_no   = 9;
		npc->ani_no   = 7;
		npc->ym       = -1*VS;
		if( npc->direct == DIR_LEFT ) npc->xm =  VS*2;
		else                          npc->xm = -VS*2;
	case 9:
		npc->act_wait++;
		if( npc->act_wait > 3 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 10;
			if( npc->direct == DIR_LEFT ) npc->direct = DIR_RIGHT;
			else                          npc->direct = DIR_LEFT;
		}
		break;

	
	case 10: //突っ伏し
		npc->xm     = 0;
		npc->ani_no = 8;
		break;

	
	case 11: //訴え
		npc->act_no   = 12;
		npc->act_wait = 0;
		npc->ani_no   = 9;
		npc->ani_wait = 0;
		npc->xm = 0;
	
	case 12:
		npc->ani_wait++;
		if( npc->ani_wait > 8 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 10 )
			npc->ani_no = 9;

		break;
	
	case 13: // つかまりました
		npc->ani_no = 11;
		npc->xm = 0;
		npc->ym = 0;
		npc->act_no = 14;

		// イゴールを検索
		for( n = 0; n < MAX_NPC; n++ ){
			if( gNPC[n].code_event == 501 ) break;
		}
		if( n == MAX_NPC ){
			npc->act_no = 0;
			break;
		}

		npc->pNpc = &gNPC[n];

	case 14:
		if( npc->pNpc->direct == DIR_LEFT ) npc->direct = DIR_RIGHT;
		else                                npc->direct = DIR_LEFT;
		
		if( npc->pNpc->direct == DIR_LEFT ) npc->x = npc->pNpc->x  - VS*6;
		else                                npc->x = npc->pNpc->x  + VS*6;
		npc->y = npc->pNpc->y  + VS*4;
		if( npc->pNpc->ani_no == 2 ||
			npc->pNpc->ani_no == 4 ){
			npc->y -= 1*VS;
		}
		break;
	case 15:// 赤い結晶
		npc->act_no = 16;
		SetNpChar( 257, npc->x + 128*VS, npc->y +0*VS, 0, 0, DIR_LEFT,  NULL, 0 );
		SetNpChar( 257, npc->x + 128*VS, npc->y +0*VS, 0, 0, DIR_RIGHT, NULL, MAX_NPC/4 );
		npc->xm     = 0;
		npc->ani_no = 0;
	case 16:
		gSuperXpos = npc->x - 24*VS;
		gSuperYpos = npc->y - 8*VS;
		break;

	case 17://あれ…
		npc->xm     = 0;
		npc->ani_no = 12;
		gSuperXpos = npc->x;
		gSuperYpos = npc->y - 8*VS;
		break;

	case 20:// 主人公の左側へ退避
		npc->act_no   = 21;
		npc->ani_no   =  2;
		npc->ani_wait =  0;
	case 21:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
		else                          npc->xm =  VS*2;

		if( npc->x < gMC.x - 8*VS ){
			npc->direct = DIR_RIGHT;
			npc->act_no =         0;
		}

		break;
	case 30: //走る
		npc->act_no   = 31;
		npc->ani_no   = 2;
		npc->ani_wait = 0;
	case 31:
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 2;
		if( npc->direct == DIR_LEFT ) npc->xm = -VS*2;
		else                          npc->xm =  VS*2;

		break;
	case 40: //ジャンプ！
		npc->act_no   = 41;
		npc->ani_no   = 9;
		npc->ym       = -VS*2;
	case 41:
		break;

	}




	if( npc->act_no != 14 ){
		npc->ym += VS/8;

		if( npc->xm >  2*VS )npc->xm =  2*VS;
		if( npc->xm < -2*VS )npc->xm = -2*VS;
		if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

//黒板／開発テーブル
void ActNpc043( NPCHAR *npc )
{
	RECT rcLeft  = {128, 80, 168, 112};
	RECT rcRight = {168, 80, 208, 112};

	switch( npc->act_no ){
	case 0:
		npc->act_no  =     1;
		npc->y      -= VS*16;
	case 1:
		break;

	}
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft;
	else                          npc->rect = rcRight;

}

//サンバッグ
void ActNpc044( NPCHAR *npc )
{
	RECT rcRight[] = {
		{  0,  0, 32, 32},
		{ 32,  0, 64, 32},
		{ 64,  0, 96, 32},
	};
	RECT rcLeft[] = {
		{  0,  0, 32, 32},
		{ 96,  0,128, 32},
		{128,  0,160, 32},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
	case 1:// 待機
		npc->ani_no = 0;
			if( npc->direct == DIR_LEFT ) npc->act_no = 8;
			else                          npc->act_no = 2;
/*		if( npc->x - 6*PARTSSIZE*VS < gMC.x &&
			npc->x + 6*PARTSSIZE*VS > gMC.x &&
			npc->y - 6*PARTSSIZE*VS < gMC.y &&
			npc->y + 6*PARTSSIZE*VS > gMC.y ){
			if( npc->direct == DIR_LEFT ) npc->act_no = 8;
			else                          npc->act_no = 2;
		}
		if( npc->shock ){
			if( npc->direct == DIR_LEFT ) npc->act_no = 8;
			else                          npc->act_no = 2;
		}
		break;
*/
	//右---------------
	case 2:// ↓G →
		npc->ym += VS/16;
		if( npc->ym > 0 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym = -VS/2;
			npc->xm += VS/2;
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->act_no = 3;
		}
		break;
	case 3:// →G ↑
		npc->xm += VS/16;
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ){
			npc->xm = -VS/2;
			npc->ym -= VS/2;
		}
		if( npc->flag & FLAG_HIT_TOP ){
			npc->act_no = 4;
		}
		break;
	case 4:// ↑G ←
		npc->ym -= VS/16;
		if( npc->ym < 0 && npc->flag & FLAG_HIT_TOP ){
			npc->ym =  VS/2;
			npc->xm -= VS/2;
		}
		if( npc->flag & FLAG_HIT_LEFT ){
			npc->act_no = 5;
		}
		break;
	case 5:// ←G ↓
		npc->xm -= VS/16;
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT ){
			npc->xm =  VS/2;
			npc->ym += VS/2;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 2;
		}
		break;
	//←---------------
	case 6:// ↓G ←
		npc->ym += VS/16;
		if( npc->ym > 0 && npc->flag & FLAG_HIT_BOTTOM ){
			npc->ym = -VS/2;
			npc->xm -= VS/2;
		}
		if( npc->flag & FLAG_HIT_LEFT ){
			npc->act_no = 7;
		}
		break;
	case 7:// ←G ↑
		npc->xm -= VS/16;
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT ){
			npc->xm =  VS/2;
			npc->ym -= VS/2;
		}
		if( npc->flag & FLAG_HIT_TOP ){
			npc->act_no = 8;
		}
		break;
	case 8:// ↑G →
		npc->ym -= VS/16;
		if( npc->ym < 0 && npc->flag & FLAG_HIT_TOP ){
			npc->ym =  VS/2;
			npc->xm += VS/2;
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->act_no = 9;
		}
		break;
	case 9:// →G ↓
		npc->xm += VS/16;
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT ){
			npc->xm = -VS/2;
			npc->ym += VS/2;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no = 6;
		}
		break;
	
	}



	if( npc->life <= 100 ){
		long i;
		for( i = 0; i < 10; i++ ){
			SetNpChar( 45, npc->x, npc->y, 0, 0, DIR_LEFT, NULL, MAX_NPC/2 );
		}
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  8 );
		PlaySoundObject( WAVE_BUNRET, 1 );
		npc->cond = 0;
	}

	//移動量
	if( npc->xm >  VS/1 )npc->xm =  VS/1;
	if( npc->xm < -VS/1 )npc->xm = -VS/1;
	if( npc->ym >  VS/1 )npc->ym =  VS/1;
	if( npc->ym < -VS/1 )npc->ym = -VS/1;
	if( npc->shock ){
		npc->x += npc->xm/2;
		npc->y += npc->ym/2;
	}else{
		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	// アニメ	
	if( 2 <= npc->act_no && npc->act_no <= 9 ){ 
		npc->ani_no++;
		if( npc->ani_no > 2 )npc->ani_no = 1;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

//サンビッツ
void ActNpc045( NPCHAR *npc )
{
	RECT rect[] = {
		{  0, 32, 16, 48},
		{ 16, 32, 32, 48},
		{ 32, 32, 48, 48},
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no = 2;

		if( Random( 0, 1 ) ){
			npc->xm = Random( -VS, -VS/2 );
		}else{
			npc->xm = Random( VS/2, VS );
		}
		if( Random( 0, 1 ) ){
			npc->ym = Random( -VS, -VS/2 );
		}else{
			npc->ym = Random( VS/2, VS );
		}

		npc->xm2 = npc->xm;
		npc->ym2 = npc->ym;
	case 1:// 待機

	case 2:// 迫る
		npc->ani_no++;
		if( npc->ani_no > 2 )npc->ani_no = 1;
		break;

	}


	// 跳ね返り
	if( npc->xm2 < 0 && npc->flag & FLAG_HIT_LEFT   ){
		npc->xm2 = npc->xm2 * (-1);
	}
	if( npc->xm2 > 0 && npc->flag & FLAG_HIT_RIGHT  ){
		npc->xm2 = npc->xm2 * (-1);
	}
	if( npc->ym2 < 0 && npc->flag & FLAG_HIT_TOP    ){
		npc->ym2 = npc->ym2 * (-1);
	}
	if( npc->ym2 > 0 && npc->flag & FLAG_HIT_BOTTOM ){
		npc->ym2 = npc->ym2 * (-1);
	}

	if( npc->xm2 >  1*VS )npc->xm2 =  1*VS;
	if( npc->xm2 < -1*VS )npc->xm2 = -1*VS;
	if( npc->ym2 >  1*VS )npc->ym2 =  1*VS;
	if( npc->ym2 < -1*VS )npc->ym2 = -1*VS;

	if( npc->shock ){
		npc->x += npc->xm2/2;
		npc->y += npc->ym2/2;
	}else{
		npc->x += npc->xm2;
		npc->y += npc->ym2;
	}

	npc->rect = rect[npc->ani_no];


}

// イベントキーパー
void ActNpc046( NPCHAR *npc )
{
	RECT rect = {0,0,16,16};

	npc->bits |= BITS_EVENT_HIT;

	//←→
	if( npc->direct == DIR_LEFT ){
		if( npc->x < gMC.x ) npc->x += MAX_MOVE;
		else                 npc->x -= MAX_MOVE;
		
	//↑↓
	}else{
		if( npc->y < gMC.y ) npc->y += MAX_MOVE;
		else                 npc->y -= MAX_MOVE;
	
	}

	npc->rect = rect;

}

// わに
void ActNpc047( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0://マイキャラ待ち
		npc->ani_no = 0;
		npc->act_no = 1;
		npc->act_wait = 0;
		npc->tgt_y = npc->y;
		npc->bits &= ~BITS_BANISH_DAMAGE;//弾は効かない
		npc->bits &= ~BITS_BLOCK_BULLET;//弾は効かない
		npc->bits &= ~BITS_BLOCK_MYCHAR;//マイキャラすり抜け
		npc->bits &= ~BITS_THROW_BLOCK;
	case 1://おっかけ待機
		if( gMC.x > npc->x - 8*VS  && gMC.x < npc->x + 8 *VS &&
			gMC.y > npc->y - 0*VS && gMC.y < npc->y+8*VS ){
			npc->act_no   = 2;
			npc->act_wait = 0;
			PlaySoundObject( WAVE_BAIT, 1 );
		}
		if( npc->x < gMC.x )npc->x += 2*VS;
		if( npc->x > gMC.x )npc->x -= 2*VS;

		break;
	case 2://食らいつき
		npc->ani_wait++;
		if( npc->ani_wait > 3 ){
			npc->ani_no++;
			npc->ani_wait = 0;
		}
		if(npc->ani_no == 3 ) npc->damage = 10;
		if(npc->ani_no == 4 ){
			npc->bits |= BITS_BANISH_DAMAGE;//弾が効くようになる
//			npc->bits |= BITS_BLOCK_BULLET;//弾が効くようになる
			npc->act_no = 3;
			npc->act_wait = 0;
		}
		break;
	case 3://・・・
		npc->bits |= BITS_BLOCK_MYCHAR;//マイキャラブロック
		npc->damage = 0;
		npc->act_wait++;
		if( npc->shock ){//|| npc->act_wait > 100){
			npc->act_no = 4;
			npc->act_wait = 0;
		}
		break;
	case 4://退散
		npc->bits |= BITS_THROW_BLOCK;
		npc->y += 1*VS;
		npc->act_wait++;
		if( npc->act_wait == 32 ){
			npc->bits &= ~BITS_BLOCK_MYCHAR;//マイキャラブロック
			npc->bits &= ~BITS_BANISH_DAMAGE;
			npc->act_no = 5;
			npc->act_wait = 0;
		}
		break;
	case 5://地中待機

		if( npc->act_wait < 100 ){
			npc->act_wait++;
		}else{
			npc->y = npc->tgt_y;
			npc->ani_no = 0;
			npc->act_no = 0;
		}
		break;
	}

	//RECT
	RECT rect[] = {
		{  0, 48, 48, 80},
		{ 48, 48, 96, 80},
		{ 96, 48,144, 80},
		{144, 48,192, 80},
		{192, 48,240, 80},
	};
	npc->rect = rect[npc->ani_no];

}

//オメガクラッカー
void ActNpc048( NPCHAR *npc )
{
	//地面に着地
	if(       npc->flag&FLAG_HIT_LEFT  && npc->xm < 0 ){
		npc->xm *= -1;
	}else if( npc->flag&FLAG_HIT_RIGHT && npc->xm > 0 ){
		npc->xm *= -1;
	}else if( npc->flag & FLAG_HIT_BOTTOM ){
		npc->count1++;
		if( npc->count1 > 2 || npc->direct == DIR_RIGHT ){
			VanishNpChar( npc );
			SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		}else{
			npc->ym = -VS/2;
		}
	}

	if( npc->direct == DIR_RIGHT ){
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->bits |=  BITS_BLOCK_BULLET;
	}

	// grav
	npc->ym += VS/96;

	npc->y += npc->ym;
	npc->x += npc->xm;

	//アニメーション------------------
	RECT rcLeft[] = {
		{288, 88,304,104},
		{304, 88,320,104},
	};
	RECT rcRight[] = {
		{288,104,304,120},
		{304,104,320,120},
	};

	npc->ani_wait++;
	if( npc->ani_wait > 2 ){
		npc->ani_wait = 0;
		npc->ani_no++;
		if( npc->ani_no > 1)npc->ani_no = 0;
	}

	npc->act_wait++;
	if( npc->act_wait > 15*50 ){
		SetCaret( npc->x, npc->y, CARET_SMOKE, DIR_LEFT );
		npc->cond = 0;
	}

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}

// ボーンヘッド
void ActNpc049( NPCHAR *npc )
{
	unsigned char deg;
	long xm, ym;


	if( npc->act_no >= 10 && npc->pNpc->code_char == 3 ){
		npc->act_no = 3;
		npc->xm     = 0;
		npc->ym     = 0;
		npc->count2 = 1;
	}
	
	//壁でターン
	if( npc->flag & FLAG_HIT_LEFT ){
		npc->direct = DIR_RIGHT;
		npc->xm = VS/2;
	}
	if( npc->flag & FLAG_HIT_RIGHT ){
		npc->direct = DIR_LEFT;
		npc->xm = -VS/2;
	}

	switch( npc->act_no ){
	case 0:
		if( npc->pNpc ) npc->act_no = 10;
		else            npc->act_no =  1;
	case 1://地面
		npc->act_wait++;
		if( npc->act_wait > 3 ){
			npc->ym     = -VS * 2;
			npc->act_no = 3;
			npc->ani_no = 2;
			if( npc->count2 ){
				if( npc->direct == DIR_LEFT ) npc->xm = -VS;
				else                          npc->xm =  VS;
			}else{
				if( npc->direct == DIR_LEFT ) npc->xm = -VS/2;
				else                          npc->xm =  VS/2;
			}
		}
		npc->ani_no = 1;
		break;
	case 2:// 点滅
		break;
	case 3://空中
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no   = 1;
			npc->act_wait = 0;
			npc->xm       = 0;
		}
/*		if( npc->ym > 0 && npc->count1 == 1 ){
			npc->count1 = 2;
			deg = GetArktan( npc->x - gMC.x, npc->y+4*VS - gMC.y );

			ym = GetSin( deg ) * 2;
			xm = GetCos( deg ) * 2;
			SetNpChar( 50, npc->x, npc->y, xm, ym, DIR_LEFT, NULL );
			PlaySoundObject( WAVE_POP, 1 );
		}
*/		//落下時に閉じる
		if( npc->flag & FLAG_HIT_BOTTOM || npc->ym > 0 ) npc->ani_no = 1;
		else                                             npc->ani_no = 2;
		break;

	//クロウs
	case 10:


		if( npc->count1 < 50 ) npc->count1++;
		else if(
			npc->x - 8*PARTSSIZE*VS < gMC.x &&
			npc->x + 8*PARTSSIZE*VS > gMC.x &&
			npc->y - 6*PARTSSIZE*VS < gMC.y &&
			npc->y + 6*PARTSSIZE*VS > gMC.y ){
			npc->act_no   = 11;
			npc->act_wait =  0;
			npc->ani_no   =  2;
		}
		break;
	case 11:// ボーン撃ち
		npc->act_wait++;
		if( npc->act_wait == 30 ||  npc->act_wait == 35 ){
			deg = GetArktan( npc->x - gMC.x, npc->y+4*VS - gMC.y );

			ym = GetSin( deg ) * 2;
			xm = GetCos( deg ) * 2;
			SetNpChar( 50, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
		}
		if( npc->act_wait > 50 ){
			npc->count1 =  0;
			npc->act_no = 10;
			npc->ani_no =  1;
		}

		break;
	}

	// クロウs
	if( npc->act_no >= 10 ){
		npc->x = npc->pNpc->x;
		npc->y = npc->pNpc->y + 16*VS;
		npc->direct = npc->pNpc->direct;
		npc->pNpc->count1--;
	}

	//Gravity
	npc->ym += VS/8;

	if( npc->ym > MAX_MOVE )npc->ym = MAX_MOVE;

	npc->x += npc->xm;
	npc->y += npc->ym;



	RECT rcLeft[] = {
		{  0, 80, 32,104},
		{ 32, 80, 64,104},
		{ 64, 80, 96,104},
	};
	RECT rcRight[] = {
		{  0,104, 32,128},
		{ 32,104, 64,128},
		{ 64,104, 96,128},
	};


	//アニメーション------------------
	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ボーン
void ActNpc050( NPCHAR *npc )
{

	switch( npc->act_no ){
	case 0:
		if( npc->direct == DIR_RIGHT ){
			npc->act_no = 2;
		}

	case 1:// 横直進
		npc->x += npc->xm;
		npc->y += npc->ym;
		if( npc->flag & FLAG_HIT_LEFT ){
			npc->act_no =   2;
			npc->xm     =  VS;
			npc->count1++;
		}
		if( npc->flag & FLAG_HIT_RIGHT ){
			npc->act_no =   2;
			npc->xm     =  -VS;
			npc->count1++;
		}
		if( npc->flag & FLAG_HIT_TOP ){
			npc->act_no =   2;
			npc->ym     =  VS;
			npc->count1++;
		}
		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->act_no =   2;
			npc->ym     =  -VS;
			npc->count1++;
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


	// アニメーション-----------------
	RECT rect[] = {
		{ 48, 32, 64, 48},
		{ 64, 32, 80, 48},
		{ 80, 32, 96, 48},
		{ 96, 32,112, 48},
	};

	if( npc->direct == DIR_LEFT ){

		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 0;

	}else{

		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no--;
		}
		if( npc->ani_no < 0 )
			npc->ani_no = 3;

	}

	npc->rect = rect[npc->ani_no];

}


// クロウ
void ActNpc051( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0://初期化
		if(
			npc->x - 20*PARTSSIZE*VS < gMC.x &&
			npc->x + 20*PARTSSIZE*VS > gMC.x &&
			npc->y - 20*PARTSSIZE*VS < gMC.y &&
			npc->y + 20*PARTSSIZE*VS > gMC.y ){
			
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			if( npc->direct == DIR_LEFT ) npc->ym = VS*2;
			else                          npc->ym = 2*VS;
			npc->act_no = 1;
			SetNpChar( 49, 0, 0, 0, 0, DIR_LEFT, npc, 0 );
		}else{
			break;
		}

	case 1://滞空
		//振り向き
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;
		if( npc->tgt_y < npc->y ) npc->ym -= VS/48;
		if( npc->tgt_y > npc->y ) npc->ym += VS/48;

		if( npc->ym >  1*VS )npc->ym =  1*VS;
		if( npc->ym < -1*VS )npc->ym = -1*VS;


		if( npc->count1 < 10 ){
			npc->count1++;
		}else{
			npc->act_no = 2;
		}
		break;
	case 2:
		//振り向き
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;

		if( npc->y > gMC.y + VS*16*2 ){
			//逃げ
			if( gMC.x < npc->x ) npc->xm += VS/32;
			if( gMC.x > npc->x ) npc->xm -= VS/32;
		}else{
			if( gMC.x < npc->x ) npc->xm -= VS/32;
			if( gMC.x > npc->x ) npc->xm += VS/32;
		}
		if( gMC.y < npc->y ) npc->ym -= VS/32;
		if( gMC.y > npc->y ) npc->ym += VS/32;

		if( npc->shock ){
			npc->ym += VS/16;
			npc->xm  =  0;
		}

		break;

	}

	
	if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT   ) npc->xm =  VS/2;
	if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT  ) npc->xm = -VS/2;
	if( npc->ym < 0 && npc->flag & FLAG_HIT_TOP    ) npc->ym =  VS/2;
	if( npc->ym > 0 && npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS/2;


	if( npc->xm >  2*VS )npc->xm =  2*VS;
	if( npc->xm < -2*VS )npc->xm = -2*VS;
	if( npc->ym >  1*VS )npc->ym =  1*VS;
	if( npc->ym < -1*VS )npc->ym = -1*VS;
	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{ 96, 80, 128, 112},
		{128, 80, 160, 112},
		{160, 80, 192, 112},
		{192, 80, 224, 112},
		{224, 80, 256, 112},
	};
	RECT rect_right[] = {
		{ 96,112, 128, 144},
		{128,112, 160, 144},
		{160,112, 192, 144},
		{192,112, 224, 144},
		{224,112, 256, 144},
	};

	if( npc->shock  ){
		npc->ani_no = 4;
	}else if( npc->act_no == 2 && npc->y < gMC.y - 16*VS*2 ){
		npc->ani_no = 0;
	}else if( npc->act_no ){
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )npc->ani_no = 0;
	}
	if( npc->direct == DIR_LEFT ) npc->rect = rect_left[npc->ani_no];
	else                          npc->rect = rect_right[npc->ani_no];
}

// 巡回ロボ（停止）
void ActNpc052( NPCHAR *npc )
{
	RECT rect = {240, 96,256,112};
	npc->rect = rect;
//	npc->x += VS;
}

// ボーンフット
void ActNpc053( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,128, 24,144},
		{ 24,128, 48,144},
	};
	RECT rcRight[] = {
		{ 48,128, 72,144},
		{ 72,128, 96,144},
	};
	unsigned char deg;

	if( npc->pNpc->code_char == 3 ){
		VanishNpChar( npc );
		SetDestroyNpChar( npc->x, npc->y, npc->view.back,  4 );
		return;
	}

	switch( npc->act_no ){
	case 0:
		npc->act_no = 1;
		npc->count1 = 10;
	case 1:


		if( npc->direct == DIR_LEFT && npc->flag&(FLAG_HIT_RDOWN) ){
			npc->pNpc->y  -= 2*VS;
			npc->pNpc->ym -= VS/2;
	
		}
		if( npc->direct == DIR_RIGHT && npc->flag&(FLAG_HIT_LDOWN) ){
			npc->pNpc->y  -= 2*VS;
			npc->pNpc->ym -= VS/2;
	
		}

		if( npc->flag & FLAG_HIT_BOTTOM ){
			npc->pNpc->y  -= 2*VS;
			npc->pNpc->ym -= VS/2;

			if( npc->pNpc->direct == DIR_LEFT )npc->pNpc->xm -= VS/4;
			else                               npc->pNpc->xm += VS/4;
		
		}

		

		deg = (unsigned char)npc->xm + (unsigned char)npc->pNpc->count2;
		npc->x = npc->pNpc->x + GetCos( deg ) * npc->count1;	
		npc->y = npc->pNpc->y + GetSin( deg ) * npc->count1;// + 2*VS;// - 16*VS;	
		npc->direct = npc->pNpc->direct;

		break;
	}

	npc->direct = npc->pNpc->direct;
	if( deg >= 20 && deg <= 108 ) npc->ani_no = 0;
	else                          npc->ani_no = 1;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];

}

// ボーンステップ
void ActNpc054( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0, 80, 32,104},
		{ 32, 80, 64,104},
		{ 64, 80, 96,104},
	};
	RECT rcRight[] = {
		{  0,104, 32,128},
		{ 32,104, 64,128},
		{ 64,104, 96,128},
	};
	unsigned char deg;

	switch( npc->act_no ){
	case 0:
		SetNpChar( 53, 0, 0,   0, 0, npc->direct, npc, MAX_NPC/2 );
		SetNpChar( 53, 0, 0, 128, 0, npc->direct, npc, 0         );
		npc->act_no = 1;
		npc->ani_no = 1;
	case 1:
		// 足を回す
		deg = (unsigned char)npc->count2;
		if( npc->direct == DIR_LEFT ) deg-=6;
		else                          deg+=6;
		npc->count2 = (long)deg;
		
		// 地面摩擦
		if( (npc->flag & FLAG_HIT_BOTTOM) ){
			npc->xm = npc->xm*3/4;
			npc->act_wait++;
			if( npc->act_wait > 60 ){
				npc->act_no   = 2;
				npc->act_wait = 0;
			}
		}else{
			npc->act_wait = 0;
		}
		// 方向転換
		if( npc->direct == DIR_LEFT  && npc->flag & FLAG_HIT_LEFT  ){
			npc->count1++;
			if( npc->count1 > 8 ){
				npc->direct = DIR_RIGHT;
				npc->xm *= -1;
			}
		}else if( npc->direct == DIR_RIGHT && npc->flag & FLAG_HIT_RIGHT ){
			npc->count1++;
			if( npc->count1 > 8 ){
				npc->direct = DIR_LEFT;
				npc->xm *= -1;
			}
		}else{
			npc->count1 = 0;
		}
		break;
	// 自爆
	case 2:
		npc->act_wait++;
		npc->shock += (unsigned char)npc->act_wait;
		if( npc->act_wait > 50 ){
			VanishNpChar( npc );
			SetDestroyNpChar( npc->x, npc->y, npc->view.back, 8 );
			PlaySoundObject( WAVE_BUNRET, 1 );
		}
		break;

	}



	npc->ym += VS/4;

	if( npc->xm >  MAX_MOVE/2   ) npc->xm =  MAX_MOVE/2;
	if( npc->xm < -MAX_MOVE/2   ) npc->xm = -MAX_MOVE/2;
	if( npc->ym >  MAX_MOVE/2 ) npc->ym =  MAX_MOVE/2;
	if( npc->ym < -MAX_MOVE/2 ) npc->ym = -MAX_MOVE/2;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[ npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}


// 坂本カズマ
void ActNpc055( NPCHAR *npc )
{
	RECT rcLeft[] = {

		{192,192,208,216},//停止

		{208,192,224,216},//1歩く
		{192,192,208,216},
		{224,192,240,216},
		{192,192,208,216},

		{240,192,256,216},//5向こう
	};

	RECT rcRight[] = {

		{192,216,208,240},//停止

		{208,216,224,240},//1歩く
		{192,216,208,240},
		{224,216,240,240},
		{192,216,208,240},

		{240,216,256,240},//5向こう
	};

	switch( npc->act_no ){
	case 0:
		npc->act_no   = 1;
		npc->ani_no   = 0;
		npc->ani_wait = 0;
	case 1://待機
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
		if( npc->direct == DIR_LEFT ) npc->x -= VS;
		else                          npc->x += VS;

		break;
	case 5:// 向こう
		npc->ani_no = 5;
		break;
	}
	npc->ym += VS/16;
	if( npc->ym > MAX_MOVE ) npc->ym = MAX_MOVE;
	npc->y += npc->ym;

	if( npc->direct == DIR_LEFT ) npc->rect = rcLeft[npc->ani_no];
	else                          npc->rect = rcRight[npc->ani_no];
}






// ビートル２
void ActNpc056( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{  0,144, 16,160},//停止
		{ 16,144, 32,160},
		{ 32,144, 48,160},
	};
	RECT rcRight[] = {
		{  0,160, 16,176},//停止
		{ 16,160, 32,176},
		{ 32,160, 48,176},
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

		if( npc->x < gMC.x &&
			npc->x > gMC.x - 16*PARTSSIZE*VS &&
			npc->y < gMC.y + 8*VS &&
			npc->y > gMC.y - 8*VS ){

			npc->act_no   = 3;
			npc->ani_wait = 0;
			npc->ani_no   = 1;
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
		if( npc->x < gMC.x + 16*PARTSSIZE*VS&&
			npc->x > gMC.x  &&
			npc->y < gMC.y + 8*VS &&
			npc->y > gMC.y - 8*VS ){

			npc->act_no   = 1;
			npc->ani_wait = 0;
			npc->ani_no   = 1;
		}
		break;
		
	}


	if( npc->direct == DIR_LEFT )
		npc->rect = rcLeft[npc->ani_no];
	else 
		npc->rect = rcRight[npc->ani_no];
}







//クロウ突進
void ActNpc057( NPCHAR *npc )
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
		npc->ani_no   = Random(0,1);
		npc->ani_wait = Random(0,4);
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


		if( npc->shock ){
			npc->act_no   = 2;
			npc->act_wait = 0;
//			npc->bits     |= BITS_THROW_BLOCK;
//			npc->ani_no   = 3;
//			npc->ani_wait = 0;
			if( npc->direct == DIR_RIGHT ) npc->xm = -VS;
			else                           npc->xm =  VS;
			npc->ym = 0;
		}
		
		break;

	case 2://突進
		//振り向き
		if( gMC.x < npc->x ) npc->direct = DIR_LEFT;
		else                 npc->direct = DIR_RIGHT;

		if( npc->y > gMC.y + VS*16*3 ){
			//逃げ
			if( gMC.x < npc->x ) npc->xm += VS/32;
			if( gMC.x > npc->x ) npc->xm -= VS/32;
		}else{
			if( gMC.x < npc->x ) npc->xm -= VS/32;
			if( gMC.x > npc->x ) npc->xm += VS/32;
		}
		if( gMC.y < npc->y ) npc->ym -= VS/32;
		if( gMC.y > npc->y ) npc->ym += VS/32;

		if( npc->shock ){
			npc->ym += VS/16;
			npc->xm  =  0;
		}


/*		npc->act_wait++;
		if( npc->flag && FLAG_HIT_BLOCK ){
			SetDestroyNpChar( npc->x, npc->y, npc->view.back,  4 );
			PlaySoundObject( WAVE_DESTROY_MIDDLE, 1 );
			npc->cond = 0;
			break;
		}
		if( gMC.y < npc->y ) npc->ym -= VS/32;
		if( gMC.y > npc->y ) npc->ym += VS/32;
		if( npc->direct == DIR_RIGHT ) npc->xm += VS/8;
		else                           npc->xm -= VS/8;
*/
		if( npc->xm < 0 && npc->flag & FLAG_HIT_LEFT   ) npc->xm =  VS;
		if( npc->xm > 0 && npc->flag & FLAG_HIT_RIGHT  ) npc->xm = -VS;
		if( npc->ym < 0 && npc->flag & FLAG_HIT_TOP    ) npc->ym =  VS;
		if( npc->ym > 0 && npc->flag & FLAG_HIT_BOTTOM ) npc->ym = -VS;
		if( npc->xm >  MAX_MOVE )npc->xm =  MAX_MOVE;
		if( npc->xm < -MAX_MOVE )npc->xm = -MAX_MOVE;
		if( npc->ym >  MAX_MOVE )npc->ym =  MAX_MOVE;
		if( npc->ym < -MAX_MOVE )npc->ym = -MAX_MOVE;
		break;

	}
	npc->x += npc->xm;
	npc->y += npc->ym;

	//アニメーション------------------
	RECT rect_left[] = {
		{ 96, 80, 128, 112},
		{128, 80, 160, 112},
		{160, 80, 192, 112},
		{192, 80, 224, 112},
		{224, 80, 256, 112},
	};
	RECT rect_right[] = {
		{ 96,112, 128, 144},
		{128,112, 160, 144},
		{160,112, 192, 144},
		{192,112, 224, 144},
		{224,112, 256, 144},
	};


/*	if( npc->act_no == 2 && npc->act_wait > 20 ){
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )npc->ani_no = 2;
	//滞空
	}else*/
	if( npc->shock ){
		npc->ani_no = 4;
	}else {
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )npc->ani_no = 0;
	}
	if( npc->direct == DIR_LEFT )
		npc->rect = rect_left[npc->ani_no];
	else 
		npc->rect = rect_right[npc->ani_no];
}



// パゴット=バス
void ActNpc058( NPCHAR *npc )
{
	RECT rcLeft[] = {
		{192,  0,216, 24},
		{216,  0,240, 24},
		{240,  0,264, 24},
	};

	RECT rcRight[] = {
		{192, 24,216, 48},
		{216, 24,240, 48},
		{240, 24,264, 48},
	};


	switch( npc->act_no ){
	case 0:
		if(	gMC.x < npc->x + 1*PARTSSIZE*VS &&
			gMC.x > npc->x - 1*PARTSSIZE*VS ){

			npc->bits    |= BITS_BANISH_DAMAGE;
			npc->ym       = -VS/2;
			npc->tgt_x    = npc->x;//最初の座標を記憶
			npc->tgt_y    = npc->y;
			npc->act_no   = 1;
			npc->act_wait = 0;
			npc->count1   = npc->direct; // 方向を保存
			npc->count2   = 0;
			npc->damage   = 6;
			if( npc->direct == DIR_LEFT ){
				npc->x  = gMC.x + 16*PARTSSIZE*VS;
				npc->xm = -MAX_MOVE/2; 
			}else{
				npc->x  = gMC.x - 16*PARTSSIZE*VS;
				npc->xm = MAX_MOVE/2; 
			}
			return;
		}else{
			npc->rect.right = 0;
			npc->damage     = 0;
			npc->xm         = 0;
			npc->ym         = 0;
			npc->bits      &= ~BITS_BANISH_DAMAGE;
			return;
		}
		break;
	case 1:
		//向き
		if( npc->x > gMC.x ){
			npc->direct = DIR_LEFT;
			npc->xm -= 16;
		}else{
			npc->direct =   DIR_RIGHT;
			npc->xm += 16;
		}


		if( npc->flag & FLAG_HIT_LEFT  ) npc->xm =  VS;
		if( npc->flag & FLAG_HIT_RIGHT ) npc->xm = -VS;


		if( npc->y < npc->tgt_y ) npc->ym += 8;
		else                      npc->ym -= 8;

		// max move
		if( npc->xm >  MAX_MOVE/2 ) npc->xm =  MAX_MOVE/2;
		if( npc->xm < -MAX_MOVE/2 ) npc->xm = -MAX_MOVE/2;
		if( npc->ym >  VS/2       ) npc->ym =  VS/2;
		if( npc->ym < -VS/2       ) npc->ym = -VS/2;

		// damage move
		if( npc->shock ){
			npc->x += npc->xm/2;
			npc->y += npc->ym/2;
		}else{
			npc->x += npc->xm;
			npc->y += npc->ym;
		}

		if(	gMC.x > npc->x + 25*PARTSSIZE*VS ||
			gMC.x < npc->x - 25*PARTSSIZE*VS ){
			npc->act_no     = 0;
			npc->xm         = 0;
			npc->direct     = npc->count1;
			npc->x          = npc->tgt_x;//元の座標へ
			npc->rect.right = 0;//表示もしない
			npc->damage     = 0;//ダメージ無し
			return;
		}



		
		break;
	}

	if( npc->act_no ){

		unsigned char deg;
		long xm;
		long ym;



		if( npc->act_wait <  150 )npc->act_wait++;
		if(	npc->act_wait == 150 ){
			npc->count2++;
			if( npc->count2%8 == 0 &&
				npc->x < gMC.x + 10*PARTSSIZE*VS &&
				npc->x > gMC.x - 10*PARTSSIZE*VS ){

			
				deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
				deg += (unsigned char)Random( -6, 6 );

				ym = GetSin( deg ) * 2;
				xm = GetCos( deg ) * 2;
				SetNpChar( 84, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
				PlaySoundObject( WAVE_POP, 1 );
			}
			if( npc->count2 > 8 ){
				npc->act_wait = 0;
				npc->count2   = 0;
			}
		}
	}

	npc->ani_wait++;
	if( npc->ani_wait > 1 ){
		npc->ani_wait = 0;
		npc->ani_no++;
	}
	if( npc->ani_no > 1 )npc->ani_no = 0;
	// 点滅
	if( npc->act_wait > 120 &&
		npc->act_wait/2%2 == 1 &&
		npc->ani_no == 1 ){
		npc->ani_no = 2;
	}

	if( npc->direct == DIR_LEFT)npc->rect = rcLeft[ npc->ani_no];
	else                        npc->rect = rcRight[npc->ani_no]; 


}


/*
// 闇鎌
void ActNpc058( NPCHAR *npc )
{
	//count2 : 向き
	//count1 : アニメーションNo
	unsigned char deg;

	RECT rcLeft[] = {
		{  0,  0, 16, 16},
		{ 16,  0, 32, 16},
		{ 32,  0, 48, 16},
		{ 48,  0, 64, 16},
		{ 64,  0, 80, 16},
		{ 80,  0, 96, 16},
		{ 96,  0,112, 16},
		{112,  0,128, 16},

		{  0, 16, 16, 32},
		{ 16, 16, 32, 32},
		{ 32, 16, 48, 32},
		{ 48, 16, 64, 32},
		{ 64, 16, 80, 32},
		{ 80, 16, 96, 32},
		{ 96, 16,112, 32},
		{112, 16,128, 32},
	
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->act_no = 1;
		npc->ani_no = 6;
	case 1://待機
		if( npc->x > gMC.x )npc->direct = DIR_LEFT;
		else                npc->direct = DIR_RIGHT;
		if( npc->x - PARTSSIZE*8*VS < gMC.x &&
			npc->x + PARTSSIZE*8*VS > gMC.x &&
			npc->y - PARTSSIZE*5*VS < gMC.y &&
			npc->y + PARTSSIZE*5*VS > gMC.y ){

			npc->act_no   = 3;
			npc->count2   = 6;
		}
		if( npc->shock ){
			npc->act_no   = 3;
			npc->count2   = 6;
		}
		break;


	case 3:// ストップモーション
		npc->xm = 0;
		npc->ym = 0;
		if( npc->act_wait < 40 ){
			npc->act_wait++;
		}else{
			npc->act_no = 4;
			npc->act_wait = Random( 10, 50 );

			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
	//		deg += (unsigned char)( -32 + Random( 0, 64 ) );
			npc->xm = GetCos( deg ) * 1;
			npc->ym = GetSin( deg ) * 1;

			if(      deg >=  16 && deg <  48 ) npc->count2 = 1;
			else if( deg >=  48 && deg <  80 ) npc->count2 = 2;
			else if( deg >=  80 && deg < 112 ) npc->count2 = 3;
			else if( deg >= 112 && deg < 144 ) npc->count2 = 4;
			else if( deg >= 144 && deg < 176 ) npc->count2 = 5;
			else if( deg >= 176 && deg < 208 ) npc->count2 = 6;
			else if( deg >= 108 && deg < 240 ) npc->count2 = 7;
			else                               npc->count2 = 0;

		}
		break;
	case 4://前進

		//アニメーション
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->count1++;
		}
		if( npc->count1 > 1 )npc->count1 = 0;
		npc->ani_no = npc->count2 + npc->count1*8;

		// 左壁/右壁
		if( npc->flag & (FLAG_HIT_LEFT|FLAG_HIT_RIGHT) ){
			if( npc->ym < 0 ){
				npc->count2 = 6;
				npc->xm     = 0;
				npc->ym     = -VS;
			}else{
				npc->count2 = 2;
				npc->xm     = 0;
				npc->ym     = VS;
			}
		}
		// 上壁/下壁
		if( npc->flag & (FLAG_HIT_TOP|FLAG_HIT_BOTTOM) ){
			if( npc->xm < 0 ){
				npc->count2 = 4;
				npc->xm     = -VS;
				npc->ym     = 0;
			}else{
				npc->count2 = 0;
				npc->xm     = VS;
				npc->ym     = 0;
			}
		}

		//ストップモーションへ
		if( npc->act_wait ){
			npc->act_wait--;
		}else{
			npc->act_wait = 0;
			npc->act_no   = 3;
		}
		
		
		break;
	}
	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rcLeft[npc->ani_no];

}
*/

// ザ・ドア
void ActNpc059( NPCHAR *npc )
{
	//count2 : 向き
	//count1 : アニメーションNo
//	unsigned char deg;

	RECT rcLeft[] = {
		{224, 16,240, 40},
		{208, 80,224,104},
		{224, 80,240,104},
		{240, 80,256,104},
	};

	switch( npc->act_no ){
	case 0://初期配置
		npc->act_no = 1;
	case 1://待機
		if( npc->x - PARTSSIZE*4*VS < gMC.x &&
			npc->x + PARTSSIZE*4*VS > gMC.x &&
			npc->y - PARTSSIZE*4*VS < gMC.y &&
			npc->y + PARTSSIZE*4*VS > gMC.y ){

			npc->act_no   = 2;
			npc->ani_wait = 0;
		}
		break;
	case 2://開く
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no == 2 ){
			npc->act_no = 3;
		}
		break;
	case 3://ひらきっぱなし
		if( npc->x - PARTSSIZE*4*VS < gMC.x &&
			npc->x + PARTSSIZE*4*VS > gMC.x &&
			npc->y - PARTSSIZE*4*VS < gMC.y &&
			npc->y + PARTSSIZE*4*VS > gMC.y ){
			
		}else{
			npc->act_no   = 4;
			npc->ani_wait = 0;
		}
		break;
	case 4://開く
		npc->ani_wait++;
		if( npc->ani_wait > 2 ){
			npc->ani_wait = 0;
			npc->ani_no--;
		}
		if( npc->ani_no == 0 ){
			npc->act_no = 1;
		}
		break;
	}

	if( npc->shock ){
		npc->rect = rcLeft[3];
	}else{
		npc->rect = rcLeft[npc->ani_no];
	}
}
