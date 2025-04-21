#include <windows.h>
#include "Sound.h"
#include "Draw.h"
#include "Define.h"

#include "Tags.h"

extern MYCHAR gMC;


extern NPCHAR gBoss[];

long Random( int min, int max );
void SetNpChar( long code_char, long x, long y, long xm, long ym,
				long dir, NPCHAR *npc, long start_index );
void SetCaret( long x, long y, long code, long dir );
void SetQuake( long time );
void ChangeAllNpCharByCode( long code1, long code2 );

int GetSin( unsigned char deg );
int GetCos( unsigned char deg );
unsigned char GetArktan( long x,long y );

void SetDestroyNpChar( long x, long y, long w, long num );
void DeleteNpCharCode( long code, BOOL bSmoke );

void SetFlash( long x, long y, long mode );

/*
 0メイン

 1カバー
 2カバー

 3コア
 4コア
 5コア
 6コア
  
 7フェイス
 8フェイス


 9足
10足
11足
12足

13ふともも
14ふともも
15ふともも
16ふともも

*/

//無限軌道
static void ActBossChar03_01( NPCHAR *npc )
{

	RECT rcUp[] = {
		{  0,  0, 72, 32},
		{  0, 32, 72, 64},
		{ 72,  0,144, 32},//左移動
		{144,  0,216, 32},
		{ 72, 32,144, 64},//右移動
		{144, 32,216, 64},
	};
	RECT rcDown[] = {
		{  0, 64, 72, 96},
		{  0, 96, 72,128},
		{ 72, 64,144, 96},//左移動
		{144, 64,216, 96},
		{ 72, 96,144,128},//右移動
		{144, 96,216,128},
	};

	switch( npc->act_no ){
	case  10://停止
		npc->ani_no = 0;
//		npc->damage = 0;
		npc->bits   &= ~BITS_BOUND_MYCHAR;
//		npc->bits   &= ~BITS_DAMAGE_SIDE;
		break;
	case 100://左移動(初速) ----
		npc->bits |= BITS_BOUND_MYCHAR;
//		npc->bits |= BITS_DAMAGE_SIDE;
		npc->act_no   = 101;
		npc->act_wait =   0;
		npc->ani_no   =   2;
		npc->ani_wait =   0; 
//		npc->damage   =  10;
	case 101:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no = 102;
		}
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;

		npc->xm -= VS/16;
		break;
	case 102://左移動
		npc->bits &= ~BITS_BOUND_MYCHAR;
		npc->act_no   = 103;
		npc->ani_no   =   0;
		npc->ani_wait =   0; 
	case 103:
		npc->act_wait++;
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		npc->xm -= VS/16;
		break;

	
	case 200://右移動(初速) ----
		npc->bits |= BITS_BOUND_MYCHAR;
		npc->bits |= BITS_DAMAGE_SIDE;
		npc->act_no   = 201;
		npc->act_wait =   0;
		npc->ani_no   =   4;
		npc->ani_wait =   0; 
//		npc->damage   =  10;
	case 201:
		npc->act_wait++;
		if( npc->act_wait > 30 ){
			npc->act_no = 202;
		}
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 4;
		npc->xm += VS/16;
		break;
	case 202://右移動
		npc->bits &= ~BITS_BOUND_MYCHAR;
		npc->act_no   = 203;
		npc->ani_no   =   0;
		npc->ani_wait =   0; 
	case 203:
		npc->act_wait++;
		npc->ani_wait++;
		if( npc->ani_wait > 1 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 1 )
			npc->ani_no = 0;
		npc->xm += VS/16;
		break;

	case 300:// 左ブレーキ
		npc->act_no   = 301;
		npc->ani_no   =   4;
		npc->ani_wait =   0; 
//		npc->damage   =  10;
		npc->bits |= BITS_BOUND_MYCHAR;
	case 301:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 5 )
			npc->ani_no = 4;

		npc->xm += VS/16;
		if( npc->xm > 0 ){
			npc->xm     = 0;
			npc->act_no = 10;
		}
		break;
	case 400:// 右ブレーキ
		npc->act_no   = 401;
		npc->ani_no   =   2;
		npc->ani_wait =   0; 
//		npc->damage   =  10;
		npc->bits |= BITS_BOUND_MYCHAR;
	case 401:
		npc->ani_wait++;
		if( npc->ani_wait > 0 ){
			npc->ani_wait = 0;
			npc->ani_no++;
		}
		if( npc->ani_no > 3 )
			npc->ani_no = 2;

		npc->xm -= VS/16;
		if( npc->xm < 0 ){
			npc->xm     = 0;
			npc->act_no = 10;
		}
		break;
	}

	if( npc->act_no == 101 || npc->act_no == 201 || npc->act_no == 301 || npc->act_no == 401 ){
		if( npc->act_wait%2 == 1 ) PlaySoundObject( WAVE_CATA2, 1 );
	}
	if( npc->act_no == 103 || npc->act_no ==  203 ){
		if( npc->act_wait%4 == 1 ) PlaySoundObject( WAVE_CATA, 1 );
	}

	if( npc->act_no >= 100 &&
		gMC.y < npc->y + 4*VS &&
		gMC.y > npc->y - 4*VS ){
		npc->damage = 10;
		npc->bits |= BITS_DAMAGE_SIDE;
	}else{
		npc->damage = 0;
		npc->bits &= ~BITS_DAMAGE_SIDE;
	}


	if( npc->xm >  VS*2 ) npc->xm =  VS*2;
	if( npc->xm < -VS*2 ) npc->xm = -VS*2;

	npc->x += npc->xm;


	if( npc->direct == DIR_UP ) npc->rect = rcUp[   npc->ani_no ];
	else                        npc->rect = rcDown[ npc->ani_no ];
}

// もも
static void ActBossChar03_02( NPCHAR *npc )
{
	RECT rect[] = {
		{  0,128, 72,160},
		{ 72,128,144,160},
		{  0,160, 72,192},
		{ 72,160,144,192},
	};
	long direct;
	long x, y;

	switch( npc->act_no ){
	case 0:// 何もしない
		break;
	case 10:
		npc->act_no   = 11;
		npc->act_wait = 30 + npc->ani_no*30;
	case 11:

		
		if( npc->act_wait ) npc->act_wait--;
		else{
			switch( npc->ani_no ){
			case 0: direct = 3; x = -30*VS; y =  6*VS; break;
			case 1: direct = 2; x = +30*VS; y =  6*VS;break;
			case 2: direct = 0; x = -30*VS; y = -6*VS;break;
			case 3: direct = 1; x = +30*VS; y = -6*VS;break;
			}
			SetNpChar( 158, npc->x+x, npc->y+y, 0, 0, direct, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			npc->act_wait = 120;
		}
		break;

	}
	
	
	npc->x = (gBoss[0].x + gBoss[npc->count1].x)/2;
	npc->y = (gBoss[0].y + gBoss[npc->count1].y)/2;

	npc->rect = rect[npc->ani_no];
}



// カバー
static void ActBossChar03_03( NPCHAR *npc )
{
	switch( npc->act_no ){
	case 0:
		break;
	case 10://オープン
		npc->tgt_x += VS;
		if( npc->tgt_x > 32*VS ){
			npc->tgt_x  = 32*VS;
			npc->act_no = 0;

			gBoss[3].act_no = 10;
			gBoss[4].act_no = 10;
			gBoss[5].act_no = 10;
			gBoss[6].act_no = 10;
		}
		break;
	case 20://クローズ
		npc->tgt_x -= VS;
		if( npc->tgt_x < 0 ){
			npc->tgt_x  = 0;
			npc->act_no = 0;

			gBoss[3].act_no = 0;
			gBoss[4].act_no = 0;
			gBoss[5].act_no = 0;
			gBoss[6].act_no = 0;
		}
		break;
	case 30://オープン
		npc->tgt_x += VS;
		if( npc->tgt_x > 20*VS ){
			npc->tgt_x  = 20*VS;
			npc->act_no = 0;

			gBoss[ 7].act_no = 10;
			gBoss[13].act_no = 10;
			gBoss[14].act_no = 10;
			gBoss[15].act_no = 10;
			gBoss[16].act_no = 10;
		}
		break;
	case 40://クローズ
		npc->tgt_x -= VS;
		if( npc->tgt_x < 0 ){
			npc->tgt_x  = 0;
			npc->act_no = 0;

			gBoss[ 7].act_no =  0;
			gBoss[13].act_no =  0;
			gBoss[14].act_no =  0;
			gBoss[15].act_no =  0;
			gBoss[16].act_no =  0;
		}
		break;
	}
	

	RECT rcLeft  = {216, 96,264,144};
	RECT rcRight = {264, 96,312,144};

	if( npc->direct == DIR_LEFT ){
		npc->rect = rcLeft;
		npc->x = gBoss[0].x - 24*VS - npc->tgt_x;
		npc->y = gBoss[0].y;
	}else{
		npc->rect = rcRight;
		npc->x = gBoss[0].x + 24*VS + npc->tgt_x;
		npc->y = gBoss[0].y;
	}
}

// コア
static void ActBossChar03_04( NPCHAR *npc )
{
	long xm, ym;
	unsigned char deg;

	RECT rect[8] = {
		{  0,192, 16,208},
		{ 16,192, 32,208},
		{ 32,192, 48,208},
		{ 48,192, 64,208},

		{  0,208, 16,224},
		{ 16,208, 32,224},
		{ 32,208, 48,224},
		{ 48,208, 64,224},
	};


	switch( npc->act_no ){
	case 0://ノーダメージ
		npc->bits &= ~BITS_BANISH_DAMAGE;
		npc->ani_no = 0;
		break;
	case 10:
		npc->act_no = 11;
		npc->act_wait = 40 + npc->tgt_x*10;
		npc->bits |=  BITS_BANISH_DAMAGE;
	case 11:

		if( npc->act_wait < 16 && npc->act_wait/2%2 ) npc->ani_no = 1;
		else                                          npc->ani_no = 0;

		if( npc->act_wait ) npc->act_wait--;
		else{
			deg = GetArktan( npc->x - gMC.x, npc->y - gMC.y );
			deg += (unsigned char)Random( -2, 2 );

			ym = GetSin( deg ) * 3;
			xm = GetCos( deg ) * 3;
			SetNpChar( 156, npc->x, npc->y, xm, ym, DIR_LEFT, NULL, MAX_NPC/2 );
			PlaySoundObject( WAVE_POP, 1 );
			npc->act_wait = 40;
		}
		break;
	}


	switch( npc->tgt_x ){
	case 0: npc->x = gBoss[0].x -VS*22; npc->y = gBoss[0].y -VS*16; break;
	case 1: npc->x = gBoss[0].x +VS*28; npc->y = gBoss[0].y -VS*16; break;
	case 2: npc->x = gBoss[0].x -VS*15; npc->y = gBoss[0].y +VS*14; break;
	case 3: npc->x = gBoss[0].x +VS*17; npc->y = gBoss[0].y +VS*14; break;
	}

	npc->rect = rect[npc->tgt_x + 4*npc->ani_no];
}

// 顔
static void ActBossChar03_face( NPCHAR *npc )
{
	RECT rect[] = {
		{216,  0,320, 48},
		{216, 48,320, 96},
		{216,144,320,192},
	};
	static unsigned char flash = 0;

	switch( npc->act_no ){
	case 0://ノーダメージ
		gBoss[0].bits &= ~BITS_BANISH_DAMAGE;
		npc->ani_no = 0;
		break;
	case 10://ダメージあり
		npc->act_no = 11;
		npc->act_wait = 40 + npc->tgt_x*10;
		gBoss[0].bits |= BITS_BANISH_DAMAGE;
	case 11:
		if( gBoss[0].shock ){
			if( (flash++)/2%2 ) npc->ani_no = 1;
			else              npc->ani_no = 0;
		}else{
			npc->ani_no = 0;
		}
		break;
	}
	gBoss[7].x = gBoss[0].x;
	gBoss[7].y = gBoss[0].y;
	if( gBoss[0].act_no <= 10 ) npc->ani_no = 2;
	npc->rect = rect[npc->ani_no];

}


void ActBossChar_MonstX( void )
{
	long i;
	NPCHAR *npc;
	
	npc = &gBoss[0];
	switch( npc->act_no ){
	case 0:// 初期設定
		npc->life = 1;
		npc->x           = -20*VS*PARTSSIZE;
		break;
	case 1:// 初期設定
		npc->life   = 700;
		npc->exp    = 1;
		npc->act_no = 1;
		npc->x           = 128*VS*PARTSSIZE;
		npc->y           =  12*VS*PARTSSIZE + 8*VS;
		npc->hit_voice   = WAVE_NPC_GOHST;
		npc->hit.front   = 24*VS;
		npc->hit.top     = 24*VS;
		npc->hit.back    = 24*VS;
		npc->hit.bottom  = 24*VS;
		npc->bits = BITS_THROW_BLOCK|BITS_EVENT_BREAK|BITS_VIEWDAMAGE;
		npc->size = NPCSIZE_LARGE;

		npc->code_event = 1000;
		npc->ani_no = 0;



		// カバー --------------
		gBoss[1].cond = COND_ALIVE;
		gBoss[1].size        = NPCSIZE_LARGE;
		gBoss[1].direct      = DIR_LEFT;
		gBoss[1].view.front  = 24*VS;
		gBoss[1].view.top    = 24*VS;
		gBoss[1].view.back   = 24*VS;
		gBoss[1].view.bottom = 24*VS;
		gBoss[1].bits = BITS_THROW_BLOCK;

		gBoss[2] = gBoss[1];
		gBoss[2].direct = DIR_RIGHT;


		// コア --------------
		gBoss[3].cond = COND_ALIVE;
		gBoss[3].life = 60;
		gBoss[3].size          = NPCSIZE_LARGE;
		gBoss[3].hit_voice     = WAVE_NPC_GOHST;
		gBoss[3].destroy_voice = WAVE_DESTROY_MIDDLE;
		gBoss[3].size          = NPCSIZE_MIDDLE;
		gBoss[3].view.front  = 8*VS;
		gBoss[3].view.top    = 8*VS;
		gBoss[3].view.back   = 8*VS;
		gBoss[3].view.bottom = 8*VS;
		gBoss[3].hit.front   = VS*5;
		gBoss[3].hit.back    = VS*5;
		gBoss[3].hit.top     = VS*5;
		gBoss[3].hit.bottom  = VS*5;
		gBoss[3].bits = BITS_THROW_BLOCK;
		gBoss[3].tgt_x       = 0;//左上

		gBoss[4] = gBoss[3];
		gBoss[4].tgt_x       = 1;//右上
		gBoss[5] = gBoss[3];
		gBoss[5].tgt_x       = 2;//左下
		gBoss[5].life = 100;
		gBoss[6] = gBoss[3];
		gBoss[6].tgt_x       = 3;//右下
		gBoss[6].life = 100;


		// 顔 -----------
		gBoss[7].cond = COND_ALIVE;
		gBoss[7].x           = 128*VS*PARTSSIZE;
		gBoss[7].y           =  12*VS*PARTSSIZE + 8*VS;
		gBoss[7].view.front  = 52*VS;
		gBoss[7].view.top    = 24*VS;
		gBoss[7].view.back   = 52*VS;
		gBoss[7].view.bottom = 24*VS;
		gBoss[7].hit_voice   = WAVE_NPC_LARGE;
		gBoss[7].hit.front   = 8*VS;
		gBoss[7].hit.top     = 24*VS;
		gBoss[7].hit.back    = 8*VS;
		gBoss[7].hit.bottom  = 16*VS;
		gBoss[7].bits = BITS_THROW_BLOCK;
		gBoss[7].size = NPCSIZE_LARGE;

		gBoss[7].ani_no = 0;

		// 無限軌道 ---------------
		gBoss[9].cond   = COND_ALIVE;
		gBoss[9].act_no = 0;
		gBoss[9].direct = DIR_UP;
		gBoss[9].x      = 124*VS*PARTSSIZE;
		gBoss[9].y      =   9*VS*PARTSSIZE;
		gBoss[9].view.front  = 36*VS;
		gBoss[9].view.top    =  8*VS;
		gBoss[9].view.back   = 36*VS;
		gBoss[9].view.bottom = 24*VS;
		gBoss[9].hit_voice = WAVE_NPC_LARGE;
		gBoss[9].hit.front  = 28*VS;
		gBoss[9].hit.top    =  8*VS;
		gBoss[9].hit.back   = 28*VS;
		gBoss[9].hit.bottom = 16*VS;
		gBoss[9].bits = BITS_THROW_BLOCK|BITS_BLOCK_MYCHAR|BITS_BLOCK_BULLET|BITS_DAMAGE_SIDE;
		gBoss[9].size = NPCSIZE_LARGE;

		gBoss[10] = gBoss[9];
		gBoss[10].x = 132*VS*PARTSSIZE;

		gBoss[11] = gBoss[9];
		gBoss[11].direct = DIR_DOWN;
		gBoss[11].x = 124*VS*PARTSSIZE;
		gBoss[11].y =  16*VS*PARTSSIZE;
		gBoss[11].view.top    = 24*VS;
		gBoss[11].view.bottom =  8*VS;
		gBoss[11].hit.top    = 16*VS;
		gBoss[11].hit.bottom =  8*VS;

		gBoss[12] = gBoss[11];
		gBoss[12].x = 132*VS*PARTSSIZE;

		// もも ---------------
		gBoss[13] = gBoss[9];
		gBoss[13].cond = COND_ALIVE;
		gBoss[13].view.top    = 16*VS;
		gBoss[13].view.bottom = 16*VS;
		gBoss[13].view.front  = 30*VS;
		gBoss[13].view.back   = 42*VS;
		gBoss[13].count1 = 9;
		gBoss[13].ani_no = 0;
		gBoss[13].bits   = BITS_THROW_BLOCK;
		
		gBoss[14] = gBoss[13];
		gBoss[14].view.front  = 42*VS;
		gBoss[14].view.back   = 30*VS;
		gBoss[14].count1 = 10;
		gBoss[14].ani_no = 1;
		gBoss[14].bits   = BITS_THROW_BLOCK;

		gBoss[15] = gBoss[13];
		gBoss[15].view.top    = 16*VS;
		gBoss[15].view.bottom = 16*VS;
		gBoss[15].count1 = 11;
		gBoss[15].ani_no = 2;
		gBoss[15].bits   = BITS_THROW_BLOCK;

		gBoss[16] = gBoss[15];
		gBoss[16].view.front  = 42*VS;
		gBoss[16].view.back   = 30*VS;
		gBoss[16].count1 = 12;
		gBoss[16].ani_no = 3;
		gBoss[16].bits   = BITS_THROW_BLOCK;

		// シールド


		npc->act_no = 2;
	case 2:// 待機
//		npc->act_no = 10;
		break;

	case 10://作動！
		npc->act_no   = 11;
		npc->act_wait =  0;
		npc->count1   =  0;//切り返しを数える。
	case 11:
		npc->act_wait++;
		if( npc->act_wait > 50*2 ){
			npc->act_wait = 0;
			if( npc->x > gMC.x ) npc->act_no = 100;
			else                     npc->act_no = 200;
		}
		
		break;
	case 100:// 左移動
		npc->act_wait = 0;
		npc->act_no   = 101;
		npc->count1++;
	case 101:
		npc->act_wait++;
		if( npc->act_wait ==  4 ) gBoss[ 9].act_no = 100;
		if( npc->act_wait ==  8 ) gBoss[10].act_no = 100;
		if( npc->act_wait == 10 ) gBoss[11].act_no = 100;
		if( npc->act_wait == 12 ) gBoss[12].act_no = 100;

		if( npc->act_wait > 120 && npc->count1 > 2 )npc->act_no = 300;
		if( npc->act_wait > 121 && gMC.x > npc->x  )npc->act_no = 200;
		break;
	case 200:// 右移動
		npc->act_wait = 0;
		npc->act_no   = 201;
		npc->count1++;
	case 201:
		npc->act_wait++;
		if( npc->act_wait ==  4 ) gBoss[ 9].act_no = 200;
		if( npc->act_wait ==  8 ) gBoss[10].act_no = 200;
		if( npc->act_wait == 10 ) gBoss[11].act_no = 200;
		if( npc->act_wait == 12 ) gBoss[12].act_no = 200;

		if( npc->act_wait > 120 && npc->count1 > 2 )npc->act_no = 400;
		if( npc->act_wait > 121 && gMC.x < npc->x  )npc->act_no = 100;
		break;

	case 300:// 左ブレーキ
		npc->act_wait = 0;
		npc->act_no   = 301;
	case 301:
		npc->act_wait++;
		if( npc->act_wait ==  4 ) gBoss[ 9].act_no = 300;
		if( npc->act_wait ==  8 ) gBoss[10].act_no = 300;
		if( npc->act_wait == 10 ) gBoss[11].act_no = 300;
		if( npc->act_wait == 12 ) gBoss[12].act_no = 300;

		if( npc->act_wait > 50 ){
			//コアぜんぶやられ
			if( !gBoss[3].cond &&
				!gBoss[4].cond &&
				!gBoss[5].cond &&
				!gBoss[6].cond ){

				npc->act_no = 600;
			}else{
				npc->act_no = 500;
			}
		}
		break;
	case 400:// 右ブレーキ
		npc->act_wait = 0;
		npc->act_no   = 401;
	case 401:
		npc->act_wait++;
		if( npc->act_wait ==  4 ) gBoss[ 9].act_no = 400;
		if( npc->act_wait ==  8 ) gBoss[10].act_no = 400;
		if( npc->act_wait == 10 ) gBoss[11].act_no = 400;
		if( npc->act_wait == 12 ) gBoss[12].act_no = 400;

		if( npc->act_wait > 50 ){
			//コアぜんぶやられ
			if( !gBoss[3].cond &&
				!gBoss[4].cond &&
				!gBoss[5].cond &&
				!gBoss[6].cond ){

				npc->act_no = 600;
			}else{
				npc->act_no = 500;
			}
		}
		break;

	case 500://停止 オープン --------------
		npc->act_no   = 501;
		npc->act_wait =  0;
		gBoss[ 1].act_no = 10;
		gBoss[ 2].act_no = 10;
	case 501:
		npc->act_wait++;

		if( npc->act_wait > 300 ){
			npc->act_no = 502;
			npc->act_wait = 0;
		}
		//コアぜんぶやられ
		if( !gBoss[3].cond &&
			!gBoss[4].cond &&
			!gBoss[5].cond &&
			!gBoss[6].cond ){

			npc->act_no = 502;
			npc->act_wait = 0;
		}
		break;
	case 502://停止 クローズ
		npc->act_no   = 503;
		npc->act_wait =  0;
		npc->count1   =  0;//切り返しを数える。
		gBoss[ 1].act_no = 20;
		gBoss[ 2].act_no = 20;
	case 503:
		npc->act_wait++;

		if( npc->act_wait > 50 ){
			if( npc->x > gMC.x ) npc->act_no = 100;
			else                     npc->act_no = 200;
		}
		break;

	
	case 600://停止 オープン (ミサイル攻撃)
		npc->act_no   = 601;
		npc->act_wait =  0;
		npc->count2   = npc->life;
		gBoss[ 1].act_no  = 30;
		gBoss[ 2].act_no  = 30;
	case 601:
		npc->act_wait++;

		if( npc->life < npc->count2 - 200 || npc->act_wait > 300 ){
			npc->act_no = 602;
			npc->act_wait = 0;
		}
		break;
	case 602://停止 クローズ
		npc->act_no   = 603;
		npc->act_wait =  0;
		npc->count1   =  0;//切り返しを数える。
		gBoss[ 1].act_no = 40;
		gBoss[ 2].act_no = 40;
	case 603:
		npc->act_wait++;

		if( npc->act_wait > 50 ){
			if( npc->x > gMC.x ) npc->act_no = 100;
			else                     npc->act_no = 200;
		}
		break;

	// やられ-------
	case 1000:
		SetQuake( 2 );
		npc->act_wait++;
		if( npc->act_wait%8 == 0 )
			PlaySoundObject( WAVE_NPC_LARGE, 1 );
			SetDestroyNpChar( npc->x + Random(-72,72)*VS,
							  npc->y + Random(-64,64)*VS, 1,  1 );
		if( npc->act_wait > 50*2 ){
			npc->act_wait =  0;
			npc->act_no   = 1001;
			SetFlash( npc->x, npc->y, FLASHMODE_EXPLOSION );
			PlaySoundObject( WAVE_EXPLOSION, 1 );
		}
		break;
	case 1001:
		SetQuake( 40 );
		npc->act_wait++;
		if( npc->act_wait > 50 ){
			for( i = 0; i < MAX_BOSS_PARTS; i++ ) gBoss[i].cond = 0;
			DeleteNpCharCode( 158, TRUE );
			SetNpChar( 159, npc->x, npc->y-VS*24, 0, 0, DIR_LEFT, NULL, 0 );
		}
		break;
	}


	// 無限軌道
	ActBossChar03_01( &gBoss[ 9] );
	ActBossChar03_01( &gBoss[10] );
	ActBossChar03_01( &gBoss[11] );
	ActBossChar03_01( &gBoss[12] );


	npc->x += ( ( gBoss[9].x + gBoss[10].x + gBoss[11].x + gBoss[12].x )/4 - npc->x )/16; 
	ActBossChar03_face( &gBoss[7] );

	// 太もも
	ActBossChar03_02( &gBoss[13] );
	ActBossChar03_02( &gBoss[14] );
	ActBossChar03_02( &gBoss[15] );
	ActBossChar03_02( &gBoss[16] );

	// カバー
	ActBossChar03_03( &gBoss[1] );
	ActBossChar03_03( &gBoss[2] );

	// コア
	if( gBoss[3].cond ) ActBossChar03_04( &gBoss[3] );
	if( gBoss[4].cond ) ActBossChar03_04( &gBoss[4] );
	if( gBoss[5].cond ) ActBossChar03_04( &gBoss[5] );
	if( gBoss[6].cond ) ActBossChar03_04( &gBoss[6] );


	// やられ
	if( !npc->life && npc->act_no < 1000 ){
		npc->act_no = 1000;
		npc->act_wait = 0;
		npc->shock   = 50*3;
		gBoss[ 9].act_no = 300;
		gBoss[10].act_no = 300;
		gBoss[11].act_no = 300;
		gBoss[12].act_no = 300;
		
	}


}